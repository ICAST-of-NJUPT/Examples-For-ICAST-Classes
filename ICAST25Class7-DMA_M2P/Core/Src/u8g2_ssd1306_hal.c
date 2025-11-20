#include "u8g2_ssd1306_hal.h"
#include "stm32f401xc.h"
#include "stm32f4xx_hal.h"
#include "u8g2.h"
#include "u8x8.h"
#include <stdint.h>

I2C_HandleTypeDef *hdevice;
static uint8_t dma_buf[I2C_DMA_BUF_SIZE];

void u8g2_init_ssd1306_i2c(u8g2_t *u8g2, I2C_HandleTypeDef *hi2c)
{   
    hdevice = hi2c;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, port_byte, u8x8_dummy_cb);
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_boutique_bitmap_9x9_bold_t_chinese1);
    u8g2_ClearBuffer(u8g2);
    u8g2_SendBuffer(u8g2);
}

uint8_t port_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    assert_param(hdevice->Instance == I2C1 || hdevice->Instance == I2C2); // 这行是参数检查，凹性能时可以去掉
    static uint8_t datalen;
    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
        for (int i = 0; i < arg_int; i++) {
            (dma_buf + datalen)[i] = ((uint8_t *)arg_ptr)[i];
        }
        datalen += arg_int;
        break;
        case U8X8_MSG_BYTE_INIT:
        // MX_I2C1_Init(); // 也可以直接在程序一开始进行 I2C 初始化，但是需要将此行去掉或者注释掉
        break;
        case U8X8_MSG_BYTE_SET_DC:
        return 0; // I2C 不会涉及 DC
        case U8X8_MSG_BYTE_START_TRANSFER:
        datalen = 0;
        while (hdevice->State != HAL_I2C_STATE_READY) {HAL_Delay(1);}
        /* 中断和DMA模式支持 */
        break;
        case U8X8_MSG_BYTE_END_TRANSFER:
        /* 也可以使用中断模式或者DMA，自行修改 */
        return HAL_OK == HAL_I2C_Master_Transmit_DMA(hdevice, u8x8->i2c_address, dma_buf, datalen);
				break ;/* do nothing */
        default:
        return 0;
    }
    return 1;	/* always succeed */
}


uint8_t port_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    if (u8x8 && msg && arg_int && arg_ptr) {
        //nowarnning
    }
    assert_param(hdevice->Instance == I2C1 || hdevice->Instance == I2C2);
    return 0;
    // 使用软件模拟时，需要实现该函数，参考 https://github.com/olikraus/u8g2 仓库中sys文件夹下的例程
}

