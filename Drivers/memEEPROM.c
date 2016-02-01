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

void writeEEPROM(unsigned char eeprom_i, unsigned char address, char data){
    if( i2c1_slave_ready(eeprom_i, 0x0FFF) == 0 ){
        printf("i2c1_slave_ready(MEP_EEPROM_ID, 0x0FFF) == 0 \r\n");
        return;
    }

    char _address[] = {eeprom_i, (char)address};
    i2c1_master_fputs(&data, 1, _address, 2);
}

unsigned char readEEPROM(unsigned char eeprom_i, unsigned char address){
    if( i2c1_slave_ready(eeprom_i, 0x0FFF) == 0 ){
        printf("i2c1_slave_ready(MEP_EEPROM_ID, 0x0FFF) == 0 \r\n");
        return 0;
    }

    char ret;
    char _address[] = {eeprom_i, (char)address};
    i2c1_master_fgets(&ret, 1, _address, 2);
    return (unsigned char)ret;
}

void writeIntEEPROM(unsigned char eeprom_i, unsigned char indx, int value){
    if(indx>0x7F){return;}    //memEEPROM es de 256 Byte => 127int

    unsigned char dataLSB=(unsigned char)value;
    unsigned char dataMSB=(unsigned char)(value>>8);

    writeEEPROM(eeprom_i, indx, dataLSB);
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
    writeEEPROM(eeprom_i, 255-indx, dataMSB);
}
int readIntEEPROM(unsigned char eeprom_i, unsigned char indx){
    int value;
    if(indx>0x7F){return 0;}    //memEEPROM es de 256 Byte => 127int

    unsigned char dataLSB = readEEPROM(eeprom_i, indx);
    unsigned char dataMSB = readEEPROM(eeprom_i, 255-indx);

    value=dataMSB;
    value=(value<<8)|( (unsigned int)(dataLSB) );

    return value;
}

/**
 * Funcion a llamar luego de un Reset del PIC, y antes de usar la memEEPROM.
 */
int mem_init_EEPROM(void){   
    printf("===============================\r\n");
    printf("MemEEPROM_Vars content: \r\n");
    printf("===============================\r\n");

    MemEEPROM_Vars indxVar; int val;
    for(indxVar=0; indxVar<mem_last_one; indxVar++){
        val = mem_getVar(indxVar);
        printf("    * mem_getVar(%d = %s) = 0x%X = %d \r\n", indxVar, mem_MemEEPROM_VarsToString(indxVar), val, val);
    }
    printf("===============================\r\n");
    
    return mem_EEPROM_isAlive();
}
/**
 * Check if MemEEPROM is working
 * @return  1 = is working 0 = not working
 */
int mem_EEPROM_isAlive(void)
{
    //check if working normally

    int indxVar;    //DAT_StateVar indxVar;
    int wvalue,rvalue;

    //indxVar = sta_MemEEPROM_testVal;
    indxVar = 0;
    
    wvalue = 5005;
    rvalue = 0;
    mem_setVar(indxVar, wvalue);
    rvalue = mem_getVar(indxVar);
    if(rvalue!=wvalue){return 0;}

    wvalue = 5006;
    rvalue = 0x00;
    mem_setVar(indxVar, wvalue);
    rvalue = mem_getVar(indxVar);
    if(rvalue!=wvalue){return 0;}

    return 1;
}

void mem_setVar(MemEEPROM_Vars var_i, int value){
    #if(MEM_USE_MEMSD_nMEMEEPROM == 0)
        writeIntEEPROM(MEP_EEPROM1_ID, (unsigned char)var_i, value);
        //writeIntEEPROM1(MEP_EEPROM2_ID, (unsigned char)var_i, value);
    #else
        msd_setVar_256BlockExtMem(DAT_RESERVED_BLOCK1, (unsigned int)var_i, value);
    #endif
}


int mem_getVar(MemEEPROM_Vars var_i){
    int value;
    #if(MEM_USE_MEMSD_nMEMEEPROM == 0)
        value = readIntEEPROM(MEP_EEPROM1_ID, (unsigned char)var_i);
        //value = readIntEEPROM1(MEP_EEPROM2_ID, (unsigned char)var_i);
    #else
        msd_getVar_256BlockExtMem(DAT_RESERVED_BLOCK1, (unsigned int)var_i, &value);
    #endif
    
    return value;
}

