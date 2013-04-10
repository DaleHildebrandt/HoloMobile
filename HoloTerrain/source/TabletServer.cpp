#include "TabletServer.h"

#define MY_MESSAGE_NOTIFICATION      1048 //Custom notification message
#define DEFAULT_PORT "2000"

using namespace std;

//LISTENONPORT – Listens on a specified port for incoming connections 
//or data
int TabletServer::StartSocket()
{

	int PortNo = 60010; 
//    char* IPAddress = "140.193.216.222"; // ip_adress of tablet samsung
	char* IPAddress = "130.179.30.127"; // ip_adress of lg device
	//ostringstream lines;
	//lines << "Line 1" << endl;
	bufferOut = "initialisation \n";
	//bufferOut = lines.str();

	//Initilize the critical section
	InitializeCriticalSection(&m_cs);

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	//recvbuflen = DEFAULT_BUFLEN;

	int error = WSAStartup (0x0202, &wsadata);   // Fill in WSA info

	if (error)
	{
		return false; //For some reason we couldn't start Winsock
	}

	if (wsadata.wVersion != 0x0202) //Wrong Winsock version?
	{
		WSACleanup ();
		return false;
	}

	SOCKADDR_IN addr; // The address structure for a TCP socket

	addr.sin_family = AF_INET;      // Address family
	addr.sin_port = htons (PortNo);   // Assign port to this socket
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	//addr.sin_addr.s_addr = inet_addr(IPAddress); 

	std::cout << "Creating socket... " << std::endl;
	ListenSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "Invalid socket " << std::endl;
		return false; //Don't continue if we couldn't create a //socket!!
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, (LPSOCKADDR)&addr, sizeof(addr));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		//freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for accepting incoming requests.

	wprintf(L"Waiting for client to connect...\n");

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}else
		wprintf(L"Client connected.\n");


	// No longer need server socket
	closesocket(ListenSocket);

	//Don't forget to clean up with CloseConnection()!
	std::cout << "Listening finished" << std::endl;

}

/*
** Method for listening to a socket.
** Has to be called throught a Thread !!!!
*/
void TabletServer::ReadSocket()
{

	while(true){

		// Lock the Critical section
		//EnterCriticalSection(&m_cs);

		memset(buffer, 0, sizeof(buffer)); //Clear the buffer

		//Put the incoming text into our buffer
		iResult = recv (ClientSocket, buffer, sizeof(buffer)-1, 0); 

		cout << "le buffer contient: ->" << buffer << "****" << endl;
		//cout << "le bufferOut  contient: " << bufferOut << "****" << endl;

		/*if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			//printf("Size of buffer: %d\n", sizeof(buffer)-1);
			//printf("Size of bufferOut: %d\n", (int)strlen(bufferOut));

			// Echo the buffer back to the sender
			iSendResult = send( ClientSocket, bufferOut, (int)strlen(bufferOut), 0 );
			//iSendResult = send( ClientSocket, buffer, 79, 0 );
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				//return 1;
			}

			//memset(bufferOut, 0, sizeof(bufferOut)); //Clear the buffer
			//printf("Bytes sent: %d\n", iSendResult);
		}*/


		//Release the Critical section
		//LeaveCriticalSection(&m_cs);

		/*if ( iResult > 0 )
		std::cout << "On a resu le message : " << buffer << std::endl;
		else if ( iResult == 0 )
		printf("Connection closed\n");
		else
		printf("recv failed: %d\n", WSAGetLastError());

		std::cout << "Receiving : " << buffer << std::endl; */
		Sleep(10);
	}

}

void TabletServer::sendData(const char* str)
{
		// Lock the Critical section
		//EnterCriticalSection(&m_cs);

		//bufferOut = str;
		OutputDebugString("sending Data\n");
		//bufferOut = "ceci est un test interactif \n";	
		int res = send(ClientSocket, str, (int)strlen(str), 0);
		OutputDebugString("sended data\n");
		if (res == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		//return 1;
		}
	//Release the Critical section
		//LeaveCriticalSection(&m_cs);

}

std::string TabletServer::parseData(){

	// Lock the Critical section
	//EnterCriticalSection(&m_cs);

	std::string test (buffer);

	//Release the Critical section
	//LeaveCriticalSection(&m_cs);

	//std::cout << "Test : " << test << std::endl;
	ostringstream ss;
	ss << "Test Message: " << test << std::endl;
	if(!test.empty()){
		OutputDebugString(ss.str().c_str());
	}
	else{

	}
	memset(buffer, 0, sizeof(buffer)); //Clear the buffer

	return test;
}

unsigned int __stdcall TabletServer::ListenSocketWrapper(void* o)
{
	TabletServer * srv = (TabletServer*)o;   // the tricky cast

	srv->ReadSocket(); 
	//    static_cast<Server*>(o)->ListenSocket(o);

	return 1;
}
