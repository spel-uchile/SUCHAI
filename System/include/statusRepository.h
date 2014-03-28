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
 *      STA_Cubesatvar
 * 
 * Data Repository:
 *      STA_FligthPlanBuff
 *      STA_TeleCmdBuff
 *      STA_PayloadVar
 *      STA_GnrlPurpBuff
 *
 */

#ifndef STATUS_REPO_H
#define STATUS_REPO_H

#include "FreeRTOS.h"
#include "semphr.h"

#include "memSD.h"
#include "memEEPROM.h"

#include "cmdIncludes.h"
#include "dataRepository.h"


//***STATUS REPOSITORY**********************************************************
#define STA_PPC_OPMODE_NORMAL   (0) /* Operacion normal*/
#define STA_PPC_OPMODE_RSSI     (1) /* No hay telecomandos, solo usa RSSI*/
#define STA_PPC_OPMODE_FAIL     (2) /* Falla total */

/**
 * Cubesat's State Variables
 */
typedef enum{
    sta_mep_testVal=0,   //just to make sure MemEEPROM is working r/w values
    //External hw satus
    sta_RTC_isAlive,
    sta_TRX_isAlive,
    sta_MemEEPROM_isAlive,
    sta_MemSD_isAlive,
    //PPC => (C&DH subsystem)
    sta_ppc_opMode,
    sta_ppc_lastResetSource,
    sta_ppc_hoursAlive,
    sta_ppc_hoursWithoutReset,
    sta_ppc_resetCounter,
    sta_ppc_enwdt,				// 1=WDT Active, 0=WDT Inactive
    sta_ppc_osc,				// holds Current Oscillator
    sta_ppc_MB_nOE_USB_nINT_stat,
    sta_ppc_MB_nOE_MHX_stat,
    sta_ppc_MB_nON_MHX_stat,
    sta_ppc_MB_nON_SD_stat,

    //DEP => (C&DH subsystem)
    sta_dep_ant_deployed,            // 1=already deployed, 0=not deployed yet
    sta_dep_ant_tries,               // Number of tries to deploy antenna
    sta_dep_year,
    sta_dep_month,
    sta_dep_week_day,
    sta_dep_day_number,
    sta_dep_hours,
    sta_dep_minutes,
    sta_dep_seconds,

    //RTC => (C&DH subsystem)
    sta_rtc_year,
    sta_rtc_month,
    sta_rtc_week_day,
    sta_rtc_day_number,
    sta_rtc_hours,
    sta_rtc_minutes,
    sta_rtc_seconds,

    //EPS => (Energy subsystem)
    sta_eps_bat0_voltage,
    sta_eps_bat0_current,
    sta_eps_bus5V_current,
    sta_eps_bus3V_current,
    sta_eps_bus_battery_current,
    sta_eps_bat0_temp,
    sta_eps_panel_pwr,
    sta_eps_status,
    sta_eps_soc,
    sta_eps_socss,
    sta_eps_state_flag,
    sta_eps_charging,

    /* Revisar de aqui hacia abajo si aun son necesarios !!! */

    //TRX => (Communication subsystem)
    sta_trx_frec_tx,          // TX Freq
    sta_trx_frec_rx,          // RX Freq
    sta_trx_opmode,           // Operation mode
    sta_trx_temp_hpa,         // Temp of HPA
    sta_trx_temp_mcu,         // Temp of MCU
    sta_trx_rssi,             // RSSI, Signal level
    sta_trx_rssi_mean,        // RSSI_MEAN
    sta_trx_beacon_pwr,       // Beacon power
    sta_trx_telemetry_pwr,    // Telemetry Power
    sta_trx_status_tc,        // Status Register of TC
    sta_trx_count_tm,         // number of sended TM
    sta_trx_count_tc,         // number of received TC
    sta_trx_lastcmd_day,      // day of the last received tc (since 1/1/00)
    // Cmd buffer control
    sta_trx_newTcFrame,       // Exist any unprocessed TcFram?
    sta_trx_newCmdBuff,       // Exist unprocessed CMD in the ernal buffer
    //trx_repo_telecmd[STA_MAX_BUFF_TELECMD],   // Assuming that each Telecommand is of the form: [cmdId | param]

    //FLIGHT PLAN
    sta_fpl_index,            // Indice del flight plan que sera editado

    //PAYLOAD
    sta_pay_lagmuirProbe_perform,
    sta_pay_sensTemp_perform,
    sta_pay_gps_perform,
    sta_pay_expFis_perform,
    sta_pay_camera_perform,
    sta_pay_gyro_perform,
    sta_pay_tmEstado_perform,
    sta_pay_test1_perform,
    sta_pay_test2_perform,

    //*************
    sta_cubesatVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}STA_CubesatVar;                // SUCHAI's most important variables


void sta_setCubesatVar(STA_CubesatVar indxVar, int value);
int sta_getCubesatVar(STA_CubesatVar indxVar);
void sta_onResetStatRepo(void);

int sta_get_ppc_lastResetSource(BOOL verb);
//STA_CubesatVar dat_pay_i_to_performVar(DAT_Payload pay_i);
STA_CubesatVar dat_pay_i_to_performVar(int pay_i);

#endif // STATUS_REPO_H

