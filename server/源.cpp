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
int bit = 1;			//01位
int totalsize = 255;
char recvbuff[256];//读缓冲区（数据+标识+校验+0）在字节中的顺序就是里面计算的顺序
char ack[6] = "ACK";
char nak[6] = "NAK";
using namespace std;

int getbit(char* recv, int size)
{
	return recv[size - 2];
}

int check(char* recvbuff, int size)
{
	unsigned short int h = 0;//差错校验值
	for (int i = 0; i < size - 1; i++)//取每8位校验
	{
		h = h + (unsigned short int)recvbuff[i];
		if (h >= max8bit)
		{
			h = h % max8bit;
			h = h + 1;
		}
	}
	if ((char)h != recvbuff[size - 1])
	{
		cout << "!";
		return 1;
	}
	return 0;
}
/*long long int a=0;
for (int i = 0; i < size - 1; i++)
{
	a = a + recvbuff[i];
	cout << a << endl;
}
return 0;*/
/*unsigned short int h = 0;//差错校验值
for (int i = 0; i < size - 2; i++)//取每8位校验
{
	h = h + (unsigned short int)recvbuff[i];
	if (h >= max8bit)
	{
		h = h % max8bit;
		h = h + 1;
	}
}
h = ~h;//取反
h = h & 0x00FF;
h = h + (unsigned short int)1;//加1
while (h >= max8bit)//检验是否有进位
{
	h = h % max8bit;
	h = h + 1;
}
h = h & 0x00FF;//取最低位
if (h == (unsigned short int)recvbuff[size - 1])
	cout << "ok";
else
	cout << "?";
return 0;
/*signed short int h = 0;//差错校验值
unsigned int length;
for (int i = 0; i < size ; i++)//取每8位校验
{
	h = h + (unsigned short int)recvbuff[i];
	if (h >= max8bit)
	{
		h = h % max8bit;
		h = h + 1;
	}
}
while (h >= max8bit)//检验是否有进位
{
	h = h % max8bit;
	h = h + 1;
}
h = h & 0x00FF;//取最低位
h--;
cout << "h:" << h << endl;
if (h == 0)
{
	return 0;

}
else
{
	cout << "r1";
	return 1;
}*/

void addbit(char* tosend, int size, int bit)
{
	tosend[size - 2] = bit;
}
int jiebao(char* recvbuff, int size, SOCKET sockServer, sockaddr_in ClintAddr)
{
	int b = getbit(recvbuff, size);
	if ((check(recvbuff, size) == 0) && bit != b)
	{
		addbit(ack, 6, b);
		if (rand() % 10 != 3)
		{
			cout << "sendack" << endl;
			sendto(sockServer, ack, 6, 0, (SOCKADDR*)&ClintAddr, sizeof(SOCKADDR));
		}
		else
		{
			cout << "丢掉" << endl;
		}
		bit = b;
		return 0;
	}
	else if (b != bit)
	{
		cout << "----nak" << endl;
		addbit(nak, 6, b);
		sendto(sockServer, nak, 6, 0, (SOCKADDR*)&ClintAddr, sizeof(SOCKADDR));
		return 1;
	}
	else
		return 1;

}
int main()
{
	WSADATA WSAData;//socker记录
	sockaddr_in ServerAddr, ClintAddr;//服务器地址信息
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(4567);
		ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
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
	//收链接信号
	cout << "con" << endl << endl;
	int che = 1;
	while (che == 1)
	{
		recvfrom(sockServer, recvbuff, totalsize, 0, (SOCKADDR*)&ClintAddr, &fromlen);
		che = jiebao(recvbuff, 6, sockServer, ClintAddr);
		string s = recvbuff;
		cout << "sss:" << s << endl;
	}
	for (int i = 0; i < 6; i++)
	{
		cout << (signed short int)recvbuff[i] << endl;
	}
	cout << "ack,收下这个包" << endl;
	//发文件
	cout << "准备接受文件" << endl;
	ofstream fout("a.jpg", ios::binary);
	cout << "准备接受文件" << endl;
	while (1)
	{
		cout << "3";
		recvfrom(sockServer, recvbuff, totalsize, 0, (SOCKADDR*)&ClintAddr, &fromlen);
		cout << "1";
		int b = jiebao(recvbuff, 255, sockServer, ClintAddr);
		if (b != 0)
		{
			cout << "erroe" << endl;
			continue;
		}
		string s = recvbuff;
		if (s == "bye")
			break;
		else
		{
			fout.write(recvbuff, sizeof(char) * SEND_A_TIME);
		}
		cout << "2";
	}
}