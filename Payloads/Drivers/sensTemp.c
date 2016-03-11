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


void sensTemp_write_reg(unsigned char sens_x, unsigned char address, unsigned char data){
//    int max;
//    for(max=0x0FFF;max>0;max--){
////TODO:        if( I2C3SlaveReady( (sens_x&0b11111110) )==1 ){break;}
//    }

    char DATA[1];
    DATA[0] = (char)data;
    unsigned int N = 1;
    char _address[] = {(char)sens_x, (char)address};
    i2c3_master_fputs(DATA, N, _address, 2);
}

unsigned char sensTemp_read_reg(char dir, char reg){
    char ret;
    char address[] = {dir, reg};
    i2c3_master_fgets(&ret, 1, address, 2);
    return (unsigned char)ret;
}
//******************************************************************************
BOOL sensTemp_isAlive(unsigned char sens_x){
    char w_reg, r_reg;
    BOOL result = TRUE;

    //        76543210
    w_reg = 0b01101000;
    r_reg = 0;
    sensTemp_write_reg(sens_x, STx_REG_CONFIG, w_reg);
    r_reg = sensTemp_read_reg( (char)sens_x, STx_REG_CONFIG);
    //        76543210
    r_reg = 0b01111111 & r_reg;     //erase and ignore OS/ALERT bit
    printf("sensTemp_isAlive: SensTemp[%d] = ", sens_x);
    if(r_reg != w_reg){ 
        result &= FALSE;
        printf("FALSE !!\n");
    }else{
        printf("TRUE !!\n");
    }

    //        76543210
    w_reg = 0b01100000;
    r_reg = 0;
    sensTemp_write_reg(sens_x, STx_REG_CONFIG, w_reg);
    r_reg = sensTemp_read_reg( (char )sens_x, STx_REG_CONFIG);
    //        76543210
    r_reg = 0b01111111 & r_reg;     //erase and ignore OS/ALERT bit
    printf("sensTemp_isAlive: SensTemp[%d] = ", sens_x);
    if(r_reg != w_reg){ 
        result &= FALSE;
        printf("FALSE !!\n");
    }else{
        printf("TRUE !!\n");
    }

    return result;
}

BOOL sensTemp_init(unsigned char sens_x){
    if( sensTemp_isAlive(sens_x)==FALSE ){return FALSE;}

    //                     76543210
    unsigned char data = 0b11100101;    //12 bit resolution, shutdown mode
    sensTemp_write_reg(sens_x, STx_REG_CONFIG, data);

//    unsigned char r_reg = sensTemp_read_reg( (char )sens_x, STx_REG_CONFIG);
//    printf("config = 0x%X\n", r_reg);

    return TRUE;
}
int sensTemp_take(unsigned char sens_addr, BOOL verb){
    //                     76543210
    unsigned char data = 0b11100101;    //12 bit resolution, shutdown mode
    sensTemp_write_reg(sens_addr, STx_REG_CONFIG, data);

    __delay_ms(400);    //convertion time for 12bits => 320ms

    unsigned char val[2];
    unsigned char reg=STx_REG_TEMP_READ;
    char address[] = {sens_addr, (char)reg};

    i2c3_master_fgets((char *)val, 2, address, 2);

    int res;
    unsigned int val2 = val[0];
    val2 = (val2<<8);
    val2 = (val2)|((unsigned int)val[1]);
    val2 = (val2>>4);
    
    if(verb){
        int st_i;
        res = (signed int)val2;
        switch(sens_addr){
            case ST1_ADDRESS:
                st_i = 1;
            break;
            case ST2_ADDRESS:
                st_i = 2;
            break;
            case ST3_ADDRESS:
                st_i = 3;
            break;
            case ST4_ADDRESS:
                st_i = 4;
            break;
        }
        if(res>0x7FF){
            res = (4088 -res);
        }
        printf("SensTemp[%d] = 0x%X => %+f [grd C]\n", st_i, val2, ( (128.0/2047.0)*res) );

//        unsigned char r_reg = sensTemp_read_reg( (char )sens_addr, STx_REG_CONFIG);
//        printf("config = 0x%X\n", r_reg);
    }
    return val2;
}
void sensTemp_stop(unsigned char sens_x){
    //nothing to do
}

//*****************************************************************************************
unsigned char readSensTemp(unsigned char sens_x, unsigned char address){
//    i2c3_slave_ready( sens_x, 0x00FFFFFF );

    unsigned int N=1;
    char BUFF[1];
    char _address[] = {sens_x, (char)address};

    i2c3_master_fgets(BUFF, N, _address, 2);

    return (unsigned char)(BUFF[0]);
}
