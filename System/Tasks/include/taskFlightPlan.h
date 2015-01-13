/**
 * @file  taskFlightPlan.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * @id 0x1102
 *
 * This task implements a listener that works as flight plan
 */

#ifndef T_FLIGHTPLAN_H
#define T_FLIGHTPLAN_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

//#include "pic_pc104_config.h"
#include "cmdIncludes.h"
#include "stateRepository.h"
//#include "dataRepository.h"

//#include "cmdRepository.h"
#include "cmdDRP.h"

void taskFlightPlan(void *param);

#endif //T_FLIGHTPLAN_H
