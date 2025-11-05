#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

/*定义消息结构*/
struct msgbuf
{
    long mtype;       // 消息类型；必须为long且>0
    char mtext[1024]; // 消息正文
};

int main(int argc, char const *argv[])
{
    int msgid;
    key_t key;
    struct msgbuf msg; // 生成结构变量

    /*生成键值并获取消息队列*/
    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }

    msgid = msgget(keyIPC_CREAT | 0777);
    if (msgid == -1)
    {
        perror("msgget failed");
        return -1;
    }

    // 写入消息(类型为 "1")
    msg.mtype = 1;
    strcpy(msg.mtext, "hello world");

    /*发送消息(阻塞模式)*/
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1)
    {
        perror("msgsnd failed");
        return -1;
    }

    printf("message send : %s\n", msg.mtext);

    return 0;
}
