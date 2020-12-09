#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define hostip "127.0.0.1"
using namespace std;
char recvbuff[1024];
int main() 
{
	WSADATA WSAData;//socker记录
	sockaddr_in ServerAddr,ClintAddr;//服务器地址信息

	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(12345);
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
	cout << "band socket as "<<sockServer;
	while (1)
	{
		recvfrom(sockServer,recvbuff,1024,0,(SOCKADDR*)ClintAddr)
	}
	return 0;
	//fread
	//怎么更改链接啊
}