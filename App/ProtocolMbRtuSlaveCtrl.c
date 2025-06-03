
#include "ProtocolMbRtuSlaveCtrl.h"

#include "App.h"

uint8_t modbusBufRxTxRtu485[MODBUS_SS_BUF_CNT];

//--------------------  PROTOCOL ---------------------//
//---1000
#define MDB_TABLE_BSP_REG_NO (1001)
#define MDB_TABLE_BSP_COUNT (100)
enum mdb_table_bsp
{
  mdb_table_bsp_1 = MDB_TABLE_BSP_REG_NO,
  mdb_table_bsp_2,
  mdb_table_bsp_3,
  mdb_table_bsp_4,
  mdb_table_bsp_5,
  mdb_table_bsp_6,
  mdb_table_bsp_7,
  mdb_table_bsp_8,
  mdb_table_bsp_9,
  mdb_table_bsp_10,
};

uint16_t mmdb_table_bsp_buf[mdb_table_bsp_10 - mdb_table_bsp_1 + 1];
ModbusSS_table_t mdb_table_bsp = {
    .buf = (uint8_t *)mmdb_table_bsp_buf,
    .quantity = MDB_TABLE_BSP_COUNT,
    .regNo = MDB_TABLE_BSP_REG_NO,
    .type = ModbusSS_Holding};

//--------------------  PROTOCOL END---------------------//

//--------------------  TABLES ARRAY ---------------------//
ModbusSS_table_t *modbusTables[] = {
    &mdb_table_bsp
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
    .tablesCount = 1};

protocolMbRtuSlaveCtrl_typedef modbusRtu_ctrlStruct; // protocol control struct
//--------------------  MODBUS STRUCT END---------------------//

//------------------------ EXTERN ------------------------

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

  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_1,    1);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_2,    2);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_3,    3);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_4,    4);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_5,    5);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_6,    6);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_7,    7);
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_8,    8);     
  ModbusSS_SetWord(&mdb_table_bsp, mdb_table_bsp_9,    9);

  asm("NOP");
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




