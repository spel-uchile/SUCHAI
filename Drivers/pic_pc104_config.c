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

#include "pic_pc104_config.h"


/**
 * DEFAULT PIC CONFIG
 * Configuiraciones por default para la computador a bordo del SUCHAI. Controla
 * todas las cinfiguraciones de pines, perifericos y servicios del uC.
 */
void default_PIC_config(void)
{
    GPIO_PPM_init();
    GPIO_MB_init();
    GPIO_PC104_init();

    //solo despues de llamar estas funciones se pueden usar los servicios
    //de printf()
    #if SCH_PIC_PC104_CONFIG_VERBOSE
        printf("\n--------- SUCHAI BOOT SEQUENCE ---------\r\n");
        printf("\nInitializing sattelite bus\r\n");
        printf("    * Microcontroller IO pins\r\n");
        printf("    * CubesatKit MB\r\n");
        printf("    * PC104 Bus\r\n");
    #endif

    #if SCH_PIC_PC104_CONFIG_VERBOSE
        printf("\nInitilizing Microcontroller\r\n");
        printf("    * Reset status\r\n");
    #endif
	init_Check_Reset_Status();

    #if SCH_PIC_PC104_CONFIG_VERBOSE
        printf("    * Setting oscillator\r\n");
    #endif
	PwrMgnt_ClkDiv_Sel(INTR_NO_EFFECT_DOZEN& PERI_CLK_RATIO_1& PERI_CLK_RATIO_ENABLE& FRC_POSTSCALER_SEL_256);
	REFO_enabled(0);
	PPC_REFO_ACTIVE_SLEEP;
	PPC_PRIMARY_OSC_ACTIVE_SLEEP;
	PPC_ENABLE_SOSC();  //Habilita el SOSC
	
    #if SCH_PIC_PC104_CONFIG_VERBOSE
        printf("    * mPWRMGNT_Regulator_ActiveInSleep\r\n");
    #endif
	mPWRMGNT_Regulator_ActiveInSleep();
	
    #if SCH_PIC_PC104_CONFIG_VERBOSE
        printf("    * Enabling WDT\r\n");
    #endif
    EnableWDT( PPC_INITIAL_WDT_STATE );
}


/**
 * Configuracion iniciar para eventos de reset
 */
void init_Check_Reset_Status(void)
{
	mPWRMGNT_Clear_PORbit();
	mPWRMGNT_Clear_BORbit();
}

/**
 *
 */
void Check_Reset_Status(void)
{
    switch (PwrMgnt_ResetSource() )
    {
        case POWER_ON_Reset:
                POWER_ON_Reset_Routine();
        break;
        case BURN_OUT_Reset:
                BURN_OUT_Reset_Routine();
        break;
        case WATCHDOG_Reset:
                WATCHDOG_Reset_Routine();
        break;
        case SOFTWARE_Reset:
                SOFTWARE_Reset_Routine();
        break;
        case EXTERNAL_Reset:
                EXTERNAL_Reset_Routine();
        break;
        case CFG_WORD_MISMATCH_Reset:
                CFG_WORD_MISMATCH_Reset_Routine();
        break;
        case ILLEGAL_INSTR_Reset:
                ILLEGAL_INSTR_Reset_Routine();
        break;
        case TRAP_Reset:
                TRAP_Reset_Routine();
        break;
        default:
        break;
    }//fin switch
}

/**
 *
 */
void POWER_ON_Reset_Routine(void)
{
    //Si NO!! ha ocurrido Reset se llama  esta Rutina
    printf("A POWER_ON_Reset event occurred\r\n");
    mPWRMGNT_Clear_PORbit();
}

/**
 *
 */
void BURN_OUT_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    /* NOTA: Por alguna extra?a razon, si no ha ocurrido algun RESET PwrMgnt_ResetSource()
     * devuelve 0x0001=BURN_OUT_Reset, pero salta al default !!.  Asi es que para no
     * confundirse aquel caso de RESET ser? ignorado y asi la lastResetSource
     * no se actualizar? equivocadamente. He dicho! caso cerrado!
     */
    printf("A BURN_OUT_Reset event occurred\r\n");
    mPWRMGNT_Clear_BORbit();
}

/**
 *
 */
void WATCHDOG_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("A WATCHDOG_Reset event occurred\r\n");
    mPWRMGNT_Clear_WDTObit();
}

/**
 *
 */
void SOFTWARE_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("A SOFTWARE_Reset event occurred\r\n");
    mPWRMGNT_Clear_SWRbit();
}

/**
 *
 */
void EXTERNAL_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("An EXTERNAL_Reset event occurred\r\n");
    mPWRMGNT_Clear_EXTRbit();
}

/**
 *
 */
void CFG_WORD_MISMATCH_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("A CFG_WORD_MISMATCH_Reset event occurred\r\n");
    mPWRMGNT_Clear_CMbit();
}

/**
 * Rutina para evento ILLEGAL_INSTR
 */
void ILLEGAL_INSTR_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("An ILLEGAL_INSTR_Reset event occurred\r\n");
    mPWRMGNT_Clear_IOPUWRbit();
}

/**
 * Rutina para evento TRAP_RESET
 */
void TRAP_Reset_Routine(void)
{
    //rutina a ejecutar luego de este reset
    printf("A TRAP_Reset event occurred\r\n");
    mPWRMGNT_Clear_TRAPRbit();
}

/**
 * Revisa estado de Power Managment y realiza acciones que correspondan
 */
