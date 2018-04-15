#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<fstream>
#include <WS2tcpip.h>
#include<ctime>
#include<vector>
#include"dao.h"
#pragma comment(lib,"ws2_32.lib")
const int PORT = 9000;//�˿ں�
const std::string mapdir = "./maps/";//��ͼ�洢�ļ���

int mapname = 1;//ȫ�ֱ������ƴ���ĵ�ͼ���

CRITICAL_SECTION mapnamectrl;//�ٽ�����mapname�Ĳ�������
// ÿ����һ���򻺳�����������1024������
class MapStream
{
	std::vector<MapInfo> maps;
	//��ǰ���䵽��һ����ͼ��
	std::vector<MapInfo>::iterator MapIterator;
	//��ǰ���䵽��ͼ����һ��λ����
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
//�ͻ��˴����߳�
DWORD WINAPI clientProc(LPARAM lparam)
{
	Dao d;
	d.ConnectInit();
	char userName[21];//�û���
	char password[30];//����
	char mapdata[4096];//��ͼ����
	char soldata[1024];//������
	char mappath[20];//��ͼ�洢·��
	char solpath[20];
	int mapin = 0;//���������±�
	int mapout = 0;//��ͼ���洢�����±�
	bool isprivate = false;
	bool ismap = true;
	MapStream ms;
	//���ݹ����Ŀͻ�������
	SOCKET sockClient = (SOCKET)lparam;
	//��������ʼ��
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	//��־λ��ֹ�ظ�����
	bool isSent = true;//��ʼ����Ϊû�����ݷ���

	while (true) {
		// ���տͻ��˵�һ�����ݣ���������   
		int ret = recv(sockClient, buf, sizeof(buf), 0);//����������������;
		//����Ƿ����ʧ��  
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
			case 'l'://��½����
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
			case 'r'://ע������
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
					//ע��ʧ��ע����½
					strcpy_s(buf, "rfexisted user name");
				}
				isSent = false;
				break;
			}
			case 'u'://�ϴ�����
			{
				if (buf[1] == 't')
					isprivate = true;
				if (ismap) {
					mapin = 2;
					//��ȡ��ͼ����
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
						break;//�ȴ�����
					}
					if (buf[mapin] == 0)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "ufmap data format incorrect");
						isSent = false;
						break;
					}
					mapin++;
					//��ȡ������
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
					break;//�ȴ�����
				}
				if (buf[mapin] == 0) //���ݶ�ȡ���
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
			case 't'://�������ݿ�
			{
				//���������ǵ�ͼ���ǽ�
				if (ismap) {
					mapin = 1;
					//��ȡ��ͼ����
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
						break;//�ȴ�����
					if (buf[mapin] == 0)
					{
						memset(buf, 0, 1024);
						strcpy_s(buf, "ufmap data format incorrect");
						isSent = false;
						break;
					}
					mapin++;
					//��ȡ������
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
					break;//�ȴ�����
				if (buf[mapin] == 0) //���ݶ�ȡ���
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
			case 'd'://���ص�ͼ����
			{
				std::vector<MapInfo> maps;
				d.getMaps(userName, maps);
				ms.openMaps(maps);
				ms >> buf;
				isSent = false;
				break;


			}
			case 'v'://��������
			{
				ms >> buf;
				isSent = false;
				break;
			}
			}
		}
		// 0 ����ͻ��������Ͽ�����  
		if (ret == 0)
		{
			printf("client close connection\n");
			closesocket(sockClient);
			d.Disconnect();
			return -1;
		}
		//���������


		// ��������  
		if (!isSent) {//��ֹ�ظ�����
			ret = send(sockClient, buf, strlen(buf), 0);
			isSent = true;
			//����Ƿ���ʧ��  
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
{	//��ʼ��
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		std::cout << "WSAStartup failed\n";
		return -1;
	}
	//����һ��tcpsocket
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		return -1;
	}
	unsigned long ul = 1;
	int ret = ioctlsocket(sServer, FIONBIO, (unsigned long *)&ul);//���óɷ�����ģʽ��  

	if (ret == SOCKET_ERROR)//����ʧ�ܡ�  

	{
		printf("socket failed\n");
		return -1;
	}
	//��
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;//��������������ip
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(Port);
	ret = bind(sServer, (sockaddr*)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		printf("socket bind failed\n");
		WSACleanup(); // �ͷ����绷��  
		closesocket(sServer); // �ر���������  
		return -1;
	}

	//����
	ret = listen(sServer, 10);

	if (SOCKET_ERROR == ret)
	{
		printf("socket bind failed\n");
		WSACleanup(); // �ͷ����绷��  
		closesocket(sServer); // �ر���������  
		return -1;
	}


	SOCKADDR_IN addrClient; // ���ڱ���ͻ��˵�����ڵ����Ϣ  
	int addrClientLen = sizeof(SOCKADDR_IN);
	while (TRUE)
	{
		Sleep(10);
		//�½�һ��socket�����ڿͻ���  
		SOCKET *sClient = new SOCKET;
		//�ȴ��ͻ��˵�����  
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
		//�����߳�Ϊ�ͻ����������շ�  
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