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

int sensTemp_init(unsigned char sens_x){
    //        11100000
    unsigned char data = 0b01100000;
    writeSensTemp(sens_x, STx_REG_CONFIG, data);

//    if(verb){
//        char ret[10];
//        con_printf("sensTemp_init: Config=");
//        unsigned char val = readSensTemp( ST1_ADDRESS, STx_REG_CONFIG);
//        //utoa(ret, (unsigned int)val, 16);
//        sprintf (ret, "0x%X", val);
//        con_printf(ret); con_printf("\r\n");
//    }

    //Como saber que init fue correcto?
    return 1;
}
int sensTemp_take(unsigned char sens_addr, BOOL verb){
    char val[2];
    unsigned char reg=STx_REG_TEMP_READ;
    char address[] = {sens_addr, (char)reg};

    i2c3_master_fgets(val, 2, address, 2);

    unsigned int val2 = val[0];
    val2=(val2<<8);
    val2=(val2)|((unsigned int)val[1]);
    val2=(val2>>4);

    if(verb){
        char ret[10];
        con_printf("SensTemp=");
        //utoa(ret, (unsigned int)val2, 10);
        sprintf (ret, "0x%X", val2);
        con_printf(ret); con_printf("\r\n");
    }
    return val2;
}
void sensTemp_stop(unsigned char sens_x){
    //nothing to do
}

//*****************************************************************************************
void writeSensTemp(unsigned char sens_x, unsigned char address, unsigned char data){
//    int max;
//    for(max=0x0FFF;max>0;max--){
////TODO:        if( I2C3SlaveReady( (sens_x&0b11111110) )==1 ){break;}
//    }

    char DATA[1];
    DATA[0] = (char)data;
    unsigned int N = 1;
    char _address[] = {sens_x, (char)address};
    i2c3_master_fputs(DATA, N, _address, 2);
}

unsigned char readSensTemp(unsigned char sens_x, unsigned char address){
    int max;
    for(max=0x0FFF;max>0;max--){
//TODO        if( I2C3SlaveReady( (sens_x&0b11111110) )==1 ){break;}
    }

    unsigned int N=1;
    char BUFF[1];
    char _address[] = {sens_x, (char)address};

    i2c3_master_fgets(BUFF, N, _address, 2);

    return (unsigned char)(BUFF[0]);
}
