/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2012, Francisco Reyes Aspe, komodotas@gmail.com
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

#include "eps_suchai.h"


static EPS_Bat_mes BatMes;
static EPS_Bus_curr BusCurr;
static EPS_pnl_mes PnlMes;
static EPS_st EPSstate;

int firstmesu=1;

/* LUT con la correspondencia de voltaje a ID de Wh disponible
        Matriz 3D VSOC[z][y][x] con z corriente, y voltaje, x temperatura*/

const int EPS_VSOC[EPS_Cvsocdim][EPS_Tvsocdim][EPS_Vvsocdim] ={
{{162,174,197,205,218,231,236,241,250},
{171,184,200,213,224,233,239,243,246},
{164,182,201,212,224,233,238,243,247},
{161,182,202,212,222,233,238,243,248},
{156,180,200,209,222,233,239,244,249},
{0,181,199,213,225,236,243,246,250},
{0,0,0,0,242,262,276,290,305}},

{{160,180,197,210,219,231,236,243,246},
{160,180,197,210,219,231,236,243,246},
{156,179,196,208,219,232,239,244,249},
{0,177,192,208,221,232,240,246,251},
{0,172,190,209,222,233,242,247,253},
{0,169,190,213,226,238,245,252,257},
{0,0,0,0,0,258,281,299,316}},

{{0,161,179,198,215,229,239,247,253},
{0,161,179,198,215,229,239,247,253},
{0,0,183,202,221,234,246,255,263},
{0,0,182,206,223,236,249,260,271},
{0,0,0,204,227,242,255,266,276},
{0,0,0,213,241,257,268,282,292},
{0,0,0,0,0,0,0,0,316}},

{{0,0,172,192,213,228,239,249,256},
{0,0,179,198,220,234,246,255,262},
{0,0,163,189,210,228,242,254,263},
{0,0,0,179,210,232,247,260,272},
{0,0,0,0,197,227,247,261,275},
{0,0,0,0,0,227,256,273,287},
{0,0,0,0,0,0,0,0,0}},

{{0,0,176,198,219,233,244,254,260},
{0,0,189,211,232,246,257,267,273},
{0,0,0,196,217,236,250,262,272},
{0,0,0,0,213,236,253,267,280},
{0,0,0,0,0,225,250,267,281},
{0,0,0,0,0,213,257,282,298},
{0,0,0,0,0,0,0,0,0}},

{{0,0,184,205,226,240,252,263,269},
{0,0,184,210,226,240,252,263,270},
{0,0,0,198,227,247,262,275,287},
{0,0,0,0,211,244,265,281,296},
{0,0,0,0,0,225,261,283,300},
{0,0,0,0,0,0,235,295,318},
{0,0,0,0,0,0,0,0,0}},

{{0,0,178,208,229,244,256,267,274},
{0,0,178,209,230,244,257,267,274},
{0,0,0,190,226,248,263,277,289},
{0,0,0,0,0,229,256,275,292},
{0,0,0,0,0,0,230,269,295},
{0,0,0,0,0,0,0,278,311},
{0,0,0,0,0,0,0,0,0}},

{{0,0,181,211,233,248,260,271,278},
{0,0,181,213,234,249,261,271,278},
{0,0,0,0,218,242,261,276,290},
{0,0,0,0,0,0,234,263,285},
{0,0,0,0,0,0,0,250,283},
{0,0,0,0,0,0,0,0,296},
{0,0,0,0,0,0,0,0,0}},

{{0,0,0,209,231,248,262,272,281},
{0,0,0,211,233,249,263,273,281},
{0,0,0,0,210,242,262,277,289},
{0,0,0,0,0,0,0,248,275},
{0,0,0,0,0,0,0,0,267},
{0,0,0,0,0,0,0,0,255},
{0,0,0,0,0,0,0,0,0}},

{{0,0,0,218,242,261,276,287,300},
{0,0,0,212,242,265,279,289,298},
{0,0,0,0,0,252,275,291,304},
{0,0,0,0,0,0,0,0,245},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0}	}
			};




