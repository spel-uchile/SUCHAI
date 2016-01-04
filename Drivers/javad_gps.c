/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "javad_gps.h"

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
2 - For "print" commands, received gps_data starts with "ER"
3 - For messages sent with "%%", the received gps_data doesn't content the "%%" indicator.
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

// Constant definition
unsigned char gps_char_nl               = {0x0A};

unsigned char gps_cmd_sleep_mode[] 	= "print,/par/sleep\r\n";
unsigned char gps_cmd_low_power[]	= "print,/par/lpm\r\n";

unsigned char gps_cmd_dismsg[]		= "%disable_msg%dm\r\n";

unsigned char gps_cmd_rcv_sn[] 		= "print,/par/rcv/sn\r\n";
unsigned char gps_cmd_rcv_id[]		= "print,/par/rcv/id\r\n";
unsigned char gps_cmd_rcv_model[]	= "print,/par/rcv/model\r\n";
unsigned char gps_cmd_rcv_vendor[]	= "print,/par/rcv/vendor\r\n";
unsigned char gps_cmd_rcv_uptime[]	= "print,/par/rcv/uptime\r\n";

unsigned char gps_cmd_rcv_mem[]		= "print,/par/rcv/mem\r\n";
unsigned char gps_cmd_rev_cfgw[]	= "print,/par/rcv/cfgw\r\n";
unsigned char gps_cmd_rev_all[]		= "print,/par/rcv\r\n";
unsigned char gps_cmd_ant_curinp[]	= "print,/par/ant/curinp\r\n";
unsigned char gps_cmd_ant_dc[]		= "print,/par/ant/dc\r\n";
//Include time information? (UTC, GPS, receiver time)
unsigned char gps_cmd_pwr_voltage[]	= "print,/par/pwr/ext\r\n";
unsigned char gps_cmd_pwr_board[]	= "print,/par/pwr/board\r\n";
unsigned char gps_cmd_ant_voltage[]	= "print,/par/pwr/extant\r\n";
unsigned char gps_cmd_ant_current[]	= "print,/par/pwr/extantdc\r\n";
unsigned char gps_cmd_board_temp[]	= "print,/par/dev/thermo/out\r\n";
unsigned char gps_cmd_proc_load[]	= "print,/par/load\r\n";

//"out" is used to get only one answer from the GPS, not a periodic one.
unsigned char gps_cmd_per_jmsg[]	= "out,,/msg/jps/{RT,PO,VE,PG,VG,PT,PS,ET,EE}\r\n";	// JAVAD's messages
unsigned char gps_cmd_per_jtext[]	= "out,,/msg/jps/NP\r\n";				// JAVAD's text messages (ASCII)
unsigned char gps_cmd_per_jsat[]	= "out,,/msg/jps/{EL,AZ,EC,TC,SI}\r\n";                 // JAVAD's messages for satellite status
unsigned char gps_cmd_per_nmea_gga[]	= "out,,/msg/nmea/{GGA}\r\n";                   // NMEA sentences
unsigned char gps_cmd_per_nmea_gsa[]	= "out,,/msg/nmea/{GSA}\r\n";                   // NMEA sentences
unsigned char gps_cmd_per_nmea_gsv[]	= "out,,/msg/nmea/{GSV}\r\n";                   // NMEA sentences
unsigned char gps_cmd_per_nmea_rmc[]	= "out,,/msg/nmea/{RMC}\r\n";                   // NMEA sentences
//unsigned char gps_cmd_per_nmea[]	= "out,,/msg/nmea/{GGA,ZDA,VTG}\r\n";                   // NMEA sentences

unsigned char* gps_buffer;
unsigned char gps_flag_isr_busy     = 0;			// This variable indicates when the MCU should be waiting answers from the GPS
unsigned int  gps_flag_isr_counter  = 0;
unsigned char gps_flag_isr_status   = 0;			// Used in one function (send_cmd), indicates when an error in reception (UART) was detected
unsigned char gps_flag_lastline     = 0;			// This variable indicates when the MCU should expect a final line from the GPS
unsigned char gps_aux[]             = {0x00, 0x00, 0x00};	// This is a variable to check for some particular set of characters in a sentence
unsigned long gps_cmdl              = 0;			// This variable stores the length of a command

