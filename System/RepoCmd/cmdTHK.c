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
    thkFunction[(unsigned char)thk_id_debug] = thk_debug;

    thkFunction[(unsigned char)thk_id_executeBeforeFlight] = thk_executeBeforeFlight;
    thkFunction[(unsigned char)thk_id_debug2] = thk_debug2;
    
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
    thk_sysReq[(unsigned char)thk_id_deploy_antenna]  = CMD_SYSREQ_MIN + SCH_DEP_SYS_REQ;
    thkFunction[(unsigned char)thk_id_suchai_deployment] = thk_suchai_deployment;
    thk_sysReq[(unsigned char)thk_id_suchai_deployment]  = CMD_SYSREQ_MIN + SCH_DEP_SYS_REQ;

}

//------------------------------------------------------------------------------
int thk_debug(void *param){
    unsigned int address=0;
    unsigned int data=0x1000;

    printf("(Destructive) Testing memEEPROM\r\n");
    for(address=MEP_FIRST_ADDR; address<=MEP_LAST_ADDR; address++, data++){
        //con_printf("testing address j="); Hex16ToAscii( address); con_printf(buffer); con_printf("\n");

        printf("writing: ");
        writeIntEEPROM1(address, data);
        printf("value[%d] = %d \r\n", address, data);

        printf("reading: ");
        unsigned int res = readIntEEPROM1(address);
        printf("value[%d] = %d \r\n", address, res);

        printf("comparing: ");
        if(data==res){ printf("ok\r\n"); }
        else{ printf("fail\r\n"); }
    }

    unsigned int indxVar;
    data = -1;  //0xFFFF

    for(indxVar=MEP_FIRST_ADDR; indxVar<=MEP_LAST_ADDR; indxVar++){
        writeIntEEPROM1(indxVar, data);
    }

    return 1;
}

