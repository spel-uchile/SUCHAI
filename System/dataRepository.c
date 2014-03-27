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

#include "dataRepository.h"
#include "cmdRTC.h"
#include "cmdPayload.h"
#include "cmdIncludes.h"

#define SCH_DATAREPOSITORY_VERBOSEMEMSD_ 0

//semaforo para sincronizar accceso a las variables de estado
extern xSemaphoreHandle dataRepositorySem; /*WARNING esto estaba asi: extern dataRepositorySem;*/

#if (SCH_DAT_TC_BUFF_MEMSD==0)
    int DAT_CMD_BUFF[DAT_MAX_BUFF_TELECMD];
#endif

#if !SCH_SYSBUS_ONBOARD
    int DAT_CUBESAT_VAR_BUFF[sta_cubesatVar_last_one];
#endif

#if (SCH_FLIGHTPLAN_MEMORY == 0)
    int DAT_FPLAN_BUFF[SCH_FLIGHTPLAN_N_CMD*2];
#endif

//variables y buffers para almacenar los block de memoria de pada estructura a guardar en la memSD
static unsigned int dat_first_1Block;
static unsigned int dat_Trx_Tc_1Block;
static unsigned int dat_FlightPlan_256Block;
static unsigned int dat_nextIndx_1Block;
static unsigned int dat_maxIndx_1Block;
static unsigned int dat_pay_i_256Block_buff[dat_pay_last_one];
static unsigned int dat_gpb_i_256Block_buff[dat_gpb_last_one];


/**
 * Erase de TC internal (memSD) buffer
 */
void dat_erase_TeleCmdBuff(void){

    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        char ret[10];
        con_printf("  dat_erase_TeleCmdBuff()..");
        con_printf("    starting at block=");
        sprintf (ret, "%d", (unsigned int)dat_Trx_Tc_1Block);
        con_printf(ret); con_printf("\r\n");
    #endif
    msd_blockErase(dat_Trx_Tc_1Block);
}

/**
 * Esta funcion retorna un comando desde el fligh plan. Lee desde la SD el
 * i-esimo comando y su parametro.
 * @param index Index-esimo dato del fligh plan */
DispCmd dat_getFlightPlan(unsigned int index)
{
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN;
    NewCmd.param = 0;

    if(index < SCH_FLIGHTPLAN_N_CMD)
    {
        int id, param;

        // La organizacion de los datos en la SD es
        // Primera mitad comandos, segunda mitad parametros

#if (SCH_FLIGHTPLAN_MEMORY == 1)
        msd_getVar_256BlockExtMem(dat_FlightPlan_256Block, index, &id);
        msd_getVar_256BlockExtMem(dat_FlightPlan_256Block, 0xFFFF - index, &param);
#elif (SCH_FLIGHTPLAN_MEMORY == 0)
        id = DAT_FPLAN_BUFF[index];//Comandos
        param = DAT_FPLAN_BUFF[index+SCH_FLIGHTPLAN_N_CMD]; //Parametros
#endif

        NewCmd.cmdId = id;
        NewCmd.param = param;
    }

    return NewCmd;
}

/**
 * Esta funcion escribe un comando en un determinado indice el flight plan
 * @param index Indice del f. plan que sera actualizado
 * @param cmdId Comando que sera escrito
 * @return 1, OK. 0, Fallo
 */
int dat_setFlightPlan_cmd(unsigned int index, unsigned int cmdID)
{
    if(index < SCH_FLIGHTPLAN_N_CMD)
    {
        #if (SCH_FLIGHTPLAN_MEMORY == 1)
            msd_setVar_256BlockExtMem( dat_FlightPlan_256Block, index, cmdID);
        #elif (SCH_FLIGHTPLAN_MEMORY == 0)
            DAT_FPLAN_BUFF[index] = cmdID;
        #endif
        return 1;
    }
    else
        return 0;
}

/**
 * Esta funcion escribe el parametro (asociado a un comando) de un determinado indice el flight plan
 * @param index Indice del f. plan que sera actualizado
 * @param param Parametro que sera escrito
 * @return 1, OK. 0, Fallo
 */
