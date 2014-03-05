#include <Rtcc.h>
#include <timer.h>
#include "rtcc_suchai.h"

unsigned int rtccCalCount = 60; //0 solo se alcanza cuando se inicia la calibracion 
						//y cuenta la cantidad de 
unsigned int rtccAccCount;//Auxiliar para calibracion, Cantidad acumulada
unsigned int rtccOldTmrVal;//Auxiliar para calibracion, valor timer antiguo
unsigned int rtccTmrVal;//Auxiliar para calibracion, valor timer
int rtccError;//Auxiliar para calibracion, error

//inicia el timer1 e indica a travez de rtccfirstint que debe iniciarse la calibracion

/*------------------------------------------------------------------------------------------
 *		 							RtccInitCal
 *------------------------------------------------------------------------------------------
 * Function Prototype : void RtccInitCal(void)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Inicial el algoritmo de configuracion del RTCC, y espera a que se realice
 *						en un loop de la mas alta prioridad lo que tarda alrrededor de 1 minuto.
 * Arguments          : BRG - Computed value for the baud rate generator
 * Return Value       :	None
 * Remarks            : Modifica la mascara de repeticion a 1 minuto si es necesario. Desactiva 
 *						el chime si es necesario. Activa la alarma si es necesario, Activa las int.
 *						Al concluir, reestablece la configuracion inicial de los parametros.
 *						Modifica las variables globales rtccCalCount, rtccAccCount, rtccOldTmrVal,
 *						rtccTmrVal y rtccError.						
 *----------------------------------------------------------------------------------------*/
void RtccInitCal(void){
	
	T1CON = 0x8030;//Inicia timer1, prescalar 256, clk source fosc/2
	
	BOOL oldChimEnDis;//valor inicial de si chime es enable o disable
	rtccRepeat oldAlarmRpt;//valor inicial de la mascara de repeticion de alarma
	BOOL oldAlarmEnDis;// valor inical de si la alarma esta enable o disable
	BOOL oldIntEnDis; // valor inicial de si la interrupciones estan activa so inactivas

	/* Se guardan los valores iniciales que se van a alterar para luego re establecerlos*/
	oldChimEnDis = mRtccGetChimeEnable();
	oldAlarmRpt = mRtccGetAlarmRpt();
	oldAlarmEnDis = mRtccIsAlrmEnabled();
	oldIntEnDis = IEC3bits.RTCIE;
	
	RtccSetChimeEnable(TRUE,TRUE);//activa chime (alarma infinitas veces)
	RtccSetAlarmRpt(RTCC_RPT_SEC,TRUE);	//setea la mascara de la alarma en sec
	mRtccAlrmEnable();//activa alarma

	mRtccSetIntPriority(7);
	_RTCIF = 0;
	mRtccSetInt(1);
	
	rtccOldTmrVal = 0;
	rtccCalCount = 0;
	rtccError = 0;
	
	while(rtccCalCount<60);//Se espera hasta completar la calibracion
	
	/* Se retoman los valores iniciales */
	RtccSetChimeEnable(oldChimEnDis,TRUE);
	RtccSetAlarmRpt(oldAlarmRpt,TRUE);
	if(!oldAlarmEnDis)
		mRtccAlrmDisable();
	mRtccSetInt(oldIntEnDis);	

}

/*------------------------------------------------------------------------------------------
 *		 							RtccCal
 *------------------------------------------------------------------------------------------
 * Function Prototype : void RtccCal(void)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Realiza el algoritmo de calibracion si esque este es activado por
 *						RtccInitCal y si RtccCal es llamada al comienzo de la rutina de int
 *						de RTCC.
 * Arguments          : None
 * Return Value       :	None
 * Remarks            : Modifica el valor de Timer1. Modifica el registro de calibracion de RTCC
 *						Modifica variables globales rtccCalCount, rtccAccCount, rtccOldTmrVal,
 *						rtccTmrVal y rtccError.
 *----------------------------------------------------------------------------------------*/
