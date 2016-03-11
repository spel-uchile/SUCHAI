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

#include "cmdTHK.h"

cmdFunction thkFunction[THK_NCMD];
int thk_sysReq[THK_NCMD];

void thk_onResetCmdTHK(){
    printf("        thk_onResetCmdTHK\n");

    int i;
    for(i=0; i<THK_NCMD; i++) thk_sysReq[i] = CMD_SYSREQ_MIN;

    //De display
    thkFunction[(unsigned char)thk_id_hw_check] = thk_hw_check;

    thkFunction[(unsigned char)thk_id_executeBeforeFlight] = thk_executeBeforeFlight;
    thkFunction[(unsigned char)thk_id_test_antenna_deployment] = thk_test_antenna_deployment;
    
    thkFunction[(unsigned char)thk_id_silent_time_and_pictures] = thk_silent_time_and_pictures;
    thkFunction[(unsigned char)thk_id_get_AntSwitch_isOpen] = thk_get_AntSwitch_isOpen;
    thkFunction[(unsigned char)thk_id_deployment_registration] = thk_deployment_registration;

    thkFunction[(unsigned char)thk_id_get_dep_ant_deployed] = thk_get_dep_ant_deployed;
    thkFunction[(unsigned char)thk_id_get_dep_ant_tries] = thk_get_dep_ant_tries;
    thkFunction[(unsigned char)thk_id_get_dep_year] = thk_get_dep_year;
    thkFunction[(unsigned char)thk_id_get_dep_month] = thk_get_dep_month;
    thkFunction[(unsigned char)thk_id_get_dep_week_day] = thk_get_dep_week_day;
    thkFunction[(unsigned char)thk_id_get_dep_day_number] = thk_get_dep_day_number;
    thkFunction[(unsigned char)thk_id_get_dep_hours] = thk_get_dep_hours;
    thkFunction[(unsigned char)thk_id_get_dep_minutes] = thk_get_dep_minutes;
    thkFunction[(unsigned char)thk_id_get_dep_seconds] = thk_get_dep_seconds;

    //Power budget restriction
    thkFunction[(unsigned char)thk_id_deploy_antenna] = thk_deploy_antenna;
    thk_sysReq[(unsigned char)thk_id_deploy_antenna]  = CMD_SYSREQ_MIN + SCH_DEPLOYMENT_SYS_REQ;
    thkFunction[(unsigned char)thk_id_suchai_deployment] = thk_suchai_deployment;
    thk_sysReq[(unsigned char)thk_id_suchai_deployment]  = CMD_SYSREQ_MIN + SCH_DEPLOYMENT_SYS_REQ;

}

