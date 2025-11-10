#include "mplayer_ctl.h"

// 显示主菜单
void show_playlist()
{
    printf("\n===== MPlayer 控制接口 =====\n");
    printf("1. 播放歌曲\n");
    printf("2. 暂停/恢复\n");
    printf("3. 停止播放\n");
    printf("4. 上一首\n");
    printf("5. 下一首\n");
    printf("6. 调整播放速度\n");
    printf("7. 跳转时间\n");
    printf("8. 设置播放模式\n");
    printf("9. 显示播放列表\n");
    printf("0. 退出程序\n");
    printf("===========================\n");
    printf("请输入选择: ");
}

// 初始化应用状态
void init_app_state()
{
    app.playlist_count = 0;
    app.current_song = -1;
    app.play_mode = MODE_SEQUEENCE; // 默认顺序播放
    app.is_playing = 0;
    app.mplayer_pid = -1;
    app.pipe_fd = 0;
    app.current_time = 0.0;
    app.total_time = 0.0;
    app.lyric_count = 0;
    pthread_mutex_init();
}

// 初始化播放列表
void init_playlist(char *dir)
{
    DIR *dp;
    struct diremt *ep;
    char path[MAX_PATH];

    dp = opendir(dir);
    if (dp == NULL)
    {
        perror("opendir");
        return -1;
    }

    while ((ep = readdir(dp) != NULL))
    {
        /* code */
    }
}

// 添加文件到播放列表
void add_to_playlist(const char *file)
{
    if (app.playlist_count < MAX_FILES)
    {
        strncpy(app.playlist[app.playlist_count], file, MAX_PATH - 1);
        app.playlist[app.playlist_count][MAX_PATH - 1] = '\0';
        app.playlist_count++;
    }
    else
    {
        printf("播放队列已满，无法添加更多文件\n");
    }
}

// 显示播放列表
void show_playlist()
{
    printf("\n===== 播放列表 =====\n");
    for (int i = 0; i < app.playlist_count; i++)
    {
        printf("%d: %s\n", i + 1, app.playlist[i]);
    }
    printf("====================\n");
}

// 启动 MPlayer 进程，并进行 I/O 重定向
void start_mplayer()
{
    int stdout_pipe[2];
    if (pipe(stdout_pipe == -1))
    {
        perror("pipe");
        exit
    }
}