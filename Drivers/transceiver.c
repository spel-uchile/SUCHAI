/**                          NANOCOM U482C DRIVER
 *                              For PIC24F
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

// WARNING DEPRECATED!  // WARNING DEPRECATED!  // WARNING DEPRECATED!
// WARNING DEPRECATED!  // WARNING DEPRECATED!  // WARNING DEPRECATED!

#include "transceiver.h"

#define TRX_STD_DELAY   (0x0002FFFF)
#define TRX_LONG_DELAY  (0x000FFFFF)

/* Local functions */
static void trx_std_delay();
static void trx_long_delay();
static void trx_enable_uart(void);
static void trx_disable_uart(void);

/* Local variables */
char* TRX_BUFFER = NULL; //Buffer de llegada (Se modifica en cada funcion)
INT16 TRX_COUNT = 0; //Contador de bytes recibidos
BOOL TRX_BUSY = 0; //Estado de la comunicacion
INT16 TRX_ERROR = 0; //Estado de error en el modulo

/*------------------------------------------------------------------------------
 *                          CONFIG COMM PORT
 *------------------------------------------------------------------------------
 * Function Prototype : void TRX_ConfigComPort()
 * Include            : i2c_comm.h
 * Description        : Confiugures I2C comunication at 100kHz
 * Arguments          : None
 * Return Value       : None
 * Remarks            :
 *----------------------------------------------------------------------------*/
void TRX_ConfigComPort()
{
    i2c1_open(37, SCH_I2C1_ADDR);; /* Config I2C @400kHz */
}

/*------------------------------------------------------------------------------
 *		 		WRITE REGISTER
 *------------------------------------------------------------------------------
 * Function Prototype : void WriteRegister(unsigned int reg, unsigned char val)
 * Include            : i2c_comm.h
 * Description        : Write some transceiver's register by I2C interface
 * Arguments          : reg - register to be write
			val - value to be write
 * Return Value       : None
 * Remarks            : Use defines in header to find the register value
 *----------------------------------------------------------------------------*/
void TRX_WriteRegister(unsigned int reg, unsigned char val)
{
    //DEPPRECATED!
}

/*------------------------------------------------------------------------------
 *                                  READ REGISTER
 *------------------------------------------------------------------------------
 * Function Prototype : void ReadRegister(unsigned int reg, unsigned char val)
 * Include            : i2c_comm.h
 * Description        : Write some transceiver's register by I2C interface
 * Arguments          : reg -register to be read
 * Return Value       : None
 * Remarks            : Use defines in header to find the register value
 *----------------------------------------------------------------------------*/
unsigned char TRX_ReadRegister(unsigned int reg)
{
    //DEPPRECATED!
    return 0;
}

/*------------------------------------------------------------------------------
 *		 			SET MODE
 *------------------------------------------------------------------------------
 * Function Prototype : void SetMode(FlagMode ModeCode)
 * Include            : i2c_comm.h
 * Description        : Confiugures Transceiver operation mode as
 *                      RESET(0), SYSRESET(1), SILENT(2), ONLYBEACON(3)
 *                      NOBEACON(4), NOMINAL(5)
 * Arguments          : ModeCode - enum defined in header.
 * Return Value       : None
 * Remarks            : This function need the I2C1 module configured and oprative in
 *                      order to entablish communication with transceiver.
 *----------------------------------------------------------------------------*/
void TRX_SetMode(int ModeCode)
{
    unsigned char Mode;
    /* Realiza ModeCode corrimientos para setear el bit
     * Correpondiente al modo deseado.*/
    Mode = 0x01;
    Mode = Mode<<ModeCode;
    if(Mode>0b100000) /* Max valid mode */
        return;

    TRX_WriteRegister(TRX_MODE, Mode);
}

/*------------------------------------------------------------------------------
 *                            BEACON ACTION
 *------------------------------------------------------------------------------
 * Function Prototype : void TRX_BeaconAction(int on_off)
 * Include            : i2c_comm.h
 * Description        : Start or stop beacon transmition
 * Arguments          : int on_off: (0) OFF - (1) ON
 * Return Value       : None
 *----------------------------------------------------------------------------*/
