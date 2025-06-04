#ifndef __BSP__H__
#define __BSP__H__

#include "main.h"


//-------------------- LEDS GPIO-------------
#define LED1                    DO_LED1_GPIO_Port,DO_LED1_Pin
#define LED2                    DO_LED2_GPIO_Port,DO_LED2_Pin

#define BSP_LED_ON(LED)			HAL_GPIO_WritePin(LED, GPIO_PIN_SET)
#define BSP_LED_OFF(LED)	    HAL_GPIO_WritePin(LED, GPIO_PIN_RESET)
#define BSP_LED_TOGGLE(LED)		HAL_GPIO_TogglePin(LED)
// //-------------------- LEDS GPIO END-------------

// ----------------------------- RS-485 -----------------------------

void bsp_rs485_setPortToModbusRtu(uint8_t portNo, uint8_t *bufRxTX, uint16_t bufSizeByte);

void bsp_rs485_sendBlock(uint8_t portNo, uint8_t *buf, uint8_t bufSizeByte);
void bsp_rs485_sendTestBlock(uint8_t portNo);

void bsp_rs485_callback_rxBlockReady(uint8_t portNo);
void bsp_rs485_callback_rxTimeout(uint8_t portNo);

#define BSP_RS485_1_IRQ_HANDLER 			        USART1_IRQHandler
// #define BSP_RS485_1_IRQ_HANDLER_DMA_RX 			DMA1_Channel3_IRQHandler
// #define BSP_RS485_1_IRQ_HANDLER_DMA_TX 			DMA1_Channel4_IRQHandler
// ----------------------------- RS-485 END-----------------------------


#define MAX_BUF_I2C (20)
typedef struct
{
    I2C_HandleTypeDef *i2c;
    uint8_t address;
    uint8_t buf[MAX_BUF_I2C];
} i2c_conf_typedef;


void bsp_i2c_write_one_reg(I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_write);
void bsp_i2c_read_one_reg (I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_read);
void bsp_i2c_read_few_reg (I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_read, uint8_t size);


#endif