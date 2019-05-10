#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"
#include "stdio.h"

int main(void)
{
    uart_init();

    // 使用C51的printf()函数打印%d/i/u/o/x/X格式时，必须要指定该变量的存储格式l/L/b/B
    printf("%c %d %s %bx Hello World!\r\n", 'H', 1000, "demo", 0x55);

    while(1);
}
