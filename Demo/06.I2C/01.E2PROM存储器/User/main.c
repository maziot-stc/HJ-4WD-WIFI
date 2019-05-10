#include "reg52.h"
#include "main.h"
#include "bsp_uart.h"
#include "bsp_at24c256.h"

int main(void)
{
    u8 buffer[16];
    int ret = 0;
    uart_init();

    buffer[0] = 0x5A;
    ret = at24c256_byte_write(0x1000, buffer);
    if(ret)
    {
        printf("byte write error!\r\n");
    }
    else
    {
        printf("write successful\r\n");
    }

    buffer[0] = 200;
    printf("data = %bu\r\n", (u8)buffer[0]);
    at24c256_random_read(0x1000, buffer);
    if(ret)
    {
        printf("byte read error!\r\n");
    }
    else
    {
        printf("data = %bd\r\n", (u8)buffer[0]);
    }

    while(1);
}
