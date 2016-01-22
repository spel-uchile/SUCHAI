#ifndef EPS_SUCHAI
#define EPS_SUCHAI

#include "i2c_comm.h"
#include "DebugIncludes.h"

#define EPS_ADDR 0x2D

/*DOD values translated to "SOC". But in current implementation DOD 50% ~ MAX_DOD
 5, DOD 80% ~ SAFE_DOD 8*/
/*Max DOD when a DOD below this value is reached high consuptions are denied*/
#define EPS_MAX_DOD         1
/*Safe DOD to which high consumptions are allowed again*/
#define EPS_SAFE_DOD        2

/*CONsumos expresados en miliwats*/
	#define EPS_CONtranciver 4300
	#define EPS_CONbeacon 200
	#define EPS_CONgps 1200
	#define EPS_CONcamara 200
	#define EPS_CONdespliegue 10000
	#define EPS_CONpayload1 0
	#define EPS_CONpayload2 0

/*This are associated to DataRepository's EPS_state and are use to read
 *  eps_suchai.c data structure*/
#define	EPS_ID_bat0_voltage		0
#define	EPS_ID_bat0_current		1
#define	EPS_ID_bus5V_current		2
#define	EPS_ID_bus3V_current		3
#define	EPS_ID_bus_battery_current	4
#define	EPS_ID_bat0_temp		5
#define	EPS_ID_panel_pwr		6
#define	EPS_ID_status			7
#define	EPS_ID_current_dir_est		8
#define	EPS_ID_soc			9
#define	EPS_ID_socss			10
#define	EPS_ID_state_flag       	11
#define EPS_ID_current_dir              12



//comandos
#define EPS_adc 0
#define EPS_status 1
#define EPS_pdmoff 2
#define EPS_version 4
#define EPS_heater 5
#define EPS_watchdog 128

//dimensiones VSOC
#define EPS_Cvsocdim 10
#define EPS_Vvsocdim 9
#define EPS_Tvsocdim 7

/*
typedef struct
{
    double Voltage;
    double iCurrent;
    char soc;
}EPS_Bat_State;*/

/*Mediciones importantes de bateria*/
typedef struct
{
    unsigned int Voltage;
    unsigned int Current;
    unsigned int Temp;
    unsigned int CurrDir;
}EPS_Bat_mes;

/*Mediciones de corriente de buses*/
typedef struct
{
    unsigned int bus5V;
    unsigned int bus3V;
    unsigned int busBat;
}EPS_Bus_curr;

/*Mediciones de paneles*/
typedef struct
{
    unsigned int MXCurr;
    unsigned int mXCurr;
    unsigned int MYCurr;
    unsigned int mYCurr;
    unsigned int MZCurr;
    unsigned int mZCurr;

    unsigned int Xvolt;
    unsigned int Yvolt;
    unsigned int Zvolt;

    unsigned int pwr;    
}EPS_pnl_mes;

/*Estados*/
typedef struct
{
    unsigned int status;
    char currDirEst;
    char soc;
    char socss;
    char state_flag;
    char energy;
}EPS_st;

