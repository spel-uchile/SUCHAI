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

/**
 * Funcion a llamar luego de un Reset del PIC, y antes de usar la memEEPROM.
 */
int init_memEEPROM(void){
    //nothing to do
    //printf("[init_memEEPROM] mem_last_one = %d\r\n", mem_last_one);
    printf("===============================\r\n");
    printf("MemEEPROM_Vars content: \r\n");
    printf("===============================\r\n");

    MemEEPROM_Vars indxVar; int val;
    for(indxVar=0; indxVar<mem_last_one; indxVar++){
        val = mem_getVar(indxVar);
        printf("    * mem_getVar(%d = %s) = 0x%X = %d \r\n", indxVar, mem_MemEEPROM_VarsToString(indxVar), val, val);
    }
    printf("===============================\r\n");
    
    //check if working normally
    return memEEPROM_isAlive();
}
/**
 * Check if MemEEPROM is working
 * @return  1 = is working 0 = not working
 */
int memEEPROM_isAlive(void)
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

void mem_setVar( unsigned char indxVar, int value){
    //Para el caso de guardar las variables en la memI2C
    writeIntEEPROM1( (unsigned char)indxVar, value);
}


int mem_getVar( unsigned char indxVar){
    int value;
    //Para el caso de obtener las variables de la memI2C
    value = readIntEEPROM1( (unsigned char)indxVar );
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

