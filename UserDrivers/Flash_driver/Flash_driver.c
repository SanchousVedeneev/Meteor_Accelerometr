#include "Flash_driver.h"
#include "App.h"

extern app_typedef App;

uint8_t write_Flash (uint32_t addr)
{
	HAL_StatusTypeDef status;
	uint32_t structureSize = sizeof(App.SetupParam);
	FLASH_EraseInitTypeDef FlashErase;
	uint32_t sectorError = 0; 

	__disable_irq();
	HAL_FLASH_Unlock();  

	FlashErase.TypeErase = FLASH_TYPEERASE_SECTORS;
	FlashErase.NbSectors = 1;
	FlashErase.Sector = FLASH_SECTOR_3;
	FlashErase.VoltageRange = VOLTAGE_RANGE_3;
	if (HAL_FLASHEx_Erase(&FlashErase, &sectorError) != HAL_OK)  
	{
		HAL_FLASH_Lock();
        __enable_irq();
		return HAL_ERROR;
	}
	uint32_t *dataPtr = (uint32_t *)&App.SetupParam;
	for (uint8_t i = 0; i < structureSize / 4; i++)
	{
		status += HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, dataPtr[i]);
		addr += 4;
	}
	__enable_irq();  
	return status;
}

void readFlash (uint32_t addr)
{
	uint32_t structureSize = sizeof(App.SetupParam);
	uint32_t *dataPtr = (uint32_t *)&App.SetupParam;
	for (int i = 0; i < structureSize / 4; i++)
	{
		dataPtr[i] = *(__IO uint32_t*)addr;
		addr += 4;
	}
}