void Check_Wakeup_Status(void)
{
    switch (PwrMgnt_WakeupSource())
    {
        case WAKEUP_SLEEP:
            WAKEUP_SLEEP_Routine();
            break;
        case WAKEUP_IDLE:
            WAKEUP_IDLE_Routine();
            break;
        default:
            break;
    }
}

/**
 * Rutina que se ejecuta ante evento WAKEUP_SLEEP
 */
void WAKEUP_SLEEP_Routine(void)
{
    //rutina a ejecutar luego de este wakeup
    printf("A WAKEUP_SLEEP event occurred\r\n");
}

/**
 * Rutina que se ejecuta ante evento WAKEUP_IDLE
 */
void WAKEUP_IDLE_Routine(void)
{
    //rutina a ejecutar luego de este wakeup
    printf("A WAKEUP_IDLE event occurred\r\n");
}

/**
 * Configuracion del oscilar, aparte de config bits 1 y 2
 * @return Configuracion actual del oscilador
 */
unsigned int Current_OSCILLATOR(void)
{
    return (OSCCON>>12);
}

/**
 * Revisa si ocurre error FSCM, y ejecuta rutina de correcion si es necesario
 */
void Check_FSCM(void)
{
    if(mPWRMGNT_GetClkFailDetectBit() ){FSCM_Routine();}
}


void sw_reset(int arg){
    if(arg){
        printf("Resetting system NOW!! ...\r\n");
    }
    PPC_SWReset;
}

int set_new_osc(int arg){
    char *txt_result;
    int result = 0;

    switch( arg ){
        case 0:
            result = PwrMgnt_OscSel(FRC_OSC);
            txt_result = "new Oscillator is FRC_OSC\r\n";
            break;
        case 1:
            result = PwrMgnt_OscSel(FRC_OSC_WITH_POSTSCALER_PLL);
            txt_result = "FRC_OSC_WITH_POSTSCALER_PLL\r\n";
            break;
        case 2:
            result = (PwrMgnt_OscSel(PRIMARY_OSC) );
            txt_result = "PRIMARY_OSC\r\n";
            break;
        case 3:
            result = (PwrMgnt_OscSel(PRIMARY_OSC_WITH_PLL) );
            txt_result = "PRIMARY_OSC_WITH_PLL\r\n";
            break;
        case 4:
            result = (PwrMgnt_OscSel(SECONDRY_OSC) );
            txt_result = "SECONDRY_OSC\r\n";
            break;
        case 5:
            result = (PwrMgnt_OscSel(LOW_POWER_RC) );
            txt_result = "LOW_POWER_RC\r\n";
            break;
        case 7:
            result = (PwrMgnt_OscSel(FRC_OSC_WITH_POSTSCALER) );
            txt_result = "FRC_OSC_WITH_POSTSCALER\r\n";
            break;
    }
    #if(SCH_PIC_PC104_CONFIG_VERBOSE>=1)
        if(result){
            printf(txt_result);
        }
        else{
            printf("Failed to set new oscilator\r\n");
        }
    #endif

    return result;
}

/**
 * Rutina para manejo de error tipo FSCM
 */
void FSCM_Routine(void)
{
    //rutina a ejecutar luego de este fallo
    printf("*******************************************************\r\n");
    printf("Ocurrio un error con el CLK configurado en pic_pc104\r\n");
    printf("Actualmente el PIC esta en modo FSCM, osea clk=FRC\r\n");
    printf("Se tratara de cambiar a clk=FRC w PLL\r\n");
    printf("*******************************************************\r\n");
    
    int arg=FRC_OSC_WITH_POSTSCALER_PLL;
    //ppc_newosc((void *)&arg);
    set_new_osc(arg);

    printf("***************************************************************\r\n");
    printf("Si ves esto, es porque ahora clk=FRC w PLL\r\n");
    printf("FSCM se acivo por una falla en el clk configurado en pic_pc104 (probablemente PRIM OSC w PLL)\r\n");
    printf("Ahora clk=FRC w PLL y se proceder? AHORA a Resetear el PIC\r\n");
    printf("***************************************************************\r\n");

    mPWRMGNT_GetClkFailDetectBit()=0;
    sw_reset(1);
}

/**
 * Reference Clock Output Control Register Configuration
 * @param arg REFO active? 0 false, 1 true
 */
void REFO_enabled(int arg)
{
    if(arg)
    {
        _ROON=1;				//REFO activado
        _ROSEL=0;				//system clock is on REFO
        _RODIV3=0; _RODIV2=0; _RODIV1=0; _RODIV0=0;
    }
}

/* WATCHDOG CONFIGURATION */
//.... ninguna funcion para WDT aun

/* CONFIGURATION WORDS */
//.... ninguna funcion para CW aun

/* PERIPHERAL PIN SELECT */
/** PPS (Peripheral Pin Select)
 *     Algunos perifericos no tienen asociados pines fijos, sino que deben asignarse
 *     (UARTx, SPIx, INTx, ICx etc...)
 *     En el PIC24FJ256GA110 se tienen RP0 al RP31 y luego PRI32 al RPI45, donde:
 *        RP= remappable pin for input and output functions
 *        RPI= remappable pin for input functions only
 *
 *  ASIGNACION:
 *      Para Input: iPPSInput(fn,pin) fn=pin Assign given pin as input pin
 *      Para Output: iPPSOutput(pin,fn) pin=fn Assign given pin as output pin
*/

