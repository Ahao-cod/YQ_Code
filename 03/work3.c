#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

//颜色宏定义
#define RED 0x00ff0000
#define BLUE 0x000000ff
#define WHITE 0x00ffffff
#define BLACK 0x00000000
#define YELLOW 0x00ffff00

int main()
{
    int lcd_fd;
    int x, y;
    //颜色数组
    unsigned int color_alter[] = {RED, BLUE};
    unsigned int color_flag = 0;
    unsigned int color_alter1[] = {BLACK, YELLOW};
    unsigned int color_flag1 = 0;
    //圆的半径
    int R = 50;
    int r = 80;
    //动态圆0初始位置在屏幕中心
    int x0 = 400, y0 = 240;
    int mx = 0, my = 0;
    //动态圆1初始位置与圆1错开
    int x1 = 600, y1 = 120;
    int mx1 = 0, my1 = 0;

    //打开lcd文件
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd_fd error");
        return -1;
    }
    printf("open lcd_fd success\n");

    //lcd的映射
    int *p_lcd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (p_lcd == NULL)
    {
        perror("mmap error");
        return -1;
    }

    while(1)
    {
        for(y = 0; y < 480; y++)
        {
            for(x = 0; x < 800; x++)
            {
                if((x - x0) * (x - x0) + (y - y0) * (y - y0) <= R * R)
                {
                    *(p_lcd + y * 800 + x) = color_alter[color_flag];
                }
                else if((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r * r)
                {
                    *(p_lcd + y * 800 + x) = color_alter1[color_flag1];
                }
                else
                {
                    *(p_lcd + y * 800 + x) = WHITE;
                }
            }
        }

        //动态圆0移动
        //触碰上下边界
        if(x0 - R == 0)
        {
            mx = 1;
            color_flag = !color_flag;
        }
        else if(x0 + R == 800)
        {
            mx = 0;
            color_flag = !color_flag;
        }

        //x0的值根据mx的值进行加减
        if(mx == 0)
        {
            x0--;
        }
        else
        {
            x0++;
        }

        //触碰左右边界
        if(y0 - R == 0)
        {
            my = 1;
            color_flag = !color_flag;
        }
        else if(y0 + R == 480)
        {
            my = 0;
            color_flag = !color_flag;
        }

        //y0的值根据my的值进行加减
        if(my == 0)
        {
            y0 = y0 - 1;
        }
        else
        {
            y0 = y0 + 1;
        }

        //动态圆1移动
        //触碰上下边界
        if(x1 - r == 0)
        {
            mx1 = 1;
            color_flag1 = !color_flag1;
        }
        else if(x1 + r == 800)
        {
            mx1 = 0;
            color_flag1 = !color_flag1;
        }

        //x1的值根据mx1的值进行加减
        if(mx1 == 0)
        {
            x1 = x1 - 2;
        }
        else
        {
            x1 = x1 + 2;
        }

        //触碰左右边界
        if(y1 - r == 0)
        {
            my1 = 1;
            color_flag1 = !color_flag1;
        }
        else if(y1 + r == 480)
        {
            my1 = 0;
            color_flag1 = !color_flag1;
        }

        //y1的值根据my1的值进行加减
        if(my1 == 0)
        {
            y1 = y1 - 1;
        }
        else
        {
            y1 = y1 + 1;
        }
        
        //圆0与圆1碰撞
        if((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) <= (R + r) * (R + r))
        {
            y0 = y0 - 5;
            y1 = y1 + 5;
        }
    }
    //关闭lcd文件，解除映射
    munmap(p_lcd, 800 * 480 * 4);
    close(lcd_fd);
}