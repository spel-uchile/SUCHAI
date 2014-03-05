/*                            AS-COM-01 DRIVER
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

#ifndef TRANSCEIVER_H /* TRANSCEIVER_H */
#define TRANSCEIVER_H

#include "pic_pc104_config.h"
#include "console.h"
#include "DebugIncludes.h"  //para con_printf

/******************************************************************************
 *
 *                  TRANSCEIVER INTERFACE HEADER
 *
 ******************************************************************************
 * FileName:        transceiver.h
 * Dependencies:    i2c_com.h - serial_com.h
 * Processor:       PIC24F
 * Compiler:        MPLAB XC16
 * Company:         SUCHAI
 * Autor:           Carlos Gonzalez C - carlgonz@ug.uchile.cl
 *****************************************************************************/

/*-------------------------------------------------------------
 *                TRANSCEIVER FLAG (From pic_pc104.h)
 *-----------------------------------------------------------*/
//#define PPC_nRX_FLAG _RB2    // Transceiver reciviendo
//#define PPC_nTX_FLAG _RB3    // Transceiver transmitiendo

/* ------------------------------------------------------------
 *		 TRANSCEIVER I2C ID
 * -----------------------------------------------------------*/
#define TRX_TRANSCEIVER_ID  0x14 /* I2C node address (7bit hex) */
#define TRX_TRANSCEIVER_IDW 0x28 /* I2C node address Write */
#define TRX_TRANSCEIVER_IDR 0x29 /* I2C node address Read */

/* ------------------------------------------------------------
 * 	 	TRANSCEIVER REGISTER DEFINES
 * -----------------------------------------------------------*/
//      REGISTER ANME           ADDR		DEF	TYPE    DESCRIPTION
#define	TRX_ID                  0x00
#define	TRX_DEVID               0x01
#define	TRX_REV                 0x02
#define	TRX_MODE                0x03
#define	TRX_UPDATE              0x04
#define	TRX_ACTION              0x05
#define	TRX_ACTION_BEACON	0x06
#define	TRX_ACTION_TM           0x07
#define	TRX_ACTION_TC           0x08
#define	TRX_EVENT               0x09
#define	TRX_EVENT_BEACON	0x0A
#define	TRX_EVENT_TM            0x0B
#define	TRX_EVENT_TC            0x0C
#define	TRX_TEMPMCU_H           0x0D
#define	TRX_TEMPMCU_L           0x0E
#define	TRX_TEMPHPA_H           0x0F
#define	TRX_TEMPHPA_L           0x10
#define	TRX_RSSI_H              0x11
#define	TRX_RSSI_L              0x12
#define	TRX_TCTF_IN_H           0x13
#define	TRX_TCTF_IN_L           0x14
#define	TRX_TCTF_OUT_H          0x15
#define	TRX_TCTF_OUT_L          0x16
#define	TRX_TMTF_IN_H           0x17
#define	TRX_TMTF_IN_L           0x18
#define	TRX_TMTF_OUT_H          0x19
#define	TRX_TMTF_OUT_L          0x1A
#define	TRX_TMTF_COUNTER        0x1B
//#define TRX_FARMB_COUNTER     0x1C
//#define TRX_FARM_STATE        0x1D
//#define TRX_STATUS_TC         0x1E
//#define TRX_FARM_VR           0x1F
//#define TRX_DISCARD_COUNTER   0x20
//#define TRX_I2CADR	0x21
#define	TRX_HKPERIOD            0x22
#define	TRX_MEASURE             0x23
#define	TRX_BEACONPOWER         0x24
#define	TRX_BEACONWPM           0x25
#define	TRX_BEACONLENGTH	0x26
#define	TRX_BEACONOFFSET	0x27
#define	TRX_BEACONBUFFER	0x28
#define	TRX_BEACONPERIOD	0x29
#define	TRX_BEACONTIMER         0x2A
#define	TRX_TMPOWER             0x2B
#define	TRX_TMPREAMBLELENGTH	0x2C
#define	TRX_TMFRAMECOUNTMAX	0x2D
#define	TRX_DEPLOY_DURATION	0x2E
#define	TRX_FRX_H               0x2F
#define	TRX_FRX_M               0x30
#define	TRX_FRX_L               0x31
#define	TRX_FTX_H               0x32
#define	TRX_FTX_M               0x33
#define	TRX_FTX_L               0x34
#define	TRX_FSEP_H              0x35
#define	TRX_FSEP_L              0x36
//#define TRX_MODEMBAUDRATE	0x37
//#define TRX_BITSTUFFING	0x38


/* ------------------------------------------------------------
 * 	 	TRANSCEIVER OTHERS DEFINES
 * -----------------------------------------------------------*/
