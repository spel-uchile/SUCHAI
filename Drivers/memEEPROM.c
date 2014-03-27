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

void writeEEPROM1(unsigned char address, char data){
    if( i2c1_slave_ready(MEP_EEPROM_ID, 0x0FFF) == 0 )
        return;

    char _address[] = {MEP_EEPROM_ID, (char)address};
    i2c1_master_fputs(&data, 1, _address, 2);
}

unsigned char readEEPROM1(unsigned char address){
    if( i2c1_slave_ready(MEP_EEPROM_ID, 0x0FFF) == 0 )
        return 0;

    char ret;
    char _address[] = {MEP_EEPROM_ID, (char)address};
    i2c1_master_fgets(&ret, 1, _address, 2);
    return (unsigned char)ret;
}

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