/*valor ADC de c/15,c/10,c/5,0.375A,0.5A,c/2,0.75A,0.875A,1A,c*/
const int EPS_CSOC[EPS_Cvsocdim] = {889,876,837,798,759,720,681,642,602,524};

/*valor ADC de t? 40,20,15,10,5,0(enverdad -1? para evitar saltos),-20*/
const int EPS_TSOC[EPS_Tvsocdim] = {432,555,585,616,647,684,800};


int eps_initialize(void){
    //nothing to do

    return eps_isAlive(FALSE);
}
int eps_isAlive(BOOL verbose){
    
    int ver = VersionReadEPS();

    if( verbose == TRUE ){
        //    printf("\r\n*********************************\n");
        printf("eps_isAlive, version = %d \r\n", ver);
        //    printf("*********************************\n");
    }

    int version = (258);
    if( ver==version ){
        return 1;
    }
    else{
        return 0;
    }
}

/*------------------------------------------------------------------------------
 *				Initializate EPS
 *------------------------------------------------------------------------------
 * Function Prototype : void initEPS(void)
 * Include            : none
 * Description        : Set the initial values of the data structure
 * Arguments          : None
 * Return Value      :  None
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void initEPS(void){
    BatMes.Current = 0;
    BatMes.Temp = 0;
    BatMes.Voltage = 0;

    BusCurr.bus5V = 0;
    BusCurr.bus3V = 0;
    BusCurr.busBat = 0;

    PnlMes.MXCurr = 0;
    PnlMes.mXCurr = 0;
    PnlMes.MYCurr = 0;
    PnlMes.mYCurr = 0;
    PnlMes.MZCurr = 0;
    PnlMes.mZCurr = 0;
    PnlMes.Xvolt = 0;
    PnlMes.Yvolt = 0;
    PnlMes.Zvolt = 0;
    PnlMes.pwr = 0;
    
    EPSstate.soc = 0;
    EPSstate.socss = 0;
    EPSstate.state_flag = 1;
    EPSstate.status =0;
}

/*------------------------------------------------------------------------------
 *			Set State Flag
 *------------------------------------------------------------------------------
 * Function Prototype : void initEPS(unsigner char value)
 * Include            : none
 * Description        : Set the value of the state flag
 * Arguments          : None
 * Return Value      :  None
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void setStateFlagEPS(unsigned char value){

    EPSstate.state_flag = value;
    
}

/*------------------------------------------------------------------------------------------
 *				Send Command EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : void SendCommandEPS(unsigned char CMDName, unsigned char CMDVal)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Send a Command and the value for the EPS CS-1UEPS2-NB-10
 * Arguments          : CMDName - This is the command name according User Manual
				#define adc_EPS 0
				#define status_EPS 1
				#define pdmoff_EPS 2
				#define version_EPS 4
				#define heater_EPS 5
				#define watchdog_EPS 128
 *			CMDVal - This is the command value
 * Return Value      :  None
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

void SendCommandEPS(unsigned char CMDName, char CMDVal){

	//I2C1SendChar(0x5A, CMDName, &CMDVal, 1);
        char address[] = {EPS_ADDR, CMDName};
        i2c1_master_fputs(&CMDVal, 1, address, 2);
}

/*------------------------------------------------------------------------------------------
 *				Read ADC EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int ADCReadEPS(unsigned char chann)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Read the out value of a ADC channel
 * Arguments          : chann - Channel to read (0-31)					
 * Return Value      :  Digital value of the channel (0-1023)
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

unsigned int ADCReadEPS(unsigned char chann){

	unsigned int ADCVal;
	char buff[2];
        char address[] = {EPS_ADDR, 0x00};

	SendCommandEPS(EPS_adc, chann);
	
        unsigned int i;
	do{
            for(i=0x6000;i>0;i--); //dealay

            //Solo se envia el ID del dispositivo
            //I2C1ReadChar(0x00, 0x00, 0x5B, buff, 2,I2C_MODE_EPS);
            i2c1_master_fgets(buff, 2, address, 1);
            ADCVal = (0x0000 | buff[0])<<8 | buff[1];
	}
	while(ADCVal==0xF000);//mientras se devuelva F000 chann no esta listo

        ADCVal &= 0x3FF;//deja solo los 10 bits menos significativos

	return ADCVal;
}

/*------------------------------------------------------------------------------------------
 *				Read STATUS EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int StatusReadEPS()
 * Include            : i2c.h - i2c_comm.h
 * Description        : Read the out value of the status bytes
 * Arguments          : None					
 * Return Value      :  Digital value of status bytes
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

unsigned int StatusReadEPS(void){

	unsigned int StatusVal;
	char buff[2];
        unsigned int i;
        char address[] = {EPS_ADDR, 0x00};

	SendCommandEPS(EPS_status, 0x00);

        for(i=0x6000;i>0;i--);//delay
        
	//Solo se envia el ID del dispositivo
        //Status responde inmediatamente
	//I2C1ReadChar(0x00, 0x00, 0x5B, buff, 2,I2C_MODE_EPS);
        i2c1_master_fgets(buff, 2, address, 1);

	StatusVal = (0x0000 | buff[0])<<8 | buff[1];
	return StatusVal;
}

/*------------------------------------------------------------------------------------------
 *				Read STATUS EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int StatusReadEPS()
 * Include            : i2c.h - i2c_comm.h
 * Description        : Read the out value of the status bytes
 * Arguments          : None
 * Return Value      :  Digital value of status bytes
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

unsigned int VersionReadEPS(void){

	unsigned int VerVal;
	char buff[2];
        char address[] = {EPS_ADDR, 0x00};

	SendCommandEPS(EPS_version, 0x00);

        unsigned int i;
        for(i=0x6000;i>0;i--);//delay

	//Solo se envia el ID del dispositivo
        //Status responde inmediatamente
	//I2C1ReadChar(0x00, 0x00, 0x5B, buff, 2,1);
        i2c1_master_fgets(buff, 2, address, 1);

	VerVal = (0x0000 | buff[0])<<8 | buff[1];
	return VerVal;
}

/*------------------------------------------------------------------------------------------
 *				Turn off PDM EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : void PdmOffEPS(char bus)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Turn of a EPS bus for a short period of time
 * Arguments          : bus - value range 0-7. 0 correspond to the battery bus
 *                            1 to the 5V bus and 2 to the 3.3V bus. Any combination
 *                            these could be turned off.
 * Return Value      :  none
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

void PdmOffEPS(char bus){

	SendCommandEPS(EPS_pdmoff, bus);
}

/*------------------------------------------------------------------------------------------
 *				Turn off Heater EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : void HeaterOffEPS(void)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Turn off the battery heater
 * Arguments          : none
 * Return Value      :  none
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

void HeaterOffEPS(void){
        /*The 0x01 value means force heater off*/
	SendCommandEPS(EPS_heater, 0x01);
}

