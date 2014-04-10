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
    //de debug
    drp_id_debug=((unsigned int)SCH_CMD_DRP*0x100), ///< @cmd_first

    //De display
    drp_id_print_dat_TelecmdBuff, ///< @cmd
    drp_id_print_dat_FlightPlan, ///< @cmd
    drp_id_print_dat_PayloadIndxs, ///< @cmd
    drp_id_print_dat_PayloadVar, ///< @cmd

    //De mem ext (EEPROM y SD)
    drp_id_executeBeforeFlight, ///< @cmd
    
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

//Comandos
//****************************************************
// command's to print dat_CubesatVar
int drp_print_dat_TelecmdBuff(void *param);
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
//de TRX
int drp_trx_newTcFrame(void *param);
int drp_trx_newCmdBuff(void *param);

//funciones auxiliares
//****************************************************
void drp_DAT_FlightPlanBuff_EBF(void);
void drp_DAT_TeleCmdBuff_EBF(void);
void drp_DAT_PayloadBuff_EBF(void);

void drp_debug1(void);
void drp_debug2(void);
void drp_debug3(void);
void drp_debug4(void);


#endif /* CMD_DRP_H */
