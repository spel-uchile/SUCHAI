#ifndef LANGMUIR_H
#define LANGMUIR_H

#include <string.h>
#include <stdlib.h>

#include "pic_pc104_config.h"
#include "rs232_suchai.h"

#if(SCH_PAY_LANGMUIR_ONBOARD==1)
    #define LAG_BUFFER_LEN      (1096)
#else
    #define LAG_BUFFER_LEN      (1)
#endif

#define LAG_COMMPORT    RS2_M_UART3 
#define LAG_BAUDRATE    (38400) /* BAUD 38400 => ABAUD 25*/
#define LAG_ABAUD       (25) /* BAUD 38400 => ABAUD 25*/

#define LAG_SYNC        (0x43)
#define LAG_CAL         (0xC1)
#define LAG_PLASMA      (0xC2)
#define LAG_SWEEP       (0xC3)

#define langmuir_printf(c)       SendStrRS232(c, LAG_COMMPORT)
#define langmuir_getc()          ((char)ReadRS232(LAG_COMMPORT))

#define langmuir_write(str,len)  SendRS232(str, len, LAG_COMMPORT)
#define langmuir_read()          ReadRS232(LAG_COMMPORT)

int lag_read_cal_packet(BOOL verb);
int lag_read_plasma_packet(BOOL verb);
int lag_read_sweep_packet(BOOL verb);
void lag_print_buffer(int len);
void lag_erase_buffer(void);
int lag_wait_busy_wtimeout(void);
unsigned int lag_get_langmuir_buffer_i(int ind);
int langmuir_isAlive(void);

#endif //LANGMUIR_H