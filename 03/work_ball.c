#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


int main(void)
{
    int x = 0, y = 0;
    int x0 = 400, y0 = 240, r = 50;
    int m_x = 0, m_y = 0;
    int lcd_fd;
    int color = 0;
	int *p;
        // 打开lcd屏
        lcd_fd = open("/dev/fb0", O_RDWR);
        if (lcd_fd == -1)
        {
            perror("lcd屏打开失败");
            return 1;
        }

        // 映射内存空间mmap
        p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
        if (p == (void *)-1)
        {
            perror("映射空间失败");
            close(lcd_fd);
            return 1;
        }

    unsigned int color_alter[] = {0x00ff0000, 0x000000ff};
    while (1)
    {
        // 写入条件
        for (y = 0; y < 480; y++)
        {
            for (x = 0; x < 800; x++)
            {
                if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= r * r)
                    *(p + y * 800 + x) = color_alter[color];
                else
                    *(p + y * 800 + x) =0xffffffff;
            }
        }
//0靠左 靠上
        if (m_x == 0)
            x0--;
		else
			x0++;
 
		if (m_y == 0)
            y0--;
		else
			y0++;
//碰到顶部
        if ((y0 - r) == 0)
        {
			m_y=1;
            color = color + 1;
            if (color == 2)
            {
                color = 0;
            }
        }

        if ((x0 - r) == 0)
        {
			m_x=1;

            color = color + 1;
            if (color == 2)
            {
                color = 0;
            }
        }
        if ((y0 + r) == 479)
        {
			m_y=0;

            color = color + 1;
            if (color == 2)
            {
                color = 0;
            }
        }

        if ((x0 + r) == 799)
        {
			m_x=0;

            color = color + 1;
            if (color == 2)
            {
                color = 0;
            }
        }
    }

    // 关闭文件，解除映射
    close(lcd_fd);
    munmap(p, 800 * 480 * 4);

    return 0;
}
