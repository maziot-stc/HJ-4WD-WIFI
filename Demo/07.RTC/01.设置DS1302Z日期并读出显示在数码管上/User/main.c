#include "reg52.h"
#include "main.h"
#include "bsp_delay.h"
#include "bsp_ds1302z.h"

/*
typedef struct __DS1302_DATE_TIME__
{
  unsigned char year;
  unsigned char week;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} ds1302_Datetime;
*/

ds1302_Datetime datetime;

#define Segment P0
#define Digital P2

/* 数码管段选码、位选码 */
u8 SegCode[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};      // 段选低电平有效
u8 DigCode[4] = {0xfe, 0xfd, 0xfb, 0xf7};                                           // 位选低电平有效

/* 数码管显示的值 */
int value[4] = {1, 2, 3, 4};

int main(void)
{
    int i = 0;

    datetime.year = 2020;
    datetime.week = 1;
    datetime.month = 8;
    datetime.day = 28;
    datetime.hour = 01;
    datetime.minute = 33;
    datetime.second = 00;
    ds1302_set_datetime(&datetime);

    value[0] = datetime.month / 10;
    value[1] = datetime.month % 10;
    value[2] = datetime.day / 10;
    value[3] = datetime.day % 10;

    while(1)
    {
        for(i = 0; i < 4; i++)
        {
            Digital = DigCode[i];
            Segment = SegCode[value[i]];
            delay_ms(1);
        }
    }
}
