/*
*	CUBESAT - SUCHAI
*	PIC24FJ256GA110
*	RTCC TEST
*	
*	Nota:	Se inicializa el modulo RTCC con RtccInit, solo luego de esto, establece fecha y hora con algunas
*			de las funciones (ej. RtccWT, RtccWD, RtccWTD). Tambien es posible establecer la alarma (ej RtccWAT)
*			sin haberla configurado. Para configurar la alarma se debe considerar las opciones de Chime, que
			repite la alarma indefinidas veces a la hora y fecha establecida. Para repetir la alarma se usan 
			mascaras que son palabras fijas por ejemplo RTCC_RPT_DAY que indica que la alarma se repite 1 vez
			al dia, o sea, solo interesa la hora y no la fecha de la alarma (recordar que para repetirse Chime
			debe estar activado o cantidad de repeticiones mayor a 0). Si Chime esta desactivado, la alarma
			se repite un numero dado de veces lo cual se debe espesificar. Todo esto se configura cuando se
			desee con RtccInitAlrm(BOOL chime, rtccRepeat rpt, unsigned char rptCnt), ademas activa la alarma.
			Para establecer las interrupciones del RTCC usar 	mRtccSetIntPriority(X);	_RTCIF = 0;	mRtccSetInt(Y);
			con X valor de la prioridad, e Y 1 si se desea activar interrupciones o 0 desactivar.
	Nota:	Para calibrar el RTCC, se debe llamar RtccInitCal(), y ademas, la funcion RtccCal() DEBE existir
			al principio de la rutina de interrupciones del RTCC.
	Nota:	Para desactivar la alarma usar mRtccAlrmDisable(). Para desactivar el RTCC usar mRtccOff()
	Nota: 	Para leer tiempo, se debe crear la variable RtccTimeVal tipo rtccTimeDate ("rtccTimeDate RtccTDVal;")
			y leer usando RtccReadTimeDate(&RtccTDVal), guardando asi los valores en RtccTDVal. Siendo leibles como
			rtccTDVal.xxx, siendo xxx year, mday, mon, hour, wday (weekday de 0 a 6), sec, min, siendo todos tipo
			unsigned char. ej:
			unsigned char minutos;
			RtccReadTimeDate(&RtccTDVal);
			minutos = RtccTDVal.min;
*/
void RtccInitCal(void);

void RtccCal(void);

BOOL RtccWT(UINT8 sec,UINT8 min,UINT8 hour);

BOOL RtccWD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year);

BOOL RtccWTD(UINT8 sec,UINT8 min,UINT8 hour, UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year);

BOOL RtccWAT(UINT8 sec,UINT8 min,UINT8 hour);

BOOL RtccWAD(UINT8 wday,UINT8 mday,UINT8 mon, UINT8 year);

BOOL RtccWATD(UINT8 sec,UINT8 min,UINT8 hour, UINT8 wday,UINT8 mday,UINT8 mon);

void RtccInit(BOOL rtccPinEnable, BOOL rtccPinInt);

unsigned int BCDToBin(unsigned int BCD);

unsigned int BinToBCD(unsigned int Bin);

void RtccInitAlrm(BOOL chime, rtccRepeat rpt, unsigned char rptCnt);

//--------------------------------------------------------------------------------------------
// TIMER 2/3 para el OsTimer(); y delay(); de SALVO
#define config_1ms_Timer23	OpenTimer23(T23_ON&T1_IDLE_CON&T23_GATE_OFF&T23_PS_1_64&T23_SOURCE_INT, 250); ConfigIntTimer23(T23_INT_PRIOR_4&T23_INT_ON)
//(1/16)*(10^-6)*(64)*(250)=1000us=1ms entre IRQ
#define config_10ms_Timer23	OpenTimer23(T23_ON&T1_IDLE_CON&T23_GATE_OFF&T23_PS_1_64&T23_SOURCE_INT, 2500); ConfigIntTimer23(T23_INT_PRIOR_4&T23_INT_ON)
//(1/16)*(10^-6)*(64)*(2500)=10ms entre IRQ

/*
void _ISRFAST _T1Interrupt(void){
	// Genero ticks del timer
	OSTimer();
	
	WriteTimer1(0);
	IFS0bits.T1IF = 0;
}
*/
