/**
 * @file  taskFlightPlan2.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2013
 * @copyright GNU GPL v3
 *
 * @id 0x1102
 *
 * This task implements a listener that executes payloads periodically
 */

#ifndef T_FLIGHTPLAN3_H
#define T_FLIGHTPLAN3_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "DebugIncludes.h"
#include "cmdIncludes.h"    //DispCmd

//cmdXXX
#include "cmdDRP.h"     //drp_id_fpl_check_and_exec;
#include "cmdPayload.h" //pay_id_fp2_default_fsm;


//#define FP2_PRE_INIT_PAY    0
//#define FP2_INIT_PAY        1
//#define FP2_TAKE_PAY        2
//#define FP2_STOP_PAY        3
//#define FP2_POST_STOP_PAY   4

void taskFlightPlan3(void *param);

//BOOL shouldDelayTask( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement);

#endif //T_FLIGHTPLAN3_H




