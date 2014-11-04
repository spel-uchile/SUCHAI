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

#include "taskDispatcher.h"

extern xQueueHandle dispatcherQueue; /* Commands queue */
extern xQueueHandle executerCmdQueue;
extern xQueueHandle executerStatQueue;

void taskDispatcher(void *param)
{
#if SCH_TASKDISPATCHER_VERBOSE
        printf("[Dispatcher] Started\r\n");
#endif

    portBASE_TYPE status; /* Status of cmd reading operation */
    DispCmd newCmd; /* The cmd readed */
    ExeCmd exeCmd; /* The cmd to executer */

    int cmdId, idOrig, sysReq, cmdResult, cmdParam; /* Cmd metadata */

    while(1)
    {
        /* Read newCmd from Queue - Blocking */
        status = xQueueReceive(dispatcherQueue, &newCmd, portMAX_DELAY);

        if(status == pdPASS)
        {
            /* Process command metadata*/
            newCmd.sysReq = repo_getsysReq(newCmd.cmdId); //Updates sysReq
            /* Gets command metadata*/
            cmdId = newCmd.cmdId;
            idOrig = newCmd.idOrig;
            sysReq = newCmd.sysReq;
            cmdParam = newCmd.param;

            /* Check if command is executable */
            if(check_if_executable(&newCmd))
            {
                #if (SCH_TASKDISPATCHER_VERBOSE >=1)
                    /* IMPRIMIR LOS COMANDOS QUE SE VAN A EJECUTAR */
                    printf("[Dispatcher] Orig: 0x%X | Cmd: 0x%X | Param: %d\n", idOrig, cmdId, cmdParam );
                #endif

                /* Fill the executer command */
                //exeCmd.fnct = repo_getCmd(newCmd.cmdId);
                exeCmd.fnct = repo_getFunction(newCmd.cmdId);
                exeCmd.param = cmdParam;

                #if(SCH_TASKEXECUTER_INSIDE_TASKDISPATCHER==1)
                    /* El comando tiene 2min aprox para ejecutarse */
                    ClrWdt();

                    /* Execute the command */
                    cmdParam = exeCmd.param;
                    cmdResult = exeCmd.fnct((void *)&cmdParam);

                    /* Si el comando se ejecuto bien reseteo el WDT */
                    ClrWdt();
                #else
                    /* Send the command to executer Queue - BLOCKING */
                    xQueueSend(executerCmdQueue, &exeCmd, portMAX_DELAY);

                    /* Get the result from Executer Stat Queue - BLOCKING */
                    xQueueReceive(executerStatQueue, &cmdResult, portMAX_DELAY);
                #endif

                #if SCH_TASKDISPATCHER_VERBOSE >= 1
                    printf("[Dispatcher] CMD Result: %d\n", cmdResult);
                #endif
            }
        }
        else
        {
            #if SCH_TASKDISPATCHER_VERBOSE >= 1
                printf("[Dispatcher] Unable to read cmd from queue\n");
            #endif
        }
    }
}

/**
 * Determines if a command is can be executed based in his metadata and current
 * status variables
 *
 * @param newCmd Command to check
 * @return 0, Do not execute. 1, can be executed.
 */
BOOL check_if_executable(DispCmd *newCmd)
{
    int cmdId, idOrig, sysReq, param; /* Cmd metadata */

    cmdId = newCmd->cmdId;
    idOrig = newCmd->idOrig;
    sysReq = newCmd->sysReq;
    param = newCmd->param;

    if(cmdId == CMD_CMDNULL)
    {
        #if SCH_TASKDISPATCHER_VERBOSE >=1
            printf("[Dispatcher] Command: 0x%X | From: 0x%X Refused because of CMD_NULL \n", cmdId, idOrig);
        #endif
        return FALSE;
    }

    #if(SCH_TASKDISPATCHER_CHECK_IF_EXECUTABLE != 0)
    {
        // Compare sysReq with SOC
        if(sta_get_stateVar(sta_eps_soc) < sysReq)
        {
            #if SCH_TASKDISPATCHER_VERBOSE >=1
                printf("[Dispatcher] Orig: 0x%X | Cmd: 0x%X | Param: %d Refused because of SOC\n", idOrig, cmdId, param );
            #endif
            return FALSE;
        }

        //Prevent TCM command to discharge bat. (Pina-Bilbao Power Budget)
        if((sta_get_stateVar(sta_eps_state_flag) == 1) && ((cmdId>>8)==SCH_CMD_TCM))
        {
            #if SCH_TASKDISPATCHER_VERBOSE >=1
                printf("[Dispatcher] Orig: 0x%X | Cmd: 0x%X | Param: %d Refused because of EPS flag\n", idOrig, cmdId, param );
            #endif
            return FALSE;
        }
    }
    #endif
    
    return TRUE;
}
