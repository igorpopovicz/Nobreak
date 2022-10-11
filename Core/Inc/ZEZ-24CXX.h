//========================================================================================
//                             lib eeprom 24Cxx
//
//    14/05/21                                                                Jonny zez
//========================================================================================


#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stdint.h"
#include "stm32g0xx_hal.h"

void write_eeprom(uint16_t end, uint8_t data);

void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase (uint16_t page);

void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float  fdata);
float EEPROM_Read_NUM (uint16_t page, uint16_t offset);
uint8_t read_eeprom(uint16_t end);
#endif /* INC_EEPROM_H_ */
