/**
 * @file  dataRepository.h
 * @author Tomas Opazo T
 * @date 2013
 * @copyright GNU Public License.
 *
 */

#ifndef STATE_REPO_H
#define STATE_REPO_H

#include "FreeRTOS.h"
#include "semphr.h"

#include "memSD.h"
#include "memEEPROM.h"

#include "cmdIncludes.h"
//#include "dataRepository.h"

//cmdXXX
#include "cmdTRX.h"
//#include "cmdDRP.h"
#include "cmdTHK.h"
#include "cmdPPC.h"


//***STATUS REPOSITORY**********************************************************
#define STA_PPC_OPMODE_NORMAL       (0) /* Operacion normal*/
#define STA_PPC_OPMODE_SENDING_TM   (1) /* Transceiver is TX */
#define STA_PPC_OPMODE_EXPFIS       (1) /* expFis is running*/
#define STA_PPC_OPMODE_CAMERA       (1) /* Camera is running */

//******************************************************************************
// BUS section
//******************************************************************************

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
    sta_AntSwitch_isOpen,  // var number 6

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

    #if (SCH_EPS_ONBOARD==1)
        //EPS => (Energy subsystem)
        sta_eps_batt_temp_0,
        sta_eps_batt_temp_1,
        sta_eps_battery_voltage,
        sta_eps_panel_current,
        sta_eps_panel_voltage_1,
        sta_eps_panel_voltage_2,
        sta_eps_panel_voltage_3,
        sta_eps_system_current,
    #endif

    /* Revisar de aqui hacia abajo si aun son necesarios !!! */

    #if (SCH_TRX_ONBOARD==1)
        //TRX => (Communication subsystem)
        sta_trx_opmode,           // Operation mode
        sta_trx_count_tm,         // number of sended TM
        sta_trx_count_tc,         // number of received TC
        sta_trx_day_last_tc,      // day of the last received tc (since 1/1/00)
        sta_trx_beacon_period,    // Beacon period in seconds
        sta_trx_beacon_bat_lvl,   // Batery voltage required to transmit beacon
        sta_trx_rx_baud,          // RX baudrate
        sta_trx_tx_baud,          // TX baudrate
    #endif

    //*************
    sta_busStateVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}STA_BusStateVar;                // SUCHAI's most important variables

//void sta_set_stateVar(STA_StateVar indxVar, int value);  //deprecated, OCt 2014
int sta_get_BusStateVar(STA_BusStateVar indxVar);
char *sta_BusStateVarToString(STA_BusStateVar var_i);
void sta_onReset_BusStateRepo(void);

//******************************************************************************
// PAYLOAD section
//******************************************************************************

typedef enum{
    //PAYLOAD
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
    sta_pay_langmuirProbe_isDeployed,

    // FP2 variables
    sta_pay_langmuirProbe_state,
    sta_pay_sensTemp_state,
    sta_pay_gps_state,
    sta_pay_expFis_state,
    sta_pay_camera_state,
    sta_pay_gyro_state,
    sta_pay_tmEstado_state,
    sta_pay_battery_state,
    sta_pay_debug_state,

    //*************
    sta_payStateVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}STA_PayStateVar;                // SUCHAI's most important variables


int sta_get_PayStateVar(STA_PayStateVar indxVar);
char* sta_PayStateVarToString(STA_PayStateVar var_i);
STA_PayStateVar sta_DAT_Payload_Buff_to_STA_PayStateVar(DAT_Payload_Buff pay_i);
void sta_onReset_PayStateRepo(void);
//STA_StateVar sta_DAT_Payload_Buff_to_STA_StateVar(int pay_i);


#endif // STATE_REPO_H

