#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h> // 注意头文件包含的顺序
#include <winbase.h>
#include <windows.h>
#include <windef.h>
#include "ipheader.h"
#include <stdio.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;
#define ICMP_ECHO_REPLY 0
#define ICMP_DEST_UNREACH 3
#define ICMP_TTL_EXPIRE 11
#define ICMP_ECHO_REQUEST 8
#define ICMP_MIN 8 // 最小的ICMP包大小
// 功能：计算校验和
unsigned short ip_checksum(unsigned short* buffer, int size)
{
    unsigned long cksum = 0;
    // 将所有16位无符号数相加
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(unsigned short);
    }
    if (size) // 加上最后一个BYTE
        cksum += *(unsigned char*)buffer;
    // 和的前16位和后16位相加
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short)(~cksum);
}
// 初始化icmp数据包的各个字段
// 给data部分填充数据, 计算校验和
void init_ping_packet(IcmpHeader* icmp_hdr, int packet_size, int seq_no)
{
    // 设置ICMP报头字段
    icmp_hdr->type = 8; // 常量ICMP_ECHO_REQUEST ＝ 8
    icmp_hdr->code = 0;
    icmp_hdr->checksum = 0;
    icmp_hdr->id = (unsigned short)GetCurrentProcessId();
    // id号随机，这里以进程号作为id号
    icmp_hdr->seq = seq_no;
    icmp_hdr->timestamp = GetTickCount();
    // 填充data域
    const unsigned long int deadmeat = 0xDEADBEEF; // 用于填充的数据部分
    char* datapart = (char*)icmp_hdr + sizeof(IcmpHeader); // 指向数据部分
    int bytes_left = packet_size - sizeof(IcmpHeader); // 包大小减去首部长度就是数据部分长度
    while (bytes_left > 0) {
        memcpy(datapart, &deadmeat, min(int(sizeof(deadmeat)), bytes_left));
        bytes_left -= sizeof(deadmeat);
        datapart += sizeof(deadmeat);
    }
    // 计算校验和
    icmp_hdr->checksum = ip_checksum((unsigned short*)icmp_hdr, packet_size);
}
// 功能：初始化RAW Socket, 初始化目标地址
// 返回值：< 0 失败
int setup_for_ping(const char* host, SOCKET& sd, sockaddr_in& dest)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData); // 初始化
    if (err != 0) {
        cerr << "WSAStartup Error" << endl;
        return -1;
    }
    // 检查winsock DLL的版本
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        cerr << "wsaData.wVersion != 2 || wsaData.wVersion != 2" << endl;
        WSACleanup();
        return -1;
    }
    // 创建原始套接字，协议类型为IPPROTO_ICMP
    sd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 0, 0);
    if (sd == INVALID_SOCKET) {
        cerr << "Failed to create raw socket: " << WSAGetLastError() << endl;
        return -1;
    }
    unsigned int timeout = 1000;
    if (setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
        cerr << "SO_RCVTIMEo setsockopt failed: " << WSAGetLastError() << endl;
        return -1;
    }
    if (setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
        cerr << "SO_SNDTIMEo setsockopt failed: " << WSAGetLastError() << endl;
        return -1;
    }
    // 初始化目标主机的信息
    memset(&dest, 0, sizeof(dest));
    // 将命令行的第1个参数转换为目标IP地址
    dest.sin_addr.S_un.S_addr = inet_addr(host);
    if (dest.sin_addr.S_un.S_addr != INADDR_NONE) {
        dest.sin_family = AF_INET;
    }
    else {
        cerr << "dest ip not right!";
        return -1;
    }
    return 0;
}
// 功能：发送生成的ICMP包
// 返回值：< 0 发送失败
int send_ping(SOCKET sd, const sockaddr_in& dest, IcmpHeader* send_buf, int packet_size)
{
    // 发送send_buf缓冲区中的报文
    cout << "Sending " << packet_size << " bytes to " << inet_ntoa(dest.sin_addr) << "..." << flush;
    int bwrote = sendto(sd, (char*)send_buf, packet_size, 0, (sockaddr*)&dest, sizeof(dest));
 
    if (bwrote == SOCKET_ERROR) {
        cerr << "send failed: " << WSAGetLastError() << endl;
        return -1;
    }
    else if (bwrote < packet_size) {
        cout << "sent " << bwrote << " bytes..." << flush;
    }
    return 0;
}
// 发送Ping报文后，接收回复ICMP报文：
// 返回值： < 0 接收失败
int recv_ping(SOCKET sd, sockaddr_in& source, IpHeader* recv_buf, int& packet_size)
{
    // 等待Ping回复
    int fromlen = sizeof(source);
    int bread = recvfrom(sd, (char*)recv_buf, 65535, 0, (sockaddr*)&source, &fromlen);
    if (bread == SOCKET_ERROR) {
        cerr << "read failed: ";
        if (WSAGetLastError() == WSAEMSGSIZE)
            cerr << "buffer too small" << endl;
        else
            cerr << "error #" << WSAGetLastError() << endl;
        return -1;
    }
    packet_size = bread;
    return 0;
}
// 对接收到的报文进行解析：
// 功能：解析接收到的ICMP报文
// 返回值：-2: 忽略, -1: 失败, 0: 成功
int decode_reply(IpHeader* reply, int bytes, sockaddr_in* from)
{
    // 将指针偏移到ICMP报头
    unsigned short header_len = reply->ip_hl * 4;  // ip包首部长度
    IcmpHeader* icmphdr = (IcmpHeader*)((char*)reply + header_len);
    // 报文太短
    if (bytes < header_len + ICMP_MIN) {
        cerr << "too few bytes from " << inet_ntoa(from->sin_addr) << endl;
        return -1;
    }
    else if (icmphdr->type != ICMP_ECHO_REPLY) { // 解析回复报文类型
     //非正常回复
        if (icmphdr->type != ICMP_TTL_EXPIRE) { // ttl减为零
            if (icmphdr->type == ICMP_DEST_UNREACH) //主机不可达
                cerr << "Destination unreachable" << endl;
            else //非法的ICMP包类型
                cerr << "Unknown ICMP packet type " << int(icmphdr->type) << " received" << endl;
            return -1;
        }
    }
    else if (icmphdr->id != (unsigned short)GetCurrentProcessId()) { // id号不匹配
     //不是本进程发的包, 可能是同机的其它ping进程发的
        return -2;
    }
    // 输出信息
    cout << endl << bytes << " bytes from " << inet_ntoa(from->sin_addr) << ", icmp_seq " << icmphdr->seq << ", ";
    if (icmphdr->type == ICMP_TTL_EXPIRE) {
        cerr << "TTL expired." << endl;
    }
    else {
        cout << "TTL:" << (int)reply->ip_ttl << "  ";
        cout << "time: " << (GetTickCount() - icmphdr->timestamp) << " ms." << endl;
    }
    return 0;
}
int main(int argc, char* argv[])
{
    int i = 0;
    char *netip;
    SOCKET s;
    sockaddr_in dest;
    // IcmpHeader icmp_hdr; // sizeof(IcmpHeader = 12)
    char sendbuf[65500];  // sendbuf包括ICMP头，还有留出写数据的部分
    char recv_buf[65500]; // sendbuf要接收IP头以及ICMP包，所以要留出足够的空间
    int recvlen = 20;
    netip = argv[1];
    // IpHeader recv_buf;
    int n = 2, looptime = 4, packagesize = 32;
    while (n < argc) {
        if (strcmp(argv[n], "-n") == 0)
        {
            looptime = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-l") == 0)
        {
            packagesize = atoi(argv[n + 1]);
        }
        n = n + 2;
    }
    int u = 0;
    while (u < looptime)
    {
        if (setup_for_ping(netip, s, dest)) {
            cerr << "destip not in proper form, exit";
            return -1;
        }
        init_ping_packet((IcmpHeader*)sendbuf, 20, 1000);
        // ICMP头长度以及数据加起来为20
        send_ping(s, dest, (IcmpHeader*)sendbuf, packagesize);
        recv_ping(s, dest, (IpHeader*)recv_buf, recvlen);
        decode_reply((IpHeader*)recv_buf, recvlen, &dest);
        u++;
    }


}
