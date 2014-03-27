/**
 * @file  dataRepository.h
 * @author Tomas Opazo T
 * @date 2013
 * @copyright GNU Public License.
 *
 * Este header contiene al Status Prepository (variables de estado) y al
 * Data Repository (datos varios). API interfaz para cada caso:
 *
 * Status Repository:
 *      DAT_Cubesatvar
 * 
 * Data Repository:
 *      DAT_FligthPlanBuff
 *      DAT_TeleCmdBuff
 *      DAT_PayloadVar
 *      DAT_GnrlPurpBuff
 *
 */

#ifndef DATA_REPO_H
#define DATA_REPO_H

#include "FreeRTOS.h"
#include "semphr.h"

#include "memSD.h"
#include "memEEPROM.h"

#include "cmdIncludes.h"


//***STATUS REPOSITORY**********************************************************

void dat_onReset_memEEPROM(void);

/**
 * Cubesat's State Variables
 */
typedef enum{
    //PPC => (C&DH subsystem)
    dat_ppc_opMode=0,
    dat_ppc_lastResetSource,
    dat_ppc_hoursAlive,
    dat_ppc_hoursWithoutReset,
    dat_ppc_resetCounter,
    dat_ppc_enwdt,				// 1=WDT Active, 0=WDT Inactive
    dat_ppc_osc,				// holds Current Oscillator
    dat_ppc_MB_nOE_USB_nINT_stat,
    dat_ppc_MB_nOE_MHX_stat,
    dat_ppc_MB_nON_MHX_stat,
    dat_ppc_MB_nON_SD_stat,

    //DEP => (C&DH subsystem)
    dat_dep_ant_deployed,            // 1=already deployed, 0=not deployed yet
    dat_dep_ant_tries,               // Number of tries to deploy antenna
    dat_dep_year,
    dat_dep_month,
    dat_dep_week_day,
    dat_dep_day_number,
    dat_dep_hours,
    dat_dep_minutes,
    dat_dep_seconds,

    //RTC => (C&DH subsystem)
    dat_rtc_year,
    dat_rtc_month,
    dat_rtc_week_day,
    dat_rtc_day_number,
    dat_rtc_hours,
    dat_rtc_minutes,
    dat_rtc_seconds,

    //EPS => (Energy subsystem)
    dat_eps_bat0_voltage,
    dat_eps_bat0_current,
    dat_eps_bus5V_current,
    dat_eps_bus3V_current,
    dat_eps_bus_battery_current,
    dat_eps_bat0_temp,
    dat_eps_panel_pwr,
    dat_eps_status,
    dat_eps_soc,
    dat_eps_socss,
    dat_eps_state_flag,
    dat_eps_charging,

    //TRX => (Communication subsystem)
    dat_trx_frec_tx,          // TX Freq
    dat_trx_frec_rx,          // RX Freq
    dat_trx_opmode,           // Operation mode
    dat_trx_temp_hpa,         // Temp of HPA
    dat_trx_temp_mcu,         // Temp of MCU
    dat_trx_rssi,             // RSSI, Signal level
    dat_trx_rssi_mean,        // RSSI_MEAN
    dat_trx_beacon_pwr,       // Beacon power
    dat_trx_telemetry_pwr,    // Telemetry Power
    dat_trx_status_tc,        // Status Register of TC
    dat_trx_count_tm,         // number of sended TM
    dat_trx_count_tc,         // number of received TC
    dat_trx_lastcmd_day,      // day of the last received tc (since 1/1/00)
    // Cmd buffer control
    dat_trx_newTcFrame,       // Exist any unprocessed TcFram?
    dat_trx_newCmdBuff,       // Exist unprocessed CMD in the ernal buffer
    //trx_repo_telecmd[DAT_MAX_BUFF_TELECMD],   // Assuming that each Telecommand is of the form: [cmdId|param]

    //FLIGHT PLAN
    dat_fpl_index,            // Indice del flight plan que sera editado

    //memSD
    dat_msd_status,

    //PAYLOAD
    dat_pay_lagmuirProbe_perform,
    dat_pay_sensTemp_perform,
    dat_pay_gps_perform,
    dat_pay_expFis_perform,
    dat_pay_camera_perform,
    dat_pay_gyro_perform,
    dat_pay_tmEstado_perform,
    dat_pay_test1_perform,
    dat_pay_test2_perform,

    //*************
    dat_cubesatVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}DAT_CubesatVar;                // SUCHAI's most important variables


void dat_setCubesatVar(DAT_CubesatVar indxVar, int value);
int dat_getCubesatVar(DAT_CubesatVar indxVar);
void dat_onResetCubesatVar(void);
void dat_init_ppc_lastResetSource(void);
void dat_reset_pay_i_performVar(void);

#define DAT_PPC_OPMODE_NORMAL   (0) /* Operacion normal*/
#define DAT_PPC_OPMODE_RSSI     (1) /* No hay telecomandos, solo usa RSSI*/
#define DAT_PPC_OPMODE_FAIL     (2) /* Falla total */

//***************************************************************************************************************

