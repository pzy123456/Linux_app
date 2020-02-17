#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
char buff[100] = {0};
void fun(void)
{
    printf("hello pthread\n");
}
int main(void) {
    pthread_t pth = -1;
    int ret = -1;
    //线程创建第三个参数是函数指针，所以指向的是一个函数
    ret = pthread_create(&pth, NULL,
                          fun, NULL);
    if(0 != ret)
    {
        printf("创建线程失败\n");
        exit(0);
    }
    while (scanf("%s",buff))
    {
        if(strncmp(buff,"end",3) == 0)
        {
            printf("over");
            exit(0);
        }
    }
    printf("%d\n",strlen(buff));
    return 0;
}