//Serial ports configuration registers
#define GPS_BRATE           34 		// 115200 Bd (BREGH=1) @ 32MHz
#define GPS_U_ENABLE        0x8008		// Enable UART, BREGH=1, 1 stop, no parity
#define GPS_U_TX            0x0400 		// Enable transmission, clear all flags
#define GPS_BUFF_LEN        (100) 		// Enable transmission, clear all flags

unsigned char gps_data[GPS_BUFF_LEN];
//

unsigned char *gps_exec_cmd(unsigned int cmd_num){
    unsigned char *gps_cmd;
    switch(cmd_num){
        case 0:
            gps_cmd = gps_cmd_sleep_mode;
            break;
        case 1:
            gps_cmd = gps_cmd_low_power;
            break;
        case 2:
            gps_cmd = gps_cmd_dismsg;
            break;
        case 3:
            gps_cmd = gps_cmd_rcv_sn;
            break;
        case 4:
            gps_cmd = gps_cmd_rcv_id;
            break;
        case 5:
            gps_cmd = gps_cmd_rcv_model;
            break;
        case 6:
            gps_cmd = gps_cmd_rcv_vendor;
            break;
        case 7:
            gps_cmd = gps_cmd_rcv_uptime;
            break;
        case 8:
            gps_cmd = gps_cmd_rcv_mem;
            break;
        case 9:
            gps_cmd = gps_cmd_rev_cfgw;
            break;
        case 10:
            gps_cmd = gps_cmd_rev_all;
            break;
        case 11:
            gps_cmd = gps_cmd_ant_curinp;
            break;
        case 12:
            gps_cmd = gps_cmd_ant_dc;
            break;
        case 13:
            gps_cmd = gps_cmd_pwr_voltage;
            break;
        case 14:
            gps_cmd = gps_cmd_pwr_board;
            break;
        case 15:
            gps_cmd = gps_cmd_ant_voltage;
            break;
        case 16:
            gps_cmd = gps_cmd_ant_current;
            break;
        case 17:
            gps_cmd = gps_cmd_board_temp;
            break;
        case 18:
            gps_cmd = gps_cmd_proc_load;
            break;
        case 19:
            gps_cmd = gps_cmd_per_jmsg;
            break;
        case 20:
            gps_cmd = gps_cmd_per_jtext;
            break;
        case 21:
            gps_cmd = gps_cmd_per_jsat;
            break;
        case 22:
            gps_cmd = gps_cmd_per_nmea_gga;
            break;
        case 23:
            gps_cmd = gps_cmd_per_nmea_gsa;
            break;
        case 24:
            gps_cmd = gps_cmd_per_nmea_gsv;
            break;
        case 25:
            gps_cmd = gps_cmd_per_nmea_rmc;
            break;
        default:
            printf("gps_send_cmd  invalid_cmd \r\n");
            return 0;
            break;
    }
    printf("gps_send_cmd  %s", gps_cmd);
    
    unsigned int exitcode;
    exitcode = gps_send_cmd(gps_cmd);
    //gps_print_buffer();

    unsigned char *gps_buff;
    gps_buff = gps_get_buffer();

    //printf("gps_parse ..\r\n");
    //success = gps_parse(success, &gps_data[0]);
    //gps_print_buffer();

//    gps_clear_buffer();
//    gps_clearUARTbuffer();
//    //gps_print_buffer();

    //return exitcode;
    return gps_buff;
}
int gps_wait_busy_wtimeout(void)
{
    long int i = 5*2; /* Maximum time to wait 5 seconds */
    while(gps_flag_isr_busy)
    {
        __delay_ms(500); /*Delay half second (0.5 secs)*/
        i--;
        if(i<=0)
        {
            printf("GPS_BUSY timeout !!\n");
            gps_flag_isr_busy = 0;
            gps_flag_isr_counter = 0;
            return 0;
        }
    }

    return 1;
}
// ============================================================================================================
// Sends a command using serial port UART4
// Arguments	: unsigned char* cmd - command to be sent
// Return	: gps_control (0 if there was an error, 1 if not) [See the interrupt routine to more info]
// ============================================================================================================
unsigned char gps_send_cmd(unsigned char* cmd){
	gps_flag_isr_status = 0;
	gps_flag_isr_busy = 1;                          // Set the gps busy (A command will be sent and then an answer should be expected)
	gps_aux[2] = 0x00;                          // Clear the aux array
	gps_aux[1] = 0x00;
	gps_aux[0] = 0x00;
	unsigned char temp;

	temp = cmd[0];
	while (temp != 0x0A){                       // Every command should finish with a '\n' (0x0A) character
		SendRS232(&temp, 1, RS2_M_UART4);
		temp = *(++cmd);
	}
	SendRS232(&gps_char_nl, 1, RS2_M_UART4);    // Sends a '\n' character (to finish the command)

	//IEC5bits.U4RXIE = 1;                        // Enable interruption to receive gps_data
        EnableIntU4RX;

	//while(gps_flag_busy);                       // Wait until UART port has received all the gps_data [See interrupt routine for more info]
        //__delay_ms(5000);
        gps_wait_busy_wtimeout();

	return gps_flag_isr_status;                    // Return the status [Modified in the interruption routine]
}



