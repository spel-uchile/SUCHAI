/**
 * @file  cmdDRP.h
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 */

#ifndef CMD_SRP_H
#define CMD_SRP_H

#include "FreeRTOS.h" //vTaskDelay
#include "task.h"

#include "dataRepository.h"
//#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para printf

/* Necesarios para update */
#include "cmdPPC.h"
#include "cmdTRX.h"
#include "cmdEPS.h"
#include "cmdRTC.h"
#include "cmdDRP.h"
#include "cmdTHK.h"


/**
 * Lista de comandos disponibles.
 */
typedef enum{
    //de debug
    srp_id_debug=((unsigned int)SCH_CMD_SRP*0x100), ///< @cmd_first

    //De display
    srp_id_print_MemEEPROM_Vars, ///< @cmd  0x9001 
    srp_id_print_STA_stateVar, ///< @cmd

    //De update en general de dat_stateVar
    srp_id_increment_STA_stateVar_hoursWithoutReset, ///< @cmd
    srp_id_increment_STA_stateVar_hoursAlive, ///< @cmd
    srp_id_increment_STA_stateVar_nSended_tm, ///< @cmd
    srp_id_increment_STA_stateVar_nReceived_tc, ///< @cmd
            
    //De EBF
    srp_id_executeBeforeFlight, ///< @cmd
    srp_id_memEEPROM_initial_state, ///< @cmd

   //*************
    srp_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}SRP_CmdIndx;

#define SRP_NCMD ((unsigned char)srp_id_last_one)
void srp_onResetCmdSRP();

//Commands
//****************************************************
int srp_debug(void *param);
int srp_print_STA_stateVar(void *param);
int srp_print_MemEEPROM_Vars(void *param);
// command's to update dat_stateVar
int srp_increment_STA_stateVar_hoursWithoutReset(void *param);
int srp_increment_STA_stateVar_hoursAlive(void *param);
int srp_increment_STA_stateVar_nSended_tm(void *param);
int srp_increment_STA_stateVar_nReceived_tc(void *param);

int srp_executeBeforeFlight(void *param);
int srp_memEEPROM_initial_state(void * param);

//funciones auxiliares
//****************************************************
void srp_debug4(void);

//PPC
void srp_ppc_enwdt(void);
void srp_ppc_osc(void);
void srp_ppc_MB_nOE_USB_nINT_stat(void);
void srp_ppc_MB_nOE_MHX_stat(void);
void srp_ppc_MB_nON_MHX_stat(void);
void srp_ppc_MB_nON_SD_stat(void);
//RTC
void srp_rtc_year(void);
void srp_rtc_month(void);
void srp_rtc_week_day(void);
void srp_rtc_day_number(void);
void srp_rtc_hours(void);
void srp_rtc_minutes(void);
void srp_rtc_seconds(void);
//EPS
void srp_eps_bat0_voltage(void);
void srp_eps_bat0_current(void);
void srp_eps_bus5V_current(void);
void srp_eps_bus3V_current(void);
void srp_eps_bus_battery_current(void);
void srp_eps_bat0_temp(void);
void srp_eps_panel_pwr(void);
void srp_eps_status(void);
void srp_eps_soc(void);
void srp_eps_socss(void);
void srp_eps_state_flag(void);
void srp_eps_charging(void);

//**********************************************************************************
//Estas no son comandos , pero si funciones llamadas SOLO desde taskDeployment
// Functions that works with persistent memory
//**********************************************************************************



#endif /* CMD_SRP_H */
