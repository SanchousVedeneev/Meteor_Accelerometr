#include "MPU6050.h"
#include "i2c.h"
#include "BSP.h"
#include "App.h"

MPU6050_typedef MPU6050;

extern app_typedef App;

#define BIT_RESET (0)
#define BIT_SET   (1)
void MPU6050_init_struct()
{
    //------------------------ i2c_conf ------------------------//
    MPU6050.i2c_conf.i2c     = &hi2c3;
    MPU6050.i2c_conf.address = (0x68 << 1);
    for(uint8_t i = 0; i < MAX_BUF_I2C; i++)
    {
        MPU6050.i2c_conf.buf[i] = 0;
    }
    //---------------------- i2c_conf END ----------------------//

    //-------------------- reg_data_address --------------------//
    MPU6050.reg_data_address.ACCEL_XOUT_H = 0x3B;
    MPU6050.reg_data_address.ACCEL_XOUT_L = 0x3C;
    MPU6050.reg_data_address.ACCEL_YOUT_H = 0x3D;
    MPU6050.reg_data_address.ACCEL_YOUT_L = 0x3E; 
    MPU6050.reg_data_address.ACCEL_ZOUT_H = 0x3F;
    MPU6050.reg_data_address.ACCEL_ZOUT_L = 0x40; 
    //------------------ reg_data_address END ------------------//

    //------------------------ SMPRT_DIV ------------------------//
    /* @brief information
    ** SMPLRT_DIV_0...SMPLRT_DIV_7 - Делитель частоты дискретизации. Default value:
    **      Частота дескритизации = 8 кГц / (1 + SMPLRT_DIV_0...SMPLRT_DIV_7)
    **      Частота дескритизации = 8 кГц / (1 + 7) = 1 кГц (не менять!!!)
    */
    MPU6050.reg_conf.SMPRT_DIV.address = 0x19;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_0  = BIT_SET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_1  = BIT_SET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_2  = BIT_SET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_3  = BIT_RESET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_4  = BIT_RESET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_5  = BIT_RESET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_6  = BIT_RESET;
    MPU6050.reg_conf.SMPRT_DIV.reg_data.bits.SMPLRT_DIV_7  = BIT_RESET;
    //---------------------- SMPRT_DIV END ----------------------//

    //------------------------ CONFIG ------------------------//
    /* @brief information
    ** DLPF_CFG_0...DLPF_CFG_2 - Цифровой фильтр низких частот,
    **      000 - частота пропускания 260 Гц, задержка 0 мс
    **      001 - 184 Гц,  2.0 мс
    **      010 -  94 Гц,  3.0 мс
    **      011 -  44 Гц,  4.9 мс
    **      100 -  21 Гц,  8.5 мс
    **      101 -  10 Гц, 13.8 мс
    **      110 -   5 Гц, 19.0 мс
    **      111 - RESERVED
    ** EXT_SYNC_SET - IN DATASHEET (нам не нужно)
    */
    MPU6050.reg_conf.CONFIG.address = 0x1A;

    if (App.SetupParam.MPU6050_freq == 0)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_freq == 1)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_freq == 2)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_freq == 3)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_freq == 4)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_SET;
    }
    else if (App.SetupParam.MPU6050_freq == 5)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_SET;
    }
    else if (App.SetupParam.MPU6050_freq == 6)
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_SET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_SET;
    }
    else
    {
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_0 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_1 = BIT_RESET;
        MPU6050.reg_conf.CONFIG.reg_data.bits.DLPF_CFG_2 = BIT_SET;
    }
    
    MPU6050.reg_conf.CONFIG.reg_data.bits.EXT_SYNC_SET_0  = BIT_RESET;
    MPU6050.reg_conf.CONFIG.reg_data.bits.EXT_SYNC_SET_1  = BIT_RESET;
    MPU6050.reg_conf.CONFIG.reg_data.bits.EXT_SYNC_SET_2  = BIT_RESET;
    MPU6050.reg_conf.CONFIG.reg_data.bits.NC_6            = BIT_RESET;
    MPU6050.reg_conf.CONFIG.reg_data.bits.NC_7            = BIT_RESET;
    //---------------------- CONFIG END ----------------------//

    //------------------------ ACCEL_CONFIG ------------------------//
    /* @brief information
    ** ZA_ST, YA_ST, XA_ST - самопроверки для осей акселерометра (нас не интересует)
    ** AFS_SEL1...AFS_SEL0 - выбор диапазона шкалы акселерометра
    **      00:  +- 2g
    **      01:  +- 4g
    **      10:  +- 8g
    **      11: +- 16g
    */
    MPU6050.reg_conf.ACCEL_CONFIG.address = 0x1C;
    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.NC0      = BIT_RESET;
    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.NC1      = BIT_RESET;
    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.NC2      = BIT_RESET;

    if (App.SetupParam.MPU6050_scale == 2)
    {
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL0 = BIT_RESET;
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL1 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_scale == 4)
    {
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL0 = BIT_SET;
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL1 = BIT_RESET;
    }
    else if (App.SetupParam.MPU6050_scale == 8)
    {
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL0 = BIT_RESET;
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL1 = BIT_SET;
    }
    else if (App.SetupParam.MPU6050_scale == 16)
    {
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL0 = BIT_SET;
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL1 = BIT_SET;
    }
    else
    {
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL0 = BIT_RESET;
        MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.AFS_SEL1 = BIT_RESET;
    }

    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.ZA_ST    = BIT_RESET;
    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.YA_ST    = BIT_RESET;
    MPU6050.reg_conf.ACCEL_CONFIG.reg_data.bits.XA_ST    = BIT_RESET;
    //---------------------- ACCEL_CONFIG END ----------------------//
    asm("Nop");

    //---------------------- accelerometr_scale_k ----------------------//
    if (App.SetupParam.MPU6050_scale == 2)
    {
        MPU6050.accelerometr_scale_k = 16.384f;
    }
    else if (App.SetupParam.MPU6050_scale == 4)
    {
        MPU6050.accelerometr_scale_k = 8.192f;
    }
    else if (App.SetupParam.MPU6050_scale == 8)
    {
        MPU6050.accelerometr_scale_k = 4.096f;
    }
    else if (App.SetupParam.MPU6050_scale == 16)
    {
        MPU6050.accelerometr_scale_k = 2.048f;
    }
    else 
    {
        MPU6050.accelerometr_scale_k = 16.384f;
    }
    //-------------------- accelerometr_scale_k END --------------------//
    asm("Nop");
    return;
}

