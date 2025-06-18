#include "App.h"
#include "i2c.h"
#include "ProtocolMbRtuSlaveCtrl.h"

app_typedef App;

extern LSM6DS3TR_typedef LSM6DS3TR;
extern MPU6050_typedef   MPU6050;

void app_main()
{
    app_SetupParam_init();
    app_acc_filter_init();

    HAL_Delay(100);

    LSM6DS3TR_init_struct();
    LSM6DS3TR_config();

    HAL_Delay(100);
    
    MPU6050_init_struct();
    MPU6050_config();

    HAL_Delay(100);

    protocolMbRtuSlaveCtrl_init(1);
    while (1)
    {
        HAL_Delay(40);
        app_get_acc_data_LSM6DS3TR();
        app_get_acc_data_MPU6050();
        app_acc_data_filter();
        protocolMbRtuSlaveCtrl_update_tables();
    }
}

void app_SetupParam_init()
{
    app_SetupParam_set_defolt();
    app_flash_load();
}

void app_SetupParam_set_defolt()
{
    /* @brief information
    ** accelerometr_scale (int16_t - full range)
    **       2: +-  2g
    **       4: +-  4g
    **       8: +-  8g
    **      16: +- 16g
    */
    App.SetupParam.MPU6050_scale   = 4;
    App.SetupParam.LSM6DS3TR_scale = 4;

    /* @brief information
    ** MPU6050_frequency
    **      0 - 260 Гц, задержка 0 мс
    **      1 - 184 Гц,  2.0 мс
    **      2 -  94 Гц,  3.0 мс
    **      3 -  44 Гц,  4.9 мс
    **      4 -  21 Гц,  8.5 мс
    **      5 -  10 Гц, 13.8 мс
    **      6 -   5 Гц, 19.0 мс
    ** LSM6DS3TR_frequency
    **       0 - Power-down 
    **       1 - 12.5 Hz (high performance)
    **       2 - 26   Hz (high performance)
    **       3 - 52   Hz (high performance)
    **       4 - 104  Hz (high performance)
    **       5 - 208  Hz (high performance)
    **       6 - 416  Hz (high performance)
    **       7 - 833  Hz (high performance)
    **       8 - 1660 Hz (high performance)
    **       9 - 3330 Hz (high performance)
    **      10 - 6660 Hz (high performance)
    **      11 - 12,5 Hz (high performance)
    **      12+ - Not allowed
    */ 
    App.SetupParam.MPU6050_freq   = 4;
    App.SetupParam.LSM6DS3TR_freq = 2;

    /* @brief information
    ** Order - number of cells in the averaging filter buffer
    ** filterN - aperiodic filter coefficient
    */
    for (uint8_t i = 0; i < (ACCELEROMETR_COUNT*ACCELEROMETR_AXIS); i++)
    {
        App.SetupParam.order[i]   = 1;
        App.SetupParam.filterN[i] = 1;
    }
}

void app_acc_filter_init()
{
    for (uint8_t i = 0; i < (ACCELEROMETR_COUNT*ACCELEROMETR_AXIS); i++)
    {
        App.acc_filter[i].value      = 0.0f;
        App.acc_filter[i].value_last = 0.0f;
        App.acc_filter[i].valueRaw   = 0.0f;
        App.acc_filter[i].valueSours = 0.0f;

        for(uint8_t j = 0; j < ACCELEROMETR_MAX_FILTER_ORDER; j++)
        {
            App.acc_filter[i].buf[j] = 0.0f;
        }

        App.acc_filter[i].bufIdx     = 0;
        App.acc_filter[i].order      = App.SetupParam.order[i];
        App.acc_filter[i].filter_N   = App.SetupParam.filterN[i];
    }
    return;
}

