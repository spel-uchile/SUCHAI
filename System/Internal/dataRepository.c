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
    int DAT_CUBESAT_VAR_BUFF[dat_cubesatVar_last_one];
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
    dat_memSD_BlockErase(dat_Trx_Tc_1Block);
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
        dat_memSD_BlockErase(dat_FlightPlan_256Block+i);
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
 * Funcion para modificar una variable de estado
 * @param indxVar. Variable de estado que quiero modificar
 * @param value. Valor a asignar a la variable de estado
 */
void dat_setCubesatVar(DAT_CubesatVar indxVar, int value){
    portBASE_TYPE semStatus = xSemaphoreTake( dataRepositorySem, portMAX_DELAY );

    #if !SCH_SYSBUS_ONBOARD
        //Para el caso de guardar las variables en memoria interna
        DAT_CUBESAT_VAR_BUFF[indxVar] = value;
    #else
        //Para el caso de guardar las variables en la memI2C
        writeIntEEPROM1( (unsigned char)indxVar, value);
    #endif

    semStatus = xSemaphoreGive(dataRepositorySem);
}
/**
 * Funcion para obtener una variable de estado
 * @param indxVar. Variable de estado que quiero modificar
 * @param value. Valor a asignar a la variable de estado
 */
int dat_getCubesatVar(DAT_CubesatVar indxVar){
    portBASE_TYPE semStatus = xSemaphoreTake( dataRepositorySem, portMAX_DELAY );
    
    int value;
    #if !SCH_SYSBUS_ONBOARD
        //Para el caso de obtener las variables de la memoria interna
        value = DAT_CUBESAT_VAR_BUFF[indxVar];
    #else
        //Para el caso de obtener las variables de la memI2C
        value = readIntEEPROM1( (unsigned char)indxVar );
    #endif

    semStatus = xSemaphoreGive(dataRepositorySem);

    return value;
}

/**
 * Funcion a llamar luego de un Reset del PIC, y antes de usar la memEEPROM.
 */
void dat_onReset_memEEPROM(void){
    //nothing to do here
}

/**
 * Funcion a llamar luego de un Reset del PIC y luego de @sa dat_onReset_memEEPROM
 * para inicializar a los valore correctos las variables de estado que necesitan ser modificadas.
 *
 *Funcion pensada para sersiorarse que luego del reseteo se tengan valores coherentes.
 *Se inicializan variables que explicitamente necesitan ser modificadas
 */
void dat_onResetCubesatVar(void)
{
    // dat_ppc_opMode siempre parte en RSSI (por ahora)
    dat_setCubesatVar(dat_ppc_opMode, DAT_PPC_OPMODE_RSSI);    
    // lee, guarda e imprime razon del reset
    dat_init_ppc_lastResetSource();
    //reseta la variable, pues hubo un reset
    dat_setCubesatVar(dat_ppc_hoursWithoutReset, 0x0000);
    //resetCounter No debe inicializarse luego de un reset
    //Su valor debe ser traido de la memEEPROM y modificado
    if( dat_getCubesatVar(dat_ppc_resetCounter) == 0xFFFF ){
        dat_setCubesatVar(dat_ppc_resetCounter, 0);
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
            con_printf("        - First time on! Setting resetCounter to 0\r\n");
        #endif
    }
    else{
        int rc=dat_getCubesatVar(dat_ppc_resetCounter) + 1;
        dat_setCubesatVar(dat_ppc_resetCounter, rc);
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
            char ret[10];
            itoa(ret, (unsigned int)rc, 10);
            con_printf("            - NOT the First time on! resetCounter++\r\n");
            con_printf("            - resetCounter = "); con_printf(ret); con_printf("\r\n");
        #endif
    }
    dat_setCubesatVar(dat_ppc_enwdt, PPC_INITIAL_WDT_STATE);

    dat_setCubesatVar(dat_ppc_MB_nOE_USB_nINT_stat, PPC_MB_nOE_USB_nINT_CHECK);
    dat_setCubesatVar(dat_ppc_MB_nOE_MHX_stat, PPC_MB_nOE_MHX_CHECK);
    dat_setCubesatVar(dat_ppc_MB_nON_MHX_stat, PPC_MB_nON_MHX_CHECK);
    dat_setCubesatVar(dat_ppc_MB_nON_SD_stat, PPC_MB_nON_SD_CHECK);

    // mDEP_state
    // No  deben inicializarse, su valor debe ser conservado en la memExt y modificado solo en deploy_antenna

    // mRTC_state

    // mEPS_state
    

    // mTRX_state
    dat_setCubesatVar(dat_trx_rssi_mean, 9999);
    dat_setCubesatVar(dat_trx_count_tc, 0);
    dat_setCubesatVar(dat_trx_count_tm, 0);

    //Telecomand Buffer
    dat_setCubesatVar(dat_trx_newCmdBuff, 0x0000);      // Doesn't exist unproccessed TC Frames in TRX
    dat_setCubesatVar(dat_trx_newTcFrame, 0x0000);      // Doues't exist unproccessed TC in internal buffer

    //Flight Plan

    //memSD

    //PAYLOAD
}
/**
 * Actualiza la variable de estado con el origen del ultimo reset
 */
