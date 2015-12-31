/*                          LAGMUIR PROBE DRIVER
 *                              For PIC24F
 *
 *      Copyright 2012, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

#include "langmuir.h"

char lag_ostream[4] = {LAG_SYNC, LAG_SYNC, LAG_SYNC, LAG_SYNC}; //Buffer de salida
unsigned int langmuir_buffer[LAG_BUFFER_LEN]; /* Buffer para datos de langmuir */

INT16 LAG_MAX_READ = 0; //Numero de bytes que se espera recibir desde lagmuir
INT16 LAG_COUNT = 0; //Contador de bytes recibidos
BOOL LAG_BUSY = 0; //Estado de la comunicacion

static int lag_send_ctrl_packet(int function);

/*------------------------------------------------------------------------------
 *                      LAG SEND CTRL PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a calibration
 *                      packet (3Values x 2Bytes)
 * Arguments          : int* buffer[3] - Receive buffer
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
static int lag_send_ctrl_packet(int function)
{
    // Check params for availible functions
    if((function > 0x00C3) || (function < 0x00C1))
        return 0;

    // Send data via RS232
    lag_ostream[3] = (char)function;
    langmuir_write((unsigned char*)lag_ostream, 4);
    return 1;
}

/**
 * Show buffer content
 * @param len
 */
void lag_print_buffer(int len)
{
    int i;
    for(i = 0; i<len; i++)
    {
        printf ("0x%X,", (unsigned int)langmuir_buffer[i] );
    }
    printf("\n");
}

/**
 * Reset buffer content
 */
void lag_erase_buffer(void)
{
    int i;
    for(i = 0; i<LAG_BUFFER_LEN; i++)
    {
        langmuir_buffer[i] = 0;
    }
}

/**
 * Waits for langmuir ends or timeout
 * @return 1 ok, 0 timeout
 */
int lag_wait_busy_wtimeout(void)
{
    long int i = 30*2; /* Maximum time to wait 30 seconds */
    while(LAG_BUSY)
    {
        __delay_ms(500); /*Delay half second (0.5 secs)*/
        i--;
        if(i<=0)
        {
            printf("LAG_BUSY timeout !!\n");
            LAG_BUSY = 0;
            LAG_COUNT = 0;
            return 0;
        }
    }

    return 1;
}

/**
 * Get langmuir buffer content
 * @param ind
 * @return
 */
unsigned int lag_get_langmuir_buffer_i(int ind)
{
    if(ind>=LAG_BUFFER_LEN)
        return 0;

    return langmuir_buffer[ind];
}

/*------------------------------------------------------------------------------
 *		 	LAG READ CAL PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a calibration
 *                      packet (4packets x 12Bytes)
 * Arguments          : unsigned int* buffer[48] - Receive buffer
 *                      Lagmuir will send 48 bytes
 *						-previous message format details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 *						-current message format (12Bytes):
 *							43 43 43 01-04 	(3Bytes header + 1Byte ID Calibration -resistance identifier-)
 *							XX XX 			(2 byte Sweep Voltage: 4V)
 *							YY YY 			(2 byte Plasma Voltage)
 *							TT TT 			(2 byte Temperature ºK)
 *							ZZ ZZ 			(2 byte Particle Counter)
 * Return Value       : Number of values to read from bubffer, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_cal_packet(BOOL verb)
{
    LAG_MAX_READ = 48;//12*4
    LAG_COUNT = 0;
    LAG_BUSY = 1;

    //erase buffer
    lag_erase_buffer();

    //Write CAL function
    lag_send_ctrl_packet(LAG_CAL);

    /* Wait some seconds (with time out) */
    int r = lag_wait_busy_wtimeout();

    if(verb)
        lag_print_buffer(LAG_MAX_READ);

    if(r)
        return LAG_MAX_READ;
    else
        return 0;
}

/*------------------------------------------------------------------------------
 *		 	LAG READ PLASMA PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma
 *                      packet (1packet x 12Bytes)
 * Arguments          : unsgined int* buffer[12] - Receive buffer
 *                      Lagmuir will send 12 bytes, details in
  *						-previous message format details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 *						-current message format (12Bytes):
 *							43 43 43 05 (3Bytes header + 1Byte ID Plasma)
 *							XX XX 		(2 byte Sweep Voltage: 4V)
 *							YY YY 		(2 byte Plasma Voltage)
 *							TT TT 		(2 byte Temperature ºK)
 *							ZZ ZZ 		(2 byte Particle Counter)
 * Return Value       : Number of values to read from bubffer, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_plasma_packet(BOOL verb)
{
    LAG_MAX_READ = 12;
    LAG_COUNT = 0;
    LAG_BUSY = 1;

    //erase buffer
    lag_erase_buffer();

    //Write PLASMA function
    lag_send_ctrl_packet(LAG_PLASMA);

    /* Wait some seconds (with time out) */
    int r = lag_wait_busy_wtimeout();

    if(verb)
        lag_print_buffer(LAG_MAX_READ);

    if(r)
        return LAG_MAX_READ;
    else
        return 0;
}

/*------------------------------------------------------------------------------
 *		 	LAG READ SWEEP PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma sweep
 *                      packet (1packet x 1096Bytes)
 * Arguments          : unsgined int* buffer[1096] - Receive buffer
 *                      Lagmuir will send 1096 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : Number of values to read from bubffer, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_sweep_packet(BOOL verb)
{
    LAG_MAX_READ = 1096;
    LAG_COUNT = 0;
    LAG_BUSY = 1;

    //erase buffer
    lag_erase_buffer();

    //Write SWEEP function
    lag_send_ctrl_packet(LAG_SWEEP);

    /* Wait some seconds (with time out) */
    int r = lag_wait_busy_wtimeout();

    if(verb)
        lag_print_buffer(LAG_MAX_READ);

    if(r)
        return LAG_MAX_READ;
    else
        return 0;
}

/**
 * Check if langmuir is woirking properly.
 * @return 1-OK, 0-FAIL
 */
int langmuir_isAlive(void)
{
    int result = lag_read_plasma_packet(FALSE);

    if(!result)
        return 0;

    int sync_1 = lag_get_langmuir_buffer_i(0);
    int sync_2 = lag_get_langmuir_buffer_i(1);
    int sync_3 = lag_get_langmuir_buffer_i(2);

    if((sync_1==0x43) && (sync_2==0x43) && (sync_3==0x43))
        return 1;
    else
        return 0;
}

/*------------------------------------------------------------------------------
 *		 	U3 INTERRUPT - LAGMUIR
 *------------------------------------------------------------------------------
 * Description        : Reads chars from Lagmuir
 * Arguments          : None
 * Return Value       : None
 *----------------------------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U3RXInterrupt(void)
{
    unsigned int c = langmuir_read();
    langmuir_buffer[LAG_COUNT] = c;
    LAG_COUNT++;

    if(LAG_COUNT >= LAG_MAX_READ)
    {
        LAG_BUSY = 0;
        LAG_COUNT = 0;
    }

    U3RX_Clear_Intr_Status_Bit;
}
