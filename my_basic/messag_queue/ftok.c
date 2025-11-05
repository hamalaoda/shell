#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
{
    
    key_t key;
    /*以当前目录（.）和proj_id=65生成键值*/
    key = ftok(".", 65);
    if (key == -1)
    {
        perror("ftok failed");
        return -1;
    }
    printf("key = %d", key);

    return 0;
}
