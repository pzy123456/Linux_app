#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/*
	socket 
	bind
	listen
	
	accept
	send/recv
*/

int main(int argc,char *argv[])
{
	int socketserver;
	int iRet;
	int socketclient;/*客户端描述*/
	unsigned char recevbuff[1000];/*接收客户端发来的缓冲区*/
	struct sockaddr_in socketserveraddr;
	struct sockaddr_in socketclientaddr;
	int recevlen;
	int addrlen;
	/*定义一个区分哪一个客户端发来的数据*/
	int clientnum = -1;

	//退出僵尸态
	signal(SIGCHLD, SIG_IGN);
	/*没有ip,port任何信息*/
	socketserver =  socket(AF_INET,SOCK_STREAM,0);/*TCP连接，流式套接字，0*/
	if(socketserver == -1)
	{
		printf("socket error\r\n");
		return -1;
	}
	/*绑定自己的IP地址，端口号*/
	socketserveraddr.sin_family = AF_INET;  //设置地址家族
    socketserveraddr.sin_port = htons(8888);  //设置端口,因为转换网络字节序htons()(host to net)
    socketserveraddr.sin_addr.s_addr = INADDR_ANY;//pc上所有ip  			inet_addr("192.168.1.0");  //设置地址
	memset(socketserveraddr.sin_zero,0,8);
	
	
    iRet = bind(socketserver,(const struct sockaddr *)&socketserveraddr,sizeof(struct sockaddr)); 
	if(iRet == -1)
	{
		printf("bind error\r\n");
		return -1;
	}
	/*listen 监听那个socket,第二个参数代表同时监听多少路连接*/
	iRet = listen(socketserver,10);
	if(iRet == -1)
	{
		printf("listen error\r\n");
		return -1;
	}
	while(1)
	{	
		addrlen = sizeof(struct sockaddr);
		/*等待接收客户端连接*/
		socketclient =  accept(socketserver, (struct sockaddr *)&socketclientaddr, &addrlen);
		/*不等于-1表示连接成功*/
		if(socketclient != -1)
		{
			clientnum++;
			/*从那个客户端得到什么消息,inet_nto将地址转换成字符串192.168.1.1*/
			printf("Get connet from client %d : %s\r\n",clientnum,inet_ntoa(socketclientaddr.sin_addr));	
			/*如果这个程序想支持多客户端来连接，就要创建子进程*/
			/*进程每来一个连接马上复制出另外一个进程(子进程)从main到fork完全一样开始进行*/
			if(!fork())	
			{
				/* 子进程 */
				while (1)
				{
					/*接收客户端发来的数据，并显式出来(源，目的)*/
					 recevlen = recv(socketclient, recevbuff, 999,0);
				     if(recevlen <= 0)
					 {
						 close(socketclient);
						 /*如果有接收到的数据小于等于0,表示没有收到数据*/
						 printf("有数据错误\r\n");
						 return -1;
					 }else
					 {
						 /*结束符*/
						 recevbuff[recevlen] = '\0';
						 /*表示收到从客户端发来的消息*/
						 printf("Get msg from client %d:%s\r\n",clientnum,recevbuff);
					 }
					 
				}
				
				
			}else
			{
				;/* 主进程 */
			}
			
		}
	}
	close(socketserver);
	return 0;	
}
