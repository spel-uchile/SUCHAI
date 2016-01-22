/**
 * @file  cmdEPS.h
 * @author Tomas Opazo T
 * @author Francisco Reyes A
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados a la EPS. Comandos
 * para configuracion y lectura de datos.
 */

#ifndef CMD_EPS_H
#define CMD_EPS_H

//#include "eps_suchai.h"
#include "cmdRTC.h"
#include "csp.h"
//#include "nanocom.h"
#include "cmdTRX.h"
#include "nanopower.h"
#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf
#include "SUCHAI_config.h"

/*This are associated to DataRepository's EPS_state and are use to read
 *  eps_suchai.c data structure*/
#define	EPS_ID_batt_temp_0          0
#define	EPS_ID_batt_temp_1          1
#define	EPS_ID_bootcount            2
#define	EPS_ID_battery_voltage      3
#define	EPS_ID_channel_status       4
#define	EPS_ID_latchup_0            5
#define	EPS_ID_panel_current        6
#define	EPS_ID_ppt_mode             7
#define	EPS_ID_panel_voltage_1      8
#define	EPS_ID_panel_voltage_2      9
#define	EPS_ID_panel_voltage_3      10
#define	EPS_ID_reset                11
#define	EPS_ID_system_current       12
#define	EPS_ID_sw_errors            13
#define	EPS_ID_temp_conv1           14
#define	EPS_ID_temp_conv2           15
#define	EPS_ID_temp_conv3           16
#define	EPS_ID_temp_board           17


/**
 * Lista de comandos disponibles.
 */
typedef enum{
    eps_id_isAlive=((unsigned int)(SCH_CMD_EPS)*0x100), ///< @cmd_first
    eps_id_initialize,   ///<@cmd
    eps_id_print_hk, ///<@cmd
    eps_id_read_vars, ///<@cmd

    //*********************
    eps_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}EPS_CmdIndx;


#define EPS_NCMD ((unsigned char)eps_id_last_one)


void eps_onResetCmdEPS(void);

//Commands
int eps_isAlive(void *param);
int eps_initialize(void *param);
int eps_print_hk(void *param);
int eps_read_vars(void *param);

#endif /* CMD_EPS_H */