/** Pines del sistema:
 *      Vdd, Vss, nMCLR, AVdd, AVss, ENVREG, Vcap/Vdecore, OSCI, OSCO, SOSCI, SOSCO (19 pines)
 *      hay 2 osciladores conectados a estos 4 pines con PPC_ENABLE_SOSC() en default_PIC_config()
 *          //63	OSCI/CLKI/CN23/RC12							8Mhz IN
 *          //64	OSCO/CLKO/CN22/RC15							8Mhz OUT
 *          //73	SOSCI/C3IND/CN1/RC13						32.768Khz IN
 *          //74	SOSCO/C3INC/RPI37/T1CK/CN0/RC14				32.768Khz OUT
 */
	
/** PINES PMM
*    100 pines:
*        -19 pines del sistema (Vcc, Vss, AVcc AVss, OSCI/O, ENVREG etc..)
*        -10 no conectados
*        -4 JTAG
*        -5 Mem Flash
*        ---------------------
*        = 62 pines pasan al MB
*
*        -4 control hardware MB (PPC_MB_nOE_USB_nINT, PPC_MB_nOE_MHX, PPC_MB_nON_MHX, PPC_MB_nON_SD)
 *       -1 pin DCD_MHX [Active Low] HS[5..0] que llega a la MB pero no sale al PC-104
*       ---------------------------
*        = 57 pines pasan al PC104
*/

/**
 * Configurar los pines de IO para PPM
 */
void GPIO_PPM_init(void)
{
    // PINES NO CONECTADOS, NO IMPLEMENTADOS
    //1		CN82/RG15	=> pin es RA5
    _TRISG15=0;			//pin 0-output 1-input
    _LATG15=0;			// parte en 0

    //84	C3INA/CN16/RD7	=> pin es RA5
    _TRISD7=0;			//pin 0-output 1-input
    _LATD7=0;			// parte en 0

    //91	CN39/RA6		=> pin es RA5
    _TRISA6=0;			//pin 0-output 1-input
    _LATA6=0;			// parte en 0

    //92	CN40/RA7		=> pin es RA5
    _TRISA7=0;			//pin 0-output 1-input
    _LATA7=0;			// parte en 0

    //93	CN58/PMD0/RE0	=> pin es RE0
    _TRISE0=0;			//pin 0-output 1-input
    _LATE0=0;			// parte en 0

    //94	CN59/PMD1/RE1	=> pin es RE1
    _TRISE1=0;			//pin 0-output 1-input
    _LATE1=0;			// parte en 0

    //95	CN81/RG14		=> pin es RG14
    _TRISG14=0;			//pin 0-output 1-input
    _LATG14=0;			// parte en 0

    //96	CN79/RG12		=> pin es RG12
    _TRISG12=0;			//pin 0-output 1-input
    _LATG12=0;			// parte en 0

    //97	CN80/RG13		=> pin es RG13
    _TRISG13=0;			//pin 0-output 1-input
    _LATG13=0;			// parte en 0

    //55	ASCK1/RPI45/INT0/CN72/RF6	=> pin es RF6
    _TRISF6=0;			//pin 0-output 1-input
    _LATF6=0;			// parte en 0

    #if SCH_PAYCAM_nMEMFLASH_ONBOARD == 1
        //SPI_1 es configurado para la Camara en los pines (H1.13 al H1.16)
    #else
        //Pines para la mem flash de 64 Mbit con SPI-1 (SCK1, SDI1, SDO1 nSS1 y nWP)

        // pin 71=	RP12/CN56/PMCS1/RD11	=> pin es RP12 (SCK)
        //iPPSOutput(OUT_PIN_PPS_RP12,OUT_FN_PPS_SCK1);
        iPPSOutput(RPOR6bits.RP12R,0x08);

        // pin 72	RP11/CN49/RD0	=> pin es RP11 (SDI)
        //iPPSOutput(OUT_PIN_PPS_RP11,OUT_FN_PPS_SDO1);
        iPPSOutput(RPOR5bits.RP11R,OUT_FN_PPS_SDO1);

        // 70	RP3/CN55/PMCS2/RD10	=> pin es RP3 (SDO)
        iPPSInput(IN_FN_PPS_SDI1,IN_PIN_PPS_RP3);

        // pin 80	CN19/RD13	=> pin es RD13 (nCS)
        _TRISD13=0;			//pin 0-output 1-input
        _LATD13=0;			// parte en 0

        // pin 83	C3INB/CN15/RD6	=> pin es RD6 (nWP)
        _TRISD6=0;			//pin 0-output 1-input

        Open_SPI_1_default();
        //SPI_SDI_1	//SDO en nomenclatura Pumpkin's
        //SPI_SCK_1	//SCK en nomenclatura Pumpkin's
        //SPI_SDO_1	//SDI en nomenclatura Pumpkin's
        SPI_nSS_1=1;	//nCS en nomenclatura Pumpkin's
        SPI_nWP_1=1;	//nWP en nomenclatura Pumpkin's
    #endif

    // PPC_DEFAULT_CW1() desactiva JTAG (TMS, TCK, TDI, TDO)
    //17	TMS/CN33/RA0	=> pin es RA0
    _TRISA0=0;			//pin 0-output 1-input
    _LATA0=0;			// parte en 0

    //38	TCK/CN34/RA1	=> pin es RA1
    _TRISA1=0;			//pin 0-output 1-input
    _LATA1=0;			// parte en 0

    //60	TDI/CN37/RA4	=> pin es RA4
    _TRISA4=0;			//pin 0-output 1-input
    _LATA4=0;			// parte en 0

    //61	TDO/CN38/RA5	=> pin es RA5
    _TRISA5=0;			//pin 0-output 1-input
    _LATA5=0;			// parte en 0
            
}

/**
 * Configurar los pines de IO de la MB
 */
