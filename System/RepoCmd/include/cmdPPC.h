/**
 * @file  cmdPPC.h
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados a computador a
 * bordo.
 */

#ifndef CMD_PCC_H
#define CMD_PCC_H

#include "SUCHAI_config.h"
#include "dataRepository.h"
#include "stateRepository.h"
#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf
//#include "cmdTRX.h"

#include "task.h"

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    ppc_id_reset=((unsigned int)(SCH_CMD_PPC)*0x100), ///< @cmd_first
    ppc_id_newosc, ///< @cmd
    ppc_id_get_osc, ///< @cmd
    ppc_id_set_wdt_state, ///< @cmd
    ppc_id_get_wdt_state, ///< @cmd
    ppc_id_reactToSOC, ///< @cmd
    ppc_id_get_lastResetSource, ///< @cmd
    ppc_id_set_PPC_MB_nOE_USB_nINT, ///< @cmd
    ppc_id_set_PPC_MB_nOE_MHX, ///< @cmd
    ppc_id_set_PPC_MB_nON_MHX, ///< @cmd
    ppc_id_set_PPC_MB_nON_SD, ///< @cmd
    ppc_id_get_PPC_MB_nOE_USB_nINT, ///< @cmd
    ppc_id_get_PPC_MB_nOE_MHX, ///< @cmd
    ppc_id_get_PPC_MB_nON_MHX, ///< @cmd
    ppc_id_get_PPC_MB_nON_SD, ///< @cmd
    ppc_id_get_opMode, ///< @cmd
    ppc_id_set_opMode, ///< @cmd
    ppc_id_get_hoursAlive, ///< @cmd
    ppc_id_get_hoursWithoutReset, ///< @cmd
    ppc_id_get_resetCounter, ///< @cmd
    ppc_id_set_hoursAlive, ///< @cmd
    ppc_id_set_hoursWithoutReset, ///< @cmd
    ppc_id_set_resetCounter, ///< @cmd
    ppc_id_rtos_debug, ///< @cmd
    ppc_id_frozen, ///< @cmd

    //*********************
    ppc_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}PPC_CmdIndx;

#define PPC_NCMD ((unsigned char)ppc_id_last_one)
void ppc_onResetCmdPPC(void);


int ppc_newosc(void* osc);
int ppc_set_wdt_state(void* on_off);
int ppc_get_wdt_state(void* param);
int ppc_get_osc(void* param);
int ppc_reset(void* param);
int ppc_reactToSOC(void* param);
int ppc_get_lastResetSource(void* param);
int ppc_set_PPC_MB_nOE_USB_nINT(void* param);
int ppc_set_PPC_MB_nOE_MHX(void* param);
int ppc_set_PPC_MB_nON_MHX(void* param);
int ppc_set_PPC_MB_nON_SD(void* param);
int ppc_get_PPC_MB_nOE_USB_nINT(void* param);
int ppc_get_PPC_MB_nOE_MHX(void* param);
int ppc_get_PPC_MB_nON_MHX(void* param);
int ppc_get_PPC_MB_nON_SD(void* param);
int ppc_get_hoursAlive(void* param);
int ppc_get_hoursWithoutReset(void* param);
int ppc_get_resetCounter(void* param);
int ppc_get_opMode(void* param);
int ppc_set_opMode(void* param);
int ppc_set_hoursAlive(void* param);
int ppc_set_hoursWithoutReset(void* param);
int ppc_set_resetCounter(void* param);

int ppc_rtos_debug(void *param);
int ppc_frozen(void *param);


#endif /* CMD_PCC_H */