void TRX_BeaconAction(int on_off)
{
    unsigned char code;

    if(on_off) //on_off > 0 means ON beacon
        code = 0x01;
    else
        code = 0x02;

    TRX_WriteRegister(TRX_ACTION_BEACON, code);
}

/*------------------------------------------------------------------------------
 *                                      SET MESURE
 *------------------------------------------------------------------------------
 * Function Prototype : void SetMesurements(Mesure MesCode)
 * Include            : i2c_comm.h
 * Description        : Confiugures Transceiver Mesurements. TEMP-> Only temp,
 *                      RSSI-> Only rssi or BOTH -> Both temp and rssi
 * Arguments          : MesCode - enum defined in header.
                            TRX_MES_TEMP
                            TRX_MES_RSSI
                            TRX_MES_BOTH
 * Return Value       :  None
 *----------------------------------------------------------------------------*/
void TRX_SetMesurements(int MesCode)
{
    unsigned char code = (unsigned char)MesCode;
    TRX_WriteRegister(TRX_MEASURE, code);
}

/*------------------------------------------------------------------------------
 *                                  SET HK PERIOD
 *------------------------------------------------------------------------------
 * Function Prototype : void TRX_SetHKPeriod(int period_s)
 * Include            : i2c_comm.h
 * Description        : Confiugures houskeeping measurements period, in seconds
 * Arguments          : int period_s - Seconds
 * Return Value       :  None
 *----------------------------------------------------------------------------*/
void TRX_SetHKPeriod(int period_s)
{
    TRX_WriteRegister(TRX_HKPERIOD, period_s*10);
}

/*------------------------------------------------------------------------------
 *                              TRANSCEIVER SETTINGS
 *------------------------------------------------------------------------------
 * Function Prototype : TransceiverSettings(TxRxSetting *Setting)
 * Include            : i2c_comm.h
 * Description        : Confiugures Transceiver Frecuency Settings.
 * Arguments          : *Setting - Struct defined in header.
 * Return Value       : None
 *----------------------------------------------------------------------------*/
void TRX_SetTransceiverSettings(TxRxSettings *Setting)
{
    TRX_WriteRegister(TRX_MODE, Setting->_MODE_TRX);            trx_std_delay();
    TRX_WriteRegister(TRX_BEACONPOWER, Setting->_BCN_PWR);      trx_std_delay();
    TRX_WriteRegister(TRX_BEACONPERIOD, Setting->_BCN_TMR);     trx_std_delay();
    TRX_WriteRegister(TRX_BEACONWPM, Setting->_BCN_WPM);        trx_std_delay();

    TRX_WriteRegister(TRX_HKPERIOD, (Setting->_HKP_PER)*10);    trx_std_delay();

    TRX_WriteRegister(TRX_TMPOWER, Setting->_TM_PWR);           trx_std_delay();
    TRX_WriteRegister(TRX_TMPREAMBLELENGTH, Setting->_TM_PRE);  trx_std_delay();
    TRX_WriteRegister(TRX_TMFRAMECOUNTMAX, Setting->_TM_CMX);   trx_std_delay();

//    TRX_WriteRegister(TRX_FRX_H, Setting->_FRX_H);              trx_std_delay();
//    TRX_WriteRegister(TRX_FRX_M, Setting->_FRX_M);              trx_std_delay();
//    TRX_WriteRegister(TRX_FRX_L, Setting->_FRX_L);              trx_std_delay();

//    TRX_WriteRegister(TRX_FTX_H, Setting->_FTX_H);              trx_std_delay();
//    TRX_WriteRegister(TRX_FTX_M, Setting->_FTX_M);              trx_std_delay();
//    TRX_WriteRegister(TRX_FTX_L, Setting->_FTX_L);              trx_std_delay();

//    TRX_WriteRegister(TRX_FSEP_H, Setting->_FSEP_H);            trx_std_delay();
//    TRX_WriteRegister(TRX_FSEP_L, Setting->_FSEP_L);            trx_std_delay();

//    TRX_WriteRegister(TRX_BITSTUFFING, Setting->_BSTUF);        trx_std_delay();

    /* Update freq command */
    TRX_WriteRegister(TRX_UPDATE, 0x01);
}

