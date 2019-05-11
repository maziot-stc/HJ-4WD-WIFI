#include "bsp_uart.h"
#include "reg52.h"

/**
 * @Description 串口简单初始化函数。波特率设置为 9600
 */
void uart_init(void)
{
    /* 定时器1工作在方式2: 8位自动重装载模式 */
    TMOD = 0x20;

    /* 定时器1计数初值和重装载值均设置为 0xfd = 253 */
    TH1 = 0xfd;
    TL1 = 0xfd;

    /* 本实验的目的是要让串口工作在8bitUART模式 */
    /* PCON[7]   = 0b  波特率不加倍 SMOD = 0 */
    /* PCON[6]   = 0b  不启用帧错误检测 */
    PCON = 0x00;

    /* SCON[7:6] = 01b UART工作在方式1，波特率可变 */
    /* SCON[5]   = 0b  不允许多主机通信 */
    /* SCON[1]   = 0b  清除发送完成标志 */
    /* SCON[4]   = 1b  使能接收 */
    SCON = 0x50;

    /* 备注: 上述 SCON 和 PCON 单路列举出来的寄存器必须配置上述值 */
    /* 备注: 波特率计算方法 */
    /*        Baud = 2^SMOD / 32 * (SYSCLK / 12 / (256 - TH1)) */
    /*        Baud = 2^0 / 32 * (11059200 / 12 / (256 - 253)) = 9600 */

    /* 启用定时器1，为UART模块提供波特率时钟 */
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

/**
 * @Description 串口发送单个字符
 */
u8 uart_receive_char(void)
{
    /* 等待串口接收到数据 */
    while (RI == 0);

    /* 清除接收标志 */
    RI = 0;

    /* 返回接收到的字符 */
    return SBUF;
}

/**
 * @Description 实现 printf 底层 putchar 函数
 * @note 实现此函数后，便可以直接使用 printf 函数，需要包含 stdio.h
 */
char putchar(char c)
{
    uart_send_char(c);
    return c;
}

/**
 * @Description 实现 scanf 底层 _getkey 函数
 * @note 实现此函数后，便可以直接使用 scanf 函数，需要包含 stdio.h
 */
char _getkey(void)
{
    return uart_receive_char();
}
