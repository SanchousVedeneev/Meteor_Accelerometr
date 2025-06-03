#include "App.h"

#include "i2c.h"
// #include "usart.h"
#define MPU6050_ADDRESS (0x68 << 1) // смещать на 1 бит влево обязательно!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

uint8_t buf_i2c[30] = {0};
uint16_t adress_i2c = 0x43;
uint16_t size_i2c_recive = 2;
uint32_t timeout_i2c = 10000;
uint8_t data_test[2] = {0x19, 0x07};

#include "ProtocolMbRtuSlaveCtrl.h"

void app_main()
{
    protocolMbRtuSlaveCtrl_init(1);
    // mpu6050_init();
    while (1)
    {
        // if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
        // {
        //     BSP_LED_ON(BSP_LED_1);
        // }
        // else
        // {
        //     BSP_LED_OFF(BSP_LED_1);
        // }
        // BSP_LED_TOGGLE(BSP_LED_1);
        HAL_Delay(100);
        protocolMbRtuSlaveCtrl_update_tables();
        // BSP_LED_TOGGLE(LED2);
        bsp_rs485_sendTestBlock(1);
        if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS, adress_i2c, 1, &buf_i2c[0], 2, timeout_i2c) == HAL_OK)
        {
            //BSP_LED_TOGGLE(BSP_LED_1);
            //BSP_LED_ON(BSP_LED_1);
        }
        else
        {
            //BSP_LED_OFF(BSP_LED_1);
        }

    }

}


#define PWR_MGMT_1_REG (0x6B)
#define SMPLRT_DIV_REG (0x19)
#define ACCEL_CONFIG_REG (0x1B)
#define GYRO_CONFIG_REG (0x1C)
#define INT_ENABLE_REG (0x38)

void mpu6050_init()
{
    uint8_t data;

    data = 0x00;

    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, PWR_MGMT_1_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, timeout_i2c) == HAL_OK)
    {
        asm("NOP");
    }
    else
    {
        asm("NOP");
    }

    HAL_Delay(10);

    data = 0x07;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, SMPLRT_DIV_REG, 1, &data, 1, timeout_i2c) == HAL_OK)
    {
        asm("NOP");
    }
    else
    {
        asm("NOP");
    }
    HAL_Delay(10);
    data = 0x18;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, ACCEL_CONFIG_REG, 1, &data, 1, timeout_i2c) == HAL_OK)
    {
        asm("NOP");
    }
    else
    {
        asm("NOP");
    }
    HAL_Delay(10);

    data = 0x18;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, GYRO_CONFIG_REG, 1, &data, 1, timeout_i2c) == HAL_OK)
    {
        asm("NOP");
    }
    else
    {
        asm("NOP");
    }
    HAL_Delay(10);
    data = 0x1;
    if  (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, INT_ENABLE_REG, 1, &data, 1, timeout_i2c) == HAL_OK)
    {
        asm("NOP");
    }
    else
    {
        asm("NOP");
    }
}




//#define BLINK_PERIOD_RS485
// void bsp_sys_tick_1k_callback()
// {

//     //static int led485counter = 0;
//     protocolMbRtuSlaveCtrl_update_tables();
//     //BSP_LED_TOGGLE(BSP_LED_RS485);
//     asm("NOP");
// }

