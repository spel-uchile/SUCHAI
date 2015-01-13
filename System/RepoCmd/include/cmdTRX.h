/**
 * @file  cmdTRX.h
 * @author Carlos Gonzalez C
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados al TRX. Son
 * comandos que permiten configurar el sistema de comunicaciones; obtener
 * los datos desde el dispositivo tales como su estado y telecomandos; y para
 * enviar la telemetria al dispositivo.
 */

#ifndef CMD_TRX_H
#define CMD_TRX_H

#include <string.h>

#include "nanocom.h"

#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para printf

#include "memEEPROM.h"


#define CMD_ADDFRAME_START      (0)
#define CMD_ADDFRAME_CONT       (1)
#define CMD_ADDFRAME_STOP       (2)
#define CMD_ADDFRAME_FIN        (3)
#define CMD_ADDFRAME_ADD        (4)
#define CMD_ADDFRAME_SINGL      (5)
#define CMD_ADDFRAME_EXIT       (-1)

/** Start frame flag
 */
#define CMD_TMFRAME_TSTARTH     (0x01)
#define CMD_TMFRAME_TSTARTL     (0x00)
#define CMD_TMFRAME_TSTART      (0x0100)

/** Stop frame flag
 */
#define CMD_TMFRAME_TSTOPH      (0x02)
#define CMD_TMFRAME_TSTOPL      (0x00)
#define CMD_TMFRAME_TSTOP       (0x0200)

/** Continuation frame flag
 */
#define CMD_TMFRAME_TCONTH      (0x03)
#define CMD_TMFRAME_TCONTL      (0x00)
#define CMD_TMFRAME_TCONT       (0x0300)

/** Single frame flag
 */
#define CMD_TMFRAME_TSINGLH     (0x04)
#define CMD_TMFRAME_TSINGLL     (0x00)
#define CMD_TMFRAME_TSINGL      (0x0400)

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    trx_id_isAlive=((unsigned int)(SCH_CMD_TRX)*0x100), ///< @cmd_first
    trx_id_get_operation_mode, ///< @cmd
    trx_id_set_operation_mode, ///< @cmd
    trx_id_get_count_tm, ///< @cmd
    trx_id_set_count_tm, ///< @cmd
    trx_id_get_count_tc, ///< @cmd
    trx_id_set_count_tc, ///< @cmd
    trx_id_get_day_last_tc, ///< @cmd
    trx_id_set_day_last_tc, ///< @cmd
    trx_id_get_tx_baud, ///< @cmd
    //next is 0xA
    trx_id_set_tx_baud, ///< @cmd
    trx_id_get_rx_baud, ///< @cmd
    trx_id_set_rx_baud, ///< @cmd
    trx_id_get_beacon_level, ///< @cmd
    //next is 0xE
    trx_id_set_beacon_level, ///< @cmd
    trx_id_get_beacon_period, ///< @cmd
    trx_id_set_beacon_period, ///< @cmd

    trx_id_set_conf, ///< @cmd
    trx_id_read_conf, ///< @cmd
    trx_id_ping, ///< @cmd
    trx_id_getstatus, ///< @cmd
    trx_id_set_beacon, ///< @cmd
    trx_id_initialize, ///< @cmd

    //*********************
    trx_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}TRX_CmdIndx;

#define TRX_NCMD ((unsigned char)trx_id_last_one)


void trx_onResetCmdTRX(void);

// Status repo
int trx_isAlive(void *param);
int trx_get_operation_mode(void *param);
int trx_set_operation_mode(void *param);
int trx_get_count_tm(void *param);
int trx_set_count_tm(void *param);
int trx_get_count_tc(void *param);
int trx_set_count_tc(void *param);
int trx_get_day_last_tc(void *param);
int trx_set_day_last_tc(void *param);
int trx_get_tx_baud(void *param);
int trx_set_tx_baud(void *param);
int trx_get_rx_baud(void *param);
int trx_set_rx_baud(void *param);
int trx_get_beacon_level(void *param);
int trx_set_beacon_level(void *param);
int trx_get_beacon_period(void *param);
int trx_set_beacon_period(void *param);

// Commands
int trx_set_conf(void *param);
int trx_read_conf(void *param);
int trx_ping(void *param);
int trx_testframe(void *param);
int trx_getstatus(void *param);
int trx_set_beacon(void *param);
int trx_initialize(void *param);

// Auxiliary
int trx_tm_addtoframe(int *data, int len, int mode);

#endif /* CMD_TRX_H */