/*------------------------------------------------------------------------------
 *                                      BEACON SETTINGS
 *------------------------------------------------------------------------------
 * Function Prototype : BeaconSettings(BeaconSett *Setting)
 * Include            : i2c_comm.h
 * Description        : Confiugures Transceiver Beacon Settings.
 * Arguments          : *Setting - Struct defined in header.
 * Return Value       :  None
 * Remarks            :  This function need the I2C1 module configured and oprative in
 			order to entablish communication with transceiver.
 *----------------------------------------------------------------------------*/
void TRX_BeaconSettings(BeaconSett *Setting)
{
    /* TODO: Check for invalid values */
    TRX_WriteRegister(TRX_BEACONPOWER, Setting->Power);
    TRX_WriteRegister(TRX_BEACONWPM, Setting->WPM);
    TRX_WriteRegister(TRX_BEACONPERIOD, (Setting->Timer)/4);
    TRX_SetBeaconContent(Setting->Content, Setting->Len);
}

/*------------------------------------------------------------------------------
 *                                  GET STATUS
 *------------------------------------------------------------------------------
 * Function Prototype : void GetStatus(unsigned char *buffer)
 * Include            : i2c_comm.h
 * Description        : Read and save all transceiver's registers
 * Arguments          : buffer - Addres of the buffer with 0x36+1 char of size
 * Return Value       :  None
 * Remarks            : Asumes thah Temp MCU, Temp HPA and RSSI values was
 *                      updated by TRX houskeeping function.
 *----------------------------------------------------------------------------*/
void TRX_GetStatus(unsigned char *buffer)
{
    //DEPRECATED
}

/*------------------------------------------------------------------------------
 *                              SET BEACON CONTENT
 *------------------------------------------------------------------------------
 * Function Prototype : void SetBeaconContent(unsigned char *content, unsigned int len)
 * Include            : i2c_comm.h
 * Description        : Set the beacon content in the TxRx memory
 * Arguments          : content - Desired string to save as beacon
 *					  : lenght - Lenght of the beacon content
 * Return Value       : None
 * Remarks            : The beacon content is any standard ASCII code for the
 *                      letters A-Z, numbers 0-9 and the minus (-)
 *----------------------------------------------------------------------------*/
 void TRX_SetBeaconContent(unsigned char *content, unsigned int len)
 {
    if (len > 255) return; /* Len exceeds the buffer lenght */
#if SCH_TRASNCEIVER_VERBOSE
    con_printf(">> TRX Setting BEACON:\n");
    con_printf("      Setting Offset...\n");
#endif
    TRX_WriteRegister(TRX_BEACONOFFSET, 0);
    trx_std_delay();

#if SCH_TRASNCEIVER_VERBOSE
    con_printf("      Setting Content... ");
#endif
    unsigned int i;
    
    for(i = 0; i < len; i++)
    {
        /* Transmission is not completed until BeaconBuffer is transfered and
         * BeaconOffset register is increased. So, a delay between transmissions
         * is needed. (TODO: Look for a better solution) */

        TRX_WriteRegister(TRX_BEACONBUFFER, content[i]);
        trx_std_delay();
//        int b = TRX_ReadRegister(TRX_BEACONOFFSET);
    }
#if SCH_TRASNCEIVER_VERBOSE
    char tmp[10]; itoa(tmp, len, 10);
    con_printf("\n      Setting Beacon Length... ");
    con_printf(tmp);
    con_printf("\n");
#endif
    TRX_WriteRegister(TRX_BEACONLENGTH, len);
 }

 /*===========================================================================
  *             TRANSCEIVER TELECOMAND - TELEMETRY OPERATIONS INTERFACE
  *===========================================================================*/

