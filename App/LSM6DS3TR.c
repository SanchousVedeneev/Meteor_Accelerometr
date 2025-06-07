#include "LSM6DS3TR.h"
#include "i2c.h"
#include "BSP.h"
#include "App.h"

LSM6DS3TR_typedef LSM6DS3TR;

extern app_typedef App;

#define BIT_RESET (0)
#define BIT_SET   (1)
void LSM6DS3TR_init_struct()
{
    //------------------------ INIT DEVICE ------------------------//
    LSM6DS3TR.i2c_conf.i2c     = &hi2c1;
    LSM6DS3TR.i2c_conf.address = (0x6B << 1);
    for(uint8_t i = 0; i < MAX_BUF_I2C; i++)
    {
        LSM6DS3TR.i2c_conf.buf[i] = 0;
    }
    //---------------------- INIT DEVICE END ----------------------//

    //-------------------- reg_data_address --------------------//
    LSM6DS3TR.reg_data_address.OUTX_L_XL = 0x28;
    LSM6DS3TR.reg_data_address.OUTX_H_XL = 0x29;
    LSM6DS3TR.reg_data_address.OUTY_L_XL = 0x2A;
    LSM6DS3TR.reg_data_address.OUTY_H_XL = 0x2B; 
    LSM6DS3TR.reg_data_address.OUTZ_L_XL = 0x2C;
    LSM6DS3TR.reg_data_address.OUTZ_H_XL = 0x2D; 
    //------------------ reg_data_address END ------------------//
    
    //------------------------ CTRL1_XL ------------------------//
    /* @brief information
    ** ODR_XL3...ODR_XL0 - Output data rate and power mode selection. Default value: 0000
    **      0000 - Power-down
    **      1011 - 12,5 Hz (high performance) 
    **      0001 - 12.5 Hz (high performance)
    **      0010 - 26   Hz (high performance)
    **      0011 - 52   Hz (high performance)
    **      0100 - 104  Hz (high performance)
    **      0101 - 208  Hz (high performance)
    **      0110 - 416  Hz (high performance)
    **      0111 - 833  Hz (high performance)
    **      1000 - 1660 Hz (high performance)
    **      1001 - 3330 Hz (high performance)
    **      1010 - 6660 Hz (high performance)
    **      10XX - Not allowed
    ** FS_XL1...FS_XL0 - Accelerometer full-scale selection. Default value: 00
    **      00 - +-  2g
    **      01 - +- 16g
    **      10 - +-  4g
    **      11 - +-  8g
    ** LPF1_BW_SEL - Accelerometer digital LPF (LPF1) bandwidth selection
    ** BW0_XL - Accelerometer analog chain bandwidth selection
    ** (only for accelerometer ODR ≥ 1.67 kHz)
    **      0 - BW @ 1.5 kHz
    **      1 - BW @ 400 Hz
    */
    LSM6DS3TR.reg_conf.CTRL1_XL.address = 0x10;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.BW0_XL      = BIT_SET;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.LPF1_BW_SEL = BIT_RESET;

    if (App.SetupParam.LSM6DS3TR_acc_scale == 2)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 4)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 8)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 16)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;        
    }
    else 
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;
    }

    if (App.SetupParam.LSM6DS3TR_freq == 1)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 2)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 3)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 4)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 5)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 6)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 7)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 8)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 9)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_freq == 10)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_SET;
    }
    else 
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3 = BIT_RESET;
    }
    //---------------------- CTRL1_XL END ----------------------//

    //------------------------ CTRL8_XL ------------------------//
     /* @brief information
    ** IN DATASH
    */
    LSM6DS3TR.reg_conf.CTRL8_XL.address = 0x17;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.LOW_PASS_ON_6D   = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.NC1              = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.HP_SLOPE_XL_EN   = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.INPUT_COMPOSITE  = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.HP_REF_MODE      = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.HPCH_XL0         = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.HPCH_XL1         = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.bits.LPF2_XL_EN       = BIT_RESET;
    //---------------------- CTRL8_XL END ----------------------//

    asm("Nop");

    //---------------------- accelerometr_scale_k ----------------------//
    if (App.SetupParam.LSM6DS3TR_acc_scale == 2)
    {
        LSM6DS3TR.accelerometr_scale_k = 16.384f;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 4)
    {
        LSM6DS3TR.accelerometr_scale_k = 8.192f;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 8)
    {
        LSM6DS3TR.accelerometr_scale_k = 4.096f;
    }
    else if (App.SetupParam.LSM6DS3TR_acc_scale == 16)
    {
        LSM6DS3TR.accelerometr_scale_k = 2.048f;
    }
    else 
    {
        LSM6DS3TR.accelerometr_scale_k = 16.384f;
    }
    //-------------------- accelerometr_scale_k END --------------------//
    return;
}

void LSM6DS3TR_config()
{
    LSM6DS3TR_i2c_write_reg(LSM6DS3TR.reg_conf.CTRL1_XL.address, LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.w8);
    LSM6DS3TR_i2c_write_reg(LSM6DS3TR.reg_conf.CTRL8_XL.address, LSM6DS3TR.reg_conf.CTRL8_XL.reg_data.w8);
    return;
}

void LSM6DS3TR_i2c_write_reg(uint8_t memory_adr, uint8_t data)
{
    bsp_i2c_write_one_reg(LSM6DS3TR.i2c_conf.i2c, LSM6DS3TR.i2c_conf.address, memory_adr, &data);
    return;
}

void LSM6DS3TR_i2c_read_reg(uint8_t memory_adr, uint8_t *buf_read, uint8_t size)
{
    if (size == 1)
    {
        bsp_i2c_read_one_reg(LSM6DS3TR.i2c_conf.i2c, LSM6DS3TR.i2c_conf.address, memory_adr, buf_read);
    }
    else
    {
        bsp_i2c_read_few_reg(LSM6DS3TR.i2c_conf.i2c, LSM6DS3TR.i2c_conf.address, memory_adr, buf_read, size);
    }
    return;
}