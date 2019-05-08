#ifndef __BSP_LCD1602_H
#define __BSP_LCD1602_H

#include "reg52.h"
#include "main.h"

void lcd1602_write_command(u8 cmd);
void lcd1602_write_data(u8 dat);
void lcd1602_init(void);
void lcd1602_display_string(u8 x, u8 y, char* str);

#endif /* __BSP_LCD1602_H */
