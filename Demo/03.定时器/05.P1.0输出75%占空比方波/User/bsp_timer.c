#include "bsp_timer.h"
#include "reg52.h"

static u8 interrupt_count = 0;
extern void run(void);

/**
 * @Description 定时器0简单初始化函数
 */
void timer_init(void)
{
    /* 定时器0工作在方式1: 16位定时计数模式 */
    TMOD = 0x01;

    /* 方式1: 16位定时计数器，最大计数值 2^16 = 65536 */
    /* 想要输出占空比为 75% 的方波，可以将 10000 个计数作为一个周期 */
    /* 7500 个计数输出高电平，2500 个计数输出低电平，即可得到 75% 占空比的方波 */
    /* 7500 个计数对应计数初值为 2^16 - 7500 = 58036 = 0xe2b4 */
    /* 2500 个计数对应计数初值为 2^16 - 2500 = 63036 = 0xf63c */

    /* HJ-4WD-WIFI 外挂晶振频率为 11.0592Mhz <==> 11059200Hz */
    /* 定时器使用系统时钟的12分频作为计数频率，即 11059200/12 = 921600Hz*/
    /* 定时器需要计数 921600 个数才能得到 1s 时间 */
    /* 因此计数 10000 次耗时: 1000ms / 921600 * 10000 = 10.85ms */
    /* 计数 7500 次耗时: 1000ms / 921600 * 7500 = 8.14ms */
    /* 计数 2500 次耗时: 1000ms / 921600 * 2500 = 2.71ms */
    TH0 = 0xf6;
    TL0 = 0x3c;

    /* 使能总中断和定时器0中断 */
    IE = 0x82;
}

/**
 * @Description 使能定时器0
 */
void timer_start(void)
{
    TR0 = 1;
}

/**
 * @Description 关闭定时器0
 */
void timer_stop(void)
{
    TR0 = 0;
}

/**
 * @Description 定时器0中断服务函数
 */
void timer0_handler() interrupt 1
{
    timer_stop();

    interrupt_count++;

    /* 手动的装载初值 */
    if(1 == interrupt_count)
    {
        TH0 = 0xe2;
        TL0 = 0xb4;
    }
    else if(interrupt_count >= 2)
    {
        TH0 = 0xf6;
        TL0 = 0x3c;
        interrupt_count = 0;
    }

    timer_start();

    run();
}