/*------------------------------------------------------------------------------------------
 *                              "Turn on" Heater EPS
 *------------------------------------------------------------------------------------------
 * Function Prototype : void HeaterOnEPS(void)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Release the battery heater allowing the controller to
 *                      take control of the heater's state
 * Arguments          : none
 * Return Value      :  none
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

void HeaterOnEPS(void){
        /*The 0x00 value means return heater control to the EPS PIC*/
	SendCommandEPS(EPS_heater, 0x00);
}

/*------------------------------------------------------------------------------------------
 *                              Soft reset of the EPS PIC
 *------------------------------------------------------------------------------------------
 * Function Prototype : void WatchdogEPS(void)
 * Include            : i2c.h - i2c_comm.h
 * Description        : Forces the reset of the I2C module returting it to the
 *                      initial state. Useful when user detects an I2C error.
 * Arguments          : none
 * Return Value      :  none
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------------------*/

void WatchdogEPS(void){
        /*The 0x00 value means return heater control to the EPS PIC*/
	SendCommandEPS(EPS_watchdog, 0x00);
}

///*------------------------------------------------------------------------------------------
// *				Read Battery Load
// *------------------------------------------------------------------------------------------
// * Function Prototype : unsigned char ReadBattLoad()
// * Include            : i2c.h - i2c_comm.h
// * Description        : Calculates the load of the Battery as percentage
// * Arguments          : None
// * Return Value      :  Digital integer value 0-100 (means 0-100 %)
// * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
// *----------------------------------------------------------------------------------------*/
///**/
//unsigned char ReadBattLoad(void){
//
//	unsigned char BattLoad;
//	unsigned int ADCread; //valor entregado por ADC
//	double BattVoltage; //Valor en volts
//        double BattDisCurrent; //corriente por la bateria
//        double BattTemp; //temperatura de bateria
//
//
//	ADCread = ADCReadEPS(24);
//	BattVoltage = EPS_Bat0FullVolt(ADCread);
//
//        ADCread = ADCReadEPS(29);
//	BattDisCurrent = EPS_Bat0Curr(ADCread);
//        ADCread = ADCReadEPS(29);
//	BattDisCurrent += EPS_Bat0Curr(ADCread);
//        ADCread = ADCReadEPS(29);
//	BattDisCurrent += EPS_Bat0Curr(ADCread);
//        BattDisCurrent /= 3;
//
//        ADCread = ADCReadEPS(23);
//	BattTemp = EPS_Bat0Temp(ADCread);
//
//	/* AGREGAR CONVERSION DE VOLTAJE A PORCENTAJE DE CARGA */
///* AGREGAR CONVERSION DE VOLTAJE A PORCENTAJE DE CARGA */
///* AGREGAR CONVERSION DE VOLTAJE A PORCENTAJE DE CARGA */
///* AGREGAR CONVERSION DE VOLTAJE A PORCENTAJE DE CARGA */
//
//	return BattLoad;
//
//}

