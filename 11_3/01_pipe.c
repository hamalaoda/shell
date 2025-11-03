#include <stdio.h>
#include <unistd.h>

#define COUNT 5
#define SIZE 128
int main(int argc, char const *argv[])
{
    int i = 0;
    int pipefd[2];    // 存储功能管道描述符
    int fd[COUNT][2]; // 存储私有管道描述符
    pid_t pid;
    char buf[SIZE];

    /* 创建公共管道 */
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    /* 创建私有管道 */
    return 0;
}
