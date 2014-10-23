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

#include "taskFlightPlan2.h"

extern xQueueHandle dispatcherQueue;

void taskFlightPlan2(void *param)
{
#if (SCH_FLIGHTPLAN2_VERBOSE)
        con_printf(">>[FlightPlan2] Started\r\n");
#endif

#if SCH_FLIGHTPLAN2_REALTIME
    portTickType _1sec_check = (1000) / portTICK_RATE_MS;      /* check every 1sec  */
    portTickType _10sec_check = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
    unsigned int elapsed_sec = 0;
    unsigned int _4sec_check = (8);                /* check every 4 sec */
    unsigned int _5min_check = (300);                /* check every 5min */
#else
    portTickType _1sec_check = (250) / portTICK_RATE_MS;      /* check every 250ms  */
    unsigned int elapsed_sec = 0;
    unsigned int _4sec_check = (4);              /* check every 4*0,25 sec=1seg */
    unsigned int _5min_check = (30);                /* check every 30 sec  */
#endif

    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    portTickType xLastWakeTime = xTaskGetTickCount();

    #if (SCH_USE_HOUSEKEEPING == 1)
        /*Avoid the acummulation of commands while the SUCHAI is still deploying.. */
        while( TRUE ){
            if( sta_getCubesatVar(sta_Antenna_isDeployed)==1 ){
                break;
            }
            vTaskDelayUntil(&xLastWakeTime, _10sec_check);
        }
    #endif

    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, _1sec_check);
        elapsed_sec++;

        /* codigo para _1sec_check */
        if(1)
        {
            //nothing to do here
        }

        // codigo para _20sec_check 
        if((elapsed_sec % _4sec_check) == 0)
        {
            #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                con_printf("[FlightPlan2] _4sec_check\r\n");
            #endif

            fp2_pay_i_multiplexing(dispatcherQueue);
            //fp2_pay_i_simultaneous(dispatcherQueue);
        }
        
        // codigo para _5min_check
        if((elapsed_sec % _5min_check) == 0)
        {
            elapsed_sec = 0;
            #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                con_printf("[FlightPlan2] _5min_check\r\n");
            #endif

            //ejecuto payload TMestado
            NewCmd.cmdId = pay_id_FSM_default;
            NewCmd.param = dat_pay_tmEstado;
            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
            
        }
    }
}

void fp2_pay_i_multiplexing(xQueueHandle dispatcherQueue){
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2;
    NewCmd.param = 0;

    static DAT_PayloadBuff current_pay_i;
    
    //multiplexo pay_i por turnos
    #if (SCH_FLIGHTPLAN2_VERBOSE>=2)
        printf("fp2_pay_i_multiplexing => pay_i = %d\r\n", (unsigned int)current_pay_i);
    #endif

    switch(current_pay_i){
        //excluyo casos particulares
        case dat_pay_tmEstado:
            //do nothing
            //printf("dat_pay_tmEstado does not use FSM_default..\r\n");
        break;
        default:
            NewCmd.cmdId = pay_id_FSM_default;
            NewCmd.param = current_pay_i;
            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
        break;
    }

    //Si el actual es el ultimo, el proximo sera el primero
    if( current_pay_i == (dat_pay_last_one-1) ){
        current_pay_i=0;
    }
    else{
        current_pay_i++;
    }
}

void fp2_pay_i_simultaneous(xQueueHandle dispatcherQueue)
{
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2;
    NewCmd.param = 0;

    //ejecuto payloads, "simultaneamente" osea, todos en cada ciclo
    STA_CubesatVar dat_pay_xxx_perform = sta_pay_i_to_performVar(dat_pay_tmEstado);
    DAT_PayloadBuff pay_i;

    for(pay_i = 0; pay_i < dat_pay_last_one; pay_i++)
    {
        if(sta_getCubesatVar(dat_pay_xxx_perform) == 0x0001)
        {
            NewCmd.cmdId = pay_id_FSM_default;
            NewCmd.param = pay_i;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        }
    }
}
