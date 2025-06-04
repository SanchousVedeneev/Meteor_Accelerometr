#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "main.h"
#include "BSP.h"

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t SMPLRT_DIV_0  : 1; // 0 bit
            uint8_t SMPLRT_DIV_1  : 1; // 1 bit
            uint8_t SMPLRT_DIV_2  : 1; // 2 bit
            uint8_t SMPLRT_DIV_3  : 1; // 3 bit
            uint8_t SMPLRT_DIV_4  : 1; // 4 bit
            uint8_t SMPLRT_DIV_5  : 1; // 5 bit
            uint8_t SMPLRT_DIV_6  : 1; // 6 bit
            uint8_t SMPLRT_DIV_7  : 1; // 7 bit
        }bits;
    } reg_data;
} SMPRT_DIV_reg_typedef;

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t DLPF_CFG_0     : 1; // 0 bit
            uint8_t DLPF_CFG_1     : 1; // 1 bit
            uint8_t DLPF_CFG_2     : 1; // 2 bit
            uint8_t EXT_SYNC_SET_0 : 1; // 3 bit
            uint8_t EXT_SYNC_SET_1 : 1; // 4 bit
            uint8_t EXT_SYNC_SET_2 : 1; // 5 bit
            uint8_t NC_6           : 1; // 6 bit
            uint8_t NC_7           : 1; // 7 bit
        }bits;
    } reg_data;
} CONFIG_reg_typedef;

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t NC0         : 1; // 0 bit
            uint8_t NC1         : 1; // 1 bit
            uint8_t NC2         : 1; // 2 bit
            uint8_t AFS_SEL0    : 1; // 3 bit
            uint8_t AFS_SEL1    : 1; // 4 bit
            uint8_t ZA_ST       : 1; // 5 bit
            uint8_t YA_ST       : 1; // 6 bit
            uint8_t XA_ST       : 1; // 7 bit
        }bits;
    } reg_data;
} ACCEL_CONFIG_reg_typedef;

typedef struct
{
    SMPRT_DIV_reg_typedef SMPRT_DIV;
    CONFIG_reg_typedef CONFIG;
    ACCEL_CONFIG_reg_typedef ACCEL_CONFIG;
} reg_conf_typedef;


typedef struct
{
    uint8_t ACCEL_XOUT_H;
    uint8_t ACCEL_XOUT_L;
    uint8_t ACCEL_YOUT_H;
    uint8_t ACCEL_YOUT_L;
    uint8_t ACCEL_ZOUT_H;
    uint8_t ACCEL_ZOUT_L;
} reg_data_address_MPU6050_typedef;

typedef struct
{
    float accelerometr_scale_k;
    reg_conf_typedef reg_conf;
    i2c_conf_typedef i2c_conf;
    reg_data_address_MPU6050_typedef reg_data_address;
} MPU6050_typedef;


void MPU6050_init_struct();
void MPU6050_config();

void MPU6050_i2c_write_reg(uint8_t memory_adr, uint8_t data);
void MPU6050_i2c_read_reg(uint8_t memory_adr, uint8_t *buf_read, uint8_t size);

#endif