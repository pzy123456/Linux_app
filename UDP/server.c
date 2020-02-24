#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

/*
	socket 
	listen
	send/recv
*/
#define SERVER_PORT 8888
int main(int argc,char *argv[])
{
	int socketserver;/*服务器描述*/
	int iRet;
	int socketclient;/*客户端描述*/
	unsigned char recevbuff[1000];/*接收客户端发来的缓冲区*/
	struct sockaddr_in socketserveraddr;
	struct sockaddr_in socketclientaddr;
	int recevlen;
	int addrlen;
	unsigned char recvbuff[1000];
	/*定义一个区分哪一个客户端发来的数据*/
	int clientnum = -1;
	/*没有ip,port任何信息*/
	socketserver =  socket(AF_INET,SOCK_DGRAM,0);/*UDP连接，流式套接字，0*/
	if(socketserver == -1)
	{
		printf("socket error\r\n");
		return -1;
	}
	/*绑定自己的IP地址，端口号*/
	socketserveraddr.sin_family = AF_INET;  //设置地址家族
    socketserveraddr.sin_port = htons(SERVER_PORT);  //设置端口,因为转换网络字节序htons()(host to net)
    socketserveraddr.sin_addr.s_addr = INADDR_ANY;//pc上所有ip  			inet_addr("192.168.1.0");  //设置地址
	memset(socketserveraddr.sin_zero,0,8);

    iRet = bind(socketserver,(const struct sockaddr *)&socketserveraddr,sizeof(struct sockaddr_in)); 
	if(iRet == -1)
	{
		printf("bind error\r\n");
		return -1;
	}
	
	while(1)
	{
		addrlen = sizeof(struct sockaddr);
		/*接收来自客户端的消息*/
		recevlen = recvfrom(socketserver, recvbuff, 999, 0,
                        (struct sockaddr *)&socketclientaddr, &addrlen);
		/*如果没有数据就在这里休眠了*/
		if(recevlen <= 0)
		{
			close(socketserver);
			printf("没有接收到数据，数据出错\r\n");
			return -1;
		}
		else
		{
			recvbuff[recevlen] = '\0';
			printf("Get Msg from %s:%s\r\n",inet_ntoa(socketclientaddr.sin_addr),recvbuff);
		}
	}
	close(socketserver);
	return 0;	
}
