/* 
 * File:   gps.h
 * Author: toopazo
 *
 * Created on August 22, 2015, 11:24 AM
 */

#ifndef GPS_H
#define	GPS_H

#ifdef	__cplusplus
extern "C" {
#endif

/*========================================================================================
Description            		:Headers for gps_control.c functions.
File                            :gps_control.h
Created by    	           	:Alex Becerra
Project                    	:SUCHAI
Creation date        		:15/04/2013

Revision history
v1.00 (15/04/2013)        	:Initial release
==========================================================================================*/


#include "pic_pc104_config.h"     // OK
#include "SUCHAI_config.h"     // OK

#include <stdio.h>          // Esto va?
#include <stdlib.h>

//void __delay_ms(unsigned long d);   // Estan definidas estas funciones en algun otro lado?
//void __delay_us(unsigned long d);   // ""

unsigned char *gps_exec_cmd(unsigned int cmd_num);
unsigned char gps_send_cmd(unsigned char* cmd);
void gps_clearUARTbuffer(void );        // esta funcion puede que no este en serial_com.h Revisar si es necesaria
int gps_wait_busy_wtimeout(void);

void gps_clear_buffer(void );
unsigned char *gps_get_buffer(void);
void gps_print_buffer(void );
unsigned char gps_parse(unsigned char success, unsigned char* buf);
unsigned char gps_serialn(void );
unsigned char gps_model(void );
unsigned char gps_senddm(void );

unsigned char gps_cs(unsigned char* d, unsigned int count);

unsigned char gps_parse_jmsg (unsigned char success, unsigned char* buf);
unsigned char gps_jmsg(void );

unsigned char gps_parse_jsat(unsigned char success, unsigned char* buf);
unsigned char gps_jsat(void );



int gps_exe_cmd(BOOL verb);

#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

