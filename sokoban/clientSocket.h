#pragma once
#include"stdafx.h"
#include"map.h"

const int PORT = 9000;
const char HOST[] = "127.0.0.1";
int clientConnectIni(SOCKET *sServer);
int clientSend(SOCKET sServer, char *buffer, int size);
int clientCloseConnect(SOCKET sServer);
int clientRecv(SOCKET sServer, char*buffer, int size);

