#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


/*
	socket 
	connect
	send/recv
*/
int main(int argc,char *argv[])
{
    int socketclient;
    struct sockaddr_in socketserveraddr;
    unsigned char sendbuf[1000];
    int iRet;
    int sendlen;
    if(argc != 2)
    {
       printf("Usage:\n"); 
       printf("%s <server_ip>\r\n",argv[0]);
       return -1;
    }

    socketclient =  socket(AF_INET,SOCK_DGRAM,0);/*UDP连接，流式套接字，0*/
    /*绑定自己的IP地址，端口号*/
	socketserveraddr.sin_family = AF_INET;  //设置地址家族
    socketserveraddr.sin_port = htons(8888);  //设置端口,因为转换网络字节序htons()(host to net)

    /*把字符串(192.1668.1.1)转换为socketserveraddr.sin_addr*/
    if(0 == inet_aton(argv[1],&socketserveraddr.sin_addr))
    {
        /*如果返回0值，则返回无效*/
        printf("invalid server_ip\r\n");
        return -1;
    }
   // socketserveraddr.sin_addr.s_addr = INADDR_ANY;//pc上所有ip  			inet_addr("192.168.1.0");  //设置地址
	memset(socketserveraddr.sin_zero,0,8);
    
    iRet = connect(socketclient, (const struct sockaddr *)&socketserveraddr,
                   sizeof(struct sockaddr));
    if(iRet == -1)
    {
        printf("connect error\r\n");
        return -1;
    }
    while (1)
    {
        /*从标准输入里获得数据*/
        if(fgets(sendbuf, 999 , stdin))
        {
            /*如果获得了数据，就发送数据*/
            /*发送的长度不一定是999只要一遇到回车换行就跳出来*/
            sendlen = send(socketclient, sendbuf, strlen(sendbuf), 0);
            if( sendlen <= 0 )
            {
                printf("发送数据出错\r\n");
                close(socketclient);
                return -1;
            }
        } 
    }
    return 0;
}