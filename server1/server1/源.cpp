#include<stdio.h>                   //标准IO
#include<WinSock2.h>                //Win的Sock2库
#include<sys/stat.h>                //_stat64函数用到该头文件
#pragma comment(lib,"ws2_32.lib")   //连接ws2_32.dll
#pragma warning(disable:4996)       //关闭4996警告
const int bufSize = 10000;          //设置用户缓冲区大小为10000Byte
sockaddr_in severAddr, clientAddr;   //声明 服务器地址三元组 客户端地址三元组
SOCKET sever, client;               //声明 服务器套接字 客户端套接字

/*该函数用来初始化WinSock的API*/
void SockStart()
{
	char severIp[16];                                    //存放服务器的IP地址
	WSADATA ws;                                          //存放Windows Socket API的描述信息
	WSAStartup(MAKEWORD(2, 2), &ws);                     //初始化Windows Socket API
	severAddr.sin_family = AF_INET;                      //设置服务器地址为TCP/IP地址家族
	severAddr.sin_port = htons(17017);                   //设置服务器开放端口为17017端口
	printf("输入服务端点分十进制IP：\n");
	scanf("%s", severIp);
	severAddr.sin_addr.S_un.S_addr = inet_addr(severIp); //将severIp转化为网络字节顺序并用其初始化服务器的IP地址
}


/*本函数用来发送文件，参数分别为：目标套接字（用来发送文件的套接字），目标文件指针，目标文件名*/
void SendModu(SOCKET target, FILE* file, const char* name)
{
	int tmp;                            //存放临时数据
	char ack = 'Y';                     //用来确认文件是否接收完毕
	char dataLen[12];                   //用符号存放发送文件的大小
	long long fileLen;                  //用整形存放文件的的大小
	char sendBuf[bufSize];              //声明用户发送缓冲区
	struct _stat64 fileStat;            //声明一个文件状态描述符
	_stat64(name, &fileStat);           //获取name文件的状态信息，存入文件状态描述符
	fileLen = fileStat.st_size;         //从文件状态描述符中获得文件的大小

	for (int i = 0; i < 12; i++)                   //将dataLen全部置零
		dataLen[i] = 0;
	for (int i = 11; i >= 0 && fileLen > 0; i--)   //将整形文件大小转化为字符格式，例如：将1717转化为"1,7,1,7"
	{
		dataLen[i] = fileLen % 10;                 //这数学问题就不在我的理解范围内了
		fileLen /= 10;                             //对吧
	}

	send(target, name, 100, 0);                    //将文件名name通过套接字target发送给接收端

	send(target, dataLen, sizeof(dataLen), 0);     //将文件长度dataLen通过套接字target发送给接收端

	fileLen = fileStat.st_size;                    //读取所发送的文件大小
	while (fileLen > 0)                            //循环发送，直至发送完毕
	{
		fread(sendBuf, bufSize, 1, file);          //从file文件中读取bufSize大小的字节到sendBuf中
		tmp = send(target, sendBuf, bufSize, 0);   //将sendBuf中的字符通过target发送bufSize字节给接收方，并返回实际发送了多少字节
		fileLen -= tmp;                            //剩余多少字节没有发送
	}
	while (recv(target, &ack, 1, 0) == 0);         //如果接收端发送'Y'，则确认对方接收完毕
	closesocket(target);                           //优雅关闭套接字
}


/*本函数用来接受文件，参数为：目标套接字（用来接收文件的套接字）*/
void RecvModu(SOCKET target)
{
	int tmp;                              //存放临时数据
	FILE* file;                           //声明文件指针
	char name[100];                       //声明文件名
	char ack = 'Y';                       //用来确认文件是否接收完毕
	char dataLen[12];                     //用符号存放发送文件的大小
	long long fileLen = 0;                  //用整形存放发送文件的大小，初始大小为0
	char recvBuf[bufSize];                //声明用户接收缓冲区
	long long mul = 100000000000;         //文件长度转换时使用的因数


	while (recv(target, name, sizeof(name), 0) == 0);         //接收对方发送的文件名
	while (recv(target, dataLen, sizeof(dataLen), 0) == 0);   //接收对方发送的文件大小

	file = fopen(name, "wb");                                 //打开name这个文件

	for (int i = 0; i < 12; i++)                              //将字符型文件大小转化为整形格式，例如：将"1,7,1,7"转化为1717  
	{
		fileLen += ((long long)dataLen[i] * mul);              //又是数学题
		mul /= 10;                                            //不会算，就很烦
	}

	while (fileLen > 0)                                       //循环接收，直至接收完毕
	{
		if (fileLen < bufSize)                                //当剩余长度不足一个bufSize时，每次接收一个字节
		{
			tmp = recv(target, recvBuf, 1, 0);                //从target接收1个字节到recvBuf中
			fwrite(recvBuf, tmp, 1, file);                    //将recvBuf的tmp个字节写入file
		}
		else                                                  //当剩余长度大于等于bufSize时，每次接收bufSize个字节
		{
			tmp = recv(target, recvBuf, bufSize, 0);          //从target接收bufSize个字节到recvBuf中
			fwrite(recvBuf, tmp, 1, file);                    //将recvBuf的tmp个字节写入file
		}
		fileLen -= tmp;                                       //剩余多少个字节没接收
	}
	send(target, &ack, 1, 0);                                 //接收完毕，告知发送发
	Sleep(17);                                               //让数据飞一会儿（17毫秒）
	closesocket(target);                                      //优雅关闭套接字
}


/*本函数用来初始化客户端的套接字并进行连接请求*/
void ClientModu()
{
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                   //初始化客户端套接字
	while (connect(client, (sockaddr*)&severAddr, sizeof(sockaddr)) != 0); //使用client套接字不断尝试连接服务器，直至连接成功
}


/*本函数用来初始化服务端的套接字并对连接请求进行监听*/
void SeverModu()
{
	int addrLen = 16;                                                    //声明并初始化客户端的网络地址长度（注意一定要初始化）
	sever = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                   //初始化服务器套接字
	bind(sever, (sockaddr*)&severAddr, sizeof(sockaddr));                //将服务器的地址三元组与sever套接字绑定
	listen(sever, 5);                                                    //使用sever套接字进行监听，缓冲队列长度为5
	client = accept(sever, (sockaddr*)&clientAddr, &addrLen);            //使用client套接字与sever监听到的套接字对接
}


/*传说中的主函数，没错，就是我！*/
void main()
{
	int mod;                                    //声明模式
	SockStart();                                //启用套接字API，或者说启用套接字服务
	printf("0.服务端\n1.客户端\n");             //服务模式选择
	scanf("%d", &mod);
	if (mod == 0)
		SeverModu();
	else
		ClientModu();
	printf("0.接收文件\n1.发送文件\n");         //发送模式选择
	scanf("%d", &mod);
	if (mod == 0)
		RecvModu(client);                       //从client套接字接收文件
	else
	{
		char name[100];                         //储存文件名
		printf("输入文件名及后缀：\n");
		scanf("%s", name);                      //获得文件名
		SendModu(client, fopen(name, "rb"), name); //通过client套接字发送name文件
	}
	system("pause");                            //你给我站住，等等我
}
