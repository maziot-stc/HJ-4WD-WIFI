#include "reg52.h"
#include "main.h"
#include "bsp_delay.h"

#define Segment P0
#define Digital P2

/* 数码管段选码、位选码 */
u8 SegCode[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};      // 段选低电平有效
u8 DigCode[4] = {0xfe, 0xfd, 0xfb, 0xf7};                                           // 位选低电平有效

/* 数码管流水灯段选码、位选码 */
u8 LightSegCode[12] = {0xfe, 0xfe, 0xfe, 0xfe, \
                       0xfd, 0xfb, 0xf7, 0xf7, \
                       0xf7, 0xf7, 0xef, 0xdf};
u8 LightDigCode[12] = {0xfe, 0xfd, 0xfb, 0xf7,
                       0xf7, 0xf7, 0xf7, 0xfb, \
                       0xfd, 0xfe, 0xfe, 0xfe};

int value[4] = {0xff, 0xff, 0xff, 0xff};

int main(void)
{
    int i = 0;
    int count = 0;                      /* 数码管动态扫描次数计数 */
    int index = 0;                      /* 流水灯分为12个小灯, 流水灯计数 */

    while(1)
    {
        for(i = 0; i < 4; i++)
        {
            /* 数码管动态显示 */
            Digital = DigCode[i];
            Segment = value[i];
            delay_ms(1);
        }
        count++;

        /* 修改数码管显示的值 */
        if(count > 50)
        {
            if(index >= 12)
            {
                for(i = 0; i < 4; i++)
                {
                    value[i] = 0xff;
                }
                index = 0;
            }

            /* 判断当前流水灯是哪个位选上的，将该位需要点亮的位置写入value数组中 */
            for(i = 0; i < 4; i++)
            {
                if(LightDigCode[index] == DigCode[i])
                {
                    value[i] = value[i] & LightSegCode[index];
                }
            }
            index++;
            count = 0;
        }
    }
}
