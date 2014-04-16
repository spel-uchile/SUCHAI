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

    //De display
    thkFunction[(unsigned char)thk_id_debug] = thk_debug;
    thk_sysReq[(unsigned char)thk_id_debug]  = CMD_SYSREQ_MIN;
    //De update en general en sta_CubesatVar
    thkFunction[(unsigned char)thk_id_periodicUpdate_STA_CubesatVar] = thk_periodicUpdate_STA_CubesatVar;
    thk_sysReq[(unsigned char)thk_id_periodicUpdate_STA_CubesatVar]  = CMD_SYSREQ_MIN;

    thkFunction[(unsigned char)thk_id_executeBeforeFlight] = thk_executeBeforeFlight;
    thk_sysReq[(unsigned char)thk_id_executeBeforeFlight]  = CMD_SYSREQ_MIN;
    thkFunction[(unsigned char)thk_id_state_hw] = thk_state_hw;
    thk_sysReq[(unsigned char)thk_id_state_hw]  = CMD_SYSREQ_MIN;

    thkFunction[(unsigned char)thk_id_deploy_antenna] = thk_deploy_antenna;
    thk_sysReq[(unsigned char)thk_id_deploy_antenna]  = CMD_SYSREQ_MIN + SCH_DEP_SYS_REQ;
    thkFunction[(unsigned char)thk_id_silent_time_and_pictures] = thk_silent_time_and_pictures;
    thk_sysReq[(unsigned char)thk_id_silent_time_and_pictures]  = CMD_SYSREQ_MIN;
}

//------------------------------------------------------------------------------
int thk_debug(void *param){
    unsigned int address=0;
    unsigned int data=0x1000;

    printf("(Destructive) Testing memEEPROM\r\n");
    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++, data++){
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
    thk_eraseAll_CubesatVar();

    return 1;
}
//------------------------------------------------------------------------------
#define THK_SILENT_TIME_MIN 30          ///< cuantos "minutos" (65,535[s]) estara en inactividad antes de iniciarse
#define THK_TRY_DEPLOY 10               ///< cuantas veces tratara desplegar la antena antes de anunciar fracaso
#define THK_DEPLOY_TIME 0xB0FF          ///< 2*THK_DEPLOY_TIME/1000 indica cuantos "s" estara activo el bus de 3.3V quemando el nilon
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
    #endif

    //Realtime=1 DebugTime=0
    unsigned int delay_dep_time, delay_rest_dep_time, delay_recheck_dep_time;
    int mode= *( (int *)param );
    if(mode)
    {
        delay_dep_time = (THK_DEPLOY_TIME) / portTICK_RATE_MS;
        delay_rest_dep_time = (THK_REST_DEPLOY_TIME) / portTICK_RATE_MS;
        delay_recheck_dep_time = (THK_RECHECK_TIME) / portTICK_RATE_MS;
    }
    else
    {
        delay_dep_time = (600) / portTICK_RATE_MS;
        delay_rest_dep_time = (400) / portTICK_RATE_MS;
        delay_recheck_dep_time = (200) / portTICK_RATE_MS;
    }


