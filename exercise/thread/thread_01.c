#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/*子线程函数*/
void *thread_callback(void *arg)
{
    int *arr = (int *)arg;
    /*获取子线程id并输出*/
    printf("child thread id = %lu\n", pthread_self());
    /*子线程执行 输出数组的逆序*/
    printf("子线程的逆序输出:\n");
    for (int i = 9; i >= 0; i--)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return NULL;
}

int main(int argc, char const *argv[])
{
    int ret;
    int a[10];           // 定义局部数组
    pthread_t thread_id; /*存储子线程id*/

    /*创建子线程 并传入数组的首地址*/
    ret = pthread_create(&thread_id, NULL, thread_callback, (void *)a);
    if (ret != 0)
    {
        fprintf(stderr, "create child thread fail :%s\n", strerror(ret));
        return -1;
    }

    /*主线程  执行写入数组并顺序输出*/
    printf("主线程的顺序输出:");
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        printf("%d ", a[i]);
    }
    printf("\n");

    /*等待子线程执行完毕，避免主线程提前退出*/
    ret = pthread_join(thread_id, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "join child thread fail: %s\n", strerror(ret));
        return -1;
    }

    return 0;
}
