#include<stdio.h>                   //��׼IO
#include<WinSock2.h>                //Win��Sock2��
#include<sys/stat.h>                //_stat64�����õ���ͷ�ļ�
#pragma comment(lib,"ws2_32.lib")   //����ws2_32.dll
#pragma warning(disable:4996)       //�ر�4996����
const int bufSize = 10000;          //�����û���������СΪ10000Byte
sockaddr_in severAddr, clientAddr;   //���� ��������ַ��Ԫ�� �ͻ��˵�ַ��Ԫ��
SOCKET sever, client;               //���� �������׽��� �ͻ����׽���

/*�ú���������ʼ��WinSock��API*/
void SockStart()
{
	char severIp[16];                                    //��ŷ�������IP��ַ
	WSADATA ws;                                          //���Windows Socket API��������Ϣ
	WSAStartup(MAKEWORD(2, 2), &ws);                     //��ʼ��Windows Socket API
	severAddr.sin_family = AF_INET;                      //���÷�������ַΪTCP/IP��ַ����
	severAddr.sin_port = htons(17017);                   //���÷��������Ŷ˿�Ϊ17017�˿�
	printf("�������˵��ʮ����IP��\n");
	scanf("%s", severIp);
	severAddr.sin_addr.S_un.S_addr = inet_addr(severIp); //��severIpת��Ϊ�����ֽ�˳�������ʼ����������IP��ַ
}


/*���������������ļ��������ֱ�Ϊ��Ŀ���׽��֣����������ļ����׽��֣���Ŀ���ļ�ָ�룬Ŀ���ļ���*/
void SendModu(SOCKET target, FILE* file, const char* name)
{
	int tmp;                            //�����ʱ����
	char ack = 'Y';                     //����ȷ���ļ��Ƿ�������
	char dataLen[12];                   //�÷��Ŵ�ŷ����ļ��Ĵ�С
	long long fileLen;                  //�����δ���ļ��ĵĴ�С
	char sendBuf[bufSize];              //�����û����ͻ�����
	struct _stat64 fileStat;            //����һ���ļ�״̬������
	_stat64(name, &fileStat);           //��ȡname�ļ���״̬��Ϣ�������ļ�״̬������
	fileLen = fileStat.st_size;         //���ļ�״̬�������л���ļ��Ĵ�С

	for (int i = 0; i < 12; i++)                   //��dataLenȫ������
		dataLen[i] = 0;
	for (int i = 11; i >= 0 && fileLen > 0; i--)   //�������ļ���Сת��Ϊ�ַ���ʽ�����磺��1717ת��Ϊ"1,7,1,7"
	{
		dataLen[i] = fileLen % 10;                 //����ѧ����Ͳ����ҵ���ⷶΧ����
		fileLen /= 10;                             //�԰�
	}

	send(target, name, 100, 0);                    //���ļ���nameͨ���׽���target���͸����ն�

	send(target, dataLen, sizeof(dataLen), 0);     //���ļ�����dataLenͨ���׽���target���͸����ն�

	fileLen = fileStat.st_size;                    //��ȡ�����͵��ļ���С
	while (fileLen > 0)                            //ѭ�����ͣ�ֱ���������
	{
		fread(sendBuf, bufSize, 1, file);          //��file�ļ��ж�ȡbufSize��С���ֽڵ�sendBuf��
		tmp = send(target, sendBuf, bufSize, 0);   //��sendBuf�е��ַ�ͨ��target����bufSize�ֽڸ����շ���������ʵ�ʷ����˶����ֽ�
		fileLen -= tmp;                            //ʣ������ֽ�û�з���
	}
	while (recv(target, &ack, 1, 0) == 0);         //������ն˷���'Y'����ȷ�϶Է��������
	closesocket(target);                           //���Źر��׽���
}


