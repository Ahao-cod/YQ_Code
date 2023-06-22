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
    char buf[1024] = {0};
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

    ret = lseek(fd, 3, SEEK_SET);
    printf("ret = %d\n", ret);
    ret = lseek(fd, 2, SEEK_CUR);

    //ret = read(fd, buf, sizeof(buf));
    ret = read(fd, buf, 4);
    printf("ret: %d, buf: %s \n", ret, buf);

    close(fd);

    return 0;
}