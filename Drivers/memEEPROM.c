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

#include "memEEPROM.h"

void writeEEPROM1(unsigned char address, unsigned char data){
    int max;
    for(max=0x0FFF;max>0;max--){
        if( I2C1SlaveReady(MEP_EEPROM1_IDW)==1 ){break;}
    }

    unsigned char DATA[1];
    DATA[0]=data;
    unsigned int N=1;
    I2C1SendChar(MEP_EEPROM1_IDW, address, DATA, N);
}

unsigned char readEEPROM1(unsigned char address){
    int max;
    for(max=0x0FFF;max>0;max--){
        if( I2C1SlaveReady(MEP_EEPROM1_IDW)==1 ){break;}
    }

    unsigned int N=1;
    unsigned char BUFF[1];
    unsigned char MODE=0;

    I2C1ReadChar(MEP_EEPROM1_IDW, address, MEP_EEPROM1_IDR, BUFF, N, MODE);
    return BUFF[0];
}

//****************************************************************************
void writeIntEEPROM1(unsigned char indx, int value){
    if(indx>0x7F){return;}    //memEEPROM es de 256 Byte => 127int

    unsigned char dataLSB=(unsigned char)value;
    unsigned char dataMSB=(unsigned char)(value>>8);

    writeEEPROM1(indx, dataLSB);
    /*
    char ret[6];
    unsigned long i; for(i=0xFFF;i>0;i--){
        int a;
        unsigned long i2; for(i2=0xF;i2>0;i2--){
            a++;
        }
        a=a++;
        ret[1]=a;
    }*/
    writeEEPROM1(255-indx, dataMSB);

}
int readIntEEPROM1(unsigned char indx){
    if(indx>0x7F){return 0;}    //memEEPROM es de 256 Byte => 127int

    unsigned char dataLSB = readEEPROM1(indx);
    unsigned char dataMSB = readEEPROM1(255-indx);

    int value=dataMSB;
    value=(value<<8)|( (unsigned int)(dataLSB) );

    return value;
}

