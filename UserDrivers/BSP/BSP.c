
#include "BSP.h"
#include "usart.h"

// ------------------------------ RS485 ------------------------------
#define BSP_RS485_1 huart1

#define BSP_RS_485_RX_TIMEOUT (2000)

static uint16_t timer_rs485_timeout[2] = {BSP_RS_485_RX_TIMEOUT, BSP_RS_485_RX_TIMEOUT};

void bsp_rs485_setPortToModbusRtu(uint8_t portNo, uint8_t *bufRxTX, uint16_t bufSizeByte)
{
  UART_HandleTypeDef *port = NULL;

  port = &BSP_RS485_1;

  HAL_Delay(100);

#ifdef USING_G4
  port->Instance->RTOR |= 0xfff << USART_RTOR_RTO_Pos;
  port->Instance->CR1 |= USART_CR1_RTOIE;
  port->Instance->CR2 |= USART_CR2_RTOEN;
#else
  //tim->SR &= ~TIM_SR_UIF;
  //tim->DIER |= TIM_DIER_UIE;
  //port->Instance->CR1 |= USART_CR1_RXNEIE;
  port->Instance->CR1 |= USART_CR1_IDLEIE;
#endif

  //__HAL_UART_ENABLE_IT(&huart3, UART_IT_TC);
  port->RxXferCount = bufSizeByte; // NbRxDataToProcess
  port->pRxBuffPtr = bufRxTX;
  // port->RxXferSize = bufSizeByte;

  //HAL_UART_Abort(port);

  // HAL_DMA_Start(port->hdmarx, (uint32_t)&port->Instance->DR, (uint32_t)bufRxTX, bufSizeByte);

  // port->Instance->CR3 |= USART_CR3_DMAR;
  HAL_UART_Receive_DMA(port,(uint32_t)port->pRxBuffPtr, 260);
}

void bsp_rs485_sendBlock(uint8_t portNo, uint8_t *buf, uint8_t bufSizeByte)
{
  UART_HandleTypeDef *port = NULL;
  if (portNo == 1)
    port = &BSP_RS485_1;
  else
    asm("NOP");
    // port = &BSP_RS485_2;
  // HAL_UART_Abort(port);
  //port->hdmatx->State = HAL_DMA_STATE_READY;
  //__HAL_UNLOCK(port->hdmatx);
  //HAL_DMA_Start(port->hdmatx, (uint32_t)buf, (uint32_t)&port->Instance->DR, bufSizeByte);
  //port->Instance->CR3 |= USART_CR3_DMAT;
  HAL_UART_Transmit_DMA(port, buf, bufSizeByte);

}

void bsp_rs485_sendTestBlock(uint8_t portNo)
{
  static uint8_t testBuf[5] = {0xAA, 1, 2, 3, 4};
  bsp_rs485_sendBlock(portNo, testBuf, 5);
}

__weak void bsp_rs485_callback_rxBlockReady(uint8_t portNo)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(portNo);
  /*
      Если callback нужен его необходимо переопределить.
      void bsp_rs485_callback_rxBlockReady(UART_HandleTypeDef* port){

      }
      Удалять функцию не надо.
  */
}

__weak void bsp_rs485_callback_rxTimeout(uint8_t portNo)
{

  asm("NOP");
}

void BSP_RS485_1_IRQ_HANDLER(void)
{
  
  if (__HAL_UART_GET_FLAG(&BSP_RS485_1, UART_FLAG_IDLE)) // RX READY
  {
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_IDLE);
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_RXNE);
    asm("NOP");

  // *            @arg DMA_FLAG_TCIFx: Transfer complete flag.
  // *            @arg DMA_FLAG_HTIFx: Half transfer complete flag.
  // *            @arg DMA_FLAG_TEIFx: Transfer error flag.
  // *            @arg DMA_FLAG_DMEIFx: Direct mode error flag.
  // *            @arg DMA_FLAG_FEIFx: FIFO error flag.

    if (BSP_RS485_1.hdmarx->Instance->NDTR !=  260 )
    {
      __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_ORE);  // OVERRUN
      __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_FE);  // OVERRUN
      __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_PE);  // OVERRUN
      __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_TXE);  // OVERRUN
      __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_NE);  // OVERRUN

    BSP_RS485_1.Instance->CR3 &= ~USART_CR3_DMAR;

    HAL_UART_Abort(&BSP_RS485_1);

    bsp_rs485_callback_rxBlockReady(1);

    timer_rs485_timeout[0] = BSP_RS_485_RX_TIMEOUT;

    HAL_DMA_Abort(BSP_RS485_1.hdmarx);

    HAL_UART_Receive_DMA(&BSP_RS485_1,BSP_RS485_1.pRxBuffPtr, 260);

    }
    else
    {
      asm("NOP");
    }

  }
  else if (__HAL_UART_GET_FLAG(&BSP_RS485_1, UART_FLAG_RXNE))
  {
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_RXNE);
  }
  else if (__HAL_UART_GET_FLAG(&BSP_RS485_1, UART_FLAG_TC)) // TRANSFER COMPLETE
  {
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_TC);
    asm("NOP");
    BSP_LED_TOGGLE(LED1);
    if (BSP_RS485_1.hdmatx->Instance->NDTR == 0)
    {
      asm("NOP");
    }
  }
  else
  {
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_CTS);  // OVERRUN
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_LBD);  // OVERRUN
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_TC);   // OVERRUN
    __HAL_UART_CLEAR_FLAG(&BSP_RS485_1, UART_FLAG_IDLE); // OVERRUN
    asm("NOP");
  }
}

// ------------------------------ RS485 END ------------------------------


//-------------------------------- I2C --------------------------------//

#define MAX_I2C_TIMEOUT (1000)
void bsp_i2c_write_one_reg(I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_write)
{
  if (HAL_I2C_Mem_Write(hic2, device_adr, memory_adr, 1,  buf_write, 1, MAX_I2C_TIMEOUT) == HAL_OK)
  {
    BSP_LED_TOGGLE(LED2);
  }
  return;
}

void bsp_i2c_read_one_reg (I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_read)
{
  if (HAL_I2C_Mem_Read(hic2, device_adr, memory_adr, 1, buf_read, 1, MAX_I2C_TIMEOUT) == HAL_OK)
  {
    BSP_LED_TOGGLE(LED2);
  }
  return;
}

void bsp_i2c_read_few_reg (I2C_HandleTypeDef *hic2, uint8_t device_adr, uint8_t memory_adr, uint8_t *buf_read, uint8_t size)
{
  if (HAL_I2C_Mem_Read(hic2, device_adr, memory_adr, 1, buf_read, size, MAX_I2C_TIMEOUT) == HAL_OK)
  {
    BSP_LED_TOGGLE(LED2);
  }
  return;
}

//------------------------------ I2C END ------------------------------//


