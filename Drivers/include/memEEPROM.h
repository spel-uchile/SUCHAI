#ifndef MEM_EEPROM_H
#define MEM_EEPROM_H

#include "i2c_comm.h"

//#define MEP_EEPROM_ID 0b1010000 /* I2C node address (7bit hex) */
#define MEP_EEPROM_ID   0b01010000 /* I2C node address (7bit hex), mem 1 */
//#define MEP_EEPROM_ID   0b01010111 /* I2C node address (7bit hex), mem 2 */

//#define MEP_EEPROM1_IDW 0b10100000 /* I2C node address Write */
//#define MEP_EEPROM1_IDR 0b10100001 /* I2C node address Read */

#define MEP_FIRST_ADDR 0x00
#define MEP_LAST_ADDR 0x7F  //127

/**
 * EEMPROM Variables, this variables MUST be reset-resistant
 */
typedef enum{
    //address 0 is ALWAYS used to make sure MemEEPROM is working r/w values (isAlive)
    mem_MemEEPROM_testVal=0,
    //PPC => (C&DH subsystem)
    mem_ppc_opMode,
    mem_ppc_hoursAlive,
    mem_ppc_resetCounter,

    //DEP => (C&DH subsystem)
    mem_dep_ant_deployed,            // 1=already deployed, 0=not deployed yet
    mem_dep_ant_tries,               // Number of tries to deploy antenna
    mem_dep_year,
    mem_dep_month,
    mem_dep_week_day,
    mem_dep_day_number,
    mem_dep_hours,
    mem_dep_minutes,
    mem_dep_seconds,

    //Flight Plan
    mem_fpl_index,

    //PAYLOAD
    mem_pay_lagmuirProbe_state,
    mem_pay_sensTemp_state,
    mem_pay_gps_state,
    mem_pay_expFis_state,
    mem_pay_camera_state,
    mem_pay_gyro_state,
    mem_pay_tmEstado_state,
    mem_pay_battery_state,
    mem_pay_debug_state,

    //TRX
    mem_trx_count_tm,
    mem_trx_count_tc,
    mem_trx_day_last_tc,
    mem_trx_beacon_period,
    mem_trx_beacon_bat_lvl,
    mem_trx_rx_baud,
    mem_trx_tx_baud,

    //*************
    mem_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}MemEEPROM_Vars;

void writeEEPROM1(unsigned char address, char data);
unsigned char readEEPROM1(unsigned char address);

void writeIntEEPROM1(unsigned char indx, int value);
int readIntEEPROM1(unsigned char indx);

int init_memEEPROM(void);
int memEEPROM_isAlive(void);

void mem_setVar( unsigned char indxVar, int value);
int mem_getVar( unsigned char indxVar);

#endif /* MEM_EEPROM_H   */
