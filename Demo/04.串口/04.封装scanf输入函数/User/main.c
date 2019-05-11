#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"
#include "stdio.h"

char str[64];

int main(void)
{
    uart_init();
    while(1)
    {
        scanf("%s", str);
        printf("rec: %s\r\n", str);
    }
}
