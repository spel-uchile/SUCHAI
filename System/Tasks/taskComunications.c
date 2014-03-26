/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *      Copyright 2013, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "taskComunications.h"

extern xQueueHandle dispatcherQueue; /* Commands queue */

void taskComunications(void *param)
{
#if (SCH_TASKCOMUNICATIONS_VERBOSE)
    con_printf(">>[Comunications] Started\r\n");
#endif

//    portTickType delay_ms    = 2000;    //Task period in [ms]
//    int xsec = (delay_ms/1000);  //se ejecuta cada Xseg ahora
//    portTickType delay_ticks = delay_ms / portTICK_RATE_MS; //Task period in ticks (==dalay_ms/10)
    
    DispCmd TcNewCmd;

    TcNewCmd.cmdId = CMD_CMDNULL;
    TcNewCmd.idOrig = CMD_IDORIG_TCOMUNICATIONS; /* Comunications */
    TcNewCmd.param = 0;

//    int COMM_OP_MODE = sta_getCubesatVar(sta_ppc_opMode); /* MODO DE OPERACION */
//    int NEW_TC = 0;
//    int NEW_CMD_BUF = 0;
//    int LAST_CMD_DAY = sta_getCubesatVar(sta_trx_lastcmd_day);
//    int RSSI = 0;
//    unsigned int RSSI_CNT = 0; /* Tiempo que hay RSSI, rel. a la ejecucion de la tarea */
//    int RSSI_MEAN = 0;
//
//    unsigned long seconds_cnt = 0; /* Contador para comandos periodicos */
//    unsigned int type_cnt = 0; /* Identificador del tipo de beacon siguiente */
//
//    char buf[10];
//
//    portTickType xLastWakeTime = xTaskGetTickCount();
//
//    /* Comienza el ciclo de la tarea */
//    while(1)
//    {
//        /* Tarea periodica cada 2 Segundos*/
//        vTaskDelayUntil(&xLastWakeTime, delay_ticks);
//        seconds_cnt += xsec;
//
//        /* Actualizar y enviar beacon */
//        if(seconds_cnt % SCH_COMM_BEACON_PERIOD_S == 0)
//        {
//            /* Ajustar el contador a los tipos de beacon que hay*/
//            type_cnt = type_cnt % 2;
//
//            /* Preguntar por nuevos telecomandos disponibles */
//            TcNewCmd.cmdId = tcm_id_send_beacon;
//            TcNewCmd.param = type_cnt; /* Tipo 1 - Variables de estado */
//            /* Queue NewCmd - Non-Blocking (Wait 0.5 task period) */
//            xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
//            type_cnt++;
//        }
//
//        /* Volver TRX Op mode to nobeacon */
//        if(seconds_cnt % 30 == 0)
//        {
//            TcNewCmd.cmdId = trx_id_setmode;
//            TcNewCmd.param = TRX_MODE_NOBEACON;
//            xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
//        }
//
//        /* Preguntar por nuevos telecomandos disponibles */
//        TcNewCmd.cmdId = trx_id_asknewtc;
//        TcNewCmd.param = 0; /* No Verbose */
//        /* Queue NewCmd - Non-Blocking (Wait 0.5 task period) */
//        xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
//
//        /* Actualizar la lectura de RSSI */
//        TcNewCmd.cmdId = drp_id_update_sta_CubesatVar_trx_rssi;
//        TcNewCmd.param = 0; /* None */
//        /* Queue NewCmd - Non-Blocking (Wait 0.5 task period) */
//        xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
//
//        /* Recuperar variables de estado */
//        NEW_TC          = sta_getCubesatVar(sta_trx_newTcFrame);
//        NEW_CMD_BUF     = sta_getCubesatVar(sta_trx_newCmdBuff);
//        LAST_CMD_DAY    = sta_getCubesatVar(sta_trx_lastcmd_day);
//        COMM_OP_MODE    = sta_getCubesatVar(sta_ppc_opMode);
//        RSSI            = sta_getCubesatVar(sta_trx_rssi);
//        RSSI_MEAN       = sta_getCubesatVar(sta_trx_rssi_mean);
//
//        #if (SCH_TASKCOMUNICATIONS_VERBOSE >=2)
//            itoa(buf,NEW_TC,10);
//            con_printf(">>[taskComunications] NewTcFrame: "); con_printf(buf);
//            itoa(buf,NEW_CMD_BUF,10);
//            con_printf(" | newCmdBuff: "); con_printf(buf);
//            itoa(buf,COMM_OP_MODE,10);
//            con_printf(" | mode: "); con_printf(buf);
//            itoa(buf,RSSI_MEAN,10);
//            con_printf(" | rssi_mean: "); con_printf(buf);
//            itoa(buf,RSSI,10);
//            con_printf(" | rssi: "); con_printf(buf);
//            itoa(buf,RSSI_CNT,10);
//            con_printf(" | rssi_cnt: "); con_printf(buf);
//            con_printf("\r\n");
//        #endif
//
//        /* Modo de operacion solo con RSSI */
//        if(COMM_OP_MODE == STA_PPC_OPMODE_RSSI)
//        {
//            /* Llegaron TC, cambiar el modo a normal */
//            if(NEW_TC)
//            {
//                /* Comando para cambiar el modo a NORMAL */
//                TcNewCmd.cmdId = drp_id_update_sta_CubesatVar_opMode;
//                TcNewCmd.param = STA_PPC_OPMODE_NORMAL;
//                xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//            }
//
//            if(RSSI >= RSSI_MEAN+4) //Mayor en 4dB que el promedio
//            {
//                RSSI_CNT +=xsec;  //Cuenta segundps con el nivel de RSSI
//
//                #if (SCH_TASKCOMUNICATIONS_VERBOSE >=1)
//                    itoa(buf, RSSI_CNT,10);
//                    con_printf("RSSI_CNT="); con_printf(buf); con_printf("\r\n");
//                #endif
//
//                /* Si hay alto nivel de RSSI durante mucho tiempo, entonces hay
//                 * alto ruido y se debe actualizar el promedio */
//                if(RSSI_CNT >= SCH_COMM_RSSI_CNT_MAX )
//                {
//                    /* Agregar el nuevo valor al promedio */
//                    TcNewCmd.cmdId = drp_id_update_sta_CubesatVar_trx_rssi_mean;
//                    TcNewCmd.param = RSSI;
//                    xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//                }
//            }
//            else
//            {
//                /* Segundos que estuvo presente el RSSI */
//                if((RSSI_CNT > SCH_COMM_RSSI_MIN_TIME) && (RSSI_CNT < SCH_COMM_RSSI_MAX_TIME))
//                {
//                    com_doOnRSSI(dispatcherQueue);
//                }
//
//                /* Cuando no hay carrier, actualizar el valor de ruido promedio */
//                TcNewCmd.cmdId = drp_id_update_sta_CubesatVar_trx_rssi_mean;
//                TcNewCmd.param = RSSI; /* Valor de RSSI actual */
//                xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//
//                /* Resetear el contador, para capturar la permanencia del rssi */
//                RSSI_CNT = 0;
//            }
//        } /* MODE RSSI*/
//
//
//        /* Modo de operacion con telecomandos */
//        else if(COMM_OP_MODE == STA_PPC_OPMODE_NORMAL)
//        {
//            /* Calcular cuandos dias han pasado desde el ultimo TC */
//            unsigned int today =    sta_getCubesatVar(sta_rtc_day_number)*
//                                    sta_getCubesatVar(sta_rtc_month)*
//                                    sta_getCubesatVar(sta_rtc_year);
//
//            unsigned int days_wo_tc = today - LAST_CMD_DAY;
//
//            /* Si no han llegado TC por varios dias, pasar a modo RSSI*/
//            if(days_wo_tc >= SCH_COMM_NO_TC_DAYS)
//            {
//                /* Comando para cambiar el modo a RSSI */
//                TcNewCmd.cmdId = drp_id_update_sta_CubesatVar_opMode;
//                TcNewCmd.param = STA_PPC_OPMODE_RSSI;
//                xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//            }
//
//            /* Si hay nuevos frames parsear telecomandos */
//            if(NEW_TC && !NEW_CMD_BUF) /* Ask new TC state */
//            {
//                TcNewCmd.cmdId = trx_id_parsetcframe; /* Parse tc frame */
//                TcNewCmd.param = 0; /* No verboso */
//                /* Queue NewCmd - Non-Blocking (Wait 0.5 task period) */
//                xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
//            }
//
//            if(NEW_CMD_BUF)
//            {
//                /* COMANDO que baja el flag de CmdBuff */
//                TcNewCmd.cmdId = drp_id_trx_newCmdBuff;
//                TcNewCmd.param = 0; /* Reset newCmdBuff flag in status repo */
//                xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//
//                /* Ejecutar los telecomandos pendientes */
//                int i;
//                /* Read each TC from data repo in format [ID,ARG] */
//                for(i=0; i < SCH_DATAREPOSITORY_MAX_BUFF_TELECMD; i++)
//                {
//                    int cmdid = dat_getTelecmdBuff(i++); /* Get int cmdID */
//                    int arg = dat_getTelecmdBuff(i); /* Get int cmdArg */
//
//                    if(cmdid != CMD_CMDNULL)
//                    {
//                        TcNewCmd.cmdId = cmdid; /* Current read TC */
//                        TcNewCmd.param = arg;
//                        xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
//                    }
//                }
//            } /* NEW BUFF */
//        } /* MODE NOMRAL */
//    }
}

/**
 * Performs all tasks needed when the carrier is detected
 *
 * @param cmdQueue Queue to send commands
 */
void com_doOnRSSI(xQueueHandle dispatcherQueue)
{
    DispCmd TcNewCmd;
    TcNewCmd.cmdId = CMD_CMDNULL;
    TcNewCmd.idOrig = CMD_IDORIG_TCOMUNICATIONS; /* Comunications */
    TcNewCmd.param = 0;

    #if (SCH_TASKCOMUNICATIONS_VERBOSE >=1)
        con_printf("com_doOnRSSI..\r\n");
    #endif

    //CubesatVar
    TcNewCmd.cmdId = tcm_id_sendTM_cubesatVar;
    TcNewCmd.param = 2;
    xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
    
    // Payload
    //envio TM de payload
    TcNewCmd.cmdId = tcm_id_sendTM_all_pay_i;
    TcNewCmd.param = 0;
    xQueueSend(dispatcherQueue, &TcNewCmd, portMAX_DELAY);
}
