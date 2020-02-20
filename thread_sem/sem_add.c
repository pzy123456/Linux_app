/******************************************************************
 * 文件：sem_add.c
 * 描述：两个线程同步读写缓冲区（生产者/消费者问题），生产者就是写线程，
 *      消费者就是读线程,用两个信号量分别检查可读可写缓冲区是否有资源，
 *      如果没有则阻塞
 * 日期：2020-02-19
 * 作者：pzy
 * 版本：Ver.1.0 | 最初版本
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

/* 全局数组 */
char buf[32];
sem_t sem;
/* 子线程用来读 */
void *fun(void *arg)
{
    while (1)
    {
        /* 释放资源 */
        sem_wait(&sem);
        /* 打印buf字符个数 */
        printf("character is %d\r\n",(int)strlen(buf));
    }

}

/* 主线程用来写 */
int main(int argc,char **argv) {
    int ret;
    pthread_t pth;

    /* 先初始化信号量，在创建线程 */
    /*
        如果先创建线程在创建信号量，你不知道主线程和子线程那个执行快
        如果主线程执行的快，那么新线程就不能执行
    */
    /* 信号量进行初始化，线程间，初始值为0 */
    if(sem_init(&sem, 0, 0) < 0)
    {
        printf("sem init fail\n");
        return -1;
    }

    ret = pthread_create(&pth, NULL,fun, NULL);
    if(-1 == ret)
    {
        printf("pthread_creat error\r\n");
        return -1;
    }      
    /* 申请资源 */
    do
    {
        /* 往命令行输入字符 */
        fgets(buf, 32, stdin);
        /* 接收到字符就收到信号量 */
        sem_post(&sem);
    } while (strncmp(buf,"quit",4) != 0);/* 直到输入到quit四个字的时候退出 */
    /* 回收子线程,这句话不能加因为到不了 */
    //pthread_join(pth , &reg);

    return 0;
}