/*------------------------------------------------------------------------------
 *                                  SEND IDLE FRAME
 *------------------------------------------------------------------------------
 * Function Prototype : void TRX_SendIdleFrame(void)
 * Include            : i2c_comm.h
 * Description        : Send an idle frame as telemetry (128 0x55). This action
 *                      is useful for a quick test of space link
 * Arguments          : None
 * Return Value       : None
 *----------------------------------------------------------------------------*/
void TRX_SendIdleFrame(void)
{
    /* TODO: CHECK */
    TRX_WriteRegister(TRX_ACTION_TM, 0x10);

    /* TRXV3 - TRX_DEBUGATIONS do not exist */
//    TRX_WriteRegister(TRX_DEBUGACTION, 0x02);
}

/*------------------------------------------------------------------------------
 *                              LOAD TELEMETRY
 *------------------------------------------------------------------------------
 * Function Prototype : TRX_LoadTelemetry(unsigned char *telemetry, unsigned int len)
 * Include            : i2c_comm.h, rs232_suchai.h
 * Description        : Stores new telemetry data in the TRX buffer.
 * Arguments          : telemetry - pointer to the buffer with the data.
 *                      len - number of bytes to be sent.
 * Return Value       : 1 - telemetry loaded successfully
 *                      0 - telemetry dada was not loaded
 * Remarks            : This function doesn't send telemetry.
 *----------------------------------------------------------------------------*/