/*------------------------------------------------------------------------------------------
*	Resumen		:	definicion de macros para transformar en valores en voltaje mA o �C
*					los diferentes canales de ADC segun el "User Manual: CubeSat 1U
*					Electronic Power System and Batteries: CS-1UEPS2-NB/-10/-20"
*
*					ADC canal 1 #define ArrplusYCurr(ADC) (-0.5*ADC+515.7)
*					ADC canal 2 #define ArrplusYTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 3 #define ArrpairYVolt(ADC) (-0.0086*ADC+8.81)
*					ADC canal 4 #define ArrminuXCurr(ADC) (-0.5*ADC+515.7)
*					ADC canal 5 #define ArrminuXTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 6 #define ArrpairXVolt(ADC) (-0.0086*ADC+8.81)
*					ADC canal 7 #define ArrplusXCurr(ADC) (-0.5*ADC+515.7)
*					ADC canal 8 #define ArrplusXTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 9 #define ArrpairZVolt(ADC) (-0.0086*ADC+8.81)
*					ADC canal 10 #define ArrplusZCurr(ADC) (-0.5*ADC+515.7)
*					ADC canal 11 #define ArrplusZTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 13 #define ArrminuYCurr(ADC) (-0.5*ADC+515.7)
*					ADC canal 14 #define ArrminuYTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 17 #define BattBusCurr(ADC) (-3.153*ADC+3250.815)
*					ADC canal 18 #define Bat1Temp(ADC) (-0.163*ADC+110.835)
*					ADC canal 19 #define Bat1FullVolt(ADC) (-0.0939*ADC+9.791)
*					//cannal 21 bat1 direction, CS-1UEPS2-20 only
*					//solo define direccion High = bat charge, Low = bat discharge
*					ADC canal 22 #define Bat1Curr(ADC) (-3.20*ADC+2926.22)
*					ADC canal 23 #define Bat0Temp(ADC) (-0.163*ADC+110.835)
*					ADC canal 24 #define Bat0FullVolt(ADC) (-0.0939*ADC+9.791)
*					ADC canal 26 #define V5BusCurr(ADC) (-3.500*ADC+3611.509)
*					ADC canal 27 #define V33BusCurr(ADC) (-4.039*ADC+4155.271)
*					//cannal 28 bat0 direction, CS-1UEPS2-20 y CS-1UEPS2-10 only
*					//solo define direccion High = bat charge, Low = bat discharge
*					ADC canal 29 #define Bat0Curr(ADC) (-3.20*ADC+2926.22)
*					ADC canal 30 #define ArrminuZTemp(ADC) (-0.163*ADC+110.338)
*					ADC canal 31 #define ArrminuZCurr(ADC) (-0.5*ADC+515.7)
------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusYCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 1 (Array +Y Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 1 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo +Y
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusYCurr(ADC) (-0.4990*(double)(ADC)+524.4126)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusYTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 2 (Array +Y Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 2 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo +Y
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusYTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrpairYVolt(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 3 (Array pair Y Voltage) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 3 (0-1023)
 * Return Value     : Valor flotante correspondiente a voltaje en [V] arreglo pair Y
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrpairYVolt(ADC) (-0.008862*(double)(ADC)+8.933533)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuXCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 4 (Array -X Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 4 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo -X
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuXCurr(ADC) (-0.5077*(double)(ADC)+527.8192)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuXTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 5 (Array -X Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 5 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo -X
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuXTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrpairXVolt(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 6 (Array pair X Voltage) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 6 (0-1023)
 * Return Value     : Valor flotante correspondiente a voltaje en [V] arreglo pair X
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrpairXVolt(ADC) (-0.008671*(double)(ADC)+8.857994)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusXCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 7 (Array +X Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 7 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo +X
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusXCurr(ADC) (-0.5049*(double)(ADC)+523.753)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusXTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 8 (Array +X Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 8 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo +X
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusXTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrpairZVolt(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 9 (Array pair Z Voltage) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 9 (0-1023)
 * Return Value     : Valor flotante correspondiente a voltaje en [V] arreglo pair Z
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrpairZVolt(ADC) (-0.008399*(double)(ADC)+8.627585)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusZCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 10 (Array +Z Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 10 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo +Z
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusZCurr(ADC) (-0.4992*(double)(ADC)+516.7345)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrplusZTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 11 (Array +Z Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 11 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo +Z
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrplusZTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuYCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 13 (Array -Y Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 13 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo -Y
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuYCurr(ADC) (-0.5024*(double)(ADC)+520.7108)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuYTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 14 (Array -Y Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 14 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo -Y
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuYTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: BattBusCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 17 (Battery Bus Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 17 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] Battery bus current
 *----------------------------------------------------------------------------------------*/
#define EPS_BattBusCurr(ADC) (-3.0854*(double)(ADC)+3189.1876)

/*------------------------------------------------------------------------------------------
 * Macro			: Bat1Temp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 18 (BAT 1 Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 18 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] BAT 1 temperature
 * Remarks			: Solo para CS-1UEPS2-20
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat1Temp(ADC) (-0.163*(double)(ADC)+110.835)

/*------------------------------------------------------------------------------------------
 * Macro			: Bat1FullVolt(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 19 (BAT 1 full voltage) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 19 (0-1023)
 * Return Value     : Valor flotante correspondiente a voltage en [V] BAT 1 full voltage
 * Remarks			: Solo para CS-1UEPS2-20
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat1FullVolt(ADC) (-0.00939*(double)(ADC)+9.791)

/*cannal 21 bat1 direction, CS-1UEPS2-20 only*/
//solo define direccion High = bat charge, Low = bat discharge

