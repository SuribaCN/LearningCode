#include <iostream>
#include <winsock2.h>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
using namespace std;
#define PORT 7000
#define IP_ADDRESS "192.168.1.105"

DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	struct sockaddr_in ClientAddr;
	int AddrLen = sizeof(ClientAddr);
	SOCKET ClientSocket_1 = (SOCKET)lpParameter;
	int Ret = 0;
	char RecvBuffer[MAX_PATH];
	while (true)
	{
		memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
		Ret = recv(ClientSocket_1, RecvBuffer, MAX_PATH, 0);
		getpeername(ClientSocket_1, (struct sockaddr*) & ClientAddr, &AddrLen);
		if (Ret == 0 || Ret == SOCKET_ERROR)
		{
			cout << "Client IP:" << inet_ntoa(ClientAddr.sin_addr) << ":" << ClientAddr.sin_port << "disconnected." << endl;//客户端退出
			break;
		}
		cout << "Client IP: " << inet_ntoa(ClientAddr.sin_addr) << ":" << ClientAddr.sin_port << " said:" << RecvBuffer<< "   Length:"<<strlen(RecvBuffer) << endl;//输出服务端收到的信息
		
	}

	return 0;
}

DWORD WINAPI SendToClient(LPVOID lpParameter)
{
	struct sockaddr_in ClientAddr;
	int AddrLen = sizeof(ClientAddr);
	SOCKET ClientSocket_1 = (SOCKET)lpParameter;
	int Ret = 0;
	char SendBuffer[MAX_PATH];
	while (true)
	{
		memset(SendBuffer, '\0', sizeof(SendBuffer));
		cin >> SendBuffer;
		getpeername(ClientSocket_1, (struct sockaddr*) & ClientAddr, &AddrLen);
		Ret = send(ClientSocket_1, SendBuffer, (int)strlen(SendBuffer), 0);
	}
}


int main(int argc, char* argv[])
{
	WSADATA  Ws;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in LocalAddr, ClientAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		cout << "Init Windows Socket Failed:" << GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "Create Socket Failed:" << GetLastError() << endl;
		return -1;
	}

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	LocalAddr.sin_port = htons(PORT);
	memset(LocalAddr.sin_zero, 0x00, 8);

	//Bind Socket
	Ret = bind(ServerSocket, (struct sockaddr*) & LocalAddr, sizeof(LocalAddr));
	if (Ret != 0)
	{
		cout << "Bind Socket Failed:" << GetLastError() << endl;
		return -1;
	}
	//listen
	Ret = listen(ServerSocket, 10);
	if (Ret != 0)
	{
		cout << "listen Socket Failed:" << GetLastError() << endl;
		return -1;
	}

	cout << "SUCESS" << endl;

	
	while (true)
	{
		AddrLen = sizeof(ClientAddr);
		ClientSocket = accept(ServerSocket, (struct sockaddr*) & ClientAddr, &AddrLen);
		if (ClientSocket == INVALID_SOCKET)
		{
			cout << "Accept Failed:" << GetLastError() << endl;
			break;
		}


		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
		CreateThread(NULL, 0, SendToClient, (LPVOID)ClientSocket, 0, NULL);
		if (hThread == NULL)
		{
			cout << "Create Thread Failed!" << endl;
			break;
		}
		cout << "Client Connect  IP Address:" << inet_ntoa(ClientAddr.sin_addr) << "    PORT:" << ClientAddr.sin_port << endl;
		CloseHandle(hThread);
	}

	closesocket(ServerSocket);
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}