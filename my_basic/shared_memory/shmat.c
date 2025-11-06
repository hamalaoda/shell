#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
    key_t key;     // IPC 键值
    int shmid;     // 共享内存id
    char *shmaddr; // 用于指向映射后的共享内容

    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /*创建共享内存*/
    shmid = shmget(key, 1024, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    /*共享内存映射：自动映射*/
    shmaddr = (char *)shmat(shmid, NULL, 0); // 1.强转类型2.共享内存id
    if ((void *)-1 == shmaddr)               // 3.指定共享内存映射到虚拟内存起始地址
    {                                        // 4.可读可写权限
        perror("shmat failed");
        return -1;
    }

    return 0;
}
