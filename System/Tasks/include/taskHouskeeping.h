/**
 * @file  taskHousekeeping.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * @id 0x1101
 *
 * This task implements a listener, that send commands at periodical times.
 */

#ifndef T_HOUSEKEEPING_H
#define T_HOUSEKEEPING_H

#include "FreeRTOS.h"
#include "queue.h"

//#include "pic_pc104_config.h"
#include "cmdIncludes.h"
#include "suchaiDeployment.h"
//#include "cmdRepository.h"
/* Add commands definitions*/
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

void taskHousekeeping(void *param);

#endif //T_HOUSEKEEPING_H

