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
extern xSemaphoreHandle statusRepositorySem;

#if (SCH_MEMEEPROM_ONBOARD==0)
    int STA_CUBESAT_VAR_BUFF[sta_cubesatVar_last_one];
#endif


/**
 * Funcion para modificar una variable de estado
 * @param indxVar. Variable de estado que quiero modificar
 * @param value. Valor a asignar a la variable de estado
 */
void sta_setCubesatVar(STA_CubesatVar indxVar, int value){
    portBASE_TYPE semStatus = xSemaphoreTake( statusRepositorySem, portMAX_DELAY );

    #if (SCH_MEMEEPROM_ONBOARD==0)
        //Para el caso de guardar las variables en memoria interna
        STA_CUBESAT_VAR_BUFF[indxVar] = value;
    #else
        //Para el caso de guardar las variables en la memI2C
        writeIntEEPROM1( (unsigned char)indxVar, value);
    #endif

    semStatus = xSemaphoreGive(statusRepositorySem);

    //callback function for every write to a STA_CubesatVar
    
}
/**
 * Funcion para obtener una variable de estado
 * @param indxVar. Variable de estado que quiero modificar
 * @param value. Valor a asignar a la variable de estado
 */
int sta_getCubesatVar(STA_CubesatVar indxVar){
    portBASE_TYPE semStatus = xSemaphoreTake( statusRepositorySem, portMAX_DELAY );
    
    int value;
    #if (SCH_MEMEEPROM_ONBOARD==0)
        //Para el caso de obtener las variables de la memoria interna
        value = STA_CUBESAT_VAR_BUFF[indxVar];
    #else
//        //Para el caso de obtener las variables de la memI2C
//        value = readIntEEPROM1( (unsigned char)indxVar );

        int param;
        switch(indxVar){
            // Bus Hw status (connected trough the PC/104 to the OBC -PIC24-)
            case sta_RTC_isAlive:
                value = RTC_isAlive();
            break;
//            case sta_TRX_isAlive:
//                value = trx_isAlive();
//            break;
            case sta_EPS_isAlive:
                value = eps_isAlive();
            break;
            case sta_MemEEPROM_isAlive:
                value = memEEPROM_isAlive();
            break;
            case sta_MemSD_isAlive:
                value = memSD_isAlive();
            break;
            case sta_Antenna_isDeployed:
                value = thk_check_antenna_isDeployed(1000);
            break;
//            // Payload Hw status (connected trough the PC/104 to the OBC -PIC24-)
//            case sta_pay_lagmuirProbe_isAlive:
//            break;
//            case sta_pay_sensTemp_isAlive:
//            break;
//            case sta_pay_gps_isAlive:
//            break;
//            case sta_pay_camera_isAlive:
//            break;
//            case sta_pay_gyro_isAlive:
//            break;
//            case sta_pay_tmEstado_isAlive:
//            break;
//            case sta_pay_test1_isAlive:
//            break;
//            case sta_pay_test2_isAlive:
//            break;
//            case sta_pay_lagmuirProbe_isDeployed:
//            break;
            //PPC => (C&DH subsystem)
            case sta_ppc_opMode:
                value = ppc_get_opMode(NULL);
            break;
            case sta_ppc_lastResetSource:
                param = 1;
                value = ppc_get_lastResetSource(&param);
            break;
            case sta_ppc_hoursAlive:
                value = ppc_get_hoursAlive(NULL);
            break;
            case sta_ppc_hoursWithoutReset:
                value = ppc_get_hoursWithoutReset(NULL);
            break;
            case sta_ppc_resetCounter:
                value = ppc_get_resetCounter(NULL);
            break;
            case sta_ppc_wdt:
                value = ppc_get_wdt_state(NULL);
            break;
            case sta_ppc_osc:
                param = 0;  //no verbose
                value = ppc_get_osc(&param);
            break;
            case sta_ppc_MB_nOE_USB_nINT_stat:
                value = ppc_get_PPC_MB_nOE_USB_nINT(NULL);
            break;
            case sta_ppc_MB_nOE_MHX_stat:
                value = ppc_get_PPC_MB_nOE_MHX(NULL);
            break;
            case sta_ppc_MB_nON_MHX_stat:
                value = ppc_get_PPC_MB_nON_MHX(NULL);
            break;
            case sta_ppc_MB_nON_SD_stat:
                value = ppc_get_PPC_MB_nON_SD(NULL);
            break;
            //DEP => (C&DH subsystem)
            case sta_dep_ant_deployed:
            break;
            case sta_dep_ant_tries:
            break;
            case sta_dep_year:
            break;
            case sta_dep_month:
            break;
            case sta_dep_week_day:
            break;
            case sta_dep_day_number:
            break;
            case sta_dep_hours:
            break;
            case sta_dep_minutes:
            break;
            case sta_dep_seconds:
            break;
//            //RTC => (C&DH subsystem)
//            case sta_rtc_year:
//            break;
//            case sta_rtc_month:
//            break;
//            case sta_rtc_week_day:
//            break;
//            case sta_rtc_day_number:
//            break;
//            case sta_rtc_hours:
//            break;
//            case sta_rtc_minutes:
//            break;
//            case sta_rtc_seconds:
//            break;
//            //EPS => (Energy subsystem)
//            case sta_eps_bat0_voltage:
//            break;
//            case sta_eps_bat0_current:
//            break;
//            case sta_eps_bus5V_current:
//            break;
//            case sta_eps_bus3V_current:
//            break;
//            case sta_eps_bus_battery_current:
//            break;
//            case sta_eps_bat0_temp:
//            break;
//            case sta_eps_panel_pwr:
//            break;
//            case sta_eps_status:
//            break;
//            case sta_eps_soc:
//            break;
//            case sta_eps_socss:
//            break;
//            case sta_eps_state_flag:
//            break;
//            case sta_eps_charging:
//            break;
//            //TRX => (Communication subsystem)
//            case sta_trx_opmode:
//            break;
//            case sta_trx_temp_hpa:
//            break;
//            case sta_trx_temp_mcu:
//            break;
//            case sta_trx_rssi:
//            break;
//            case sta_trx_rssi_mean:
//            break;
//            case sta_trx_status_tc:
//            break;
//            case sta_trx_count_tm:
//            break;
//            case sta_trx_count_tc:
//            break;
//            case sta_trx_lastcmd_day:
//            break;
//            case sta_trx_newTcFrame:
//            break;
//            case sta_trx_newCmdBuff:
//            break;
//            //FLIGHT PLAN
//            case sta_fpl_index:
//            break;
//            //PAYLOAD
//            case sta_pay_lagmuirProbe_perform:
//            break;
//            case sta_pay_sensTemp_perform:
//            break;
//            case sta_pay_gps_perform:
//            break;
//            case sta_pay_expFis_perform:
//            break;
//            case sta_pay_camera_perform:
//            break;
//            case sta_pay_gyro_perform:
//            break;
//            case sta_pay_tmEstado_perform:
//            break;
//            case sta_pay_test1_perform:
//            break;
//            case sta_pay_test2_perform:
//            break;
//            case :
//            break;
            default:
                printf("[sta_getCubesatVar] Error: No associated function for STA_CubesatVar %d \r\n", indxVar);
                value = -1;
            break;
        }
    #endif

    semStatus = xSemaphoreGive(statusRepositorySem);

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
    STA_CubesatVar indxVar;
    for(indxVar=0;indxVar<sta_cubesatVar_last_one;indxVar++){
        int r = sta_getCubesatVar(indxVar);
        printf("[sta_onResetStatRepo] sta_getCubesatVar(%d) = %d \r\n", indxVar, r);
    }
//    //External hw satus were already set
//    //sta_RTC_isAlive,
//    //sta_TRX_isAlive,
//    //sta_EPS_isAlive,
//    //sta_MemEEPROM_isAlive,
//    //sta_MemSD_isAlive,
//
//    sta_setCubesatVar(sta_ppc_opMode, STA_PPC_OPMODE_NORMAL);
//    // lee, guarda e imprime razon del reset
//    sta_setCubesatVar(sta_ppc_lastResetSource, sta_get_ppc_lastResetSource(TRUE) );
//    //reseta la variable, pues hubo un reset
//    sta_setCubesatVar(sta_ppc_hoursWithoutReset, 0x0000);
//    //resetCounter No debe inicializarse luego de un reset
//    //Su valor debe ser traido de la memEEPROM y modificado
//    if( sta_getCubesatVar(sta_SUCHAI_isDeployed) == 0 ){
//        sta_setCubesatVar(sta_ppc_resetCounter, 0);
//        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
//            printf("        - First time on! Setting resetCounter to 0\r\n");
//        #endif
//    }
//    else{
//        int rc=sta_getCubesatVar(sta_ppc_resetCounter) + 1;
//        sta_setCubesatVar(sta_ppc_resetCounter, rc);
//        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
//
//            printf("            - NOT the First time on! resetCounter++\r\n");
//            printf("            - resetCounter = %d\n", rc);
//        #endif
//    }
//    sta_setCubesatVar(sta_ppc_enwdt, PPC_INITIAL_WDT_STATE);
//    sta_setCubesatVar(sta_ppc_MB_nOE_USB_nINT_stat, PPC_MB_nOE_USB_nINT_CHECK);
//    sta_setCubesatVar(sta_ppc_MB_nOE_MHX_stat, PPC_MB_nOE_MHX_CHECK);
//    sta_setCubesatVar(sta_ppc_MB_nON_MHX_stat, PPC_MB_nON_MHX_CHECK);
//    sta_setCubesatVar(sta_ppc_MB_nON_SD_stat, PPC_MB_nON_SD_CHECK);
//
//    // mDEP_state
//    // No  deben inicializarse, su valor debe ser conservado en la memExt y modificado solo en deploy_antenna
//
//    // mRTC_state
//
//    // mEPS_state
//
//
//    // mTRX_state
//    sta_setCubesatVar(sta_trx_rssi_mean, 9999);
//    sta_setCubesatVar(sta_trx_count_tc, 0);
//    sta_setCubesatVar(sta_trx_count_tm, 0);
//
//    //Telecomand Buffer
//    sta_setCubesatVar(sta_trx_newCmdBuff, 0x0000);      // Doesn't exist unproccessed TC Frames in TRX
//    sta_setCubesatVar(sta_trx_newTcFrame, 0x0000);      // Doues't exist unproccessed TC in internal buffer
//
//    //Flight Plan
//
//    //memSD
//
//    //PAYLOAD
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