int dat_setFlightPlan_param(unsigned int index, int param)
{
    if(index < SCH_FLIGHTPLAN_N_CMD)
    {
        #if (SCH_FLIGHTPLAN_MEMORY == 1)
            msd_setVar_256BlockExtMem( dat_FlightPlan_256Block, 0xFFFF-index, param);
        #elif (SCH_FLIGHTPLAN_MEMORY == 0)
            DAT_FPLAN_BUFF[index+SCH_FLIGHTPLAN_N_CMD] = param;
        #endif
        return 1;
    }
    else
        return 0;
}

/**
 * Funcion con acciones a seguri sobre el Fligth Plan, luego de un Reset del PIC.
 * @return 1 si OK, 0 si fallo
 */
int dat_onResetFlightPlan(void)
{
    //Do nothing onReset
    unsigned long i; 
    int cmdid, param;

    for(i=0; i < SCH_FLIGHTPLAN_N_CMD; i++)
    {
        cmdid = CMD_CMDNULL;
        param = 0;

        dat_setFlightPlan_cmd(i, cmdid);
        dat_setFlightPlan_param(i, param);
    }

    return 1;
}

/**
 * Borra todo el Fligth Plan
 */
void dat_erase_FlightPlanBuff(void){

#if (SCH_FLIGHTPLAN_MEMORY == 1)
    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        char ret[10];
        con_printf("  dat_erase_FlightPlanBuff()..");
        con_printf("    starting at block=");
        sprintf (ret, "%d", (unsigned int)dat_FlightPlan_256Block);
        con_printf(ret); con_printf("\r\n");
    #endif

    unsigned int i;
    for(i=0;i<256;i++){
        msd_blockErase(dat_FlightPlan_256Block+i);
        ClrWdt();
    }
#elif (SCH_FLIGHTPLAN_MEMORY == 0)
    int i=0;
    for(i=0; i<SCH_FLIGHTPLAN_N_CMD; i++)
    {
        DAT_FPLAN_BUFF[i] = CMD_CMDNULL; //Comandos
        DAT_FPLAN_BUFF[i+SCH_FLIGHTPLAN_N_CMD]=0; //Parametros
    }
#endif
}

/**
 * Obtiene el i-esima valor del buffer de Telecomandos
 * @param indx Posicion del buffer a recuperar
 * @return indx-esima posicion del Buffer
 */
int dat_getTelecmdBuff(int indx){
    int data;
    if(indx>=DAT_MAX_BUFF_TELECMD){
        return 0;
    }
    else{
        #if (SCH_DAT_TC_BUFF_MEMSD==0)
            data = DAT_CMD_BUFF[indx];
        #else
            msd_getVar_1BlockExtMem(dat_Trx_Tc_1Block, indx, &data);
        #endif
        return data;
    }
}
/**
 * Modifica el i-esima valor del buffer de Telecomandos
 * @param indx Posicion del buffer a recuperar
 * @param data Valor de la indx-esima posicion del buffer
 */
void dat_setTelecmdBuff(int indx, int data)
{
    if(indx>=DAT_MAX_BUFF_TELECMD){
        return;
    }
    else{
        #if (SCH_DAT_TC_BUFF_MEMSD==0)
            DAT_CMD_BUFF[indx] = data;
        #else
            msd_setVar_1BlockExtMem(dat_Trx_Tc_1Block, indx, data);
        #endif
    }
}
/**
 * 
 * @return
 */
int dat_onResetTelecmdBuff(void){
    int indx1;
    for(indx1=0; indx1<DAT_MAX_BUFF_TELECMD; indx1++)
    {
        dat_setTelecmdBuff(indx1, CMD_CMDNULL);
    }

    return 1;
}


/**
 * Setea la variable dat_pay_xxx_perform a su correspondiente valor segun
 * los defines de @ref SUCHAI_config.h
 */
