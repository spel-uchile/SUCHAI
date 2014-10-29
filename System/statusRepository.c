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
            case sta_TRX_isAlive:
                value = trx_isAlive(NULL);
                break;
            case sta_EPS_isAlive:
                value = eps_isAlive(FALSE);
                break;
            case sta_MemEEPROM_isAlive:
                value = memEEPROM_isAlive();
                break;
            case sta_MemSD_isAlive:
                value = memSD_isAlive();
                break;
            case sta_Antenna_isDeployed:
                value = (int)thk_check_antenna_isDeployed(1000);
                break;
            // Payload Hw status (connected trough the PC/104 to the OBC -PIC24-)
            case sta_pay_lagmuirProbe_isAlive:
                printf("PAY => (Payload subsystem)\r\n");
                value = pay_isAlive_lagmuirProbe(NULL);
                break;
            case sta_pay_sensTemp_isAlive:
                value = pay_isAlive_sensTemp(NULL);
                break;
            case sta_pay_gps_isAlive:
                value = pay_isAlive_gps(NULL);
                break;
            case sta_pay_expFis_isAlive:
                value = pay_isAlive_expFis(NULL);
                break;
            case sta_pay_camera_isAlive:
                value = pay_isAlive_camera(NULL);
                break;
            case sta_pay_gyro_isAlive:
                value = pay_isAlive_gyro(NULL);
                break;
            case sta_pay_tmEstado_isAlive:
                value = pay_isAlive_tmEstado(NULL);
                break;
            case sta_pay_battery_isAlive:
                value = pay_isAlive_battery(NULL);
                break;
            case sta_pay_debug_isAlive:
                value = pay_isAlive_debug(NULL);
                break;
            case sta_pay_lagmuirProbe_isDeployed:
                value = 0;  //no hay forma de saberlo !!
                break;
            //FPL => (C&DH subsystem)
            case sta_fpl_index:
                printf("FPL => (C&DH subsystem, Fligth Plan)\r\n");
                value =  drp_fpl_get_index(NULL);
                break;
            //PPC => (C&DH subsystem)
            case sta_ppc_opMode:
                printf("PPC => (C&DH subsystem)\r\n");
                value = ppc_get_opMode(NULL);
                break;
            case sta_ppc_lastResetSource:
                param = 0;  //verbosity level
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
                param = 0;  //verbosity level
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
                printf("DEP => (C&DH subsystem)\r\n");
                value = thk_get_dep_ant_deployed(NULL);
                break;
            case sta_dep_ant_tries:
                value = thk_get_dep_ant_tries(NULL);
                break;
            case sta_dep_year:
                value = thk_get_dep_year(NULL);
                break;
            case sta_dep_month:
                value = thk_get_dep_month(NULL);
                break;
            case sta_dep_week_day:
                value = thk_get_dep_week_day(NULL);
                break;
            case sta_dep_day_number:
                value = thk_get_dep_day_number(NULL);
                break;
            case sta_dep_hours:
                value = thk_get_dep_hours(NULL);
                break;
            case sta_dep_minutes:
                value = thk_get_dep_minutes(NULL);
                break;
            case sta_dep_seconds:
                value = thk_get_dep_seconds(NULL);
                break;
            //RTC => (C&DH subsystem)
            case sta_rtc_year:
                printf("RTC => (C&DH subsystem)\r\n");
                value = RTC_get_year();
                break;
            case sta_rtc_month:
                value = RTC_get_month();
                break;
            case sta_rtc_week_day:
                value = RTC_get_week_day();
                break;
            case sta_rtc_day_number:
                value = RTC_get_day_num();
                break;
            case sta_rtc_hours:
                value = RTC_get_hours();
                break;
            case sta_rtc_minutes:
                value = RTC_get_minutes();
                break;
            case sta_rtc_seconds:
                value = RTC_get_seconds();
                break;
            #if (SCH_EPS_ONBOARD==1)
                //EPS => (Energy subsystem)
                case sta_eps_bat0_voltage:
                    printf("EPS => (Energy subsystem)\r\n");
                    value = readEPSvars(EPS_ID_bat0_voltage);
                    break;
                case sta_eps_bat0_current:
                    value = readEPSvars(EPS_ID_bat0_current);
                    break;
                case sta_eps_bus5V_current:
                    value = readEPSvars(EPS_ID_bus5V_current);
                    break;
                case sta_eps_bus3V_current:
                    value = readEPSvars(EPS_ID_bus3V_current);
                    break;
                case sta_eps_bus_battery_current:
                    value = readEPSvars(EPS_ID_bus_battery_current);
                    break;
                case sta_eps_bat0_temp:
                    value = readEPSvars(EPS_ID_bat0_temp);
                    break;
                case sta_eps_panel_pwr:
                    value = readEPSvars(EPS_ID_panel_pwr);
                    break;
                case sta_eps_status:
                    value = readEPSvars(EPS_ID_status);
                    break;
                case sta_eps_soc:
                    value = readEPSvars(EPS_ID_soc);
                    break;
                case sta_eps_socss:
                    value = readEPSvars(EPS_ID_socss);
                    break;
                case sta_eps_state_flag:
                    value = readEPSvars(EPS_ID_state_flag);
                    break;
                case sta_eps_charging:
                    value = readEPSvars(EPS_ID_current_dir);
                    break;
            #endif
            #if (SCH_TRX_ONBOARD==1)
                //TRX => (Communication subsystem)
                case sta_trx_opmode:
                    printf("TRX => (Communication subsystem)\r\n");
                    break;
                case sta_trx_temp_hpa:
                    break;
                case sta_trx_temp_mcu:
                    break;
                case sta_trx_rssi:
                    break;
                case sta_trx_rssi_mean:
                    break;
                case sta_trx_status_tc:
                    break;
                case sta_trx_count_tm:
                    break;
                case sta_trx_count_tc:
                    break;
                case sta_trx_lastcmd_day:
                    break;
                case sta_trx_newTcFrame:
                    break;
                case sta_trx_newCmdBuff:
                    break;
            #endif
            //PAY => (Payload subsystem)
            case sta_pay_lagmuirProbe_state:
                printf("PAY => (Payload subsystem)\r\n");
                value = pay_get_state_lagmuirProbe(NULL);
                break;
            case sta_pay_sensTemp_state:
                value = pay_get_state_sensTemp(NULL);
                break;
            case sta_pay_gps_state:
                value = pay_get_state_gps(NULL);
                break;
            case sta_pay_expFis_state:
                value = pay_get_state_expFis(NULL);
                break;
            case sta_pay_camera_state:
                value = pay_get_state_camera(NULL);
                break;
            case sta_pay_gyro_state:
                value = pay_get_state_gyro(NULL);
                break;
            case sta_pay_tmEstado_state:
                value = pay_get_state_tmEstado(NULL);
                break;
            case sta_pay_battery_state:
                value = pay_get_state_battery(NULL);
                break;
            case sta_pay_debug_state:
                value = pay_get_state_debug(NULL);
