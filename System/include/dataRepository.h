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
//#include "memEEPROM.h"

#include "cmdIncludes.h"

//#include "statusRepository.h"


//******************************************************************************
/**
 * Defines que contienen los General Purpose Buffers (max indx = 2^16 = 65536 )
 * del SUCHAI, estos son usados por otros buffer (Payloads buffers, TC buffer,
 * Flight Plan Buffer ). Estos usan wrappers para accederlos mas comodamente
 */
#define DAT_GPB_FIRST_BLOCK 31
#define DAT_GPB_0       ( DAT_GPB_FIRST_BLOCK + 256*0 )
#define DAT_GPB_1       ( DAT_GPB_FIRST_BLOCK + 256*1 )
#define DAT_GPB_2       ( DAT_GPB_FIRST_BLOCK + 256*2 )
#define DAT_GPB_3       ( DAT_GPB_FIRST_BLOCK + 256*3 )
#define DAT_GPB_4       ( DAT_GPB_FIRST_BLOCK + 256*4 )
//#define DAT_GPB_5       ( DAT_GPB_FIRST_BLOCK + 256*5 )
//#define DAT_GPB_6       ( DAT_GPB_FIRST_BLOCK + 256*6 )
//#define DAT_GPB_7       ( DAT_GPB_FIRST_BLOCK + 256*7 )
//#define DAT_GPB_8       ( DAT_GPB_FIRST_BLOCK + 256*8 )
//#define DAT_GPB_9       ( DAT_GPB_FIRST_BLOCK + 256*9 )
//#define DAT_GPB_10      ( DAT_GPB_FIRST_BLOCK + 256*10 )
//#define DAT_GPB_11      ( DAT_GPB_FIRST_BLOCK + 256*11 )
//#define DAT_GPB_12      ( DAT_GPB_FIRST_BLOCK + 256*12 )
//#define DAT_GPB_13      ( DAT_GPB_FIRST_BLOCK + 256*13 )
//#define DAT_GPB_14      ( DAT_GPB_FIRST_BLOCK + 256*14 )
//#define DAT_GPB_15      ( DAT_GPB_FIRST_BLOCK + 256*15 )
//#define DAT_GPB_16      ( DAT_GPB_FIRST_BLOCK + 256*16 )
//#define DAT_GPB_17      ( DAT_GPB_FIRST_BLOCK + 256*17 )
//#define DAT_GPB_18      ( DAT_GPB_FIRST_BLOCK + 256*18 )
//#define DAT_GPB_19      ( DAT_GPB_FIRST_BLOCK + 256*19 )
//#define DAT_GPB_20      ( DAT_GPB_FIRST_BLOCK + 256*20 )


/**
 * This function assigns the Memory Map of the external memSD
 */
void dat_onReset_dataRepo(BOOL verbose);

//******************************************************************************
// DAT_TeleCmdBuff
//******************************************************************************

typedef enum _DAT_TeleCmdBuff{
    //*************
    dat_tcb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_TeleCmdBuff;

/* Max internal (not Transceiver mem) TeleCMD buffer length considering TC in
 * format [0xTCID|0xARGM|0xSTOP] in frames of 63 bytes each one. So 10,5
 * commands max, that means 40 bytes or 10cmd + 10 param */
#define DAT_MAX_BUFF_TELECMD SCH_DATAREPOSITORY_MAX_BUFF_TELECMD

//Get
int dat_get_TeleCmdBuff(int indx);
//Set
void dat_set_TeleCmdBuff(int indx, int data);
//onReset
int dat_onReset_TeleCmdBuff(void);
//Erase
void dat_erase_TeleCmdBuff(void);

//******************************************************************************
// DAT_FligthPlanBuff
//******************************************************************************
typedef enum _DAT_FligthPlanBuff{
    //*************
    dat_fpb_last_one    //Se utiliza para marcar el largo del arreglo.
}DAT_FligthPlanBuff;

//Get
DispCmd dat_get_FlightPlan(unsigned int index);
//Set
int dat_set_FlightPlan_cmd(unsigned int index, unsigned int cmdID);
int dat_set_FlightPlan_param(unsigned int index, int param);
//onReset
int dat_onReset_FlightPlan(void);
//Erase
void dat_erase_FlightPlanBuff(void);

//******************************************************************************
// DAT_PayloadBuff
//******************************************************************************
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
}DAT_PayloadBuff;

//Get
BOOL dat_get_PayloadBuff(DAT_PayloadBuff pay_i, unsigned int indx, int *value);
unsigned int dat_get_MaxPayIndx(DAT_PayloadBuff pay_i);
unsigned int dat_get_NextPayIndx(DAT_PayloadBuff pay_i);

//Set
BOOL dat_set_PayloadBuff(DAT_PayloadBuff pay_i, int value);
void dat_set_MaxPayIndx(DAT_PayloadBuff pay_i, unsigned int lastIndx);
void dat_set_NextPayIndx(DAT_PayloadBuff pay_i, unsigned int nextIndx);

//onReset
void dat_onReset_PayloadBuff(void);
void dat_reset_PayloadBuff(DAT_PayloadBuff pay_i, unsigned int lenBuff, int mode);

//Erase
void dat_erase_PayloadBuff(DAT_PayloadBuff pay_i);

//Auxiliary functions
unsigned long dat_pay_i_to_block(DAT_PayloadBuff pay_i);
BOOL dat_isFull_PayloadBuff(DAT_PayloadBuff pay_i);

//******************************************************************************
// DAT_AuxBuff
//******************************************************************************
typedef enum _DAT_GnrlPurpBuff{
    dat_auxBuff_0,
    dat_auxBuff_1,
    dat_auxBuff_2,
    dat_auxBuff_3,
    dat_auxBuff_4,
    dat_auxBuff_5,
    dat_auxBuff_6,
    dat_auxBuff_7,
    dat_auxBuff_8,
    dat_auxBuff_9,
    dat_auxBuff_10,
    dat_auxBuff_11,
    dat_auxBuff_12,
    //*************
    dat_aux_last_one    //Se utiliza para marcar el largo del arreglo.
                        //Y para indicar el ID de TM de CUbesatVar
}DAT_AuxBuff;

//Get
int dat_get_AuxBuff(DAT_AuxBuff  aux_i, unsigned int indx);
//Set
void dat_set_AuxBuff(DAT_AuxBuff  aux_i, unsigned int indx, int value);
//onReset
int dat_onReset_AuxBuff(void);
//Erase
int dat_erase_AuxBuff(DAT_AuxBuff aux_i);
//Auxiliary functions
unsigned long dat_aux_i_to_block(DAT_AuxBuff aux_i);


#endif // DATA_REPO_H

