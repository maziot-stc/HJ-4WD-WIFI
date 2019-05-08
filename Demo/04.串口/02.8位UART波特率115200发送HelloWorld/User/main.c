#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"

int main(void)
{
    uart_init();
    uart_send_string("Hello World 115200\r\n");

    while(1);
}
