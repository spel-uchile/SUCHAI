/*========================================================================================
Description            		:Sotfware for Javad's TR-G2 GPS
File                            :gps_control.c
Created by    	           	:Alex Becerra
Project                    	:SUCHAI
Creation date        		:27/03/2013

Note(1)               		:Using commands as described in Javad's GREIS manual

Revision history
v1.00 (27/03/2013)        	:Initial release
v2.00 (07/05/2013)		:The software reads well the commands only when the GPS is UNLOCKED.
v3.00 (10/05/2013)		:Functional for GPS LOCKED.

EXITCODE DECODER ========================================================================================
0 - Success
1 - Error in UART receiving routine 
2 - For "print" commands, received data starts with "ER"
3 - For messages sent with "%%", the received data doesn't content the "%%" indicator.
4 - For JAVAD messages, indicates that, at least, one message wasn't received (maybe due to unlocked GPS)
5 - For ALL messages, indicates that a mismatch checksum was detected, at least, in one answer
6 - For JAVAD messages, indicates a mismatch in time epoch of RT and ET messages
9 - Unknown error
=========================================================================================================

 * Falta corregir
 * - Agregar un actualizador de actual_length para comprobar si hay otras sentencias que si llegaron bien
 * - Agregar timeout en la recepción del puerto serial
 * - Agregar el estado de los satelites
 * - Agregar el estado del receptor
 * - Agregar el parsing de los datos duros
 *

==========================================================================================*/

#include <p24FJ96GA010.h>       // Revisar si estos archivos van o no
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <uart.h>
#include "gps_control.h"
#include "serial_com.h"

// Constant definition
unsigned char nl		= {0x0A};

unsigned char sleep_mode[] 	= "print,/par/sleep\r\n";
unsigned char low_power[]	= "print,/par/lpm\r\n";

unsigned char dismsg[]		= "%disable_msg%dm\r\n";

unsigned char rcv_sn[] 		= "print,/par/rcv/sn\r\n";
unsigned char rcv_id[]		= "print,/par/rcv/id\r\n";
unsigned char rcv_model[]	= "print,/par/rcv/model\r\n";
unsigned char rcv_vendor[]	= "print,/par/rcv/vendor\r\n";
unsigned char rcv_uptime[]	= "print,/par/rcv/uptime\r\n";

unsigned char rcv_mem[]		= "print,/par/rcv/mem\r\n";
unsigned char rev_cfgw[]	= "print,/par/rcv/cfgw\r\n";
unsigned char rev_all[]		= "print,/par/rcv\r\n";
unsigned char ant_curinp[]	= "print,/par/ant/curinp\r\n";
unsigned char ant_dc[]		= "print,/par/ant/dc\r\n";
//Include time information? (UTC, GPS, receiver time)
unsigned char pwr_voltage[]	= "print,/par/pwr/ext\r\n";
unsigned char pwr_board[]	= "print,/par/pwr/board\r\n";
unsigned char ant_voltage[]	= "print,/par/pwr/extant\r\n";
unsigned char ant_current[]	= "print,/par/pwr/extantdc\r\n";
unsigned char board_temp[]	= "print,/par/dev/thermo/out\r\n";
unsigned char proc_load[]	= "print,/par/load\r\n";

//"out" is used to get only one answer from the GPS, not a periodic one.
unsigned char per_jmsg[]	= "out,,/msg/jps/{RT,PO,VE,PG,VG,PT,PS,ET,EE}\r\n";	// JAVAD's messages	
unsigned char per_jtext[]	= "out,,/msg/jps/NP\r\n";				// JAVAD's text messages (ASCII)
unsigned char per_nmea[]	= "out,,/msg/nmea/{GGA,ZDA,VTG}\r\n";                   // NMEA sentences
unsigned char per_jsat[]	= "out,,/msg/jps/{EL,AZ,EC,TC,SI}\r\n";                 // JAVAD's messages for satellite status

