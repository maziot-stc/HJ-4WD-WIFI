#include "reg52.h"
#include "main.h"
#include "bsp_delay.h"
#include "bsp_timer.h"

#define Segment P0
#define Digital P2

/* 数码管段选码、位选码 */
u8 SegCode[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};      // 段选低电平有效
u8 DigCode[4] = {0xfe, 0xfd, 0xfb, 0xf7};                                           // 位选低电平有效

/* 数码管显示的值 */
int value[4] = {0, 0, 0, 0};

int main(void)
{
    int i = 0;

    timer_init();
    timer_start();

    while(1)
    {
        for(i = 0; i < 4; i++)
        {
            Digital = DigCode[i];
            Segment = (1 == i) ? (SegCode[value[i]] & 0x7f) : SegCode[value[i]];
            delay_ms(1);
        }
    }
}

/**
 * @Description 定时器0中断回调函数
 */
void run(void)
{
    if(value[3] < 9)
    {
        value[3]++;
    }
    else
    {
        value[3] = 0;
        if(value[2] < 5)
        {
            value[2]++;
        }
        else
        {
            value[2] = 0;
            if(value[1] < 9)
            {
                value[1]++;
            }
            else 
            {
                value[1] = 0;
                if(value[0] < 5)
                {
                    value[0]++;
                }
                else
                {
                    value[0] = 0;
                    value[1] = 0;
                    value[2] = 0;
                    value[3] = 0;
                }
            }
        }
    }
}
