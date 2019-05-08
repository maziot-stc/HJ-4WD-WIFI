#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"
#include "stdio.h"

int main(void)
{
    uart_init();

    /* 验证下来发现 %x 无法正确打印，其他格式没有问题 */
    /* 打印结果为: H 1000 demo 5500 Hello World! */
    printf("%c %d %s %x Hello World!\r\n", 'H', 1000, "demo", 0x55);

    while(1);
}