/*------------------------------------------------------------------------------
 *                          Current ID
 *------------------------------------------------------------------------------
 * Function Prototype : unsigned char CurrIDEPS(unsigned int ADCcurr, const int *CSOC)
 * Include            : none
 * Description        : Return a ID of the current
 * Arguments          : ADCcurr - ADC value of the measured current
 *                      CSOC    - Array of the threshold current values
 * Return Value      :  Return Value      Current
 *                      C/15                0
 *                      c/10                1
 *                      C/5                 2
 *                      C/2                 3
 *                      C                   4
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
unsigned char CurrIDEPS(unsigned int ADCcurr, const int *CSOC){
    unsigned char id = 0;
    int i;

    /*se asume que la corriente es muy baja y por tanto el id inicia en 0,
     correspondiente al indice de la matriz de baja corriente. Se pregunta si la
     corriente es mayor al siguiente valor de umbral que delimita las corrientes
     y si es asi, se suma 1 al id, esto es, se avanza en 1 el indice*/

    for(i=0; i<EPS_Cvsocdim; i++){
        if(ADCcurr < CSOC[i]){ //valor corriente inverso a valor ADC
            id++;
        }
        else{
            i=EPS_Cvsocdim;
        }
    }
    id = id>=EPS_Cvsocdim ? EPS_Cvsocdim-1 : id;
    return id;
}

/*------------------------------------------------------------------------------
 *                          Temperature ID
 *------------------------------------------------------------------------------
 * Function Prototype : unsigned char TempIDEPS(unsigned int ADCtemp, const int *TSOC)
 * Include            : none
 * Description        : Return a ID of the temperature
 * Arguments          : ADCtemp - ADC value of the measured temp
 *                      TSOC    - Array of the threshold temperatures values
 * Return Value      :  Return Value      Temp
 *                      40                  0
 *                      20                  1
 *                      15                  2
 *                      10                  3
 *                      5                   4
 *                      0                   5
 *                      -20                 6
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
unsigned char TempIDEPS(unsigned int ADCtemp, const int *TSOC){
    unsigned char id = 0;
    int i;

    /*El algoritmo es similar al de CurrIDEPS*/

    for(i=0; i<EPS_Tvsocdim; i++){
        if(ADCtemp > TSOC[i]){ //valor temperatura inverso a valor ADC
            id++;
        }
        else{
            i=EPS_Tvsocdim;
        }
    }
    id = id>=EPS_Tvsocdim ? EPS_Tvsocdim-1 : id;
    return id;
}

