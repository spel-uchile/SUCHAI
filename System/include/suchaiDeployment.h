/**
 * @file  taskDeployment.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * @id 0x1102
 *
 * This task implements a listener that initializes all the flight software.
 */

#ifndef _DEPLOYMENT_H
#define _DEPLOYMENT_H

/* RTOS Includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"

#include "pic_pc104_config.h"

#include "cmdIncludes.h"
#include "cmdRepository.h"
#include "dataRepository.h"
/* Add ommands definitions*/
#include "cmdTCM.h"
#include "cmdCON.h"
#include "cmdPPC.h"
#include "cmdTRX.h"
#include "cmdEPS.h"
#include "cmdRTC.h"
#include "cmdDRP.h"
#include "cmdSRP.h"
#include "cmdTHK.h"
#include "cmdPayload.h"


//FreeRTOS tasks
#include "taskTest.h"
#include "taskDispatcher.h"
#include "taskConsole.h"
#include "taskExecuter.h"
#include "taskHouskeeping.h"
#include "taskFlightPlan.h"
#include "taskFlightPlan2.h"
#include "taskFlightPlan3.h"
#include "taskComunications.h"

#include "csp.h"
#include "csp_if_i2c.h"
#include "taskTest.h"

void dep_init_suchai_hw(void);
void dep_init_suchai_repos(void);
void dep_init_suchai_tasks(void);

int dep_init_sysbus_hw(void *param);
int dep_init_dataRepo(void *param);
int dep_init_cmdRepo(void *param);
int dep_init_stateRepo(void *param);
int dep_init_adHoc_strcts(void *param);

int dep_suicide(void *param);           //deprecated
//int dat_sd_init(void);                //deprecated
//void dep_csp_initialization(void);    //deprecated

#endif //_DEPLOYMENT_H
