#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

// 打印文件权限
void print_permissions(mode_t mode) {
    // 文件类型
    if (S_ISDIR(mode)) putchar('d');
    else if (S_ISLNK(mode)) putchar('l');
    else if (S_ISREG(mode)) putchar('-');
    else if (S_ISCHR(mode)) putchar('c');
    else if (S_ISBLK(mode)) putchar('b');
    else if (S_ISFIFO(mode)) putchar('p');
    else if (S_ISSOCK(mode)) putchar('s');
    else putchar('?');

    // 用户权限
    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');

    // 组权限
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');

    // 其他用户权限
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');
}

// 安全构建完整路径
void build_full_path(const char *dir_path, const char *filename, char *full_path, size_t max_len) {
    if (dir_path == NULL || filename == NULL || full_path == NULL || max_len == 0)
        return;

    // 处理根目录情况
    if (strcmp(dir_path, "/") == 0) {
        snprintf(full_path, max_len, "/%s", filename);
    }
    // 处理当前目录情况
    else if (strcmp(dir_path, ".") == 0) {
        snprintf(full_path, max_len, "%s", filename);
    }
    // 普通目录
    else {
        snprintf(full_path, max_len, "%s/%s", dir_path, filename);
    }
}

// 列出目录内容（带属性）
void list_directory(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[1024];
    struct stat file_stat;
    struct passwd *pwd;
    struct group *grp;
    char time_str[20];

    dir = opendir(dir_path);
    if (!dir) {
        perror("无法打开目录");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // 跳过.和..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // 构建完整路径（修复路径处理逻辑）
        build_full_path(dir_path, entry->d_name, full_path, sizeof(full_path));

        // 获取文件状态
        if (lstat(full_path, &file_stat) == -1) {
            fprintf(stderr, "无法获取文件状态: %s (路径: %s)\n", entry->d_name, full_path);
            continue;
        }

        // 打印权限
        print_permissions(file_stat.st_mode);
        printf(" ");

        // 打印硬链接数
        printf("%2ld ", file_stat.st_nlink);

        // 打印所有者
        if ((pwd = getpwuid(file_stat.st_uid)) != NULL)
            printf("%-8s ", pwd->pw_name);
        else
            printf("%-8d ", file_stat.st_uid);

        // 打印所属组
        if ((grp = getgrgid(file_stat.st_gid)) != NULL)
            printf("%-8s ", grp->gr_name);
        else
            printf("%-8d ", file_stat.st_gid);

        // 打印文件大小
        printf("%6lld ", (long long)file_stat.st_size);

        // 打印修改时间
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", 
                localtime(&file_stat.st_mtime));
        printf("%s ", time_str);

        // 打印文件名
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        list_directory(".");
    } else if (argc == 2) {
        list_directory(argv[1]);
    } else {
        fprintf(stderr, "用法: %s [目录名]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
