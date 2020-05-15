#include "stdafx.h"  
#include <winsock2.h>  
#include <cstdio>
#include <wincrypt.h>
#include <time.h>
#include <cstring>
#include <iostream>  
#include <string.h> 
#define PORT 2345
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")   
#pragma comment(lib, "crypt32.lib")  

using namespace std;
const int MAX1 = 1000;
typedef long long LL;
typedef unsigned long long ULL;

int recvn(SOCKET s, char* recvbuf, unsigned int fixedlen)
{
	int iResult;
	int cnt = fixedlen; //ʣ������ֽ���δ���� 
	while (cnt > 0)
	{
		iResult = recv(s, recvbuf, cnt, 0);
		if (iResult < 0)
		{
			printf("error: %d\n", WSAGetLastError());
			return -1;
		}
		if (iResult == 0)//�Է��ر����ӣ������ѽ��յ���С��fixedlen���ֽ��� 
			return fixedlen - cnt;
		recvbuf += iResult;
		cnt -= iResult;
	}
	return fixedlen;
}

int recvvl(SOCKET s)
{
	int iResult;
	unsigned int reclen; //���ڴ洢����ͷ���洢�ĳ�����Ϣ 
	unsigned int recvbuflen = 0;
	char filename[100];
	TCHAR name[100];
	char tmp[1000], recvbuf[1000];
	memset(tmp, 0, sizeof(tmp));
	int i = 0;
	memset(filename, 0, sizeof(filename));
	while (true)
	{
		iResult = recvn(s, tmp, sizeof(char));
		if (tmp[0] == '#') break;
		filename[i++] = tmp[0];
		if (iResult != sizeof(char))//û�гɹ����յ�һ����������
		{
			if (iResult == -1)
			{
				printf("get the filename error: %d\n", WSAGetLastError());
				return -1;
			}
			else
			{
				printf("connect closed when get the filename\n");
				return 0;
			}
		}
	}
	cout << "filename is " << filename << endl;
	memset(name, 0, sizeof(name));
	for (int i = 0; filename[i]; i++)
		name[i] = filename[i];
	HANDLE hFile;               // �ļ����
	DWORD count;                // д������ݼ���
	char path_name[200];
	hFile = CreateFile(
		name,	 // �ļ���
		GENERIC_WRITE,          // д��Ȩ��
		0,                      // ��ֹ�������̷���
		NULL,                   // �ӽ��̲��ɼ̳б����
		CREATE_ALWAYS,             // ��������ʱ�������ļ�
		FILE_ATTRIBUTE_NORMAL,  // ��ͨ�ļ�
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE)
		cout << "fail to creat file" << endl;
	iResult = recvn(s, (char*)&reclen, sizeof(unsigned int));
	if (iResult != sizeof(unsigned int))
	{
		if (iResult == -1)
		{
			printf("get the size of file error: %d\n", WSAGetLastError());
			CloseHandle(hFile);
			return -1;
		}
		else
		{
			printf("connect closed when get the size of file\n");
			CloseHandle(hFile);
			return 0;
		}
	}
	reclen = ntohl(reclen);
	cout << "the size of the file is " << reclen << endl;
	recvbuflen = min(1000, reclen);//������
	int packnum = 1;
	while (reclen > 0)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		iResult = recvn(s, recvbuf, recvbuflen);
		cout << "packet " << packnum++ << " is:" << recvbuf << endl;
		WriteFile(hFile, recvbuf, strlen(recvbuf), &count, 0);//��recvbuflen����д���ļ�
		reclen -= recvbuflen; //�������һ�����ݳ��� 
		recvbuflen = min(reclen, recvbuflen);
		cout << "reclen is " << reclen << endl;
	}
	char endsign[10] = { "OK" };
	cout << "send OK to client\n get file!" << endl;
	send(s, endsign, strlen(endsign), 0);
	CloseHandle(hFile);
	return 0;
}


DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	SOCKET socketClient = (SOCKET)lpParameter;
	cout << "begin to receive file" << endl;
	recvvl(socketClient);
	cout << "closed" << endl;
	return 0;
}

int main(int argc, const char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	int error = WSAStartup(sockVersion, &wsaData);
	if (error != 0)
	{
		cout << "fail to startup! " << WSAGetLastError() << endl;
		return 0;
	}
	SOCKET socketSever = socket(AF_INET, SOCK_STREAM, 0);
	if (socketSever == INVALID_SOCKET)
	{
		cout << "socket error! " << WSAGetLastError() << endl;
		WSACleanup();
		closesocket(socketSever);
		return 0;
	}
	//�����socket ��ַ
	sockaddr_in addrServer;
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);
	//��socket���ڱ��ض˿�
	if (bind(socketSever, (SOCKADDR*)&addrServer, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "bind error! " << WSAGetLastError() << endl;;
		closesocket(socketSever);
		WSACleanup();
		return 0;
	}
	cout << "listening the client" << endl;
	if (listen(socketSever, 10) == SOCKET_ERROR)
		cout << "Listen failed with error " << WSAGetLastError() << endl;
	int len = sizeof(SOCKADDR);

	SOCKET socketClient;
	sockaddr_in addrClient;
	char* ip;
	while (true)
	{
		socketClient = accept(socketSever, (SOCKADDR*)&addrClient, &len);
		ip = inet_ntoa(addrClient.sin_addr);
		cout << "succeed connection      ip: " << ip << "    port: " << addrClient.sin_port << endl;
		HANDLE hThread = NULL;
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)socketClient, 0, NULL);
		if (hThread == NULL)
		{
			cout << "Creat Thread Failed! " << WSAGetLastError() << endl;
			break;
		}
		CloseHandle(hThread);
	}
	closesocket(socketSever);
	return 0;
}