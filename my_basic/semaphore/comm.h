#ifndef __COMM_H__
#define __COMM_H__
#endif

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

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

extern int ShmInit(const char *pathname, int proj_id, size_t size, void **shmaddr);
extern int ShmDestroy(const void **shmaddr, int shmid);
