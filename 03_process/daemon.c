#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* 守护进程初始化函数 */
void daemon_init(void)
{
    int fd;
    /* (必须)创建一个孤儿进程, 创建子进程并将父进程退出，其作用是将孤儿进程脱离终端在后台运行 */
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        _exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        _exit(EXIT_SUCCESS);
    }

    /* 创建新的会话(可选) */
    pid = setsid();
    if (pid == -1)
    {
        perror("setsid");
        _exit(EXIT_FAILURE);
    }

    /* 修改工作目录：将工作目录设置为/tmp目录(可选)  */
    if (chdir("/tmp") == -1)
    {
        perror("chdir");
        _exit(EXIT_FAILURE);
    }

    /* 修改文件访问权限掩码(可选) */
    umask(0);

    /* 关闭进程中所分配成功的所以文件描述(可选) */
    for (fd = 0; fd < getdtablesize(); fd++)
        close(fd);
}

int main()
{
    FILE *fp;
    daemon_init();

    fp = fopen("test.txt", "a+");

    while (1)
    {
        sleep(1);
        fprintf(fp, "time:%ld\n", time(NULL));
        fflush(fp);
    }
}