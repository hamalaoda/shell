#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
    /* 获取IPC对象键值key*/
    key_t key; // 连接文件，生成唯一标识符
    int shmid;
    char *shmaddr; //

    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /*创建共享内存 不存在则创建存在则获取*/
    shmid = shmget(key, 1024, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    return 0;
}
