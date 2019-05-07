#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"

int main(void)
{
    uart_init();

    uart_send_char((u8)'H');
    uart_send_char((u8)'e');
    uart_send_char((u8)'l');
    uart_send_char((u8)'l');
    uart_send_char((u8)'o');

    uart_send_char((u8)' ');

    uart_send_char((u8)'W');
    uart_send_char((u8)'o');
    uart_send_char((u8)'r');
    uart_send_char((u8)'l');
    uart_send_char((u8)'d');

    uart_send_char((u8)'\r');
    uart_send_char((u8)'\n');

    while(1);
}
