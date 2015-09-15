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

#include "taskFlightPlan.h"

extern xQueueHandle dispatcherQueue;

void taskFlightPlan(void *param)
{
#if SCH_TFLIGHTPLAN_VERBOSE
    con_printf(">>[FlightPlan] Started\r\n");
#endif

#if SCH_TFLIGHTPLAN_REALTIME
    /* Resolution = 30[s] */
    const portTickType xDelay_ticks = (30 * 1000 / portTICK_RATE_MS);
#else
    /* Resolution = 10[s] */
    const unsigned int xDelay_ticks = (10 * 1000) / portTICK_RATE_MS;
#endif
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = SCH_TFLIGHTPLAN_IDORIG;
    NewCmd.param = 0;

    /*Avoid the acummulation of commands while the SUCHAI is still deploying.. */
    portTickType xLastWakeTime = xTaskGetTickCount();
    #if (SCH_THOUSEKEEPING_USE == 1)
        portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
        while( TRUE ){
            if( sta_get_BusStateVar(sta_dep_ant_deployed)==1 ){
                break;
            }
            vTaskDelayUntil(&xLastWakeTime, check_deployment_time);
        }
    #endif

    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, xDelay_ticks);
        /* Check if the next tick to wake has already
         * expired (*pxPreviousWakeTime = xTimeToWake;)
         * This avoids multiple reentries on vTaskDelayUntil */
        BOOL xShouldDelay = shouldDelayTask(&xLastWakeTime, xDelay_ticks);
        if( xShouldDelay == FALSE )
        {
             xLastWakeTime = xTaskGetTickCount();
            #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                printf("[FlightPlan] xLastWakeTime + xDelay_ticks < xTickCount, "
                        "update xLastWakeTime to xTickCount ..\r\n");
            #endif
        }

        //execute programmed/itinerary actions ..
        NewCmd.cmdId = drp_id_fpl_check_and_exec;
        NewCmd.param = 0;
        xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
    }
}
