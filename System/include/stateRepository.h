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
    sta_trx_count_tm,         // number of sended TM
    sta_trx_count_tc,         // number of received TC
    sta_trx_day_last_tc,      // day of the last received tc (since 1/1/00)
    sta_trx_beacon_period,    // Beacon period in seconds
    sta_trx_beacon_bat_lvl,   // Batery voltage required to transmit beacon
    sta_trx_rx_baud,          // RX baudrate
    sta_trx_tx_baud,          // TX baudrate

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
    sta_stateVar_last_one     //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}STA_StateVar;                // SUCHAI's most important variables


//void sta_set_stateVar(STA_StateVar indxVar, int value);  //deprecated, OCt 2014
int sta_get_stateVar(STA_StateVar indxVar);
void sta_onReset_stateRepo(void);


STA_StateVar sta_DAT_Payload_Buff_to_STA_StateVar(DAT_Payload_Buff pay_i);
//STA_StateVar sta_DAT_Payload_Buff_to_STA_StateVar(int pay_i);

//debug funcitons
char *sta_varToString(STA_StateVar var_i);

//#define SRP_PAY_XXX_STATE_INACTIVE    0x0000
//#define SRP_PAY_XXX_STATE_ACTIVE      0x0001
//#define SRP_PAY_XXX_STATE_RUN_INIT    0x0002
//#define SRP_PAY_XXX_STATE_RUN_TAKE    0x0003
//#define SRP_PAY_XXX_STATE_RUN_STOP    0x0004
//#define SRP_PAY_XXX_STATE_WAITING_TX  0x0005

#endif // STATE_REPO_H

