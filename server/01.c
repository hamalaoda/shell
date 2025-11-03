#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FIFO1 "fifo_lucy_to_peter" // lucy到peter的管道
#define FIFO2 "fifo_peter_to_lucy" // peter到lucy的管道
/*创建管道文件*/
void create_fifo(const char *fifo_name)
{
    if (access(fifo_name, F_OK) == -1)
    {
        if (mkfifo(fifo_name, 0666) == -1)
        {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }
}

/*Lucy 进程：父进程发送，子进程接收*/
void lucy_chat()
{
    int fd_write, fd_read;
    char buffer[BUFFER_SIZE];
    pid_t pid;

    // 创建子进程负责接收消息
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) // 进入子进程 接收消息(peter 发来的)
    {
        // 阻塞方式打开管道
        if ((fd_read = open(FIFO2, O_RDONLY)) == -1)
        {
            perror("open fifo2 failed");
            exit(EXIT_FAILURE);
        }

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_read = read(fd_read, buffer, BUFFER_SIZE - 1); // 从0开始所以-1
            if (bytes_read > 0)
            {
                printf("\n Peter: %s", buffer);
                printf("lucy:");
                fflush(stdout); // 刷新缓冲区
            }
            else if (bytes_read == -1)
            {
                perror("read failed");
                break;
            }
        }
        close(fd_read);
        exit(EXIT_SUCCESS);
    }
    else // 父进程 --- 发送消息 (给Peter)
    {
        // 阻塞方式打开管道
        if (fd_write = open(FIFO1, O_WRONLY) == -1)
        {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        printf("Lucy 聊天开始 (输入exit退出聊天)\n ");

        while (1)
        {
            printf("lucy:");
            fflush(stdout); // 刷新缓冲区
            memset(buffer, 0, BUFFER_SIZE);

            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            {
                break;
            }

            /*发送消息*/
            if (write(fd_write, buffer, strlen(buffer)) == -1)
            {
                perror("write failed");
                break;
            }

            /*检查退出条件*/
            if (strncmp(buffer, "exit", 4) == 0)
            {
                break;
            }
        }
        close(fd_write);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}

/*Peter进程：父进程发送，子进程接收*/
void peter_chat()
{
    int fd_write, fd_read;
    char buffer[BUFFER_SIZE];
    pid_t pid;

    /*创建子进程负责接收消息*/
    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) /*进入子进程，负责接收消息lucy发来*/
    {
        // 以只读的方式打开
        if (fd_read = open(FIFO1, O_RDONLY) == -1)
        {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        while (1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_read = read(fd_read, buffer, BUFFER_SIZE - 1);
            if (bytes_read > 0)
            {
                printf("\n Lucy: %s", buffer);
                printf("Peter:");
                fflush(stdout);
            }
            else if (bytes_read == -1)
            {
                perror("read failed");
                break;
            }
        }
        close(fd_read);
        exit(EXIT_SUCCESS);
    }
    else
    { // 父进程 - 发送消息(给Lucy)
        if ((fd_write = open(FIFO2, O_WRONLY)) == -1)
        {
            perror("open fifo2 failed");
            exit(EXIT_FAILURE);
        }

        printf("Peter 聊天开始 (输入exit退出)\n");
        while (1)
        {
            printf("Peter: ");
            fflush(stdout);
            memset(buffer, 0, BUFFER_SIZE);

            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            {
                break;
            }

            if (write(fd_write, buffer, strlen(buffer)) == -1)
            {
                perror("write failed");
                break;
            }

            if (strncmp(buffer, "exit", 4) == 0)
            {
                break;
            }
        }

        close(fd_write);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    // 创建两个FIFO管道
    create_fifo(FIFO1);
    create_fifo(FIFO2);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 子进程作为Peter
        peter_chat();
    } else {
        // 父进程作为Lucy
        lucy_chat();
        wait(NULL);  // 等待Peter进程结束
        
        // 清理FIFO文件
        unlink(FIFO1);
        unlink(FIFO2);
    }

    return 0;
}
