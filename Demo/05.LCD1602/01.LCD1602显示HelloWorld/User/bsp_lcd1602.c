#include "bsp_lcd1602.h"
#include "bsp_delay.h"

sbit LCD1602_RS = P2^3;
sbit LCD1602_RW = P2^4;
sbit LCD1602_EN = P2^5;

#define LCD1602_DATA P0

/**
 * @Description LCD1602 写命令
 * @param cmd 8位命令值
 */
void lcd1602_write_command(u8 cmd)
{
    LCD1602_EN = 0;
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DATA = cmd;
    LCD1602_EN = 1;
    delay_ms(1);
    LCD1602_EN = 0;
}

/**
 * @Description LCD1602 写数据
 * @param dat 8位数据值
 */
void lcd1602_write_data(u8 dat)
{
    LCD1602_EN = 0;
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DATA = dat;
    LCD1602_EN = 1;
    delay_ms(1);
    LCD1602_EN = 0;
}

/**
 * @Description LCD1602 初始化函数
 */
void lcd1602_init(void)
{
    lcd1602_write_command(0x38);
    lcd1602_write_command(0x0c);
    lcd1602_write_command(0x06);
    lcd1602_write_command(0x01);
    lcd1602_write_command(0x80);
}

/**
 * @Description LCD1602 显示字符串
 * @param x 行号，取值范围为0-1
 * @param x 列号，取值范围为0-15
 */
void lcd1602_display_string(u8 x, u8 y, char* str)
{
    if(x == 0)
    {
        lcd1602_write_command(0x80 | y);
    }
    else if(x == 1)
    {
        lcd1602_write_command(0xc0 | y);
    }
    while(*str!='\0')
    {
        lcd1602_write_data(*str);
        str++;
    }
}
