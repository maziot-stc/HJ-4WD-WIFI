#include "bsp_uart.h"
#include "reg52.h"

/**
 * @Description 串口简单初始化函数。波特率设置为 9600
 */
void uart_init(void)
{
    /* 这里需要详细的注释 */
    TMOD = 0x20;
    TH1 = 0xfd; 
    TL1 = 0xfd; 
    PCON = 0x00;

    SCON = 0x40;
    TI = 0;
    TR1 = 1;
}

/**
 * @Description 串口发送单个字符
 */
void uart_send_char(u8 ch)
{
    /* 将要发送的字符填入发送缓冲区 */
    SBUF = ch;

    /* 等待发送完成 */
    while(TI == 0);

    /* 清除发送标志 */
    TI = 0;
}
