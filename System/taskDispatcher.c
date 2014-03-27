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
        con_printf(">>[Dispatcher] Started\r\n");
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
                    char buf[10];
                    /* IMPRIMIR LOS COMANDOS QUE SE VAN A EJECUTAR */
                    utoa(buf,idOrig,16);
                    con_printf("[Dispatcher] Orig: 0x"); con_printf(buf);
                    utoa(buf,cmdId,16);
                    con_printf(" | Cmd: 0x"); con_printf(buf);
                    itoa(buf,cmdParam,10);
                    con_printf(" | Param: "); con_printf(buf); con_printf("\r\n");
                #endif

                /* Fill the executer command */
				exeCmd.fnct = repo_getCmd(newCmd.cmdId);
				exeCmd.param = cmdParam;

                /* Send the command to executer Queue - BLOCKING */
                xQueueSend(executerCmdQueue, &exeCmd, portMAX_DELAY);
                
                /* Get the result from Executer Stat Queue - BLOCKING */
                xQueueReceive(executerStatQueue, &cmdResult, portMAX_DELAY);

                #if SCH_TASKDISPATCHER_VERBOSE >= 1
                    itoa(buf, cmdResult, 10);
                    con_printf("[Dispatcher] CMD Result: "); con_printf(buf);
                    con_printf("\r\n");
                #endif
            }
        }
        else
        {
            #if SCH_TASKDISPATCHER_VERBOSE >= 1
                con_printf("[Dispatcher] Unable to read cmd from queue");
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
int check_if_executable(DispCmd *newCmd)
{
    int cmdId, idOrig, sysReq, param; /* Cmd metadata */

    cmdId = newCmd->cmdId;
    idOrig = newCmd->idOrig;
    sysReq = newCmd->sysReq;
    param = newCmd->param;

    if(cmdId == CMD_CMDNULL)
    {
        #if SCH_TASKDISPATCHER_VERBOSE >=1
            char ret[10];
            con_printf("[Dispatcher] Command: 0x");
            utoa(ret, cmdId, 16); con_printf(ret);
            utoa(ret, idOrig, 16);
            con_printf(" | From: 0x"); con_printf(ret);
            con_printf(" Refused because of CMD_NULL \n\r");
        #endif
        return 0;
    }

    #if(SCH_TASKDISPATCHER_CHECK_IF_EXECUTABLE != 0)
    {
        // Compare sysReq with SOC
        if(dat_getCubesatVar(dat_eps_soc) < sysReq)
        {
            #if SCH_TASKDISPATCHER_VERBOSE >=1
                char ret[10];
                con_printf("[Dispatcher] Command: 0x");
                utoa(ret, cmdId, 16); con_printf(ret);
                utoa(ret, idOrig, 16);
                con_printf(" | From: 0x"); con_printf(ret);
                con_printf(" Refused because of SOC \n\r");
            #endif
            return 0;
        }

        //Prevent TCM command to discharge bat. (Pina-Bilbao Power Budget)
        if((dat_getCubesatVar(dat_eps_state_flag) == 1) && ((cmdId>>8)==CMD_TCM))
        {
            #if SCH_TASKDISPATCHER_VERBOSE >=1
                char ret[10];
                con_printf("[Dispatcher] Command: 0x");
                utoa(ret, cmdId, 16); con_printf(ret);
                utoa(ret, idOrig, 16);
                con_printf(" | From: 0x"); con_printf(ret);
                con_printf(" Refused because of EPS Flag \n\r");
            #endif
            return 0;
        }

    }
    #endif
    
    return 1;
}
