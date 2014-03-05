/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 * 
 *      Copyright 2013, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sensTemp.h"

void sensTemp_init(unsigned char sens_x){
    //        11100000
    unsigned char data = 0b01100000;
    writeSensTemp(sens_x, STx_REG_CONFIG, data);

    #if (SCH_SENS_TEMP_VERBOSE>=1)
        char ret[10];
        con_printf("sensTemp_init: Config=");
        unsigned char val = readSensTemp( ST1_ADD_WRITE, STx_REG_CONFIG);
        //utoa(ret, (unsigned int)val, 16);
        sprintf (ret, "0x%X", val);
        con_printf(ret); con_printf("\r\n");
    #endif
}
int sensTemp_take(unsigned char sens_x){
    unsigned char val[2];
    unsigned char reg=STx_REG_TEMP_READ;
    I2C3ReadChar( (sens_x&0b11111110), reg, (sens_x|0b00000001), val, 2, I2C_MODE_STD);

    unsigned int val2=val[0];
    val2=(val2<<8);
    val2=(val2)|((unsigned int)val[1]);
    val2=(val2>>4);

    #if (SCH_SENS_TEMP_VERBOSE>=1)
        char ret[10];
        con_printf("SensTemp=");
        //utoa(ret, (unsigned int)val2, 10);
        sprintf (ret, "0x%X", val2);
        con_printf(ret); con_printf("\r\n");
    #endif
    return val2;
}
void sensTemp_stop(unsigned char sens_x){
    
}

//*****************************************************************************************
void writeSensTemp(unsigned char sens_x, unsigned char address, unsigned char data){
    int max;
    for(max=0x0FFF;max>0;max--){
        if( I2C3SlaveReady( (sens_x&0b11111110) )==1 ){break;}
    }

    unsigned char DATA[1];
    DATA[0]=data;
    unsigned int N=1;
    I2C3SendChar( (sens_x&0b11111110), address, DATA, N);
}

unsigned char readSensTemp(unsigned char sens_x, unsigned char address){
    int max;
    for(max=0x0FFF;max>0;max--){
        if( I2C3SlaveReady( (sens_x&0b11111110) )==1 ){break;}
    }

    unsigned int N=1;
    unsigned char BUFF[1];
    unsigned char MODE=0;

    I2C3ReadChar( (sens_x&(0b11111110)), address, (sens_x|0b00000001), BUFF, N, MODE);
    return BUFF[0];
}