unsigned char* buffer;	
unsigned char gps_busy 		= 0;			// This variable indicates when the MCU should be waiting answers from the GPS
unsigned char gps_control	= 0;			// Used in one function (send_cmd), indicates when an error in reception (UART) was detected
unsigned char lastline		= 0;			// This variable indicates when the MCU should expect a final line from the GPS
unsigned char aux[]		= {0x00, 0x00, 0x00};	// This is a variable to check for some particular set of characters in a sentence
unsigned long cmdl 		= 0;			// This variable stores the length of a command

unsigned char data[250];

//Serial ports configuration registers 
#define BRATE 		34 		// 115200 Bd (BREGH=1) @ 32MHz
#define U_ENABLE 	0x8008		// Enable UART, BREGH=1, 1 stop, no parity
#define U_TX 		0x0400 		// Enable transmission, clear all flags

// Milisecond and microsecond definition
#define FCY 		16000000UL		
#define MS_2_CLK 	16000UL 	/* asumes 16MIPS (FCY/1000)= (16000000/1000)=16000*/
#define US_2_CLK 	16UL 		/* asumes 16MIPS (FCY/1000000)= (16000000/1000000)=16*/

//Delay functions for ms and us
void __delay_ms(unsigned long d){ 
        __delay32( (unsigned long) (d)*(MS_2_CLK) ); 
}

void __delay_us(unsigned long d){ 
        __delay32( (unsigned long) (d)*(US_2_CLK) - 2); /* El 2 es por el tiempo que tarda la copia de los argumentos */
}


// ============================================================================================================
// Sends a command using serial port UART1
// Arguments	: unsigned char* cmd - command to be sent
// Return	: gps_control (0 if there was an error, 1 if not) [See the interrupt routine to more info]
// ============================================================================================================
unsigned char send_cmd(unsigned char* cmd){

	gps_control = 0;						
	gps_busy = 1;							// Set the gps busy (A command will be sent and then an answer should be expected)
	aux[2] = 0x00;							// Clear the aux array
	aux[1] = 0x00;
	aux[0] = 0x00;
	unsigned char temp;

        // Verificar si es UART 1 o 2
	temp = cmd[0];
	while (temp != 0x0A){			// Every command should finish with a '\n' (0x0A) character
		SendRS232(&temp, 1, RS2_M_UART1);
		temp = *(++cmd);
	}
	SendRS232(&nl, 1, RS2_M_UART1);         // Sends a '\n' character (to finish the command)
	
	IEC0bits.U1RXIE = 1;			// Enable interruption to receive data

	while(gps_busy);			// Wait until UART port has received all the data [See interrupt routine for more info]
	return gps_control;			// Return the status [Modified in the interruption routine]
}

// ============================================================================================================
// Clears the UART1 receiver buffer to erase previous unwanted data and to clear overflow flag
// Arguments	: None
// Return	: None
// ============================================================================================================
void clearUARTbuffer(void ){
	unsigned int i;
	unsigned char r;
	for(i=0; i<=5; i++){
		r = U1RXREG;
	}
	U1STAbits.OERR = 0;
}

