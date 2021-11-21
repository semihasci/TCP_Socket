#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <fstream>
#include <algorithm>

#pragma comment (lib, "ws2_32.lib")//windows TCP socket library

using namespace std;

void main() {
	// winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't Initialize winsock! Quitting..." << endl;
		return;
	}

	// Create a socket!
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Can't create a socket! Quitting..." << endl;
		return;
	}

	// Bind the ip and port to socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5004);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock for listening
	listen(listening, SOMAXCONN);

	// Wait for connection
	cout << "Baglaniyor bekleyiniz.." << endl;
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; // Client's name
	char service[NI_MAXSERV]; // Service (IP:PORT) wthe client when connect

	ZeroMemory(host, NI_MAXHOST); // same of memset (host, 0 , NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close socket listening
	closesocket(listening);

	// loop accept message back to client
	char buf[4096];
	string name;
	string correct = "correct";
	

	while (true) {
		ZeroMemory(buf, 4096);

		// wait for client send data
		int byteReceived = recv(clientSocket, buf, 4096, 0);
		if (byteReceived == SOCKET_ERROR) {
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (byteReceived == 0) {
			cout << "Client is disconnected!" << endl;
			break;
		}
		if(true)
		{
			name = (string(buf, 0, byteReceived));
			cout << "name" << string(buf, 0, byteReceived) << endl;
			send(clientSocket, correct.c_str(), correct.size() + 1, 0);
			int byteReceivedpass = recv(clientSocket, buf, 4096, 0);
			name = (string(buf, 0, byteReceivedpass));
			cout << "pass" << string(buf, 0, byteReceivedpass) << endl;
			send(clientSocket, correct.c_str(), correct.size() + 1, 0);
			send(clientSocket, "correct", byteReceived + 1, 0);
			
		}
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}