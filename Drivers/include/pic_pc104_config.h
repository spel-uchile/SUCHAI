/**
 * @file  pic_pc104_config.c
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @date 2012
 * @copyright GNU GPL v3.
 *
 * This library configures all microcontroller's functionalities
 *
 * Este .h y su correspondiente .c contendra la configuracion del PIC.
 * Muchas partes de las librerias incluidas se han repetido, para efecto de
 * claridad y completitud, de modo que solo sea necesario acceder a este .h
 * para poder configurar todo
 *
 * Para el bus PC104:
 * -PPS
 * -IO ports (input-output, digital-analog)
 *
 * Config General
 * -Power Management
 * -WDT
 * -Config words
 * -Resets
 * -Interrupt controllers
 * -Oscillator config
 */

#ifndef PIC_PC104_CONFIG
#define PIC_PC104_CONFIG

//Modulos que PIC_PC104_CONFIG debe CONFIGURAR
#include <PwrMgnt.h>
#include <PPS.h>			
#include <ports.h>
#include <timer.h>
#include <wdt.h>
//includes para estados default y configuraciones on-board off-board
#include "SUCHAI_config.h"
//Modulos del PIC24
#include "i2c_comm.h"
#include "rs232_suchai.h"
#include "interfaz_SPI.h"
//Otros
#include "DebugIncludes.h"  //para con_printf


/**
 * DEFAULT_PIC_CONFIG
 * Para una configuracion rapida y "estandar" debe usar la funcion default_PIC_config();
 * ademas de las 3 macros para los Config Words
 *      PPC_DEFAULT_CW1();
 *      PPC_DEFAULT_CW2();
 *      PPC_DEFAULT_CW3();
 */
void default_PIC_config(void);


/**
 * PERIPHERAL PIN SELECT
 * inicializan pines y perifericos. DEBEN ser llamados luego de cada reset del sistema
 */
void GPIO_PPM_init(void);
void GPIO_MB_init(void);
void GPIO_PC104_init(void);

/**
 * Defines para los pines que controlan/chequean payloads/perifericos/agentes externos
 */
#if (SCH_MGN_ONBOARD==1)
    #define PPC_MGN_SWITCH _LATA3
#endif
#if (SCH_PAY_CAM_nMEMFLASH_ONBOARD==1)
    #define PPC_CAM_SWITCH          _LATA2
    #define PPC_CAM_SWITCH_CHECK    _RA2
    #define PPC_CAM_HOLD_CHECK      _RD12
#else
    #define PPC_CAM_SWITCH          _RA2
    #define PPC_CAM_SWITCH_CHECK    (-1)
    #define PPC_CAM_HOLD_CHECK      (-1)
#endif
#if (SCH_PAY_LANGMUIR_ONBOARD==1)
    #define PPC_LANGMUIR_DEP_SWITCH          _LATF13
    #define PPC_LANGMUIR_DEP_SWITCH_CHECK    _RF13
#else
    #define PPC_LANGMUIR_DEP_SWITCH_CHECK    (-1)
#endif
#if (SCH_PAY_GPS_ONBOARD==1)
    #define PPC_GPS_SWITCH          _LATD15
    #define PPC_GPS_SWITCH_CHECK    _RD15
    #define PPC_GPS_nRESET          _LATD4
#else
    #define PPC_GPS_SWITCH          _LATD15
    #define PPC_GPS_SWITCH_CHECK    _RD15
    #define PPC_GPS_nRESET          _LATD4
#endif
#if (SCH_ANTENNA_ONBOARD==1)
    #define PPC_ANT12_SWITCH    _LATG1
    #define PPC_ANT1_SWITCH     _LATE9
    #define PPC_ANT2_SWITCH     _LATE8
    #define PPC_ANT12_CHECK     _RF0
#else
    #define PPC_ANT12_CHECK     (-1)
#endif
#if (SCH_PAY_GYRO_ONBOARD==1)
    #define PPC_GYRO_INT2_CHECK   _RF1
#else
    #define PPC_GYRO_INT2_CHECK   (-1)
#endif

/**
 * MB Hardware IO pines
 */
#define PPC_MB_nOE_USB_nINT     _LATC1
#define PPC_MB_nOE_MHX          _LATE2
#define PPC_MB_nON_MHX          _LATE3
#define PPC_MB_nON_SD           _LATE4

#define PPC_MB_nOE_USB_nINT_CHECK   _RC1
#define PPC_MB_nOE_MHX_CHECK        _RE2
#define PPC_MB_nON_MHX_CHECK        _RE3
#define PPC_MB_nON_SD_CHECK         _RE4

#define PPC_SWReset {__asm__ volatile("reset");}    ///< Microcontroller software reset

