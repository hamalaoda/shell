#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

/*实现功能，子进程循环向共享内存写入数据，父进程循环读取数据*/

int main(int argc, char const *argv[])
{
    /* 1、创建共享内存*/
    key_t key;
    int shmid;
    char *shmaddr;
    pid_t pid; // 标识进程id

    /* 获取 system V IPC对象 键值 */
    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /*创建共享内存 存在则获取 不存在则创建*/
    shmid = shmget(key, 1024, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    /*共享内存映射*/
    shmaddr = (char *)shmat(shmid, NULL, 0); // 共享内存id 系统分配的虚拟地址 读写权限
    if (shmaddr == (void *)-1)
    {
        perror("shmat failed");
        return -1;
    }

    /*将顺序字符串数据写入到共享内存中*/
    strcpy(shmaddr, "0123456789");

    /*创建子进程*/
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return -1;
    }

    /*进入子进程*/
    else if (pid == 0)
    {
        /*循环写数据到共享内存  实现倒叙*/
        while (1)
        {
            int i;
            char temp;
            char *p = shmaddr;
            printf("倒叙前: %s\n", shmaddr);
            for (int i = 0; i < 5; i++)
            {
                temp = p[i];
                p[i] = p[9 - i];
                p[9 - i] = temp;
            }
            sleep(1);
        }
    }

    printf("main process\n");
    /*进入父进程：循环读取共享内存中的数据并通过标准输出*/
    while (1)
    {
        printf("倒叙后的数据：shmaddr :%s\n", shmaddr);
        sleep(1);
    }

    return 0;
}