// ============================================================================================================
// Interrupt that manages data reception in RX line of UART1
// Arguments	: None
// Return	: None
// ============================================================================================================
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void ){

	unsigned char rec = U1RXREG;

	if ((U1STAbits.PERR == 0) && (U1STAbits.FERR == 0)){ 	// Only use the data if there was no error in reception		
		*buffer = rec;					// Put data into a buffer

		if(lastline){				// Check if this is the last line that should be received
			if (rec == 0x0A){		// When lastline = 1, the data should end with a '\n' charcater.
				gps_control = 0;	// gpscontrol = 0 indicates no error
				gps_busy = 0;		// gpsbusy = 0 returns the control to send_cmd function
				IFS0bits.U1RXIF = 0;    // Clear RX interrupt flag 
				IEC0bits.U1RXIE = 0;    // Disable RX interrupts (No more characters should be received)		
				return;
			}
		}

		else{	
			aux[2] = aux[1];				
			aux[1] = aux[0];
			aux[0] = rec;
			
			// Check for a particular set of characters to raise lastline variable
			// The set to be checked depends on the order in which the commands are sent. 
			// (See per_jmsg[], per_jtext[] and per_nmea[] variables)
			if((aux[1] == '|' && aux[0] == '|') || (aux[2] == 'V' && aux[1] == 'T' && aux[0] == 'G')){
				lastline = 1;
			}
		}

		IFS0bits.U1RXIF = 0;        // Clear RX interrupt flag
		IEC0bits.U1RXIE = 1;        // Enable RX interrupts
		buffer++;

                // ************************************
		// NEEDS A CODE TO EXIT USING A TIMEOUT
                // ************************************
	}

	else{
		gps_control = 1;			// If there was an error in reception, set gps_control
		gps_busy = 0;				// Return the control to send_cmd function
		IFS0bits.U1RXIF = 0;       	// Clear RX interrupt flag 
		IEC0bits.U1RXIE = 0;        // Disable RX interrupt
	}
}	

// ============================================================================================================
// Clears the receiver buffer named "buffer" (The common place for receiving responses from GPS).
// Arguments	: Unsigned char* buf - buffer to be cleared
// Return	: None
// ============================================================================================================
void clear_buffer(void ){
	buffer = data;
	unsigned int i;
	for (i=0; i<249; i++){		// Limit for i should be length(buffer)-1. In this case, length is 250.
		buffer[i] = 0x30;
	}
	buffer[249] = '\0';
}

// ============================================================================================================
// Parses the content of the buffer for a command in particular (not periodic or position data)
// Arguments	: unsigned char success - status from send_cm
//		  unsigned char* buff - buffer with data received
// Return	: char exitcode (see explanation at upper part of this file)
// ============================================================================================================
unsigned char parse(unsigned char success, unsigned char* buf){
	
	buffer = buf;					// Uses the buffer from input parameters
	unsigned long i;	
	unsigned char exitcode;
	char a[] = {0x00, 0x00, 0x00, 0x00};
	char* end;

	if (success == 0){					// If a command was received successfully (gps_control from send_cmd function)
		if (buffer[0] == 0x52 && buffer[1] == 0x45){	// Check if the data starts with "RE"	
			a[0] = buffer[2]; 		
			a[1] = buffer[3];
			a[2] = buffer[4];
			a[3] = '\0';
			cmdl = strtoul(a, &end, 16)+2;		// Calculates the length of the answer
								// + 2 is for CR and LF (Only for commands with "print" preamble)
			for (i=0; i<cmdl; i++){			// Using the length, send all the data stored in the buffer to screen using UART2

				SendRS232(&buffer[5+i],1, RS2_M_UART1);
				__delay_ms(10);
			}

		}
		else if (buffer[0] == 0x45 && buffer[1] == 0x52){	// If first two characters are 'E' and 'R', return exitcode 2
			exitcode = 2;
		}
		else							// This case is for an unknown error.
			exitcode = 9;
	}	
	else                                                            // If there was an error in data reception, send this exitcode.
		exitcode = 1;

	return exitcode;	
}

// ============================================================================================================
// Gets de model of the receiver
// Arguments	: void
// Return	: char with an exitcode (see explanation at upper part of this file)
// ============================================================================================================
unsigned char model(void ){
	buffer = data;
	unsigned char success;
	lastline = 1;
	success = send_cmd(rcv_model);
	return parse(success, &data[0]);
}

// ============================================================================================================
// Gets de serial number of the receiver
// Arguments	: 
// Return	: 0 for success, 1 for error receiving the answer, 2 for ER from the GPS and
//		  3 for unknown error.
// ============================================================================================================
unsigned char serialn(void ){
	buffer = data;
	unsigned char success;
	lastline = 1;
	success = send_cmd(rcv_sn);
	return parse(success, &data[0]);
}

