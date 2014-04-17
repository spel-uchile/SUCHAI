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

#if (SCH_TC_BUFF_EXTMEMORY==0 || SCH_SYSBUS_ONBOARD==0)
    int DAT_CMD_BUFF[DAT_MAX_BUFF_TELECMD];
#endif

#if (SCH_FLIGHTPLAN_EXTMEMORY == 0 || SCH_SYSBUS_ONBOARD==0)
    int DAT_FPLAN_BUFF[SCH_FLIGHTPLAN_N_CMD*2];
#endif

//variables y buffers para almacenar los block de memoria de pada estructura a guardar en la memSD
static unsigned int dat_gpb_TeleCmd_256Block;
static unsigned int dat_gpb_FlightPlan_256Block;
static unsigned int dat_gpb_Pay_nextIndx_256Block;
static unsigned int dat_gpb_Pay_maxIndx_256Block;
static unsigned int dat_gpb_Pay_i_256Block[dat_pay_last_one];
static unsigned int dat_gpb_Aux_i_256Block[dat_aux_last_one];


//******************************************************************************
// DAT_TeleCmdBuff
//******************************************************************************
/**
 * Erase de TC internal (memSD) buffer
 */
void dat_erase_TeleCmdBuff(void){

//    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
//        printf("  dat_erase_TeleCmdBuff()..\n");
//        printf("    starting at block= %u\n", (unsigned int)dat_gpb_TeleCmd_256Block);
//    #endif
//
//    msd_blockErase(dat_gpb_TeleCmd_256Block);

    int indx;
    for(indx=0; indx<DAT_MAX_BUFF_TELECMD; indx++)
    {
        dat_set_TeleCmdBuff(indx, CMD_CMDNULL);
    }
}
/**
 * Obtiene el i-esima valor del buffer de Telecomandos
 * @param indx Posicion del buffer a recuperar
 * @return indx-esima posicion del Buffer
 */
int dat_get_TeleCmdBuff(int indx){
    int data;
    if(indx>=DAT_MAX_BUFF_TELECMD){
        return 0;
    }
    else{
        #if (SCH_FLIGHTPLAN_EXTMEMORY == 0 || SCH_SYSBUS_ONBOARD==0)
            data = DAT_CMD_BUFF[indx];
        #else
            msd_getVar_256BlockExtMem(dat_gpb_TeleCmd_256Block, indx, &data);
        #endif
        return data;
    }
}
/**
 * Modifica el i-esima valor del buffer de Telecomandos
 * @param indx Posicion del buffer a recuperar
 * @param data Valor de la indx-esima posicion del buffer
 */
void dat_set_TeleCmdBuff(int indx, int data)
{
    if(indx>=DAT_MAX_BUFF_TELECMD){
        return;
    }
    else{
        #if (SCH_FLIGHTPLAN_EXTMEMORY == 0 || SCH_SYSBUS_ONBOARD==0)
            DAT_CMD_BUFF[indx] = data;
        #else
            msd_setVar_256BlockExtMem(dat_gpb_TeleCmd_256Block, indx, data);
        #endif
    }
}
/**
 *
 * @return
 */
int dat_onReset_TeleCmdBuff(void){
    int indx1;
    for(indx1=0; indx1<DAT_MAX_BUFF_TELECMD; indx1++)
    {
        dat_set_TeleCmdBuff(indx1, CMD_CMDNULL);
    }

    return 1;
}
//******************************************************************************
// DAT_FligthPlanBuff
//******************************************************************************
/**
 * Esta funcion retorna un comando desde el fligh plan. Lee desde la SD el
 * i-esimo comando y su parametro.
 * @param index Index-esimo dato del fligh plan */
DispCmd dat_get_FlightPlan(unsigned int index)
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

#if (SCH_FLIGHTPLAN_EXTMEMORY == 1)
        msd_getVar_256BlockExtMem(dat_gpb_FlightPlan_256Block, index, &id);
        msd_getVar_256BlockExtMem(dat_gpb_FlightPlan_256Block, 0xFFFF - index, &param);
