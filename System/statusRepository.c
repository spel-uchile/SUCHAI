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

#include "statusRepository.h"
#include "cmdRTC.h"
#include "cmdPayload.h"
#include "include/cmdIncludes.h"

#define SCH_DATAREPOSITORY_VERBOSEMEMSD_ 0

//semaforo para sincronizar accceso a las variables de estado
extern xSemaphoreHandle dataRepositorySem;

#if (SCH_MEMEEPROM_ONBOARD==0)
    int STA_CUBESAT_VAR_BUFF[sta_cubesatVar_last_one];
#endif


/**
 * Funcion para modificar una variable de estado
 * @param indxVar. Variable de estado que quiero modificar
 * @param value. Valor a asignar a la variable de estado
 */
void sta_setCubesatVar(STA_CubesatVar indxVar, int value){
    portBASE_TYPE semStatus = xSemaphoreTake( dataRepositorySem, portMAX_DELAY );

    #if (SCH_MEMEEPROM_ONBOARD==0)
        //Para el caso de guardar las variables en memoria interna
        STA_CUBESAT_VAR_BUFF[indxVar] = value;
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
int sta_getCubesatVar(STA_CubesatVar indxVar){
    portBASE_TYPE semStatus = xSemaphoreTake( dataRepositorySem, portMAX_DELAY );
    
    int value;
    #if (SCH_MEMEEPROM_ONBOARD==0)
        //Para el caso de obtener las variables de la memoria interna
        value = STA_CUBESAT_VAR_BUFF[indxVar];
    #else
        //Para el caso de obtener las variables de la memI2C
        value = readIntEEPROM1( (unsigned char)indxVar );
    #endif

    semStatus = xSemaphoreGive(dataRepositorySem);

    return value;
}


/**
 * Funcion a llamar luego de un Reset del PIC y luego de @sa sta_onReset_memEEPROM
 * para inicializar a los valore correctos las variables de estado que necesitan ser modificadas.
 *
 *Funcion pensada para sersiorarse que luego del reseteo se tengan valores coherentes.
 *Se inicializan variables que explicitamente necesitan ser modificadas
 */
void sta_onResetStatRepo(void)
{
    //External hw satus were already set
    //sta_RTC_isAlive,
    //sta_TRX_isAlive,
    //sta_EPS_isAlive,
    //sta_MemEEPROM_isAlive,
    //sta_MemSD_isAlive,

    sta_setCubesatVar(sta_ppc_opMode, STA_PPC_OPMODE_NORMAL);
    // lee, guarda e imprime razon del reset
    sta_setCubesatVar(sta_ppc_lastResetSource, sta_get_ppc_lastResetSource(TRUE) );
    //reseta la variable, pues hubo un reset
    sta_setCubesatVar(sta_ppc_hoursWithoutReset, 0x0000);
    //resetCounter No debe inicializarse luego de un reset
    //Su valor debe ser traido de la memEEPROM y modificado
    if( sta_getCubesatVar(sta_SUCHAI_isDeployed) == 0 ){
        sta_setCubesatVar(sta_ppc_resetCounter, 0);
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
            printf("        - First time on! Setting resetCounter to 0\r\n");
        #endif
    }
    else{
        int rc=sta_getCubesatVar(sta_ppc_resetCounter) + 1;
        sta_setCubesatVar(sta_ppc_resetCounter, rc);
        #if (SCH_DATAREPOSITORY_VERBOSE>=1)

            printf("            - NOT the First time on! resetCounter++\r\n");
            printf("            - resetCounter = %d\n", rc);
        #endif
    }
    sta_setCubesatVar(sta_ppc_enwdt, PPC_INITIAL_WDT_STATE);

    sta_setCubesatVar(sta_ppc_MB_nOE_USB_nINT_stat, PPC_MB_nOE_USB_nINT_CHECK);
    sta_setCubesatVar(sta_ppc_MB_nOE_MHX_stat, PPC_MB_nOE_MHX_CHECK);
    sta_setCubesatVar(sta_ppc_MB_nON_MHX_stat, PPC_MB_nON_MHX_CHECK);
    sta_setCubesatVar(sta_ppc_MB_nON_SD_stat, PPC_MB_nON_SD_CHECK);

    // mDEP_state
    // No  deben inicializarse, su valor debe ser conservado en la memExt y modificado solo en deploy_antenna

    // mRTC_state

    // mEPS_state
    

    // mTRX_state
    sta_setCubesatVar(sta_trx_rssi_mean, 9999);
    sta_setCubesatVar(sta_trx_count_tc, 0);
    sta_setCubesatVar(sta_trx_count_tm, 0);

    //Telecomand Buffer
    sta_setCubesatVar(sta_trx_newCmdBuff, 0x0000);      // Doesn't exist unproccessed TC Frames in TRX
    sta_setCubesatVar(sta_trx_newTcFrame, 0x0000);      // Doues't exist unproccessed TC in internal buffer

    //Flight Plan

    //memSD

    //PAYLOAD
}
/**
 *
 * @return last Reset Source
 */
int sta_get_ppc_lastResetSource(BOOL verb){
    static char ft;
    static int lreset;
    if(ft==0){
        lreset = PwrMgnt_ResetSource();
        ft = 1; //enter only once
    }

    if(verb){
        printf("        - LastResetSouce: ");
        switch ( lreset )
        {
            case POWER_ON_Reset:
                printf("POWER_ON_Reset\r\n");      /* 0x00 Aun nose cuando salta */
                mPWRMGNT_Clear_PORbit();
            break;
            case BURN_OUT_Reset:
                printf("BURN_OUT_Reset\r\n");      /* 0x01 ver nota mas arriba*/
                mPWRMGNT_Clear_BORbit();
            break;
            case WATCHDOG_Reset:
                printf("WATCHDOG_Reset\r\n");      /* 0x02 al overflow del WDT, luego de aprox 2 min sin ejecutarse ClrWdt(); */
                mPWRMGNT_Clear_WDTObit();
            break;
            case SOFTWARE_Reset:
                printf("SOFTWARE_Reset\r\n");      /* 0x03 ocurre luego de ejecutar ppc_reset() */
                mPWRMGNT_Clear_SWRbit();
            break;
            case EXTERNAL_Reset:
                printf("EXTERNAL_Reset\r\n");      /* 0x04 Ocurre cuando se programa y cuando se energiza/desenergiza el PIC*/
                mPWRMGNT_Clear_EXTRbit();
            break;
            case CFG_WORD_MISMATCH_Reset:
                printf("CFG_WORD_MISMATCH_Reset\r\n");
                mPWRMGNT_Clear_CMbit();
            break;
            case ILLEGAL_INSTR_Reset:
                printf("ILLEGAL_INSTR_Reset\r\n");
                mPWRMGNT_Clear_IOPUWRbit();
            break;
            case TRAP_Reset:
                printf("TRAP_Reset\r\n");
                mPWRMGNT_Clear_TRAPRbit();
            break;
            default:
                printf("No new RESET\r\n");
                lreset= -1;
                /* ver nota mas arriba */
            break;
        }
    }
    
    return lreset;
}

/**
 * Asocia el STA_Payload pay_i a la DAT_CubesatVar que controla la ejecucion o
 * o no de ese Payload
 * @param pay_i DAT_Payload del que quiero obtener el DAT_CubesatVar
 * @return DAT_CubesatVar dat_pay_xxx_perform
 */
//STA_CubesatVar dat_pay_i_to_performVar(DAT_Payload pay_i){
STA_CubesatVar sta_pay_i_to_performVar(int pay_i){
    STA_CubesatVar dat_pay_xxx_perform;

    switch(pay_i){
        case dat_pay_lagmuirProbe:
            dat_pay_xxx_perform = sta_pay_lagmuirProbe_perform;
        break;
        case dat_pay_sensTemp:
            dat_pay_xxx_perform = sta_pay_sensTemp_perform;
        break;
        case dat_pay_gps:
            dat_pay_xxx_perform = sta_pay_gps_perform;
        break;
        case dat_pay_gyro:
            dat_pay_xxx_perform = sta_pay_gyro_perform;
        break;
        case dat_pay_expFis:
            dat_pay_xxx_perform = sta_pay_expFis_perform;
        break;
        case dat_pay_camera:
            dat_pay_xxx_perform = sta_pay_camera_perform;
        break;
        case dat_pay_tmEstado:
            dat_pay_xxx_perform = sta_pay_tmEstado_perform;
        break;
        case dat_pay_test1:
            dat_pay_xxx_perform = sta_pay_test1_perform;
        break;
        case dat_pay_test2:
            dat_pay_xxx_perform = sta_pay_test2_perform;
        break;
        default:
            dat_pay_xxx_perform=-1;
        break;
    }

    return dat_pay_xxx_perform;
}