//------------------------------------------------------------------------------
int thk_hw_check(void *param){
    
    printf("[thk_debug]  Testing SUCHAI satellite Hw ..\r\n");
    int arg_param;
    
    int arg_ebf = *((int *)param);
    if(arg_ebf == 1){
//        arg_param = 1;
//        thk_executeBeforeFlight((void *)&arg_param);
//        int tries = 1;
//        thk_deployment_registration(&tries);
    }
    
    
    ClrWdt();
    printf("[thk_debug]    Testing Subsystem OBC ..\r\n");
    printf("[thk_debug]      mem_EEPROM_isAlive = %d \r\n", mem_EEPROM_isAlive());
    printf("[thk_debug]      memSD_isAlive = %d \r\n", memSD_isAlive());
    printf("[thk_debug]      RTC_isAlive = %d \r\n", RTC_isAlive());
    printf("[thk_debug]    Testing Subsystem TRX ..\r\n");
    printf("[thk_debug]      trx_isAlive = %d \r\n", trx_isAlive(NULL));
    printf("[thk_debug]    Testing Subsystem EPS ..\r\n");
    printf("[thk_debug]      eps_isAlive = %d \r\n", eps_isAlive(NULL));
    printf("[thk_debug]      eps_print_hk(0) \r\n"); eps_print_hk(0); __delay_ms(1000);
    printf("[thk_debug]    Testing Subsystem PAY ..\r\n");
    //typedef enum _DAT_Payload{
    //    dat_pay_tmEstado=0,
    //    dat_pay_battery,
    //    dat_pay_debug,
    //    dat_pay_langmuirProbe,   //pay_i=3
    //    dat_pay_gps,
    //    dat_pay_camera,         //pay_i=5
    //    dat_pay_sensTemp,   
    //    dat_pay_gyro,           //pay_i=7
    //    dat_pay_expFis,     
    //    //*************
    //    dat_pay_last_one    //Se utiliza para marcar el largo del arreglo.
    //                        //Y para indicar el ID de TM de stateVar
    //}DAT_Payload_Buff;
     
    ClrWdt();
    printf("[thk_debug]      tmEstado ..\r\n");
    pay_init_tmEstado(NULL);
    pay_take_tmEstado(NULL);
    pay_stop_tmEstado(NULL);
    
//    pay_init_battery(NULL);
//    pay_take_battery(NULL);
//    pay_stop_battery(NULL);
    
//    pay_init_debug(NULL);
//    pay_take_debug(NULL);
//    pay_stop_debug(NULL);
 
    ClrWdt();
    printf("[thk_debug]      langmuirProbe ..\r\n");
    arg_param = -1;
    pay_init_langmuirProbe((void *)&arg_param); ClrWdt();
    pay_take_langmuirProbe(NULL); ClrWdt();
    pay_stop_langmuirProbe(NULL);
    
    ClrWdt();
    printf("[thk_debug]      gps ..\r\n");
    pay_init_gps(NULL);
    pay_take_gps(NULL);
    pay_stop_gps(NULL);

    ClrWdt();
    int resol = 0x03;
    pay_takePhoto_camera((void *)&resol);
//    pay_get_savedPhoto_camera(NULL);
//    pay_init_camera(NULL);
//    pay_take_camera(NULL);
//    pay_stop_camera(NULL);
       
    ClrWdt();
    printf("[thk_debug]      sensTemp ..\r\n");
    pay_init_sensTemp(NULL);
    pay_take_sensTemp(NULL);
    pay_stop_sensTemp(NULL);

    ClrWdt();
    printf("[thk_debug]      gyro ..\r\n");
    pay_init_gyro(NULL);
    pay_take_gyro(NULL);
    pay_stop_gyro(NULL);

//    pay_init_expFis(NULL);
//    pay_take_expFis(NULL);
//    pay_stop_expFis(NULL);
    
    ClrWdt();
    printf("[thk_debug]    srp_print_STA_stateVar ..\r\n");
    srp_print_STA_stateVar(NULL);
    printf("[thk_debug]    srp_print_MemEEPROM_Vars ..\r\n");
    srp_print_MemEEPROM_Vars(NULL);
    //printf("[thk_debug]    drp_print_dat_FlightPlan ..\r\n");
    //drp_print_dat_FlightPlan(NULL);
    printf("[thk_debug]    drp_print_dat_PayloadIndxs ..\r\n");
    drp_print_dat_PayloadIndxs(NULL);
    printf("[thk_debug]    drp_print_dat_PayloadVar ..\r\n");
    DAT_Payload_Buff pay_i = 0;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++){
        drp_print_dat_PayloadVar(&pay_i);
    }
    printf("[thk_debug]    eps_print_hk ..\r\n");
    eps_print_hk(0);

    arg_param = 10000;
    thk_test_antenna_deployment((void *)&arg_param);
    
    if(arg_ebf == 1){
        printf("[thk_debug]    thk_executeBeforeFlight ..\r\n");
        arg_param = 1;
        thk_executeBeforeFlight((void *)&arg_param);
    }
    if(arg_ebf >= 1){
        printf("[thk_debug]    thk_deployment_registration ..\r\n");
        int tries = 1;
        thk_deployment_registration(&tries);
    }
    
    return 1;
}

