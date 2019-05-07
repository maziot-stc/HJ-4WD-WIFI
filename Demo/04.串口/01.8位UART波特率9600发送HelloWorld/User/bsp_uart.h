#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

void uart_init(void);
void uart_send_char(u8 ch);

#endif /* __BSP_UART_H */
