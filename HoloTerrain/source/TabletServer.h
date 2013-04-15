#ifndef TABLETSERVER_H
#define TABLETSERVER_H

#include <process.h> 
#include <Vector>
#include <sstream>
#include <iostream>
#include <winsock.h>


//#define DEFAULT_BUFLEN 512
#pragma comment (lib, "Ws2_32.lib")

class TabletServer{
public:
	int StartSocket();
	void ReadSocket();
	std::string parseData();
	static  unsigned int __stdcall ListenSocketWrapper(void*o);
	void sendData(const char* str);

private:

	SOCKET ListenSocket;
    SOCKET ClientSocket;

	//SOCKET s;
	//SOCKET AcceptSocket;
	WSADATA wsadata;
	char buffer[80];
	//char bufferOut[80];
	const char*  bufferOut;
	int iResult ;
	 int iSendResult;

	CRITICAL_SECTION m_cs;
	//char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen;
};

#endif