// ============================================================================================================
// Stops all the messages in the current terminal of the GPS
// Arguments	: 
// Return	: 0 for success, 1 for error receiving the answer, 2 for ER from the GPS and
//                3 for unknown error.
// ============================================================================================================
unsigned char senddm(void ){
	buffer = data;
	unsigned char success;
	unsigned char retparse;
	lastline = 1;
	success = send_cmd(dismsg);
	retparse = parse(success, &data[0]);
	unsigned int j;
	if (retparse == 0){
		for (j=0; j<(cmdl-2); j++){                // -2 is for '\r' and '\n'
			if(dismsg[j] != buffer[5+j]){
				retparse = 3;
				break;
			}
		}
	}
	
	return retparse;
}

// ============================================================================================================
// Calculates the checksum of achar array (Algorithm taken from JAVAD's GREIS Manual)
// Arguments	: char* data - Data for checksum calculation
//                unsigned int count - Length of the data
// Return	: unsigned char checksum
// ============================================================================================================
unsigned char cs(unsigned char* d, unsigned int count){
	unsigned char res = 0;
	while(count--)
		res = ((res << 2) | (res >> 6))^(*d++);
	return ((res << 2) | (res >> 6));
}

// ============================================================================================================
// Parses the contents of JAVAD messages (per_jmsg[] command)
// Arguments	: void
// Return	: char exitcode
// ============================================================================================================
unsigned char parse_jmsg(unsigned char success, unsigned char* buf){
	buffer = buf;
	unsigned long i;
	unsigned char exitcode;
	char a[] = {0x00, 0x00, 0x00, 0x00};
	char* end;
	unsigned long actual_length = 0;	// This variable stores the current position in buffer array
	unsigned long cmd_length = 0;
	unsigned char errors[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char timeRT[4];
	unsigned char timeET[4];

	exitcode = 0;                           // Success by default

	if(success == 0){			// If there wasn't problem at reception

		// ============== Check for RT message =======================================================
		// (The description in this part applies for all the following messages)

		if(buffer[actual_length] == '~' && buffer[actual_length+1] == '~'){	// Check if the sentence starts with "~~"

                       
			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);	// Put the first message identifier character
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);	// Put the second message identifier character
                       

			a[0] = buffer[actual_length+2]; 		// Store the length of the received answer
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';

                        cmd_length = strtoul(a, &end, 16)+1;		// Calculate the length of the sentence received (strtoul function)


                        // +1 is because the answer always ends with '\n' character
                        
			timeRT[0] = buffer[actual_length+5+0];		// Store the data tag received in this message (start of epoch)
			timeRT[1] = buffer[actual_length+5+1];		// +5 is for the message identifier (two chars) and the length (three chars)
			timeRT[2] = buffer[actual_length+5+2];		
			timeRT[3] = buffer[actual_length+5+3];
                        
                        SendStrRS232("Receiver time: ", RS2_M_UART1);
			if (cmd_length < 5){
				SendStrRS232("N/A", RS2_M_UART1);
			}
			else if (cmd_length == 5){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else
				SendStrRS232("Error", RS2_M_UART1);

			//Check the checksum of the received sentence
			unsigned char check1 = cs(&buffer[actual_length], cmd_length+5-2);	// -2 is for the checksum char and '\n' 
			if(check1 != buffer[actual_length+cmd_length+5-2])	
				errors[0] = 2;													// If the checksum doesn't match set an error

			actual_length = actual_length+5+cmd_length;				// Update the current position at buffer (+5 is for the message
																				// identifier and the three chars for length calculation)
		}	
			
		else
			errors[0] = 1; 								// [~~] expected but it wasn't received, 
		

		// ============== Check for PO message =======================================================
		if(buffer[actual_length] == 'P' && buffer[actual_length+1] == 'O'){			

			SendRS232(&buffer[actual_length],1, RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);
	
			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("X coordenate: N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Y coordenate: N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Z coordenate: N/A\r\n", RS2_M_UART1);
                        }
                        else if (cmd_length == 30){
                            // Algo con las coordenadas
                        }
                        else {
                            SendStrRS232("X coordenate: Error\r\n", RS2_M_UART1);
                            SendStrRS232("Y coordenate: Error\r\n", RS2_M_UART1);
                            SendStrRS232("Z coordenate: Error\r\n", RS2_M_UART1);
                        }

			unsigned char check2 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check2 != buffer[actual_length+cmd_length+5-2])
				errors[1] = 2;

			actual_length = actual_length+5+cmd_length;	
		}
	
		else
			errors[1] = 1;
	
		// ============== Check for VE message =======================================================
		if(buffer[actual_length] == 'V' && buffer[actual_length+1] == 'E'){	
                        
			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("X velocity: N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Y velocity: N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Z velocity: N/A\r\n", RS2_M_UART1);
                        }
                        else if (cmd_length == 30){
                            // Algo con la velocidad
                        }
                        else {
                            SendStrRS232("X velocity: Error\r\n", RS2_M_UART1);
                            SendStrRS232("Y velocity: Error\r\n", RS2_M_UART1);
                            SendStrRS232("Z velocity: Error\r\n", RS2_M_UART1);
                        }

			unsigned char check3 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check3 != buffer[actual_length+cmd_length+5-2])
				errors[2] = 2;

			actual_length = actual_length+5+cmd_length;	
		}
		
		else
			errors[2] = 1;

		// ============== Check for PG message =======================================================
		if(buffer[actual_length] == 'P' && buffer[actual_length+1] == 'G'){	

			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;

                        if (cmd_length < 30){
                            SendStrRS232("Latitude  : N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Longitude : N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Altitude  : N/A\r\n", RS2_M_UART1);
                        }
                        else if (cmd_length == 30){
                            // Algo con la posicion geodetica
                        }
                        else {
                            SendStrRS232("Latitude  : Error\r\n", RS2_M_UART1);
                            SendStrRS232("Longitude : Error\r\n", RS2_M_UART1);
                            SendStrRS232("Altitude  : Error\r\n", RS2_M_UART1);
                        }

			unsigned char check4 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check4 != buffer[actual_length+cmd_length+5-2])
				errors[3] = 2;

			actual_length = actual_length+5+cmd_length;	
		}
		
		else
			errors[3] = 1;

		// ============== Check for VE message =======================================================
		if(buffer[actual_length] == 'V' && buffer[actual_length+1] == 'G'){	

			SendRS232(&buffer[actual_length],1, RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1, RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("Lat velocity : N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Lon velocity : N/A\r\n", RS2_M_UART1);
                            SendStrRS232("Alt velocity : N/A\r\n", RS2_M_UART1);
                        }
                        else if (cmd_length == 30){
                            // Algo con la posicion geodetica
                        }
                        else {
                            SendStrRS232("Lat velocity : Error\r\n", RS2_M_UART1);
                            SendStrRS232("Lon velocity : Error\r\n", RS2_M_UART1);
                            SendStrRS232("Alt velocity : Error\r\n", RS2_M_UART1);
                        }


			unsigned char check5 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check5 != buffer[actual_length+cmd_length+5-2])
				errors[4] = 2;

			actual_length = actual_length+5+cmd_length;	
		}
		
		else
			errors[4] = 1;


		// ============== Check for PT message =======================================================
		if(buffer[actual_length] == 'P' && buffer[actual_length+1] == 'T'){	

			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			unsigned char check6 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check6 != buffer[actual_length+cmd_length+5-2])
				errors[5] = 2;

			actual_length = actual_length+5+cmd_length;	
		}
	
		else
			errors[5] = 1;

		// ============== Check for PS message =======================================================
		if(buffer[actual_length] == 'P' && buffer[actual_length+1] == 'S'){	

			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

                        if (cmd_length < 9){
				SendStrRS232("GPS Locked : N/A\r\n", RS2_M_UART1);
				SendStrRS232("GPS Avail  : N/A\r\n", RS2_M_UART1);
				SendStrRS232("GPS Used   : N/A\r\n", RS2_M_UART1);
			}
			else if (cmd_length == 9){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("GPS Locked : Error\r\n", RS2_M_UART1);
				SendStrRS232("GPS Avail  : Error\r\n", RS2_M_UART1);
				SendStrRS232("GPS USed   : Error\r\n", RS2_M_UART1);
			}

			unsigned char check7 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check7 != buffer[actual_length+cmd_length+5-2])
				errors[6] = 2;

			actual_length = actual_length+5+cmd_length;	
		}


		else
			errors[6] = 1;

		// ============== Check for ET message =======================================================
		if(buffer[actual_length] == ':' && buffer[actual_length+1] == ':'){	

			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;		

			timeET[0] = buffer[actual_length+5+0];		// Store the time tag for "::" message. (End of epoch)	
			timeET[1] = buffer[actual_length+5+1];
			timeET[2] = buffer[actual_length+5+2];
			timeET[3] = buffer[actual_length+5+3];

                        SendStrRS232("Epoch time: ", RS2_M_UART1);
			if (cmd_length < 5){
				SendStrRS232("N/A", RS2_M_UART1);
			}
			else if (cmd_length == 5){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else
				SendStrRS232("Error", RS2_M_UART1);


			unsigned char check8 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check8 != buffer[actual_length+cmd_length+5-2])
				errors[7] = 2;

			actual_length = actual_length+5+cmd_length;	
		}

		else
			errors[7] = 1;
		
		// This message doesn't contain useful information. It's only to indicate the end of the data
		// It only contains one char of data (checksum) that should be the same for every EE message
		// ============== Check for EE message =======================================================
		if(buffer[actual_length] == '|' && buffer[actual_length+1] == '|'){	

			SendRS232(&buffer[actual_length],1 ,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1 ,RS2_M_UART1);

			a[0] = buffer[actual_length+2]; 
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;		

			unsigned char check9 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check9 != buffer[actual_length+cmd_length+5-2])
				errors[8] = 2;

			actual_length = actual_length+5+cmd_length;	
		}

		else
			errors[8] = 1;

		// ========= Check for desynchronization ==============================
		// (Compare time tags from start and end of epoch - [~~] and [::] messages)
		if(errors[0] == 0 && errors[7] == 0){
			if( (timeET[0] != timeRT[0]) || (timeET[1] != timeRT[1]) || (timeET[2] != timeRT[2]) || (timeET[3] != timeRT[3]))
				errors[9] = 1;
		}

		SendStrRS232("Error array ",1);			// Put the error array in screen
		SendRS232(&errors[0], 10 ,RS2_M_UART1);
		SendStrRS232("\n\r",1);

		if(errors[0] == 1 || errors[1] == 1 || errors[2] == 1 || errors[3] == 1 || errors[4] == 1 || errors[5] == 1 || errors[6] == 1 || errors[7] == 1 || errors[8] == 1)
			exitcode = 4;				// Exitcode for a missing sentence

		if(errors[0] == 2 || errors[1] == 2 || errors[2] == 2 || errors[3] == 2 || errors[4] == 2 || errors[5] == 2 || errors[6] == 2 || errors[7] == 2 || errors[8] == 2)
			exitcode = 5;				// Exitcode for a failed checksum

		if(errors[9] != 0)
			exitcode = 6;				// Exitcode for mismatch epochs from RT and ET messages
	}
		
	else
		exitcode = 1;					// If there was an error in reception put exitcode 1.

	return exitcode;

}