void dat_init_ppc_lastResetSource(void){
    int lreset=PwrMgnt_ResetSource();

    /* NOTA: Por alguna extrana razon, si no ha ocurrido algun RESET PwrMgnt_ResetSource()
     * devuelve 0x0001=BURN_OUT_Reset, pero salta al default !!.  Asi es que para no
     * confundirse aquel caso de RESET seria ignorado y asi la lastResetSource
     * no se actualizaria equivocadamente. He dicho! caso cerrado!
     */

    #if (SCH_DATAREPOSITORY_VERBOSE>=1)

        con_printf("        - LastResetSouce: ");
        switch ( lreset )
        {
            case POWER_ON_Reset:
                con_printf("POWER_ON_Reset\r\n");      /* 0x00 Aun nose cuando salta */
                mPWRMGNT_Clear_PORbit();
            break;
            case BURN_OUT_Reset:
                con_printf("BURN_OUT_Reset\r\n");      /* 0x01 ver nota mas arriba*/
                mPWRMGNT_Clear_BORbit();
            break;
            case WATCHDOG_Reset:
                con_printf("WATCHDOG_Reset\r\n");      /* 0x02 al overflow del WDT, luego de aprox 2 min sin ejecutarse ClrWdt(); */
                mPWRMGNT_Clear_WDTObit();
            break;
            case SOFTWARE_Reset:
                con_printf("SOFTWARE_Reset\r\n");      /* 0x03 ocurre luego de ejecutar ppc_reset() */
                mPWRMGNT_Clear_SWRbit();
            break;
            case EXTERNAL_Reset:
                con_printf("EXTERNAL_Reset\r\n");      /* 0x04 Ocurre cuando se programa y cuando se energiza/desenergiza el PIC*/
                mPWRMGNT_Clear_EXTRbit();
            break;
            case CFG_WORD_MISMATCH_Reset:
                con_printf("CFG_WORD_MISMATCH_Reset\r\n");
                mPWRMGNT_Clear_CMbit();
            break;
            case ILLEGAL_INSTR_Reset:
                con_printf("ILLEGAL_INSTR_Reset\r\n");
                mPWRMGNT_Clear_IOPUWRbit();
            break;
            case TRAP_Reset:
                con_printf("TRAP_Reset\r\n");
                mPWRMGNT_Clear_TRAPRbit();
            break;
            default:
                con_printf("No new RESET\r\n");
                lreset= -1;
                /* ver nota mas arriba */
            break;
        }
    #endif

    //if(lreset!=0x0001)
    //{
        dat_setCubesatVar(dat_ppc_lastResetSource, lreset);
    //}
}
/**
 * Setea la variable dat_pay_xxx_perform a su correspondiente valor segun
 * los defines de @ref SUCHAI_config.h
 */
void dat_reset_pay_i_performVar(void){
    #if (SCH_PAY_LAGMUIR_ONBOARD==1)
        dat_setCubesatVar(dat_pay_lagmuirProbe_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_lagmuirProbe_perform, 0x0000 );
    #endif

    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        dat_setCubesatVar(dat_pay_sensTemp_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_sensTemp_perform, 0x0000 );
    #endif

    #if (SCH_PAY_GPS_ONBOARD==1)
        dat_setCubesatVar(dat_pay_gps_perform, 0x0001);
    #else
        dat_setCubesatVar(dat_pay_gps_perform, 0x0000);
    #endif

    #if (SCH_PAY_FIS_ONBOARD==1)
        dat_setCubesatVar(dat_pay_expFis_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_expFis_perform, 0x0000 );
    #endif

    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        dat_setCubesatVar(dat_pay_camera_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_camera_perform, 0x0000 );
    #endif

    #if (SCH_PAY_GYRO_ONBOARD==1)
        dat_setCubesatVar(dat_pay_gyro_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_gyro_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TMESTADO_ONBOARD==1)
        dat_setCubesatVar(dat_pay_tmEstado_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_tmEstado_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TEST1_ONBOARD==1)
        dat_setCubesatVar(dat_pay_test1_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_test1_perform, 0x0000 );
    #endif

    #if (SCH_PAY_TEST2_ONBOARD==1)
        dat_setCubesatVar(dat_pay_test2_perform, 0x0001 );
    #else
        dat_setCubesatVar(dat_pay_test2_perform, 0x0000 );
    #endif
}

//Payload API
void dat_memSD_BlockErase(unsigned long block_address){
    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        static int ind; char ret[10]; unsigned int ib;
        ind++;
        if(ind==1){ib=block_address;}
        if( (ind%32)==0 ){
            con_printf("    dat_memSD_BlockErase\r\n");
            con_printf("    erasing also.. block=");
            sprintf (ret, "%d", (unsigned int)block_address);
            con_printf(ret); con_printf("\r\n");
        }
        if(ind==256){
            con_printf("    Sucessfully erased 256Block (starting in block ");
            sprintf (ret, "%d", (unsigned int)ib);
            con_printf(ret); con_printf(")\r\n");
            ind=0;
        }
    #endif

    unsigned char buff[512]; int i;
    for(i=0;i<512;i++){buff[i]=0xFF;}
    unsigned char resp;

    resp = Single_Block_Write(block_address, buff);
}
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
        dat_memSD_BlockErase(block+i);
        ClrWdt();
    }
}

