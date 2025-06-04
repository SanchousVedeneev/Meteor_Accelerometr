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
    LSM6DS3TR.i2c_conf.i2c = &hi2c1;
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
    LSM6DS3TR.reg_conf.CTRL1_XL.address = 0x10;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.BW0_XL      = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.LPF1_BW_SEL = BIT_SET;

    if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 2)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 4)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 8)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_SET;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 16)
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_SET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;        
    }
    else 
    {
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL0 = BIT_RESET;
        LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.FS_XL1 = BIT_RESET;
    }

    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL0     = BIT_SET;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL1     = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL2     = BIT_RESET;
    LSM6DS3TR.reg_conf.CTRL1_XL.reg_data.bits.ODR_XL3     = BIT_RESET;
    //---------------------- CTRL1_XL END ----------------------//
    asm("Nop");

    //---------------------- accelerometr_scale_k ----------------------//
    if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 2)
    {
        LSM6DS3TR.accelerometr_scale_k = 16.384f;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 4)
    {
        LSM6DS3TR.accelerometr_scale_k = 8.192f;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 8)
    {
        LSM6DS3TR.accelerometr_scale_k = 4.096f;
    }
    else if (App.SetupParam.LSM6DS3TR_accelerometr_scale == 16)
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