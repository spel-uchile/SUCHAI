#ifndef MEM_EEPROM_H
#define MEM_EEPROM_H

#include "i2c_comm.h"

//#define MEP_EEPROM_ID 0b1010000 /* I2C node address (7bit hex) */
#define MEP_EEPROM_ID   0b01010000 /* I2C node address (7bit hex) */
#define MEP_EEPROM1_IDW 0b10100000 /* I2C node address Write */
#define MEP_EEPROM1_IDR 0b10100001 /* I2C node address Read */

#define MEP_FIRST_ADDR 0x00
#define MEP_LAST_ADDR 0x7F  //127

void writeEEPROM1(unsigned char address, char data);
unsigned char readEEPROM1(unsigned char address);

void writeIntEEPROM1(unsigned char indx, int value);
int readIntEEPROM1(unsigned char indx);

int init_memEEPROM(void);
int memEEPROM_isAlive(void);

void mem_setVar( unsigned char indxVar, int value);
int mem_getVar( unsigned char indxVar);

#endif /* MEM_EEPROM_H   */