int TRX_LoadTelemetry(unsigned char *telemetry, unsigned int len)
{
    unsigned int buff1, buff2;
    unsigned char buf1L, buf1H, buf2L, buf2H;
    unsigned int ok = 0;

    int trx_type = 0;
    #if (SCH_TRX_TYPE_ONBOARD==1)
        trx_type=1;
    #elif (SCH_TRX_TYPE_ONBOARD==2)
        trx_type=2;
    #endif

    switch(trx_type){
        case 0:
            con_printf("[Error]: No hay TRX configurado..\r\n ");
            return 0;
        break;
        case 1:
            #if SCH_TRASNCEIVER_VERBOSE
                con_printf(">> Sending START_WRITE_TM command... ");
                con_printf("\n>>> CONSOLE DISABLED <<<\n");
            #endif

            /* Start loading data setting STARTWRITETM bit
             * in ACTION_TM register */
            TRX_WriteRegister(TRX_ACTION_TM, 0x04);

            /* There is a Busy Wating - A delay is necesary before
             * asking updated value of another regiter
             * TODO: Look for a better solution */
            trx_long_delay();

            /* Checking if command was successfully read. Wait
             * setting STARTWRITETM bit in ACTION_TM register */
            ok = TRX_ReadRegister(TRX_EVENT_TM);
            ok = (ok == 0x02) ? 1:0;

            #if SCH_TRASNCEIVER_VERBOSE > 2
                if(ok) con_printf("[OK]\n");
                else con_printf("[FAIL]\n");
            #endif

            /* Checking inital state of IN pointer*/
            buf1L = TRX_ReadRegister(TRX_TMTF_IN_L);
            buf1H = TRX_ReadRegister(TRX_TMTF_IN_H);
            buff1 = buf1H; buff1 = (buff1 << 8) | buf1L;

            #if SCH_TRASNCEIVER_VERBOSE > 2
                con_printf(">> Loading telemetry in TRX... ");
            #endif

            /* Send data only if the command was read ok */
            if(ok)
            {
                //UART1 - CONSOLA SERIAL - 9600, 8, N, 1 */
                DisableIntU1RX;
                CloseUART1();
                ConfigRS232(103, RS2_M_UART1);
                EnableIntU1RX;
                SetPriorityIntU1RX(5);

                trx_enable_uart(); /* Redirigir uart a TRX */
                SendRS232(telemetry,len,RS2_M_UART1); /* Transmito por UART1 */
                trx_disable_uart(); /* Redirigir uart a consola */

                //UART1 - CONSOLA SERIAL - 19200, 8, N, 1 */
                DisableIntU1RX;
                CloseUART1();
                ConfigRS232(51, RS2_M_UART1);
                EnableIntU1RX;
                SetPriorityIntU1RX(5);
            }

            /* Stop loading data setting STOPWRITETM
             *  bit in ACTION_TM register */
            TRX_WriteRegister(TRX_ACTION_TM, 0x08);

            /* There is a Busy Wating - A delay is necesary before
             * asking updated value of another regiter
             * TODO: Look for a better solution */
            trx_long_delay();

            /* Check if new data was loaded */
            buf2L = TRX_ReadRegister(TRX_TMTF_IN_L);
            buf2H = TRX_ReadRegister(TRX_TMTF_IN_H);
            buff2 = buf2H; buff2 = (buff2 << 8) | buf2L;

            ok = (buff2 > buff1);
            
            #if (SCH_TRASNCEIVER_VERBOSE>=1)
                con_printf(">>> CONSOLE ENABLED <<<\n");
                if(ok){
                    con_printf("[OK]\n");
                }
                else{
                    con_printf("[FAIL]\n");
                }
            #endif

            return ok;
        break;
        case 2:
            ok=1;
            #if (SCH_TRASNCEIVER_VERBOSE >= 1)
                con_printf(">> Loading telemetry in TRX... ");
            #endif

            //UART1 - CONSOLA SERIAL - 1200, 8, N, 1 */
            DisableIntU1RX;
            CloseUART1();
            ConfigRS232(832, RS2_M_UART1);
            EnableIntU1RX;
            SetPriorityIntU1RX(5);

            trx_enable_uart(); /* Redirigir uart a TRX */
            unsigned char sep[] = {0xAF,0xFA};
            SendRS232(sep, 2, RS2_M_UART1); /* Transmito por UART1 */
            SendRS232(telemetry, len, RS2_M_UART1); /* Transmito por UART1 */
            trx_disable_uart(); /* Redirigir uart a consola */

            //UART1 - CONSOLA SERIAL - 19200, 8, N, 1 */
            DisableIntU1RX;
            CloseUART1();
            ConfigRS232(51, RS2_M_UART1);
            EnableIntU1RX;
            SetPriorityIntU1RX(5);

            #if (SCH_TRASNCEIVER_VERBOSE>=1)
                con_printf(">>> CONSOLE ENABLED <<<\n");
                if(ok){
                    con_printf("[OK]\n");
                }
                else{
                    con_printf("[FAIL]\n");
                }
            #endif

            return ok;
        break;
    }
    return 0;
}

/*------------------------------------------------------------------------------
 *                                  SEND TELEMETRY
 *------------------------------------------------------------------------------
 * Function Prototype : void TRX_SendTelemetry(void)
 * Include            : i2c_comm.h
 * Description        : Sends the telemetry stored
 * Arguments          : None
 * Return Value       : 1 - start sending telemetry successfully
 *                      0 - telemetry sending doesn't started
 * Remarks            : This function send all the unsent telemetry data stored
 *                      in the TRX.
 *----------------------------------------------------------------------------*/
int TRX_SendTelemetry(void)
{
    unsigned char ok;

    int trx_type=0;
    #if (SCH_TRX_TYPE_ONBOARD==1)
        trx_type=1;
    #elif (SCH_TRX_TYPE_ONBOARD==2)
        trx_type=2;
    #endif

    switch(trx_type){
        case 0:
            con_printf("[Error]: No hay TRX configurado..\r\n ");
            return 0;
        break;
        case 1:
            #if SCH_TRASNCEIVER_VERBOSE
                con_printf(">>Sending START_SEND_TM command...\n");
            #endif

            /* Start sending TM setting
             * STARTSENDTM bit in ACTION_TM register */
            TRX_WriteRegister(TRX_ACTION_TM, 0x01);

            /* There is a Busy Wating - A delay is necesary before
             * asking updated value of another regiter
             * TODO: Look for a better solution */
            trx_long_delay();

            /* Cheking if TM is sending */
            ok = TRX_ReadRegister(TRX_EVENT_TM);
            ok = (ok == 0x01) ? 1:0;

            #if (SCH_TRASNCEIVER_VERBOSE>=1)
                con_printf(">> Telemetry transmited ");
                if(ok){
                    con_printf("[OK]\n");
                }
                else{
                    con_printf("[FAIL]\n");
                }
            #endif

            return ok;
        break;
        case 2:
            ok=1;
            #if (SCH_TRASNCEIVER_VERBOSE>=1)
                con_printf(">> Telemetry transmited ");
            if(ok){
                con_printf("[OK]\n");
            }
            else{
                con_printf("[FAIL]\n");
            }
            #endif

            return ok;
        break;
    }
    return 0;
}

