#ifndef __LSM6DS3TR_H__
#define __LSM6DS3TR_H__

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
            uint8_t INT1_DRDY_XL       : 1; // 0 bit
            uint8_t INT1_DRDY_G        : 1; // 1 bit
            uint8_t INT1_BOOT          : 1; // 2 bit 
            uint8_t INT1_FTH           : 1; // 3 bit
            uint8_t INT1_FIFO_OVR      : 1; // 4 bit
            uint8_t INT1_FULL_FLAG     : 1; // 5 bit
            uint8_t INT1_SIGN_MODE     : 1; // 6 bit
            uint8_t INT1_STEP_DETECTOR : 1; // 7 bit
        }bits;
    } reg_data;
} INT1_CNTRL_reg_typedef;

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t INT2_DRDY_XL       : 1; // 0 bit
            uint8_t INT2_DRDY_G        : 1; // 1 bit
            uint8_t INT2_DRDY_TEMP     : 1; // 2 bit
            uint8_t INT2_FTH           : 1; // 3 bit
            uint8_t INT2_FIFO_OVR      : 1; // 4 bit
            uint8_t INT2_FULL_FLAG     : 1; // 5 bit
            uint8_t INT2_STEP_COUNT_OV : 1; // 6 bit
            uint8_t INT2_STEP_DELTA    : 1; // 7 bit
        }bits;
    } reg_data;
} INT2_CNTRL_reg_typedef;

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t BW0_XL      : 1; // 0 bit
            uint8_t LPF1_BW_SEL : 1; // 1 bit
            uint8_t FS_XL0      : 1; // 2 bit
            uint8_t FS_XL1      : 1; // 3 bit
            uint8_t ODR_XL0     : 1; // 4 bit
            uint8_t ODR_XL1     : 1; // 5 bit
            uint8_t ODR_XL2     : 1; // 6 bit
            uint8_t ODR_XL3     : 1; // 7 bit
        }bits;
    } reg_data;
} CTRL1_XL_reg_typedef;

typedef struct
{
    uint8_t address;
    union
    {
        uint8_t w8;
        struct 
        {
            uint8_t LOW_PASS_ON_6D  : 1; // 0 bit
            uint8_t NC1             : 1; // 1 bit
            uint8_t HP_SLOPE_XL_EN  : 1; // 2 bit
            uint8_t INPUT_COMPOSITE : 1; // 3 bit
            uint8_t HP_REF_MODE     : 1; // 4 bit
            uint8_t HPCH_XL0        : 1; // 5 bit
            uint8_t HPCH_XL1        : 1; // 6 bit
            uint8_t LPF2_XL_EN      : 1; // 7 bit
        }bits;
    } reg_data;
} CTRL8_XL_reg_typedef;

typedef struct
{
    CTRL1_XL_reg_typedef   CTRL1_XL;
    CTRL8_XL_reg_typedef   CTRL8_XL;
    INT1_CNTRL_reg_typedef INT1_CTRL;
    INT2_CNTRL_reg_typedef INT2_CTRL;
} reg_conf_LSM6DS3TR_typedef;

typedef struct
{
    uint8_t OUTX_L_XL;
    uint8_t OUTX_H_XL;
    uint8_t OUTY_L_XL;
    uint8_t OUTY_H_XL;
    uint8_t OUTZ_L_XL;
    uint8_t OUTZ_H_XL;
} reg_data_address_LSM6DS3TR_typedef;

typedef struct
{
    float accelerometr_scale_k;
    reg_conf_LSM6DS3TR_typedef reg_conf;
    i2c_conf_typedef i2c_conf;
    reg_data_address_LSM6DS3TR_typedef reg_data_address;
} LSM6DS3TR_typedef;


void LSM6DS3TR_init_struct();
void LSM6DS3TR_config();

void LSM6DS3TR_i2c_write_reg(uint8_t memory_adr, uint8_t data);
void LSM6DS3TR_i2c_read_reg(uint8_t memory_adr, uint8_t *buf_read, uint8_t size);


#endif