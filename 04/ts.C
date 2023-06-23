#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main()
{
    //打开触摸屏
    int fd_ts = open("/dev/input/event0", O_RDWR);
    if (fd_ts == -1)
    {
        perror("open ts error");
        return -1;
    }

    //读取触摸屏数据
    struct input_event ts_event;
    //定义触摸屏坐标
    int x, y = 0;
    while(1)
    {
        read(fd_ts, &ts_event, sizeof(ts_event));
        //判断触摸屏事件
        if(ts_event.type == EV_ABS && ts_event.code == ABS_X)
        {
            printf("x = %d\n", ts_event.value * 800 / 1024);
            x = ts_event.value * 800 / 1024;
        }
        if(ts_event.type == EV_ABS && ts_event.code == ABS_Y)
        {
            printf("y = %d\n", ts_event.value * 480 / 600);
            y = ts_event.value * 480 / 600;
        }
        //判断触摸屏坐标
        if(x < 400)
        {
            printf("left\n");
        }
        else if(x > 400)
        {
            printf("right\n");
        }
        else
        {
            printf("center\n");
        }

    }

    //关闭触摸屏
    close(fd_ts);

    return 0;

}