char * mem_MemEEPROM_VarsToString(MemEEPROM_Vars var_i){
    char *pc;
    switch(var_i){
        //address 0 is ALWAYS used to make sure MemEEPROM is working r/w values (isAlive)
        case mem_MemEEPROM_testVal:
            pc = "mem_MemEEPROM_testVal";
            break;
        //PPC => (C&DH subsystem)
        case mem_ppc_opMode:
            pc = "mem_ppc_opMode";
            break;
        case mem_ppc_hoursAlive:
            pc = "mem_ppc_hoursAlive";
            break;
        case mem_ppc_resetCounter:
            pc = "mem_ppc_resetCounter";
            break;
        //DEP => (C&DH subsystem)
        case mem_dep_ant_deployed:
            pc = "mem_dep_ant_deployed";
            break;
        case mem_dep_ant_tries:
            pc = "mem_dep_ant_tries";
            break;
        case mem_dep_year:
            pc = "mem_dep_year";
            break;
        case mem_dep_month:
            pc = "mem_dep_month";
            break;
        case mem_dep_week_day:
            pc = "mem_dep_week_day";
            break;
        case mem_dep_day_number:
            pc = "mem_dep_day_number";
            break;
        case mem_dep_hours:
            pc = "mem_dep_hours";
            break;
        case mem_dep_minutes:
            pc = "mem_dep_minutes";
            break;
        case mem_dep_seconds:
            pc = "mem_dep_seconds";
            break;
            //Flight Plan
        case mem_fpl_index:
            pc = "mem_fpl_index";
            break;
            //PAYLOAD
        case mem_pay_langmuirProbe_state:
            pc = "mem_pay_langmuirProbe_state";
            break;
        case mem_pay_sensTemp_state:
            pc = "mem_pay_sensTemp_state";
            break;
        case mem_pay_gps_state:
            pc = "mem_pay_gps_state";
            break;
        case mem_pay_expFis_state:
            pc = "mem_pay_expFis_state";
            break;
        case mem_pay_camera_state:
            pc = "mem_pay_camera_state";
            break;
        case mem_pay_gyro_state:
            pc = "mem_pay_gyro_state";
            break;
        case mem_pay_tmEstado_state:
            pc = "mem_pay_tmEstado_state";
            break;
        case mem_pay_battery_state:
            pc = "mem_pay_battery_state";
            break;
        case mem_pay_debug_state:
            pc = "mem_pay_debug_state";
            break;
            //TRX
        case mem_trx_count_tm:
            pc = "mem_trx_count_tm";
            break;
        case mem_trx_count_tc:
            pc = "mem_trx_count_tc";
            break;
        case mem_trx_day_last_tc:
            pc = "mem_trx_day_last_tc";
            break;
        case mem_trx_beacon_period:
            pc = "mem_trx_beacon_period";
            break;
        case mem_trx_beacon_bat_lvl:
            pc = "mem_trx_beacon_bat_lvl";
            break;
        case mem_trx_rx_baud:
            pc = "mem_trx_rx_baud";
            break;
        case mem_trx_tx_baud:
            pc = "mem_trx_tx_baud";
            break;
        default:
            pc = "unknown MemEEPROM_Vars";
            break;
    }
    return pc;
}

void mem_test_EEPROM(void){
    printf("mem_test_EEPROM() \r\n");
    
    unsigned int address=0;
    unsigned int data=0xA000;
    char buffer[10];

    printf("(Destructive) Testing memEEPROM1 ..\r\n");
    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++, data++){
        //printf("testing address j="); Hex16ToAscii( address); printf(buffer); printf("\n");

        printf("writing: ");
        //mem_setVar(address, data);
        writeIntEEPROM(MEP_EEPROM1_ID, address, data);
        itoa(buffer, address,10); printf("value["); printf(buffer); printf("]=");
        itoa(buffer, data,10); printf(buffer); printf("  |  ");

        printf("reading: ");
        //unsigned int res = mem_getVar(address);
        unsigned int res = readIntEEPROM(MEP_EEPROM1_ID, address);
        itoa(buffer, address,10); printf("value["); printf(buffer); printf("]=");
        itoa(buffer, res,10); printf(buffer); printf("  |  ");

        printf("comparing: ");
        if(data==res){ printf("ok"); }
        else{ printf("fail"); }
        printf("\n");
        
        ClrWdt();
    }
    
    printf("(Destructive) Testing memEEPROM2 ..\r\n");
    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++, data++){
        //printf("testing address j="); Hex16ToAscii( address); printf(buffer); printf("\n");

        printf("writing: ");
        //mem_setVar(address, data);
        writeIntEEPROM(MEP_EEPROM2_ID, address, data);
        itoa(buffer, address,10); printf("value["); printf(buffer); printf("]=");
        itoa(buffer, data,10); printf(buffer); printf("  |  ");

        printf("reading: ");
        //unsigned int res = mem_getVar(address);
        unsigned int res = readIntEEPROM(MEP_EEPROM2_ID, address);
        itoa(buffer, address,10); printf("value["); printf(buffer); printf("]=");
        itoa(buffer, res,10); printf(buffer); printf("  |  ");

        printf("comparing: ");
        if(data==res){ printf("ok"); }
        else{ printf("fail"); }
        printf("\n");
        
        ClrWdt();
    }
}