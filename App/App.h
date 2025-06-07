#ifndef __APP_H__
#define __APP_H__

#include "main.h"

#include "BSP.h"
#include "LSM6DS3TR.h"
#include "MPU6050.h"
#include "Flash_driver.h"

void app_main();

#define ACCELEROMETR_COUNT (2)
#define ACCELEROMETR_AXIS  (3)
typedef enum
{
    LSM6DS3TR_aX = 0,
    LSM6DS3TR_aY,
    LSM6DS3TR_aZ,
    MPU6050_aX,
    MPU6050_aY,
    MPU6050_aZ
} Acc_data_enum;

typedef struct
{
    uint8_t MPU6050_acc_scale;
    uint8_t LSM6DS3TR_acc_scale;

    uint8_t MPU6050_freq;
    uint8_t LSM6DS3TR_freq;

    uint8_t order[ACCELEROMETR_COUNT*ACCELEROMETR_AXIS];
    uint8_t filterN[ACCELEROMETR_COUNT*ACCELEROMETR_AXIS];
} SetupParam_typedef;

#define ACCELEROMETR_MAX_FILTER_ORDER (10)
typedef struct {
    float value;
    float value_last;
    float valueRaw;
    float valueSours;
    float buf[ACCELEROMETR_MAX_FILTER_ORDER];
    uint8_t bufIdx;
    uint8_t filter_N;
    uint8_t order;
}Acceleromentr_filter_typedef;

typedef struct
{
    SetupParam_typedef SetupParam;
    int16_t acc_data[ACCELEROMETR_COUNT*ACCELEROMETR_AXIS]; //[g*1000]
    Acceleromentr_filter_typedef acc_filter[ACCELEROMETR_COUNT*ACCELEROMETR_AXIS];
} app_typedef;

void app_main();

void app_SetupParam_init();

void app_acc_filter_init();

void app_SetupParam_set_to_defolt();

void app_get_accelerometr_data_LSM6DS3TR();

void app_get_accelerometr_data_MPU6050();

void app_accelerometr_data_filter();

void app_flash_load();

void app_flash_save();

void app_system_reset();

#endif