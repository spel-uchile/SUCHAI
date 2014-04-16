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

#ifndef CMD_THK_H
#define CMD_THK_H

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
#include "cmdSRP.h"
#include "cmdPayload.h"


/**
 * Lista de comandos disponibles.
 */
typedef enum{
    //de debug
    thk_id_debug=((unsigned int)SCH_CMD_THK*0x100), ///< @cmd_first

    //Deplyment
    thk_id_deploy_antenna, ///< @cmd
    thk_id_silent_time_and_pictures, ///< @cmd

    //De update en general de dat_CubesatVar
    thk_id_periodicUpdate_STA_CubesatVar, ///< @cmd
    
    //de EBF
    thk_id_state_hw, ///< @cmd
    thk_id_executeBeforeFlight, ///< @cmd

   //*************
    thk_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}THK_CmdIndx;

#define THK_NCMD ((unsigned char)thk_id_last_one)
void thk_onResetCmdTHK();



//Comandos
//****************************************************
int thk_debug(void *param);
int thk_periodicUpdate_STA_CubesatVar(void *param);
int thk_executeBeforeFlight(void *param);
int thk_state_hw(void *param);
/**
 * Deploys satellite antennas
 * @param param 1 realime, 0 debug time
 * @return 1 success, 0 fails
 */
int thk_deploy_antenna(void *param);
int thk_silent_time_and_pictures(void *param);

//funciones auxiliares
//****************************************************
BOOL thk_check_antenna_isDeployed(unsigned int delay_recheck_dep_time);
void thk_STA_CubesatVar_EBF(void);
void thk_eraseAll_CubesatVar(void);

int thk_CubesatVar_update(int indxVar);
//PPC
//void thk_ppc_hoursAlive(void); => no existen, pues no se actualizan en updateAll
//void thk_ppc_hoursWithoutReset(void);
void thk_ppc_enwdt(void);
void thk_ppc_osc(void);
void thk_ppc_MB_nOE_USB_nINT_stat(void);
void thk_ppc_MB_nOE_MHX_stat(void);
void thk_ppc_MB_nON_MHX_stat(void);
void thk_ppc_MB_nON_SD_stat(void);

/* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
 * comandos para actualizarlo */
//void thk_dep_ant_deployed(void);
//void thk_dep_ant_tries(void);
//void thk_dep_year(void);
//void thk_dep_month(void);
//void thk_dep_week_day(void);
//void thk_dep_day_number(void);
//void thk_dep_hours(void);
//void thk_dep_minutes(void);
//void thk_dep_seconds(void);

//RTC
void thk_rtc_year(void);
void thk_rtc_month(void);
void thk_rtc_week_day(void);
void thk_rtc_day_number(void);
void thk_rtc_hours(void);
void thk_rtc_minutes(void);
void thk_rtc_seconds(void);

//EPS
void thk_eps_bat0_voltage(void);
void thk_eps_bat0_current(void);
void thk_eps_bus5V_current(void);
void thk_eps_bus3V_current(void);
void thk_eps_bus_battery_current(void);
void thk_eps_bat0_temp(void);
void thk_eps_panel_pwr(void);
void thk_eps_status(void);
void thk_eps_soc(void);
void thk_eps_socss(void);
void thk_eps_state_flag(void);
void thk_eps_charging(void);

//TRX
void thk_trx_opmode(void);
void thk_trx_temp_hpa(void);
void thk_trx_temp_mcu(void);
void thk_trx_rssi(void);
void thk_trx_status_tc(void);


//**********************************************************************************
//Estas no son comandos , pero si funciones llamadas SOLO desde taskDeployment
// Functions that works with persistent memory
void thk_dep_write_deployed(int dep_state, int dep_tries);
//**********************************************************************************



#endif /* CMD_THK_H */