/*------------------------------------------------------------------------------
 *                          TRX CHECK NEW TC
 *------------------------------------------------------------------------------
 * Function Prototype : int TRX_ReadTelecomadFrame(char *buffer)
 * Include            : i2c_comm.h
 * Description        : Check how many tc are availible
 * Arguments          : None
 * Return Value       : Number of unreaded TC. 0 if no new TC exist
 *----------------------------------------------------------------------------*/
int TRX_CheckNewTC(void)
{
    /* Get position of oldest unread TC */
    unsigned char outl = TRX_ReadRegister(TRX_TCTF_OUT_L);
    unsigned char outh = TRX_ReadRegister(TRX_TCTF_OUT_H);

    /* Get position of next new incoming TC*/
    unsigned char inl = TRX_ReadRegister(TRX_TCTF_IN_L);
    unsigned char inh = TRX_ReadRegister(TRX_TCTF_IN_H);

    unsigned int p_out  = (outh<<8)|outl;
    unsigned int p_in = (inh<<8)|inl;

    int how_many = p_in - p_out;

#if SCH_TRASNCEIVER_VERBOSE >= 2
    char tmp[10];
    itoa(tmp, how_many, 10);
    con_printf(">>Availible TC: "); con_printf(tmp); con_printf("\n\r");
#endif

    /*
     * If new incoming is greater than last unread pointer
     * means that some unprocesed TC exists
     */
    if(how_many < 0)
        how_many = 0;

    return how_many;
}

/*------------------------------------------------------------------------------
 *                          READ TELECOMAD FRAME
 *------------------------------------------------------------------------------
 * Function Prototype : int TRX_ReadTelecomadFrame(char *buffer)
 * Include            : i2c_comm.h
 * Description        : Orders a telecomand reading from TRX buffer and save
 *                      the received frame in a buffer
 * Arguments          : char* buffer - Pointer to an initialized buffer of len
 *                      TRX_TCFRAMELEN
 * Return Value       : 1 - Telecomands read successfully
 *                      0 - Telecomand reading failed
 * Remarks            : This function orders the TRX to start sending a
 *                      telecomd but the data is being recived by serial
 *                      interface. So, prior to call this function the user
 *                      should configure a buffer to receive the data
 *----------------------------------------------------------------------------*/
