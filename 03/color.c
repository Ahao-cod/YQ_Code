#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
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
    int red = 0x00ff0000;
    int green = 0x0000ff00;
    int blue = 0x000000ff;
    int black = 0x00000000;
    int yellow = 0x00ffff00;
    int white = 0x00ffffff;
    int x, y;
    //在屏幕中间画一个黄色实心五角星
    for (y = 0; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            if ((y - 240) * (y - 240) + (x - 400) * (x - 400) <= 100 * 100)
            {
                *(p_lcd + y * 800 + x) = yellow;
            }
            else if ((y - 240) * (y - 240) + (x - 400) * (x - 400) <= 200 * 200)
            {
                *(p_lcd + y * 800 + x) = white;
            }
            else
            {
                *(p_lcd + y * 800 + x) = black;
            }
        }
    }


    munmap(p_lcd, 800 * 480 * 4);
    close(fd_lcd);

    return 0;
}