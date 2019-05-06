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
    /* HJ-4WD-WIFI 外挂晶振频率为 11.0592Mhz <==> 11059200Hz */
    /* 定时器使用系统时钟的12分频作为计数频率，即 11059200/12 = 921600Hz*/
    /* 定时器需要计数 921600 个数才能得到 1s 时间 */
    /* 将 921600 分解为 61440*15，即定时器每次计数 61440 个数，计数 15 次 */
    /* 因此定时器计数初值为 2^16-61440 = 4096 = 0x1000 */
    TH0 = 0x10;
    TL0 = 0x00;

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

    /* 手动的装载初值 */
    TH0 = 0x10;
    TL0 = 0x00;

    timer_start();

    interrupt_count++;

    if(interrupt_count >= 15)
    {
        interrupt_count = 0;
        run();
    }
}
