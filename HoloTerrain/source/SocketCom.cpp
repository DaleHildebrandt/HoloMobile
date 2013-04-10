#include "SocketCom.h"
#include "CustomStereoNoTracking.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "OpenGlScene.h"
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
using namespace std;

#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "60010"


SocketCom::SocketCom(void)
{
}


SocketCom::~SocketCom(void)
{
}

void SocketCom::Listen(void)
{
	WSADATA wsaData;
    int iResult, test;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0 ) {
        printf("WSAStartup failed with error: %d\n", iResult);
        //return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        //return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }

    // Receive until the peer shuts down the connection
    do {

		    // Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			//closesocket(ListenSocket);
			WSACleanup();
			//return 1;
		}

		cout << "Client accepted\n";
        iResult = recv(ClientSocket, recvbuf, recvbuflen-1, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
			setMessageRecv();

			OpenGlScene::setCurrTexture(recvbuf[0] - '0');
			recvbuf[iResult] = '\0';
			printf("MSG!!!!!:\n\n");
			cout << recvbuf << endl;
			printf("MSG: %s\n", recvbuf);
			/*for(int i = 0; i < iResult; i++){
				printf("%c", recvbuf[iResult]);
			}
			printf("MSG: %s\n", recvbuf);*/
			stringstream ss;
			std::string str(recvbuf);
			std::string oput;
			ss << "Message from Android: " << str << std::endl;
			oput = ss.str();
			OutputDebugString(oput.c_str());


        // Echo the buffer back to the sender
            /*iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);*/
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            //return 1;
        }

    } while (iResult > 0);

    // No longer need server socket
    closesocket(ListenSocket);


    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        //return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
	cin >> test;
    //return 0;
}