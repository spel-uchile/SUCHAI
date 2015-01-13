/**
 * @file  taskComunications.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * @id 0x1101
 *
 * This task implements a listener, that send commands at periodical times.
 */

#ifndef T_COMUNICATIONS_H
#define T_COMUNICATIONS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "csp.h"
#include "csp_port.h"
#include "csp_i2c.h"

#include "cmdIncludes.h"    //DispCmd
#include "DebugIncludes.h"    //DispCmd
#include "SUCHAI_config.h"

//cmdXXX
#include "cmdTCM.h"
#include "cmdCON.h"


void taskComunications(void *param);
void com_doOnRSSI(xQueueHandle cmdQueue);
//void com_RxI2C(i2c_frame_t *frame_p, xQueueHandle i2c_rx_queue);

#endif //T_COMUNICATIONS_H
