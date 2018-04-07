#include"clientSocket.h"
//SOCKET sServer;
int clientConnectIni(SOCKET *sServer) 
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
	*sServer= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		return -1;
	}
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(PORT);
	//客户端只需要连接指定的服务器地址，127.0.0.1是本机的回环地址  
	inet_pton(AF_INET, HOST, &addrServ.sin_addr.S_un.S_addr);
	int ret = connect(*sServer, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));//开始连接  
	if (SOCKET_ERROR == ret)
	{
		printf("socket connect failed\n");
		WSACleanup();
		closesocket(*sServer);
		return -1;
	}
	return 0;

}
int clientSend(SOCKET sServer,char *buffer,int size)
{
	int ret;
	ret = send(sServer, buffer, strlen(buffer), 0);
	if (SOCKET_ERROR == ret)
	{
		printf("socket send failed\n");
		closesocket(sServer);
		return -1;
	}
	std::cout << "send:" << buffer << std::endl;
	
	return 0;
	
}


int clientCloseConnect(SOCKET sServer)
{
	//关闭连接  
	closesocket(sServer);
	WSACleanup();
	return 0;
}
int clientRecv(SOCKET sServer,char*buffer,int size)
{
	

	memset(buffer, 0, size);
	// 服务器Bind 客户端是进行连接  
	int ret = recv(sServer, buffer, size, 0);
	if (SOCKET_ERROR == ret)
	{
		printf("socket recv failed\n");
		closesocket(sServer);
		return -1;
	}
	std::cout<<"recv:"<<buffer<<std::endl;
	return 0;

}
