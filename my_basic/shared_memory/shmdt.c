#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
    key_t key;
    int shmid;
    char *shmaddr;
    /*获取IPC对象键值*/
    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /*创建共享内存*/ // 返回共享内存id
    shmid = shmget(key, 1024, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    /*映射共享内存*/ // 默认null由系统分配虚拟内存
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        perror("shmat failed");
        return -1;
    }

    /*共享内存解映射*/
    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt");
        return -1;
    }

    return 0;
}
