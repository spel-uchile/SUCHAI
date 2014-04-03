/**
 * @file  cmdTCM.h
 * @author Carlos Gonzalez C
 * @author Tomas Opazo T
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados a telemetria y
 * telecomandos. Son comandos para recopilar telemetria especifica y luego ser
 * enviada por el sistema de comunicaciones asi como comandos para poder parsear
 * los telecomandos enviados desde la estacion terrena y ejecutarlos
 */

#ifndef CMD_TCM_H
#define CMD_TCM_H

#include <string.h>

#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf
#include "dataRepository.h"
#include "cmdTRX.h"

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    tcm_id_testframe=((unsigned int)(SCH_CMD_TCM)*0x100), ///< @cmd_first
    tcm_id_resend, ///< @cmd
    //Beacon
    tcm_id_update_beacon, ///< @cmd
    //CubesatVar
    tcm_id_sendTM_cubesatVar, ///< @cmd
    //Payload
    tcm_id_sendTM_all_pay_i, ///< @cmd
    tcm_id_sendTM_pay_i, ///< @cmd
    //Debug
    tcm_id_set_sysreq, ///< @cmd

    //*********************
    tcm_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}TCM_CmdIndx;

#define TCM_NCMD ((unsigned char)tcm_id_last_one)

void tcm_onResetCmdTCM(void);

//Comandos (Telemetria y Telecomandos)
int tcm_resend(void *param);
int tcm_testframe(void *param);
int tcm_sendTM_all_pay_i(void *param);
int tcm_sendTM_pay_i(void *param);
int tcm_sendTM_cubesatVar(void *param);
int tcm_sendTM_expFis(void *param);
int tcm_sendTM_gyro(void *param);
int tcm_sendTM_camera(void *param);
int tcm_sendTM_gps(void *param);
int tcm_sendTM_lagmuirProbe(void *param);
int tcm_sendTM_temp(void *param);
int tcm_update_beacon(void *param);

int tcm_set_sysreq(void *param);

//funciones aux
int tcm_sendTM_PayloadVar(int mode, DAT_PayloadBuff pay_i);

#endif

