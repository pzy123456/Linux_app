/******************************************************************
 * 文件：clientA.c
 * 描述：两个进程通过消息队列轮流将键盘上的字符串发给对方，
 *      接收并打印对方的消息，输入quit删除当前创建的消息队列
 * 日期：2020-02-24
 * 作者：pzy
 * 版本：Ver.1.0 | 最初版本
*******************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

typedef struct
{
    /* 消息类型 */
    long mtype;
    /* 发送的字符串 */
    char mtext[64];
}MSG;

/* 消息的正文长度是结构体大小减去消息类型 */
#define LEN sizeof(sizeof(MSG) - sizeof(long))
/* 消息A */
#define TYPEA 100
#define TYPEB 200
int main(int argc, char const *argv[])
{
    key_t key;
    int msgid;
    MSG clientA;
    /* 创建key值 */
    if((key = ftok(".", 'q')) < 0)
    {
        printf("ftok error\r\n");
        return -1;
    }
    /* 根据key值创建消息队列 */
    if((msgid = msgget(key, IPC_CREAT|0666)) < 0)
    {
        printf("msgget fail\n");
        return -1;
    }
    /* 循环里 */
    while (1)  
    {
        /* 定义消息队列B的类型名为200 */
        clientA.mtype = TYPEB;
        printf("input >:");
        /* 从键盘上输入字符串 */
        fgets(clientA.mtext,64,stdin);
        /* 消息发送是整个结构体,无消息阻塞 */
        msgsnd(msgid, &clientA, LEN, 0);
        /* 如果主动发送quit则直接break循环 */
        if(strcmp(clientA.mtext , "quit\n") == 0)  break;
        /* 如果接受到自己发来的消息就打印出来 */
        if(msgrcv(msgid, &clientA,LEN, TYPEA, 0) < 0)
        {   
            printf("msgrcv fail\n");
            return 0;
        }
        /* 接收到quit字符串 */
        if(strcmp(clientA.mtext , "quit\n") == 0)
        {
            /* 控制队列并删除消息队列 */
            msgctl(msgid, IPC_RMID, 0);
            return -1;
        }
        /* 将对方B发来的消息打印出来 */
        printf("recv from clientB.mtext is %s\n",clientA.mtext);

    
    }
    

    return 0;
}

 