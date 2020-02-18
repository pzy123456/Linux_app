/*
	本实验创建线程，返回传参
	打印返参值，和数组共享的部分
*/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/* 全局变量是存放到静态存储区供其他线程函数使用 */
/* 不能定义为字符指针因为“”是常量区，线程引用会被常量区会被改变发送段错误 */
char message[32] = "pzy linux";

void *fun(void *arg)
{
    sleep(1);
    strcpy(message,"panzhiyong");
    pthread_exit("hello world");
}

int main(int argc,char **argv) {
    pthread_t pth;
    void *result;/* 用于接收线程函数返回值 */
    int ret;
    ret = pthread_create(&pth, NULL, fun, NULL);
    if(-1 == ret)
    {
        printf("pthread_creat error\n");
        return -1;
    }
    /* 等待子线程结束 */
    pthread_join(pth, &result); 
    /* 字符串首地址打印出来 */                  
    printf("result is %s\n",result);
    printf("message is %s\n",message);
    return 0;
}