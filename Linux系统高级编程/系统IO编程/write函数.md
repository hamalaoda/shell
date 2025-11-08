```c
#include <unistd.h> 
ssize_t write(int fd, const void *buf, size_t count); 
```

- 功能：从buf指针所指向缓冲区数据的count字节写入到文件描述符fd所引用的文件中； 
- 参数：    参数1：fd表示所有写文件的文件描述；    参数2：buf所有写数据的缓冲区起始地址；    参数3：count所要写入数据的字节数(期望写入字节，实际写入字节数可能小于count)。
- 返回值：    成功返回实际写入文件的字节数，失败返回-1且修改errno的值。
-  注意：    如果count为0，实际不会写入数据，此时的返回值可能为0或者-1.