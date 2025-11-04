#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /*实现文件写操作访问*/
    int fd;        // 文件描述符 用于系统直接操作文件
    char buf[128]; // 数据缓冲区

    /*以读写的权限打开文件，不存在则创建*/
    fd = open("eg.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    fgets(buf, sizeof(buf), stdin);//从键盘向buf写入数据
    /*获取的数据写入文件中*/
    

    return 0;
}
