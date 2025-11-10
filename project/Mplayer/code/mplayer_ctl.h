#ifndef __MPLAYER_CTL_H__
#define __MPLAYER_CTL_H__

#include <stdio.h>
#include <string.h>

#endif

// 播放模式枚举
typedef enum
{
    MODE_SINGLE,    // 单曲播放
    MODE_SEQUEENCE, // 顺序播放
    MODE_RANDOM     // 随机播放
} PlayMode;

// 歌词结构体
typedef struct
{
    double time;
    char text[256];
} Lyric;

// 全局状态变量（互斥访问）
/*
 * char playlist[MAX_FILES][MAX_PATH]: 存储媒体文件的完整地址；以及最大支持的歌曲数量
 * int playlist_count :      记录当前播放列表中总的歌曲数量
 * int current_song :        代表当前歌曲的索引 (播放/暂停)
 * PlayMode play_mode :      表示当前的播放模式
 * volatile int is_playing : 布尔标志:表示播放状态 (正在/暂停)
 * pid_t mplayer_pid :       存储子进程Mplayer 的 ID
 * int pipe_fd :             代表父进程打开的、用于向 MPlayer 子进程发送命令的命名管道（FIFO）的文件描述符。
 * volatitle double current_time : 存储已播放时间
 * volatitle double total_time:  存储全部时间
 * Lyric lyrics[MAX_LYRICS]:    存储全部歌词信息
 * int lyric_count:             记录 lyrics 数组中实际加载的歌词行数。
 * pthread_mutex_t mutex :       互斥锁
 */
typedef struct
{
    char playlist[MAX_FILES][MAX_PATH];
    int playlist_count;
    int current_song;
    PlayMode play_mode;
    pid_t mplayer_pid;
    int pipe_fd;
    volatile int is_playing;
    volatile double current_time; // 当前播放时间（秒）
    volatile double total_time;   // 总时长（秒）
    Lyric lyrics[MAX_LYRICS];
    int lyric_count;
    pthread_mutex_t mutex;
} AppState;

AppState app;

void show_playlist();
void init_app_state();