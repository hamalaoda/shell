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

void init_app_state()
{
    app.playlist_count = 0;
}
