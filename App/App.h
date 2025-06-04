#ifndef __APP_H__
#define __APP_H__

#include "main.h"

#include "BSP.h"
#include "LSM6DS3TR.h"
#include "MPU6050.h"

void app_main();

typedef struct
{
    uint8_t MPU6050_accelerometr_scale;
    uint8_t LSM6DS3TR_accelerometr_scale;
} SetupParam_typedef;

typedef struct
{
    /* @brief information
    ** acceleronetr data [g*1000]
    */
    int16_t MPU6050_aX; 
    int16_t MPU6050_aY;
    int16_t MPU6050_aZ;

    int16_t LSM6DS3TR_aX;
    int16_t LSM6DS3TR_aY;
    int16_t LSM6DS3TR_aZ;
} Acceleromentr_data_typedef;

typedef struct
{
    SetupParam_typedef SetupParam;
    Acceleromentr_data_typedef acc_data;
} app_typedef;

void app_main();

void app_SetupParam_init();

void app_get_accelerometr_data_LSM6DS3TR();

void app_get_accelerometr_data_MPU6050();






#endif