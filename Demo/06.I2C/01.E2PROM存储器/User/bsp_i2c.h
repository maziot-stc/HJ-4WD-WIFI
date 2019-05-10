#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "main.h"
#include "reg52.h"

void i2c_init(void);
void i2c_bus_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_response(void);
void i2c_no_response(void);
void i2c_send_byte(u8 dat);
int i2c_wait_response(void);
u8 i2c_receive_byte(void);

#endif /* __BSP_I2C_H */
