#include"stdafx.h"
#include "dao.h"

#ifndef TEST


#pragma comment(lib,"ws2_32.lib")
const int PORT = 9000;//�˿ں�
const std::string mapdir = "./maps/";//��ͼ�洢�ļ���

int mapname = 1;//ȫ�ֱ������ƴ���ĵ�ͼ���

CRITICAL_SECTION mapnamectrl;//�ٽ�����mapname�Ĳ�������
// ÿ����һ���򻺳�����������1024������
class MapStream
{
	//��ǰ���䵽��ͼ����һ��λ����
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
			case 'r'://ע������
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
					//ע��ʧ��ע����½
					strcpy_s(buf, "rfexisted user name");
				}
				isSent = false;
				break;
			}
			case 'u'://�ϴ�����
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
					//���ݴ������
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
			case 't'://�������ݿ�
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
					//���ݴ������
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

			case 'd'://���ص�ͼ����
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
			case 'v'://��������
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
	InitializeCriticalSection(&mapnamectrl);
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
	DeleteCriticalSection(&mapnamectrl);
	return 0;
}
int main()
{
	ServerSocket(PORT);
	std::cin.get();
}
#endif // TEST
