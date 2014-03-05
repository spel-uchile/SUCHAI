//#ifndef I2C_SUCHAI_H /*I2C_SUCHAI_H  */
//#define I2C_SUCHAI_H

/******************************************************************************
 *
 *                  I2C SUCHAI PERIPHERAL INTERFACE HEADER FILE
 *
 ******************************************************************************
 * FileName:        i2c_suchai.h
 * Dependencies:    i2c.h
 * Processor:       PIC24F
 * Compiler:        MPLAB C30
 * Company:         SUCHAI
 *****************************************************************************/
#include <i2c.h>


//#define I2C_EEPROM_IDW	0xA0
//#define I2C_EEPROM_IDR	0xA1

#define I2C_MODE_STD    0
#define I2C_MODE_EPS    1

/*---------------------------------
 *		 I2C STATE CODING TABLE
 *---------------------------------*/
#define I2C_IDLE	 	0x0001
#define I2C_START 		0x0002
#define I2C_BUSY_START          0x0003
#define I2C_START2 		0xF002
#define I2C_BUSY_START2         0xF003
#define I2C_WRITE1 		0x0004
#define I2C_BUSY1 		0x0005
#define I2C_WRITE2 		0x0006
#define I2C_BUSY2 		0x0007
#define I2C_WRITE3 		0x0008
#define I2C_BUSY3 		0x0009
#define I2C_MODE_ACK 		0x000A
#define I2C_BUSY_ACK            0x000B
#define I2C_STOP 		0x000C
#define I2C_BUSY_STOP           0x000D
#define I2C_READ 		0xF009
#define I2C_READ2		0xF00A
#define I2C_ACK2 		0xF00B
#define I2C_BUSY4		0xF00C
#define I2C_BUSY_ACK2           0xF00D

/*---------------------------------
 *      I2C CLOCK RATES TABLE
 *---------------------------------
 * Req.Fr -  FCY  - BRG
 * 100kHz - 16MHz - 157
 * 100kHz - 08MHz - 78
 * 400kHz - 16MHz - 37
 * 001MHz - 16MHz - 13
 *--------------------------------*/

void ConfigI2C1(unsigned int BRG);
void I2C1SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N);
void I2C1ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE);
unsigned int I2C1SlaveReady(unsigned char ID);

//I2C2
void ConfigI2C2(unsigned int BRG);
void I2C2SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N);
void I2C2ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE);
unsigned int I2C2SlaveReady(unsigned char ID);

//I2C3
void ConfigI2C3(unsigned int BRG);
void I2C3SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N);
void I2C3ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE);
unsigned int I2C3SlaveReady(unsigned char ID);

//#endif /* I2C_SUCHAI_H  */ 
