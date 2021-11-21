#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {
	string Host = "127.0.0.1"; // Ip Address of the server!!
	int port = 5004; // Server Port

	// Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start winsockl, error #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, Error #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill structure in
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, Host.c_str(), &hint.sin_addr);

	// connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, error #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}
	char buf[4096];
	string userInput ;
	string userpassword ;

	do {
		cout << "Username:";
		getline(cin, userInput);

		if (userInput.size() > 0) { 
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				cout << "Password:";
				getline(cin, userpassword);
				if (userpassword.size() > 0) { 
					int sendResult = send(sock, userpassword.c_str(), userpassword.size() + 1, 0);
					if (sendResult != SOCKET_ERROR) {
						ZeroMemory(buf, 4096);
						int bytesReceived = recv(sock, buf, 4096, 0);
						if (bytesReceived) {
							
						}
					}
				}
			}
		}
		
		int bytesReceived = recv(sock, buf, 4096, 0);
		int choice;
		string choiceStr;
		int i = 1;
		if (string(buf, 0, bytesReceived) == ("correct")) {
			cout << "" << endl;
			cout << "--Giris Ýslemi Basarili--" << endl;
			cout << "" << endl;
			do {

				cout << "1-Hesap Bilgilerini Gösterme." << endl;
				cout << "2-Hesaba Para Yatirma." << endl;
				cout << "3-Hesaptan Para Cekme." << endl;
				cout << "4-Baska Hesaba Para gönderme." << endl;
				cout << " " << endl;
				cout << "Lutfen yapmak istediginiz islem icin sayi giriniz: ";
				cin >> choice;
				cout << "" << endl;
				if (choice <= 0 || choice >= 5) {
					cout << "Tekrar Deneyin." << endl;
					cout << "" << endl;
				}

			} while (choice <= 0 || choice >= 5);


			string response = "1";
			int choice = recv(sock, buf, 4096, 0);
			switch (choice) {
			case 1: {
				choiceStr = to_string(choice);
				send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
				for (int i = 0; i < 5; i++) {
					if (i == 0) {
						cout << "Ýsim: ";
					}
					else if (i == 1) {
						cout << "Soyisim: ";
					}
					else if (i == 2) {
						cout << "Banka Adý: ";
					}
					else if (i == 3) {
						cout << "Hesap Numarasý: ";
					}
					else
					{
						cout << "Hesap Bakiyesi: $";

					}
					bytesReceived = recv(sock, buf, 4096, 0);
					cout << string(buf, 0, bytesReceived) << endl;
					send(sock, response.c_str(), response.size() + 1, 0);

				}
				cout << " " << endl;
			}
			}

		}
	} while (userInput.size() > 0);

	// close socket
	closesocket(sock);
	WSACleanup();
}