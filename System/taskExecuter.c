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

#include "taskExecuter.h"

extern xQueueHandle executerCmdQueue; /* Comands queue*/
extern xQueueHandle executerStatQueue; /* Comands queue*/

void taskExecuter(void *param)
{
#if (SCH_TASKEXECUTER_VERBOSE>=1)
        printf("[Executer] Started\r\n");
#endif

    ExeCmd RunCmd;
    int cmdStat, queueStat, cmdParam;
        
    while(1)
    {
        /* Read the CMD that Dispatcher sent - BLOCKING */
        queueStat = xQueueReceive(executerCmdQueue, &RunCmd, portMAX_DELAY);

#if (SCH_TASKEXECUTER_VERBOSE > 1)
        printf("[Executer] Executing a new command...\r\n");
#endif
        
        if(queueStat == pdPASS)
        {
            /* El comando tiene 2min aprox para ejecutarse */
            ClrWdt();

            /* Execute the command */
            cmdParam = RunCmd.param;
            cmdStat = RunCmd.fnct((void *)&cmdParam);

            /* Si el comando se ejecuto bien reseteo el WDT */
            ClrWdt();
            
#if (SCH_TASKEXECUTER_VERBOSE > 1)
            printf("[Executer] Return status: %d\n", (unsigned int)cmdStat );
#endif
            /* Send the result to Dispatcher - BLOCKING */
            xQueueSend(executerStatQueue, &cmdStat, portMAX_DELAY);

        }
    }
}
