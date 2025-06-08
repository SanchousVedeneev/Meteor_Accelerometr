
#include "ProtocolMbRtuSlaveCtrl.h"

#include "App.h"

uint8_t modbusBufRxTxRtu485[MODBUS_SS_BUF_CNT];

//--------------------  PROTOCOL ---------------------//
//---1
#define MDB_TABLE_ACC_REG_NO (1)

enum mdb_table_acc
{
  mdb_table_acc_LSM6DS3TR_aX = MDB_TABLE_ACC_REG_NO,
  mdb_table_acc_LSM6DS3TR_aY,
  mdb_table_acc_LSM6DS3TR_aZ,
  mdb_table_acc_MPU6050_aX,
  mdb_table_acc_MPU6050_aY,
  mdb_table_acc_MPU6050_aZ,
};
#define MDB_TABLE_ACC_COUNT (mdb_table_acc_MPU6050_aZ - mdb_table_acc_LSM6DS3TR_aX + 1)
uint16_t mdb_table_acc_buf[MDB_TABLE_ACC_COUNT];
ModbusSS_table_t mdb_table_acc = {
    .buf = (uint8_t *)mdb_table_acc_buf,
    .quantity = MDB_TABLE_ACC_COUNT,
    .regNo = MDB_TABLE_ACC_REG_NO,
    .type = ModbusSS_Holding};

//---100
#define MDB_TABLE_SETUP_REG_NO (100)
enum mdb_table_setup
{
  mdb_table_setup_MPU6050_acc_scale = MDB_TABLE_SETUP_REG_NO,
  mdb_table_setup_LSM6DS3TR_acc_scale,
  mdb_table_setup_MPU6050_freq,
  mdb_table_setup_LSM6DS3TR_freq,
  mdb_table_setup_order_1 = 104,
  mdb_table_setup_order_8 = 109,
  mdb_table_setup_filterN_1 = 110,
  mdb_table_setup_filterN_8 = 115
};
#define MDB_TABLE_SETUP_COUNT (mdb_table_setup_filterN_8 - mdb_table_setup_MPU6050_acc_scale + 1)
uint16_t mdb_table_setup_buf[MDB_TABLE_SETUP_COUNT];
ModbusSS_table_t mdb_table_setup = {
    .buf = (uint8_t *)mdb_table_setup_buf,
    .quantity = MDB_TABLE_SETUP_COUNT,
    .regNo = MDB_TABLE_SETUP_REG_NO,
    .type = ModbusSS_Holding};

//---200
#define MDB_TABLE_CONTROL_NO (200)
enum mdb_table_control
{
  mdb_table_control_cmd = MDB_TABLE_CONTROL_NO
};
#define MDB_TABLE_CONTROL_COUNT (1)
uint16_t mdb_table_control_buf[MDB_TABLE_CONTROL_COUNT];
ModbusSS_table_t mdb_table_control = {
    .buf = (uint8_t *)mdb_table_control_buf,
    .quantity = MDB_TABLE_CONTROL_COUNT,
    .regNo = MDB_TABLE_CONTROL_NO,
    .type = ModbusSS_Holding};


  
//--------------------  PROTOCOL END---------------------//

//--------------------  TABLES ARRAY ---------------------//
ModbusSS_table_t *modbusTables[] = {
    &mdb_table_acc,
    &mdb_table_setup,
    &mdb_table_control
};
//--------------------  TABLES ARRAY END---------------------//

//--------------------  MODBUS STRUCT ---------------------//
ModbusSS_t modbusSS_rtu_rs485 = {
    .cbHoldingUpdate = protocolMbRtuSlaveCtrl_callback_H_WRITE,
    .cbHoldingRequest = NULL, // protocolMbRtuSlaveCtrl_callback_H_REQ, //modbusHoldingReq,
    .rtuTcp = MODBUS_SS_RTU,
    .bufRxTx = modbusBufRxTxRtu485,
    .slaveId = 1,
    .tables = modbusTables,
    .tablesCount = 3};

protocolMbRtuSlaveCtrl_typedef modbusRtu_ctrlStruct; // protocol control struct
//--------------------  MODBUS STRUCT END---------------------//

//------------------------ EXTERN ------------------------
extern app_typedef App;
//------------------------ EXTERN END------------------------

//------------------------ REGULAR FCN ------------------------
void protocolMbRtuSlaveCtrl_init(uint8_t portNo)
{
  HAL_Delay(100);
  if (portNo == 1)
  {
    bsp_rs485_setPortToModbusRtu(portNo, modbusBufRxTxRtu485, MODBUS_SS_BUF_CNT);
  }
  else
  {
    // bug with init modbus
    asm("NOP");
  }
}

