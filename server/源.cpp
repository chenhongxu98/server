#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define hostip "127.0.0.1"
#define SEND_A_TIME 250	//һ�η���250���ֽ�
#define SIZE_OF_CHECK 1	//У��ռ1�ֽ�
#define SIZE_OF_BIT 1	//ʶ�����ռ1�ֽ�
#define TIME_OUT 500	//��ʱ500ms
#define max8bit 256		//ʹ��8λУ�����256ȡģ
int bit = 0;			//01λ
int totalsize =  SEND_A_TIME + SIZE_OF_BIT + SIZE_OF_CHECK + 1;
char recvbuff[ SEND_A_TIME + SIZE_OF_BIT + SIZE_OF_CHECK + 1];//��������������+��ʶ+У��+0�����ֽ��е�˳�������������˳��
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
	unsigned short int h = 0;//���У��ֵ
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
	if (h / max8bit == 1)//�����Ƿ��н�λ
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
	WSADATA WSAData;//socker��¼
	sockaddr_in ServerAddr,ClintAddr;//��������ַ��Ϣ
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(4567);
		ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)//��ʼ���׽���
	{
		cout << "fail to start" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "success to init" << endl;
	}
	SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//����һ��socket
	if (bind(sockServer, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)//��ʼ���׽���
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
	//��ô�������Ӱ�
}