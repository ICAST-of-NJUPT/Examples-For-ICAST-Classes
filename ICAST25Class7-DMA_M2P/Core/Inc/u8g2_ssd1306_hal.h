#ifndef U8G2_PORT_H
#define U8G2_PORT_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"
#include "u8g2.h"

#define I2C_DMA_BUF_SIZE 256
#define I2C_TIMEOUT 100 // ms

uint8_t port_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t port_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr); // 仅在软件模拟时，替换dummy

void u8g2_init_ssd1306_i2c(u8g2_t *u8g2, I2C_HandleTypeDef *hi2c);

#endif