void init_Check_Reset_Status(void);

/**
 * Check if any reset event ocurred
 * RESET_SOURCE:
 * POWER_ON_Reset, 		0		  BURN_OUT_Reset,		1		WATCHDOG_Reset, 			2
 * SOFTWARE_Reset, 		3		  EXTERNAL_Reset,		4		CFG_WORD_MISMATCH_Reset, 	5
 * ILLEGAL_INSTR_Reset,	6	   	  TRAP_Reset			7
 */
void Check_Reset_Status(void);

/**
 * Reset event handler functiosns
 */
void POWER_ON_Reset_Routine(void);
void BURN_OUT_Reset_Routine(void);
void WATCHDOG_Reset_Routine(void);
void SOFTWARE_Reset_Routine(void);
void EXTERNAL_Reset_Routine(void);
void CFG_WORD_MISMATCH_Reset_Routine(void);
void ILLEGAL_INSTR_Reset_Routine(void);
void TRAP_Reset_Routine(void);

/**
 * Returns current oscillator
 * 
 * #define FRC_OSC                           0x0000 //Internal oscillator
 * #define FRC_OSC_WITH_POSTSCALER_PLL       0x0001 //Internal oscillator with Postscalar PLL enabled
 * #define PRIMARY_OSC                       0x0002 //Primary oscillator
 * #define PRIMARY_OSC_WITH_PLL              0x0003 //Primary oscillator with PLL
 * #define SECONDRY_OSC                      0x0004 //Secondary oscillator
 * #define LOW_POWER_RC                      0x0005 //Low Power RC oscillator
 * #define FRC_OSC_WITH_POSTSCALER           0x0007 //Internal oscillator with postscalar
 * #define FRC_OSC_MASK                      (~FRC_OSC_WITH_POSTSCALER)
 */
unsigned int Current_OSCILLATOR(void);						

/**
 * PPC_SW_LOCK_CLK_SWITCH
 * 
 * Si FCKSM1=0 =>clock swhitch always enabled => SW_lock_Clk_Switch() NO tiene efecto
 * Si FCKSM1=1 =>clock swhitch diabled => SW_lock_Clk_Switch() SI tiene efecto y:
 * Luego de llamar a esta funcion, bloquea por software todo clk switch, solo
 * un reset permite volver a hacer clk switch.
 */
#define PPC_SW_LOCK_CLK_SWITCH	(_CLKLOCK=1)

#define PPC_PRIMARY_OSC_ACTIVE_SLEEP		(OSCCON=OSCCON|0x0004)		//PrimOSC enable in sleep mode
#define PPC_PRIMARY_OSC_INACTIVE_SLEEP		(OSCCON=OSCCON&0xFFFB)		//PrimOSC disable in sleep mode

#define PPC_ENABLE_SOSC()	_SOSCEN=1   ///< SOSC Enable
#define PPC_DISABLE_SOSC()	_SOSCEN=0	///< SOSC Disabled

void sw_reset(int arg);
int set_new_osc(int arg);
void Check_FSCM(void);
void FSCM_Routine(void);


/**
 * REFOCON: Reference Oscillator Control Register
 * REFO is enable as system clock (arg=1 enable, 0=disable)
 */
void REFO_enabled(int arg);

#define PPC_REFO_DISABLED           (_ROON=0)   ///< REFO disable
#define PPC_REFO_ACTIVE_SLEEP		(_ROSSLP=1)	///< REFO enable in sleep mode
#define PPC_REFO_INACTIVE_SLEEP		(_ROSSLP=0)	///< REFO disable in sleep mode

/**
 * Power management configuration
 * mPWRMGNT_GotoIdleMode asm ( " PWRSAV #1 " )		// halts CPU only
 * mPWRMGNT_GotoSleepMode asm ( " PWRSAV #0 " )		// halts CPU and Peripheral
 */

void Check_Wakeup_Status(void);
void WAKEUP_SLEEP_Routine(void);
void WAKEUP_IDLE_Routine(void);

/**
 * Watchdod initial state
 */
#define PPC_INITIAL_WDT_STATE   WDT_ENABLE

/**
 * Configuration words
 */
#define PPC_DEFAULT_CW1()	_CONFIG1(JTAGEN_OFF& GCP_OFF& GWRP_OFF& FWDTEN_OFF& FWPSA_PR128& WDTPS_PS32768 )
#define PPC_DEFAULT_CW2()	_CONFIG2(IESO_OFF& FNOSC_PRIPLL& FCKSM_CSECME& OSCIOFNC_OFF& IOL1WAY_ON& POSCMOD_XT)
#define PPC_DEFAULT_CW3()	_CONFIG3(WPCFG_WPCFGDIS& WPDIS_WPDIS)

#endif
