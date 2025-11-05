#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 与发送方一致的消息结构
struct msgbuf
{
    long mtype;
    char mtext[1024];
};

int main(int argc, char const *argv[])
{
    key_t key;
    int msgid;
    struct msgbuf msg;
    ssize_t nbytes;

    // 生成键值并获取消息队列 ID（需与发送方使用相同的 key）
    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok");
        return 1;
    }
    msgid = msgget(key, 0666); // 仅获取已有队列（不创建）
    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    long type;
    while (1)
    {
        printf("请输入要接收类型的消息");
        scanf("%ld", &type);
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), type, 0) == -1)
        {
            perror("msgrcv failed");
            return -1;
        }
        printf("%s", msg.mtext);
    }

    return 0;
}
