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

#include "taskConsole.h"

extern xQueueHandle dispatcherQueue;



const char console_baner[] =   
"______________________________________________________________________________\n\
                     ___ _   _  ___ _  _   _   ___ \n\
                    / __| | | |/ __| || | /_\\ |_ _|\n\
                    \\__ \\ |_| | (__| __ |/ _ \\ | | \n\
                    |___/\\___/ \\___|_||_/_/ \\_\\___|\n\
______________________________________________________________________________\n\n";


//"\n\n====== WELCOME TO THE SUCHAI CONSOLE - PRESS ANY KEY TO START ======\n\r"

void taskConsole(void *param)
{
#if (SCH_TASKCONSOLE_VERBOSE)
    printf(">>[Console] Started\r\n");
#endif

    //char ret[10];

    const unsigned int Delayms = 250 / portTICK_RATE_MS;
    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_TCONSOLE; /* Consola */
    NewCmd.cmdId = CMD_CMDNULL;  /* cmdNULL */
    NewCmd.param = 0;

    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */

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

    /* Initializing console */
    con_init();

#if (SCH_TASKCONSOLE_VERBOSE>=1)
    __delay_ms(500);    //helps printing a cleaner banner (avoid interruption)
    printf((char *)console_baner);
#endif

    while(1)
    {
        vTaskDelay(Delayms);     //just delay is enough
        //vTaskDelayUntil(&xLastWakeTime, Delayms);

        /* Parsing command - return CmdDisp structure*/
        NewCmd = con_cmd_handler();

        /* cmdId = 0xFFFF means no new command */
        if(NewCmd.cmdId != CMD_CMDNULL)
        {
            printf("\r\n");

            #if (SCH_TASKCONSOLE_VERBOSE >=1)
                /* Print the command code */
//                sprintf( ret, "0x%X", (unsigned int)NewCmd.cmdId );
//                con_printf("[Console] Se genera comando: ");
//                con_printf(ret); con_printf("\n\0");
                printf("[Console] Se genera comando: 0x%X \r\n", (unsigned int)NewCmd.cmdId);
            #endif

            /* Queue NewCmd - Blocking */
            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
        }        
    }
}