void GPIO_MB_init(void)
{
    //6	RPI38/CN45/RC1	=> pin es RC1
    //-OE_USB -INT
    _TRISC1=0; 			//pin 0-output 1-input.

    //98	CN60/PMD2/RE2	=> pin es RE2
    //-OE_MHX
    _TRISE2=0; 			//pin 0-output 1-input.

    //99	CN61/PMD3/RE3	=> pin es RE3
    //-ON_MHX
    _TRISE3=0; 			//pin 0-output 1-input.

    //100	CN62/PMD4/RE4	=> pin es RE4
    //-ON_SD
    _TRISE4=0; 			//pin 0-output 1-input.

    PPC_MB_nON_SD = 1;  // Conversor de voltaje SD apagado
    PPC_MB_nON_MHX = 0; // Conversor de voltaje TRX encendido

    #if (SCH_UART_DEBUG_PORT == 0)
        //Consola por puerto DB9
        PPC_MB_nOE_MHX      = 1; // Aislador TRX Apagado, lo activa el driver TRX
        PPC_MB_nOE_USB_nINT = 1; //Aislador USB Apagado, uso puerto DB9
    #else
        //Consola por puerto USB
        PPC_MB_nOE_MHX      = 1; // Aislador TRX Apagado, lo activa el driver TRX
        PPC_MB_nOE_USB_nINT = 0; // Aislador USB Encendido, uso puerto SUB
    #endif

    //7	RPI39/CN46/RC2	=> pin es RC2
    //Es DCD_MHX [Active Low] HS[5..0]
    //HS2 es un caso sui-generis, se "pierde" en el MB y no llega al PC104 (Pumkin's fail)
    _TRISC2=0; 			//pin 0-output 1-input.
    _LATC2=0;			// parte en 0
}

/**
 * Configurar los pines de I/O para el bus PC-104
 */
void GPIO_PC104_init(void)
{
//------------------------------------------------------------------------------
    #if (SCH_PAYBUS_ONBOARD==1)
        // I2C3 para  perifericos de Payload
        // H1.01	SDA3/CN65/PMD7/RE7 (Pin 5) => pin es RE7
        // H1.02	SCL3/CN64/PMD6/RE6 (Pin 4) => pin es RE6
    
        // I2C-3 habilita el mismo sus pines cuando se configura
        i2c3_open(157, SCH_I2C3_ADDR);
    #else
        // H1.01	SDA3/CN65/PMD7/RE7 (Pin 5) => pin es RE7
        _TRISE7=0; 			//pin 0-output 1-input.
        _LATE7=0;			//parte en 0
        
        // H1.02	SCL3/CN64/PMD6/RE6 (Pin 4) => pin es RE6
        _TRISE6=0; 			//pin 0-output 1-input.
        _LATE6=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_MGN_ONBOARD==1)
        // H1.03	SDA2/CN36/RA3 (Pin 59) => pin es RA3
        _TRISA3=0; 			//pin 0-output 1-input.
        PPC_MGN_SWITCH=0;       	//parte en 0
        //pin es Magnetorquer_switch
    #else
        // H1.03	SDA2/CN36/RA3 (Pin 59) => pin es RA3
        _TRISA3=0; 			//pin 0-output 1-input.
        _LATA3=0;             	//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        // H1.04	SCL2/CN35/RA2 (Pin 58) => pin es RA2
        _TRISA2=0; 			//pin 0-output 1-input.
        PPC_CAM_SWITCH=0;       	//parte en 0
        //pin es Camera_switch
    #else
        // H1.04	SCL2/CN35/RA2 (Pin 58) => pin es RA2
        _TRISA2=0; 		//pin 0-output 1-input.
        _LATA2=0;              	//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_PAY_GPS_ONBOARD==1)
        // H1.05	RP25/CN13/PMWR/RD4 (Pin 81) => pin es RD4
        _TRISD4=0; 			//pin 0-output 1-input.
        PPC_GPS_nRESET=1;               //parte en 1 <=> no-reset
        //pin es GPS_nReset
    #else
        // H1.05	RP25/CN13/PMWR/RD4 (Pin 81) => pin es RD4
        _TRISD4=0; 			//pin 0-output 1-input.
        _LATD4=0;              	//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        // H1.06	RPI42/CN57/RD12 (Pin 79) => pin es RD12
        _TRISD12=1;			//pin 0-output 1-input.
        PPC_CAM_HOLD_CHECK;
        //pin es Hold_Camera
    #else
        // H1.06	RPI42/CN57/RD12 (Pin 79) => pin es RD12
        _TRISD12=1;			//pin 0-output 1-input.
        _LATD12=0;          	//parte en 0
    #endif
//------------------------------------------------------------------------------
    // H1.07	RP22/CN52/PMBE/RD3 (Pin 78) => pin es RD3
    _TRISD3=0; 			//pin 0-output 1-input.
    _LATD3=0;			//parte en 0

    // H1.08	RTCC/RP2/CN53/RD8 (Pin 68) => pin es RD8
    _TRISD8=0; 			//pin 0-output 1-input.
    _LATD8=0;			//parte en 0

    // H1.09	RPI32/CN75/RF12 (Pin 40) => pin es RF12
    _TRISF12=0; 		//pin 0-output 1-input.
    _LATF12=0;			//parte en 0

//------------------------------------------------------------------------------
    // H1.10	RP31/CN76/RF13 (Pin 39) => pin es RF13
    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        _TRISF13=0;		//pin 0-output 1-input.
        PPC_LANGMUIR_DEP_SWITCH=0;       	//parte en 0
        //pin es LANGMUIR_switch
    #else
        _TRISF13=0;		//pin 0-output 1-input.
        _LATF13=0;			//parte en 0.
    #endif
