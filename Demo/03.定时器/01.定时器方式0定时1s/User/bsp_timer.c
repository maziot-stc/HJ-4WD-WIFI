#include "bsp_timer.h"
#include "reg52.h"

static u8 interrupt_count = 0;
extern void run(void);

/**
 * @Description 定时器0简单初始化函数
 */
void timer_init(void)
{
    /* 定时器0工作在方式0: 13位定时计数模式 */
    /* 定时器13位组成，TL0低5位，TH08位，其中TL0低5位溢出向TH0进位 */
    TMOD = 0x00;

    /* 方式0: 13位定时计数器，最大计数值 2^13 = 8192 */
    /* HJ-4WD-WIFI 外挂晶振频率为 11.0592Mhz <==> 11059200Hz */
    /* 定时器使用系统时钟的12分频作为计数频率，即 11059200/12 = 921600Hz*/
    /* 定时器需要计数 921600 个数才能得到 1s 时间 */
    /* 将 921600 分解为 7680*120，即定时器每次计数 7680 个数，计数 120 次 */
    /* 因此定时器计数初值为 2^13-7680 = 512 = 0x200 */
    /* 将 0x200 填入 TH0[7:0] 和 TL0[4:0] 中 */
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

    if(interrupt_count >= 120)
    {
        interrupt_count = 0;
        run();
    }
}
