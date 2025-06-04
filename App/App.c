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
    LSM6DS3TR_init_struct();
    LSM6DS3TR_config();
    MPU6050_init_struct();
    MPU6050_config();
    while (1)
    {
        HAL_Delay(5);
        app_get_accelerometr_data_LSM6DS3TR();
        app_get_accelerometr_data_MPU6050();
        protocolMbRtuSlaveCtrl_update_tables();
    }
}

void app_SetupParam_init()
{
    /* @brief information
    ** accelerometr_scale =  2: +- 2g
    ** accelerometr_scale =  4: +- 4g
    ** accelerometr_scale =  8: +- 8g
    ** accelerometr_scale = 16: +- 16g
    */
    App.SetupParam.MPU6050_accelerometr_scale = 4;
    App.SetupParam.LSM6DS3TR_accelerometr_scale = 4;
}

#define COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z (8)
void app_get_accelerometr_data_LSM6DS3TR()
{
    LSM6DS3TR_i2c_read_reg(LSM6DS3TR.reg_data_address.OUTX_L_XL,  (uint8_t*)LSM6DS3TR.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_data.LSM6DS3TR_aX = (int16_t)(((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[0] | (LSM6DS3TR.i2c_conf.buf[1] << 8)))/LSM6DS3TR.accelerometr_scale_k);

    App.acc_data.LSM6DS3TR_aY = (int16_t)(((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[2] | (LSM6DS3TR.i2c_conf.buf[3] << 8)))/LSM6DS3TR.accelerometr_scale_k);

    App.acc_data.LSM6DS3TR_aZ = (int16_t)(((float)(int16_t)(LSM6DS3TR.i2c_conf.buf[4] | (LSM6DS3TR.i2c_conf.buf[5] << 8)))/LSM6DS3TR.accelerometr_scale_k);

    return;
}

void app_get_accelerometr_data_MPU6050()
{

    MPU6050_i2c_read_reg(MPU6050.reg_data_address.ACCEL_XOUT_H, (uint8_t*)MPU6050.i2c_conf.buf, COUNT_BYTE_ACCELEROMETR_DATA_X_Y_Z);

    App.acc_data.MPU6050_aX = (int16_t)(((float)(int16_t)(MPU6050.i2c_conf.buf[1] | (MPU6050.i2c_conf.buf[0] << 8)))/MPU6050.accelerometr_scale_k);

    App.acc_data.MPU6050_aY = (int16_t)(((float)(int16_t)(MPU6050.i2c_conf.buf[3] | (MPU6050.i2c_conf.buf[2] << 8)))/MPU6050.accelerometr_scale_k);

    App.acc_data.MPU6050_aZ = (int16_t)(((float)(int16_t)(MPU6050.i2c_conf.buf[5] | (MPU6050.i2c_conf.buf[4] << 8)))/MPU6050.accelerometr_scale_k);

    return;
}