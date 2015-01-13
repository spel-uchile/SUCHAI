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
        printf(">>[FlightPlan2] Started\r\n");
#endif

#if SCH_FLIGHTPLAN2_REALTIME
    unsigned int min_check_period_ms = 10000;      /* check every x ms  */
    portTickType xDelay_ticks = (min_check_period_ms) / portTICK_RATE_MS;
    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
#else
    unsigned int min_check_period_ms = 1000;      /* check every 2sec  */
    portTickType xDelay_ticks = (min_check_period_ms) / portTICK_RATE_MS;
    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
#endif

    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    /*Avoid the acummulation of commands while the SUCHAI is still deploying.. */
    portTickType xLastWakeTime = xTaskGetTickCount();
    #if (SCH_USE_HOUSEKEEPING == 1)
        while( TRUE ){
            if( sta_get_stateVar(sta_dep_ant_deployed)==1 ){
                break;
            }
            vTaskDelayUntil(&xLastWakeTime, check_deployment_time);
        }
    #endif

    while(1)
    {
        /* min_check_period_ms actions */
        vTaskDelayUntil(&xLastWakeTime, xDelay_ticks);
        
        /* Check if the next tick to wake has already
         * expired (*pxPreviousWakeTime = xTimeToWake;)
         * This avoids multiple reentries on vTaskDelayUntil */
        BOOL xShouldDelay = shouldDelayTask(&xLastWakeTime, xDelay_ticks);
        if( xShouldDelay == FALSE )
        {
             xLastWakeTime = xTaskGetTickCount();
            #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                printf("[FlightPlan2] xLastWakeTime + xDelay_ticks < xTickCount, "
                        "update xLastWakeTime to xTickCount ..\r\n");
            #endif
        }

        //Add commands below ..

        #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
            printf("[FlightPlan2] min_check_period_ms (%d) actions ..\r\n", min_check_period_ms);
        #endif

        if(sta_get_stateVar(sta_ppc_opMode)==STA_PPC_OPMODE_NORMAL){
            NewCmd.cmdId = pay_id_fp2_default_fsm;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
        }

    }
}