// ============================================================================================================
// Clears the receiver buffer named "buffer" (The common place for receiving responses from GPS).
// Arguments	: Unsigned char* buf - buffer to be cleared
// Return	: None
// ============================================================================================================
unsigned char *gps_get_buffer(void){
    return gps_data;
}
void gps_print_buffer(void){
    printf("gps_data: %s", gps_data);
//    printf("gps_data: %s \r\n");
//    printf("%s", gps_data);
}
void gps_clear_buffer(void ){
    gps_buffer = gps_data;
    unsigned int i;
    for (i=0; i<(GPS_BUFF_LEN-1); i++){		// Limit for i should be length(buffer)-1. In this case, length is GPS_BUFF_LEN.
        gps_buffer[i] = '\0';
    }
    //gps_buffer[(GPS_BUFF_LEN-2)] = '\n';
    gps_buffer[(GPS_BUFF_LEN-1)] = '\0';

    gps_buffer = gps_data;  // set back gps_buffer pointer to the first character of gps_data
}

// ============================================================================================================
// Parses the content of the buffer for a command in particular (not periodic or position gps_data)
// Arguments	: unsigned char success - status from send_cm
//		  unsigned char* buff - buffer with gps_data received
// Return	: char exitcode (see explanation at upper part of this file)
// ============================================================================================================
unsigned char gps_parse(unsigned char success, unsigned char* buf){

	gps_buffer = buf;					// Uses the buffer from input parameters
	unsigned long i;
	unsigned char exitcode;
	char a[] = {0x00, 0x00, 0x00, 0x00};
	char* end;

	if (success == 0){					// If a command was received successfully (gps_control from send_cmd function)
		if (gps_buffer[0] == 0x52 && gps_buffer[1] == 0x45){	// Check if the gps_data starts with "RE"
			a[0] = gps_buffer[2];
			a[1] = gps_buffer[3];
			a[2] = gps_buffer[4];
			a[3] = '\0';
			gps_cmdl = strtoul(a, &end, 16)+2;		// Calculates the length of the answer
								// + 2 is for CR and LF (Only for commands with "print" preamble)
			for (i=0; i<gps_cmdl; i++){			// Using the length, send all the gps_data stored in the buffer to screen using UART2

				//SendRS232(&buffer[5+i],1, RS2_M_UART4);
                                SendRS232(&gps_buffer[5+i],1, RS2_M_UART1);
				__delay_ms(10);
			}

		}
		else if (gps_buffer[0] == 0x45 && gps_buffer[1] == 0x52){	// If first two characters are 'E' and 'R', return exitcode 2
			exitcode = 2;
		}
		else							// This case is for an unknown error.
			exitcode = 9;
	}
	else                                                            // If there was an error in gps_data reception, send this exitcode.
		exitcode = 1;

	return exitcode;
}

// ============================================================================================================
// Gets de model of the receiver
// Arguments	: void
// Return	: char with an exitcode (see explanation at upper part of this file)
// ============================================================================================================
unsigned char gps_model(void){
    //gps_buffer = gps_data;
    unsigned char success;
    //gps_flag_lastline = 1;

    printf("gps_send_cmd ..\r\n");
    success = gps_send_cmd(gps_cmd_rcv_model);
    gps_print_buffer();

    //printf("gps_parse ..\r\n");
    //success = gps_parse(success, &gps_data[0]);
    //gps_print_buffer();

    //printf("gps_clear_buffer ..\r\n");
    gps_clear_buffer();
    //gps_print_buffer();

    //printf("gps_clearUARTbuffer ..\r\n");
    gps_clearUARTbuffer();
    //gps_print_buffer();

    return success;
}

