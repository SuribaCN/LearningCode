#pragma once
#ifndef IPHEADER_H
#define IPHEADER_H
// ����TCPͷ�ṹ
struct TcpHeader
{
    unsigned short int th_sport; // 16λԴ�˿�
    unsigned short int th_dport; // 16λĿ�Ķ˿�
    unsigned int th_seq; // 32λ���к�
    unsigned int th_ack; // 32λȷ�Ϻ�
    unsigned char th_lenres; // 4λ�ײ�����/6λ������
    unsigned char th_flag; // 6λ��־λ
    unsigned short int th_win; // 16λ���ڴ�С
    unsigned short int th_sum; // 16λУ���
    unsigned short int th_urp; // 16λ��������ƫ����
};
struct PseudoHeader // ����α�ײ�
{
    unsigned long saddr; // Դ��ַ
    unsigned long daddr; // Ŀ�ĵ�ַ
    char mbz;      // �̶�Ϊ0
    char ptcl; // Э������
    unsigned short tcpl; // TCP����UDP���ĳ���
};
// ����IPͷ�ṹ
struct IpHeader
{
    unsigned char ip_hl : 4, ip_v : 4; // 4λ�ײ����� + 4λIP�汾��
    unsigned char ip_tos; // 8λ��������
    unsigned short int ip_len; // 16λ�ܳ���(�ֽ�)
    unsigned short int ip_id; // 16λ��ʶ
    unsigned short int ip_off; // 3λ��־λ
    unsigned char ip_ttl; // 8λ����ʱ��TTL
    unsigned char ip_p; // 8λЭ��(TCP, UDP, ...)
    unsigned short int ip_sum; // 16λIP�ײ�У���
    unsigned int ip_src; // 32λԴIP��ַ
    unsigned int ip_dst; // 32λĿ��IP��ַ
};
// ����UDPͷ�ṹ
struct UdpHeader
{
    unsigned short uh_sport;
    unsigned short uh_dport;
    unsigned short uh_len;
    unsigned short uh_cksum;
};
// ����ICMP echo request���Ľṹ
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
