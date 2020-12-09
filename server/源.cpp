#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main() 
{
	WSADATA WSAData;//socker记录
	sockaddr_in ServerAddr;//服务器地址信息
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(12345);
		ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1")
	}
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "fail to start" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "success to init" << endl;
	}
	SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	cout << sockServer;
	return 0;
	//fread
	//怎么更改链接啊
}