#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<fstream>
#include <WS2tcpip.h>
#include<ctime>
#include<vector>
#include"dao.h"
#pragma comment(lib,"ws2_32.lib")
const int PORT = 9000;//端口号
const std::string mapdir = "./maps/";//地图存储文件夹

int mapname = 1;//全局变量控制传入的地图序号

CRITICAL_SECTION mapnamectrl;//临界区对mapname的操作互斥
// 每调用一次向缓冲区中送入下1024个数据
class MapStream
{
	std::vector<MapInfo> maps;
	//当前传输到哪一个地图了
	std::vector<MapInfo>::iterator MapIterator;
	//当前传输到地图的哪一个位置了
	std::ifstream fin;

	bool first;
public:
	MapStream()
	{


	}
	void openMaps(std::vector<MapInfo> map)
	{
		first = true;
		maps = map;
		MapIterator = maps.begin();
	}
	void operator >> (char buf[])
	{
		if (first)
		{
			first = false;
			fin.open(MapIterator->mappath, std::ios::in);

		}
		buf[0] = 'd';
		fin.read(buf + 1, 1023);

		if (fin.eof())
		{
			int readCount = fin.gcount();
			buf[readCount++] = ';';
			fin.close();
			MapIterator++;
			fin.open(MapIterator->mappath);
			fin.read(buf + readCount, 1024 - readCount);
		}
	}


};
//客户端处理线程
DWORD WINAPI clientProc(LPARAM lparam)
{
	Dao d;
	d.ConnectInit();
	char userName[21];//用户名
	char password[30];//密码
	char mapdata[4096];//地图数据
	char soldata[1024];//解数据
	char mappath[20];//地图存储路径
	char solpath[20];
	int mapin = 0;//网络数据下标
	int mapout = 0;//地图与解存储数据下标
	bool isprivate = false;
	bool ismap = true;
	MapStream ms;
	//传递过来的客户端连接
	SOCKET sockClient = (SOCKET)lparam;
	//缓冲区初始化
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	//标志位防止重复发送
	bool isSent = true;//初始设置为没有数据发送

	while (true) {
		// 接收客户端的一条数据（非阻塞）   
		int ret = recv(sockClient, buf, sizeof(buf), 0);//若无数据立即返回;
		//检查是否接收失败  
		if (SOCKET_ERROR == ret)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{

				printf("socket recv failederror:%d\n", WSAGetLastError());
				closesocket(sockClient);
				d.Disconnect();
				return -1;
			}
		}
		else
		{
			switch (buf[0])
			{
			case 'l'://登陆请求
			{
				strcpy_s(userName, 21, buf + 1);
				strcpy_s(password, 30, buf + 21);
				memset(buf, 0, 1024);

				if (d.Login(userName, password))
				{
					strcpy_s(buf, 1024,"lt");
				}
				else
				{
					memset(userName, 0, 21);
					memset(password, 0, 30);
					strcpy_s(buf,1024, "lfusername or password is incorrect");
				}
				isSent = false;
				break;
			}
			case 'r'://注册请求
			{
				strcpy_s(userName, 21, buf + 1);
				strcpy_s(password, 30, buf + 21);
				memset(buf, 0, 1024);

				time_t t;
				t = time(0);
				tm time;
				localtime_s(&time ,&t);
				SQL_DATE_STRUCT date;
				date.year = time.tm_year + 1900;
				date.month = time.tm_mon + 1;
				date.day = time.tm_mday;
				if (d.Register(userName, password, date, false))
				{
					strcpy_s(buf, "rt");
				}
				else
				{
					memset(userName, 0, 21);
					memset(password, 0, 30);
					//注册失败注销登陆
					strcpy_s(buf, "rfexisted user name");
				}
				isSent = false;
				break;
			}
			case 'u'://上传请求
			{
				if (buf[1] == 't')
					isprivate = true;
				if (ismap) {
					mapin = 2;
					//读取地图数据
					while (mapin < 1024 && buf[mapin] != 0 && buf[mapin] != ';')
					{
						buf[mapin++] = mapdata[mapout++];
						if (mapout > 4096)
						{
							memset(buf, 0, 1024);
							strcpy_s(buf, "uftoo large map");
							isSent = false;
							break;
						}
					}
					if (mapin == 1024)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "v");
						isSent = false;
						break;//等待数据
					}
					if (buf[mapin] == 0)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "ufmap data format incorrect");
						isSent = false;
						break;
					}
					mapin++;
					//读取解数据
					mapout = 0;

				}
				while (mapin != 1024 && buf[mapin] != 0)
				{

					buf[mapin++] = mapdata[mapout++];
					if (mapout > 4096)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "uftoo large map");
						isSent = false;
						break;
					}
				}
				if (mapin == 1024)
				{
					memset(buf, 0, 1024);
					strcpy_s(buf, "v");
					isSent = false;
					break;//等待数据
				}
				if (buf[mapin] == 0) //数据读取完成
				{
					ismap = true;
					std::ofstream fout;

					sprintf_s(mappath, (mapdir + "%d.xsb").c_str(), mapname);
					fout.open(mappath, std::ios::out);
					fout << mapdata;
					fout.flush();
					fout.close();


					EnterCriticalSection(&mapnamectrl);
					sprintf_s(solpath, (mapdir + "%d.lurd").c_str(), mapname++);
					LeaveCriticalSection(&mapnamectrl);
					fout.open(solpath, std::ios::out);
					fout << soldata;
					fout.flush();
					fout.close();


					d.upload(mappath, solpath, true, userName);

					memset(mappath, 0, sizeof mappath);
					memset(buf, 0, 1024);
					strcpy_s(buf, "ut");
					isSent = false;
				}
				break;
			}
			case 't'://额外数据块
			{
				//额外数据是地图还是解
				if (ismap) {
					mapin = 1;
					//读取地图数据
					while (mapin < 1024 && buf[mapin] != 0 && buf[mapin] != ';')
					{
						buf[mapin++] = mapdata[mapout++];
						if (mapout > 4096)
						{
							memset(buf, 0, 1024);
							strcpy_s(buf, "uftoo large map");
							isSent = false;
							break;
						}
					}
					if (mapin == 1024)
						break;//等待数据
					if (buf[mapin] == 0)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "ufmap data format incorrect");
						isSent = false;
						break;
					}
					mapin++;
					//读取解数据
					mapout = 0;

				}
				while (mapin != 1024 && buf[mapin] != 0)
				{

					buf[mapin++] = mapdata[mapout++];
					if (mapout > 4096)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "uftoo large map");
						isSent = false;
						break;
					}
				}
				if (mapin == 1024)
					break;//等待数据
				if (buf[mapin] == 0) //数据读取完成
				{
					ismap = true;
					std::ofstream fout;

					sprintf_s(mappath, (mapdir + "%d.xsb").c_str(), mapname);
					fout.open(mappath, std::ios::out);
					fout << mapdata;
					fout.flush();
					fout.close();


					EnterCriticalSection(&mapnamectrl);
					sprintf_s(solpath, (mapdir + "%d.lurd").c_str(), mapname++);
					LeaveCriticalSection(&mapnamectrl);
					fout.open(solpath, std::ios::out);
					fout << soldata;
					fout.flush();
					fout.close();


					d.upload(mappath, solpath, true, userName);

					memset(mappath, 0, sizeof mappath);
					memset(buf, 0, 1024);
					strcpy_s(buf, "ut");
					isSent = false;
				}
				break;
			}
			case 'd'://下载地图请求
			{
				std::vector<MapInfo> maps;
				d.getMaps(userName, maps);
				ms.openMaps(maps);
				ms >> buf;
				isSent = false;
				break;


			}
			case 'v'://反复发送
			{
				ms >> buf;
				isSent = false;
				break;
			}
			}
		}
		// 0 代表客户端主动断开连接  
		if (ret == 0)
		{
			printf("client close connection\n");
			closesocket(sockClient);
			d.Disconnect();
			return -1;
		}
		//请求处理代码


		// 发送数据  
		if (!isSent) {//防止重复发送
			ret = send(sockClient, buf, strlen(buf), 0);
			isSent = true;
			//检查是否发送失败  
			if (SOCKET_ERROR == ret)
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{

					printf("socket send failed error:%d\n", WSAGetLastError());
					closesocket(sockClient);
					d.Disconnect();
					return -1;
				}
			memset(buf, 0, 1024);
		}
		Sleep(500);
	}
	d.Disconnect();
	closesocket(sockClient);
	return 0;
}
int ServerSocket(int Port)
{	//初始化
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		std::cout << "WSAStartup failed\n";
		return -1;
	}
	//创建一个tcpsocket
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		return -1;
	}
	unsigned long ul = 1;
	int ret = ioctlsocket(sServer, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。  

	if (ret == SOCKET_ERROR)//设置失败。  

	{
		printf("socket failed\n");
		return -1;
	}
	//绑定
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;//所有网卡的所有ip
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(Port);
	ret = bind(sServer, (sockaddr*)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		printf("socket bind failed\n");
		WSACleanup(); // 释放网络环境  
		closesocket(sServer); // 关闭网络连接  
		return -1;
	}

	//监听
	ret = listen(sServer, 10);

	if (SOCKET_ERROR == ret)
	{
		printf("socket bind failed\n");
		WSACleanup(); // 释放网络环境  
		closesocket(sServer); // 关闭网络连接  
		return -1;
	}


	SOCKADDR_IN addrClient; // 用于保存客户端的网络节点的信息  
	int addrClientLen = sizeof(SOCKADDR_IN);
	while (TRUE)
	{
		Sleep(10);
		//新建一个socket，用于客户端  
		SOCKET *sClient = new SOCKET;
		//等待客户端的连接  
		*sClient = accept(sServer, (sockaddr*)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == *sClient)
		{
			int ret = WSAGetLastError();
			if (ret == WSAEWOULDBLOCK)
				continue;
			printf("socket accept failed\n");
			WSACleanup();
			closesocket(sServer);
			delete sClient;
			return -1;
		}
		char buf[1024];
		inet_ntop(AF_INET, &addrClient.sin_addr.S_un.S_addr, buf, 1024);
		std::cout << "client:" << buf << "connected" << std::endl;
		//创建线程为客户端做数据收发  
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)clientProc, (LPVOID)*sClient, 0, 0);
	}
	closesocket(sServer);
	WSACleanup();
	return 0;
}
int main()
{
	ServerSocket(PORT);
	std::cin.get();
}