//            case:
//                break;
            default:
                printf("[sta_getCubesatVar] Error: No function/command for STA_CubesatVar %d \r\n", indxVar);
                value = -(0x7FFF);
            break;
        }
    #endif

    semStatus = xSemaphoreGive(statusRepositorySem);

    return value;
}


/**
 * Funcion a llamar luego de un Reset del PIC y luego de 
 * @sa sta_onReset_memEEPROM para inicializar a los valore correctos las
 * variables de estado que necesitan ser modificadas.
 *
 * No todas son modificadas, hay muchas que se inicializan en otros
 * llamados anteriores o posteriores (@sa default_PIC_config,
 * @sa dep_init_suchai_hw, @sa dep_init_suchai_repos).
 */
void sta_onResetStatRepo(void)
{
    int param, res;

    // Set to default
    param = STA_PPC_OPMODE_NORMAL;
    ppc_set_opMode(&param);

    // Increment reset counter
    param = ppc_get_resetCounter(NULL);
    param++;
    ppc_set_resetCounter(&param);
    

    //print important StatusVars
    printf("[sta_onResetStatRepo] Important STA_CubesatVar variables:\r\n");

    res = ppc_get_resetCounter(NULL);
    printf("    * ppc_get_resetCounter: %d\r\n", res);

    param = 1;  //verbose
    ppc_get_lastResetSource(&param);

    res = ppc_get_hoursAlive(NULL);
    printf("    * ppc_get_hoursAlive: %d\r\n", res);

    res = ppc_get_hoursWithoutReset(NULL);
    printf("    * ppc_get_hoursWithoutReset: %d\r\n", res);

    res = ppc_get_wdt_state(NULL);
    printf("    * ppc_get_wdt_state: %d\r\n", res);

    param = 1;  //verbose
    res = ppc_get_osc(&param);


    /*
     * Comment all this block ONLY after all var have a callable function "get"
     */
    printf("[sta_onResetStatRepo] All STA_CubesatVar variables:\r\n");
    //print all SatatusVars
    STA_CubesatVar indxVar;
    for(indxVar=0;indxVar<sta_cubesatVar_last_one;indxVar++){
        int r = sta_getCubesatVar(indxVar);
        //printf("    * sta_getCubesatVar(%d) = %d \r\n", indxVar, r);
        printf("    * sta_getCubesatVar(%s) = %d \r\n", varToString(indxVar), r);
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
//            printf("        * First time on! Setting resetCounter to 0\r\n");
//        #endif
//    }
//    else{
//        int rc=sta_getCubesatVar(sta_ppc_resetCounter) + 1;
//        sta_setCubesatVar(sta_ppc_resetCounter, rc);
//        #if (SCH_DATAREPOSITORY_VERBOSE>=1)
//
//            printf("            * NOT the First time on! resetCounter++\r\n");
//            printf("            * resetCounter = %d\n", rc);
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
            dat_pay_xxx_perform = sta_pay_lagmuirProbe_state;
        break;
        case dat_pay_sensTemp:
            dat_pay_xxx_perform = sta_pay_sensTemp_state;
        break;
        case dat_pay_gps:
            dat_pay_xxx_perform = sta_pay_gps_state;
        break;
        case dat_pay_gyro:
            dat_pay_xxx_perform = sta_pay_gyro_state;
        break;
        case dat_pay_expFis:
            dat_pay_xxx_perform = sta_pay_expFis_state;
        break;
        case dat_pay_camera:
            dat_pay_xxx_perform = sta_pay_camera_state;
        break;
        case dat_pay_tmEstado:
            dat_pay_xxx_perform = sta_pay_tmEstado_state;
        break;
        case dat_pay_battery:
            dat_pay_xxx_perform = sta_pay_battery_state;
        break;
        case dat_pay_debug:
            dat_pay_xxx_perform = sta_pay_debug_state;
        break;
        default:
            dat_pay_xxx_perform=-1;
        break;
    }

    return dat_pay_xxx_perform;
}

