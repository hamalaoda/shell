#ifndef __MPLAYER_CTL_H__
#define __MPLAYER_CTL_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

#define MAX_FILES 100 // 最大歌曲数量
#define MAX_PATH 256
#define MAX_LYRICS 500
#define PIPE_NAME "/tmp/mplayer_control_pipe"
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
void add_to_playlist(const char *file);
void show_playlist();
void init_playlist(const char *file);
void start_mplayer();