//------------------------------------------------------------------------------

    // H1.11	RPI43/CN20/RD14 (Pin 47) => pin es RD14
    _TRISD14=0;			//pin 0-output 1-input.
    _LATD14=0;			//parte en 0

//------------------------------------------------------------------------------
    // H1.12	RP5/CN21/RD15 (Pin 48) => pin es RD15
    #if (SCH_PAY_GPS_ONBOARD==1)
        _TRISD15=0;			//pin 0-output 1-input.
        PPC_GPS_SWITCH=0;       	//parte en 0
        //pin es GPS_switch
    #else
        _TRISD15=0;			//pin 0-output 1-input.
        _LATD15=0;                 	//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        //Pines para la Camara con SPI-1 (SCK1, SDI1, SDO1 nSS1 y nWP)
        // H1.13	C1IND/RP21/CN8/PMA5/RG6 (Pin 10) => pin es RP21 (SCK)
        //iPPSOutput(OUT_PIN_PPS_RP21,OUT_FN_PPS_SCK1);
        iPPSOutput(RPOR10bits.RP21R,0x08);

        // H1.14	C1INC/RP26/CN9/PMA4/RG7 (Pin 11) => pin es RP26 (SDI)
        _TRISG7=1;			//pin 0-output 1-input.
        iPPSInput(IN_FN_PPS_SDI1,IN_PIN_PPS_RP26);

        // H1.15	C2IND/RP19/CN10/PMA3/RG8 (Pin 12) => pin es RP19 (SDO)
        _TRISG8=0;			//pin 0-output 1-input.
        iPPSOutput(OUT_PIN_PPS_RP19,OUT_FN_PPS_SDO1);
        //iPPSOutput(RPOR5bits.RP19R,OUT_FN_PPS_SDO1);

        // H1.16	C2INC/RP27/CN11/PMA2/RG9 (Pin 14) => pin es RG9 (nCS)
        _TRISG9=0; 			//pin 0-output 1-input.
        _LATG9=0;			//parte en 0

        Open_SPI_1_default();
        SPI_nSS_1=1;	//nCS de la camara

    #else
        //SPI_1 es configurado en PPM_init() para la MemFlash en los pines 70, 71, 72, 80 y 83
        // H1.13	C1IND/RP21/CN8/PMA5/RG6 (Pin 10) => pin es RG6
        _TRISG6=0; 			//pin 0-output 1-input.
        _LATG6=0;			//parte en 0

        // H1.14	C1INC/RP26/CN9/PMA4/RG7 (Pin 11) => pin es RG7
        _TRISG7=0; 			//pin 0-output 1-input.
        _LATG7=0;			//parte en 0

        // H1.15	C2IND/RP19/CN10/PMA3/RG8 (Pin 12) => pin es RG8
        _TRISG8=0; 			//pin 0-output 1-input.
        _LATG8=0;			//parte en 0

        // H1.16	C2INC/RP27/CN11/PMA2/RG9 (Pin 14) => pin es RG9
        _TRISG9=0; 			//pin 0-output 1-input.
        _LATG9=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------  
    //Conifg para Consola:
    // H1.17 - U1RX - RP10 - IO.7 - UART 1 PARA CONSOLA SERIAL
    iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP10);

    // H1.18 - U1TX - RP17 - IO.6 - UART 1 PARA CONSOLA SERIAL
    iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U1TX);

    //UART1 - CONSOLA SERIAL - 19200, 8, N, 1 */
    //ConfigRS232(51, RS2_M_UART1);   //19200
    ConfigRS232(25, RS2_M_UART1);   //38400
    EnableIntU1RX;
    SetPriorityIntU1RX(5);

