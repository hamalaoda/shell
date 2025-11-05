#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char const *argv[])
{
    
    key_t key; // 定义键值
    int msgid; // 定义消息队列id，是函数的返回值（非负整数），用于表示消息队列

    key = ftok(".", 65); // 生成键值基于当前目录和 proj_id=65
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    /*创建消息队列 （权限为 0666），存在则获取，不存在则创建*/
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("msgget failed");
        return -1;
    }

    printf("消息队列标识符为:%d\n", msgid);
    return 0;
}