/**
 * Asocia el DAT_Payload pay_i a la DAT_CubesatVar que controla la ejecucion o
 * o no de ese Payload
 * @param pay_i DAT_Payload del que quiero obtener el DAT_CubesatVar
 * @return DAT_CubesatVar dat_pay_xxx_perform
 */
DAT_CubesatVar dat_pay_i_to_performVar(DAT_Payload pay_i){
    DAT_CubesatVar dat_pay_xxx_perform;
    
    switch(pay_i){
        case dat_pay_lagmuirProbe:
            dat_pay_xxx_perform = dat_pay_lagmuirProbe_perform;
        break;
        case dat_pay_sensTemp:
            dat_pay_xxx_perform = dat_pay_sensTemp_perform;
        break;
        case dat_pay_gps:
            dat_pay_xxx_perform = dat_pay_gps_perform;
        break;
        case dat_pay_gyro:
            dat_pay_xxx_perform = dat_pay_gyro_perform;
        break;
        case dat_pay_expFis:
            dat_pay_xxx_perform = dat_pay_expFis_perform;
        break;
        case dat_pay_camera:
            dat_pay_xxx_perform = dat_pay_camera_perform;
        break;
        case dat_pay_tmEstado:
            dat_pay_xxx_perform = dat_pay_tmEstado_perform;
        break;
        case dat_pay_test1:
            dat_pay_xxx_perform = dat_pay_test1_perform;
        break;
        case dat_pay_test2:
            dat_pay_xxx_perform = dat_pay_test2_perform;
        break;
        default:
            dat_pay_xxx_perform=-1;
        break;
    }

    return dat_pay_xxx_perform;
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

void dat_onReset_memSD(void){
    con_printf("  dat_onReset_memSD()..\r\n");
    DAT_Payload pay_i; DAT_GnrlPurpBuff gpb_i;

    //calcula y ASIGNA los block para cada estructura
    con_printf("    Static structures..\r\n");
    dat_first_1Block=31;
    dat_Trx_Tc_1Block=dat_first_1Block;
    dat_FlightPlan_256Block=dat_Trx_Tc_1Block+1;
    dat_nextIndx_1Block=dat_FlightPlan_256Block+256;
    dat_maxIndx_1Block=dat_nextIndx_1Block+1;
    #if (SCH_DATAREPOSITORY_VERBOSEMEMSD_>=1)
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
    #endif

    //calcula y ASIGNA los block para cada pay_i
    con_printf("    DAT_Payload()..\r\n");
    
    dat_pay_i_256Block_buff[0]=dat_maxIndx_1Block+256;
    #if (SCH_DATAREPOSITORY_VERBOSEMEMSD_>=1)
        con_printf("    pay_i=0 => 256Block=");
        sprintf (ret, "%d", (unsigned int)( dat_pay_i_256Block_buff[0] ) );
        con_printf(ret); con_printf("\r\n");
    #endif

    for(pay_i=1; pay_i<dat_pay_last_one; pay_i++){
        dat_pay_i_256Block_buff[pay_i]=dat_pay_i_256Block_buff[(pay_i-1)] + 256;

        #if (SCH_DATAREPOSITORY_VERBOSEMEMSD_>=1)
            con_printf("    pay_i=");
            sprintf (ret, "%d", (unsigned int)pay_i);
            con_printf(ret);
            con_printf(" => 256Block=");
            sprintf (ret, "%d", (unsigned int)( dat_pay_i_256Block_buff[pay_i] ) );
            con_printf(ret); con_printf("\r\n");
        #endif
    }

    //calcula y ASIGNA los block para cada gpb_i
    con_printf("    DAT_GnrlPurpBuff()..\r\n");

    dat_gpb_i_256Block_buff[0]=dat_pay_i_256Block_buff[ (dat_pay_last_one-1) ] + 256;
    #if (SCH_DATAREPOSITORY_VERBOSEMEMSD_>=1)
        con_printf("    gpb_i=0 => block=");
        sprintf (ret, "%d", (unsigned int)( dat_gpb_i_256Block_buff[0] ) );
        con_printf(ret); con_printf("\r\n");
    #endif

    for(gpb_i=1; gpb_i<dat_gpb_last_one; gpb_i++){
        dat_gpb_i_256Block_buff[gpb_i]=dat_gpb_i_256Block_buff[(gpb_i-1)] + 256;

        #if (SCH_DATAREPOSITORY_VERBOSEMEMSD_>=1)
            con_printf("    gpb_i=");
            sprintf (ret, "%d", (unsigned int)gpb_i);
            con_printf(ret);
            con_printf(" => block=");
            sprintf (ret, "%d", (unsigned int)( dat_gpb_i_256Block_buff[gpb_i] ) );
            con_printf(ret); con_printf("\r\n");
        #endif
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