//-----------------------------------------------------
int thk_get_dep_ant_deployed(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_deployed;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_ant_tries(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_tries;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
//rtc
int thk_get_dep_year(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_year;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_month(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_month;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_week_day(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_week_day;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_day_number(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_day_number;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_hours(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_hours;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_minutes(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_minutes;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int thk_get_dep_seconds(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_seconds;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}

//
//int thk_set_dep_ant_deployed(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_deployed;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_ant_tries(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_tries;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_year(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_year;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_month(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_month;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_week_day(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_week_day;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_day_number(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_day_number;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_hours(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_hours;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_minutes(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_minutes;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_seconds(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_seconds;
//    int value = *((int*)param);
//    mem_setVar(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//------------------------------------------------------------------------------
int thk_suchai_deployment(void *param)
{    
    if( sta_get_BusStateVar(sta_dep_ant_deployed) == 0 ){
        printf("[thk_suchai_deployment] Starting antenna deployment ..\r\n");
    }
    else{
        printf("[thk_suchai_deployment] Antennas already deployed ..\r\n");
        return 1;
    }

    int lvl;
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        printf("\n[thk_deploy_antenna] Starting TRX silence .. \r\n");
            lvl = (12000UL);
            trx_set_beacon_level((void *)&lvl);
    #endif
    
    int delay_min = *( (int*)param );
    printf("[thk_suchai_deployment] delay_min = %d\r\n", delay_min);

    unsigned long initial_tick_10ms = xTaskGetTickCount(); //get initial tick-time
    unsigned long silent_time_10ms = delay_min*(60UL)*(100UL); // time_s*100 [ms] = time_ms
    unsigned long final_tick_10ms = initial_tick_10ms + silent_time_10ms;

    printf("[thk_suchai_deployment] initial_tick_10ms = %lu | final_tick_10ms = %lu \r\n", initial_tick_10ms, final_tick_10ms);

    // print rtc time
    rtc_print(NULL);

    //take picture
    #if(SCH_PAY_CAM_nMEMFLASH_ONBOARD==1 )
        #if(SCH_THOUSEKEEPING_SILENT_REALTIME == 1)
            int resol = 0x03;
            pay_takePhoto_camera((void *)&resol); //takes 10min to complete
            pay_get_savedPhoto_camera(NULL);
        #else
            printf("  Skipping pay_takePhoto_camera(NULL) call ..\r\n");
        #endif
    #endif

    // print rtc time
    rtc_print(NULL);

    // Waiting time
    #if(SCH_THOUSEKEEPING_ANT_DEP_REALTIME == 1)
        unsigned int elapsed_mins = 0;
        while(TRUE){
            unsigned long int cu_tick_10ms = xTaskGetTickCount();
            if( cu_tick_10ms >= final_tick_10ms || elapsed_mins>35 ){
                printf("[thk_suchai_deployment] Waiting timeout, cu_tick_10ms = %lu, elapsed_mins = %d\r\n", cu_tick_10ms, elapsed_mins);
                break;
            }
            printf("[thk_suchai_deployment] Waiting for timeout, cu_tick_10ms = %lu, elapsed_mins = %d\r\n", cu_tick_10ms, elapsed_mins);
            //vTaskDelayUntil(&xLastWakeTime, delay_tick_60s); //Suspend task 60 sec
            ClrWdt();
            __delay_ms(60000UL);  //delay 60sec
            ClrWdt();
            elapsed_mins++;

        }
    #endif

    // print rtc time
    rtc_print(NULL);

    /* Deploy Antena */
    #if (SCH_ANTENNA_ONBOARD==1)
        int rt_mode = SCH_THOUSEKEEPING_ANT_DEP_REALTIME; /* 1=Real Time, 0=Debug Time */
        thk_deploy_antenna(&rt_mode);
    #endif

    // print rtc time
    rtc_print(NULL);

    //delay 60sec to avoid drain-out the EPS
    #if (SCH_THOUSEKEEPING_ANT_DEP_REALTIME == 1)
        ClrWdt();
        __delay_ms(60000);

        //other "only once" tasks
        //..
    #endif

    //Restore TRX capabilities
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        printf("\n[thk_deploy_antenna] Ending TRX silence .. \r\n");
            lvl = SCH_TRX_BEACON_BAT_LVL;
            trx_set_beacon_level((void *)&lvl);
    #endif

    ClrWdt();
    __delay_ms(2000);   // make sure to print the prompt

    return 1;
}

#define THK_SILENT_TIME_MIN 30          ///< cuantos "minutos" (65,535[s]) estara en inactividad antes de iniciarse
#define THK_MAX_TRIES_ANT_DEPLOY 15     ///< cuantas veces tratara desplegar la antena antes de anunciar fracaso
#define THK_DEPLOY_TIME 45311           ///< 2*THK_DEPLOY_TIME/1000 indica cuantos "ms" estara activo el bus de 3.3V quemando el nilon
#define THK_REST_DEPLOY_TIME 5000       ///< cuantos "ms" estara inactivo el bus de 3.3V descanzando de tratar de quemar el nilon
#define THK_RECHECK_TIME 2000           ///< despues de cuantos "ms" RE-chequeara que efectivamente se desplego la antena
/**
 * Deploys satellite antennas
 * @param param 1 realime, 0 debug time
 * @return 1 success, 0 fails
 */
int thk_deploy_antenna(void *param)
{
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        printf("\n[thk_deploy_antenna] Deploying TRX Antenna... \r\n");
        //rtc_print(NULL);
    #endif

    //Realtime=1 DebugTime=0
    unsigned int delay_dep_time, delay_rest_dep_time, delay_recheck_dep_time;
    int mode= *( (int *)param );
    if(mode)
    {
//        delay_dep_time = (THK_DEPLOY_TIME) / portTICK_RATE_MS;
//        delay_rest_dep_time = (THK_REST_DEPLOY_TIME) / portTICK_RATE_MS;
//        delay_recheck_dep_time = (THK_RECHECK_TIME) / portTICK_RATE_MS;
        delay_dep_time = (THK_DEPLOY_TIME);
        delay_rest_dep_time = (THK_REST_DEPLOY_TIME);
        delay_recheck_dep_time = (THK_RECHECK_TIME);
    }
    else
    {
//        delay_dep_time = (600) / portTICK_RATE_MS;
//        delay_rest_dep_time = (400) / portTICK_RATE_MS;
//        delay_recheck_dep_time = (200) / portTICK_RATE_MS;
        delay_dep_time = (600);
        delay_rest_dep_time = (400);
        delay_recheck_dep_time = (200);
    }
    
    //intentos quemando el nylon
    int tries_indx = 0;

    #if(SCH_ANTENNA_ONBOARD == 1)
    {
        for(tries_indx=1; tries_indx<=THK_MAX_TRIES_ANT_DEPLOY; tries_indx++)
        {
            #if (SCH_TDEPLOYMENT_VERBOSE>=2)
                printf("    [Deploying] Attempt #%d\r\n", tries_indx);
            #endif

            #if (SCH_TDEPLOYMENT_VERBOSE>=2)
                printf("      Burning ANT1 .. \r\n");
            #endif
            PPC_ANT12_SWITCH=1;
            PPC_ANT1_SWITCH=1;
            PPC_ANT2_SWITCH=0;
            //PPC_ANT1_SWITCH=0;
            //PPC_ANT2_SWITCH=1;
//            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT1 */
//            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT1 */
            __delay_ms(delay_dep_time);
            ClrWdt();
            __delay_ms(delay_dep_time);
            
            #if (SCH_TDEPLOYMENT_VERBOSE>=2)
                printf("          Taking a rest .. \r\n");
            #endif
            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
//            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */
            __delay_ms(delay_rest_dep_time);
            ClrWdt();

            #if (SCH_TDEPLOYMENT_VERBOSE>=2)
                printf("      Burning ANT2 .. \r\n");
            #endif
            PPC_ANT12_SWITCH=1;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=1;
            //PPC_ANT1_SWITCH=1;
            //PPC_ANT2_SWITCH=0;
//            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT2 */
//            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT2 */
            __delay_ms(delay_dep_time);
            ClrWdt();
            __delay_ms(delay_dep_time);

            #if (SCH_TDEPLOYMENT_VERBOSE>=2)
                printf("          Taking a rest .. \r\n");
            #endif
            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
//            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */
            __delay_ms(delay_rest_dep_time);
            ClrWdt();

            if( thk_get_AntSwitch_isOpen(&delay_recheck_dep_time) == 1 )
            {
                thk_deployment_registration(&tries_indx);

                #if (SCH_TDEPLOYMENT_VERBOSE>=1)
                    printf("    ANTENNA DEPLOYED SUCCESSFULLY [%d TRIES]\r\n", tries_indx);
                    rtc_print(NULL);
                #endif 
                //return 1;     =>>> try  HK_MAX_TRIES_ANT_DEPLOY   always !!!!!!
            }
        }
    }
    #endif

    //after the for() tries_indx == THK_MAX_TRIES_ANT_DEPLOY+1
    tries_indx = THK_MAX_TRIES_ANT_DEPLOY+1; //por si acaso
    thk_deployment_registration(&tries_indx);

    #if (SCH_TDEPLOYMENT_VERBOSE>=2)
        printf("    ANTENNA DEPLOY FAIL [%d TRIES]\r\n", THK_MAX_TRIES_ANT_DEPLOY);
        rtc_print(NULL);
    #endif

    return 0;
}

int thk_test_antenna_deployment(void *param){
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        printf("\n[thk_test_antenna_demployment] Testing Antenna ..\r\n");
        //rtc_print(NULL);
    #endif

    unsigned int delay_recheck_dep_time;
    delay_recheck_dep_time = (THK_RECHECK_TIME);

    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        int ant_state = thk_get_AntSwitch_isOpen(&delay_recheck_dep_time);
        printf("    thk_get_AntSwitch_isOpen(&delay_recheck_dep_time) = %d \r\n", ant_state);
        rtc_print(NULL);
    #endif
    
    #if(SCH_ANTENNA_ONBOARD == 1)
    {
        unsigned int delay_dep_time, delay_rest_dep_time;
        unsigned int test_time = *((int *)param);
        delay_dep_time = (test_time);
        delay_rest_dep_time = (test_time);
            
        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            printf("      Burning ANT1 for %d ms.. \r\n", delay_dep_time);
        #endif
        PPC_ANT12_SWITCH=1;
        PPC_ANT1_SWITCH=1;
        PPC_ANT2_SWITCH=0;
        __delay_ms(delay_dep_time);
        ClrWdt();

        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            printf("          Taking a rest .. \r\n");
        #endif
        PPC_ANT12_SWITCH=0;
        PPC_ANT1_SWITCH=0;
        PPC_ANT2_SWITCH=0;
        __delay_ms(delay_rest_dep_time);
        ClrWdt();

        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            printf("      Burning ANT2 for %d ms.. \r\n", delay_dep_time);
        #endif
        PPC_ANT12_SWITCH=1;
        PPC_ANT1_SWITCH=0;
        PPC_ANT2_SWITCH=1;        
        __delay_ms(delay_dep_time);
        ClrWdt();

        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            printf("          Taking a rest .. \r\n");
        #endif
        PPC_ANT12_SWITCH=0;
        PPC_ANT1_SWITCH=0;
        PPC_ANT2_SWITCH=0;
        __delay_ms(delay_rest_dep_time);
        ClrWdt();
    }
    #endif

    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        ant_state = thk_get_AntSwitch_isOpen(&delay_recheck_dep_time);
        printf("    thk_get_AntSwitch_isOpen(&delay_recheck_dep_time) = %d \r\n", ant_state);
        rtc_print(NULL);
    #endif        
    
    return 1;
}
/**
 *
 * @param param
 * @return 1=AntennaSqitch is open, 0 = AntennaSqitch is closed
 */
int thk_get_AntSwitch_isOpen(void* param){
    unsigned int recheck_time_ms = *( (unsigned int*)param );
    
    if(recheck_time_ms >= 10000){   //cut everything longer than 10seg
        recheck_time_ms = 10000;
    }
    
    if(PPC_ANT12_CHECK == 0){   /* reviso */
//        vTaskDelay(delay_recheck_dep_time);   /* tiempo de RE-chequeo */
        __delay_ms(recheck_time_ms);
        if(PPC_ANT12_CHECK == 0){   /* RE-reviso */
            return 1;
        }
    }
    return 0;
}

/**
 * Command that writes DEP registration. It takes care of:
 * 1) Register success or failure of antenna deployment
 * 2) Reset Antenna DEP vars to a Before Fligth condition
 * @param param -1 = Reset, [1, MAX_TRIES] registration as succes,
 * [MAX_TRIES+1 = registration as failure
 * @return 1 = On a correct parameter, 0 incorrect parameter (no change in vars)
 */
int thk_deployment_registration(void *param)
{
    int dep_tries = *( (int *)param );
    int dep_state;
    MemEEPROM_Vars mem_eeprom_var; int value;
    BOOL valid_dep_tries = FALSE;

    //Reset antenna DEP variables (EBF routine)
    if(dep_tries == (-1) ) {
        mem_eeprom_var = mem_dep_ant_deployed; value = 0;
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_ant_tries; value = 0;
        mem_setVar(mem_eeprom_var, value);

        //RTC
        mem_eeprom_var = mem_dep_year; value = 0;
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_month; value = 0;
        mem_setVar(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_day_number; value = 0;
        mem_setVar(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_week_day; value = 0;
        mem_setVar(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_hours; value = 0;
        mem_setVar(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_minutes; value = 0;
        mem_setVar(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_seconds; value = 0;
        mem_setVar(mem_eeprom_var, value);
        return 1;
    }
    // Antennas were NOT deployed
    if(dep_tries == (THK_MAX_TRIES_ANT_DEPLOY+1) ){
        dep_state = 0;
        dep_tries = THK_MAX_TRIES_ANT_DEPLOY+1;
        valid_dep_tries = TRUE;
    }
    //Antennas were deployed
    if( (dep_tries >= 1) && (dep_tries <= THK_MAX_TRIES_ANT_DEPLOY) ){
        dep_state = 1;
        valid_dep_tries = TRUE;
    }

    //Register only if valid_dep_tries == TRUE;
    if( valid_dep_tries == TRUE ){
        mem_eeprom_var = mem_dep_ant_deployed; value = dep_state;
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_ant_tries; value = dep_tries;
        mem_setVar(mem_eeprom_var, value);


        //RTC
        mem_eeprom_var = mem_dep_year; value = RTC_get_year();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_month; value = RTC_get_month();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_day_number; value = RTC_get_day_num();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_week_day; value = RTC_get_week_day();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_hours; value = RTC_get_hours();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_minutes; value = RTC_get_minutes();
        mem_setVar(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_seconds; value = RTC_get_seconds();
        mem_setVar(mem_eeprom_var, value);

        return 1;
    }
    else{
        return 0;
    }
}
//------------------------------------------------------------------------------

int thk_silent_time_and_pictures(void *param){
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        con_printf("\n[thk_silent_time_and_pictures] Mandatory inactivity time...\r\n");
        rtc_print(NULL);
    #endif

    //1) Silencio el TRX
    #if (SCH_TRX_ONBOARD==1)
        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            con_printf("    * Turning off TX\r\b");
        #endif

        int trx_mode = 2; //TODO: Implement trx_setmode
        trx_set_operation_mode( (void *)&trx_mode );
    #endif

    //2) tomo foto
    pay_init_camera(NULL);
    //int arg = CAM_MODE_VERBOSE;
    int arg = CAM_MODE_BOTH;
    pay_take_camera(&arg);
    pay_stop_camera(NULL);
   //parar ciclo de Payload
    //int cam_state = SRP_PAY_XXX_STATE_INACTIVE;
    PAY_xxx_State cam_state = pay_xxx_state_inactive;
    pay_set_state_camera(&cam_state);

    //3) duermo el SUCHAI por 30min
    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        con_printf("    * System halted at ");
        rtc_print(NULL);
    #endif

    int mode= *( (int *)param );
    if(mode)    /* RealTIme */
    {
//        const unsigned int time_out = (0xFFFF) / portTICK_RATE_MS; /* 65,535[s]*/
        const unsigned int time_out = (0xFFFF); /* 65,535[s]*/

        unsigned int indx2;
        for(indx2=0; indx2<THK_SILENT_TIME_MIN-1; indx2++)
        {
            //vTaskDelay(time_out);
            __delay_ms(time_out);
            ClrWdt()
        }

        con_printf("    * 65[s] remaining ...\r\n");
        //vTaskDelay(time_out);
        __delay_ms(time_out);
        ClrWdt()
    }
    else    /* NO RealTIme */
    {
//        const unsigned int time_out = (10000) / portTICK_RATE_MS; /* 10[s]*/
        const unsigned int time_out = (10000); /* 10[s]*/
        
        //vTaskDelay(time_out);
        __delay_ms(time_out);
        ClrWdt()
    }

    #if (SCH_TDEPLOYMENT_VERBOSE>=1)
        con_printf("    * System resumed at ");
        rtc_print(NULL);
        con_printf("    FINISHING SILENT TIME\r\n");
    #endif

    //4) normalizo el TRX
    #if (SCH_TRX_ONBOARD==1)
        #if (SCH_TDEPLOYMENT_VERBOSE>=2)
            con_printf("    * Turning on TX\r\b");
        #endif

        trx_mode = 5; //TODO: Implement trx_setmode
        trx_set_operation_mode( (void *)&trx_mode );
    #endif

    return 1;
}

//------------------------------------------------------------------------------
int thk_executeBeforeFlight(void *param){
    printf("thk_executeBeforeFlight()..\n");

    drp_executeBeforeFlight(NULL);
    srp_executeBeforeFlight(NULL);

    printf("****************************************************\r\n");
    printf("thk_executeBeforeFlight finalizo\r\n");
    printf("Para quedar en config de vuelo, se\r\n");
    printf("DEBE apagar el SUCHAI, hagalo ANTES de:\r\n");
    printf("****************************************************\r\n");

    int i;
    for(i=10;i>=1;i--){
        __delay_ms(1000);
        printf("%d segundos..\r\n", i);
    }

    //ppc_reset(NULL);

    return 1;
}