//------------------------------------------------------------------------------
    #if (SCH_PAY_LANGMUIR_ONBOARD == 1)
        // UART3 para la Langmiur
        // H1.19	RP30/CN70/RF2 (Pin 52) => (URX3) para el PAY_LANGMUIR
        iPPSInput(IN_FN_PPS_U3RX,IN_PIN_PPS_RP30);

        // H1.20	RP16/CN71/RF3 (Pin 51) => (UTX3) para PAY_LANGMUIR
        iPPSOutput(OUT_PIN_PPS_RP16,OUT_FN_PPS_U3TX);

        //UART3 para PAY_LANGMUIR
        ConfigRS232(25, RS2_M_UART3);
        EnableIntU3RX;
        SetPriorityIntU3RX(5);
    #else
        // H1.19	RP30/CN70/RF2 (Pin 52) => pin es RF2
        _TRISF2=0; 			//pin 0-output 1-input.
        _LATF2=0;			//parte en 0
        // H1.20	RP16/CN71/RF3 (Pin 51) => pin es RF3
        _TRISF3=0; 			//pin 0-output 1-input.
        _LATF3=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_MEMSD_ONBOARD==1)
        // SPI2 para Tarjeta SD de la MB
        // H1.21	RP4/CN54/RD9 (Pin 69) => pin es RP4 (SCK)
        iPPSOutput(OUT_PIN_PPS_RP4,OUT_FN_PPS_SCK2OUT);

        // H1.22	RPI44/CN73/RF7 (pin 54) => pin es RPI44 (SDI)
        iPPSInput(IN_FN_PPS_SDI2,IN_PIN_PPS_RPI44);

        // H1.23	RP15/CN74/RF8 (Pin 53) => pin es RP15 (SDO)
        iPPSOutput(OUT_PIN_PPS_RP15,OUT_FN_PPS_SDO2);

        // H1.24	CN63/PMD5/RE5 (Pin 3) => pin es RE5 (nSS)
        _TRISE5=0;			//pin 0-output 1-input
        SPI_nSS_2=1;

        Open_SPI_2_default();
        //SPI_SDI_2
        //SPI_SCK_2
        //SPI_SDO_2
        SPI_nSS_2=1;
    #else
        // H1.21	RP4/CN54/RD9 (Pin 69) => pin es RD9
        _TRISD9=0;			//pin 0-output 1-input
        _LATD9=0;			//parte en 0

        // H1.22	RPI44/CN73/RF7 (pin 54) => pin es RF7
        _TRISF7=0;			//pin 0-output 1-input
        _LATF7=0;			//parte en 0

        // H1.23	RP15/CN74/RF8 (Pin 53) => pin es RF8
        _TRISF8=0;			//pin 0-output 1-input
        _LATF8=0;			//parte en 0

        // H1.24	CN63/PMD5/RE5 (Pin 3) => pin es RE5
        _TRISE5=0;			//pin 0-output 1-input
        _LATE5=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    // H1.25	Fault [Active Low]
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.26	PMA6/VREF+/CN42/RA10 (Pin 29) => pin es RA10
    _VCFG=0b000;		// Vr+=Vdd, Vr-=Vss => Vref+ no es usado (_VCFG es bit 15:13 de ADC1CON2)
    _TRISA10=0;			//pin 0-output 1-input.
    _LATA10=0;			//parte en 0

    // H1.27	Sense
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.28	VREF1
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.29	Reset [Active Low]
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.30	VREF-/CN41/PMA7/RA9 (Pin 28) => pin es RA9
    _VCFG=0b000;		// Vr+=Vdd, Vr-=Vss => Vref- no es usado (_VCFG es bit 15:13 de ADC1CON2)
    _TRISA9=0; 			//pin 0-output 1-input.
    _LATA9=0;			//parte en 0

    // H1.31	OFF_VCC
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.32	5V_USB
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.33	PWR_MHX
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.34	RST_MHX [Active Low] HS[5..0]
    //9		RPI41/CN48/RC4	=> pin es RC4
    //HSO
    _TRISC4=0; 			//pin 0-output 1-input.
    _LATC4=0;			// parte en 0

    // H1.35	CTS_MHX [Active Low] HS[5..0]
    //8		RPI40/CN47/RC3	=> pin es RC3
    //HS1
    _TRISC3=0; 			//pin 0-output 1-input.
    _LATC3=0;			// parte en 0

    // H1.36    RTS_MHX [Active Low] HS[5..0]
    //76	RP24/CN50/RD1	=> pin es RD1
    //HS5
    _TRISD1=0; 			//pin 0-output 1-input.
    _LATD1=0;			// parte en 0

    // H1.37	DSR_MHX [Active Low] HS[5..0]
    //82	RP20/CN14/PMRD/RD5	=> pin es RD5
    //HS3
    _TRISD5=0; 			//pin 0-output 1-input.
    _LATD5=0;			// parte en 0

    // H1.38	DTR_MHX [Active Low] HS[5..0]
    //77	RP23/CN51/RD2	=> pin es RD2
    //HS4
    _TRISD2=0; 			//pin 0-output 1-input.
    _LATD2=0;			// parte en 0

    // H1.39	TXD_MHX
    // copia de H1.18
            
    // H1.40	RXD_MHX
    // copia de H1.17

//------------------------------------------------------------------------------
    #if ((SCH_SYSBUS_ONBOARD == 1) || (SCH_TRX_ONBOARD == 1))
        // I2C1 para TRX, EPS y otros perifericos del sistema
        // H1.41	SDA1/CN84/RG3 (Pin 56) => pin es SDA1

        // H1.42	VBACKUP
        // Este pin del header, no tiene un pin del PIC asignado

        // H1.43 	SCL1/CN83/RG2 (Pin 57) => pin es SCL1
        // I2C-1 habilita el mismo sus pines cuando se configura
        i2c1_open(37, SCH_I2C1_ADDR);
    #else
        // H1.41	SDA1/CN84/RG3 (Pin 56) => pin es RG3
        _TRISG3=0; 			//pin 0-output 1-input.
        _LATG3=0;			// parte en 0

        // H1.42	VBACKUP
        // Este pin del header, no tiene un pin del PIC asignado

        // H1.43 	SCL1/CN83/RG2 (Pin 57) => pin es RG2
        _TRISG2=0; 			//pin 0-output 1-input.
        _LATG2=0;			// parte en 0
    #endif
//------------------------------------------------------------------------------
    // H1.44	RSVD0
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.45	RSVD1
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.46	RSVD2
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.47	USER0
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.48	USER1
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.49	USER2
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.50	USER3
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.51	USER4
    // Este pin del header, no tiene un pin del PIC asignado

    // H1.52	USER5
    // Este pin del header, no tiene un pin del PIC asignado

//------------------------------------------------------------------------------
    // H2.01	AN15/REFO/RP29/CN12/PMA0/RB15 (Pin 44) => pin es RB15
    _PCFG15=1; 		//pin 0-ADC 1- I/O
    _TRISB15=0; 	//pin 0-output 1-input.
    _LATB15=0;		//parte en 0

    // H2.02	AN14/CTPLS/RP14/CN32/PMA1/RB14 (Pin 43) => pin es RB14
    _PCFG14=1; 		//pin 0-ADC 1- I/O
    _TRISB14=0; 	//pin 0-output 1-input.
    _LATB14=0;		//parte en 0
