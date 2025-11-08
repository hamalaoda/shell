在 Linux 系统中，操作文件（打开、创建、修改权限等）主要通过 **系统调用（system calls）** 实现，其中最核心的是 `open()` 系统调用。它不仅用于打开已有文件，还能<font color='red'>创建新文件</font>，并返回一个文件描述符（file descriptor，非负整数），后续对文件的读写、关闭等操作都通过该描述符进行。

## `open()` 系统调用的原型

`open()` 定义在 `<fcntl.h>` 头文件中，有两种常见形式：

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 形式1：打开已有文件或创建新文件（需指定权限）
int open(const char *pathname, int flags);

// 形式2：创建新文件时，需额外指定文件权限（mode）
int open(const char *pathname, int flags, mode_t mode);
```

### 参数说明

1. **`pathname`**：

   字符串，表示要打开 / 创建的文件路径（相对路径或绝对路径），例如 `"./test.txt"` 或 `/home/user/data.log`。

2. **`flags`**：

   整数，用于指定文件的打开模式、操作权限等，由多个标志位组合而成（通过按位或 `|` 连接）。常见标志如下：

   - **必选标志（互斥，必须指定其一）**：
     - `O_RDONLY`：只读模式打开。
     - `O_WRONLY`：只写模式打开。
     - `O_RDWR`：读写模式打开。
   - **可选标志（可与必选标志组合）**：
     - `O_CREAT`：若文件不存在，则创建它（此时需使用第二种原型，指定 `mode` 参数）。
     - `O_EXCL`：与 `O_CREAT` 一起使用时，若文件已存在则报错（确保创建的是新文件，避免覆盖）。
     - `O_TRUNC`：若文件存在且以可写模式打开，清空文件原有内容（长度截断为 0）。
     - `O_APPEND`：写入时数据追加到文件末尾（类似 C 语言的 `"a"` 模式）。
     - `O_NONBLOCK`：非阻塞模式（对管道、设备文件等有效，避免读写时阻塞）。
     - `O_SYNC`：写入数据时强制同步到磁盘（保证数据持久化，牺牲性能）。

3. **`mode`**：

   仅当 `flags` 包含 `O_CREAT` 时有效，用于指定新文件的**初始权限**（如 `0644` 表示所有者读写、组和其他用户只读）。

   注意：实际权限会受进程的 `umask` 影响，最终权限为 `mode & ~umask`。

### 返回值

- 成功时，返回一个**非负整数**（文件描述符，通常从 3 开始，0/1/2 分别预留给标准输入 `stdin`、标准输出 `stdout`、标准错误 `stderr`）。
- 失败时，返回 `-1`，并设置全局变量 `errno` 表示错误原因（如文件不存在、权限不足等，可通过 `perror()` 打印错误信息）。

### 示例

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // 包含 close()

int main() {
    // 以读写模式打开文件，若不存在则创建（权限 0644）
    int fd = open("test.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open failed"); // 打印错误信息（如 "open failed: No such file or directory"）
        return 1;
    }

    // 操作文件（读写等）...

    close(fd); // 关闭文件，释放描述符  
               // 通过文件描述符fd来直接操作文件
    return 0;
}
```

### 与 C 语言 `fopen()` 的区别

- `fopen()` 是 C 标准库函数，封装了 `open()` 系统调用，返回的是 `FILE*` 流指针（带缓冲区的高级接口）。
- `open()` 是 Linux 系统调用，返回文件描述符（底层接口），更接近操作系统内核，适合需要精细控制文件的场景（如非阻塞 I/O、权限控制等）。

在 Linux 编程中，系统调用 `open()` 是文件操作的基础，许多高级库（如 C 标准库、C++ STL）的文件操作最终都会通过它实现。



### **open函数的实例**

使用系统IO实现标准IO实例

- 以只读方式打开已存在的file文件    
  - fopen(file, "r");       <==>  open(file, O_RDONLY); 

- 以读写方式打开已存在的file文件    
  - fopen(file, "r+");       <==>  open(file, O_RDWR); 

- 以只写方式创建并打开file文件，如果文件不存在则创建文件并打开，如果文件存在打开并将写位置设置为起始位置        
  - fopen(file, "w");        <==>  open(file, O_WRONLY | O_CREAT, 0777); 

- 以读写方式创建并打开file文件，如果文件不存在则创建文件并打开，如果文件存在则打开文件并清空文件。  
  -  fopen(file, "w+");       <==>  open(file, O_RDWR | O_CREAT, 0777);  

- 以追加模式打开指定文件`file`，用于向文件末尾添加内容（而非覆盖原有内容）（不存在创建新文件）
  - fopen(file, "a");     <= => open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);

- 读写权限，写入时从末尾追加，可读但读操作需手动移动指针（如 `fseek`）
  - fopen(file, "a+");       <==>  open(file, O_RDWR | O_CREAT | O_APPEND, 0777);



### 权限:

-   r = 4;              w = 2;         x=1(执行)
- 文件所有者（第一位）、（第二位）同组用户、（第三位）其他用户





## close()*系统调用*

```c
#include <unistd.h> 
int close(int fd);
```

-  功能：关闭文件描述符，释放文件描述符所引用文件的所有资源。

-  参数：fd表示所有关闭的文件描述符；

-  返回值：成功返回0，失败返回-1且修改errno的值。

  

