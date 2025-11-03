#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    int source_fd, dest_fd;
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];

    // 检查命令行参数
    if (argc != 3)
    {
        fprintf(stderr, "用法: %s <源文件> <目标文件>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 打开源文件（只读模式）
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
    {
        perror("打开源文件失败");
        exit(EXIT_FAILURE);
    }

    // 打开目标文件（写入模式，不存在则创建，权限为644）
    // O_CREAT: 若文件不存在则创建
    // O_WRONLY: 只写模式
    // O_TRUNC: 若文件存在则清空内容
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dest_fd == -1)
    {
        perror("打开目标文件失败");
        close(source_fd); // 确保关闭已打开的源文件
        exit(EXIT_FAILURE);
    }

    // 循环读取并写入数据
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(dest_fd, buffer, bytes_read) != bytes_read)
        {
            perror("写入文件失败");
            close(source_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    // 检查读取错误
    if (bytes_read == -1)
    {
        perror("读取文件失败");
        close(source_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    // 关闭文件描述符
    if (close(source_fd) == -1)
    {
        perror("关闭源文件失败");
        exit(EXIT_FAILURE);
    }
    if (close(dest_fd) == -1)
    {
        perror("关闭目标文件失败");
        exit(EXIT_FAILURE);
    }

    printf("文件拷贝成功: %s -> %s\n", argv[1], argv[2]);
    return 0;
}