/*------------------------------------------------------------------------------
 *                          Voltage ID
 *------------------------------------------------------------------------------
 * Function Prototype : unsigned char VoltIDEPS(unsigned int ADCvolt, const int *VSOCfrac)
 * Include            : none
 * Description        : Return a ID of the voltage based on VSOC LUT
 *                      (small ID-> greater voltage)
 * Arguments          : ADCvolt - ADC value of the measured voltage
 *                      VSOCfrac - Array of voltage threshold ADC values
 * Return Value      :  0-(EPS_Vvsocdim-1)
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
unsigned char VoltIDEPS(unsigned int ADCvolt, const int *VSOCfrac){

    unsigned char id = 0;
    int i;

    for(i=0; i<EPS_Vvsocdim; i++){
        if(ADCvolt > VSOCfrac[i]){ //valor voltaje inverso a valor ADC
            id++;
        }
        else{
            i=EPS_Vvsocdim;
        }
    }
    id = id>=EPS_Vvsocdim ? EPS_Vvsocdim-1 : id;
    return id;

}

/*------------------------------------------------------------------------------
 *                          Battery measurements
 *------------------------------------------------------------------------------
 * Function Prototype : void BatMessuEPS(void)
 * Include            : i2c.h
 * Description        : Ask the EPS for the battery measurements using i2c
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void BatMessuEPS(void){
    BatMes.Voltage = ADCReadEPS(24);
    BatMes.Current = ADCReadEPS(29);
    if(firstmesu){
        BatMes.Temp = (ADCReadEPS(23)+ADCReadEPS(23)+ADCReadEPS(23))/3;
        firstmesu=0;
    }
    else{
        BatMes.Temp = (ADCReadEPS(23)+ADCReadEPS(23)+ADCReadEPS(23)
                +BatMes.Temp)/4;
    }
    BatMes.CurrDir = ADCReadEPS(28);
}

/*------------------------------------------------------------------------------
 *                          Busses current measurements
 *------------------------------------------------------------------------------
 * Function Prototype : void BusCurrEPS(void)
 * Include            : i2c.h
 * Description        : Ask the EPS for the busses currents using i2c
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void BusCurrEPS(void){
    BusCurr.busBat = ADCReadEPS(17);
    BusCurr.bus5V = ADCReadEPS(26);
    BusCurr.bus3V = ADCReadEPS(27);
}


/*------------------------------------------------------------------------------
 *                          Panel Power EPS
 *------------------------------------------------------------------------------
 * Function Prototype : unsigned int pnlPwrEPS(void)
 * Include            : none
 * Description        : Return the total power that panels are sourcing based on
 *                      the last panel measurement
 * Arguments          : void
 * Return Value      :  Total panels power [mW] (int)
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
unsigned int pnlPwrEPS(void){

    double pwrAux = 0;

    /*Los "IF" (a?b:c) son para evitar sumar corriente las que producen varias
     decenas de mW de error*/
    pwrAux +=((PnlMes.MYCurr<1023?EPS_ArrplusYCurr(PnlMes.MYCurr):0)
               +(PnlMes.mYCurr<1023?EPS_ArrminuYCurr(PnlMes.mYCurr):0))*
                    EPS_ArrpairYVolt(PnlMes.Yvolt);
    /*El panel -X no va*/
    pwrAux +=((PnlMes.MXCurr<1023?EPS_ArrplusXCurr(PnlMes.MXCurr):0)
                /*+(PnlMes.mXCurr<1023?EPS_ArrminuXCurr(PnlMes.mXCurr):0)*/)*
                    EPS_ArrpairXVolt(PnlMes.Xvolt);
    pwrAux +=((PnlMes.MZCurr<1023?EPS_ArrplusZCurr(PnlMes.MZCurr):0)
                +(PnlMes.mZCurr<1023?EPS_ArrminuZCurr(PnlMes.mZCurr):0))*
                    EPS_ArrpairZVolt(PnlMes.Zvolt);

    return (int)pwrAux;
}

