#include "reg52.h"
#include "main.h"
#include "bsp_lcd1602.h"

int main(void)
{
    lcd1602_init();
    lcd1602_display_string(0, 0, "Hello World");
    while(1);
}
