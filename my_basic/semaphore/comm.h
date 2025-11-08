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

union semun
{
    int              val;    //用于setval传递单个信号量的初始值
    struct semid_ds  *buf;   //用于IPC_STAT/IPC_SET :传递集合属性结构体
    unsigned short   *array; //GETALL/SETLL :传递信号量值数组
    struct seminfo   *_buf;  //IPC_INFO :(GNU扩展。查询系统限制)
};

extern int ShmInit(const char *pathname, int proj_id, size_t size, void **shmaddr);
extern int ShmDestroy(const void **shmaddr, int shmid);
extern int SemInit(const char *pathname, int proj_id, int nsems, int value);