//------------------------------------------------------------------------------
    #if (SCH_PAY_FIS_ONBOARD==1)
        // H2.03	AN13/CTED1/CN31/PMA10/RB13 (Pin 42) => pin es AN13 (SENS1)
        _PCFG13=0;                  //pin 0-ADC 1- I/O
        _TRISB13=1;			//pin 0-output 1-input.
        //_LATB13=0;			//parte en 0

        // H2.04	AN12/CTED2/CN30/PMA11/RB12 (Pin 41) => pin es RB12 (nSS)
        _PCFG12=1;                      //pin 0-ADC 1- I/O
        _TRISB12=0;                     //pin 0-output 1-input.
        SPI_nSS_3=1;                    //parte en 1

        // H2.05	AN11/CN29/PMA12/RB11 (Pin 35) => pin es AN11 (SENS2)
        _PCFG11=0;                      //pin 0-ADC 1- I/O
        _TRISB11=1;                     //pin 0-output 1-input.
        //_LATB11=0;			//parte en 0

        // H2.06	AN10/CVREF/CN28/PMA13/RB10 (Pin 34) => pin es RB10
        _PCFG10=1;                  //pin 0-ADC 1- I/O
        _TRISB10=0;			//pin 0-output 1-input.
        _LATB10=0;			//parte en 0

        // H2.07	AN9/RP9/CN27/RB9 (Pin 33) => pin es RP9 (SCK)
        _PCFG9=1;                   //pin 0-ADC 1- I/O
        _TRISB9=0; 			//pin 0-output 1-input.
        iPPSOutput(OUT_PIN_PPS_RP9,OUT_FN_PPS_SCK3OUT);
        //_LATB9=0;			//parte en 0

        // H2.08	AN8/RP8/CN26/RB8 (Pin 32) => pin es RP8 (SDO)
        _PCFG8=1;                   //pin 0-ADC 1- I/O
        _TRISB8=0; 			//pin 0-output 1-input.
        iPPSOutput(OUT_PIN_PPS_RP8,OUT_FN_PPS_SDO3);
        //_LATB8=0;			//parte en 0
            
        Open_SPI_3_default();
        //SPI_SDI_3 no existe
        //SPI_SCK_3
        //SPI_SDO_3
        SPI_nSS_3=1;
    #else
        // H2.03	AN13/CTED1/CN31/PMA10/RB13 (Pin 42) => pin es RB13
        _PCFG13=1;                      //pin 0-ADC 1- I/O
        _TRISB13=0;			//pin 0-output 1-input.
        _LATB13=0;			//parte en 0

        // H2.04	AN12/CTED2/CN30/PMA11/RB12 (Pin 41) => pin es RB12
        _PCFG12=1;                      //pin 0-ADC 1- I/O
        _TRISB12=0;                     //pin 0-output 1-input.
        _LATB12=0;			//parte en 0

        // H2.05	AN11/CN29/PMA12/RB11 (Pin 35) => pin es RB11
        _TRISB11=0;                     //pin 0-output 1-input.
        _LATB11=0;			//parte en 0

        // H2.06	AN10/CVREF/CN28/PMA13/RB10 (Pin 34) => pin es RB10
        _PCFG10=1;                      //pin 0-ADC 1- I/O
        _TRISB10=0;			//pin 0-output 1-input.
        _LATB10=0;			//parte en 0

        // H2.07	AN9/RP9/CN27/RB9 (Pin 33) => pin es RB9
        _PCFG9=1;                       //pin 0-ADC 1- I/O
        _TRISB9=0; 			//pin 0-output 1-input.
        _LATB9=0;			//parte en 0

        // H2.08	AN8/RP8/CN26/RB8 (Pin 32) => pin es RB8
        _PCFG8=1;                       //pin 0-ADC 1- I/O
        _TRISB8=0; 			//pin 0-output 1-input.
        _LATB8=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    //Estos pines son TMS_COM, TCK_COM, TDI_COM, TDO_COM del JTAG del TRX
    //Sin embargo no son ni serian ocupados

    // H2.09	PGED2/AN7/RP7/CN25/RB7 (Pin 27) => pin es RB7
    _PCFG7=1;               	//pin 0-ADC 1- I/O
    _TRISB7=0; 			//pin 0-output 1-input.
    _LATB7=0;			//parte en 0

    // H2.10	PGEC2/AN6/RP6/CN24/RB6 (Pin 26) => pin es RB6
    _PCFG6=1;            		//pin 0-ADC 1- I/O
    _TRISB6=0; 			//pin 0-output 1-input.
    _LATB6=0;			//parte en 0

    // H2.11	PGED1/AN0/RP0/CN2/RB0 (Pin 25) => pin es PGED1
    // este pin es usado como PGED1 para programming/debugging con ICD
    // por lo que modificarlo implica posibles errores al quierer debuggear con el ICD3

    // H2.12	PGEC1/AN1/RP1/CN3/RB1 (Pin 24) => pin es PGEC1
    // este pin es usado como PGEC1 para programming/debugging con ICD
    // por lo que modificarlo implica posibles errores al querer debuggear con el ICD3
//------------------------------------------------------------------------------
    // H2.13	AN2/C2INB/RP13/CN4/RB2 (Pin 23)	 => pin es RB2
    _PCFG2=1; 			//pin 0-ADC 1- I/O
    _TRISB2=0; 			//pin 0-output 1-input.
    _LATB2=1;			//parte en 1 (por si TRX esta conectado, aunque no deberia)

    // H2.14	AN3/C2INA/CN5/RB3 (Pin 22) => pin es RB3
    _PCFG3=1; 			//pin 0-ADC 1- I/O
    _TRISB3=0; 			//pin 0-output 1-input.
    _LATB3=1;			//parte en (por si TRX esta conectado, aunque no deberia)
