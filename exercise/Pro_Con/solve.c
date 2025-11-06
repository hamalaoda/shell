#include "comm.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid; // 定义进程号
    int shmid;
    char *shmaddr;

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
    }

    return 0;
}