/*------------------------------------------------------------------------------------------
 * Macro			: Bat1Curr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 22 (BAT 1 current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 22 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] BAT 1 current
 * Remarks			: Solo para CS-1UEPS2-20
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat1Curr(ADC) (-3.20*(double)(ADC)+2926.22)

/*------------------------------------------------------------------------------------------
 * Macro			: Bat0Temp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 23 (BAT 0 Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 23 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] BAT 0 temperature
 * Remarks			: Solo para CS-1UEPS2-20 y CS-1UEPS2-10
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat0Temp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: Bat0FullVolt(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 24 (BAT 0 full voltage) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 24 (0-1023)
 * Return Value     : Valor flotante correspondiente a voltage en [V] BAT 0 full voltage
 * Remarks			: Solo para CS-1UEPS2-20 y CS-1UEPS2-10
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat0FullVolt(ADC) (-0.00939*(double)(ADC)+9.791)

/*------------------------------------------------------------------------------------------
 * Macro			: V5BusCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 26 (5V bus current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 26 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] 5V bus current
 *----------------------------------------------------------------------------------------*/
#define EPS_V5BusCurr(ADC) (-3.525*(double)(ADC)+3638.812)

/*------------------------------------------------------------------------------------------
 * Macro			: V33BusCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 27 (3.3V bus current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 27 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] 3.3V bus current
 *----------------------------------------------------------------------------------------*/
#define EPS_V33BusCurr(ADC) (-4.497*(double)(ADC)+4650.508)

/*cannal 28 bat0 direction, CS-1UEPS2-20 y CS-1UEPS2-10 only*/
//solo define direccion High = bat charge, Low = bat discharge

/*------------------------------------------------------------------------------------------
 * Macro			: Bat0Curr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 29 (BAT 0 current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 29 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] BAT 0 current
 * Remarks			: Solo para CS-1UEPS2-20 y CS-1UEPS2-10
 *----------------------------------------------------------------------------------------*/
#define EPS_Bat0Curr(ADC) (-3.20*(double)(ADC)+2926.22)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuZTemp(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 30 (Array -Z Temperature) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 30 (0-1023)
 * Return Value     : Valor flotante correspondiente a temperatura en [�C] arreglo -Z
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuZTemp(ADC) (-0.163*(double)(ADC)+110.338)

/*------------------------------------------------------------------------------------------
 * Macro			: ArrminuZCurr(ADC)
 * Include			: eps_suchai.h
 * Description		: Transforma valor de ADC canal 31 (Array -Z Current) en flotante					
 * Arguments		: ADC - Valor digital del canal analogo 31 (0-1023)
 * Return Value     : Valor flotante correspondiente a corriente en [mA] arreglo -Z
 *----------------------------------------------------------------------------------------*/
#define EPS_ArrminuZCurr(ADC) (-0.4966*(double)(ADC)+512.6627)

// toopazo: debe haber una funcioncion de init, aunque sea vacia, debe al menos
// decir si la EPS responde (isALive)
int eps_initialize(void);

void setStateFlagEPS(unsigned char value);

void SendCommandEPS(unsigned char CMDName, char CMDVal);

unsigned int ADCReadEPS(unsigned char chann);

unsigned int StatusReadEPS(void);

unsigned int VersionReadEPS(void);

void PdmOffEPS(char bus);

void HeaterOffEPS(void);

void WatchdogEPS(void);

//unsigned char ReadBattLoad(void);

unsigned char CurrIDEPS(unsigned int ADCcurr, const int *CSOC);

unsigned char TempIDEPS(unsigned int ADCtemp, const int *TSOC);

unsigned char VoltIDEPS(unsigned int ADCvolt, const int *VSOCfrac);

void BatMessuEPS(void);

void BusCurrEPS(void);

unsigned int pnlPwrEPS(void);

void PnlMesEPS(void);

void updateStatusEPS(void);

char * SOCfromLUT(void);

void updateSOCEPS(void);

void updateENERGYEPS(void);

int eps_read_vars(char var2read);

char CurrDirEstEPS(void);

#endif