char* varToString(STA_CubesatVar var_i){
    char *pc;
    switch(var_i){
        case sta_RTC_isAlive:
            pc = "sta_RTC_isAlive";
            break;
        case sta_TRX_isAlive:
            pc = "sta_TRX_isAlive";
            break;
        case sta_EPS_isAlive:
            pc = "sta_EPS_isAlive";
            break;
        case sta_MemEEPROM_isAlive:
            pc = "sta_MemEEPROM_isAlive";
            break;
        case sta_MemSD_isAlive:
            pc = "sta_MemSD_isAlive";
            break;
        case sta_Antenna_isDeployed:
            pc = "sta_Antenna_isDeployed";
            break;

        // Payload Hw status (connected trough the PC/104 to the OBC -PIC24-)
        case sta_pay_lagmuirProbe_isAlive:
            pc = "sta_pay_lagmuirProbe_isAlive";
            break;
        case sta_pay_sensTemp_isAlive:
            pc = "sta_pay_sensTemp_isAlive";
            break;
        case sta_pay_gps_isAlive:
            pc = "sta_pay_gps_isAlive";
            break;
        case sta_pay_expFis_isAlive:
            pc = "sta_pay_expFis_isAlive";
            break;
        case sta_pay_camera_isAlive:
            pc = "sta_pay_camera_isAlive";
            break;
        case sta_pay_gyro_isAlive:
            pc = "sta_pay_gyro_isAlive";
            break;
        case sta_pay_tmEstado_isAlive:
            pc = "sta_pay_tmEstado_isAlive";
            break;
        case sta_pay_battery_isAlive:
            pc = "sta_pay_battery_isAlive";
            break;
        case sta_pay_debug_isAlive:
            pc = "sta_pay_debug_isAlive";
            break;
        case sta_pay_lagmuirProbe_isDeployed:
            pc = "sta_pay_lagmuirProbe_isDeployed";
            break;

        //FLIGHT PLAN => (C&DH subsystem)
        case sta_fpl_index:         // Indice del flight plan que sera editado
            pc = "sta_fpl_index";
            break;

        //PPC => (C&DH subsystem)
        case sta_ppc_opMode:
            pc = "sta_ppc_opMode";
            break;
        case sta_ppc_lastResetSource:
            pc = "sta_ppc_lastResetSource";
            break;
        case sta_ppc_hoursAlive:
            pc = "sta_ppc_hoursAlive";
            break;
        case sta_ppc_hoursWithoutReset:
            pc = "sta_ppc_hoursWithoutReset";
            break;
        case sta_ppc_resetCounter:
            pc = "sta_ppc_resetCounter";
            break;
        case sta_ppc_wdt:				// 1=WDT Active: 0=WDT Inactive
            pc = "sta_ppc_wdt";
            break;
        case sta_ppc_osc:				// holds Current Oscillator
            pc = "sta_ppc_osc";
            break;
        case sta_ppc_MB_nOE_USB_nINT_stat:
            pc = "sta_ppc_MB_nOE_USB_nINT_stat";
            break;
        case sta_ppc_MB_nOE_MHX_stat:
            pc = "sta_ppc_MB_nOE_MHX_stat";
            break;
        case sta_ppc_MB_nON_MHX_stat:
            pc = "sta_ppc_MB_nON_MHX_stat";
            break;
        case sta_ppc_MB_nON_SD_stat:
            pc = "sta_ppc_MB_nON_SD_stat";
            break;

        //DEP => (C&DH subsystem)
        case sta_dep_ant_deployed:      // 1=already deployed: 0=not deployed yet
            pc = "sta_dep_ant_deployed";
            break;
        case sta_dep_ant_tries:         // Number of tries to deploy antenna
            pc = "sta_dep_ant_tries";
            break;
        case sta_dep_year:
            pc = "sta_dep_year";
     	    break;
        case sta_dep_month:
            pc = "sta_dep_month";
     	    break;
        case sta_dep_week_day:
            pc = "sta_dep_week_day";
     	    break;
        case sta_dep_day_number:
            pc = "sta_dep_day_number";
     	    break;
        case sta_dep_hours:
            pc = "sta_dep_hours";
     	    break;
        case sta_dep_minutes:
            pc = "sta_dep_minutes";
     	    break;
        case sta_dep_seconds:
            pc = "sta_dep_seconds";
     	    break;

        //RTC => (C&DH subsystem)
        case sta_rtc_year:
            pc = "sta_rtc_year";
     	    break;
        case sta_rtc_month:
            pc = "sta_rtc_month";
     	    break;
        case sta_rtc_week_day:
            pc = "sta_rtc_week_day";
     	    break;
        case sta_rtc_day_number:
            pc = "sta_rtc_day_number";
     	    break;
        case sta_rtc_hours:
            pc = "sta_rtc_hours";
     	    break;
        case sta_rtc_minutes:
            pc = "sta_rtc_minutes";
     	    break;
        case sta_rtc_seconds:
            pc = "sta_rtc_seconds";
     	    break;

        //EPS => (Energy subsystem)
        case sta_eps_bat0_voltage:
            pc = "sta_eps_bat0_voltage";
     	    break;
        case sta_eps_bat0_current:
            pc = "sta_eps_bat0_current";
     	    break;
        case sta_eps_bus5V_current:
            pc = "sta_eps_bus5V_current";
     	    break;
        case sta_eps_bus3V_current:
            pc = "sta_eps_bus3V_current";
     	    break;
        case sta_eps_bus_battery_current:
            pc = "sta_eps_bus_battery_current";
     	    break;
        case sta_eps_bat0_temp:
            pc = "sta_eps_bat0_temp";
     	    break;
        case sta_eps_panel_pwr:
            pc = "sta_eps_panel_pwr";
     	    break;
        case sta_eps_status:
            pc = "sta_eps_status";
     	    break;
        case sta_eps_soc:
            pc = "sta_eps_soc";
     	    break;
        case sta_eps_socss:
            pc = "sta_eps_socss";
     	    break;
        case sta_eps_state_flag:
            pc = "sta_eps_state_flag";
     	    break;
        case sta_eps_charging:
            pc = "sta_eps_charging";
     	    break;

        /* Revisar de aqui hacia abajo si aun son necesarios !!! */

        //TRX => (Communication subsystem)
        case sta_trx_opmode:           // Operation mode
            pc = "sta_trx_opmode";
     	    break;
        case sta_trx_temp_hpa:         // Temp of HPA
            pc = "sta_trx_temp_hpa";
     	    break;
        case sta_trx_temp_mcu:         // Temp of MCU
            pc = "sta_trx_temp_mcu";
     	    break;
        case sta_trx_rssi:             // RSSI: Signal level
            pc = "sta_trx_rssi";
     	    break;
        case sta_trx_rssi_mean:        // RSSI_MEAN
            pc = "sta_trx_rssi_mean";
     	    break;
        case sta_trx_status_tc:        // Status Register of TC
            pc = "sta_trx_status_tc";
     	    break;
        case sta_trx_count_tm:         // number of sended TM
            pc = "sta_trx_count_tm";
     	    break;
        case sta_trx_count_tc:         // number of received TC
            pc = "sta_trx_count_tc";
     	    break;
        case sta_trx_lastcmd_day:      // day of the last received tc (since 1/1/00)
            pc = "sta_trx_lastcmd_day";
     	    break;
        // Cmd buffer control
        case sta_trx_newTcFrame:       // Exist any unprocessed TcFram?
            pc = "sta_trx_newTcFrame";
     	    break;
        case sta_trx_newCmdBuff:       // Exist unprocessed CMD in the external buffer
            pc = "sta_trx_newCmdBuff";
     	    break;
        //trx_repo_telecmd[STA_MAX_BUFF_TELECMD]:   // Assuming that each Telecommand is of the form: [cmdId | param]

        //PAYLOAD
        case sta_pay_lagmuirProbe_state:
            pc = "sta_pay_lagmuirProbe_state";
     	    break;
        case sta_pay_sensTemp_state:
            pc = "sta_pay_sensTemp_state";
     	    break;
        case sta_pay_gps_state:
            pc = "sta_pay_gps_state";
     	    break;
        case sta_pay_expFis_state:
            pc = "sta_pay_expFis_state";
     	    break;
        case sta_pay_camera_state:
            pc = "sta_pay_camera_state";
     	    break;
        case sta_pay_gyro_state:
            pc = "ta_pay_gyro_state";
     	    break;
        case sta_pay_tmEstado_state:
            pc = "sta_pay_tmEstado_state";
     	    break;
        case sta_pay_battery_state:
            pc = "sta_pay_battery_state";
     	    break;
        case sta_pay_debug_state:
            pc = "sta_pay_debug_state";
     	    break;
        default:
            pc = "No string for this var_i";
     	    break;
    }
    return pc;
}