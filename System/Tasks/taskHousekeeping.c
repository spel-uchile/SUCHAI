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

void taskHouskeeping(void *param)
{
    #if SCH_TASKHOUSEKEEPING_VERBOSE
        con_printf(">>[Houskeeping] Started\r\n");
    #endif

    portTickType delay_ms    = 1000;    //Task period in [ms]
    portTickType delay_ticks = delay_ms / portTICK_RATE_MS; //Task period in ticks

#if SCH_TASKHOUSEKEEPING_REALTIME
    unsigned int elapsed_sec = 0;       // Seconds counter
    unsigned int elapsed_hrs = 1;       // Hours counter
    unsigned int _1sec_check = 1;       //1[s] condition
    unsigned int _20sec_check = 20;     //20[s] condition
    unsigned int _1min_check = 60;      //1[m] condition
    unsigned int _5min_check = 5*60;    //5[m] condition
    unsigned int _1hour_check = 60*60;  //1[h] condition
    unsigned int _1day_check = 24;      //24[hrs] condition
#else
    unsigned int elapsed_sec = 0;       // Seconds counter
    unsigned int elapsed_hrs = 0;       // Hours counter
    unsigned int _1sec_check = 1;       //1[ms] condition
    unsigned int _20sec_check = 2;      //2[s] condition
    unsigned int _1min_check = 3;      //3[s] condition
    unsigned int _5min_check = 5;       //5[s] condition
    unsigned int _1hour_check = 10;     //10[s] condition
    unsigned int _1day_check = 60;      //1[m] condition
#endif

    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_THOUSEKEEPING; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    portTickType xLastWakeTime = xTaskGetTickCount();
  
    check_deploy_antenna();

    //thk_periodicUpdate_STA_CubesatVar(NULL); //TODO: Why?
    
    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, delay_ticks); //Suspend task
        elapsed_sec += delay_ms/1000; //Update seconds counts

        /* 1 seconds actions */
        if((elapsed_sec % _1sec_check) == 0)
        {
            //nothing to do here
        }

        /* 20 seconds actions */
        if((elapsed_sec % _20sec_check) == 0)
        {
            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=1)
                con_printf("[Houskeeping] 20[s] actions..\r\n");
            #endif

            NewCmd.cmdId = thk_id_periodicUpdate_STA_CubesatVar;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
//
//            NewCmd.cmdId = ppc_id_reactToSOC;
//            NewCmd.param = 0;
//            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=2)
                NewCmd.cmdId = drp_id_print_sta_CubesatVar;
                NewCmd.param = 0;
                xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
            #endif
        }

        /* 1 minute actions */
        if((elapsed_sec % _1min_check) == 0)
        {
            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=1)
                con_printf("[Houskeeping] 1[min] actions..\r\n");
            #endif

            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=2)
                NewCmd.cmdId = srp_id_print_STA_CubesatVar;
                NewCmd.param = 0;
                xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
            #endif
        }

        /* 5 minutes actions */
        if((elapsed_sec % _5min_check) == 0)
        {
            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=1)
                con_printf("[Houskeeping] 5[min] actions..\r\n");
            #endif

            #if (SCH_EPS_ONBOARD == 1)
                #if (SCH_TASKHOUSEKEEPING_VERBOSE>=2)
                    NewCmd.cmdId = eps_id_print_all_reg;
                    NewCmd.param = 0;
                    xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
                #endif
            #endif
        }

        /* 1 hour actions  */
        if((elapsed_sec % _1hour_check) == 0)
        {
            elapsed_hrs++;
            elapsed_sec = 0; //Reset prevent overflow

            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=1)
                con_printf("[Houskeeping] 1[hr] actions..\r\n");
            #endif

            NewCmd.cmdId = srp_id_increment_STA_CubesatVar_hoursWithoutReset;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

            NewCmd.cmdId = srp_id_increment_STA_CubesatVar_hoursAlive;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        }

        /* codigo para _1day_check */
        if((elapsed_hrs % _1day_check) == 0)
        {
            #if (SCH_TASKHOUSEKEEPING_VERBOSE>=1)
                con_printf("[Houskeeping] 1[day] actions..\r\n");
            #endif
            elapsed_hrs = 1;  //Reset prevent overflow

            //Nothing to do here
        }
    }
}


void check_deploy_antenna(void){
    portTickType xLastWakeTime = xTaskGetTickCount();
    portTickType delay_60s    = 60000;    //Task period in [ms]
    portTickType delay_tick_60s = delay_60s / portTICK_RATE_MS; //Task period in ticks

    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_THOUSEKEEPING; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;
    
    int rt_mode;
    if( sta_getCubesatVar(sta_dep_ant_deployed) == 0 ){
    //if( sta_getCubesatVar(sta_Antenna_isDeployed) == 0 ){
        printf("[Houskeeping] sta_SUCHAI_isDeployed = 0 => Antenna is not yet deployed..\r\n");

        unsigned long initial_tick_10ms = xTaskGetTickCount(); //get initial tick-time
        unsigned long silent_time_10ms = (180000);     // 30 minutes = 1800 sec = 180000 [10ms]
        unsigned long final_tick_10ms = initial_tick_10ms + silent_time_10ms;

        // print rtc time
        NewCmd.cmdId = rtc_id_print;
        NewCmd.param = 0;
        xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        //take picture
        #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
            NewCmd.cmdId = pay_id_takePhoto_camera;
            NewCmd.param = 0;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        #endif

        // print rtc time
        NewCmd.cmdId = rtc_id_print;
        NewCmd.param = 0;
        xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        while(TRUE){
            unsigned long int cu_tick_10ms = xTaskGetTickCount();
            if( cu_tick_10ms >= final_tick_10ms ){
                printf("[Houskeeping] Timeout of 30min silence time ..\r\n");
                break;
            }
            vTaskDelayUntil(&xLastWakeTime, delay_tick_60s); //Suspend task 60 sec
            printf("[Houskeeping] Waiting 30min silence time ..\r\n");
        }

        // print rtc time
        NewCmd.cmdId = rtc_id_print;
        NewCmd.param = 0;
        xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        /* Deploy Antena */
        #if (SCH_ANTENNA_ONBOARD==1)
            rt_mode = SCH_THK_ANTENNA_REALTIME; /* 1=Real Time, 0=Debug Time */
            NewCmd.cmdId = thk_id_deploy_antenna;
            NewCmd.param = rt_mode;
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        #endif

        // print rtc time
        NewCmd.cmdId = rtc_id_print;
        NewCmd.param = 0;
        xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        //deploy langmuir
        //..
        //other "only once"-tasks
        //..

//        sta_setCubesatVar(sta_SUCHAI_isDeployed, 1);
//        ppc_reset(NULL);
    }
}