/**
 * @file  cmdDRP.h
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 *
 */

#ifndef CMD_THK_H
#define CMD_THK_H

#include "FreeRTOS.h" //vTaskDelay
#include "task.h"

#include "cmdIncludes.h"    //cmdFunction
#include "DebugIncludes.h"  //para printf

#include "stateRepository.h"
#include "dataRepository.h"

//cmdXXX
#include "cmdRTC.h"
#include "cmdDRP.h"
#include "cmdSRP.h"
#include "cmdPayload.h"

#include "memEEPROM.h"

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    //de debug
    thk_id_hw_check=((unsigned int)SCH_CMD_THK*0x100), ///< @cmd_first
    thk_id_test_antenna_deployment, ///< @cmd   0xA001

    //de EBF
    thk_id_executeBeforeFlight, ///< @cmd       0xA002

    //Deplyment
    thk_id_suchai_deployment, ///< @cmd         0xA003
    thk_id_deploy_antenna, ///< @cmd            0xA004
    thk_id_get_AntSwitch_isOpen, ///< @cmd
    thk_id_silent_time_and_pictures, ///< @cmd
    thk_id_deployment_registration, ///< @cmd

    //get Deployment registration
    thk_id_get_dep_ant_deployed, ///< @cmd
    thk_id_get_dep_ant_tries, ///< @cmd

    thk_id_get_dep_year, ///< @cmd
    thk_id_get_dep_minutes, ///< @cmd
    thk_id_get_dep_month, ///< @cmd
    thk_id_get_dep_week_day, ///< @cmd
    thk_id_get_dep_day_number, ///< @cmd
    thk_id_get_dep_hours, ///< @cmd
    thk_id_get_dep_seconds, ///< @cmd

//    thk_id_set_dep_ant_deployed, ///< @cmd
//    thk_id_set_dep_ant_tries, ///< @cmd
//
//    thk_id_set_dep_week_day, ///< @cmd
//    thk_id_set_dep_day_number, ///< @cmd
//    thk_id_set_dep_hours, ///< @cmd
//    thk_id_set_dep_month, ///< @cmd
//    thk_id_set_dep_year, ///< @cmd
//    thk_id_set_dep_seconds, ///< @cmd
//    thk_id_set_dep_minutes, ///< @cmd

   //*************
    thk_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}THK_CmdIndx;

#define THK_NCMD ((unsigned char)thk_id_last_one)
void thk_onResetCmdTHK();



//Comandos
//****************************************************
int thk_hw_check(void *param);
int thk_executeBeforeFlight(void *param);
int thk_debug2(void *param);
int thk_suchai_deployment(void *param);
/**
 * Deploys satellite antennas
 * @param param 1 realime, 0 debug time
 * @return 1 success, 0 fails
 */
int thk_deploy_antenna(void *param);
int thk_get_AntSwitch_isOpen(void* param);
int thk_deployment_registration(void *param);
int thk_silent_time_and_pictures(void *param);
int thk_test_antenna_deployment(void *param);

//antenna registration
int thk_get_dep_ant_deployed(void* param);
int thk_get_dep_ant_tries(void* param);

int thk_get_dep_year(void* param);
int thk_get_dep_seconds(void* param);
int thk_get_dep_month(void* param);
int thk_get_dep_minutes(void* param);
int thk_get_dep_week_day(void* param);
int thk_get_dep_day_number(void* param);
int thk_get_dep_hours(void* param);

////set
//int thk_set_dep_ant_deployed(void* param);
//int thk_set_dep_ant_tries(void* param);
//
//int thk_set_dep_day_number(void* param);
//int thk_set_dep_hours(void* param);
//int thk_set_dep_minutes(void* param);
//int thk_set_dep_seconds(void* param);
//int thk_set_dep_year(void* param);
//int thk_set_dep_month(void* param);
//int thk_set_dep_week_day(void* param);

//funciones auxiliares
//****************************************************


////PPC
////void thk_ppc_hoursAlive(void); => no existen, pues no se actualizan en updateAll
////void thk_ppc_hoursWithoutReset(void);
//void thk_ppc_enwdt(void);
//void thk_ppc_osc(void);
//void thk_ppc_MB_nOE_USB_nINT_stat(void);
//void thk_ppc_MB_nOE_MHX_stat(void);
//void thk_ppc_MB_nON_MHX_stat(void);
//void thk_ppc_MB_nON_SD_stat(void);
//
///* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
// * comandos para actualizarlo */
////void thk_dep_ant_deployed(void);
////void thk_dep_ant_tries(void);
////void thk_dep_year(void);
////void thk_dep_month(void);
////void thk_dep_week_day(void);
////void thk_dep_day_number(void);
////void thk_dep_hours(void);
////void thk_dep_minutes(void);
////void thk_dep_seconds(void);
//
////RTC
//void thk_rtc_year(void);
//void thk_rtc_month(void);
//void thk_rtc_week_day(void);
//void thk_rtc_day_number(void);
//void thk_rtc_hours(void);
//void thk_rtc_minutes(void);
//void thk_rtc_seconds(void);
//
////EPS
////void thk_eps_bat0_voltage(void);
////void thk_eps_bat0_current(void);
////void thk_eps_bus5V_current(void);
////void thk_eps_bus3V_current(void);
////void thk_eps_bus_battery_current(void);
////void thk_eps_bat0_temp(void);
////void thk_eps_panel_pwr(void);
////void thk_eps_status(void);
////void thk_eps_soc(void);
////void thk_eps_socss(void);
////void thk_eps_state_flag(void);
////void thk_eps_charging(void);
//
////TRX
//void thk_trx_opmode(void);
//void thk_trx_temp_hpa(void);
//void thk_trx_temp_mcu(void);
//void thk_trx_rssi(void);
//void thk_trx_status_tc(void);


#endif /* CMD_THK_H */
