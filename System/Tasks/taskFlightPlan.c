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
#if SCH_TASKFLIGHTPLAN_VERBOSE
    char buff[50];
    con_printf(">>[FlightPlan] Started\r\n");
#endif

#if SCH_TASKFLIGHTPLAN_REALTIME
    /* Resolution = 30[s] */
    portTickType xDelayms = (30 * 1000 / portTICK_RATE_MS);
#else
    /* Resolution = 10[s] */
    const unsigned int xDelay_ticks = (10 * 1000) / portTICK_RATE_MS;
#endif
    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */

    int index, current_hour, current_mins;
    int last_index = -1;
    
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN;
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
        vTaskDelayUntil(&xLastWakeTime, xDelay_ticks);
        /* Check if the next tick to wake has already
         * expired (*pxPreviousWakeTime = xTimeToWake;)
         * This avoids multiple reentries on vTaskDelayUntil */
        BOOL xShouldDelay = shouldDelayTask(&xLastWakeTime, xDelay_ticks);
        if( xShouldDelay == FALSE )
        {
             xLastWakeTime = xTaskGetTickCount();
            #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                printf("[FlightPlan] xLastWakeTime + xDelay_ticks < xTickCount, "
                        "update xLastWakeTime to xTickCount ..\r\n");
            #endif
        }

        /* Map hh:mm to MM minutues of the day to obtain the
         * index of the next command to read from fligh plan */
        current_hour = sta_get_stateVar(sta_rtc_hours);
        current_mins = sta_get_stateVar(sta_rtc_minutes);
        index = current_hour*60 + current_mins;
        index = index / SCH_FLIGHTPLAN_RESOLUTION;

        /* Task period is minor than flight plan resolution, so we need to
         * prevent an index repetition */
        if(last_index != index)
        {
            /* Get the next command from flight plan */
            last_index = index;
            NewCmd = dat_get_FlightPlan(index);

            /* Check if valid cmd */
            if(NewCmd.cmdId == CMD_CMDNULL) continue;

            #if SCH_TASKFLIGHTPLAN_VERBOSE
                /* Print the command code */
                sprintf(buff, "[FlightPlan] Se genera comando: 0x%X\r\n", (unsigned int)NewCmd.cmdId );
                con_printf(buff);
//                con_printf("[FlightPlan] Se genera comando: ");
//                con_printf(ret); con_printf("\n\0");
            #endif

            /* Queue NewCmd - Blocking */
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        }
    }
}
