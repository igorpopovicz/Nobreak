
//========================================================================================
//                             lib eeprom 24Cxx
//
//    14/05/21                                                                Jonny zez
//========================================================================================

#include "ZEZ-24CXX.h"
#include "math.h"
#include "string.h"

// Define the I2C
extern I2C_HandleTypeDef hi2c2;
#define EEPROM_I2C &hi2c2

// EEPROM endereço 
#define EEPROM_ADDR 0xA0

#define END_SIZE 2      // tamanho do end . 24c01 e 24c02 END_SIZE=1 .. outras = 2
#define PAGE_SIZE 8
#define PAGE_NUM  32    // tamanho da memoria

//=======================================================================================
uint8_t bytes_temp[4];

//definição de tamanho 
uint16_t bytestowrite (uint16_t size, uint16_t offset)
{
	if ((size+offset)<PAGE_SIZE) return size;
	else return PAGE_SIZE-offset;
}

//=======================================================================================
void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	
	int paddrposition = log(PAGE_SIZE)/log(2);

	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;
	
	for (int i=0; i<numofpages; i++)
	{
	
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset);  

		HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, END_SIZE, &data[pos], bytesremaining, 1000);

		startPage += 1;  
		offset=0;   
		size = size-bytesremaining;  
		pos = bytesremaining;  

		HAL_Delay (5);  
	}
}

//=======================================================================================
void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}

//=======================================================================================

void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float data)
{

	float2Bytes(bytes_temp, data);

	EEPROM_Write(page, offset, bytes_temp, 4);
}

//=======================================================================================
float EEPROM_Read_NUM (uint16_t page, uint16_t offset)
{
	uint8_t buffer[4];

	EEPROM_Read(page, offset, buffer, 4);

	return (Bytes2float(buffer));
}

//=======================================================================================
void write_eeprom(uint16_t end, uint8_t data)
{

 HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, end, END_SIZE, &data, 1, 1);

 HAL_Delay (5);

}

//=======================================================================================
uint8_t read_eeprom(uint16_t end)
{
	uint8_t dado;

 HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, end, END_SIZE, &dado, 1, 1);

 HAL_Delay (2);
 return (dado);

}

void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	int paddrposition = log(PAGE_SIZE)/log(2);

	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);

	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;

	for (int i=0; i<numofpages; i++)
	{
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, MemAddress, END_SIZE, &data[pos], bytesremaining, 10);
		startPage += 1;
		offset=0;
		size = size-bytesremaining;
		pos = bytesremaining;
	}
}

//=======================================================================================
void EEPROM_PageErase (uint16_t page)
{
	
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t MemAddress = page<<paddrposition;
	
	uint8_t data[PAGE_SIZE];
	memset(data,0xff,PAGE_SIZE);
	
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, END_SIZE, data, PAGE_SIZE, 1000);

	HAL_Delay (5);  
}
