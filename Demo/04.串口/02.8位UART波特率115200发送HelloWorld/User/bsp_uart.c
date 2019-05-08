#include "bsp_uart.h"
#include "reg52.h"

/**
 * @Description 串口简单初始化函数。波特率设置为 115200
 */
void uart_init(void)
{
    /* 定时器1工作在方式2: 8位自动重装载模式 */
    TMOD = 0x20;

    /* 定时器1计数初值和重装载值均设置为 0xff = 255 */
    TH1 = 0xff;
    TL1 = 0xff;

    /* 本实验的目的是要让串口工作在8bitUART模式 */
    /* PCON[7]   = 1b  波特率加倍 SMOD = 1 */
    /* PCON[6]   = 0b  不启用帧错误检测 */
    PCON = 0x80;

    /* SCON[7:6] = 01b UART工作在方式1，波特率可变 */
    /* SCON[5]   = 0b  不允许多主机通信 */
    /* SCON[1]   = 0b  清除发送完成标志 */
    SCON = 0x40;

    /* 备注: 上述 SCON 和 PCON 单路列举出来的寄存器必须配置上述值 */
    /* 备注: 波特率计算方法 */
    /*        Baud = 2^SMOD / 32 * (SYSCLK / 12 / (256 - TH1)) */
    /*        Baud = 2^1 / 32 * (11059200 / 12 / (256 - 255)) = 57600 */
    /*        即使将可配置的所有参数均配置为最优状态，也无法达到 115200 的波特率 */
    /*        十分庆幸的是 STC89C52RC 是增强型的单片机，支持 6T 模式 */
    /*        Baud = 2^1 / 32 * (11059200 / 6 / (256 - 255)) = 115200 */
    /*        下载时需要手动指定工作模式为 6T 模式，并且下载成功必须重启一次，6T工作模式才会生效 */

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
 * @Description 串口发送字符串
 */
void uart_send_string(u8 *str)
{
    while((u8)*str != (u8)'\0')
    {
        uart_send_char((u8)*str);
        str++;
    }
}
