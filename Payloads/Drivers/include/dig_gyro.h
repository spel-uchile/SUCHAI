#ifndef DIG_GYRO_H
#define	DIG_GYRO_H
/*========================================================================================
Description            		:Function headers set for the digital gyroscope
File                            :dig_gyro.h
Created by    	           	:Alex Becerra & Tomas Opazo
Project                    	:SUCHAI
Creation date        		:01/06/2012
==========================================================================================*/

#include <stdlib.h>
#include "pic_pc104_config.h"

#define GYR_INT2  	PPC_GYRO_INT2_CHECK				// Button for INT2

// Register's addresses for each possible command of the gyro
// if register[7] = 1, it allows multiple read/write
#define GYR_WHO_AM_I  		0x0F 		// Read only
#define GYR_CTRL_REG1 		0x20
#define GYR_CTRL_REG2 		0x21
#define GYR_CTRL_REG3 		0x22
#define GYR_CTRL_REG4 		0x23
#define GYR_CTRL_REG5 		0x24
#define GYR_REFERENCE 		0x25
#define GYR_OUT_TEMP 		0x26		// Output
#define GYR_STATUS_REG 		0x27 		// Output
#define GYR_OUT_X_L 		0x28 		// Output
#define GYR_OUT_X_L_RB		0xA8 		// Output - It allows to read multiple registers
#define GYR_OUT_X_H 		0x29 		// Output
#define GYR_OUT_Y_L 		0x2A 		// Output
#define GYR_OUT_Y_H 		0x2B 		// Output
#define GYR_OUT_Z_L 		0x2C 		// Output
#define GYR_OUT_Z_H 		0x2D 		// Output
#define GYR_FIFO_CTRL_REG 	0x2E
#define GYR_FIFO_SRC_REG 	0x2F 		// Output
#define GYR_INT1_CFG 		0x30
#define GYR_INT1_SRC 		0x31 		// Output
#define GYR_INT1_TSH_XH  	0x32
#define GYR_INT1_TSH_XL  	0x33
#define GYR_INT1_TSH_YH  	0x34
#define GYR_INT1_TSH_YL  	0x35
#define GYR_INT1_TSH_ZH  	0x36
#define GYR_INT1_TSH_ZL  	0x37
#define GYR_INT1_DURATION  	0x38

// Read and write addresses for the gyro (depends on hardware configuration)
//#define GYR_ADD_READ	0xD3    //0b1101001-1
//#define GYR_ADD_WRITE   0xD2    //0b1101001-0
//#define GYR_ADD   0xD2    //0b1101001-0
#define GYR_ADD   0b01101001

typedef struct{
    int a_x;
    int a_y;
    int a_z;
}GYR_DATA;

// Digital Gyro's functions
void gyr_do_delay(unsigned long d);
void gyr_write_reg(unsigned char reg, unsigned char val);
unsigned char gyr_read_reg(unsigned char reg);
void whoami(void);
BOOL gyr_isAlive(void);

void gyr_config_CTRL_REG3(unsigned char reg);
void gyr_config_CTRL_REG4(unsigned char reg);
void gyr_config_CTRL_REG1(unsigned char reg);
void gyr_config_CTRL_REG2(unsigned char reg);
void gyr_config_FIFO_CTRL_REG(unsigned char watermark);

void gyr_config_FIFO_mode(unsigned char mode);
void gyr_powermode(unsigned char powerdown);
void gyr_enable_axis(unsigned char axis);

void gyr_print_remain_samp(void);
void gyr_print_FIFO_int_source(void);
void gyr_print_temp(void);
void gyr_get_FIFO_samples(BOOL verb, GYR_DATA *res_data);
void gyr_get_data(BOOL verb, unsigned char *dir, unsigned char muestras, GYR_DATA *res_data);


int gyr_init_config(void);
void gyr_take_samples(BOOL verb, GYR_DATA *res_data);
#endif
