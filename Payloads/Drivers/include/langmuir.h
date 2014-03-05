#ifndef LANGMUIR_H
#define LANGMUIR_H

#include <string.h>
#include <stdlib.h>

#include "pic_pc104_config.h"
#include "rs232_suchai.h"

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

/*------------------------------------------------------------------------------
 *		 	LAG READ CAL PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a calibration
 *                      packet (4packets x 10Bytes)
 * Arguments          : unsigned int* buffer[40] - Receive buffer
 *                      langmuir will send 40 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_cal_packet(unsigned int* buffer);

/*------------------------------------------------------------------------------
 *		 	LAG READ PLASMA PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma
 *                      packet (1packet x 10Bytes)
 * Arguments          : unsgined int* buffer[10] - Receive buffer
 *                      langmuir will send 40 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_plasma_packet(unsigned int* buffer);

/*------------------------------------------------------------------------------
 *		 	LAG READ SWEEP PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma sweep
 *                      packet (1packet x 1096Bytes)
 * Arguments          : unsgined int* buffer[1096] - Receive buffer
 *                      langmuir will send 1096 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_sweep_packet(unsigned int* buffer);

#endif //LANGMUIR_H