// ============================================================================================================
// Gets de serial number of the receiver
// Arguments	:
// Return	: 0 for success, 1 for error receiving the answer, 2 for ER from the GPS and
//		  3 for unknown error.
// ============================================================================================================
unsigned char gps_serialn(void ){
	gps_buffer = gps_data;
	unsigned char success;
	gps_flag_lastline = 1;
	success = gps_send_cmd(gps_cmd_rcv_sn);
	return gps_parse(success, &gps_data[0]);
}

// ============================================================================================================
// Stops all the messages in the current terminal of the GPS
// Arguments	:
// Return	: 0 for success, 1 for error receiving the answer, 2 for ER from the GPS and
//                3 for unknown error.
// ============================================================================================================
unsigned char gps_senddm(void ){
	gps_buffer = gps_data;
	unsigned char success;
	unsigned char retparse;
	gps_flag_lastline = 1;
	success = gps_send_cmd(gps_cmd_dismsg);
	retparse = gps_parse(success, &gps_data[0]);
	unsigned int j;
	if (retparse == 0){
		for (j=0; j<(gps_cmdl-2); j++){                // -2 is for '\r' and '\n'
			if(gps_cmd_dismsg[j] != gps_buffer[5+j]){
				retparse = 3;
				break;
			}
		}
	}

	return retparse;
}

