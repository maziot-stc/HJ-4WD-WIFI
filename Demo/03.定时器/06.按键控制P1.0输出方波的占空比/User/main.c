#include "reg52.h"
#include "main.h"
#include "bsp_delay.h"
#include "bsp_timer.h"

sbit pwm = P1^0;

int main(void)
{
    pwm = 0;
    timer_init();
    timer_start();

    while(1)
    {
        delay_ms(100);
    }
}

/**
 * @Description 定时器0中断回调函数
 */
void run(void)
{
    pwm = ~pwm;
}