/*���������������ļ�������Ϊ��Ŀ���׽��֣����������ļ����׽��֣�*/
void RecvModu(SOCKET target)
{
	int tmp;                              //�����ʱ����
	FILE* file;                           //�����ļ�ָ��
	char name[100];                       //�����ļ���
	char ack = 'Y';                       //����ȷ���ļ��Ƿ�������
	char dataLen[12];                     //�÷��Ŵ�ŷ����ļ��Ĵ�С
	long long fileLen = 0;                  //�����δ�ŷ����ļ��Ĵ�С����ʼ��СΪ0
	char recvBuf[bufSize];                //�����û����ջ�����
	long long mul = 100000000000;         //�ļ�����ת��ʱʹ�õ�����


	while (recv(target, name, sizeof(name), 0) == 0);         //���նԷ����͵��ļ���
	while (recv(target, dataLen, sizeof(dataLen), 0) == 0);   //���նԷ����͵��ļ���С

	file = fopen(name, "wb");                                 //��name����ļ�

	for (int i = 0; i < 12; i++)                              //���ַ����ļ���Сת��Ϊ���θ�ʽ�����磺��"1,7,1,7"ת��Ϊ1717  
	{
		fileLen += ((long long)dataLen[i] * mul);              //������ѧ��
		mul /= 10;                                            //�����㣬�ͺܷ�
	}

	while (fileLen > 0)                                       //ѭ�����գ�ֱ���������
	{
		if (fileLen < bufSize)                                //��ʣ�೤�Ȳ���һ��bufSizeʱ��ÿ�ν���һ���ֽ�
		{
			tmp = recv(target, recvBuf, 1, 0);                //��target����1���ֽڵ�recvBuf��
			fwrite(recvBuf, tmp, 1, file);                    //��recvBuf��tmp���ֽ�д��file
		}
		else                                                  //��ʣ�೤�ȴ��ڵ���bufSizeʱ��ÿ�ν���bufSize���ֽ�
		{
			tmp = recv(target, recvBuf, bufSize, 0);          //��target����bufSize���ֽڵ�recvBuf��
			fwrite(recvBuf, tmp, 1, file);                    //��recvBuf��tmp���ֽ�д��file
		}
		fileLen -= tmp;                                       //ʣ����ٸ��ֽ�û����
	}
	send(target, &ack, 1, 0);                                 //������ϣ���֪���ͷ�
	Sleep(17);                                               //�����ݷ�һ�����17���룩
	closesocket(target);                                      //���Źر��׽���
}


/*������������ʼ���ͻ��˵��׽��ֲ�������������*/
void ClientModu()
{
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                   //��ʼ���ͻ����׽���
	while (connect(client, (sockaddr*)&severAddr, sizeof(sockaddr)) != 0); //ʹ��client�׽��ֲ��ϳ������ӷ�������ֱ�����ӳɹ�
}


/*������������ʼ������˵��׽��ֲ�������������м���*/
void SeverModu()
{
	int addrLen = 16;                                                    //��������ʼ���ͻ��˵������ַ���ȣ�ע��һ��Ҫ��ʼ����
	sever = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                   //��ʼ���������׽���
	bind(sever, (sockaddr*)&severAddr, sizeof(sockaddr));                //���������ĵ�ַ��Ԫ����sever�׽��ְ�
	listen(sever, 5);                                                    //ʹ��sever�׽��ֽ��м�����������г���Ϊ5
	client = accept(sever, (sockaddr*)&clientAddr, &addrLen);            //ʹ��client�׽�����sever���������׽��ֶԽ�
}


/*��˵�е���������û�������ң�*/
void main()
{
	int mod;                                    //����ģʽ
	SockStart();                                //�����׽���API������˵�����׽��ַ���
	printf("0.�����\n1.�ͻ���\n");             //����ģʽѡ��
	scanf("%d", &mod);
	if (mod == 0)
		SeverModu();
	else
		ClientModu();
	printf("0.�����ļ�\n1.�����ļ�\n");         //����ģʽѡ��
	scanf("%d", &mod);
	if (mod == 0)
		RecvModu(client);                       //��client�׽��ֽ����ļ�
	else
	{
		char name[100];                         //�����ļ���
		printf("�����ļ�������׺��\n");
		scanf("%s", name);                      //����ļ���
		SendModu(client, fopen(name, "rb"), name); //ͨ��client�׽��ַ���name�ļ�
	}
	system("pause");                            //�����վס���ȵ���
}
