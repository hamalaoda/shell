#include "comm.h"

/* 共享内存初始化函数
 * 参数：
 *   pathname: 工程运行工作目录名称
 *   pro_id:   工程编号
 *   size:     共享内存空间字节数
 *   shmaddr:  返回共享内存空间映射成功的地址
 * 返回值：
 *   成功： 返回共享内存的ID
 *   失败： 返回-1
 */
int ShmInit(const *pathname, int proj_id, size_t size, void **shmaddr)
{
    key_t key;
    int shmid;

    /*获取system V IPC对象所需的键值 key*/
    key = ftok(pathname, proj_id);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /* 创建/获取共享内存空间 ：不存在则创建，存在则获取 */
    shmid = shmget(key, size, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    /*共享内存映射：将共享内存空间映射到用户空间*/
    *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        perror("shmat failed");
        return -1;
    }

    return shmid;
}

/* 共享内存释放函数 */
int ShmDestroy(const void **shmaddr, int shmid)
{
    /*共享内存解映射*/
    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt failed");
        return -1;
    }

    // 指针置为空，防止野指针
    *shmaddr = NULL;

    /*释放共享内存*/
    if (shmctl(shmid, IPC_RMID, NULL))
    {
        perror("shmctl failed");
        return -1;
    }

    return 0;
}