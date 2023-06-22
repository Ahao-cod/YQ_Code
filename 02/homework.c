#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int f1, f2;
    char buf[1024] = {0};
    //打开文件1
    f1 = open("1.txt", O_RDWR);
    if(f1 < 0)
    {
        printf("Error opening\n");
        return -1;
    }
    printf("open successfully\n");

    lseek(f1, 10, SEEK_SET);

    //读取文件1
    read(f1, buf, sizeof(buf));
    printf("buf: %s\n", buf);
    close(f1);
    //打开文件2
    f2 = open("2.txt", O_RDWR);
    if(f2 < 0)
    {
        printf("Error opening\n");
        return -1;
    }
    printf("open successfully\n");
    //写入文件2
    write(f2, buf, strlen(buf));
    close(f2);
    return 0;
}




