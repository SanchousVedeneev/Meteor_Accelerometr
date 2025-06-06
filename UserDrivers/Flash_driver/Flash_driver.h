#ifndef __FLASH_DRIVER_H__
#define __FLASH_DRIVER_H__

#include "main.h"

// По умолчанию использую FLASH_ADDRESS_SECTOR_3
#define FLASH_ADDRESS_SECTOR_0  (uint32_t)(0x08000000)
#define FLASH_ADDRESS_SECTOR_1  (uint32_t)(0x08004000)
#define FLASH_ADDRESS_SECTOR_2  (uint32_t)(0x08008000)
#define FLASH_ADDRESS_SECTOR_3  (uint32_t)(0x0800C000)
#define FLASH_ADDRESS_SECTOR_4  (uint32_t)(0x08010000)
#define FLASH_ADDRESS_SECTOR_5  (uint32_t)(0x08020000)

uint8_t write_Flash (uint32_t addr);

void readFlash (uint32_t addr);

#endif