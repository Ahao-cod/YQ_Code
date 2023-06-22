#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    //打开lcd ---dev/fb0
    int fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd < 0)
    {
        printf("open lcd fail\n");
        return -1;
    }

    //lcd的映射---mmap
   int *addr = mmap(NULL, //If addr is NULL, then the kernel chooses the address
       800*480*4, //lcd的大小
       PROT_READ|PROT_WRITE, //权限
       MAP_SHARED,//标志量
       fd_lcd,
       0); //偏移量
    if(addr == NULL)
    {
        printf("mmap fail\n");
        return -1;
    }

    //打开坤坤图片
    int fd_bmp = open("01.bmp",O_RDWR);
    if(fd_bmp < 0)
    {
        printf("open bmp fail\n");
        return -1;
    }

    //去除bmp图片的头54个字节
    lseek(fd_bmp,54,SEEK_SET);

    //读取坤坤图片 800*480*3 + 54 = 1152054
    char buf[800*480*3] = {0};
    read(fd_bmp,buf,800*480*3);

    //写lcd
    int x; //横轴
    int y; //纵轴
    for(y=0;y<480;y++)
    {
        for(x=0;x<800;x++)
        {
            *(addr+(479-y)*800+x)  = buf[(y*800+x)*3 + 0] |
                               buf[(y*800+x)*3 + 1]<<8 |
                               buf[(y*800+x)*3 + 2]<<16 ;    
        }
    }

    //lcd映射的释放
    munmap(addr,800*480*4);

    //关闭lcd
    close(fd_lcd);
    close(fd_bmp);
}