#elif (SCH_FLIGHTPLAN_EXTMEMORY == 0)
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
int dat_set_FlightPlan_cmd(unsigned int index, unsigned int cmdID)
{
    if(index < SCH_FLIGHTPLAN_N_CMD)
    {
        #if (SCH_FLIGHTPLAN_EXTMEMORY == 1)
            msd_setVar_256BlockExtMem( dat_gpb_FlightPlan_256Block, index, cmdID);
        #elif (SCH_FLIGHTPLAN_EXTMEMORY == 0)
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
int dat_set_FlightPlan_param(unsigned int index, int param)
{
    if(index < SCH_FLIGHTPLAN_N_CMD)
    {
        #if (SCH_FLIGHTPLAN_EXTMEMORY == 1)
            msd_setVar_256BlockExtMem( dat_gpb_FlightPlan_256Block, 0xFFFF-index, param);
        #elif (SCH_FLIGHTPLAN_EXTMEMORY == 0)
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
int dat_onReset_FlightPlan(void)
{
    //Do nothing onReset
//    unsigned long i;
//    int cmdid, param;
//
//    for(i=0; i < SCH_FLIGHTPLAN_N_CMD; i++)
//    {
//        cmdid = CMD_CMDNULL;
//        param = 0;
//
//        dat_set_FlightPlan_cmd(i, cmdid);
//        dat_set_FlightPlan_param(i, param);
//    }

    return 1;
}

/**
 * Borra todo el Fligth Plan
 */
void dat_erase_FlightPlanBuff(void){

#if (SCH_FLIGHTPLAN_EXTMEMORY == 1)
    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        printf("  dat_erase_FlightPlanBuff()..\n");
    #endif

    unsigned long i;
    int cmdid, param;
    for(i=0; i < SCH_FLIGHTPLAN_N_CMD; i++)
    {
        cmdid = CMD_CMDNULL;
        param = 0;

        dat_set_FlightPlan_cmd(i, cmdid);
        dat_set_FlightPlan_param(i, param);
    }
#elif (SCH_FLIGHTPLAN_EXTMEMORY == 0)
    int i=0;
    for(i=0; i<SCH_FLIGHTPLAN_N_CMD; i++)
    {
        DAT_FPLAN_BUFF[i] = CMD_CMDNULL; //Comandos
        DAT_FPLAN_BUFF[i+SCH_FLIGHTPLAN_N_CMD]=0; //Parametros
    }
#endif
}
//******************************************************************************
// DAT_PayloadBuff
//******************************************************************************
void dat_erase_PayloadBuff(DAT_PayloadBuff pay_i){
    unsigned long i, block;
    block = dat_pay_i_to_block(pay_i);

    #if (SCH_DATAREPOSITORY_VERBOSE>=1)
        printf("  dat_erase_pay_i_buff()..\n");
        printf("    starting at block = %u\n", (unsigned int)block);
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
unsigned long dat_pay_i_to_block(DAT_PayloadBuff pay_i){
    unsigned long block;

    block = dat_gpb_Pay_i_256Block[pay_i];
    return block;
}

//Setea el valor del ultimo/maximo indice del buffer de cierto payload
void dat_set_MaxPayIndx(DAT_PayloadBuff pay_i, unsigned int maxIndx){
    msd_setVar_256BlockExtMem(dat_gpb_Pay_maxIndx_256Block, (unsigned char)pay_i, maxIndx);
}

//Obtiene el valor del ultimo/maximo indice del buffer de cierto payload
unsigned int dat_get_MaxPayIndx(DAT_PayloadBuff pay_i){
    unsigned int maxIndx;
    msd_getVar_256BlockExtMem(dat_gpb_Pay_maxIndx_256Block, (unsigned char)pay_i, (int *)&maxIndx);
    return maxIndx;
}

//Setea el valor del indice actual del buffer de cierto payload
void dat_set_NextPayIndx(DAT_PayloadBuff pay_i, unsigned int nextIndx){
    msd_setVar_256BlockExtMem(dat_gpb_Pay_nextIndx_256Block, (unsigned char)pay_i, nextIndx);
}

//Obtiene el valor del indice actual del buffer de cierto payload
unsigned int dat_get_NextPayIndx(DAT_PayloadBuff pay_i){
    unsigned int nextIndx;
    msd_getVar_256BlockExtMem(dat_gpb_Pay_nextIndx_256Block, (unsigned char)pay_i, (int *)&nextIndx);
    return nextIndx;
}

//Retorna FALSE si el buffer se llena, TRUE si todo OK
BOOL dat_set_PayloadBuff(DAT_PayloadBuff pay_i, int value){
    // guarda "value" en la sgte posicion libre del buffer,
    // y retorna si lo logro o no (buffer lleno, payload invalido)
    unsigned int nextIndx;

    if( dat_isFull_PayloadBuff(pay_i)==TRUE){
        return FALSE;    //buffer lleno
    }
    nextIndx = dat_get_NextPayIndx(pay_i);

    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        printf("setPayloadVar [%04d] = %0x%X\n", nextIndx, value);
    #endif

    //guardo el valor de value
    unsigned long block = dat_pay_i_to_block(pay_i);
    msd_setVar_256BlockExtMem(block, nextIndx, value);
    
    //actualizo nextIndx
    dat_set_NextPayIndx(pay_i, nextIndx+1);
    
    return TRUE;
}

//Retorna FALSE si el indiex es invalido, TRUE si todo OK
BOOL dat_get_PayloadBuff(DAT_PayloadBuff pay_i, unsigned int indx, int *value){
    unsigned int desiredIndx, maxIndx;

    if(pay_i>=dat_pay_last_one){
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
            printf("getPayloadVar: payload invalido\n");
        #endif
        return FALSE;   //payload invalido
    }

    desiredIndx = indx;
    maxIndx = dat_get_MaxPayIndx(pay_i);
    if(desiredIndx>maxIndx){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            printf("getPayloadVar: buffer lleno, desiredIndx>maxIndx\n");
        #endif
        return FALSE;   //buffer lleno, indice fuera de rango
    }

    //obtengo el valor de value
    unsigned long block = dat_pay_i_to_block(pay_i);
    msd_getVar_256BlockExtMem(block, desiredIndx, value);

    return TRUE;
}

BOOL dat_isFull_PayloadBuff(DAT_PayloadBuff pay_i){
    unsigned int nextIndx, maxIndx;
    
    if(pay_i>=dat_pay_last_one){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            printf("isFullPayloadBuffer: payload invalido\n");
        #endif
        return FALSE;   //payload invalido
    }

    nextIndx = dat_get_NextPayIndx(pay_i);
    maxIndx = dat_get_MaxPayIndx(pay_i);
    if(nextIndx>maxIndx){
        #if (SCH_DATAREPOSITORY_VERBOSE>=2)
            printf("isFullPayloadBuffer: buffer lleno, desiredIndx>maxIndx\n");
        #endif
        return TRUE;   //buffer lleno, indice fuera de rango
    }
    return FALSE;
}

void dat_onReset_dataRepo(BOOL verbose){
//    if(verbose){
//        printf("        dat_onReset_dataRepo()..\n");
//    }
    DAT_PayloadBuff pay_i; DAT_AuxBuff aux_i;

    //calcula y ASIGNA los block para cada estructura
    if(verbose>=2){
        printf("        Map between DAT_xxxBuff structures and DAT_GPB addresses\n");
        printf("        GPB: 256Block memSD => 2^16 [int] buffer\n");
    }

    dat_gpb_TeleCmd_256Block = DAT_GPB_0;
    dat_gpb_FlightPlan_256Block = DAT_GPB_1;
    dat_gpb_Pay_nextIndx_256Block = DAT_GPB_2;
    dat_gpb_Pay_maxIndx_256Block = DAT_GPB_3;
    
    if(verbose){
        printf("        DAT_TeleCmdBuff <=> %u\n", (unsigned int)( dat_gpb_TeleCmd_256Block ) );
        printf("        DAT_FlightPlanBuff <=> %u\n", (unsigned int)( dat_gpb_FlightPlan_256Block ) );
        printf("        DAT_nextIndxBuff <=> %u\n", (unsigned int)( dat_gpb_Pay_nextIndx_256Block ) );
        printf("        DAT_maxIndxBuff <=> %u\n", (unsigned int)( dat_gpb_Pay_maxIndx_256Block ) );
    }

    //calcula y ASIGNA los block para cada pay_i
    dat_gpb_Pay_i_256Block[0] = DAT_GPB_4;

    if(verbose){
        printf("        DAT_PayloadBuff[0] <=> %u | number of Payloads = %d\n", dat_gpb_Pay_i_256Block[0], dat_pay_last_one);
    }

    for(pay_i=1; pay_i<dat_pay_last_one; pay_i++){
        dat_gpb_Pay_i_256Block[pay_i]=dat_gpb_Pay_i_256Block[(pay_i-1)] + 256;

        if(verbose>=2){
            printf("          DAT_PayloadBuff[%d] <=> %u\n", (unsigned int)pay_i, (unsigned int)( dat_gpb_Pay_i_256Block[pay_i]) );
        }
    }

    //calcula y ASIGNA los block para cada aux_i
    dat_gpb_Aux_i_256Block[0] = dat_gpb_Pay_i_256Block[ (dat_pay_last_one-1) ] + 256;
    
    if(verbose){
        printf("        DAT_AuxBuff[0] <=> %u | number of Auxs = %d\n", dat_gpb_Aux_i_256Block[0], dat_aux_last_one);
    }

    for(aux_i=1; aux_i<dat_aux_last_one; aux_i++){
        dat_gpb_Aux_i_256Block[aux_i]=dat_gpb_Aux_i_256Block[(aux_i-1)] + 256;

        if(verbose>=2){
            printf("          DAT_AuxBuff[%d] <=> %u\n", (unsigned int)aux_i, (unsigned int)dat_gpb_Aux_i_256Block[aux_i] );
        }
    }   
}

void dat_onReset_PayloadBuff(void){
    /* Erase nothing onReset
    DAT_Payload i;
    for(i=0;i<DAT_PAYLOADS_LENGTH;i++){
        dat_setNextPayIndx(i, 0);
        dat_setMaxPayIndx(i, 0x0AFF);
    }
    */
}

void dat_reset_PayloadBuff(DAT_PayloadBuff pay_i, unsigned int lenBuff, int mode){
    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        printf("Borrando buffer de pay_i = %u\n", (unsigned int)pay_i );
        printf("Usando mode = ");
        if(mode==1){
            printf("1 (con borrado)..\n");
        }
        else{
                printf("0 (sin borrado)..\n");
        }
    #endif

    //Seteo limites del Buffer
    dat_set_MaxPayIndx(pay_i, lenBuff-1);
    dat_set_NextPayIndx(pay_i, 0);

    if(mode==1){
        //borro el conteido del buffer
        //dat_erase_PayloadBuff(pay_i);
    }
    //Reinicio el Buffer
    dat_set_NextPayIndx(pay_i, 0);
    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        printf("reseteo de payload completo\n");
    #endif
}
void dat_print_pay_i_name(DAT_PayloadBuff pay_i){
    switch(pay_i){
        case dat_pay_lagmuirProbe:
            printf("dat_pay_lagmuirProbe");
        break;
        case dat_pay_sensTemp:
            printf("dat_pay_sensTemp");
        break;
        case dat_pay_gps:
            printf("dat_pay_gps");
        break;
        case dat_pay_gyro:
            printf("dat_pay_gyro");
        break;
        case dat_pay_expFis:
            printf("dat_pay_expFis");
        break;
        case dat_pay_camera:
            printf("dat_pay_camera");
        break;
        case dat_pay_tmEstado:
            printf("dat_pay_tmEstado");
        break;
        case dat_pay_test1:
            printf("dat_pay_test1");
        break;
        case dat_pay_test2:
            printf("dat_pay_test2");
        break;
        default:
            printf("No payload name %d", pay_i);
        break;
    }
}
//******************************************************************************
// DAT_AuxBuff
//******************************************************************************
//Get 
int dat_get_AuxBuff(DAT_AuxBuff aux_i, unsigned int indx){
    int val;
    unsigned long block = dat_aux_i_to_block(aux_i);
    msd_getVar_256BlockExtMem(block, indx, &val);

    return val;
}
//Set
void dat_set_AuxBuff(DAT_AuxBuff aux_i, unsigned int indx, int value){
    unsigned long block = dat_aux_i_to_block(aux_i);
    msd_setVar_256BlockExtMem(block, indx, value);

}
//onReset
int dat_onReset_AuxBuff(void){
    return 1;
}
//erase
int dat_erase_AuxBuff(DAT_AuxBuff aux_i){
    return 1;
}
//aux functions
/**
 * Asocia el DAT_GnrlPurpBuff gpb_i a un 256block de la msmSD
 * @param gpb_i del que quiero obtener el 256block
 * @return block de la memSD correspondiente
 */
unsigned long dat_aux_i_to_block(DAT_AuxBuff aux_i){
    unsigned long block;
    block = dat_gpb_Aux_i_256Block[aux_i];
    return block;
}
