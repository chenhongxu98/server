#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define hostip "127.0.0.1"
#define SEND_A_TIME 250	//一次发送250个字节
#define SIZE_OF_CHECK 1	//校验占1字节
#define SIZE_OF_BIT 1	//识别编码占1字节
#define TIME_OUT 500	//超时500ms
#define max8bit 256		//使用8位校验和用256取模
int bit = 0;			//01位
int totalsize =  SEND_A_TIME + SIZE_OF_BIT + SIZE_OF_CHECK + 1;
char recvbuff[ SEND_A_TIME + SIZE_OF_BIT + SIZE_OF_CHECK + 1];//读缓冲区（数据+标识+校验+0）在字节中的顺序就是里面计算的顺序
char ack[5] = "ACK";
char nak[5] = "NAK";
using namespace std;

void getbit(char* recv)
{
	//cout << "getbit";
	bit = recv[251];
	recv[SEND_A_TIME] = 0;
}
int check(char* mas)
{
	//cout << "errorcheck";
	unsigned short int h = 0;//差错校验值
	unsigned int length = SEND_A_TIME + SIZE_OF_BIT + SIZE_OF_CHECK;
	for (int i = 0; i < length+1; i++)
	{
		h = h + mas[i];
		if (h / max8bit == 1)
		{
			h = h % max8bit;
			h = h + 1;
			h = h & 0x00FF;
		}
	}
	if (h / max8bit == 1)//检验是否有进位
	{
		h = h % max8bit;
		h = h + 1;
	}
	h = h & 0x00FF;
	cout << h << endl;
	recvbuff[252] = 0;
	if (h == 0)
	{
		return 0;
		
	}
	else
	{
		return 1;
	}
}
int main() 
{
	WSADATA WSAData;//socker记录
	sockaddr_in ServerAddr,ClintAddr;//服务器地址信息
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(4567);
		ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)//初始化套接字
	{
		cout << "fail to start" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "success to init" << endl;
	}
	SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//建立一个socket
	if (bind(sockServer, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)//初始化套接字
	{
		cout << "fail to bind" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "success to bind" << endl;
	}
	cout << "band socket as " << sockServer << endl;
	int fromlen = sizeof(SOCKADDR);
	while (1)
	{
		ofstream fout("a.jpg", ios::binary);
		string s = "1";
		do
		{
			//cout << "0";
			memset(recvbuff, 0, totalsize);
			cout << "123";
			recvfrom(sockServer, recvbuff, totalsize, 0, (SOCKADDR*)&ClintAddr, &fromlen);
			cout << "123";
			if (check(recvbuff))
			{
				cout << "error";
				nak[4] = recvbuff[251];
			}
			
			s = recvbuff;
			if (s == "end")
			{
				break;
			}
			else if (s == "SYN")
			{
				memset(recvbuff, 0, totalsize);
				ack[4] = bit;
				sendto(sockServer, ack, 5, 0, (SOCKADDR*)&ClintAddr, sizeof(SOCKADDR));
				cout << "sendsyn" << endl;
			}
			else 
			{
				fout.write(recvbuff, sizeof(char) * SEND_A_TIME);
			}
			
			
			
		} while (!(s=="end"));
		cout << "end";
		fout.close();
		cout << "over";
		getchar();
		
	}
	return 0;
	//fread
	//怎么更改链接啊
}