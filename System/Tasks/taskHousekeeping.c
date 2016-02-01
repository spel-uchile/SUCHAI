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

#include "taskHouskeeping.h"
#include "task.h"

extern xQueueHandle dispatcherQueue; /* Commands queue */

void taskHousekeeping(void *param)
{
    #if SCH_THOUSEKEEPING_VERBOSE
        printf(">>[Housekeeping] Started\r\n");
    #endif

    portTickType delay_ms    = 10000;    //Task period in [ms]
    portTickType xDelay_ticks = delay_ms / portTICK_RATE_MS; //Task period in ticks

#if (SCH_THOUSEKEEPING_REALTIME == 1)
    unsigned int elapsed_sec = 0;       // Seconds counter
    unsigned int elapsed_hrs = 0;       // Hours counter
    unsigned int check_20sec = 20;     //20[s] condition
    unsigned int check_1min = 1*60;      //1[m] condition
    unsigned int check_5min = 5*60;    //5[m] condition
    unsigned int check_1hour = 60*60;  //1[h] condition
    unsigned int check_1day = 24;      //24[hrs] condition
#else
    unsigned int elapsed_sec = 0;                    // Seconds counter
    unsigned int elapsed_hrs = 0;                    // Hours counter
    unsigned int check_20sec = 2;                    //2[s] condition
    unsigned int check_1min = 3*check_20sec;         //3[s] condition
    unsigned int check_5min = 3*3*check_20sec;       //5[s] condition
    unsigned int check_1hour = 3*3*3*check_20sec;   //10[s] condition
    unsigned int check_1day = 3;                     //1[m] condition
#endif

    DispCmd NewCmd;
    NewCmd.idOrig = SCH_THOUSEKEEPING_IDORIG; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    //deploy if necessary
    //if( sta_get_BusStateVar(sta_dep_ant_deployed) == 0 ){

        #if (SCH_THOUSEKEEPING_SILENT_REALTIME==1)
            NewCmd.cmdId = thk_id_suchai_deployment;
            NewCmd.param = 31;  //in minutes
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        #else
            NewCmd.cmdId = thk_id_suchai_deployment;
            NewCmd.param = 3;  //in minutes
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        #endif
    //}

    /*Avoid the acummulation of commands while the SUCHAI is still deploying.. */
    portTickType xLastWakeTime = xTaskGetTickCount();
    while(TRUE)
    {
        vTaskDelayUntil(&xLastWakeTime, xDelay_ticks); //Suspend task
        elapsed_sec += delay_ms/1000; //Update seconds counts
        /* Check if the next tick to wake has already
         * expired (*pxPreviousWakeTime = xTimeToWake;)
         * This avoids multiple reentries on vTaskDelayUntil */
        BOOL xShouldDelay = shouldDelayTask(&xLastWakeTime, xDelay_ticks);
        if( xShouldDelay == FALSE )
        {
             xLastWakeTime = xTaskGetTickCount();
            #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                printf("[Housekeeping] xLastWakeTime + xDelay_ticks < xTickCount, "
                        "update xLastWakeTime to xTickCount ..\r\n");
            #endif
        }

        //Add commands below ..

        /* 20 seconds actions */
        if((elapsed_sec % check_20sec) == 0)
        {
            #if (SCH_THOUSEKEEPING_VERBOSE>=2)
                printf("[Houskeeping]:  20[s] actions ..\r\n");
            #endif
            //Add commands below ..
        }

        /* 1 minute actions */
        if((elapsed_sec % check_1min) == 0)
        {
            #if (SCH_THOUSEKEEPING_VERBOSE>=2)
                printf("[Houskeeping] 1[min] actions ..\r\n");
            #endif
            //Add commands below ..
                
        }

        /* 5 minutes actions */
        if((elapsed_sec % check_5min) == 0)
        {
            #if (SCH_THOUSEKEEPING_VERBOSE>=1)
                printf("[Houskeeping] 5[min] actions ..\r\n");
            #endif
            //Add commands below ..
                

//            //print StateVars, debug purposes
//            NewCmd.cmdId = srp_id_print_STA_stateVar;
//            NewCmd.param = 1;
//            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        }

        /* 1 hour actions  */
        if((elapsed_sec % check_1hour) == 0)
        {
            elapsed_hrs++;
            elapsed_sec = 0; //Prevent overflow

            #if (SCH_THOUSEKEEPING_VERBOSE>=1)
                printf("[Houskeeping] 1[hr] actions ..\r\n");
            #endif
            //Add commands below ..

            //test trx ping
            NewCmd.cmdId = trx_id_isAlive;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
            
            //test eps ping
            NewCmd.cmdId = eps_id_isAlive;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

            // hoursWithoutReset++
            NewCmd.cmdId = srp_id_increment_STA_stateVar_hoursWithoutReset;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

            // hoursAlive ++
            NewCmd.cmdId = srp_id_increment_STA_stateVar_hoursAlive;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        }

        /* codigo para _1day_check */
        if( (elapsed_hrs % check_1day == 0) && (elapsed_hrs != 0) )
        {
            #if (SCH_THOUSEKEEPING_VERBOSE>=1)
                printf("[Houskeeping]:        1[day] actions ..\r\n");
            #endif
            elapsed_hrs = 0;  //Prevent overflow
            //Add commands below ..
        }
    }
}