/**
 * This function assigns the Memory Map of the external memSD
 */
void dat_onReset_memSD(void);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat telecomand buffer

typedef enum _DAT_TeleCmdBuff{
    //*************
    dat_tcb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_TeleCmdBuff;

/* Max internal (not Transceiver mem) TeleCMD buffer length considering TC in
 * format [0xTCID|0xARGM|0xSTOP] in frames of 63 bytes each one. So 10,5
 * commands max, that means 40 bytes or 10cmd + 10 param */
#define DAT_MAX_BUFF_TELECMD SCH_DATAREPOSITORY_MAX_BUFF_TELECMD

int dat_getTelecmdBuff(int indx);
void dat_setTelecmdBuff(int indx, int data);
int dat_onResetTelecmdBuff(void);
void dat_erase_TeleCmdBuff(void);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat fligthPlan

typedef enum _DAT_FligthPlanBuff{
    //*************
    dat_fpb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_FligthPlanBuff;

DispCmd dat_getFlightPlan(unsigned int index);
int dat_setFlightPlan_cmd(unsigned int index, unsigned int cmdID);
int dat_setFlightPlan_param(unsigned int index, int param);
int dat_onResetFlightPlan(void);
void dat_erase_FlightPlanBuff(void);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat Payload buffer/data

/**
 * Enum que contiene los Payloads en el SUCHAI. Para agregar uno, se debe:
 * 1) Agergar el dat_pay_xxx a #DAT_Payload
 * 2) Agregar dat_pay_xxx_perform a #DAT_cubesatVar
 * 3) Agregar case a <dat_pay_i_to_performVar>"()"
 * 4) hacer funciones pay_init_xxx, pay_take_xxx y pay_stop_xxx en \sa cmdPayload.c
 * 5) Agregar las funciones de 4) en los case de <pay_currentStateLogic>"()"
 *
 */
typedef enum _DAT_Payload{
    dat_pay_lagmuirProbe=0,
    dat_pay_test2,
    dat_pay_gps,
    dat_pay_camera,     //pay_i=3
    dat_pay_sensTemp,   
    dat_pay_gyro,       //pay_i=5
    dat_pay_tmEstado,
    dat_pay_test1,
    dat_pay_expFis,     //pay_i=8
    //*************
    dat_pay_last_one    //Se utiliza para marcar el largo del arreglo.
                        //Y para indicar el ID de TM de CUbesatVar
}DAT_Payload;

//Retorna FALSE si el buffer se llena, TRUE si todo OK
BOOL dat_setPayloadVar(DAT_Payload pay_i, int value);
//Retorna FALSE si el indiex es invalido, TRUE si todo OK
BOOL dat_getPayloadVar(DAT_Payload pay_i, unsigned int indx, int *value);
//Inicializa la estructura de data payload
void dat_onResetPayloadVar(void);
//Inicializa la estructura de data payload
void dat_resetPayloadBuffer(DAT_Payload pay_i, unsigned int maxIndx, int mode);
//Inicializa la estructura de data payload
BOOL dat_isFullPayloadBuffer(DAT_Payload pay_i);

//Funciones Auxiliares
//Setea el valor del ultimo/maximo indice del buffer de cierto payload
void dat_setMaxPayIndx(DAT_Payload pay_i, unsigned int lastIndx);
//Obtiene el valor del ultimo/maximo indice del buffer de cierto payload
unsigned int dat_getMaxPayIndx(DAT_Payload pay_i);
//Setea el valor del indice actual del buffer de cierto payload
void dat_setNextPayIndx(DAT_Payload pay_i, unsigned int nextIndx);
//Obtiene el valor del indice actual del buffer de cierto payload
unsigned int dat_getNextPayIndx(DAT_Payload pay_i);
void dat_memSD_BlockErase(unsigned long block_address);
void dat_erase_pay_i_buff(DAT_Payload pay_i);
unsigned long dat_pay_i_to_block(DAT_Payload pay_i);
DAT_CubesatVar dat_pay_i_to_performVar(DAT_Payload pay_i);
//*************************************************************************************************

/**
 * Enum que contiene los General Purpose Buffers del SUCHAI.
 */
typedef enum _DAT_GnrlPurpBuff{
    dat_gpb_expFis_f0=0,
    dat_gpb_expFis_f1,
    dat_gpb_expFis_f2,
    dat_gpb_expFis_f3,
    dat_gpb_expFis_f4,
    dat_gpb_expFis_f5,
    dat_gpb_expFis_f6,
    dat_gpb_expFis_f7,
    dat_gpb_expFis_f8,
    dat_gpb_expFis_f9,
    dat_gpb_expFis_hist,

    dat_gpb_test1,
    dat_gpb_test2,
    //*************
    dat_gpb_last_one    //Se utiliza para marcar el largo del arreglo.
                        //Y para indicar el ID de TM de CUbesatVar
}DAT_GnrlPurpBuff;

unsigned long dat_gpb_i_to_block(DAT_GnrlPurpBuff gpb_i);
//Set
void dat_setGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx, int value);
//Get 
int dat_getGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx);

#endif // DATA_REPO_H
