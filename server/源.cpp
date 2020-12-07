#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main() 
{
	WSADATA WSAData;
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