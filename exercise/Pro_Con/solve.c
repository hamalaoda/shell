#include "../../my_basic/semaphore/comm.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    pid_t pid; // 定义进程号
    int shmid;
    char *shmaddr;
    int min, max;

    /*定义生成随机数函数*/
    printf("请输入最小值和最大值:\n");
    scanf("%d %d", &min, &max);
    long rand_in_range(int min, int max)
    {
        // 确保范围有效
        if (min > max)
        {
            int temp = min;
            min = max;
            max = temp;
        }
        // 生成 [min, max] 之间的随机数
        return min + (random() % (max - min + 1));
    }
    /* 1、创建共享内存并映射共享内存空间 */
    shmid = ShmInit(".", 65, 1024, &shmaddr);
    if (shmid == -1)
    {
        perror("ShmInit failed");
        return -1;
    }

    /* 2、创建父子进程 */
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return -1;
    }

    /* 3、子进程往共享内存写数据 (确保共享内存空间只有一个进程在读写) */
    else if (pid == 0)
    {
        // 子进程循环写入100个整数
        int i = 0;
        int count = 100;

        for (int i = 0; i < count; i++)
        {
        }
    }

    return 0;
}
