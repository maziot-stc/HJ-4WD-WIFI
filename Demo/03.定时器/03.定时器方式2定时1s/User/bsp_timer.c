#include "bsp_timer.h"
#include "reg52.h"

static interrupt_count = 0;
extern void run(void);

/**
 * @Description 定时器0简单初始化函数
 */
void timer_init(void)
{
    /* 定时器0工作在方式2: 8位自动重装载模式 */
    /* 定时器中断后将 TH0 中的值自动装载到 TL0 中 */
    TMOD = 0x02;

    /* 方式0: 8位自动重装载模式，最大计数值 2^8 = 256 */
    /* HJ-4WD-WIFI 外挂晶振频率为 11.0592Mhz <==> 11059200Hz */
    /* 定时器使用系统时钟的12分频作为计数频率，即 11059200/12 = 921600Hz*/
    /* 定时器需要计数 921600 个数才能得到 1s 时间 */
    /* 将 921600 分解为 256*3600，即定时器每次计数 256 个数，计数 3600 次 */
    /* 因此定时器计数初值为 2^8-256 = 0 = 0x00 */
    TL0 = 0x00;

    /* 设置重装载值也是 0x00 */
    TH0 = 0x00;

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
    interrupt_count++;

    if(interrupt_count >= 3600)
    {
        interrupt_count = 0;
        run();
    }
}