//------------------------------------------------------------------------------
    #if (SCH_PAY_GPS_ONBOARD==1)
        // H2.15	PGED3/AN4/C1INB/RP28/CN6/RB4 (Pin 21) => pin es RP28 (TX del UART4)
        _PCFG4=1; 			//pin 0-ADC 1- I/O
        _TRISB4=0; 			//pin 0-output 1-input.
        _LATB4=0;			//parte en 0
        iPPSOutput(OUT_PIN_PPS_RP28,OUT_FN_PPS_U4TX);

        // H2.16	PGEC3/AN5/C1INA/RP18/CN7/RB5 (Pin 20) => pin es RP18 (RX del UART4)
        _PCFG5=1; 			//pin 0-ADC 1- I/O
        _TRISB5=1; 			//pin 0-output 1-input.
        //_LATB5=0;			//parte en 0
        iPPSInput(IN_FN_PPS_U4RX,IN_PIN_PPS_RP18);

        //UART4 para GPS
        ConfigRS232(25, RS2_M_UART4);
        //EnableIntU4RX;
        //SetPriorityIntU4RX(5);
    #else
        // H2.15	PGED3/AN4/C1INB/RP28/CN6/RB4 (Pin 21) => pin es RB4
        _PCFG4=1; 			//pin 0-ADC 1- I/O
        _TRISB4=0; 			//pin 0-output 1-input.
        _LATB4=0;			//parte en 0

        // H2.16	PGEC3/AN5/C1INA/RP18/CN7/RB5 (Pin 20) => pin es RB5
        _PCFG5=1; 			//pin 0-ADC 1- I/O
        _TRISB5=0; 			//pin 0-output 1-input.
        _LATB5=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_ANTENNA_ONBOARD==1)
        // H2.17	RPI34/CN67/RE9 (Pin 19) => pin es RE9 (ANTENNA1)
        _TRISE9=0; 			//pin 0-output 1-input.
        PPC_ANT1_SWITCH=0;         //parte en 0
        //pin es ANT1_switch
        
        // H2.18	RPI33/CN66/RE8 (Pin 18) => pin es RE8  (ANTENNA2)
        _TRISE8=0; 			//pin 0-output 1-input.
        PPC_ANT2_SWITCH=0;         //parte en 0
        //pin es ANT2_switch
    #else
        // H2.17	RPI34/CN67/RE9 (Pin 19) => pin es RE9
        _TRISE9=0; 			//pin 0-output 1-input.
        _LATE9=0;                       //parte en 0

        // H2.18	RPI33/CN66/RE8 (Pin 18) => pin es RE8
        _TRISE8=0; 			//pin 0-output 1-input.
        _LATE8=0;                       //parte en 0
    #endif
//------------------------------------------------------------------------------
    // H2.19	ASDA2/RPI35/CN44/RA15 (Pin 67) => pin es RA15, pues I2C-2 esta en H1.3 y H1.4
    _TRISA15=0;			//pin 0-output 1-input.
    _LATA15=0;			//parte en 0

    // H2.20 ASCL2/RPI36/CN43/RA14 (Pin 66) => pin es RA14, pues I2C-2 esta en H1.3 y H1.4
    _TRISA14=0;			//pin 0-output 1-input.
    _LATA14=0;			//parte en 0

    // H2.21	CN77/RG0 (Pin 90) => pin es RG0
    _TRISG0=0; 			//pin 0-output 1-input.
    _LATG0=0;			//parte en 0
//------------------------------------------------------------------------------
    #if (SCH_ANTENNA_ONBOARD==1)
        // H2.22	CN78/RG1 (Pin 89) => pin es RG1
        _TRISG1=0; 			//pin 0-output 1-input.
        PPC_ANT12_SWITCH=0;         //parte en 0

        //pin es ANT12_switch
        // H2.23	CN68/RF0 (Pin 87) => pin es PPC_ANT12_CHECK (CHECK ANTENNA DEPLOY)
        _TRISF0=1; 			//pin 0-output 1-input.
    #else
        // H2.22	CN78/RG1 (Pin 89) => pin es RG1
        _TRISG1=0; 			//pin 0-output 1-input.
        _LATG1=0;			//parte en 0
        
        // H2.23	CN68/RF0 (Pin 87) => pin es RF0
        _TRISF0=1; 			//pin 0-output 1-input.
        _LATF0=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    #if (SCH_PAY_GYRO_ONBOARD==1)
        // H2.24	CN69/RF1 (Pin 88) => pin es PPC_GYRO_INT2_CHECK
        _TRISF1=1; 			//pin 0-output 1-input.
        //_LATF1=0;			//parte en 0
    #else
        // H2.24	CN69/RF1 (Pin 88) => pin es RF1
        _TRISF1=0; 			//pin 0-output 1-input.
        _LATF1=0;			//parte en 0
    #endif
//------------------------------------------------------------------------------
    // H2.25	5V_SYS
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.26	5V_SYS
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.27	VCC_SYS
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.28	VCC_SYS
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.29	DGND
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.30	DGND
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.31	AGND
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.32	DGND
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.33	S0
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.34	S0
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.35	S1
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.36	S1
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.37	S2
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.38	S2
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.39	S3
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.40	S3
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.41	S4
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.42	S4
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.43	S5
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.44	S5
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.45	VBATT
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.46	VBATT
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.47	USER6
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.48	USER7
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.49	USER8
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.50	USER9
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.51	USER10
    // Este pin del header, no tiene un pin del PIC asignado

    // H2.52	USER11
    // Este pin del header, no tiene un pin del PIC asignado
//------------------------------------------------------------------------------
}
