#include"stdafx.h"
#include "dao.h"

#ifndef TEST


#pragma comment(lib,"ws2_32.lib")
const int PORT = 9000;//端口号
const std::string mapdir = "./maps/";//地图存储文件夹

int mapname = 1;//全局变量控制传入的地图序号

CRITICAL_SECTION mapnamectrl;//临界区对mapname的操作互斥
// 每调用一次向缓冲区中送入下1024个数据
class MapStream
{
	//当前传输到地图的哪一个位置了
	std::ifstream fin;
	std::string mappath;
	bool first;
public:
	MapStream()
	{


	}
	void openMaps(std::string mappath)
	{
		first = true;
		fin.open(mappath);
		this->mappath = mappath;
	}


	void operator >> (char buf[])
	{
		if (first)
		{
			first = false;
			buf[0] = 'd';
			std::string s = mappath;
			s.erase(s.end() - 5, s.end());
			s.erase(s.begin(), s.begin() + 7);
			buf[1] = (int)std::stoi(s);
			fin.read(buf + 2, 1022);
		}
		else 
		{
			buf[0] = 't';
			fin.read(buf + 1, 1023);
		}
	
		if (fin.eof()) 
		{
			fin.close();
			fin.clear();
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
					strcpy_s(buf, 1024, "lt");
				}
				else
				{
					memset(userName, 0, 21);
					memset(password, 0, 30);
					strcpy_s(buf, 1024, "lfusername or password is incorrect");
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
				localtime_s(&time, &t);
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
				if (buf[1] == 't')
					isprivate = true;
				else
					isprivate = false;
				memset(mapdata, 0, 4096);
				mapin = 2, mapout = 0;
				while (mapin < 1024 && buf[mapin] != 0)
				{
					mapdata[mapout++] = buf[mapin++];
					if (mapout >= 4096)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "uftoo large map");
						isSent = false;
						break;
					}
				}
				if (buf[mapin] == 0)
				{
					//数据传输完成
					std::ofstream fout;

					EnterCriticalSection(&mapnamectrl);
					sprintf_s(mappath, (mapdir + "%d.xsbs").c_str(), mapname++);
					LeaveCriticalSection(&mapnamectrl);

					fout.open(mappath);
					fout << mapdata;
					fout.flush();
					fout.close();
					char solpath[20] = { 0 };
					d.upload(mappath, solpath, isprivate, userName);

					memset(mappath, 0, sizeof mappath);
					memset(buf, 0, 1024);
					strcpy_s(buf, "ut");
					isSent = false;
				}
				else
				{
					memset(buf, 0, 1024);
					strcpy_s(buf, "v");
					isSent = false;
				}
				break;
			case 't'://额外数据块
			{
				mapin = 1;
				while (mapin < 1024 && buf[mapin] != 0)
				{
					mapdata[mapout++] = buf[mapin++];
					if (mapout >= 4096)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "uftoo large map");
						isSent = false;
						break;
					}
				}
				if (buf[mapin] == 0)
				{
					//数据传输完成
					std::ofstream fout;

					EnterCriticalSection(&mapnamectrl);
					sprintf_s(mappath, (mapdir + "%d.xsbs").c_str(), mapname++);
					LeaveCriticalSection(&mapnamectrl);

					fout.open(mappath);
					fout << mapdata;
					fout.flush();
					fout.close();
					char solpath[20] = { 0 };
					d.upload(mappath, solpath, isprivate, userName);

					memset(mappath, 0, sizeof mappath);
					memset(buf, 0, 1024);
					strcpy_s(buf, "ut");
					isSent = false;
				}
				else
				{
					memset(buf, 0, 1024);
					strcpy_s(buf, "v");
					isSent = false;
				}
				break;
			}

			case 'd'://下载地图请求
			{
				std::string path;
				std::ostringstream strstream;
				strstream << ".\\maps\\"<<(int)buf[1]<<".xsbs";
				path = strstream.str();
				ms.openMaps(path);
				ms >> buf;
				isSent = false;
				break;


			}
			case 'v'://反复请求
			{
				ms >> buf;
				isSent = false;
				break;
			}


			case 'm':
			


				std::vector<MapInfo> maps;
				d.getMaps(userName, maps);
				memset(buf, 0, 1024);
				buf[0] = 'm';
				int i = 1;
				for (auto it = maps.begin(); it != maps.end(); ++it)
				{
					std::string s = it->mappath;
					s.erase(s.end() - 5,s.end());
					s.erase(s.begin(), s.begin() + 7);
					buf[i++] = std::stoi(s);
				}
				isSent = false;
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
	InitializeCriticalSection(&mapnamectrl);
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
	DeleteCriticalSection(&mapnamectrl);
	return 0;
}
int main()
{
	ServerSocket(PORT);
	std::cin.get();
}
#endif // TEST