// ============================================================================================================
// Sends a command to get JAVAD messages (per_jmsg[] command)
// Arguments	: 
// Return	: 
// ============================================================================================================
unsigned char jmsg(void ){
	buffer = data;
	unsigned char success;
	lastline = 0;
	success = send_cmd(per_jmsg);
	return parse_jmsg(success, &data[0]);
}

// ============================================================================================================
// Parses the contents of JAVAD satellite status message (per_jsat[] command)
// Arguments	: void
// Return	: char exitcode
// ============================================================================================================
unsigned char parse_jsat(unsigned char success, unsigned char* buf){
	buffer = buf;
	unsigned long i;
	unsigned char exitcode;
	char a[] = {0x00, 0x00, 0x00, 0x00};
	char* end;
	unsigned long actual_length = 0;						// This variable stores the current position in buffer array
	unsigned long cmd_length = 0;
	unsigned char errors[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	unsigned char nsat;

	exitcode = 0;										// Success by default

	if(success == 0){									// If there wasn't problem at reception


		// ============== Check for EL message =======================================================
		if(buffer[actual_length] == 'E' && buffer[actual_length+1] == 'L'){

			SendRS232(&buffer[actual_length],1,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1,RS2_M_UART1);

			a[0] = buffer[actual_length+2];
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;
			nsat = cmd_length - 2;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("EL Error\r\n", RS2_M_UART1);
			}


			unsigned char check3 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check3 != buffer[actual_length+cmd_length+5-2])
				errors[0] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[0] = 1;

		// ============== Check for AZ message =======================================================
		if(buffer[actual_length] == 'A' && buffer[actual_length+1] == 'Z'){

			SendRS232(&buffer[actual_length],1,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1,RS2_M_UART1);

			a[0] = buffer[actual_length+2];
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("AZ Error\r\n", RS2_M_UART1);
			}

			unsigned char check4 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check4 != buffer[actual_length+cmd_length+5-2])
				errors[1] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[1] = 1;

		// ============== Check for EC message =======================================================
		if(buffer[actual_length] == 'E' && buffer[actual_length+1] == 'C'){

			SendRS232(&buffer[actual_length],1,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1,RS2_M_UART1);

			a[0] = buffer[actual_length+2];
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("EC Error\r\n", RS2_M_UART1);
			}

			unsigned char check5 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check5 != buffer[actual_length+cmd_length+5-2])
				errors[2] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[2] = 1;


		// ============== Check for TC message =======================================================
		if(buffer[actual_length] == 'T' && buffer[actual_length+1] == 'C'){

			SendRS232(&buffer[actual_length],1,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1,RS2_M_UART1);

			a[0] = buffer[actual_length+2];
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("TC Error\r\n", RS2_M_UART1);
			}

			unsigned char check6 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check6 != buffer[actual_length+cmd_length+5-2])
				errors[3] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[3] = 1;

		// ============== Check for SI message =======================================================
		if(buffer[actual_length] == 0x53 && buffer[actual_length+1] == 0x49){

			SendRS232(&buffer[actual_length],1,RS2_M_UART1);
			SendRS232(&buffer[actual_length+1],1,RS2_M_UART1);

			a[0] = buffer[actual_length+2];
			a[1] = buffer[actual_length+3];
			a[2] = buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;
			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("SI Error\r\n", RS2_M_UART1);
			}

			unsigned char check2 = cs(&buffer[actual_length], cmd_length+5-2);
			if(check2 != buffer[actual_length+cmd_length+5-2])
				errors[4] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[4] = 1;


		SendStrRS232("Error array ");			// Put the error array in screen
		SendRS232(&errors[0], 5, RS2_M_UART1);
		SendStrRS232("\n\r");

		if(errors[0] == 1 || errors[1] == 1 || errors[2] == 1 || errors[3] == 1 || errors[4] == 1 || errors[5] == 1)
			exitcode = 4;				// Exitcode for a missing sentence

		if(errors[0] == 2 || errors[1] == 2 || errors[2] == 2 || errors[3] == 2 || errors[4] == 2 || errors[5] == 2)
			exitcode = 5;				// Exitcode for a failed checksum
	}

	else
		exitcode = 1;					// If there was an error in reception put exitcode 1.

	return exitcode;
}

// ============================================================================================================
// Sends a command to get JAVAD messages for satellite status (per_jsat[] command)
// Arguments	:
// Return		:
// ============================================================================================================
unsigned char jsat(void ){
	buffer = data;
	unsigned char success;
	lastline = 0;
	success = send_cmd(per_jsat);
	return parse_jsat(success, &data[0]);
}