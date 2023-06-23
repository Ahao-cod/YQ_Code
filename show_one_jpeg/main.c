#include <stdio.h>
#include "lcdjpg.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
参数说明：
0:x坐标
0:y坐标
./2.jpg：图片的名字
NULL:pjpg_buf
0:jpg_buf_size
0:jpg_half
编译：
	arm-linux-gcc *.c -o main -I./libjpeg -L./libjpeg -ljpeg 

*/

#define WHITE 0x00ffffff


int main(void)
{
	int fd_lcd;

    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd < 0)
    {
        printf("open /dev/fb0 fail\n");
        return -1;
    }
    printf("open /dev/lcd success\n");

    //lcd的映射 -- mmap
    int *p_lcd = mmap(NULL, 
    800 * 480 * 4, 
    PROT_READ | PROT_WRITE, 
    MAP_SHARED, 
    fd_lcd, 
    0);

    if(p_lcd == NULL)
    {
        perror("mmap error");
        return -1;
    }

	//屏幕显示白色
	int i,j;
	for(i=0;i<480;i++)
	{
		for(j=0;j<800;j++)
		{
			*(p_lcd+i*800+j) = WHITE;
		}
	}

	lcd_draw_jpg(0,240,"200.jpg",NULL,0,0);
	lcd_draw_jpg(0,480,"02.jpg",NULL,0,0);
	lcd_draw_jpg(200,240,"03.jpg",NULL,0,0);
	lcd_draw_jpg(200,480,"04.jpg",NULL,0,0);

	while(1)
	{
		lcd_draw_jpg(400,480,"05.jpg",NULL,0,0);
		sleep(2);
		lcd_draw_jpg(400,480,"06.jpg",NULL,0,0);
		sleep(2);
		lcd_draw_jpg(400,480,"07.jpg",NULL,0,0);
		sleep(2);
		lcd_draw_jpg(400,480,"08.jpg",NULL,0,0);
		sleep(2);
	}

	//关闭lcd文件，解除映射
	munmap(p_lcd, 800 * 480 * 4);
    close(fd_lcd);

	return 0;
}

