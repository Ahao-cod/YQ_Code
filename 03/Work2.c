#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    //打开lcd ---dev/fb0
    int lcd_fd = open("/dev/fb0",O_RDWR);
    if(lcd_fd < 0)
    {
        printf("open lcd fail\n");
        return -1;
    }
    printf("open lcd success\n");
    //lcd的映射
    int *p_lcd = mmap(NULL, //If addr is NULL, then the kernel chooses the address
        800*480*4, //lcd的大小
        PROT_READ|PROT_WRITE, //权限
        MAP_SHARED,//标志量
        lcd_fd,
        0); //偏移量
    if(p_lcd == NULL)
    {
        printf("mmap fail\n");
        return -1;
    }
    printf("mmap success\n");

    while(1)
    {
        //打开图片01
        int bmp_fd1 = open("01.bmp",O_RDWR);
        if(bmp_fd1 < 0)
        {
            printf("open bmp1 fail\n");
            return -1;
        }
        printf("open bmp1 success\n");
        //去除bmp图片的头54个字节
        lseek(bmp_fd1,54,SEEK_SET);
        //读取图片01
        char buf1[800*480*3] = {0};
        read(bmp_fd1,buf1,800*480*3);
        printf("read bmp1 success\n");

        //写入lcd
        int x; //横轴
        int y; //纵轴
        for(y=0;y<480;y++)
        {
            for(x=0;x<800;x++)
            {
                *(p_lcd+(479-y)*800+x)  = buf1[(y*800+x)*3 + 0] |
                                buf1[(y*800+x)*3 + 1]<<8 |
                                buf1[(y*800+x)*3 + 2]<<16 ;    
            }
        }

        //关闭图片01
        close(bmp_fd1);
        sleep(2);

        //打开图片02
        int bmp_fd2 = open("02.bmp",O_RDWR);
        if(bmp_fd2 < 0)
        {
            printf("open bmp2 fail\n");
            return -1;
        }
        printf("open bmp2 success\n");
        //去除bmp图片的头54个字节
        lseek(bmp_fd2,54,SEEK_SET);
        //读取图片02
        char buf2[800*480*3] = {0};
        read(bmp_fd2,buf2,800*480*3);
        printf("read bmp2 success\n");

        //写入lcd
        for(y=0;y<480;y++)
        {
            for(x=0;x<800;x++)
            {
                *(p_lcd+(479-y)*800+x)  = buf2[(y*800+x)*3 + 0] |
                                buf2[(y*800+x)*3 + 1]<<8 |
                                buf2[(y*800+x)*3 + 2]<<16 ;    
            }
        }

        //关闭图片02
        close(bmp_fd2);
        sleep(2);

        //打开图片03
        int bmp_fd3 = open("03.bmp",O_RDWR);
        if(bmp_fd3 < 0)
        {
            printf("open bmp3 fail\n");
            return -1;
        }
        printf("open bmp3 success\n");
        //去除bmp图片的头54个字节
        lseek(bmp_fd3,54,SEEK_SET);
        //读取图片03
        char buf3[800*480*3] = {0};
        read(bmp_fd3,buf3,800*480*3);
        printf("read bmp3 success\n");

        //写入lcd
        for(y=0;y<480;y++)
        {
            for(x=0;x<800;x++)
            {
                *(p_lcd+(479-y)*800+x)  = buf3[(y*800+x)*3 + 0] |
                                buf3[(y*800+x)*3 + 1]<<8 |
                                buf3[(y*800+x)*3 + 2]<<16 ;    
            }
        }

        //关闭图片03
        close(bmp_fd3);
        sleep(2);
    }

    //关闭lcd
    munmap(p_lcd,800*480*4);
    close(lcd_fd);

    return 0;
}