void dat_reset_pay_i_performVar(void){
    #if (SCH_PAY_LAGMUIR_ONBOARD==1)
        sta_setCubesatVar(sta_pay_lagmuirProbe_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_lagmuirProbe_perform, 0x0000 );
    #endif

    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        sta_setCubesatVar(sta_pay_sensTemp_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_sensTemp_perform, 0x0000 );
    #endif

    #if (SCH_PAY_GPS_ONBOARD==1)
        sta_setCubesatVar(sta_pay_gps_perform, 0x0001);
    #else
        sta_setCubesatVar(sta_pay_gps_perform, 0x0000);
    #endif

    #if (SCH_PAY_FIS_ONBOARD==1)
        sta_setCubesatVar(sta_pay_expFis_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_expFis_perform, 0x0000 );
    #endif

    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        sta_setCubesatVar(sta_pay_camera_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_camera_perform, 0x0000 );
    #endif

    #if (SCH_PAY_GYRO_ONBOARD==1)
        sta_setCubesatVar(sta_pay_gyro_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_gyro_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TMESTADO_ONBOARD==1)
        sta_setCubesatVar(sta_pay_tmEstado_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_tmEstado_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TEST1_ONBOARD==1)
        sta_setCubesatVar(sta_pay_test1_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_test1_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TEST2_ONBOARD==1)
        sta_setCubesatVar(sta_pay_test2_perform, 0x0001 );
    #else
        sta_setCubesatVar(sta_pay_test2_perform, 0x0000 );
    #endif
}

//Payload API
void dat_erase_pay_i_buff(DAT_Payload pay_i){
    unsigned long i, block;
    block = dat_pay_i_to_block(pay_i);

    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        char ret[10];
        con_printf("  dat_erase_pay_i_buff()..");
        con_printf("    starting at block=");
        sprintf (ret, "%d", (unsigned int)block);
        con_printf(ret); con_printf("\r\n");
    #endif

    for(i=0;i<256;i++){
        msd_blockErase(block+i);
        ClrWdt();
    }
}


/**
 * Asocia el DAT_Payload pay_i a un 256block de la msmSD
 * @param pay_i DAT_Payload del que quiero obtener el 256block
 * @return block de la memSD correspondiente
 */
unsigned long dat_pay_i_to_block(DAT_Payload pay_i){
    unsigned long block;

    block = dat_pay_i_256Block_buff[pay_i];
    return block;
}

//Setea el valor del ultimo/maximo indice del buffer de cierto payload
void dat_setMaxPayIndx(DAT_Payload pay_i, unsigned int maxIndx){
    msd_setVar_1BlockExtMem(dat_maxIndx_1Block, (unsigned char)pay_i, maxIndx);
}

//Obtiene el valor del ultimo/maximo indice del buffer de cierto payload
unsigned int dat_getMaxPayIndx(DAT_Payload pay_i){
    unsigned int maxIndx;
    msd_getVar_1BlockExtMem(dat_maxIndx_1Block, (unsigned char)pay_i, (int *)&maxIndx);
    return maxIndx;
}

//Setea el valor del indice actual del buffer de cierto payload
void dat_setNextPayIndx(DAT_Payload pay_i, unsigned int nextIndx){
    msd_setVar_1BlockExtMem(dat_nextIndx_1Block, (unsigned char)pay_i, nextIndx);
}

//Obtiene el valor del indice actual del buffer de cierto payload
unsigned int dat_getNextPayIndx(DAT_Payload pay_i){
    unsigned int nextIndx;
    msd_getVar_1BlockExtMem(dat_nextIndx_1Block, (unsigned char)pay_i, (int *)&nextIndx);
    return nextIndx;
}

