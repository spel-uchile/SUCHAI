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

#include "taskConsole.h"
#include "taskHouskeeping.h"
#include "taskComunications.h"
#include "taskFlightPlan.h"
#include "taskFlightPlan2.h"

#define TDP_SILENT_TIME_MIN 30          ///< cuantos "minutos" (65,535[s]) estara en inactividad antes de iniciarse
#define TDP_TRY_DEPLOY 10               ///< cuantas veces tratara desplegar la antena antes de anunciar fracaso
#define TDP_DEPLOY_TIME 0xB0FF          ///< 2*TDP_DEPLOY_TIME/1000 indica cuantos "s" estara activo el bus de 3.3V quemando el nilon
#define TDP_REST_DEPLOY_TIME 5000       ///< cuantos "ms" estara inactivo el bus de 3.3V descanzando de tratar de quemar el nilon
#define TDP_RECHECK_TIME 2000           ///< despues de cuantos "ms" RE-chequeara que efectivamente se desplego la antena


void taskDeployment(void *param);

int dep_init_Repos(void *param);
int dep_init_GnrlStrct(void *param);
int dep_suicide(void *param);
int dep_launch_tasks(void *param);
int dep_init_hw(void *param);
int dat_sd_init(void);
int dep_deploy_antenna(void *param);
int dep_silent_time(void *param);

#endif //_DEPLOYMENT_H
