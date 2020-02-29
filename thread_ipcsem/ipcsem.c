/******************************************************************
 * 文件：ipcsem.c
 * 描述：父子进程通过System V信号灯同步到共享内存的读写
 * 日期：2020-02-29
 * 作者：pzy
 * 版本：Ver.1.0 | 最初版本
*******************************************************************/
/*
    解题步骤：
    需要创建两个信号灯：一个表示可读缓冲区的个数，一个表示可写缓冲区个数
    1.父进程从键盘输入字符串到共享内存
    2.子进程删除字符串中的空格并打印
    3.父进程输入quit后删除共享内存和信号灯集
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define WRITE 1
#define READ  0
#define N     64

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


/* 信号量初始化 */
void sem_init(int semid,int s[],int n)
{
    int i;
    union semun myun;
    for(i = 0; i < n; i++)
    {
        /* 给信号灯赋值 */
        myun.val = s[i];
        /* 再给每个成员进行初始化 */
        semctl( semid, i , SETVAL ,myun);
    }
}

/* 获取释放信号量 ,对信号量的编号进行Pv操作*/
void pv(int semid , int num, int pv)
{
    struct sembuf buf;
    buf.sem_num = num;
    buf.sem_op = pv;
    buf.sem_flg = 0;
    /* 对一个信号灯进行pv操作 */
    semop(semid, &buf, 1);
}

int main(int argc,char **argv)
{
    int shmid,semid,s[] = {0,1};
    pid_t id;
    key_t key;
    char *shmaddr;//字符串类型

    /* 创建key值 */
    if(key = ftok(".",'s') < 0)
    {
        printf("ftok\n");
        return -1;
    }

    /* 创建共享内存 */
    if((shmid = shmget( key, N, IPC_CREAT| 0666)) < 0)
    {
        printf("shmget\n");
        return -1;
    }

    /* 创建信号量 */
    if((semid = semget( key, 2, IPC_CREAT| 0666)) < 0)
    {
        printf("semget\n");
        /* 退出之前要把之前创建好的共享内存删除 */
        shmctl(shmid,IPC_RMID,NULL);
    }

    /* 初始化信号量集合 */
    sem_init( semid, s, 2);
    /* 映射共享内存 */
    if((shmaddr = (char *)shmat( shmid, NULL, 0)) == (char *)-1)
    {
        printf("shmat\n");
        /* 映射内存出错，就删除信号灯集 */
        semctl( semid, 0, IPC_RMID);
    }

    /* 创建父子进程用于操作 */
    if((id = fork()) < 0)
    {
        printf("fork\n");
        /* 创建进程出错，就删除信号灯集 */
        semctl( semid, 0, IPC_RMID);
    }
    else if(id == 0)
    {
        char *p,*q;
        /* 子进程循环输出 */
        while (1)
        {
            /* 判断有没有可读资源，没有进行p操作 */
            pv( semid, READ, -1);
            /* 指针指向共享资源的首地址 */
            p = q = shmaddr;
            /* 逐个扫描 */
            while( *q )
            {
                /* 如果没有遇到空格 */
                if ( *q != ' ' )
                {
                    /* 进行赋值 */
                    *p++ = *q;
                }
                q++;
            }
            /* 加上结束符 */
            *p = '\0';
            /* 打印共享内存 */
            printf("%s\n",shmaddr);//换成p试试
            /* 释放父进程继续写操作 */
            pv( semid, WRITE, 1);
        }       
    }
    else
    {
        /* 父进程循环输入 */
        while (1)
        {
            /* 父进程申请写缓冲区资源 */
            pv( semid, WRITE, -1);
            /* 如果可以则用户输入 */
            printf("input > ");
            /* 从键盘里输入字符串到共享内存地址 */
            fgets( shmaddr, N, stdin);
            /* 判断如果遇到quit字符串输入时则退出进程 */
            if(strcmp(shmaddr,"quit\n") == 0) break;
            /* 如果不是quit字符串那就释放读资源子进程进行读操作,申请读资源 */
            pv( semid, READ, 1);
        }
        /* 发出杀死进程信号 */
         kill( id, SIGUSR1);
    }
    
    return 0;
}