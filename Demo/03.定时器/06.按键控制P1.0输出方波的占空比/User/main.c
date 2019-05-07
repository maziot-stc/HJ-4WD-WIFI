#include "reg52.h"
#include "main.h"
#include "bsp_delay.h"
#include "bsp_timer.h"
#include "bsp_key.h"

#define Segment P0
#define Digital P2

/* 数码管段选码、位选码 */
u8 SegCode[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};      // 段选低电平有效
u8 DigCode[4] = {0xfe, 0xfd, 0xfb, 0xf7};                                           // 位选低电平有效

/* 数码管显示的值 */
int value[4] = {0, 0, 5, 0};

/* PWM 输出的 GPIO 引脚 */
sbit pwm_pin = P1^0;

/* PWM 默认占空比设置为 50 */
int pwm_value = 50;

int main(void)
{
    int i = 0;
    pwm_pin = 0;

    timer_init();
    timer_start();

    while(1)
    {
        for(i = 0; i < 4; i++)
        {
            Digital = DigCode[i];
            Segment = (i < 2) ? 0xff: SegCode[value[i]];
            delay_ms(1);
            key_control(key_scan());
        }
    }
}

/**
 * @Description 定时器0中断回调函数
 */
void run(void)
{
    pwm_pin = ~pwm_pin;
}
