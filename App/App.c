#include "App.h"
#include "i2c.h"
#include "ProtocolMbRtuSlaveCtrl.h"

app_typedef App;

extern LSM6DS3TR_typedef LSM6DS3TR;
extern MPU6050_typedef   MPU6050;

void app_main()
{
    protocolMbRtuSlaveCtrl_init(1);

    app_SetupParam_init();

    app_acc_filter_init();

    LSM6DS3TR_init_struct();

    LSM6DS3TR_config();

    MPU6050_init_struct();
    
    MPU6050_config();
    while (1)
    {
        static uint8_t count = 0;
        HAL_Delay(40);

        app_get_accelerometr_data_LSM6DS3TR();

        app_get_accelerometr_data_MPU6050();

        app_accelerometr_data_filter();

        protocolMbRtuSlaveCtrl_update_tables();
        // if (count++ > 100)
        // {
        //     LSM6DS3TR_init_struct();
        //     LSM6DS3TR_config();
        //     MPU6050_init_struct();
        //     MPU6050_config();
        // }
    }
}

void app_SetupParam_init()
{
    /* @brief information
    ** accelerometr_scale (int16_t - full range)
    **       2: +-  2g
    **       4: +-  4g
    **       8: +-  8g
    **      16: +- 16g
    */
    App.SetupParam.MPU6050_accelerometr_scale   = 4;
    App.SetupParam.LSM6DS3TR_accelerometr_scale = 4;

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
void app_get_accelerometr_data_LSM6DS3TR()
{
    LSM6DS3TR_i2c_read_reg(LSM6DS3TR.reg_data_address.OUTX_L_XL, (uint8_t*)LSM6DS3TR.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_filter[LSM6DS3TR_aX].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[0] | (LSM6DS3TR.i2c_conf.buf[1] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    App.acc_filter[LSM6DS3TR_aY].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[2] | (LSM6DS3TR.i2c_conf.buf[3] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    App.acc_filter[LSM6DS3TR_aZ].valueSours = ((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[4] | (LSM6DS3TR.i2c_conf.buf[5] << 8))) / LSM6DS3TR.accelerometr_scale_k;

    return;
}

void app_get_accelerometr_data_MPU6050()
{

    MPU6050_i2c_read_reg(MPU6050.reg_data_address.ACCEL_XOUT_H, (uint8_t*)MPU6050.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_filter[MPU6050_aX].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[1] | (MPU6050.i2c_conf.buf[0] << 8))) / MPU6050.accelerometr_scale_k;

    App.acc_filter[MPU6050_aY].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[3] | (MPU6050.i2c_conf.buf[2] << 8))) / MPU6050.accelerometr_scale_k;

    App.acc_filter[MPU6050_aZ].valueSours = ((float)(int16_t)(MPU6050.i2c_conf.buf[5] | (MPU6050.i2c_conf.buf[4] << 8))) / MPU6050.accelerometr_scale_k;

    return;
}

void app_accelerometr_data_filter()
{
    for (uint8_t i = 0; i < (ACCELEROMETR_AXIS*ACCELEROMETR_COUNT); i++)
    {
        float value = 0.0f;
        float valueLast = 0.0f;
        float kFilter = 0.0f;
        float sum = 0.0f;

        App.acc_filter[i].buf[App.acc_filter[i].bufIdx++] = App.acc_filter[i].valueSours;

        if (App.acc_filter[i].bufIdx == App.acc_filter[i].order)
        {
            App.acc_filter[i].bufIdx = 0;
        }

        for (uint8_t idx = 0; idx < App.acc_filter[i].order; idx++)
        {
            sum += App.acc_filter[i].buf[idx];
        }

        if ((sum > -1.0f) && (sum < 1.0) )
        {
            App.acc_filter[i].valueRaw = 0.0f;
        }
        else
        {
            App.acc_filter[i].valueRaw = sum / App.acc_filter[i].order;
        }

        //--------------------//
        value = App.acc_filter[i].valueRaw;
        valueLast = App.acc_filter[i].value_last;
        kFilter = 2.0f / ((float)App.acc_filter[i].filter_N + 1.0f);
        value = valueLast + kFilter * (value - valueLast);
        App.acc_filter[i].value = App.acc_filter[i].value_last = App.acc_data[i] = value;
    }
    asm("Nop");
}