// ============================================================================================================
// Calculates the checksum of achar array (Algorithm taken from JAVAD's GREIS Manual)
// Arguments	: char* gps_data - Data for checksum calculation
//                unsigned int count - Length of the gps_data
// Return	: unsigned char checksum
// ============================================================================================================
unsigned char gps_cs(unsigned char* d, unsigned int count){
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
unsigned char gps_parse_jmsg(unsigned char success, unsigned char* buf){
	gps_buffer = buf;
	//unsigned long i;
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

		if(gps_buffer[actual_length] == '~' && gps_buffer[actual_length+1] == '~'){	// Check if the sentence starts with "~~"


			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);	// Put the first message identifier character
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);	// Put the second message identifier character


			a[0] = gps_buffer[actual_length+2]; 		// Store the length of the received answer
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';

                        cmd_length = strtoul(a, &end, 16)+1;		// Calculate the length of the sentence received (strtoul function)


                        // +1 is because the answer always ends with '\n' character

			timeRT[0] = gps_buffer[actual_length+5+0];		// Store the gps_data tag received in this message (start of epoch)
			timeRT[1] = gps_buffer[actual_length+5+1];		// +5 is for the message identifier (two chars) and the length (three chars)
			timeRT[2] = gps_buffer[actual_length+5+2];
			timeRT[3] = gps_buffer[actual_length+5+3];

                        SendStrRS232("Receiver time: ", RS2_M_UART4);
			if (cmd_length < 5){
				SendStrRS232("N/A", RS2_M_UART4);
			}
			else if (cmd_length == 5){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else
				SendStrRS232("Error", RS2_M_UART4);

			//Check the checksum of the received sentence
			unsigned char check1 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);	// -2 is for the checksum char and '\n'
			if(check1 != gps_buffer[actual_length+cmd_length+5-2])
				errors[0] = 2;													// If the checksum doesn't match set an error

			actual_length = actual_length+5+cmd_length;				// Update the current position at buffer (+5 is for the message
																				// identifier and the three chars for length calculation)
		}

		else
			errors[0] = 1; 								// [~~] expected but it wasn't received,


		// ============== Check for PO message =======================================================
		if(gps_buffer[actual_length] == 'P' && gps_buffer[actual_length+1] == 'O'){

			SendRS232(&gps_buffer[actual_length],1, RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("X coordenate: N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Y coordenate: N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Z coordenate: N/A\r\n", RS2_M_UART4);
                        }
                        else if (cmd_length == 30){
                            // Algo con las coordenadas
                        }
                        else {
                            SendStrRS232("X coordenate: Error\r\n", RS2_M_UART4);
                            SendStrRS232("Y coordenate: Error\r\n", RS2_M_UART4);
                            SendStrRS232("Z coordenate: Error\r\n", RS2_M_UART4);
                        }

			unsigned char check2 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check2 != gps_buffer[actual_length+cmd_length+5-2])
				errors[1] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[1] = 1;

		// ============== Check for VE message =======================================================
		if(gps_buffer[actual_length] == 'V' && gps_buffer[actual_length+1] == 'E'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("X velocity: N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Y velocity: N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Z velocity: N/A\r\n", RS2_M_UART4);
                        }
                        else if (cmd_length == 30){
                            // Algo con la velocidad
                        }
                        else {
                            SendStrRS232("X velocity: Error\r\n", RS2_M_UART4);
                            SendStrRS232("Y velocity: Error\r\n", RS2_M_UART4);
                            SendStrRS232("Z velocity: Error\r\n", RS2_M_UART4);
                        }

			unsigned char check3 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check3 != gps_buffer[actual_length+cmd_length+5-2])
				errors[2] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[2] = 1;

		// ============== Check for PG message =======================================================
		if(gps_buffer[actual_length] == 'P' && gps_buffer[actual_length+1] == 'G'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;

                        if (cmd_length < 30){
                            SendStrRS232("Latitude  : N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Longitude : N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Altitude  : N/A\r\n", RS2_M_UART4);
                        }
                        else if (cmd_length == 30){
                            // Algo con la posicion geodetica
                        }
                        else {
                            SendStrRS232("Latitude  : Error\r\n", RS2_M_UART4);
                            SendStrRS232("Longitude : Error\r\n", RS2_M_UART4);
                            SendStrRS232("Altitude  : Error\r\n", RS2_M_UART4);
                        }

			unsigned char check4 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check4 != gps_buffer[actual_length+cmd_length+5-2])
				errors[3] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[3] = 1;

		// ============== Check for VE message =======================================================
		if(gps_buffer[actual_length] == 'V' && gps_buffer[actual_length+1] == 'G'){

			SendRS232(&gps_buffer[actual_length],1, RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1, RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;


                        if (cmd_length < 30){
                            SendStrRS232("Lat velocity : N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Lon velocity : N/A\r\n", RS2_M_UART4);
                            SendStrRS232("Alt velocity : N/A\r\n", RS2_M_UART4);
                        }
                        else if (cmd_length == 30){
                            // Algo con la posicion geodetica
                        }
                        else {
                            SendStrRS232("Lat velocity : Error\r\n", RS2_M_UART4);
                            SendStrRS232("Lon velocity : Error\r\n", RS2_M_UART4);
                            SendStrRS232("Alt velocity : Error\r\n", RS2_M_UART4);
                        }


			unsigned char check5 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check5 != gps_buffer[actual_length+cmd_length+5-2])
				errors[4] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[4] = 1;


		// ============== Check for PT message =======================================================
		if(gps_buffer[actual_length] == 'P' && gps_buffer[actual_length+1] == 'T'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			unsigned char check6 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check6 != gps_buffer[actual_length+cmd_length+5-2])
				errors[5] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[5] = 1;

		// ============== Check for PS message =======================================================
		if(gps_buffer[actual_length] == 'P' && gps_buffer[actual_length+1] == 'S'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

                        if (cmd_length < 9){
				SendStrRS232("GPS Locked : N/A\r\n", RS2_M_UART4);
				SendStrRS232("GPS Avail  : N/A\r\n", RS2_M_UART4);
				SendStrRS232("GPS Used   : N/A\r\n", RS2_M_UART4);
			}
			else if (cmd_length == 9){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("GPS Locked : Error\r\n", RS2_M_UART4);
				SendStrRS232("GPS Avail  : Error\r\n", RS2_M_UART4);
				SendStrRS232("GPS USed   : Error\r\n", RS2_M_UART4);
			}

			unsigned char check7 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check7 != gps_buffer[actual_length+cmd_length+5-2])
				errors[6] = 2;

			actual_length = actual_length+5+cmd_length;
		}


		else
			errors[6] = 1;

		// ============== Check for ET message =======================================================
		if(gps_buffer[actual_length] == ':' && gps_buffer[actual_length+1] == ':'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			timeET[0] = gps_buffer[actual_length+5+0];		// Store the time tag for "::" message. (End of epoch)
			timeET[1] = gps_buffer[actual_length+5+1];
			timeET[2] = gps_buffer[actual_length+5+2];
			timeET[3] = gps_buffer[actual_length+5+3];

                        SendStrRS232("Epoch time: ", RS2_M_UART4);
			if (cmd_length < 5){
				SendStrRS232("N/A", RS2_M_UART4);
			}
			else if (cmd_length == 5){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else
				SendStrRS232("Error", RS2_M_UART4);


			unsigned char check8 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check8 != gps_buffer[actual_length+cmd_length+5-2])
				errors[7] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[7] = 1;

		// This message doesn't contain useful information. It's only to indicate the end of the gps_data
		// It only contains one char of gps_data (checksum) that should be the same for every EE message
		// ============== Check for EE message =======================================================
		if(gps_buffer[actual_length] == '|' && gps_buffer[actual_length+1] == '|'){

			SendRS232(&gps_buffer[actual_length],1 ,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1 ,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			unsigned char check9 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check9 != gps_buffer[actual_length+cmd_length+5-2])
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
		SendRS232(&errors[0], 10 ,RS2_M_UART4);
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
unsigned char gps_jmsg(void ){
	gps_buffer = gps_data;
	unsigned char success;
	gps_flag_lastline = 0;
	success = gps_send_cmd(gps_cmd_per_jmsg);
	return gps_parse_jmsg(success, &gps_data[0]);
}

// ============================================================================================================
// Parses the contents of JAVAD satellite status message (per_jsat[] command)
// Arguments	: void
// Return	: char exitcode
// ============================================================================================================
unsigned char gps_parse_jsat(unsigned char success, unsigned char* buf){
	gps_buffer = buf;
	//unsigned long i;
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
		if(gps_buffer[actual_length] == 'E' && gps_buffer[actual_length+1] == 'L'){

			SendRS232(&gps_buffer[actual_length],1,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;
			nsat = cmd_length - 2;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("EL Error\r\n", RS2_M_UART4);
			}


			unsigned char check3 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check3 != gps_buffer[actual_length+cmd_length+5-2])
				errors[0] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[0] = 1;

		// ============== Check for AZ message =======================================================
		if(gps_buffer[actual_length] == 'A' && gps_buffer[actual_length+1] == 'Z'){

			SendRS232(&gps_buffer[actual_length],1,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("AZ Error\r\n", RS2_M_UART4);
			}

			unsigned char check4 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check4 != gps_buffer[actual_length+cmd_length+5-2])
				errors[1] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[1] = 1;

		// ============== Check for EC message =======================================================
		if(gps_buffer[actual_length] == 'E' && gps_buffer[actual_length+1] == 'C'){

			SendRS232(&gps_buffer[actual_length],1,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("EC Error\r\n", RS2_M_UART4);
			}

			unsigned char check5 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check5 != gps_buffer[actual_length+cmd_length+5-2])
				errors[2] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[2] = 1;


		// ============== Check for TC message =======================================================
		if(gps_buffer[actual_length] == 'T' && gps_buffer[actual_length+1] == 'C'){

			SendRS232(&gps_buffer[actual_length],1,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';
			cmd_length = strtoul(a, &end, 16)+1;

			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("TC Error\r\n", RS2_M_UART4);
			}

			unsigned char check6 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check6 != gps_buffer[actual_length+cmd_length+5-2])
				errors[3] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[3] = 1;

		// ============== Check for SI message =======================================================
		if(gps_buffer[actual_length] == 0x53 && gps_buffer[actual_length+1] == 0x49){

			SendRS232(&gps_buffer[actual_length],1,RS2_M_UART4);
			SendRS232(&gps_buffer[actual_length+1],1,RS2_M_UART4);

			a[0] = gps_buffer[actual_length+2];
			a[1] = gps_buffer[actual_length+3];
			a[2] = gps_buffer[actual_length+4];
			a[3] = '\0';

			cmd_length = strtoul(a, &end, 16)+1;
			if (nsat == 0){
			}
			else if (nsat > 0){
				// algo con buffer[actual_length+5+i] desde i = hasta i<cmd_length
			}
			else{
				SendStrRS232("SI Error\r\n", RS2_M_UART4);
			}

			unsigned char check2 = gps_cs(&gps_buffer[actual_length], cmd_length+5-2);
			if(check2 != gps_buffer[actual_length+cmd_length+5-2])
				errors[4] = 2;

			actual_length = actual_length+5+cmd_length;
		}

		else
			errors[4] = 1;


		printf("Error array ");			// Put the error array in screen
		//RS232(&errors[0], 5, RS2_M_UART4);
                printf("%c %c %c %c %c ", errors[0], errors[1], errors[2], errors[3], errors[4]);
		printf("\n\r");

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
unsigned char gps_jsat(void ){
	gps_buffer = gps_data;
	unsigned char success;
	gps_flag_lastline = 0;
	success = gps_send_cmd(gps_cmd_per_jsat);
	return gps_parse_jsat(success, &gps_data[0]);
}

// ============================================================================================================
// Clears the UART4 receiver buffer to erase previous unwanted gps_data and to clear overflow flag
// Arguments	: None
// Return	: None
// ============================================================================================================
void gps_clearUARTbuffer(void ){
	unsigned int i;
	unsigned char r;
	for(i=0; i<=5; i++){
		r = U4RXREG;
	}
	U4STAbits.OERR = 0;
}

// ============================================================================================================
// Interrupt that manages gps_data reception in RX line of UART4
// Arguments	: None
// Return	: None
// ============================================================================================================
//void __attribute__((__interrupt__, auto_psv)) _U4RXInterrupt(void ){
//
//	unsigned char rec = U4RXREG;
//
//	if ((U4STAbits.PERR == 0) && (U4STAbits.FERR == 0)){ 	// Only use the gps_data if there was no error in reception
//		*gps_buffer = rec;					// Put gps_data into a buffer
//
//		if(gps_flag_lastline){				// Check if this is the last line that should be received
//			if (rec == 0x0A){		// When lastline = 1, the gps_data should end with a '\n' charcater.
//				gps_flag_control = 0;	// gpscontrol = 0 indicates no error
//				gps_flag_busy = 0;		// gpsbusy = 0 returns the control to send_cmd function
//				IFS5bits.U4RXIF = 0;    // Clear RX interrupt flag
//				IEC5bits.U4RXIE = 0;    // Disable RX interrupts (No more characters should be received)
//				return;
//			}
//		}
//
//		else{
//			gps_aux[2] = gps_aux[1];
//			gps_aux[1] = gps_aux[0];
//			gps_aux[0] = rec;
//
//			// Check for a particular set of characters to raise lastline variable
//			// The set to be checked depends on the order in which the commands are sent.
//			// (See per_jmsg[], per_jtext[] and per_nmea[] variables)
//			if((gps_aux[1] == '|' && gps_aux[0] == '|') || (gps_aux[2] == 'V' && gps_aux[1] == 'T' && gps_aux[0] == 'G')){
//				gps_flag_lastline = 1;
//			}
//		}
//
//		IFS5bits.U4RXIF = 0;        // Clear RX interrupt flag
//		IEC5bits.U4RXIE = 1;        // Enable RX interrupts
//		gps_buffer++;
//
//                // ************************************
//		// NEEDS A CODE TO EXIT USING A TIMEOUT
//                // ************************************
//	}
//
//	else{
//		gps_flag_control = 1;			// If there was an error in reception, set gps_control
//		gps_flag_busy = 0;				// Return the control to send_cmd function
//		IFS5bits.U4RXIF = 0;       	// Clear RX interrupt flag
//		IEC5bits.U4RXIE = 0;        // Disable RX interrupt
//	}
//}













//////////////////

//INT16 GPS_MAX_READ = 0; //Numero de bytes que se espera recibir desde lagmuir
//INT16 GPS_COUNT = 0; //Contador de bytes recibidos
//BOOL GPS_BUSY = 0; //Estado de la comunicacion
//#define GPS_BUFFER_LEN  (1000)
//unsigned int gps_buffer[GPS_BUFFER_LEN];     /* Buffer para datos de langmuir */
//
//unsigned char gps_send_cmd2(unsigned char* cmd){
//	gps_control = 0;
//	gps_busy = 1;							// Set the gps busy (A command will be sent and then an answer should be expected)
//	aux[2] = 0x00;							// Clear the aux array
//	aux[1] = 0x00;
//	aux[0] = 0x00;
//	unsigned char temp;
//
//        // Verificar si es UART 1 o 2
//	temp = cmd[0];
//	while (temp != 0x0A){			// Every command should finish with a '\n' (0x0A) character
//		SendRS232(&temp, 1, RS2_M_UART4);
//		temp = *(++cmd);
//	}
//	SendRS232(&nl, 1, RS2_M_UART4);         // Sends a '\n' character (to finish the command)
//
//	//IEC5bits.U4RXIE = 1;			// Enable interruption to receive gps_data
//
//	while(gps_busy);			// Wait until UART port has received all the gps_data [See interrupt routine for more info]
//	return gps_control;			// Return the status [Modified in the interruption routine]
//}
//unsigned char gps_send_cmd_model(unsigned char* cmd){
//    buffer = gps_data;
//    unsigned char success;
//    lastline = 1;
//    success = gps_send_cmd2(rcv_model);
//    return 'c';
//}
//
//void gps_print_buffer(int len)
//{
//    printf ("gps_print_buffer ..\r\n");
//
//    int i;
//    printf ("HEX\r\n");
//    for(i = 0; i<len; i++)
//    {
//        printf ("0x%X,", (unsigned int)gps_buffer[i] );
//    }
//    printf ("DEC\r\n");
//    for(i = 0; i<len; i++)
//    {
//        printf ("%d,", (unsigned int)gps_buffer[i] );
//    }
//    printf ("ASCII\r\n");
//    for(i = 0; i<len; i++)
//    {
//        printf ("%c,", (unsigned int)gps_buffer[i] );
//    }
//    printf("\n");
//}
//
//int gps_wait_busy_wtimeout(void)
//{
//    long int i = 30*2; /* Maximum time to wait 30 seconds */
//    while(GPS_BUSY)
//    {
//        __delay_ms(500); /*Delay half second (0.5 secs)*/
//        i--;
//        if(i<=0)
//        {
//            printf("GPS_BUSY timeout !!\n");
//            GPS_BUSY = 0;
//            GPS_COUNT = 0;
//            return 0;
//        }
//    }
//
//    return 1;
//}
//void gps_erase_buffer(void)
//{
//    int i;
//    for(i = 0; i<GPS_BUFFER_LEN; i++)
//    {
//        gps_buffer[i] = 0;
//    }
//}
//int gps_exe_cmd(BOOL verb)
//{
//    gps_erase_buffer();
//    GPS_MAX_READ = 48;//12*4
//    GPS_COUNT = 0;
//    GPS_BUSY = 1;
//    gps_print_buffer(GPS_MAX_READ);
//    return 1;
//
////    GPS_MAX_READ = 48;//12*4
////    GPS_COUNT = 0;
////    GPS_BUSY = 1;
////
////    //erase buffer
////    gps_erase_buffer();
////
////    //Write CAL function
////    gps_send_cmd_model(NULL);
////
////    /* Wait some seconds (with time out) */
////    int r = gps_wait_busy_wtimeout();
////
////    if(verb)
////        gps_print_buffer(GPS_MAX_READ);
////
////    if(r)
////        return GPS_MAX_READ;
////    else
////        return 0;
//}

void __attribute__((__interrupt__, auto_psv)) _U4RXInterrupt(void ){
    unsigned int c = ReadRS232(RS2_M_UART4);
    gps_data[gps_flag_isr_counter] = c;
    gps_flag_isr_counter++;

    if(gps_flag_isr_counter >= GPS_BUFF_LEN){ //Overflow
        gps_data[(GPS_BUFF_LEN-1)] = '\0';
        gps_flag_isr_counter = 0;
        DisableIntU4RX;
    }

    if(c=='\n'){  // New line => end of cmd
        gps_data[gps_flag_isr_counter] = '\0';
        gps_data[(GPS_BUFF_LEN-1)] = '\0';
        gps_flag_isr_counter = 0;
        DisableIntU4RX;
        gps_flag_isr_busy = 0;
        gps_flag_isr_status = 1;
    }

    U4RX_Clear_Intr_Status_Bit;
}