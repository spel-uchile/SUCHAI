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
unsigned int* LAG_BUFFER; //Buffer de llegada (Se modifica en cada funcion)
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

/*------------------------------------------------------------------------------
 *		 	LAG READ CAL PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a calibration
 *                      packet (4packets x 10Bytes)
 * Arguments          : unsigned int* buffer[40] - Receive buffer
 *                      Lagmuir will send 40 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/

/**
 * Sends proper control packet to receive a calibration
 * @param buffer unsigned int* buffer[40] - Receive buffer
 * @return  1 - OK, 0 - Fail
 */
int lag_read_cal_packet(unsigned int* buffer)
{
    LAG_MAX_READ = 40;
    LAG_COUNT = 0;
    LAG_BUSY = 1;
    LAG_BUFFER = buffer;

    //Write CAL function
    lag_send_ctrl_packet(LAG_CAL);

    while(LAG_BUSY);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 	LAG READ PLASMA PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma 
 *                      packet (1packet x 10Bytes)
 * Arguments          : unsgined int* buffer[10] - Receive buffer
 *                      Lagmuir will send 40 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_plasma_packet(unsigned int* buffer)
{
    LAG_MAX_READ = 10;
    LAG_COUNT = 0;
    LAG_BUSY = 1;
    LAG_BUFFER = buffer;

    //Write PLASMA function
    lag_send_ctrl_packet(LAG_PLASMA);

    while(LAG_BUSY);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 	LAG READ SWEEP PACKET
 *------------------------------------------------------------------------------
 * Description        : Sends proper control packet to receive a plasma sweep
 *                      packet (1packet x 1096Bytes)
 * Arguments          : unsgined int* buffer[1096] - Receive buffer
 *                      Lagmuir will send 1096 bytes, details in
 *                      https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdHpNbVROMFg1cWtiNXVRa3hHb091Ync#gid=0
 * Return Value       : 1 - OK, 0 - Fail
 *----------------------------------------------------------------------------*/
int lag_read_sweep_packet(unsigned int* buffer)
{
    LAG_MAX_READ = 1096;
    LAG_COUNT = 0;
    LAG_BUSY = 1;
    LAG_BUFFER = buffer;

    //Write SWEEP function
    lag_send_ctrl_packet(LAG_SWEEP);

    while(LAG_BUSY);

    return 1;
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
    LAG_BUFFER[LAG_COUNT] = c;
    LAG_COUNT++;
    U3RX_Clear_Intr_Status_Bit;

    if(LAG_COUNT >= LAG_MAX_READ)
    {
        LAG_BUSY = 0;
        LAG_BUFFER = NULL;
        LAG_COUNT = 0;
        LAG_MAX_READ = 0;
    }
}