/*------------------------------------------------------------------------------
 *                          Panels meassurements
 *------------------------------------------------------------------------------
 * Function Prototype : void PnlMesEPS(void)
 * Include            : i2c.h
 * Description        : Ask the EPS for the panels meassuremets using i2c
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void PnlMesEPS(void){
    PnlMes.MXCurr = ADCReadEPS(7);
    PnlMes.mXCurr = ADCReadEPS(4);
    PnlMes.MYCurr = ADCReadEPS(1);
    PnlMes.mYCurr = ADCReadEPS(13);
    PnlMes.MZCurr = ADCReadEPS(10);
    PnlMes.mZCurr = ADCReadEPS(31);
    PnlMes.Xvolt = ADCReadEPS(6);
    PnlMes.Yvolt = ADCReadEPS(3);
    PnlMes.Zvolt = ADCReadEPS(9);

    PnlMes.pwr = (int)(pnlPwrEPS()*0.77);//PanelPower*BCReff/1000[mW]
}

/*------------------------------------------------------------------------------
 *                          Update Status
 *------------------------------------------------------------------------------
 * Function Prototype : void updateStatusEPS(void)
 * Include            : i2c.h
 * Description        : update the EPS status asking the EPS for it                 
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void updateStatusEPS(void){
    EPSstate.status = StatusReadEPS();
}

/*------------------------------------------------------------------------------
 *                           SOC from LUT
 *------------------------------------------------------------------------------
 * Function Prototype : char ReadSOCLut(void)
 * Include            : i2c.h
 * Description        : estimate a SOC from the VSOC LUT
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
char * SOCfromLUT(void){
    static char StateID[2];
    StateID[0] = -1;
    StateID[1] = -1;

    unsigned char currIndx, tempIndx, voltIndx;

    BatMessuEPS();
    PnlMesEPS();
    BusCurrEPS();
    
    currIndx = CurrIDEPS(BatMes.Current, EPS_CSOC);
    tempIndx = TempIDEPS(BatMes.Temp, EPS_TSOC);
    voltIndx = VoltIDEPS(BatMes.Voltage, EPS_VSOC[currIndx][tempIndx]);

    /*Si se esta cargando entrega el valor anterior. Si no, solo estimacion
     CurrDirEst estima la direccion de la corriente dado que la medicion es
     muchas veces erronea*/
    EPSstate.currDirEst=CurrDirEstEPS();
    //         Current Dir meassu    || Current Dir estimat || first soc update (init case)
    StateID[0] = (BatMes.CurrDir&0x03ff || EPSstate.currDirEst || EPSstate.soc==0) ? EPS_Vvsocdim - voltIndx: EPSstate.soc;
    //(current is low || First socss update) && temperature >= 0?
    StateID[1] = ((currIndx<=1 || EPSstate.socss==0) && tempIndx<=5) ? EPS_Vvsocdim - voltIndx: EPSstate.socss;

    //StateID += (int)(PnlMes.pwr/1000);

    /*Prueba inicial se realiza sin paneles*/  
    //StateID = EPS_Vvsocdim - voltIndx;
    return StateID;
}

