/******************************************************************
 * 文件：mutex.c
 * 描述：创建线程，在主线程计数，在新线程判断两个临界资源的值是否相等，
 *       来判断是否用了互斥锁
 * 日期：2020-02-20
 * 作者：pzy
 * 版本：Ver.1.0 | 最初版本
*******************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
/* 定义临界资源 */
unsigned int value1,value2,count;
pthread_mutex_t lock;

/* 新线程 */
void *fun(void *arg)
{
    while (1)
    {
#ifdef _LOCK_
        /* 上锁 */
            pthread_mutex_lock(&lock);
#endif // _LOCK_
          /* 没有锁帮助肯定不相等 */
        if(value1 != value2)
        {
            printf("value1 = %d,value2 = %d\n",value1,value2);
            /* 延时1s再来判断 */
            sleep(1);
        }
#ifdef _LOCK_
        /* 解锁 */
            pthread_mutex_unlock(&lock);
#endif // _LOCK_
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t pth;
    /* 创建互斥锁 */
    if(pthread_mutex_init( &lock , NULL) != 0 )
    {
        printf("pthread_mutex_init error\n");
        return -1;
    }
    /* 创建线程 */
    if(pthread_create(&pth , NULL, fun, NULL) != 0)
    {
        printf("pthread_create error\r\n");
        return -1;
    }

    /* 主线程 */
    while (1)
    {
        count++;
        /* 判断是否有锁的宏 */
#ifdef _LOCK_
    /* 加锁 */
        pthread_mutex_lock(&lock);
#endif // _LOCK_
        value1 = count;
        /* 如果不用锁的话时间片到这里运行完之后就会运行其他线程 */
        value2 = count;
#ifdef _LOCK_
    /* 解锁 */
          pthread_mutex_unlock(&lock); 
#endif
    }
     return 0;
}