//Retorna FALSE si el buffer se llena, TRUE si todo OK
BOOL dat_setPayloadVar(DAT_Payload pay_i, int value){
    // guarda "value" en la sgte posicion libre del buffer,
    // y retorna si lo logro o no (buffer lleno, payload invalido)
    unsigned int nextIndx;

    if( dat_isFullPayloadBuffer(pay_i)==TRUE){
        return FALSE;    //buffer lleno
    }
    nextIndx = dat_getNextPayIndx(pay_i);

    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        char ret[10];
        con_printf("setPayloadVar [0x");
        itoa(ret, nextIndx, 16); con_printf(ret); con_printf("] = 0x");
        itoa(ret, value, 16); con_printf(ret); con_printf("\r\n");
    #endif

    //guardo el valor de value
    unsigned long block = dat_pay_i_to_block(pay_i);
    msd_setVar_256BlockExtMem(block, nextIndx, value);
    
    //actualizo nextIndx
    dat_setNextPayIndx(pay_i, nextIndx+1);
    
    return TRUE;
}

//Retorna FALSE si el indiex es invalido, TRUE si todo OK
BOOL dat_getPayloadVar(DAT_Payload pay_i, unsigned int indx, int *value){
    unsigned int desiredIndx, maxIndx;

    if(pay_i>=dat_pay_last_one){
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
            con_printf("getPayloadVar: payload invalido\r\n");
        #endif
        return FALSE;   //payload invalido
    }

    desiredIndx = indx;
    maxIndx = dat_getMaxPayIndx(pay_i);
    if(desiredIndx>maxIndx){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            con_printf("getPayloadVar: buffer lleno, desiredIndx>maxIndx\r\n");
        #endif
        return FALSE;   //buffer lleno, indice fuera de rango
    }

    //obtengo el valor de value
    unsigned long block = dat_pay_i_to_block(pay_i);
    msd_getVar_256BlockExtMem(block, desiredIndx, value);

    return TRUE;
}

BOOL dat_isFullPayloadBuffer(DAT_Payload pay_i){
    unsigned int nextIndx, maxIndx;
    
    if(pay_i>=dat_pay_last_one){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            con_printf("isFullPayloadBuffer: payload invalido\r\n");
        #endif
        return FALSE;   //payload invalido
    }

    nextIndx = dat_getNextPayIndx(pay_i);
    maxIndx = dat_getMaxPayIndx(pay_i);
    if(nextIndx>maxIndx){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            con_printf("isFullPayloadBuffer: buffer lleno, desiredIndx>maxIndx\r\n");
        #endif
        return TRUE;   //buffer lleno, indice fuera de rango
    }
    return FALSE;
}