void protocolMbRtuSlaveCtrl_update_tables()
{
  //1__

  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_LSM6DS3TR_aX, App.acc_data[LSM6DS3TR_aX]);
  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_LSM6DS3TR_aY, App.acc_data[LSM6DS3TR_aY]);
  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_LSM6DS3TR_aZ, App.acc_data[LSM6DS3TR_aZ]);
  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_MPU6050_aX,   App.acc_data[MPU6050_aX]);
  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_MPU6050_aY,   App.acc_data[MPU6050_aY]);
  ModbusSS_SetWord(&mdb_table_acc, mdb_table_acc_MPU6050_aZ,   App.acc_data[MPU6050_aZ]);

  //100__
  ModbusSS_SetWord(&mdb_table_setup, mdb_table_setup_MPU6050_acc_scale,   App.SetupParam.MPU6050_acc_scale);
  ModbusSS_SetWord(&mdb_table_setup, mdb_table_setup_LSM6DS3TR_acc_scale, App.SetupParam.LSM6DS3TR_acc_scale);
  ModbusSS_SetWord(&mdb_table_setup, mdb_table_setup_MPU6050_freq,        App.SetupParam.MPU6050_freq);
  ModbusSS_SetWord(&mdb_table_setup, mdb_table_setup_LSM6DS3TR_freq,      App.SetupParam.LSM6DS3TR_freq);

  for (uint8_t i = mdb_table_setup_order_1, j = 0; i <= mdb_table_setup_order_8; i++, j++)
  {
    ModbusSS_SetWord(&mdb_table_setup, i, App.SetupParam.order[j]);
  }

  for (uint8_t i = mdb_table_setup_filterN_1, j = 0; i <= mdb_table_setup_filterN_8; i++, j++)
  {
    ModbusSS_SetWord(&mdb_table_setup, i, App.SetupParam.filterN[j]);
  }
  
  return;
}
//------------------------ REGULAR FCN END------------------------

//------------------------------- MODBUS CALLBACKS -------------------------------------------//
#define PROTOCOL_MB_RTU_SLAVE_CTRL_CMD_OK (0xAAAA)
#define PROTOCOL_MB_RTU_SLAVE_CTRL_CMD_FAIL (0xF00F)
#define CMD_PARAM_SET_DEFOLT (0x0001)
__weak void protocolMbRtuSlaveCtrl_callback_H_WRITE(ModbusSS_table_t *table, uint16_t reg, uint16_t quantity)
{
  // uint16_t response = PROTOCOL_MB_RTU_SLAVE_CTRL_CMD_FAIL;
  // uint16_t param = 0;
  // float kMul = 0.001f;
  // float value = 0.0f;
  // uint8_t idx = 0;
  asm("NOP");

  if (table == &mdb_table_control) // Диапазон PROGRAM
  {
    switch (reg)
    {
    case mdb_table_control_cmd:
      modbusRtu_ctrlStruct.cmd = ModbusSS_GetWord(&mdb_table_control, reg);
      switch (modbusRtu_ctrlStruct.cmd)
      {
      case protocol_cmd_save_param:
        app_flash_save();
        break;
      case protocol_cmd_reset:
        app_system_reset();
        break;
      case protocol_cmd_param_set_defolt:
        app_SetupParam_set_defolt();
        app_system_reset();
        break;
      default:
        break;
      }
    default:
      break;
    }
  }
}

__weak void protocolMbRtuSlaveCtrl_callback_H_READ(ModbusSS_table_t *table, uint16_t reg, uint16_t quantity)
{
  asm("NOP");

}
//------------------------------- MODBUS CALLBACKS END-------------------------------------------//

//------------------------------- HW CALLBACK -------------------------------------------//
void bsp_rs485_callback_rxBlockReady(uint8_t portNo)
{

  int32_t blockSizeByte = 0;

  if (portNo == 1)
  {
    if ((blockSizeByte = ModbusSS_ParseRxData(&modbusSS_rtu_rs485)) == 0)
    {
      // bug with reset modbus!!!
      asm("NOP");
    }
    else if (blockSizeByte != -1)
    {
      asm("NOP");
      bsp_rs485_sendBlock(portNo, modbusSS_rtu_rs485.bufRxTx, blockSizeByte);
      //HAL_UART_Transmit(&huart2)
      asm("NOP");
    }
  }
  else 
  {
    // bug with reset modbus!!!
    asm("NOP");
  }
  
}
//------------------------------- HW CALLBACK END----------- -----------------------------//




