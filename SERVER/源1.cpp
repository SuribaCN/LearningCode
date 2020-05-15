#include <iostream>
#include <winsock2.h>
#pragma warning (disable : 4996)
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT 4000
#define IP_ADDRESS "192.168.1.105"

DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	struct sockaddr_in ClientAddr;
	int AddrLen = sizeof(ClientAddr);
	SOCKET CientSocket = (SOCKET)lpParameter;
	int Ret = 0;
	char RecvBuffer[MAX_PATH];
	while (true)
	{
		memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
		Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);
		getpeername(CientSocket, (struct sockaddr*) & ClientAddr, &AddrLen);
		if (Ret == 0 || Ret == SOCKET_ERROR)
		{
			cout << "客户" << inet_ntoa(ClientAddr.sin_addr) << ":" << ClientAddr.sin_port << "退出。" << endl;//客户端退出
			break;
		}
		cout << "客户" << inet_ntoa(ClientAddr.sin_addr) << ":" << ClientAddr.sin_port << "说:" << RecvBuffer << endl;//输出服务端收到的信息
		Ret = send(CientSocket, RecvBuffer, (int)strlen(RecvBuffer), 0);//将接收到的字符串回送给客户端
	}

	return 0;
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
		cout << "Init Windows Socket Failed::" << GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "Create Socket Failed::" << GetLastError() << endl;
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
		cout << "Bind Socket Failed::" << GetLastError() << endl;
		return -1;
	}
	//listen
	Ret = listen(ServerSocket, 10);
	if (Ret != 0)
	{
		cout << "listen Socket Failed::" << GetLastError() << endl;
		return -1;
	}

	cout << "服务端已经启动" << endl;

	while (true)
	{
		AddrLen = sizeof(ClientAddr);
		ClientSocket = accept(ServerSocket, (struct sockaddr*) & ClientAddr, &AddrLen);
		if (ClientSocket == INVALID_SOCKET)
		{
			cout << "Accept Failed::" << GetLastError() << endl;
			break;
		}

		cout << "客户端连接::" << inet_ntoa(ClientAddr.sin_addr) << ":" << ClientAddr.sin_port << endl;

		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
		if (hThread == NULL)
		{
			cout << "Create Thread Failed!" << endl;
			break;
		}

		CloseHandle(hThread);
	}

	closesocket(ServerSocket);
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}