#define COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z (8)
void app_get_acc_data_LSM6DS3TR()
{
    LSM6DS3TR_i2c_read_reg(LSM6DS3TR.reg_data_address.OUTX_L_XL, (uint8_t*)LSM6DS3TR.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_filter[LSM6DS3TR_aX].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[0] | (LSM6DS3TR.i2c_conf.buf[1] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    App.acc_filter[LSM6DS3TR_aY].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[2] | (LSM6DS3TR.i2c_conf.buf[3] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    App.acc_filter[LSM6DS3TR_aZ].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[4] | (LSM6DS3TR.i2c_conf.buf[5] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    return;
}

void app_get_acc_data_MPU6050()
{

    MPU6050_i2c_read_reg(MPU6050.reg_data_address.ACCEL_XOUT_H, (uint8_t*)MPU6050.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_filter[MPU6050_aX].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[1] | (MPU6050.i2c_conf.buf[0] << 8))) / MPU6050.accelerometr_scale_k;

    App.acc_filter[MPU6050_aY].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[3] | (MPU6050.i2c_conf.buf[2] << 8))) / MPU6050.accelerometr_scale_k;

    App.acc_filter[MPU6050_aZ].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[5] | (MPU6050.i2c_conf.buf[4] << 8))) / MPU6050.accelerometr_scale_k;

    return;
}

void app_acc_data_filter()
{
    float value = 0.0f;
    float valueLast = 0.0f;
    float kFilter = 0.0f;        
    float sum = 0.0f;

    for (uint8_t i = 0; i < (ACCELEROMETR_AXIS*ACCELEROMETR_COUNT); i++)
    {
        value = 0.0f;
        valueLast = 0.0f;
        kFilter = 0.0f;
        sum = 0.0f;

        App.acc_filter[i].buf[App.acc_filter[i].bufIdx++] = App.acc_filter[i].valueSours;

        if (App.acc_filter[i].bufIdx == App.acc_filter[i].order)
        {
            App.acc_filter[i].bufIdx = 0;
        }

        for (uint8_t idx = 0; idx < App.acc_filter[i].order; idx++)
        {
            sum += App.acc_filter[i].buf[idx];
        }

        if ((-1.0f < sum) && (sum < 1.0))
        {
            App.acc_filter[i].valueRaw = 0.0f;
        }
        else
        {
            App.acc_filter[i].valueRaw = sum / App.acc_filter[i].order;
        }

        value = App.acc_filter[i].valueRaw;
        valueLast = App.acc_filter[i].value_last;
        kFilter = 2.0f / ((float)App.acc_filter[i].filter_N + 1.0f);
        value = valueLast + kFilter * (value - valueLast);
        App.acc_filter[i].value = App.acc_filter[i].value_last = App.acc_data[i] = value;
    }
    asm("Nop");
}

void app_acc_set_scale_SM6DS3TR(uint16_t value)\
{
    if ((value >= 2) || (value < 16))
    {
        App.SetupParam.LSM6DS3TR_scale = value;
    }
    else 
    {
        // Неверное значение
        asm("Nop");
    }
    return;
}

void app_acc_set_scale_MPU6050(uint16_t value)
{
    if ((value >= 2) || (value <= 16))
    {
        App.SetupParam.MPU6050_scale = value;
    }
    else 
    {
        // Неверное значение
        asm("Nop");
    }
    return;
}

void app_acc_set_freq_SM6DS3TR(uint16_t value)
{
    if ((value >= 1) || (value <= 11))
    {
        App.SetupParam.LSM6DS3TR_freq = value;
    }
    else
    {
        // Неверное значение
        asm("Nop");
    }
    return;
}

void app_acc_set_freq_MPU6050(uint16_t value)
{
    if ((value >= 0) || (value <= 6))
    {
        App.SetupParam.MPU6050_freq = value;
    }
    else
    {
        // Неверное значение
        asm("Nop");
    }
    return;

}

void app_flash_load()
{
    readFlash(FLASH_ADDRESS_SECTOR_3);
}

void app_flash_save()
{
    write_Flash(FLASH_ADDRESS_SECTOR_3);
}

void app_system_reset()
{
    __NVIC_SystemReset();
}