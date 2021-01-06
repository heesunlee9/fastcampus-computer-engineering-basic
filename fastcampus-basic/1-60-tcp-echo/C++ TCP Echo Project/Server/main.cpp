#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[Error]: " << message << '\n';
	system("pause");
	exit(1);
}

int main(void) {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	// initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ShowErrorMessage("WSAStartup()");
	}

	// create a TCP socket
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		ShowErrorMessage("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	// convert 4 byte-integer into network bytes
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// convert 2 byte-integer into network bytes
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		ShowErrorMessage("bind()");
	}
	cout << "[current state] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ShowErrorMessage("listen()");
	}
	cout << "[current state] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[current state] accept()\n";
	if (clientSocket == INVALID_SOCKET) {
		ShowErrorMessage("accept()");
	}

	// receive and send the client's message (echo)
	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[client's message]: " << received << '\n';
		cout << "[send message]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[terminate server]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}