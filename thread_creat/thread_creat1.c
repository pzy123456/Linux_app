#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *mythread(void *id)
{
    printf("thread run,value is %d\n",*(int *)id);
    
    //return NULL;
    pthread_exit((void *)0);//退出线程
}

int main(void)
{
    pthread_t tid;
    int i = 10;
    //i是传入线程参数，mythread是线程函数名，传入函数指针
    int status = pthread_create(&tid, NULL,
                          mythread, (void*)&i);
    if(status < 0)
    {
        printf("creat fail\r\n");
    }
   
    printf("main func finish\n");
    //sleep(1);//发现运行的结果并不如我们预期那样，就好像线程没有执行一样
            //原因在于如果主线程退出了，那么其他线程也会退出。
            //所以我们要在主线程退出之前，延时一会以便把子线程退出然后主线程在退出
            //这个1s是线程执行的时间

    //那如果线程的时间超过1S，很多时候不知道线程要执行多长的时间，那怎么办
    //用pthread_join()
    pthread_join(tid, NULL);//该函数是让子线程函数退出
    return 0;
}