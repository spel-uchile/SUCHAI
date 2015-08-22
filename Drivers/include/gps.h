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

#include <p24FJ96GA010.h>   // Esto va?
#include <libpic30.h>       // Esto va?
#include <stdio.h>          // Esto va?
//#include "serial_com.h"     // OK
#include "pic_pc104_config.h"     // OK
#include "SUCHAI_config.h"     // OK

//void __delay_ms(unsigned long d);   // Estan definidas estas funciones en algun otro lado?
//void __delay_us(unsigned long d);   // ""

unsigned char gps_send_cmd(unsigned char* cmd);
void gps_clearUARTbuffer(void );        // esta funcion puede que no este en serial_com.h Revisar si es necesaria

void gps_clear_buffer(void );
unsigned char gps_parse(unsigned char success, unsigned char* buf);
unsigned char gps_serialn(void );
unsigned char gps_model(void );
unsigned char gps_senddm(void );

unsigned char gps_cs(unsigned char* d, unsigned int count);

unsigned char gps_parse_jmsg (unsigned char success, unsigned char* buf);
unsigned char gps_jmsg(void );

unsigned char gps_parse_jsat(unsigned char success, unsigned char* buf);
unsigned char gps_jsat(void );



#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