void MPU6050_config()
{
    MPU6050_i2c_write_reg(MPU6050.reg_conf.SMPRT_DIV.address, MPU6050.reg_conf.SMPRT_DIV.reg_data.w8);
    MPU6050_i2c_write_reg(MPU6050.reg_conf.CONFIG.address, MPU6050.reg_conf.CONFIG.reg_data.w8);
    MPU6050_i2c_write_reg(MPU6050.reg_conf.ACCEL_CONFIG.address, MPU6050.reg_conf.ACCEL_CONFIG.reg_data.w8);
    return;
}

void MPU6050_i2c_write_reg(uint8_t memory_adr, uint8_t data)
{
    bsp_i2c_write_one_reg(MPU6050.i2c_conf.i2c, MPU6050.i2c_conf.address, memory_adr, &data);
    return;
}

void MPU6050_i2c_read_reg(uint8_t memory_adr, uint8_t *buf_read, uint8_t size)
{
    if (size == 1)
    {
        bsp_i2c_read_one_reg(MPU6050.i2c_conf.i2c, MPU6050.i2c_conf.address, memory_adr, buf_read);
    }
    else
    {
        bsp_i2c_read_few_reg(MPU6050.i2c_conf.i2c, MPU6050.i2c_conf.address, memory_adr, buf_read, size);
    }
    return;
}