//-----------------------------------------------------
int thk_get_dep_ant_deployed(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_deployed;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_ant_tries(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_tries;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
//rtc
int thk_get_dep_year(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_year;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_month(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_month;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_week_day(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_week_day;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_day_number(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_day_number;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_hours(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_hours;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_minutes(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_minutes;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int thk_get_dep_seconds(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_dep_seconds;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}

//
//int thk_set_dep_ant_deployed(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_deployed;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_ant_tries(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_ant_tries;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_year(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_year;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_month(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_month;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_week_day(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_week_day;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_day_number(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_day_number;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_hours(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_hours;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_minutes(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_minutes;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//int thk_set_dep_seconds(void* param){
//    MemEEPROM_Vars mem_eeprom_var = mem_dep_seconds;
//    int value = *((int*)param);
//    writeIntEEPROM1(mem_eeprom_var, value);
//    return 1;   //se asume operacion exitosa
//}
//------------------------------------------------------------------------------
int thk_suchai_deployment(void *param)
{
    printf("[thk_suchai_deployment] Suchai deployment routine..\r\n");
    
    int delay_min = *( (int*)param );
//    portTickType xLastWakeTime = xTaskGetTickCount();
//    portTickType delay_60s    = 60000;    //Task period in [ms]
//    portTickType delay_tick_60s = delay_60s / portTICK_RATE_MS; //Task period in ticks

    unsigned long initial_tick_10ms = xTaskGetTickCount(); //get initial tick-time
    //unsigned long silent_time_10ms = (180000);     // 30 minutes = 1800 sec = 180000 [10ms]
    unsigned long silent_time_10ms = delay_min*60*100; // time_s*100 [ms] = time_ms
    unsigned long final_tick_10ms = initial_tick_10ms + silent_time_10ms;

    printf("[thk_suchai_deployment] initial_tick_10ms = %lu | final_tick_10ms = %lu \r\n", initial_tick_10ms, final_tick_10ms);

    // print rtc time
    rtc_print(NULL);

    //take picture
    #if(SCH_PAYCAM_nMEMFLASH_ONBOARD==1 )
        #if(SCH_THK_SILENT_REALTIME==1)
            pay_takePhoto_camera(NULL); //takes 10min to complete
        #else
            printf("  Jumping pay_takePhoto_camera(NULL) call, it takes 10min to complete ..\r\n");
        #endif
    #endif

    // print rtc time
    rtc_print(NULL);

    while(TRUE){
        unsigned long int cu_tick_10ms = xTaskGetTickCount();
        if( cu_tick_10ms >= final_tick_10ms ){
            printf("[thk_suchai_deployment] Waiting timeout, cu_tick_10ms = %lu\r\n", cu_tick_10ms);
            break;
        }
        printf("[thk_suchai_deployment] Waiting for timeout, cu_tick_10ms = %lu\r\n", cu_tick_10ms);
        //vTaskDelayUntil(&xLastWakeTime, delay_tick_60s); //Suspend task 60 sec
        __delay_ms(60000);  //delay 60sec
    }

    // print rtc time
    rtc_print(NULL);

    /* Deploy Antena */
    #if (SCH_ANTENNA_ONBOARD==1)
        int rt_mode = SCH_THK_ANTENNA_REALTIME; /* 1=Real Time, 0=Debug Time */
        thk_deploy_antenna(&rt_mode);
    #endif

    // print rtc time
    rtc_print(NULL);

    __delay_ms(60000);  //delay 60sec to avoid drain-out the EPS

    /* Ceploy langmuir should NOT be here, but there is no way
     * to check deployment, so its included here */
    #if (SCH_ANTENNA_ONBOARD==1)
        if( sta_get_stateVar(sta_pay_lagmuirProbe_isDeployed)==0 ){
            int rt_mode = SCH_THK_ANTENNA_REALTIME; /* 1=Real Time, 0=Debug Time */
            pay_deploy_langmuirProbe(rt_mode);    //realtime mode
            //set var lang dep = 1
        }
    #endif

    //other "only once"-tasks
    //..

    return 1;
}

#define THK_SILENT_TIME_MIN 30          ///< cuantos "minutos" (65,535[s]) estara en inactividad antes de iniciarse
#define THK_MAX_TRIES_ANT_DEPLOY 10               ///< cuantas veces tratara desplegar la antena antes de anunciar fracaso
#define THK_DEPLOY_TIME 45311          ///< 2*THK_DEPLOY_TIME/1000 indica cuantos "ms" estara activo el bus de 3.3V quemando el nilon
#define THK_REST_DEPLOY_TIME 5000       ///< cuantos "ms" estara inactivo el bus de 3.3V descanzando de tratar de quemar el nilon
#define THK_RECHECK_TIME 2000           ///< despues de cuantos "ms" RE-chequeara que efectivamente se desplego la antena
/**
 * Deploys satellite antennas
 * @param param 1 realime, 0 debug time
 * @return 1 success, 0 fails
 */
int thk_deploy_antenna(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
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
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("    [Deploying] Attempt #%d\r\n", tries_indx);
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

            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
//            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */
            __delay_ms(delay_rest_dep_time);
            ClrWdt();

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

            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
//            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */
            __delay_ms(delay_rest_dep_time);
            ClrWdt();

            if( thk_get_AntSwitch_isOpen(&delay_recheck_dep_time) == 1 )
            {
                thk_deployment_registration(&tries_indx);

                #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
                    printf("    ANTENNA DEPLOYED SUCCESSFULLY [%d TRIES]\r\n", tries_indx);
                    rtc_print(NULL);
                #endif
                return 1;
            }
        }
    }
    #endif

    //after the for() tries_indx == THK_MAX_TRIES_ANT_DEPLOY+1
    tries_indx = THK_MAX_TRIES_ANT_DEPLOY+1; //por si acaso
    thk_deployment_registration(&tries_indx);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    ANTENNA DEPLOY FAIL [%d TRIES]\r\n", THK_MAX_TRIES_ANT_DEPLOY);
        rtc_print(NULL);
    #endif

    return 0;
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
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_ant_tries; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);

        //RTC
        mem_eeprom_var = mem_dep_year; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_month; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_day_number; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_week_day; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_hours; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_minutes; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
        
        mem_eeprom_var = mem_dep_seconds; value = 0;
        writeIntEEPROM1(mem_eeprom_var, value);
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
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_ant_tries; value = dep_tries;
        writeIntEEPROM1(mem_eeprom_var, value);


        //RTC
        mem_eeprom_var = mem_dep_year; value = RTC_get_year();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_month; value = RTC_get_month();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_day_number; value = RTC_get_day_num();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_week_day; value = RTC_get_week_day();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_hours; value = RTC_get_hours();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_minutes; value = RTC_get_minutes();
        writeIntEEPROM1(mem_eeprom_var, value);

        mem_eeprom_var = mem_dep_seconds; value = RTC_get_seconds();
        writeIntEEPROM1(mem_eeprom_var, value);

        return 1;
    }
    else{
        return 0;
    }
}
//------------------------------------------------------------------------------

int thk_silent_time_and_pictures(void *param){
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        con_printf("\n[thk_silent_time_and_pictures] Mandatory inactivity time...\r\n");
        rtc_print(NULL);
    #endif

    //1) Silencio el TRX
    #if (SCH_TRX_ONBOARD==1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
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
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
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

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        con_printf("    * System resumed at ");
        rtc_print(NULL);
        con_printf("    FINISHING SILENT TIME\r\n");
    #endif

    //4) normalizo el TRX
    #if (SCH_TRX_ONBOARD==1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            con_printf("    * Turning on TX\r\b");
        #endif

        trx_mode = 5; //TODO: Implement trx_setmode
        trx_set_operation_mode( (void *)&trx_mode );
    #endif

    return 1;
}
//------------------------------------------------------------------------------
int thk_debug2(void *param){
    rtc_print(NULL);
    
    printf("Bus Hardware (initialized in dep_init_bus_hw)..\r\n");
    /* this info is updated at start-up vy dep_init_bus_hw()..*/
    printf("PPC_MB_nOE_USB_nINT_CHECK = %d \r\n", PPC_MB_nOE_USB_nINT_CHECK );
    printf("PPC_MB_nOE_MHX_CHECK = %d \r\n", PPC_MB_nOE_MHX_CHECK );
    printf("PPC_MB_nON_MHX_CHECK = %d \r\n", PPC_MB_nON_MHX_CHECK );
    printf("PPC_MB_nON_SD_CHECK = %d \r\n", PPC_MB_nON_SD_CHECK );
    printf("sta_RTC_isAlive = %d \r\n", sta_get_stateVar(sta_RTC_isAlive) );
    printf("sta_TRX_isAlive = %d \r\n", sta_get_stateVar(sta_TRX_isAlive) );
    printf("sta_EPS_isAlive = %d \r\n", sta_get_stateVar(sta_EPS_isAlive) );
    printf("sta_MemEEPROM_isAlive = %d \r\n", sta_get_stateVar(sta_MemEEPROM_isAlive) );
    printf("sta_MemSD_isAlive = %d \r\n", sta_get_stateVar(sta_MemSD_isAlive) );
    printf("sta_SUCHAI_isDeployed = %d \r\n", sta_get_stateVar(sta_AntSwitch_isOpen) );
    #if (SCH_ANTENNA_ONBOARD==1)
        int ant12 = PPC_ANT12_CHECK;
        printf("PPC_ANT12_CHECK = %d => ", ant12 );
        if(ant12==1){
            printf("Antenna NOT deployed \r\n");
        }
        else{
            printf("Antenna deployed \r\n");
        }
    #endif
    printf("******************************************\r\n");

    printf("Payload Hardware (initialized by each pay_init_xxx)..\r\n");
    /* this info is updated at start-up by dep_init_bus_hw()..*/
    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        pay_init_sensTemp(NULL);
        pay_stop_sensTemp(NULL);
    #endif
    #if (SCH_PAY_GYRO_ONBOARD==1)
        pay_init_gyro(NULL);
        pay_stop_gyro(NULL);
    #endif
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        pay_init_camera(NULL);
        pay_stop_camera(NULL);

    #endif
    #if (SCH_PAY_GPS_ONBOARD==1)
        pay_init_gps(NULL);
        pay_stop_gps(NULL);
    #endif
    #if (SCH_PAY_FIS_ONBOARD==1)
        pay_init_expFis(NULL);
        pay_stop_expFis(NULL);
    #endif
    #if (SCH_PAY_BATTERY_ONBOARD==1)
        pay_init_battery(NULL);
        printf("  sta_pay_test1_isAlive = %d \r\n", sta_get_stateVar(sta_pay_battery_isAlive) );
    #endif
    #if (SCH_PAY_DEBUG_ONBOARD==1)
        pay_init_debug(NULL);
        printf("  sta_pay_test2_isAlive = %d \r\n", sta_get_stateVar(sta_pay_debug_isAlive) );
    #endif
    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        pay_init_lagmuirProbe(NULL);
        pay_stop_lagmuirProbe(NULL);
    #endif
    pay_init_tmEstado(NULL);
    printf("******************************************\r\n");

    rtc_print(NULL);

    //thk_executeBeforeFlight(NULL);
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

    ppc_reset(NULL);

    return 1;
}
//------------------------------------------------------------------------------
//int thk_periodicUpdate_STA_stateVar(void *param){
//    #if (SCH_CMDDRP_VERBOSE>=1)
//        printf("thk_periodicUpdate_STA_stateVar()..\r\n");
//    #endif
//
//    #if (SCH_EPS_ONBOARD==1)
//        updateSOCEPS();
//        updateStatusEPS();
//    #else
//        sta_setstateVar(sta_eps_soc, CMD_SYSREQ_MAX);
//    #endif
//
//    int res, nVarUpdt=0;
//    STA_stateVar indxVar;
//    for(indxVar=0;indxVar<sta_stateVar_last_one;indxVar++){
//        res = thk_stateVar_update(indxVar);
//        if(res == 1){ nVarUpdt++; }
//    }
//    printf("  updated state variables (STA_stateVar) = %d/%d\r\n", nVarUpdt, sta_stateVar_last_one);
//
//    return 1;
//}
//int thk_stateVar_update(int indxVar){
//    int res;
//
//    /* Aquellas variables que no se actualicen aca son aquellas que:
//    * 1) Se actualizan solo en la inicializacion del sistema (taskDeployment o en algun onReset anidado)
//    * 2) Son actualizadas por alguna tarea (taskComunication, taskHousekeeping, taskFlightPlan..)
//    */
//
//    switch(indxVar){
//        //PPC => (C&DH subsystem)
//        case sta_ppc_wdt: // 1=WDT Active, 0=WDT Inactive
//            thk_ppc_enwdt();
//            res = 1;
//            break;
//        case sta_ppc_osc: // holds Current Oscillator
//            thk_ppc_osc();
//            res = 1;
//            break;
//        case sta_ppc_MB_nOE_USB_nINT_stat:
//            thk_ppc_MB_nOE_USB_nINT_stat();
//            res = 1;
//            break;
//        case sta_ppc_MB_nOE_MHX_stat:
//            thk_ppc_MB_nOE_MHX_stat();
//            res = 1;
//            break;
//        case sta_ppc_MB_nON_MHX_stat:
//            thk_ppc_MB_nON_MHX_stat();
//            res = 1;
//            break;
//        case sta_ppc_MB_nON_SD_stat:
//            thk_ppc_MB_nON_SD_stat();
//            res = 1;
//            break;
//
//        //RTC => (C&DH subsystem)
//        case sta_rtc_year:
//            thk_rtc_year();
//            res = 1;
//            break;
//        case sta_rtc_month:
//            thk_rtc_month();
//            res = 1;
//            break;
//        case sta_rtc_week_day:
//            thk_rtc_week_day();
//            res = 1;
//            break;
//        case sta_rtc_day_number:
//            thk_rtc_day_number();
//            res = 1;
//            break;
//        case sta_rtc_hours:
//            thk_rtc_hours();
//            res = 1;
//            break;
//        case sta_rtc_minutes:
//            thk_rtc_minutes();
//            res = 1;
//            break;
//        case sta_rtc_seconds:
//            thk_rtc_seconds();
//            res = 1;
//            break;
//
//        //EPS => (Energy subsystem)
//    #if SCH_EPS_ONBOARD==1
//        case sta_eps_bat0_voltage:
//            thk_eps_bat0_voltage();
//            res = 1;
//            break;
//        case sta_eps_bat0_current:
//            thk_eps_bat0_current();
//            res = 1;
//            break;
//        case sta_eps_bus5V_current:
//            thk_eps_bus5V_current();
//            res = 1;
//            break;
//        case sta_eps_bus3V_current:
//            thk_eps_bus3V_current();
//            res = 1;
//            break;
//        case sta_eps_bus_battery_current:
//            thk_eps_bus_battery_current();
//            res = 1;
//            break;
//        case sta_eps_bat0_temp:
//            thk_eps_bat0_temp();
//            res = 1;
//            break;
//        case sta_eps_panel_pwr:
//            thk_eps_panel_pwr();
//            res = 1;
//            break;
//        case sta_eps_status:
//            thk_eps_status();
//            res = 1;
//            break;
//        case sta_eps_soc:
//            thk_eps_soc();
//            res = 1;
//            break;
//        case sta_eps_socss:
//            thk_eps_socss();
//            res = 1;
//            break;
//        case sta_eps_state_flag:
//            thk_eps_state_flag();
//            res = 1;
//            break;
//        case sta_eps_charging:
//            thk_eps_charging();
//            res = 1;
//            break;
//    #endif
//
//    #if (SCH_TRX_ONBOARD==1)
//        //TRX => (Communication subsystem)
//        case sta_trx_opmode:
//            thk_trx_opmode();
//            res = 1;
//            break;           // Operation mode
//        case sta_trx_temp_hpa:
//            thk_trx_temp_hpa();
//            res = 1;
//            break;         // Temp of HPA
//        case sta_trx_temp_mcu:
//            thk_trx_temp_mcu();
//            res = 1;
//            break;         // Temp of MCU
//        case sta_trx_rssi:
//            thk_trx_rssi();
//            res = 1;
//            break;             // RSSI Signal level
//        case sta_trx_status_tc:
//            thk_trx_status_tc();
//            res = 1;
//            break;        // Status Register of TC
//    #endif
//
//        //On default (para aquellos que no tienen un periodicUpdate asociado)
//        default:
//            //do nothing on default
//            res = 0;
//            break;
//    }
//
//    return res;
//}
//
////------------------------------------------------------------------------------
//
///*------------------------------------------------------------------------------
// *                                  DRP PPC ENWDT
// *------------------------------------------------------------------------------
// * Description        : Update the state of the WDT
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_enwdt(void)
//{
//    /* Leer el registro de los CONFIGWORD no es directo
//     * (esta "beyond user visibility pages"). Asi es que la
//     * implementacion de este comando es asumir que se parte en WDT_ENABLE
//     *  y luego registrar las desactivaciones/activaciones del WDT a traves
//     * de ppc_enwdt (ojo: no es lo mismo que thk_ppc_enwdt )*/
//}
///*------------------------------------------------------------------------------
// *                                  DRP PPC OSC
// *------------------------------------------------------------------------------
// * Description        : Update current oscillator
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_osc(void)
//{
//    /* FRC_OSC                           0x0000 //Internal oscillator
//    *  FRC_OSC_WITH_POSTSCALER_PLL       0x0001 //Internal oscillator with Postscalar PLL enabled
//    *  PRIMARY_OSC                       0x0002 //Primary oscillator
//    *  PRIMARY_OSC_WITH_PLL              0x0003 //Primary oscillator with PLL
//    *  SECONDRY_OSC                      0x0004 //Secondary oscillator
//    *  LOW_POWER_RC                      0x0005 //Low Power RC oscillator
//    *  FRC_OSC_WITH_POSTSCALER           0x0007 //Internal oscillator with postscalar
//     */
//    sta_setstateVar(sta_ppc_osc, (int)Current_OSCILLATOR() );
//}
///*------------------------------------------------------------------------------
// *                                  PPC LAST RESET SOURCE
// *------------------------------------------------------------------------------
// * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_MB_nOE_USB_nINT_stat(void)
//{
//    if(PPC_MB_nOE_USB_nINT_CHECK)
//    {
//        sta_setstateVar(sta_ppc_MB_nOE_USB_nINT_stat, 1);
//    }
//    else
//    {
//        sta_setstateVar(sta_ppc_MB_nOE_USB_nINT_stat, 0);
//    }
//}
///*------------------------------------------------------------------------------
// *                                  thk_ppc_MB_nOE_MHX_stat
// *------------------------------------------------------------------------------
// * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_MB_nOE_MHX_stat(void)
//{
//    if(PPC_MB_nOE_MHX_CHECK)
//    {
//        sta_setstateVar(sta_ppc_MB_nOE_MHX_stat, 1);
//    }
//    else
//    {
//        sta_setstateVar(sta_ppc_MB_nOE_MHX_stat, 0);
//    }
//
//}
///*------------------------------------------------------------------------------
// *                                  thk_ppc_MB_nON_MHX_stat
// *------------------------------------------------------------------------------
// * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_MB_nON_MHX_stat(void)
//{
//    if(PPC_MB_nON_MHX_CHECK)
//    {
//        sta_setstateVar(sta_ppc_MB_nON_MHX_stat, 1);
//    }
//    else
//    {
//        sta_setstateVar(sta_ppc_MB_nON_MHX_stat, 0);
//    }
//
//}
///*------------------------------------------------------------------------------
// *                                  thk_ppc_MB_nON_SD_stat
// *------------------------------------------------------------------------------
// * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_ppc_MB_nON_SD_stat(void)
//{
//    if(PPC_MB_nON_SD_CHECK)
//    {
//        sta_setstateVar(sta_ppc_MB_nON_SD_stat, 1);
//    }
//    else
//    {
//        sta_setstateVar(sta_ppc_MB_nON_SD_stat, 0);
//    }
//
//}
///*------------------------------------------------------------------------------
// *                                  thk_rtc_year
// *------------------------------------------------------------------------------
// * Description        : Updates the RTC structure inside Cubestat
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_rtc_year(void)
//{
//    sta_setstateVar(sta_rtc_year, RTC_get_year() );
//}
//void thk_rtc_month(void)
//{
//    sta_setstateVar(sta_rtc_month, RTC_get_month() );
//}
//void thk_rtc_week_day(void)
//{
//    sta_setstateVar(sta_rtc_week_day, RTC_get_week_day() );
//}
//void thk_rtc_day_number(void)
//{
//    sta_setstateVar(sta_rtc_day_number, RTC_get_day_num() );
//}
//void thk_rtc_hours(void)
//{
//    sta_setstateVar(sta_rtc_hours, RTC_get_hours() );
//}
//void thk_rtc_minutes(void)
//{
//    sta_setstateVar(sta_rtc_minutes, RTC_get_minutes() );
//}
//void thk_rtc_seconds(void)
//{
//    sta_setstateVar(sta_rtc_seconds, RTC_get_seconds() );
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_bat0_voltage
// *------------------------------------------------------------------------------
// * Description        : Updates the battery voltage based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bat0_voltage(void)
//{
//    sta_setstateVar(sta_eps_bat0_voltage, readEPSvars(EPS_ID_bat0_voltage));
//
//}
//
///*------------------------------------------------------------------------------
// *                                  thk_eps_bat0_current
// *------------------------------------------------------------------------------
// * Description        : Updates the battery current based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bat0_current(void)
//{
//    sta_setstateVar(sta_eps_bat0_current, readEPSvars(EPS_ID_bat0_current));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_bus5V_current
// *------------------------------------------------------------------------------
// * Description        : Updates the 5V bus current based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bus5V_current(void)
//{
//    sta_setstateVar(sta_eps_bus5V_current, readEPSvars(EPS_ID_bus5V_current));
//}
///*------------------------------------------------------------------------------
// *                                   thk_eps_bus3V_current
// *------------------------------------------------------------------------------
// * Description        : Updates the 3V bus current based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bus3V_current(void)
//{
//    sta_setstateVar(sta_eps_bus3V_current, readEPSvars(EPS_ID_bus3V_current));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_bus_battery_current
// *------------------------------------------------------------------------------
// * Description        : Updates the battery bus current based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bus_battery_current(void)
//{
//    sta_setstateVar(sta_eps_bus_battery_current, readEPSvars(EPS_ID_bus_battery_current));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_bat0_temp
// *------------------------------------------------------------------------------
// * Description        : Updates the battery temp based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_bat0_temp(void)
//{
//    sta_setstateVar(sta_eps_bat0_temp, readEPSvars(EPS_ID_bat0_temp));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_panel_pwr
// *------------------------------------------------------------------------------
// * Description        : Updates the power sourced by the panels based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_panel_pwr(void)
//{
//    sta_setstateVar(sta_eps_panel_pwr, readEPSvars(EPS_ID_panel_pwr));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_status
// *------------------------------------------------------------------------------
// * Description        : Updates the EPS status based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_status(void)
//{
//    sta_setstateVar(sta_eps_status, readEPSvars(EPS_ID_status));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_soc
// *------------------------------------------------------------------------------
// * Description        : Updates the battery soc estimation based on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_soc(void)
//{
//    int soc = readEPSvars(EPS_ID_soc);
//    sta_setstateVar(sta_eps_soc, soc);
//
//    #if (SCH_CMDDRP_VERBOSE >=1)
//        char buf[10];
//        itoa(buf, soc, 10);
//        con_printf("SOC= "); con_printf(buf); con_printf("\r\n");
//    #endif
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_socss
// *------------------------------------------------------------------------------
// * Description        : Updates the battery "steady state" soc estimation based
// *                      on the last meassurement
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_socss(void)
//{
//    sta_setstateVar(sta_eps_socss, readEPSvars(EPS_ID_socss));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_state_flag
// *------------------------------------------------------------------------------
// * Description        : Updates the state flag that have the following meaning
// *                      state_flag = 1 : max DOD was reached so high current
// *                                       commands won't be excecuted.
// *                      state_flag = 2 : After a state_flag=1, a safe DOD was
// *                                       reached so high current commands are
// *                                       allowed
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_state_flag(void)
//{
//    sta_setstateVar(sta_eps_state_flag, readEPSvars(EPS_ID_state_flag));
//}
///*------------------------------------------------------------------------------
// *                                  thk_eps_charging
// *------------------------------------------------------------------------------
// * Description        : Updates the battery "charging state"
// * Arguments          : None
// * Return Value       : int: 1=Always successfull
// *----------------------------------------------------------------------------*/
//void thk_eps_charging(void)
//{
//    int i_value = readEPSvars(EPS_ID_current_dir);
//    i_value = i_value < 256 ? 1:0;
//    sta_setstateVar(sta_eps_charging, i_value);
//}
///*------------------------------------------------------------------------------
// *		 	DRP TRX OPMODE
// *------------------------------------------------------------------------------
// * Description        : Update TRX Operation Mode status in data repository
// * Arguments          : void
// * Return Value       : 1 - OK, 0 - FAIL
// * ID                 : 0x5013
// *----------------------------------------------------------------------------*/
//void thk_trx_opmode(void)
//{
//    /* TODO: Reading OP_MODE */
//    char char_value = 0;
////    char_value = TRX_ReadRegister(TRX_MODE);
//    /* Writing TRX_FTX to repo */
//    sta_setstateVar(sta_trx_opmode, (int)char_value);
//
//
//}
///*------------------------------------------------------------------------------
// *		 	DRP TRX TEMP
// *------------------------------------------------------------------------------
// * Description        : Update TRX TEMP_MCU and TEMP_HPA status in data
// *                      repository
// * Arguments          : void
// * Return Value       : 1 - OK, 0 - FAIL
// * ID                 : 0x5014
// *----------------------------------------------------------------------------*/
//void thk_trx_temp_hpa(void)
//{
//    int int_value = -1;
//
//    /*TODO: Reading TEMP_HPA */
////    int_value = (int)TRX_ReadRegister(TRX_TEMPHPA_H)<<8;
////    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPHPA_L);
//    /* Writing TEMP_HPA to repo */
//    sta_setstateVar(sta_trx_temp_hpa, int_value);
//
//}
///*------------------------------------------------------------------------------
// *		 	DRP TRX TEMP_MCU
// *------------------------------------------------------------------------------
// * Description        : Update TRX TEMP_MCU and TEMP_HPA status in data
// *                      repository
// * Arguments          : void
// * Return Value       : 1 - OK, 0 - FAIL
// * ID                 : 0x5014
// *----------------------------------------------------------------------------*/
//void thk_trx_temp_mcu(void)
//{
//    int int_value = -1;
//
//    /* TODO: Reading TEMP_MCU */
////    int_value = (int)TRX_ReadRegister(TRX_TEMPMCU_H)<<8;
////    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPMCU_L);
//    /* Writing TEMP_MCU to repo */
//    sta_setstateVar(sta_trx_temp_mcu, int_value);
//
//
//}
///*------------------------------------------------------------------------------
// *		 	DRP TRX RSSI
// *------------------------------------------------------------------------------
// * Description        : Update TRX RSSI status in data repository
// * Arguments          : void
// * Return Value       : 1 - OK, 0 - FAIL
// * ID                 : 0x5015
// *----------------------------------------------------------------------------*/
//void thk_trx_rssi(void)
//{
//    srp_update_STA_stateVar_trx_rssi(NULL);
//}
///*------------------------------------------------------------------------------
// *		 	DRP TRX STATUS TC
// *------------------------------------------------------------------------------
// * Description        : Update TRX FRX status in data repository
// * Arguments          : void
// * Return Value       : 1 - OK, 0 - FAIL
// * ID                 : 0x5018
// *----------------------------------------------------------------------------*/
//void thk_trx_status_tc(void)
//{
//    int value = 0;
//    /* TODO: Reading STATUS_TC */
////    value = TRX_CheckNewTC();
//    /* Writing BEACON_PWR to repo */
//    sta_setstateVar(sta_trx_status_tc, value);
//}
//
////------------------------------------------------------------------------------
//
//
