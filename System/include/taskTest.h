#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "SUCHAI_config.h"

#include "csp.h"
#include "csp_if_i2c.h"
#include "csp_i2c.h"


void taskClientCSP(void *param);
void taskServerCSP(void *param);
void taskRxI2C(void *param);

#endif

