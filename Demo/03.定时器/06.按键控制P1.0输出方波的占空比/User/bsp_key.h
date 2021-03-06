#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "reg52.h"
#include "main.h"

#define GPIO_KEY        P2
#define GPIO_KEY_MASK   0xf0

u8 key_scan(void);
void key_control(u8);

#endif /* __BSP_KEY_H */
