/**
 * @file  cmdRTC.h
 * @author Carlos Gonzalez C
 * @date 2012
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados al RTC externo.
 * Comandos para configurar y leer datos desde el dispositivo.
 */

#ifndef CMD_RTC_H
#define CMD_RTC_H

#include "SUCHAI_config.h"
#include "cmdIncludes.h"    //para CMD_XXX
#include "DebugIncludes.h"  //para con_printf
#include "rtc_mb.h"

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    rtc_id_adjust_year=((unsigned int)(SCH_CMD_RTC)*0x100), ///< @cmd_first
    rtc_id_adjust_month, ///< @cmd
    rtc_id_adjust_day, ///< @cmd
    rtc_id_adjust_weekday, ///< @cmd
    rtc_id_adjust_hour, ///< @cmd
    rtc_id_adjust_minutes, ///< @cmd
    rtc_id_adjust_seconds, ///< @cmd
    rtc_id_print, ///< @cmd
    rtc_id_reset, ///< @cmd

    //*********************
    rtc_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}RTC_CmdIndx;

#define RTC_NCMD ((unsigned char)rtc_id_last_one)

void rtc_onResetCmdRTC(void);

int rtc_adjust_year(void *param);
int rtc_adjust_month(void *param);
int rtc_adjust_day(void *param);
int rtc_adjust_weekday(void *param);
int rtc_adjust_hour(void *param);
int rtc_adjust_minutes(void *param);
int rtc_adjust_seconds(void *param);
int rtc_print(void *param);
int rtc_reset(void *param);

#endif /* CMD_RTC_H */
