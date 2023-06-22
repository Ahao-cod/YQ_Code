#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int f1,f2;
    char buf[1024]={0};
    int ret=0;
    f1 = open("P1.jpg", O_RDWR);
    f2 = open("P2.jpg", O_RDWR | O_CREAT);
    if (f1 < 0 || f2 < 0)
    {
        printf("open file fail\n");
        return -1;
    }
    printf("open file success\n");
    while(1)
    {
        ret=read(f1,buf,sizeof(buf));
        if(ret==0)
        {
            break;
        }
        write(f2,buf,ret);
    }
    close(f1);
    close(f2);
    return 0;

}