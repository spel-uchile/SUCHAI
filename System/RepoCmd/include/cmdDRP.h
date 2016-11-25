/**
 * @file  cmdDRP.h
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 *
 */

#ifndef CMD_DRP_H
#define CMD_DRP_H

#include "FreeRTOS.h" //vTaskDelay
#include "task.h"

#include "dataRepository.h"
#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf
#include "cmdRepository.h"

/* Necesarios para update */
#include "cmdPPC.h"
#include "cmdTRX.h"
#include "cmdEPS.h"
#include "cmdRTC.h"


/**
 * Lista de comandos disponibles.
 */
typedef enum{
    //de debug
    drp_id_debug=((unsigned int)SCH_CMD_DRP*0x100), ///< @cmd_first

    //De display
    drp_id_print_dat_FlightPlan, ///< @cmd
    drp_id_print_dat_PayloadIndxs, ///< @cmd
    drp_id_print_dat_PayloadVar, ///< @cmd

    //De EBF
    drp_id_executeBeforeFlight, ///< @cmd
    
    //Flight Plan
    drp_id_fpl_set_index, ///< @cmd
    drp_id_fpl_get_index, ///< @cmd
    drp_id_fpl_set_cmd, ///< @cmd
    drp_id_fpl_set_param, ///< @cmd
    drp_id_fpl_check_and_exec, ///< @cmd

    //*************
    drp_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}DRP_CmdIndx;

#define DRP_NCMD ((unsigned char)drp_id_last_one)
void drp_onResetCmdDRP();

//Comandos
//****************************************************
// command's to print
int drp_print_dat_FlightPlan(void *param);
int drp_print_dat_PayloadIndxs(void *param);
int drp_print_dat_PayloadVar(void *param);
//comandos para mem ext (SD y EEPROM)
int drp_executeBeforeFlight(void *param);
//de debug
int drp_debug(void *param);
//Comandos para Flight plan
int drp_fpl_set_index(void *param);
int drp_fpl_set_cmd(void *param);
int drp_fpl_set_param(void *param);
int drp_fpl_get_index(void* param);
int drp_fpl_set_index(void* param);
int drp_fpl_check_and_exec(void* param);

//funciones auxiliares
//****************************************************
void drp_DAT_FlightPlan_EBF(void);
void drp_DAT_Payload_Buff_EBF(void);

void drp_debug1(void);
void drp_debug2(void);
void drp_debug3(void);
void drp_debug4(void);
void drp_debug5(void);


#endif /* CMD_DRP_H */
