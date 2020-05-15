#pragma once
#ifndef IPHEADER_H
#define IPHEADER_H
// 定义TCP头结构
struct TcpHeader
{
    unsigned short int th_sport; // 16位源端口
    unsigned short int th_dport; // 16位目的端口
    unsigned int th_seq; // 32位序列号
    unsigned int th_ack; // 32位确认号
    unsigned char th_lenres; // 4位首部长度/6位保留字
    unsigned char th_flag; // 6位标志位
    unsigned short int th_win; // 16位窗口大小
    unsigned short int th_sum; // 16位校验和
    unsigned short int th_urp; // 16位紧急数据偏移量
};
struct PseudoHeader // 定义伪首部
{
    unsigned long saddr; // 源地址
    unsigned long daddr; // 目的地址
    char mbz;      // 固定为0
    char ptcl; // 协议类型
    unsigned short tcpl; // TCP或者UDP包的长度
};
// 定义IP头结构
struct IpHeader
{
    unsigned char ip_hl : 4, ip_v : 4; // 4位首部长度 + 4位IP版本号
    unsigned char ip_tos; // 8位服务类型
    unsigned short int ip_len; // 16位总长度(字节)
    unsigned short int ip_id; // 16位标识
    unsigned short int ip_off; // 3位标志位
    unsigned char ip_ttl; // 8位生存时间TTL
    unsigned char ip_p; // 8位协议(TCP, UDP, ...)
    unsigned short int ip_sum; // 16位IP首部校验和
    unsigned int ip_src; // 32位源IP地址
    unsigned int ip_dst; // 32位目的IP地址
};
// 定义UDP头结构
struct UdpHeader
{
    unsigned short uh_sport;
    unsigned short uh_dport;
    unsigned short uh_len;
    unsigned short uh_cksum;
};
// 定义ICMP echo request包的结构
struct IcmpHeader
{
    byte type;
    byte code;
    unsigned short checksum;
    unsigned short id;
    unsigned short seq;
    unsigned long timestamp;
};
#endif // IPHEADER_H
