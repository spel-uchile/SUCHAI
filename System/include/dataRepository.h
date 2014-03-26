/**
 * @file  dataRepository.h
 * @author Tomas Opazo T
 * @date 2013
 * @copyright GNU Public License.
 *
 * Este header contiene al Status Prepository (variables de estado) y al
 * Data Repository (datos varios). API interfaz para cada caso:
 *
 * Status Repository:
 *      DAT_Cubesatvar
 * 
 * Data Repository:
 *      DAT_FligthPlanBuff
 *      DAT_TeleCmdBuff
 *      DAT_PayloadVar
 *      DAT_GnrlPurpBuff
 *
 */

#ifndef DATA_REPO_H
#define DATA_REPO_H

#include "FreeRTOS.h"
#include "semphr.h"

#include "memSD.h"
#include "memEEPROM.h"

#include "cmdIncludes.h"

#include "statusRepository.h"


//***STATUS REPOSITORY**********************************************************

int dat_init_memEEPROM(void);
int dat_memEEPROM_isAlive(void);


void dat_init_ppc_lastResetSource(void);
void dat_reset_pay_i_performVar(void);

//***************************************************************************************************************

/**
 * This function assigns the Memory Map of the external memSD
 */
void dat_onReset_memSD(BOOL verbose);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat telecomand buffer

typedef enum _DAT_TeleCmdBuff{
    //*************
    dat_tcb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_TeleCmdBuff;

/* Max internal (not Transceiver mem) TeleCMD buffer length considering TC in
 * format [0xTCID|0xARGM|0xSTOP] in frames of 63 bytes each one. So 10,5
 * commands max, that means 40 bytes or 10cmd + 10 param */
#define DAT_MAX_BUFF_TELECMD SCH_DATAREPOSITORY_MAX_BUFF_TELECMD

int dat_getTelecmdBuff(int indx);
void dat_setTelecmdBuff(int indx, int data);
int dat_onResetTelecmdBuff(void);
void dat_erase_TeleCmdBuff(void);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat fligthPlan

typedef enum _DAT_FligthPlanBuff{
    //*************
    dat_fpb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_FligthPlanBuff;

DispCmd dat_getFlightPlan(unsigned int index);
int dat_setFlightPlan_cmd(unsigned int index, unsigned int cmdID);
int dat_setFlightPlan_param(unsigned int index, int param);
int dat_onResetFlightPlan(void);
void dat_erase_FlightPlanBuff(void);
//***************************************************************************************************************
//The following is an API to interface with the dataRepository cubesat Payload buffer/data

/**
 * Enum que contiene los Payloads en el SUCHAI. Para agregar uno, se debe:
 * 1) Agergar el dat_pay_xxx a #DAT_Payload
 * 2) Agregar dat_pay_xxx_perform a #DAT_cubesatVar
 * 3) Agregar case a <dat_pay_i_to_performVar>"()"
 * 4) hacer funciones pay_init_xxx, pay_take_xxx y pay_stop_xxx en \sa cmdPayload.c
 * 5) Agregar las funciones de 4) en los case de <pay_currentStateLogic>"()"
 *
 */
typedef enum _DAT_Payload{
    dat_pay_lagmuirProbe=0,
    dat_pay_test2,
    dat_pay_gps,
    dat_pay_camera,     //pay_i=3
    dat_pay_sensTemp,   
    dat_pay_gyro,       //pay_i=5
    dat_pay_tmEstado,
    dat_pay_test1,
    dat_pay_expFis,     //pay_i=8
    //*************
    dat_pay_last_one    //Se utiliza para marcar el largo del arreglo.
                        //Y para indicar el ID de TM de CUbesatVar
}DAT_Payload;

//Retorna FALSE si el buffer se llena, TRUE si todo OK
BOOL dat_setPayloadVar(DAT_Payload pay_i, int value);
//Retorna FALSE si el indiex es invalido, TRUE si todo OK
BOOL dat_getPayloadVar(DAT_Payload pay_i, unsigned int indx, int *value);
//Inicializa la estructura de data payload
void dat_onResetPayloadVar(void);
//Inicializa la estructura de data payload
void dat_resetPayloadBuffer(DAT_Payload pay_i, unsigned int maxIndx, int mode);
//Inicializa la estructura de data payload
BOOL dat_isFullPayloadBuffer(DAT_Payload pay_i);

//Funciones Auxiliares
//Setea el valor del ultimo/maximo indice del buffer de cierto payload
void dat_setMaxPayIndx(DAT_Payload pay_i, unsigned int lastIndx);
//Obtiene el valor del ultimo/maximo indice del buffer de cierto payload
unsigned int dat_getMaxPayIndx(DAT_Payload pay_i);
//Setea el valor del indice actual del buffer de cierto payload
void dat_setNextPayIndx(DAT_Payload pay_i, unsigned int nextIndx);
//Obtiene el valor del indice actual del buffer de cierto payload
unsigned int dat_getNextPayIndx(DAT_Payload pay_i);
void msd_blockErase(unsigned long block_address);
void dat_erase_pay_i_buff(DAT_Payload pay_i);
unsigned long dat_pay_i_to_block(DAT_Payload pay_i);
//*************************************************************************************************

/**
 * Enum que contiene los General Purpose Buffers del SUCHAI.
 */
typedef enum _DAT_GnrlPurpBuff{
    dat_gpb_expFis_f0=0,
    dat_gpb_expFis_f1,
    dat_gpb_expFis_f2,
    dat_gpb_expFis_f3,
    dat_gpb_expFis_f4,
    dat_gpb_expFis_f5,
    dat_gpb_expFis_f6,
    dat_gpb_expFis_f7,
    dat_gpb_expFis_f8,
    dat_gpb_expFis_f9,
    dat_gpb_expFis_hist,

    dat_gpb_test1,
    dat_gpb_test2,
    //*************
    dat_gpb_last_one    //Se utiliza para marcar el largo del arreglo.
                        //Y para indicar el ID de TM de CUbesatVar
}DAT_GnrlPurpBuff;

unsigned long dat_gpb_i_to_block(DAT_GnrlPurpBuff gpb_i);
//Set
void dat_setGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx, int value);
//Get 
int dat_getGPB(DAT_GnrlPurpBuff gpb_i, unsigned int indx);

#endif // DATA_REPO_H