#define TRX_TCFRAMELEN          (63)    /* TC frame lenght */
#define TRX_TMFRAMELEN          (126)   /* TM frame lenght */

#define TRX_MODE_RESET          0//(0x01)
#define TRX_MODE_SYSRST         1//(0x02)
#define TRX_MODE_SILENT         2//(0x04)
#define TRX_MODE_ONLYBCN        3//(0x08)
#define TRX_MODE_NOBEACON       4//(0x10)
#define TRX_MODE_NOMINAL        5//(0x20)

/* ------------------------------------------------------------
 * 	 	TRANSCEIVER DEFAULT VALUES DEFINES
 * -----------------------------------------------------------*/
#define TRX_DEF_MODE        (0x20)  /* Nominal */
#define TRX_DEF_BCNPWR      (0x04)
#define TRX_DEF_BCNWPM      (0x0F)
#define TRX_DEF_BCNTMR      ((0x0A)*4) /*Segundos*/
#define TRX_DEF_HKPER       (0x01)  /* Segundos*/

#define TRX_DEF_TMPWR       (0x08)
#define TRX_DEF_TMPRE       (0x50)//(0x28)
#define TRX_DEF_TMCMX       (0x00) /* No limit */

#define TRX_DEF_FTXH        (0x58) /* 437 kHz */
#define TRX_DEF_FTXM        (0xC7)
#define TRX_DEF_FTXL        (0x52)

#define TRX_DEF_FRXH        (0x58) /* 437 kHz */
#define TRX_DEF_FRXM        (0xBF)
#define TRX_DEF_FRXL        (0xC0)

#define TRX_DEF_FSEPH       (0x00) /* 6 kHz*/
#define TRX_DEF_FSEPL       (0x50)

//#define TRX_DEF_BSTUF        (0x91)

/* ------------------------------------------------------------
 * 	 	TRANSCEIVER UNIT CONSTANTS DEFINES
 * -----------------------------------------------------------*/
#define TRX_RSSI_DBM
#ifdef TRX_RSSI_DBM
    #define TRX_RSSI_GAIN   (-0.0375732)
    #define TRX_RSSI_OFFSET (-49.2)
#endif

//#define TRX_TEMP_C
#ifdef TRX_TEMP_C
    #define TRX_TEMP_GAIN   (1)
    #define TRX_TEMP_OFFSET (0)
#endif

//#define TRX_FREQ_KHZ
#ifdef TRX_FREQ_KHZ
    #define TRX_FREQ_GAIN   (1)
    #define TRX_FREQ_OFFSET (0)
#endif
/* ------------------------------------------------------------
 *                  TRANSCEIVER STRUCTURES
 * -----------------------------------------------------------*/

typedef struct TXRXSETT {
    unsigned char _MODE_TRX:8;

    unsigned char _HKP_PER:8; /*!< Segundos*/
    unsigned char _BCN_PWR:8;
    unsigned char _BCN_WPM:8;
    unsigned char _BCN_TMR:8; /*!< Segundos */

    unsigned char _TM_PWR:8;
    unsigned char _TM_PRE:8;
    unsigned char _TM_CMX:8;

    unsigned char _FRX_H:8;
    unsigned char _FRX_M:8;
    unsigned char _FRX_L:8;

    unsigned char _FTX_H:8;
    unsigned char _FTX_M:8;
    unsigned char _FTX_L:8;

    unsigned char _FSEP_H:8;
    unsigned char _FSEP_L:8;

//    unsigned char _BSTUF:8;

} TxRxSettings;

typedef struct BEACONSET {
    unsigned char *Content;
    unsigned char Len;
    unsigned char Timer; /*!< Beacon period, seconds */
    unsigned char Power;
    unsigned char WPM;
}BeaconSett;

#define TRX_MES_TEMP    1
#define TRX_MES_RSSI    2
#define TRX_MES_BOTH    3

void TRX_WriteRegister(unsigned int reg, unsigned char val);
unsigned char TRX_ReadRegister(unsigned int reg);

void TRX_ConfigComPort();
void TRX_SetMode(int ModeCode);
void TRX_BeaconAction(int BeaconCode);
void TRX_SetMesurements(int MesCode);
void TRX_SetHKPeriod(int period_s);
void TRX_SetTransceiverSettings(TxRxSettings *Setting);
void TRX_BeaconSettings(BeaconSett *Setting);
void TRX_SetBeaconContent(unsigned char *content, unsigned int len);

void TRX_GetStatus(unsigned char *buffer);

void TRX_SendIdleFrame(void);
int TRX_LoadTelemetry(unsigned char *telemetry, unsigned int len);
int TRX_SendTelemetry(void);
int TRX_ReadTelecomadFrame(char *buffer);
int TRX_CheckNewTC(void);

#endif /* TRANSCEIVER_H  */