int TRX_ReadTelecomadFrame(char *buffer)
{
    /*
     * Primero revisar si hay TC por leer
     * Salir con codigo 0, si no hay TC por leer
     */
    if(TRX_CheckNewTC()<= 0)
        return 0;
    
#if SCH_TRASNCEIVER_VERBOSE
    con_printf(">>Sending READ_TC command... ");
    con_printf("\n>>> CONSOLE DISABLED <<<\n");
#endif

    /* Apaga la consola y activa el serial para TRX*/
    trx_enable_uart();

    /* Inicializar las variables de control */
    TRX_COUNT = 0;
    TRX_ERROR = 0;
    TRX_BUSY = 1;
    TRX_BUFFER = buffer;

    WriteTimer2(0);
    /* Si inicia la transmision seteando el bit READTCTF en ACTION_TC  */
    TRX_WriteRegister(TRX_ACTION_TC, 0x01);

//#if SCH_TRASNCEIVER_VERBOSE > 0
//    trx_long_delay();
//    int ok = TRX_ReadRegister(TRX_EVENT_TC);
//    if(ok)con_printf("[OK]\n");
//    else con_printf("[FAIL]\n");
//#endif


    /* Activa la consola y apaga serial para TRX*/
    trx_disable_uart();

    /* Se espera a recibir todos los bytes */
    while(TRX_BUSY);

#if SCH_TRASNCEIVER_VERBOSE
    con_printf(">>> CONSOLE ENABLED <<<\n");
    if(TRX_ERROR) con_printf(">>ERROR\n");
    con_printf(">>TC frame received:");
    SendRS232((unsigned char *)buffer, TRX_TCFRAMELEN, RS2_M_UART1);
    con_printf("\n");
#endif

    return (TRX_COUNT==0) && (!TRX_ERROR);
}

/*------------------------------------------------------------------------------
 *		 		STATIC TRX STD DELAY
 *------------------------------------------------------------------------------
 * Function Prototype : void trx_delay(void)
 * Include            : Local routine
 * Description        : Busy waiting (INT)
 * ---------------------------------------------------------------------------*/
static void trx_std_delay(void)
{
    unsigned long i;
    for(i = TRX_STD_DELAY; i > 0; i--);
}

/*------------------------------------------------------------------------------
 *		 		STATIC TRX LONG DELAY
 *------------------------------------------------------------------------------
 * Function Prototype : void trx_delay(void)
 * Include            : Local routine
 * Description        : Busy waiting (LONG)
 * ---------------------------------------------------------------------------*/
static void trx_long_delay(void)
{
    unsigned long i;
    for(i = TRX_LONG_DELAY; i > 0; i--);
}

/**
 * Redirect uart signal to TRX. Disables serial console
 */
static void trx_enable_uart(void)
{
    /* Redirigir puerto serial de consola para conectarse con TRX*/
    while(BusyUART1());

    _LATC1 = 1; 		/* -OE_USB OFF */
    _LATE2 = 0; 		/* -OE_MHX ON */

    con_set_active(FALSE);
}

/**
 * Redirect uart signal to serial console. Enbables serial console.
 */
static void trx_disable_uart(void)
{
    while(BusyUART1());

    /* Redirigir puerto serial de consola para conectarse con TRX*/
    _LATE2 = 1; 		/* -OE_MHX OFF */
    _LATC1 = 0; 		/* -OE_USB ON */

    con_set_active(TRUE);
}

/*------------------------------------------------------------------------------
 *                              UART2 INTERRUPT
 *------------------------------------------------------------------------------
 * Description        : Each character is recived by UART2 Interrupt. A total of
 *                      TRX_TMFRAMELEN chars are saved in TRX_BUFFER
 * Arguments          : None
 * Return Value       : None
 *----------------------------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
    unsigned int c = ReadUART2();
    U2RX_Clear_Intr_Status_Bit;

    WriteTimer2(0);

    if(TRX_BUFFER == NULL)
    {
        /*TODO: TEST THIS*/
        return;
    }

    if(TRX_COUNT < TRX_TCFRAMELEN)
    {
        *TRX_BUFFER = (char)c;
        TRX_BUFFER++;
        TRX_COUNT++;
    }

    if(TRX_COUNT >= TRX_TCFRAMELEN)
    {
        TRX_BUSY = 0;
        TRX_BUFFER = NULL;
        TRX_COUNT = 0;
    }
}

/**
 * TIMER 2 INTERRUPT FOR UART2
 * Funciona como watchdog para la comunicaci�n as�ncrona serial
 */
void __attribute__((__interrupt__, auto_psv)) _T2Interrupt( void )
{
    /* Clear the timer interrupt. */
    WriteTimer2(0);
    TRX_BUSY = 0;
    TRX_ERROR = 1;
    TRX_BUFFER = NULL;
    T2_Clear_Intr_Status_Bit;
}