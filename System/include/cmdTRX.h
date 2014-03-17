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
#include "dataRepository.h"
#include "DebugIncludes.h"  //para con_printf

#define CMD_ADDFRAME_START      (0)
#define CMD_ADDFRAME_CONT       (1)
#define CMD_ADDFRAME_STOP       (2)
#define CMD_ADDFRAME_FIN        (3)
#define CMD_ADDFRAME_ADD        (4)
#define CMD_ADDFRAME_SINGL      (5)
#define CMD_ADDFRAME_EXIT       (-1)

#define CMD_TMFRAME_TSTARTH     (0x01)
#define CMD_TMFRAME_TSTARTL     (0x00)
#define CMD_TMFRAME_TSTOPH      (0x02)
#define CMD_TMFRAME_TSTOPL      (0x00)
#define CMD_TMFRAME_TCONTH      (0x03)
#define CMD_TMFRAME_TCONTL      (0x00)
#define CMD_TMFRAME_TSINGLH     (0x04)
#define CMD_TMFRAME_TSINGLL     (0x00)

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    trx_id_send_beacon=((unsigned int)(CMD_TRX)*0x100), ///< @cmd_first
    trx_id_readreg, ///< @cmd
    trx_id_idleframe, ///< @cmd
    trx_id_getstatus, ///< @cmd
    trx_id_set_beacon, ///< @cmd
    trx_id_initialize, ///< @cmd
    trx_id_setmode, ///< @cmd
    trx_id_asknewtc, ///< @cmd
    trx_id_parsetcframe, ///< @cmd
    trx_id_set_tm_pwr, ///< @cmd
    trx_id_set_bc_pwr, ///< @cmd
    trx_id_read_tcframe, ///< @cmd
    trx_id_tm_trxstatus, ///< @cmd
    trx_id_write_reg, ///< @cmd
    trx_id_set_reg_val, ///< @cmd
    trx_id_resend, ///< @cmd
    trx_id_reset_tm_pointer, ///< @cmd

    //*********************
    trx_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}TRX_CmdIndx;

#define TRX_NCMD ((unsigned char)trx_id_last_one)

void trx_onResetCmdTRX(void);
int trx_set_conf(void *param);
int trx_send_beacon(void *param);
int trx_read_conf(void *param);
int trx_test_frame(void *param);
int trx_testframe(void *param);
int trx_getstatus(void *param);
int trx_set_beacon(void *param);
int trx_initialize(void *param);
int trx_setmode(void *param);
int trx_asknewtc(void *param);
int trx_parsetcframe(void *param);
int trx_set_tm_pwr(void *param);
int trx_set_bc_pwr(void *param);
int trx_read_tcframe(void *param);
int trx_tm_trxstatus(void *param);
int trx_write_reg(void *param);
int trx_set_reg_val(void *param);
int trx_resend(void *param);
int trx_reset_tm_pointer(void *param);

int trx_tm_addtoframe(int *data, int len, int mode);

#endif /* CMD_TRX_H */
