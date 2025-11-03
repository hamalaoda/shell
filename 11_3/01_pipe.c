#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define COUNT 5
#define SIZE 128
int main(int argc, char const *argv[])
{
    int i = 0;
    int pipefd[2];    // 存储功能管道描述符
    int fd[COUNT][2]; // 存储私有管道描述符
    pid_t pid;
    char buf[SIZE];
    int ret;

    /* 创建公共管道 : 父进程读取所有子进程的数据请求*/
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    for (int i = 0; i < COUNT; i++)
    {
        /*创建私有进程：父进程发送数据到子进程*/
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            return -1;
        }

        /*创建子进程*/
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return -1;
        }
        else if (pid == 0) // 进入子进程
        {
            /*关闭私有管道写文件描述符 和 公共管道读文件描述符*/
            close(pipefd[0]); // 读
            close(fd[i][1]);  // 写
            while (1)
            {
                printf("该进程为：%d", i);
                int len;
                char buffer[128];
                memset(buf, 0, sizeof(buf));
                fgets(buf, SIZE, stdin);
                len = snprintf(buffer, 128, "%d:%s", i, buf);
                write(pipefd[1], buffer, len);

                memset(buffer, 0, 128);
                ret = read(fd[i][0], buffer, 128);

                printf("buf = %s \n", buffer);
            }
            exit(EXIT_SUCCESS);
        }
    }

    /*进入父进程*/
    if (pid > 0)
    {
        close(pipefd[1]);
        for (i = 0; i < COUNT; i++)
        {
            close(fd[i][0]);
        }

        while (1)
        {
            int num;
            memset(buf, 0, sizeof(buf));
            ret = read(pipefd[0], buf, COUNT);
            if (ret == -1)
            {
                perror("read");
                return -1;
            }
            else if (ret == 0)
            {
                break;
            }
            sscanf(buf, "%d:", &num);
            ret = write(fd[num][1], buf, ret);
            if (ret == -1)
            {
                perror("write");
                return -1;
            }
        }
    }

    return 0;
}