void RtccCal(void){
	if(rtccCalCount==0)
		TMR1 = 0;
	else
		if(rtccCalCount<60){
			/*leer timer acumulado en 1 segundo (interrupcion cada 1 sec)*/
			rtccTmrVal = TMR1;
			rtccAccCount = rtccTmrVal - rtccOldTmrVal;
			rtccOldTmrVal = rtccTmrVal;
			
			/*Computar el error*/
			rtccError += ((65536-rtccAccCount)>>1);///2 => comparable con reloj
			
			if(rtccCalCount == 59){//si ha pasado 1 minuto
				/*El valor de calibracion es error/4, ademas, se desea promediar
				el valor sumatoria del error (error/2). Luego el valor a escribir
				en el registro de calibracion es rtccError/8*/
				RtccSetCalibration(rtccError>>3);			
			}
		}
	if(rtccCalCount<60)
		++rtccCalCount;			
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWT
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWT(UINT8 sec,UINT8 min,UINT8 hour)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe el tiempo de horas minutos y segundos en formato BCD del RTCC.
 * Arguments          : sec	-	Segundos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						min	-	Minutos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						hour-	Horas (en formato BCD, valor mayor o igual a 0 y menor a 24)
 * Return Value       :	TRUE-	Bit de valor 1 si el tiempo fue escrito satisfactoriamente
 *						FALSE-	Bit de valor 0 si el tiempo tiene un formato incorrecto
 * Remarks            : Modifica los registros de segundos, minutos y horas.
 *----------------------------------------------------------------------------------------*/

BOOL RtccWT(UINT8 sec,UINT8 min,UINT8 hour){
	rtccTime RT;
	RT.f.sec = sec;
	RT.f.min = min;
	RT.f.hour = hour;

	return RtccWriteTime(&RT, FALSE);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWD
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe (establece) la fecha del RTCC
 * Arguments          : wday-	Dia de la semana (valor de 0 a 6, lunes a domingo)
 *						mday-	Dia del mes (en formato BCD, valor mayor o igual a 1 y menor a 31)
 *						mon-	Horas (en formato BCD, valor mayor o igual a 1 y menor a 12)
 *						year-	En formato BCD valor mayor igual a 0 y menor a 99
 * Return Value       :	TRUE-	Bit de valor 1 si la fecha fue escrita satisfactoriamente
 *						FALSE-	Bit de valor 0 si la fecha tiene un formato incorrecto
 * Remarks            : Modifica los registros de weekday, month day, month y year.
 *----------------------------------------------------------------------------------------*/
 
BOOL RtccWD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year){
	rtccDate RT;
	RT.f.wday = wday;
	RT.f.mday = mday;
	RT.f.mon = mon;
	RT.f.year = year;

	return RtccWriteDate(&RT, FALSE);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWTD
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWTD(UINT8 sec,UINT8 min,UINT8 hour,
 *						UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe (establece) la fecha y hora del RTCC
 * Arguments          : sec	-	Segundos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						min	-	Minutos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						hour-	Horas (en formato BCD, valor mayor o igual a 0 y menor a 24)
 *						wday-	Dia de la semana (valor de 0 a 6, lunes a domingo)
 *						mday-	Dia del mes (en formato BCD, valor mayor o igual a 1 y menor a 31)
 *						mon-	Horas (en formato BCD, valor mayor o igual a 1 y menor a 12)
 *						year-	En formato BCD valor mayor igual a 0 y menor a 99
 * Return Value       :	TRUE-	Bit de valor 1 si la fecha fue escrita satisfactoriamente
 *						FALSE-	Bit de valor 0 si la fecha tiene un formato incorrecto
 * Remarks            : Modifica los registros de weekday, month day, month y year.
 *----------------------------------------------------------------------------------------*/

BOOL RtccWTD(UINT8 sec,UINT8 min,UINT8 hour,
			UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year){
	rtccTimeDate RT;
	RT.f.wday = wday;
	RT.f.mday = mday;
	RT.f.mon = mon;
	RT.f.year = year;
	RT.f.sec = sec;
	RT.f.min = min;
	RT.f.hour = hour;

	return RtccWriteTimeDate(&RT, FALSE);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWAT
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWAT(UINT8 sec,UINT8 min,UINT8 hour)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe la hora de la alarma BCD del RTCC.
 * Arguments          : sec	-	Segundos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						min	-	Minutos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						hour-	Horas (en formato BCD, valor mayor o igual a 0 y menor a 24)
 * Return Value       :	TRUE-	Bit de valor 1 si el tiempo fue escrito satisfactoriamente
 *						FALSE-	Bit de valor 0 si el tiempo tiene un formato incorrecto
 * Remarks            : Modifica los registros de alarma de segundos, minutos y horas.
 *----------------------------------------------------------------------------------------*/
BOOL RtccWAT(UINT8 sec,UINT8 min,UINT8 hour){
	rtccTime RT;
	RT.f.sec = sec;
	RT.f.min = min;
	RT.f.hour = hour;

	return RtccWriteAlrmTime(&RT);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWAD
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWAD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe (establece) la fecha de la alarma del RTCC
 * Arguments          : wday-	Dia de la semana (valor de 0 a 6, lunes a domingo)
 *						mday-	Dia del mes (en formato BCD, valor mayor o igual a 1 y menor a 31)
 *						mon-	Horas (en formato BCD, valor mayor o igual a 1 y menor a 12)
 *						year-	En formato BCD valor mayor igual a 0 y menor a 99
 * Return Value       :	TRUE-	Bit de valor 1 si la fecha fue escrita satisfactoriamente
 *						FALSE-	Bit de valor 0 si la fecha tiene un formato incorrecto
 * Remarks            : Modifica los registros de alarma de weekday, month day, month y year.
 *----------------------------------------------------------------------------------------*/
BOOL RtccWAD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year){
	rtccDate RT;
	RT.f.wday = wday;
	RT.f.mday = mday;
	RT.f.mon = mon;
	RT.f.year = year;

	return RtccWriteAlrmDate(&RT);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccWATD
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccWATD(UINT8 sec,UINT8 min,UINT8 hour,
 *						UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Escribe (establece) la fecha y hora la alarma del RTCC
 * Arguments          : sec	-	Segundos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						min	-	Minutos (en formato BCD, valor mayor o igual a 0 y menor a 60)
 *						hour-	Horas (en formato BCD, valor mayor o igual a 0 y menor a 24)
 *						wday-	Dia de la semana (valor de 0 a 6, lunes a domingo)
 *						mday-	Dia del mes (en formato BCD, valor mayor o igual a 1 y menor a 31)
 *						mon-	Horas (en formato BCD, valor mayor o igual a 1 y menor a 12)
 *						year-	En formato BCD valor mayor igual a 0 y menor a 99
 * Return Value       :	TRUE-	Bit de valor 1 si la fecha fue escrita satisfactoriamente
 *						FALSE-	Bit de valor 0 si la fecha tiene un formato incorrecto
 * Remarks            : Modifica los registros de weekday, month day, month y year.
 *----------------------------------------------------------------------------------------*/
BOOL RtccWATD(UINT8 sec,UINT8 min,UINT8 hour,
			UINT8 wday,UINT8 mday,UINT8 mon){
	rtccTimeDate RT;
	RT.f.wday = wday;
	RT.f.mday = mday;
	RT.f.mon = mon;	
	RT.f.sec = sec;
	RT.f.min = min;
	RT.f.hour = hour;

	return RtccWriteAlrmTimeDate(&RT);
}

/*------------------------------------------------------------------------------------------
 *		 							RtccInit
 *------------------------------------------------------------------------------------------
 * Function Prototype : BOOL RtccInit(BOOL rtccPinEnable, BOOL rtccPinInt)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Inicia el modulo RTCC y establece las salidas
 * Arguments          : rtccPinEnable	-	TRUE establece pin 68  como salida.
 *						rtccPinInt	-	TRUE establece que el pin cambios cuando ocurra alarma.
										FALSE establece que el pin cambia cada segundo.						
 * Return Value       :	None
 * Remarks            : 
 *----------------------------------------------------------------------------------------*/
  
void RtccInit(BOOL rtccPinEnable, BOOL rtccPinInt){	
	RtccInitClock(); //turn on clock source  
	RtccWrOn(); //enable RTCC peripheral
	if(rtccPinEnable)
		mRtccSetClockOe(1);
	if(!rtccPinInt)
		_RTSECSEL = 1;
	mRtccOn();
}

/*------------------------------------------------------------------------------------------
 *		 							RtccInitAlrm
 *------------------------------------------------------------------------------------------
 * Function Prototype : void RtccInitAlrm(BOOL chime, rtccRepeat rpt, unsigned char rptCnt)
 * Include            : rtcc.h - rtcc_suchai.h
 * Description        : Inicia y configura la Alarma del RTCC
 * Arguments          : chime	-	TRUE se activa chime FALSE se desactiva	
 *						rpt		-	Cada cuanto se puede repetir la alarma
									Valores posibles:	RTCC_RPT_HALF_SEC,
														RTCC_RPT_SEC,
														RTCC_RPT_TEN_SEC,
														RTCC_RPT_MIN,
														RTCC_RPT_TEN_MIN,
														RTCC_RPT_HOUR,
														RTCC_RPT_DAY,
														RTCC_RPT_WEEK,
														RTCC_RPT_MON,
														RTCC_RPT_YEAR.
						rptCnt	-	En caso de que chime este desactivado, cuantas repeticiones
									deben hacerse de la alarma.
 * Return Value       :	None
 * Remarks            : 
 *----------------------------------------------------------------------------------------*/

void RtccInitAlrm(BOOL chime, rtccRepeat rpt, unsigned char rptCnt){
	RtccSetChimeEnable(chime,TRUE);
	RtccSetAlarmRpt(rpt,TRUE);
	RtccSetAlarmRptCount(rptCnt, TRUE);
	mRtccAlrmEnable();
}
/*------------------------------------------------------------------------------------------
 *		 							BCDToBin
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int BCDToBin(unsigned int BCD)
 * Include            : None
 * Description        : Convierte un entero positivo en BCD en entero positivo en binario puro
 * Arguments          : BCD	-	valor BCD mayor igual a 0 y menor a 9999											
 * Return Value       :	entero de igual valor a BCD pero en formato binario puro
 * Remarks            : 
 *----------------------------------------------------------------------------------------*/

unsigned int BCDToBin(unsigned int BCD){	
	return (BCD&0xF000)*1000+(BCD&0x0F00)*100+(BCD&0x00F0)*10+(BCD&0x000F);
}

/*------------------------------------------------------------------------------------------
 *		 							BinToBCD
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int BinToBCD(unsigned int Bin)
 * Include            : None
 * Description        : Convierte un entero positivo binario en entero positivo en BCD
 * Arguments          : Bin	-	valor binario mayor igual a 0 y menor a 9999											
 * Return Value       :	entero de igual valor a Bin pero en formato BCD
 * Remarks            : 
 *----------------------------------------------------------------------------------------*/
unsigned int BinToBCD(unsigned int Bin){	
	return ((Bin/1000)<<12)|(((Bin/100)%10)<<8)|(((Bin/10)%10)<<4)|(Bin%10);
}




