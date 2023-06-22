#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    int ret = 0;
    char buf[1024] = "丘昌浩，男，大小便正常，会玩智能手机，未来可期";
    //自动打开，O_CREATE
    // fd = open("2.txt", O_RDWR | O_CREAT);
    //手动打开,不加O_CREATE
    fd = open("testfile.txt", O_RDWR);
    if(fd < 0)
    {
        printf("Error opening\n");
        return -1;
    }
    printf("open successfully\n");

    ret = write(fd, buf, strlen(buf));
    printf("ret : %d\n",ret);

    close(fd);

    return 0;
}