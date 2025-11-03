// 父进程发送数据给子进程接收
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    pid_t pid; // 进程号的定义

    int pipefd[2]; // 定义数组存储管道通信口
    int ret;       // 接收读取文件结果 1.正数为实际读取的字节 2. -1读取失败 3.0读完了数据(或对方关闭管道)

    /*创建管道文件*/
    if (pipe(pipefd) == -1)
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
    // 进入子进程
    else if (pid == 0)
    /*pipefd[1] 为管道文件中的写文件描述符 pipefd[0]为读文件描述符*/
    {
        close(pipefd[1]); // 关闭子文件的写功能
        char recv_buf[128];
        while (1)
        {
            memset(recv_buf, 0, sizeof(recv_buf));             // 将接收字节流的数组清零
            ret = read(pipefd[0], recv_buf, sizeof(recv_buf)); /*读取数据存入缓冲区recv_buf*/
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            else if (ret == 0)
            {
                printf("子进程检测到父进程结束,即将退出\n");
                break; // 读取完的状态
            }
            printf("recv_buf : %s\n", recv_buf); // 打印读取的文件
        }
    }
    // 进入父进程
    else
    /*关闭读文件描述符pipefd[0]，保留写文件描述符pipefd[1]*/
    {
        close(pipefd[0]);
        char send_buf[128];
        while (1)
        {
            fgets(send_buf, sizeof(send_buf), stdin);
            ret = write(pipefd[1], send_buf, sizeof(send_buf));
            if (ret == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
