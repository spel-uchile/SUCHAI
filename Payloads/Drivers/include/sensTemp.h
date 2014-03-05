#ifndef SENS_TEMP_H
#define	SENS_TEMP_H

#include "pic_pc104_config.h"

#define ST1_ADD_READ    0b10010001
#define ST1_ADD_WRITE   0b10010000
#define ST1_ADDRESS     0b10010001

#define ST2_ADD_READ    0b10010101
#define ST2_ADD_WRITE   0b10010100
#define ST2_ADDRESS     0b10010101

#define ST3_ADD_READ    0b10011001
#define ST3_ADD_WRITE   0b10011000
#define ST3_ADDRESS     0b10011001

#define ST4_ADD_READ    0b10011101
#define ST4_ADD_WRITE   0b10011100
#define ST4_ADDRESS     0b10011101

#define STx_REG_TEMP_READ       0b00000000
#define STx_REG_CONFIG          0b00000001
#define STx_REG_TLOW            0b00000010
#define STx_REG_THIGH           0b00000011

void sensTemp_init(unsigned char sens_x);
int sensTemp_take(unsigned char sens_x);
void sensTemp_stop(unsigned char sens_x);


void writeSensTemp(unsigned char sens_x, unsigned char address, unsigned char data);
unsigned char readSensTemp(unsigned char sens_x, unsigned char address);

#endif
