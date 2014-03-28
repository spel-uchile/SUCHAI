/**
 * @file  cmdDRP.h
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados al repositorio de
 * datos. Comandos para accesar a las variables de estado y a la memoria externa.
 *
 * Estos comandos seran SOLO aquellos relacionados con dataRepository, por tanto tendran que ver con:
 * actualizacion                =>  update
 * visualizacion en consola     =>  print
 * condiciones iniciales        =>  executeBeforFligth
 * modificaion de FlightPlan    =>  setFlightPlan
 *
 * Lo anterior para las estructuras de dataRepository:
 * dat_CubesatVar
 * dat_TelecmdBuff
 * dat_FligthPlan
 * dat_PayloadVar
 */

#ifndef CMD_DRP_H
#define CMD_DRP_H

#include "FreeRTOS.h" //vTaskDelay
#include "task.h"

#include "dataRepository.h"
#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf

/* Necesarios para update */
#include "cmdPPC.h"
#include "cmdTRX.h"
#include "cmdEPS.h"
#include "cmdRTC.h"


/**
 * Lista de comandos disponibles.
 */
typedef enum{
    //De update en general de dat_CubesatVar
    drp_id_updateAll_sta_CubesatVar=((unsigned int)SCH_CMD_DRP*0x100), ///< @cmd_first
    drp_id_update_sta_CubesatVar_hoursWithoutReset, ///< @cmd
    drp_id_update_sta_CubesatVar_opMode, ///< @cmd
    drp_id_update_sta_CubesatVar_hoursAlive, ///< @cmd
    drp_id_update_sta_CubesatVar_nSended_tm, ///< @cmd
    drp_id_update_sta_CubesatVar_nReceived_tc, ///< @cmd
    drp_id_update_sta_CubesatVar_trx_rssi, ///< @cmd
    drp_id_update_sta_CubesatVar_trx_rssi_mean, ///< @cmd

    //De display
    drp_id_print_sta_CubesatVar, ///< @cmd
    drp_id_print_dat_TelecmdBuff, ///< @cmd
    drp_id_print_dat_FlightPlan, ///< @cmd
    drp_id_print_dat_PayloadIndxs, ///< @cmd
    drp_id_print_dat_PayloadVar, ///< @cmd

    //De mem ext (EEPROM y SD)
    drp_id_executeBeforeFlight, ///< @cmd

    //de debug
    drp_id_debug, ///< @cmd
    
    //Flight Plan
    drp_id_fpl_set_index, ///< @cmd
    drp_id_fpl_set_cmd, ///< @cmd
    drp_id_fpl_set_param, ///< @cmd

    //De TRX
    drp_id_trx_newTcFrame, ///< @cmd
    drp_id_trx_newCmdBuff, ///< @cmd

    //*************
    drp_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}DRP_CmdIndx;

#define DRP_NCMD ((unsigned char)drp_id_last_one)
void drp_onResetCmdDRP();


// command's to print dat_CubesatVar
int drp_print_sta_CubesatVar(void *param);
int drp_print_dat_TelecmdBuff(void *param);
int drp_print_dat_FlightPlan(void *param);
int drp_print_dat_PayloadIndxs(void *param);
int drp_print_dat_PayloadVar(void *param);

// command's to update dat_CubesatVar
int drp_updateAll_sta_CubesatVar(void *param);
int drp_update_sta_CubesatVar_hoursWithoutReset(void *param);
int drp_update_sta_CubesatVar_opMode(void *param);
int drp_update_sta_CubesatVar_hoursAlive(void *param);
int drp_update_sta_CubesatVar_nSended_tm(void *param);
int drp_update_sta_CubesatVar_nReceived_tc(void *param);
int drp_update_sta_CubesatVar_trx_rssi(void *param);
int drp_update_sta_CubesatVar_trx_rssi_mean(void *param);

//comandos para mem ext (SD y EEPROM)
int drp_executeBeforeFlight(void *param);

//de debug
int drp_debug(void *param);

//Comandos para Flight plan
int drp_fpl_set_index(void *param);
int drp_fpl_set_cmd(void *param);
int drp_fpl_set_param(void *param);

//de TRX
int drp_trx_newTcFrame(void *param);
int drp_trx_newCmdBuff(void *param);

//****************************************************
//funciones auxiliares
void drp_sta_CubesatVar_EBF(void);
void drp_dat_FlightPlan_EBF(void);
void drp_dat_TelecmdBuffer_EBF(void);
void drp_memEEPROM_erase(void);

void drp_debug1(void);
void drp_debug2(void);
void drp_debug3(void);
void drp_debug4(void);

void drp_CubesatVar_update(int indxVar);
//PPC
//void drp_ppc_hoursAlive(void); => no existen, pues no se actualizan en updateAll
//void drp_ppc_hoursWithoutReset(void);
void drp_ppc_enwdt(void);
void drp_ppc_osc(void);
void drp_ppc_MB_nOE_USB_nINT_stat(void);
void drp_ppc_MB_nOE_MHX_stat(void);
void drp_ppc_MB_nON_MHX_stat(void);
void drp_ppc_MB_nON_SD_stat(void);

/* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
 * comandos para actualizarlo */
//void drp_dep_ant_deployed(void);
//void drp_dep_ant_tries(void);
//void drp_dep_year(void);
//void drp_dep_month(void);
//void drp_dep_week_day(void);
//void drp_dep_day_number(void);
//void drp_dep_hours(void);
//void drp_dep_minutes(void);
//void drp_dep_seconds(void);

//RTC
void drp_rtc_year(void);
void drp_rtc_month(void);
void drp_rtc_week_day(void);
void drp_rtc_day_number(void);
void drp_rtc_hours(void);
void drp_rtc_minutes(void);
void drp_rtc_seconds(void);

//EPS
void drp_eps_bat0_voltage(void);
void drp_eps_bat0_current(void);
void drp_eps_bus5V_current(void);
void drp_eps_bus3V_current(void);
void drp_eps_bus_battery_current(void);
void drp_eps_bat0_temp(void);
void drp_eps_panel_pwr(void);
void drp_eps_status(void);
void drp_eps_soc(void);
void drp_eps_socss(void);
void drp_eps_state_flag(void);
void drp_eps_charging(void);

//TRX
void drp_trx_frec_tx(void);
void drp_trx_frec_rx(void);
void drp_trx_opmode(void);
void drp_trx_temp_hpa(void);
void drp_trx_temp_mcu(void);
void drp_trx_rssi(void);
int drp_trx_rssi_mean(int arg);
void drp_trx_beacon_pwr(void);
void drp_trx_telemetry_pwr(void);
void drp_trx_status_tc(void);


//**********************************************************************************
//Estas no son comandos , pero si funciones llamadas SOLO desde taskDeployment
// Functions that works with persistent memory
void drp_dep_write_deployed(int dep_state, int dep_tries);
//**********************************************************************************



#endif /* CMD_DRP_H */