//    if( thk_check_antenna_isDeployed(delay_recheck_dep_time)==TRUE )
//    {
//        #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
//            printf("    * Antenna is already deployed\r\n");
//        #endif
//        return 1;
//    }
    
    //intentos quemando el nylon
    int tries_indx = 0;

    #if(SCH_ANTENNA_ONBOARD == 1)
    {
        for(tries_indx=1; tries_indx<=THK_TRY_DEPLOY; tries_indx++)
        {
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("    [Deploying] Attempt #%d\r\n", tries_indx);
            #endif

            PPC_ANT12_SWITCH=1;
            PPC_ANT1_SWITCH=1;
            PPC_ANT2_SWITCH=0;
            //PPC_ANT1_SWITCH=0;
            //PPC_ANT2_SWITCH=1;
            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT1 */
            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT1 */

            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */

            PPC_ANT12_SWITCH=1;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=1;
            //PPC_ANT1_SWITCH=1;
            //PPC_ANT2_SWITCH=0;
            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT2 */
            vTaskDelay(delay_dep_time);   /* tiempo de intento ANT2 */

            PPC_ANT12_SWITCH=0;
            PPC_ANT1_SWITCH=0;
            PPC_ANT2_SWITCH=0;
            vTaskDelay(delay_rest_dep_time);   /* tiempo de descanso */


            if( thk_check_antenna_isDeployed(delay_recheck_dep_time)==TRUE )
            {
                #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
                    printf("    ANTENNA DEPLOYED SUCCESSFULLY [%d TRIES]\r\n", tries_indx);
                #endif

                srp_deployment_registration(&tries_indx);
                return 1;
            }
        }
    }
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    ANTENNA DEPLOY FAIL [%d TRIES]\r\n", THK_TRY_DEPLOY);
    #endif

    tries_indx = -tries_indx;
    srp_deployment_registration(&tries_indx);

    return 0;
}
BOOL thk_check_antenna_isDeployed(unsigned int delay_recheck_dep_time){
    if(PPC_ANT12_CHECK==0)   /* reviso */
    {
        vTaskDelay(delay_recheck_dep_time);   /* tiempo de RE-chequeo */
        if(PPC_ANT12_CHECK==0)   /* RE-reviso */
        {
            return TRUE;
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------------
int thk_silent_time_and_pictures(void *param){
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        con_printf("\n[thk_silent_time_and_pictures] Mandatory inactivity time...\r\n");
    #endif

    //1) Silencio el TRX
    #if (SCH_TRX_ONBOARD==1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            con_printf("    * Turning off TX\r\b");
        #endif

        /* A delay before config TRX */
        const unsigned long Delaytrx = 1000 / portTICK_RATE_MS;
        vTaskDelay(Delaytrx);

        int trx_mode=2;
        trx_setmode( (void *)&trx_mode );
    #endif

    //Y 2) duermo el SUCHAI por 30min
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        con_printf("    * System halted at ");
        rtc_print(NULL);
    #endif

    int mode= *( (int *)param );
    if(mode)    /* RealTIme */
    {
        const unsigned int time_out = (0xFFFF) / portTICK_RATE_MS; /* 65,535[s]*/

        unsigned int indx2;
        for(indx2=0; indx2<THK_SILENT_TIME_MIN-1; indx2++)
        {
            vTaskDelay(time_out);
        }

        con_printf("    * 65[s] remaining ...\r\n");
        vTaskDelay(time_out);
    }
    else    /* NO RealTIme */
    {
        const unsigned int time_out = (10000) / portTICK_RATE_MS; /* 10[s]*/
        vTaskDelay(time_out);
    }

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        con_printf("    * System resumed at ");
        rtc_print(NULL);
        con_printf("    FINISHING SILENT TIME\r\n");
    #endif

    return 1;
}
//------------------------------------------------------------------------------
int thk_state_hw(void *param){

    printf("Bus Hardware (initialized in dep_init_bus_hw)..\r\n");
    /* this info is updated at start-up vy dep_init_bus_hw()..*/
    printf("PPC_MB_nOE_USB_nINT_CHECK = %d \r\n", PPC_MB_nOE_USB_nINT_CHECK );
    printf("PPC_MB_nOE_MHX_CHECK = %d \r\n", PPC_MB_nOE_MHX_CHECK );
    printf("PPC_MB_nON_MHX_CHECK = %d \r\n", PPC_MB_nON_MHX_CHECK );
    printf("PPC_MB_nON_SD_CHECK = %d \r\n", PPC_MB_nON_SD_CHECK );
    printf("sta_RTC_isAlive = %d \r\n", sta_getCubesatVar(sta_RTC_isAlive) );
    printf("sta_TRX_isAlive = %d \r\n", sta_getCubesatVar(sta_TRX_isAlive) );
    printf("sta_EPS_isAlive = %d \r\n", sta_getCubesatVar(sta_EPS_isAlive) );
    printf("sta_MemEEPROM_isAlive = %d \r\n", sta_getCubesatVar(sta_MemEEPROM_isAlive) );
    printf("sta_MemSD_isAlive = %d \r\n", sta_getCubesatVar(sta_MemSD_isAlive) );
    printf("sta_SUCHAI_isDeployed = %d \r\n", sta_getCubesatVar(sta_SUCHAI_isDeployed) );
    int ant12 = PPC_ANT12_CHECK;
    printf("PPC_ANT12_CHECK = %d \r\n", ant12 );
    printf("******************************************\r\n");

    printf("Payload Hardware (initialized by each pay_init_xxx)..\r\n");
    /* this info is updated at start-up by dep_init_bus_hw()..*/
    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        pay_init_sensTemp(NULL);
        printf("  sta_pay_sensTemp_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_sensTemp_isAlive) );
    #endif
    #if (SCH_PAY_GYRO_ONBOARD==1)
        pay_init_gyro(NULL);
        printf("  sta_pay_gyro_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_gyro_isAlive) );
        printf("  PPC_GYRO_INT2_CHECK = %d \r\n", PPC_GYRO_INT2_CHECK );
    #endif
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        pay_init_camera(NULL);
        printf("  sta_pay_camera_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_camera_isAlive) );
        printf("  PPC_CAM_HOLD_CHECK = %d \r\n", PPC_CAM_HOLD_CHECK );
    #endif
    #if (SCH_PAY_GPS_ONBOARD==1)
        pay_init_gps(NULL);
        printf("  sta_pay_gps_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_gps_isAlive) );
    #endif
    #if (SCH_PAY_FIS_ONBOARD==1)
        pay_init_expFis(NULL);
        printf("  sta_pay_expFis_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_expFis_isAlive) );
    #endif
    #if (SCH_PAY_TEST1_ONBOARD==1)
        pay_init_test1(NULL);
        printf("  sta_pay_test1_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_test1_isAlive) );
    #endif
    #if (SCH_PAY_TEST2_ONBOARD==1)
        pay_init_test2(NULL);
        printf("  sta_pay_test2_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_test2_isAlive) );
    #endif
    #if (SCH_PAY_LAGMUIR_ONBOARD==1)
        pay_init_lagmuirProbe(NULL);
        printf("  sta_pay_lagmuirProbe_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_lagmuirProbe_isAlive) );
    #endif
    pay_init_tmEstado(NULL);
    printf("  sta_pay_tmEstado_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_tmEstado_isAlive) );
    printf("******************************************\r\n");

    thk_executeBeforeFlight(NULL);
    return 1;
}
//------------------------------------------------------------------------------
int thk_executeBeforeFlight(void *param){
    con_printf("thk_executeBeforeFlight()..\n");

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

    return 1;
}
void thk_STA_CubesatVar_EBF(void){
    sta_onResetStatRepo();

    sta_setCubesatVar(sta_SUCHAI_isDeployed, 0);  //First time on!
    /* Es la unica variable que debe gatillar las acciones de despliegue
     * como quemar antenas, sacar fotos lenzamiento, quemar langmuir, etc.
     */
}
void thk_eraseAll_CubesatVar(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("    Erasing memEEPROM\n");
    #endif

    unsigned int indxVar;
    int data = -1;  //oxFFFF

    for(indxVar=0; indxVar<sta_cubesatVar_last_one; indxVar++){
        sta_setCubesatVar(indxVar, data);
    }
}
//------------------------------------------------------------------------------
int thk_periodicUpdate_STA_CubesatVar(void *param){
    #if (SCH_CMDDRP_VERBOSE>=1)
        printf("thk_periodicUpdate_STA_CubesatVar()..\r\n");
    #endif

    #if (SCH_EPS_ONBOARD==1)
        updateSOCEPS();
        updateStatusEPS();
    #else
        sta_setCubesatVar(sta_eps_soc, CMD_SYSREQ_MAX);
    #endif

    int res, nVarUpdt=0;
    STA_CubesatVar indxVar;
    for(indxVar=0;indxVar<sta_cubesatVar_last_one;indxVar++){
        res = thk_CubesatVar_update(indxVar);
        if(res == 1){ nVarUpdt++; }
    }
    printf("  updated state variables (STA_CubesatVar) = %d/%d\r\n", nVarUpdt, sta_cubesatVar_last_one);

    return 1;
}
int thk_CubesatVar_update(int indxVar){
    int res;

    /* Aquellas variables que no se actualicen aca son aquellas que:
    * 1) Se actualizan solo en la inicializacion del sistema (taskDeployment o en algun onReset anidado)
    * 2) Son actualizadas por alguna tarea (taskComunication, taskHousekeeping, taskFlightPlan..)
    */

    switch(indxVar){
        //PPC => (C&DH subsystem)
        case sta_ppc_enwdt: // 1=WDT Active, 0=WDT Inactive
            thk_ppc_enwdt();
            res = 1;
            break;
        case sta_ppc_osc: // holds Current Oscillator
            thk_ppc_osc();
            res = 1;
            break;
        case sta_ppc_MB_nOE_USB_nINT_stat:
            thk_ppc_MB_nOE_USB_nINT_stat();
            res = 1;
            break;
        case sta_ppc_MB_nOE_MHX_stat:
            thk_ppc_MB_nOE_MHX_stat();
            res = 1;
            break;
        case sta_ppc_MB_nON_MHX_stat:
            thk_ppc_MB_nON_MHX_stat();
            res = 1;
            break;
        case sta_ppc_MB_nON_SD_stat:
            thk_ppc_MB_nON_SD_stat();
            res = 1;
            break;

        //RTC => (C&DH subsystem)
        case sta_rtc_year:
            thk_rtc_year();
            res = 1;
            break;
        case sta_rtc_month:
            thk_rtc_month();
            res = 1;
            break;
        case sta_rtc_week_day:
            thk_rtc_week_day();
            res = 1;
            break;
        case sta_rtc_day_number:
            thk_rtc_day_number();
            res = 1;
            break;
        case sta_rtc_hours:
            thk_rtc_hours();
            res = 1;
            break;
        case sta_rtc_minutes:
            thk_rtc_minutes();
            res = 1;
            break;
        case sta_rtc_seconds:
            thk_rtc_seconds();
            res = 1;
            break;

        //EPS => (Energy subsystem)
    #if SCH_EPS_ONBOARD==1            
        case sta_eps_bat0_voltage:
            thk_eps_bat0_voltage();
            res = 1;
            break;
        case sta_eps_bat0_current:
            thk_eps_bat0_current();
            res = 1;
            break;
        case sta_eps_bus5V_current:
            thk_eps_bus5V_current();
            res = 1;
            break;
        case sta_eps_bus3V_current:
            thk_eps_bus3V_current();
            res = 1;
            break;
        case sta_eps_bus_battery_current:
            thk_eps_bus_battery_current();
            res = 1;
            break;
        case sta_eps_bat0_temp:
            thk_eps_bat0_temp();
            res = 1;
            break;
        case sta_eps_panel_pwr:
            thk_eps_panel_pwr();
            res = 1;
            break;
        case sta_eps_status:
            thk_eps_status();
            res = 1;
            break;
        case sta_eps_soc:
            thk_eps_soc();
            res = 1;
            break;
        case sta_eps_socss:
            thk_eps_socss();
            res = 1;
            break;
        case sta_eps_state_flag:
            thk_eps_state_flag();
            res = 1;
            break;
        case sta_eps_charging:
            thk_eps_charging();
            res = 1;
            break;
    #endif

    #if (SCH_TRX_ONBOARD==1)
        //TRX => (Communication subsystem)
        case sta_trx_opmode:
            thk_trx_opmode();
            res = 1;
            break;           // Operation mode
        case sta_trx_temp_hpa:
            thk_trx_temp_hpa();
            res = 1;
            break;         // Temp of HPA
        case sta_trx_temp_mcu:
            thk_trx_temp_mcu();
            res = 1;
            break;         // Temp of MCU
        case sta_trx_rssi:
            thk_trx_rssi();
            res = 1;
            break;             // RSSI Signal level
        case sta_trx_status_tc:
            thk_trx_status_tc();
            res = 1;
            break;        // Status Register of TC
    #endif

        //On default (para aquellos que no tienen un periodicUpdate asociado)
        default:
            //do nothing on default
            res = 0;
            break;
    }

    return res;
}
//------------------------------------------------------------------------------
/*
void thk_ppc_hoursAlive(void){
    int arg = sta_getCubesatVar(sta_ppc_hoursAlive)+1;
    sta_setCubesatVar(sta_ppc_hoursAlive, arg);
}
void thk_ppc_hoursWithoutReset(void)
{
    // En el futro esta funcion deberia ser llamada desde alguna interrupcion periodica del RTCC.
    // O leer del RTCC y comparar con su valor actual (get_ppc_hoursWithoutReset) y decidir
    // si debe incrementarlo y por cuanto 


    //int ar=*( (int *)param ); char ret[6];
    //con_printf("param= ");  Hex16ToAscii( ar, ret); con_printf(ret); con_printf("\n");
    int arg = sta_getCubesatVar(sta_ppc_hoursWithoutReset)+1;
    sta_setCubesatVar(sta_ppc_hoursWithoutReset, arg);
    
}
*/
/*------------------------------------------------------------------------------
 *                                  DRP PPC ENWDT
 *------------------------------------------------------------------------------
 * Description        : Update the state of the WDT
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_enwdt(void)
{
    /* Leer el registro de los CONFIGWORD no es directo
     * (esta "beyond user visibility pages"). Asi es que la 
     * implementacion de este comando es asumir que se parte en WDT_ENABLE
     *  y luego registrar las desactivaciones/activaciones del WDT a traves
     * de ppc_enwdt (ojo: no es lo mismo que thk_ppc_enwdt )*/   
}
/*------------------------------------------------------------------------------
 *                                  DRP PPC OSC
 *------------------------------------------------------------------------------
 * Description        : Update current oscillator
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_osc(void)
{
    /* FRC_OSC                           0x0000 //Internal oscillator
    *  FRC_OSC_WITH_POSTSCALER_PLL       0x0001 //Internal oscillator with Postscalar PLL enabled
    *  PRIMARY_OSC                       0x0002 //Primary oscillator
    *  PRIMARY_OSC_WITH_PLL              0x0003 //Primary oscillator with PLL
    *  SECONDRY_OSC                      0x0004 //Secondary oscillator
    *  LOW_POWER_RC                      0x0005 //Low Power RC oscillator
    *  FRC_OSC_WITH_POSTSCALER           0x0007 //Internal oscillator with postscalar
     */
    sta_setCubesatVar(sta_ppc_osc, (int)Current_OSCILLATOR() );   
}
/*------------------------------------------------------------------------------
 *                                  PPC LAST RESET SOURCE
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_MB_nOE_USB_nINT_stat(void)
{
    if(PPC_MB_nOE_USB_nINT_CHECK)
    {
        sta_setCubesatVar(sta_ppc_MB_nOE_USB_nINT_stat, 1);
    }
    else
    {
        sta_setCubesatVar(sta_ppc_MB_nOE_USB_nINT_stat, 0);
    }   
}
/*------------------------------------------------------------------------------
 *                                  thk_ppc_MB_nOE_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_MB_nOE_MHX_stat(void)
{
    if(PPC_MB_nOE_MHX_CHECK)
    {
        sta_setCubesatVar(sta_ppc_MB_nOE_MHX_stat, 1);
    }
    else
    {
        sta_setCubesatVar(sta_ppc_MB_nOE_MHX_stat, 0);
    }
    
}
/*------------------------------------------------------------------------------
 *                                  thk_ppc_MB_nON_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_MB_nON_MHX_stat(void)
{
    if(PPC_MB_nON_MHX_CHECK)
    {
        sta_setCubesatVar(sta_ppc_MB_nON_MHX_stat, 1);
    }
    else
    {
        sta_setCubesatVar(sta_ppc_MB_nON_MHX_stat, 0);
    }
    
}
/*------------------------------------------------------------------------------
 *                                  thk_ppc_MB_nON_SD_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_ppc_MB_nON_SD_stat(void)
{
    if(PPC_MB_nON_SD_CHECK)
    {
        sta_setCubesatVar(sta_ppc_MB_nON_SD_stat, 1);
    }
    else
    {
        sta_setCubesatVar(sta_ppc_MB_nON_SD_stat, 0);
    }
    
}
/*------------------------------------------------------------------------------
 *                                  thk_rtc_year
 *------------------------------------------------------------------------------
 * Description        : Updates the RTC structure inside Cubestat
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_rtc_year(void)
{
    sta_setCubesatVar(sta_rtc_year, RTC_get_year() );   
}
void thk_rtc_month(void)
{
    sta_setCubesatVar(sta_rtc_month, RTC_get_month() );   
}
void thk_rtc_week_day(void)
{
    sta_setCubesatVar(sta_rtc_week_day, RTC_get_week_day() );   
}
void thk_rtc_day_number(void)
{
    sta_setCubesatVar(sta_rtc_day_number, RTC_get_day_num() );   
}
void thk_rtc_hours(void)
{
    sta_setCubesatVar(sta_rtc_hours, RTC_get_hours() );   
}
void thk_rtc_minutes(void)
{
    sta_setCubesatVar(sta_rtc_minutes, RTC_get_minutes() );   
}
void thk_rtc_seconds(void)
{
    sta_setCubesatVar(sta_rtc_seconds, RTC_get_seconds() );    
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_bat0_voltage
 *------------------------------------------------------------------------------
 * Description        : Updates the battery voltage based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bat0_voltage(void)
{
    sta_setCubesatVar(sta_eps_bat0_voltage, readEPSvars(EPS_ID_bat0_voltage));
    
}

/*------------------------------------------------------------------------------
 *                                  thk_eps_bat0_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bat0_current(void)
{
    sta_setCubesatVar(sta_eps_bat0_current, readEPSvars(EPS_ID_bat0_current));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_bus5V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 5V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bus5V_current(void)
{
    sta_setCubesatVar(sta_eps_bus5V_current, readEPSvars(EPS_ID_bus5V_current));   
}
/*------------------------------------------------------------------------------
 *                                   thk_eps_bus3V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 3V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bus3V_current(void)
{
    sta_setCubesatVar(sta_eps_bus3V_current, readEPSvars(EPS_ID_bus3V_current));   
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_bus_battery_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bus_battery_current(void)
{
    sta_setCubesatVar(sta_eps_bus_battery_current, readEPSvars(EPS_ID_bus_battery_current));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_bat0_temp
 *------------------------------------------------------------------------------
 * Description        : Updates the battery temp based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_bat0_temp(void)
{
    sta_setCubesatVar(sta_eps_bat0_temp, readEPSvars(EPS_ID_bat0_temp));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_panel_pwr
 *------------------------------------------------------------------------------
 * Description        : Updates the power sourced by the panels based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_panel_pwr(void)
{
    sta_setCubesatVar(sta_eps_panel_pwr, readEPSvars(EPS_ID_panel_pwr));   
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_status
 *------------------------------------------------------------------------------
 * Description        : Updates the EPS status based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_status(void)
{
    sta_setCubesatVar(sta_eps_status, readEPSvars(EPS_ID_status));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_soc
 *------------------------------------------------------------------------------
 * Description        : Updates the battery soc estimation based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_soc(void)
{
    int soc = readEPSvars(EPS_ID_soc);
    sta_setCubesatVar(sta_eps_soc, soc);
    
    #if (SCH_CMDDRP_VERBOSE >=1)
        char buf[10];
        itoa(buf, soc, 10);
        con_printf("SOC= "); con_printf(buf); con_printf("\r\n");
    #endif
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_socss
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "steady state" soc estimation based
 *                      on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_socss(void)
{
    sta_setCubesatVar(sta_eps_socss, readEPSvars(EPS_ID_socss));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_state_flag
 *------------------------------------------------------------------------------
 * Description        : Updates the state flag that have the following meaning
 *                      state_flag = 1 : max DOD was reached so high current
 *                                       commands won't be excecuted.
 *                      state_flag = 2 : After a state_flag=1, a safe DOD was
 *                                       reached so high current commands are
 *                                       allowed
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_state_flag(void)
{
    sta_setCubesatVar(sta_eps_state_flag, readEPSvars(EPS_ID_state_flag));
}
/*------------------------------------------------------------------------------
 *                                  thk_eps_charging
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "charging state"
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void thk_eps_charging(void)
{
    int i_value = readEPSvars(EPS_ID_current_dir);
    i_value = i_value < 256 ? 1:0;
    sta_setCubesatVar(sta_eps_charging, i_value);
}
/*------------------------------------------------------------------------------
 *		 	DRP TRX OPMODE
 *------------------------------------------------------------------------------
 * Description        : Update TRX Operation Mode status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5013
 *----------------------------------------------------------------------------*/
void thk_trx_opmode(void)
{
    /* TODO: Reading OP_MODE */
    char char_value = 0;
//    char_value = TRX_ReadRegister(TRX_MODE);
    /* Writing TRX_FTX to repo */
    sta_setCubesatVar(sta_trx_opmode, (int)char_value);

    
}
/*------------------------------------------------------------------------------
 *		 	DRP TRX TEMP
 *------------------------------------------------------------------------------
 * Description        : Update TRX TEMP_MCU and TEMP_HPA status in data
 *                      repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5014
 *----------------------------------------------------------------------------*/
void thk_trx_temp_hpa(void)
{
    int int_value = -1;

    /*TODO: Reading TEMP_HPA */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPHPA_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPHPA_L);
    /* Writing TEMP_HPA to repo */
    sta_setCubesatVar(sta_trx_temp_hpa, int_value);

}
/*------------------------------------------------------------------------------
 *		 	DRP TRX TEMP_MCU
 *------------------------------------------------------------------------------
 * Description        : Update TRX TEMP_MCU and TEMP_HPA status in data
 *                      repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5014
 *----------------------------------------------------------------------------*/
void thk_trx_temp_mcu(void)
{
    int int_value = -1;

    /* TODO: Reading TEMP_MCU */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPMCU_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPMCU_L);
    /* Writing TEMP_MCU to repo */
    sta_setCubesatVar(sta_trx_temp_mcu, int_value);

    
}
/*------------------------------------------------------------------------------
 *		 	DRP TRX RSSI
 *------------------------------------------------------------------------------
 * Description        : Update TRX RSSI status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5015
 *----------------------------------------------------------------------------*/
void thk_trx_rssi(void)
{
    srp_update_STA_CubesatVar_trx_rssi(NULL);
}
/*------------------------------------------------------------------------------
 *		 	DRP TRX STATUS TC
 *------------------------------------------------------------------------------
 * Description        : Update TRX FRX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5018
 *----------------------------------------------------------------------------*/
void thk_trx_status_tc(void)
{
    int value = 0;
    /* TODO: Reading STATUS_TC */
//    value = TRX_CheckNewTC();
    /* Writing BEACON_PWR to repo */
    sta_setCubesatVar(sta_trx_status_tc, value);
}

//------------------------------------------------------------------------------

/*------------------------------------------------------------------------------
 *                              DRP_DEP_WRITE_DEPLOYED
 *------------------------------------------------------------------------------
 * Description        : Updates the dep_ant_deployed flag in persistent memory
 * Arguments          : flag value
 * Return Value       : 1 - Success, 0 - Fail
 * ID                 : 0x5026
 *----------------------------------------------------------------------------*/
void thk_dep_write_deployed(int dep_state, int dep_tries)
{
    sta_setCubesatVar(sta_dep_ant_deployed, dep_state);
    sta_setCubesatVar(sta_dep_ant_tries, dep_tries);

    if(dep_state)
    {
        sta_setCubesatVar(sta_dep_year, RTC_get_year() );
        sta_setCubesatVar(sta_dep_month, RTC_get_month() );
        sta_setCubesatVar(sta_dep_day_number, RTC_get_day_num() );
        sta_setCubesatVar(sta_dep_week_day, RTC_get_week_day() );
        sta_setCubesatVar(sta_dep_hours, RTC_get_hours() );
        sta_setCubesatVar(sta_dep_minutes, RTC_get_minutes() );
        sta_setCubesatVar(sta_dep_seconds, RTC_get_seconds() );
    }
}

