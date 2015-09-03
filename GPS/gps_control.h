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
#include "serial_com.h"     // OK

void __delay_ms(unsigned long d);   // Estan definidas estas funciones en algun otro lado?
void __delay_us(unsigned long d);   // ""

unsigned char send_cmd(unsigned char* cmd);
void clearUARTbuffer(void );        // esta funcion puede que no este en serial_com.h Revisar si es necesaria

void clear_buffer(void );
unsigned char parse(unsigned char success, unsigned char* buf);
unsigned char serialn(void );
unsigned char model(void );
unsigned char senddm(void );

unsigned char cs(unsigned char* d, unsigned int count);

unsigned char parse_jmsg (unsigned char success, unsigned char* buf);
unsigned char jmsg(void );

unsigned char parse_jsat(unsigned char success, unsigned char* buf);
unsigned char jsat(void );

