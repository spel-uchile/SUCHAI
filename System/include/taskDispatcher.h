/**
 * @file  taskDispatcher.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * @id 0x1105
 *
 * This task implements the dispatcher. Reads commands from queue, determines
 * if the commands is executable, asks to command repository the function to
 * send to taskExecuter. It's an event driven task.
 */

#include "FreeRTOS.h"
#include "queue.h"

#include "cmdIncludes.h"    //DispCmd 
#include "cmdRepository.h"  //repo_getsysReq

void taskDispatcher(void *param);
BOOL check_if_executable(DispCmd *newCmd);
