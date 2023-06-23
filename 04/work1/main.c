#include <stdio.h>
#include "lcdjpg.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>

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
    //打开lcd文件
    int fd_lcd = open("/dev/fb0", O_RDWR);
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

    //打开触摸屏文件
    int fd_ts = open("/dev/input/event0", O_RDWR);

    if(fd_ts < 0)
    {
        perror("open ts error");
        return -1;
    }

    //定义一个结构体变量
    struct input_event ts_buf;
    //定义触摸屏的坐标
    int x,y = 0;

	//屏幕显示白色
	int i,j;
	for(i=0;i<480;i++)
	{
		for(j=0;j<800;j++)
		{
			*(p_lcd+i*800+j) = WHITE;
		}
	}

    lcd_draw_jpg(400,480,"02.jpg",NULL,0,0);

    while(1)
    {
        //读取触摸屏的坐标
        read(fd_ts, &ts_buf, sizeof(ts_buf));
        //判断是否是触摸屏事件
        if(ts_buf.type == EV_ABS)
        {
            //判断是否是x轴坐标,并记录
            if(ts_buf.code == ABS_X)
            {
                x = ts_buf.value * 800 / 1024;
            }
            //判断是否是y轴坐标,并记录
            if(ts_buf.code == ABS_Y)
            {
                y = ts_buf.value * 480 / 600;
            }
        }
        //按下去
        if(ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH && ts_buf.value == 1)
        {
            lcd_draw_jpg(400,480,"01.jpg",NULL,0,0);
        }
        //抬起来
        if(ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH && ts_buf.value == 0)
        {
            lcd_draw_jpg(400,480,"02.jpg",NULL,0,0);
        }
    }

	//关闭lcd文件，解除映射
	munmap(p_lcd, 800 * 480 * 4);
    close(fd_lcd);

	return 0;
}

