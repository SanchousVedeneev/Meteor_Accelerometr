#ifndef __ProtocolMbRtuSlaveCtrl__H__
#define __ProtocolMbRtuSlaveCtrl__H__


#include "ModbusSS.h"
#include "BSP.h"


typedef enum{
    protocol_cmd_save_param =1,
    protocol_cmd_reset,
    protocol_cmd_param_set_defolt
}protocolMbRtuSlaveCtrl_control_typedef;

typedef struct
{
    protocolMbRtuSlaveCtrl_control_typedef cmd;
} protocolMbRtuSlaveCtrl_typedef;



void protocolMbRtuSlaveCtrl_init(uint8_t portNo);
void protocolMbRtuSlaveCtrl_update_tables();


void protocolMbRtuSlaveCtrl_callback_H_READ(ModbusSS_table_t* table, uint16_t reg, uint16_t quantity);
void protocolMbRtuSlaveCtrl_callback_H_WRITE(ModbusSS_table_t* table, uint16_t reg, uint16_t quantity);







#endif