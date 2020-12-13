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
int bit = 1;			//01λ
int totalsize = 255;
char recvbuff[256];//��������������+��ʶ+У��+0�����ֽ��е�˳�������������˳��
char ack[6] = "ACK";
char nak[6] = "NAK";
using namespace std;

int getbit(char* recv, int size)
{
	return recv[size - 2];
}

int check(char* recvbuff, int size)
{
	unsigned short int h = 0;//���У��ֵ
	for (int i = 0; i < size - 1; i++)//ȡÿ8λУ��
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
/*unsigned short int h = 0;//���У��ֵ
for (int i = 0; i < size - 2; i++)//ȡÿ8λУ��
{
	h = h + (unsigned short int)recvbuff[i];
	if (h >= max8bit)
	{
		h = h % max8bit;
		h = h + 1;
	}
}
h = ~h;//ȡ��
h = h & 0x00FF;
h = h + (unsigned short int)1;//��1
while (h >= max8bit)//�����Ƿ��н�λ
{
	h = h % max8bit;
	h = h + 1;
}
h = h & 0x00FF;//ȡ���λ
if (h == (unsigned short int)recvbuff[size - 1])
	cout << "ok";
else
	cout << "?";
return 0;
/*signed short int h = 0;//���У��ֵ
unsigned int length;
for (int i = 0; i < size ; i++)//ȡÿ8λУ��
{
	h = h + (unsigned short int)recvbuff[i];
	if (h >= max8bit)
	{
		h = h % max8bit;
		h = h + 1;
	}
}
while (h >= max8bit)//�����Ƿ��н�λ
{
	h = h % max8bit;
	h = h + 1;
}
h = h & 0x00FF;//ȡ���λ
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
			cout << "����" << endl;
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
	WSADATA WSAData;//socker��¼
	sockaddr_in ServerAddr, ClintAddr;//��������ַ��Ϣ
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(4567);
		ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
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
	//�������ź�
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
	cout << "ack,���������" << endl;
	//���ļ�
	cout << "׼�������ļ�" << endl;
	ofstream fout("a.jpg", ios::binary);
	cout << "׼�������ļ�" << endl;
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