/*------------------------------------------------------------------------------
 *                          Update SOC
 *------------------------------------------------------------------------------
 * Function Prototype : void updateSOCEPS(void)
 * Include            : i2c.h
 * Description        : Update the SOC estimation based on last battery measurements
 *                      (i.e. DO NOT read the battery voltage,current etc).                      
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void updateSOCEPS(void){
    char * StateID;
    StateID = SOCfromLUT();
    EPSstate.soc = StateID[0];
    EPSstate.socss = StateID[1];
    if(EPSstate.socss<EPS_MAX_DOD && EPSstate.state_flag == 2){
        EPSstate.state_flag = 1;
    }
    else
        if(EPSstate.socss>=EPS_SAFE_DOD && EPSstate.state_flag == 1){
            EPSstate.state_flag = 2;
        }
}

/*------------------------------------------------------------------------------
 *                          Update energy
 *------------------------------------------------------------------------------
 * Function Prototype : void updateSOC_PNLEPS(void)
 * Include            : i2c.h
 * Description        : Update available energy estimation.
 *                      This estimation assume the power of the panels as
 *                      constant over time i.e. Wh.
 * Arguments          : void
 * Return Value      :  void
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
void updateENERGYEPS(void){
    updateSOCEPS();
    /*Se asume que los niveles de energia son en intervalos de 500mWh*/
    EPSstate.energy = EPSstate.soc+(PnlMes.pwr%1000)*1;
}

/*------------------------------------------------------------------------------
 *                          Read EPS variables
 *------------------------------------------------------------------------------
 * Function Prototype : int readEPSvars(char var2read)
 * Include            : none
 * Description        : Read the eps_suchai.c data structure variables
 * Arguments          : var2read - ID of variable to read
 * Return Value      :  read variable
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
int readEPSvars(char var2read){
    switch(var2read){
        case EPS_ID_bat0_voltage :
            return BatMes.Voltage;
            break;
        case EPS_ID_bat0_current :
            return BatMes.Current;
            break;
        case EPS_ID_bus5V_current :
            return BusCurr.bus5V;
            break;
        case EPS_ID_bus3V_current :
            return BusCurr.bus3V;
            break;
        case EPS_ID_bus_battery_current :
            return BusCurr.busBat;
            break;
        case EPS_ID_bat0_temp :
            return BatMes.Temp;
            break;
        case EPS_ID_panel_pwr :
            return (int)(PnlMes.pwr/1000);
            break;
        case EPS_ID_status :
            return EPSstate.status;
            break;
        case EPS_ID_current_dir_est :
            return EPSstate.currDirEst;
            break;
        case EPS_ID_current_dir :
            return BatMes.CurrDir;
            break;
        case EPS_ID_soc :
            return EPSstate.soc;
            break;
        case EPS_ID_socss :
            return EPSstate.socss;
            break;
        case EPS_ID_state_flag :
            return EPSstate.state_flag;
            break;
        default :
            return 0;
            break;
    }
}

/*------------------------------------------------------------------------------
 *                          Current Direction Estimation
 *------------------------------------------------------------------------------
 * Function Prototype : char updateSOC_PNLEPS(void)
 * Include            : i2c.h
 * Description        : Estimate the battery current direction based on input
 *                      power (panels), fly swich mode consumption and output
 *                      power (through buses)
 * Arguments          : void
 * Return Value      :  char - as boolean (0-1)
 *                          0 : Charge
 *                          1 : Discharge
 * Remarks           :  Designed for EPS CS-1UEPS2-NB-10
 *----------------------------------------------------------------------------*/
char CurrDirEstEPS(void){
    /*outputPwr its the "Upwind" power of PCM output i.e. includes lowest PCM
     *  efficiency (lowest to avoid worst cases)*/
    int outputPwr = (BusCurr.bus3V<1023?EPS_V33BusCurr(BusCurr.bus3V):0)*3.3*0.94
                    +(BusCurr.bus5V<1023?EPS_V5BusCurr(BusCurr.bus5V):0)*5.0*0.95
                    +(BusCurr.busBat<1023?EPS_BattBusCurr(BusCurr.busBat):0)*BatMes.Voltage*0.985;

    /*input power > output power plus flying swich mode consumption [mW]
     input power has been over estimated and output power underestimated to avoid
     the the bad case (say discharge when is charging)*/
    if((PnlMes.pwr/0.77)*0.8 > outputPwr+160)
        return 0;
    else
        return 1;
}

//struct consumos{
//	unsigned int tranciver;
//	unsigned int procesador;
//	unsigned int beacon;
//	unsigned int gps;
//	unsigned int camara;
//	unsigned int payload1;
//	unsigned int payload2;
//	//etc.
//};