void dat_onReset_memSD(BOOL verbose){
    char ret[10];
    
    if(verbose){
        con_printf("  dat_onReset_memSD()..\r\n");
    }
    DAT_Payload pay_i; DAT_GnrlPurpBuff gpb_i;

    //calcula y ASIGNA los block para cada estructura
    if(verbose){
        con_printf("    Static structures..\r\n");
    }
    dat_first_1Block=31;
    dat_Trx_Tc_1Block=dat_first_1Block;
    dat_FlightPlan_256Block=dat_Trx_Tc_1Block+1;
    dat_nextIndx_1Block=dat_FlightPlan_256Block+256;
    dat_maxIndx_1Block=dat_nextIndx_1Block+1;
    if(verbose){
        char ret[10];
        con_printf("    dat_Trx_Tc_1Block=");
        sprintf (ret, "%d", (unsigned int)( dat_Trx_Tc_1Block ) );
        con_printf(ret); con_printf("\r\n");
        con_printf("    dat_FlightPlan_256Block=");
        sprintf (ret, "%d", (unsigned int)( dat_FlightPlan_256Block ) );
        con_printf(ret); con_printf("\r\n");
        con_printf("    dat_nextIndx_1Block=");
        sprintf (ret, "%d", (unsigned int)( dat_nextIndx_1Block ) );
        con_printf(ret); con_printf("\r\n");
        con_printf("    dat_maxIndx_1Block=");
        sprintf (ret, "%d", (unsigned int)( dat_maxIndx_1Block ) );
        con_printf(ret); con_printf("\r\n");
    }

    //calcula y ASIGNA los block para cada pay_i
    if(verbose){
        con_printf("    DAT_Payload()..\r\n");
    }
    dat_pay_i_256Block_buff[0]=dat_maxIndx_1Block+256;
    if(verbose){
        con_printf("    pay_i=0 => 256Block=");
        sprintf (ret, "%d", (unsigned int)( dat_pay_i_256Block_buff[0] ) );
        con_printf(ret); con_printf("\r\n");
    }

    for(pay_i=1; pay_i<dat_pay_last_one; pay_i++){
        dat_pay_i_256Block_buff[pay_i]=dat_pay_i_256Block_buff[(pay_i-1)] + 256;

        if(verbose){
            con_printf("    pay_i=");
            sprintf (ret, "%d", (unsigned int)pay_i);
            con_printf(ret);
            con_printf(" => 256Block=");
            sprintf (ret, "%d", (unsigned int)( dat_pay_i_256Block_buff[pay_i] ) );
            con_printf(ret); con_printf("\r\n");
        }
    }

    //calcula y ASIGNA los block para cada gpb_i
    if(verbose){
        con_printf("    DAT_GnrlPurpBuff()..\r\n");
    }

    dat_gpb_i_256Block_buff[0]=dat_pay_i_256Block_buff[ (dat_pay_last_one-1) ] + 256;
    if(verbose){
        con_printf("    gpb_i=0 => block=");
        sprintf (ret, "%d", (unsigned int)( dat_gpb_i_256Block_buff[0] ) );
        con_printf(ret); con_printf("\r\n");
    }

    for(gpb_i=1; gpb_i<dat_gpb_last_one; gpb_i++){
        dat_gpb_i_256Block_buff[gpb_i]=dat_gpb_i_256Block_buff[(gpb_i-1)] + 256;

        if(verbose){
            con_printf("    gpb_i=");
            sprintf (ret, "%d", (unsigned int)gpb_i);
            con_printf(ret);
            con_printf(" => block=");
            sprintf (ret, "%d", (unsigned int)( dat_gpb_i_256Block_buff[gpb_i] ) );
            con_printf(ret); con_printf("\r\n");
        }
    }   
}

void dat_onResetPayloadVar(void){

    /* Erase nothing onReset
    DAT_Payload i;
    for(i=0;i<DAT_PAYLOADS_LENGTH;i++){
        dat_setNextPayIndx(i, 0);
        dat_setMaxPayIndx(i, 0x0AFF);
    }
    */
}

void dat_resetPayloadBuffer(DAT_Payload pay_i, unsigned int maxIndx, int mode){
    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        con_printf("Borrando buffer de pay_i=");
        char ret[10];
        //(ret, (unsigned int)pay_i, 16);
        sprintf (ret, "0x%X", (unsigned int)pay_i);
        con_printf(ret);
        con_printf("\r\nUsando mode=");
        if(mode==1){
            con_printf("1 (con borrado)..\r\n");
        }
        else{
                con_printf("0 (sin borrado)..\r\n");
        }
    #endif

    //Seteo limites del Buffer
    dat_setMaxPayIndx(pay_i, maxIndx);
    dat_setNextPayIndx(pay_i, 0);

    if(mode==1){
        //borro el conteido del buffer
        dat_erase_pay_i_buff(pay_i);
    }
    //Reinicio el Buffer
    dat_setNextPayIndx(pay_i, 0);
    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        con_printf("reseteo de payload completo\n");
    #endif
}

/**
 * Asocia el DAT_GnrlPurpBuff gpb_i a un 256block de la msmSD
 * @param gpb_i del que quiero obtener el 256block
 * @return block de la memSD correspondiente
 */
unsigned long dat_gpb_i_to_block(DAT_GnrlPurpBuff gpb_i){
    unsigned long block;

    block = dat_gpb_i_256Block_buff[gpb_i];
    return block;
}

//Set
void dat_setGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx, int value){

    //guardo el valor de value
    unsigned long block = dat_gpb_i_to_block(gpb_i);
    msd_setVar_256BlockExtMem(block, indx, value);

}

//Get 
int dat_getGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx){
    int val;

    //obtengo el valor de value
    unsigned long block = dat_gpb_i_to_block(gpb_i);
    msd_getVar_256BlockExtMem(block, indx, &val);

    return val;
}


