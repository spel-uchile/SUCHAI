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
    // Bus Hw status (connected trough the PC/104 to the OBC -PIC24-)
    sta_RTC_isAlive=0,
    sta_TRX_isAlive,
    sta_EPS_isAlive,
    sta_MemEEPROM_isAlive,
    sta_MemSD_isAlive,
    sta_Antenna_isDeployed,  // var number 6

    // Payload Hw status (connected trough the PC/104 to the OBC -PIC24-)
    sta_pay_lagmuirProbe_isAlive,
    sta_pay_sensTemp_isAlive,
    sta_pay_gps_isAlive,
    sta_pay_expFis_isAlive,
    sta_pay_camera_isAlive,
    sta_pay_gyro_isAlive,
    sta_pay_tmEstado_isAlive,
    sta_pay_battery_isAlive,
    sta_pay_debug_isAlive,
    sta_pay_lagmuirProbe_isDeployed,

    //FLIGHT PLAN => (C&DH subsystem)
    sta_fpl_index,            // Indice del flight plan que sera editado

    //PPC => (C&DH subsystem)
    sta_ppc_opMode,
    sta_ppc_lastResetSource,
    sta_ppc_hoursAlive,
    sta_ppc_hoursWithoutReset,
    sta_ppc_resetCounter,
    sta_ppc_wdt,				// 1=WDT Active, 0=WDT Inactive
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
    sta_trx_opmode,           // Operation mode
    sta_trx_temp_hpa,         // Temp of HPA
    sta_trx_temp_mcu,         // Temp of MCU
    sta_trx_rssi,             // RSSI, Signal level
    sta_trx_rssi_mean,        // RSSI_MEAN
    sta_trx_status_tc,        // Status Register of TC
    sta_trx_count_tm,         // number of sended TM
    sta_trx_count_tc,         // number of received TC
    sta_trx_lastcmd_day,      // day of the last received tc (since 1/1/00)
    // Cmd buffer control
    sta_trx_newTcFrame,       // Exist any unprocessed TcFram?
    sta_trx_newCmdBuff,       // Exist unprocessed CMD in the external buffer
    //trx_repo_telecmd[STA_MAX_BUFF_TELECMD],   // Assuming that each Telecommand is of the form: [cmdId | param]

    //PAYLOAD
    sta_pay_lagmuirProbe_state,
    sta_pay_sensTemp_state,
    sta_pay_gps_state,
    sta_pay_expFis_state,
    sta_pay_camera_state,
    sta_pay_gyro_state,
    sta_pay_tmEstado_state,
    sta_pay_battery_state,
    sta_pay_debug_state,

    //*************
    sta_cubesatVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}STA_CubesatVar;                // SUCHAI's most important variables


void sta_setCubesatVar(STA_CubesatVar indxVar, int value);
int sta_getCubesatVar(STA_CubesatVar indxVar);
void sta_onResetStatRepo(void);


//STA_CubesatVar dat_pay_i_to_performVar(DAT_Payload pay_i);
STA_CubesatVar sta_pay_i_to_performVar(int pay_i);

//debug funcitons
char *varToString(STA_CubesatVar var_i);

#define SRP_PAY_XXX_PERFORM_INACTIVE    0x0000
#define SRP_PAY_XXX_PERFORM_ACTIVE      0x0001

#endif // STATUS_REPO_H

