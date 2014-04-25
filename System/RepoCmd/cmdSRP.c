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

#include "cmdSRP.h"

cmdFunction srpFunction[SRP_NCMD];
int srp_sysReq[SRP_NCMD];

nanocom_rssi_t rssi_data[10];

void srp_onResetCmdSRP(){
    printf("        srp_onResetCmdSRP\n");

    //De display
    srpFunction[(unsigned char)srp_id_print_STA_CubesatVar] = srp_print_STA_CubesatVar;
    srp_sysReq[(unsigned char)srp_id_print_STA_CubesatVar]  = CMD_SYSREQ_MIN;
    //De update en general en sta_CubesatVar
    srpFunction[(unsigned char)srp_id_increment_STA_CubesatVar_hoursAlive] = srp_increment_STA_CubesatVar_hoursAlive;
    srp_sysReq[(unsigned char)srp_id_increment_STA_CubesatVar_hoursAlive]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_increment_STA_CubesatVar_hoursWithoutReset] = srp_increment_STA_CubesatVar_hoursWithoutReset;
    srp_sysReq[(unsigned char)srp_id_increment_STA_CubesatVar_hoursWithoutReset]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_increment_STA_CubesatVar_nSended_tm] = srp_increment_STA_CubesatVar_nSended_tm;
    srp_sysReq[(unsigned char)srp_id_increment_STA_CubesatVar_nSended_tm]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_increment_STA_CubesatVar_nReceived_tc] = srp_increment_STA_CubesatVar_nReceived_tc;
    srp_sysReq[(unsigned char)srp_id_increment_STA_CubesatVar_nReceived_tc]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_STA_CubesatVar_opMode] = srp_update_sta_CubesatVar_opMode;
    srp_sysReq[(unsigned char)srp_id_update_STA_CubesatVar_opMode]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_STA_CubesatVar_trx_rssi] = srp_update_STA_CubesatVar_trx_rssi;
    srp_sysReq[(unsigned char)srp_id_update_STA_CubesatVar_trx_rssi]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_STA_CubesatVar_trx_rssi_mean] = srp_update_STA_CubesatVar_trx_rssi_mean;
    srp_sysReq[(unsigned char)srp_id_update_STA_CubesatVar_trx_rssi_mean]  = CMD_SYSREQ_MIN;

    srpFunction[(unsigned char)srp_id_update_STA_CubesatVar_trx_newTcFrame] = srp_trx_update_STA_CubesatVar_newTcFrame;
    srp_sysReq[(unsigned char)srp_id_update_STA_CubesatVar_trx_newTcFrame]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_STA_CubesatVar_trx_newCmdBuff] = srp_trx_STA_CubesatVar_newCmdBuff;
    srp_sysReq[(unsigned char)srp_id_update_STA_CubesatVar_trx_newCmdBuff]  = CMD_SYSREQ_MIN;

    srpFunction[(unsigned char)srp_id_executeBeforeFlight] = srp_executeBeforeFlight;
    srp_sysReq[(unsigned char)srp_id_executeBeforeFlight]  = CMD_SYSREQ_MIN;

    srpFunction[(unsigned char)srp_id_deployment_registration] = srp_deployment_registration;
    srp_sysReq[(unsigned char)srp_id_deployment_registration]  = CMD_SYSREQ_MIN;
}

//------------------------------------------------------------------------------
int srp_deployment_registration(void *param)
{
    int dep_tries = *( (int *)param );
    int dep_state;

    if(dep_tries < 0){  //Antennas are NOT deployed
        dep_state = 0;
        dep_tries = - dep_tries;
    }
    else{               //Antennas are deployed
        dep_state = 1;
    }

    sta_setCubesatVar(sta_dep_ant_deployed, dep_state);
    sta_setCubesatVar(sta_dep_ant_tries, dep_tries);

    //RTC
    sta_setCubesatVar(sta_dep_year, RTC_get_year() );
    sta_setCubesatVar(sta_dep_month, RTC_get_month() );
    sta_setCubesatVar(sta_dep_day_number, RTC_get_day_num() );
    sta_setCubesatVar(sta_dep_week_day, RTC_get_week_day() );
    sta_setCubesatVar(sta_dep_hours, RTC_get_hours() );
    sta_setCubesatVar(sta_dep_minutes, RTC_get_minutes() );
    sta_setCubesatVar(sta_dep_seconds, RTC_get_seconds() );

    return 1;
}
int srp_increment_STA_CubesatVar_hoursWithoutReset(void *param)
{
    /* En el futro esta funcion deberia ser llamada desde alguna interrupcion periodica del RTCC.
     * O leer del RTCC y comparar con su valor actual (get_ppc_hoursWithoutReset) y decidir
     * si debe incrementarlo y por cuanto */

    //int ar=*( (int *)param ); char ret[6];
    //con_printf("param= ");  Hex16ToAscii( ar, ret); con_printf(buffer); con_printf("\n");
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_ppc_hoursWithoutReset)+1;
    sta_setCubesatVar(sta_ppc_hoursWithoutReset, arg);
    return 1;
}
int srp_increment_STA_CubesatVar_hoursAlive(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_ppc_hoursAlive)+1;
    sta_setCubesatVar(sta_ppc_hoursAlive, arg);
    return 1;
}
int srp_increment_STA_CubesatVar_nSended_tm(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_trx_count_tm)+1;
    sta_setCubesatVar(sta_trx_count_tm, arg);
    return 1;
}
int srp_increment_STA_CubesatVar_nReceived_tc(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_trx_count_tc)+1;
    sta_setCubesatVar(sta_trx_count_tc, arg);
    return 1;
}
int srp_update_sta_CubesatVar_opMode(void *param){

    int arg = *((int *)param);
    sta_setCubesatVar(sta_ppc_opMode, arg);

    return 1;
}
int srp_update_STA_CubesatVar_trx_rssi_mean(void *param)
{
    int new_value=*((int *)param);
    int res = srp_trx_rssi_mean(new_value);
    
    sta_setCubesatVar(sta_trx_rssi_mean, res);

    return 1;
}
/*------------------------------------------------------------------------------
 *                               UPDATE RSSI
 *------------------------------------------------------------------------------
 * Description        : Updates the rssi level, when no TC availible mode
 * Arguments          : none
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
int srp_update_STA_CubesatVar_trx_rssi(void *param)
{
    //Actualizar periodicamente el rssi, para modo sin telecomandos
    srp_trx_rssi();

    return 1;
}
/*------------------------------------------------------------------------------
 *                                  DRP TRX NEWTCFRAME
 *------------------------------------------------------------------------------
 * Description        : Updates the new_TcFrame flag
 * Arguments          : flag value
 * Return Value       : int: 1=Always successfull
 * ID                 : 0x5019
 *----------------------------------------------------------------------------*/
int srp_trx_update_STA_CubesatVar_newTcFrame(void *param)
{
#if (SCH_CMDSRP_VERBOSE >= 1)
    char buffer[6];
    con_printf("trx_newTcFrame= ");
    itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newTcFrame), 10);
    con_printf(buffer);
    con_printf("\r\n");
#endif

    int flag=*( (int *)param );
    sta_setCubesatVar(sta_trx_newTcFrame, flag);     /* indico que llego un telecomando */
    return 1;
}

/*------------------------------------------------------------------------------
 *                                  DRP TRX NEWCMDBUFF
 *------------------------------------------------------------------------------
 * Description        : Updates the new_CmdBuff flag
 * Arguments          : flag value
 * Return Value       : int: 1=Always successfull
 * ID                 : 0x501A
 *----------------------------------------------------------------------------*/
int srp_trx_STA_CubesatVar_newCmdBuff(void *param)
{
#if (SCH_CMDSRP_VERBOSE >= 1)
    char buffer[6];
    con_printf("trx_newCmdBuff= ");
    itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newCmdBuff), 10);
    con_printf(buffer);
    con_printf("\r\n");
#endif

    int flag = *(int *)param;
    sta_setCubesatVar(sta_trx_newCmdBuff, flag);     /* indico que llegaron Comandos al  buff interno */
    return 1;
}
int srp_executeBeforeFlight(void * param){
    printf("  srp_executeBeforeFlight()..\n");

    sta_onResetStatRepo();

    // Bus Hw status (connected trough the PC/104 to the OBC -PIC24-)
    sta_setCubesatVar(sta_RTC_isAlive, 0);
    sta_setCubesatVar(sta_TRX_isAlive, 0);
    sta_setCubesatVar(sta_EPS_isAlive, 0);
    sta_setCubesatVar(sta_MemEEPROM_isAlive, 0);
    sta_setCubesatVar(sta_MemSD_isAlive, 0);
    sta_setCubesatVar(sta_SUCHAI_isDeployed, 0);

    // Payload Hw status (connected trough the PC/104 to the OBC -PIC24-)
    sta_setCubesatVar(sta_pay_lagmuirProbe_isAlive, 0);
    sta_setCubesatVar(sta_pay_sensTemp_isAlive, 0);
    sta_setCubesatVar(sta_pay_gps_isAlive, 0);
    sta_setCubesatVar(sta_pay_expFis_isAlive, 0);
    sta_setCubesatVar(sta_pay_camera_isAlive, 0);
    sta_setCubesatVar(sta_pay_gyro_isAlive, 0);
    sta_setCubesatVar(sta_pay_tmEstado_isAlive, 0);
    sta_setCubesatVar(sta_pay_test1_isAlive, 0);
    sta_setCubesatVar(sta_pay_test2_isAlive, 0);
    sta_setCubesatVar(sta_pay_lagmuirProbe_isDeployed, 0);

    //PPC => (C&DH subsystem)
    sta_setCubesatVar(sta_ppc_opMode, STA_PPC_OPMODE_NORMAL);
    sta_setCubesatVar(sta_ppc_lastResetSource, -1);
    sta_setCubesatVar(sta_ppc_hoursAlive, 0);
    sta_setCubesatVar(sta_ppc_hoursWithoutReset, 0);
    sta_setCubesatVar(sta_ppc_resetCounter, 0);
    sta_setCubesatVar(sta_ppc_enwdt, PPC_INITIAL_WDT_STATE);	// 1=WDT Active, 0=WDT Inactive

    //DEP => (C&DH subsystem)
    sta_setCubesatVar(sta_dep_ant_deployed, 0);            // 1=already deployed, 0=not deployed yet
    sta_setCubesatVar(sta_dep_ant_tries, 0);               // Number of tries to deploy antenna
    sta_setCubesatVar(sta_dep_year, 0);
    sta_setCubesatVar(sta_dep_month, 0);
    sta_setCubesatVar(sta_dep_week_day, 0);
    sta_setCubesatVar(sta_dep_day_number, 0);
    sta_setCubesatVar(sta_dep_hours, 0);
    sta_setCubesatVar(sta_dep_minutes, 0);
    sta_setCubesatVar(sta_dep_seconds, 0);

    //PAYLOAD
    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        sta_setCubesatVar(sta_pay_lagmuirProbe_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_lagmuirProbe_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        sta_setCubesatVar(sta_pay_sensTemp_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_sensTemp_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_GPS_ONBOARD==1)
        sta_setCubesatVar(sta_pay_gps_perform, SRP_PAY_XXX_PERFORM_ACTIVE);
    #else
        sta_setCubesatVar(sta_pay_gps_perform, SRP_PAY_XXX_PERFORM_INACTIVE);
    #endif
    #if (SCH_PAY_FIS_ONBOARD==1)
        sta_setCubesatVar(sta_pay_expFis_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_expFis_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        sta_setCubesatVar(sta_pay_camera_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_camera_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_GYRO_ONBOARD==1)
        sta_setCubesatVar(sta_pay_gyro_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_gyro_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_TMESTADO_ONBOARD==1)
        sta_setCubesatVar(sta_pay_tmEstado_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_tmEstado_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_TEST1_ONBOARD==1)
        sta_setCubesatVar(sta_pay_test1_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_test1_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif
    #if (SCH_PAY_TEST2_ONBOARD==1)
        sta_setCubesatVar(sta_pay_test2_perform, SRP_PAY_XXX_PERFORM_ACTIVE );
    #else
        sta_setCubesatVar(sta_pay_test2_perform, SRP_PAY_XXX_PERFORM_INACTIVE );
    #endif

    sta_setCubesatVar(sta_SUCHAI_isDeployed, 0);  //First time on!
    /* Es la unica variable que gatilla las acciones de despliegue en THK */
    return 1;
}
//------------------------------------------------------------------------------
// functions to "read" Cubestat
/*------------------------------------------------------------------------------
 *                                  DRP READALL DAT CUBESAT
 *------------------------------------------------------------------------------
 * Description        : Read/Prints all Cubestat structure
 * Arguments          : None
 * Return Value       : int
 * ID                 : 0x501D
 *----------------------------------------------------------------------------*/
int srp_print_STA_CubesatVar(void *param)
{
    char buffer[10];
    int arg=*((int *)param);

    con_printf("===================================\r\n");
    con_printf("Contenido de CubestatVar: \r\n");
    con_printf("===================================\r\n");

    if(arg==1)
    {
        STA_CubesatVar indxVar;
        for(indxVar=0; indxVar<sta_cubesatVar_last_one; indxVar++)
        {
            con_printf("sta_CubesatVar[");
            //itoa(buffer, (unsigned int)indxVar, 10);
            sprintf( buffer, "%d", (unsigned int)indxVar );
            con_printf(buffer); con_printf("]=");
            //itoa(buffer,(unsigned int)sta_getCubesatVar(indxVar), 10);
            sprintf( buffer, "0x%X", (unsigned int)sta_getCubesatVar(indxVar) );
            con_printf(buffer); con_printf("\r\n");
        }
    }
    else
    {
        /* Get mPPC */
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_opMode), 10);
        con_printf("ppc_opMode= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_lastResetSource), 10);
        con_printf("ppc_lastResetSource= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_hoursAlive), 10);
        con_printf("ppc_hoursAlive= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_hoursWithoutReset), 10);
        con_printf("ppc_hoursWithoutReset= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_resetCounter), 10);
        con_printf("ppc_resetCounter= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_enwdt), 10);
        con_printf("ppc_enwdt= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_osc), 10);
        con_printf("ppc_osc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_MB_nOE_USB_nINT_stat), 10);
        con_printf("ppc_MB_nOE_USB_nINT_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_MB_nOE_MHX_stat), 10);
        con_printf("ppc_MB_nOE_MHX_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_MB_nON_MHX_stat), 10);
        con_printf("ppc_MB_nON_MHX_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_ppc_MB_nON_SD_stat), 10);
        con_printf("ppc_MB_nON_SD_stat= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mDEP */
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_ant_deployed), 10);
        con_printf("dep_ant_deployed= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_ant_tries), 10);
        con_printf("dep_ant_tries= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_year), 10);
        con_printf("dep_year= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_month), 10);
        con_printf("dep_month= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_week_day), 10);
        con_printf("dep_week_day= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_day_number), 10);
        con_printf("dep_day_number= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_hours), 10);
        con_printf("dep_hours= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_minutes), 10);
        con_printf("dep_minutes= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_dep_seconds), 10);
        con_printf("dep_seconds= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mRTC */
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_year), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_year), 10);
        con_printf("rtc_year= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_month), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_month), 10);
        con_printf("rtc_month= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_week_day), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_week_day), 10);
        con_printf("rtc_week_day= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_day_number), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_day_number), 10);
        con_printf("rtc_day_number= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_hours), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_hours), 10);
        con_printf("rtc_hours= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_minutes), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_minutes), 10);
        con_printf("rtc_minutes= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_rtc_seconds), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_rtc_seconds), 10);
        con_printf("rtc_seconds= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mEPS */

        itoa(buffer,   (unsigned int)sta_getCubesatVar(sta_eps_bat0_voltage), 10);
        con_printf("eps_bat0_voltage= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_bat0_current), 10);
        con_printf("eps_bat0_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_bus5V_current), 10);
        con_printf("eps_bus5V_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_bus3V_current), 10);
        con_printf("eps_bus3V_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_bus_battery_current), 10);
        con_printf("eps_bus_battery_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_bat0_temp), 10);
        con_printf("eps_bat0_temp= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_panel_pwr), 10);
        con_printf("eps_panel_pwr= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_status), 10);
        con_printf("eps_status= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_soc), 10);
        con_printf("eps_soc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_socss), 10);
        con_printf("eps_socss= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_state_flag), 10);
        con_printf("eps_state_flag= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_eps_charging), 10);
        con_printf("eps_charging= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mTRX */
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_opmode), 10);
        con_printf("trx_opmode= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_temp_hpa), 10);
        con_printf("trx_temp_hpa= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_temp_mcu), 10);
        con_printf("trx_temp_mcu= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_trx_rssi), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_rssi), 10);
        con_printf("trx_rssi= "); con_printf(buffer); con_printf(" dBm\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_status_tc), 10);
        con_printf("trx_status_tc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_count_tm), 10);
        con_printf("trx_count_tm= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_count_tc), 10);
        con_printf("trx_count_tc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_lastcmd_day), 10);
        con_printf("trx_lastcmd_day= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        // Cmd buffer control
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newTcFrame), 10);
        con_printf("trx_newTcFrame= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newCmdBuff), 10);
        con_printf("trx_newCmdBuff= "); con_printf(buffer); con_printf("\r\n");        
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_fpl_index), 10);
        con_printf("fpl_index= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_MemSD_isAlive), 10);
        con_printf("msd_status= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");
    }

    return 1;
}

int srp_debug(void *param){
    int ind=*((int*)param);

    char ret[10];
    itoa(ret, (unsigned int)ind, 10); con_printf("calling srp_debug"); con_printf(ret); con_printf("()...\r\n");

    switch(ind){
        case 4:
            srp_debug4();
            break;
        default:
            //
            break;
    }

    return 1;
}

//------------------------------------------------------------------------------
//Aux functions
//------------------------------------------------------------------------------
void srp_eraseAll_CubesatVar(void){
    #if (SCH_CMDSRP_VERBOSE>=1)
        con_printf("    Erasing memEEPROM\n");
    #endif

    unsigned int indxVar;
    int data = -1;  //oxFFFF

    for(indxVar=0; indxVar<sta_cubesatVar_last_one; indxVar++){
        sta_setCubesatVar(indxVar, data);
    }
}
/*------------------------------------------------------------------------------
 *		 	DRP TRX RSSI
 *------------------------------------------------------------------------------
 * Description        : Update TRX RSSI status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5015
 *----------------------------------------------------------------------------*/
void srp_trx_rssi(void)
{
    uint8_t count = 0;
    int result = 0;
    int rssi_value = 0;

    //result = com_get_log_rssi(rssi_data, &count, NODE_COM, 1000);
    if(result)
    {
        rssi_value = rssi_data[count].rssi; //Get last RSSI measure
    }

    /* Writing RSSI to repo */
    sta_setCubesatVar(sta_trx_rssi, rssi_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX RSSI_MEAN
 *------------------------------------------------------------------------------
 * Description        : Update TRX RSSI status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5015
 *----------------------------------------------------------------------------*/

#define SRP_RSSI_LIST_LEN   20  /*!< Largo del buffer de datos para calcular rssi promedio */
#define SRP_RSSI_MEAN_INIT  99 /*|< Aca hay que tener cuidado de que (RSSI_MEAN_INIT)*(RSSI_LIST_LEN)
                            no porvoque overflow, por eso extendi el tipo de RSSI_LIST[] a long */

#define SRP_RSSI_CNT_MAX   600  /*!< Segundos que RSSI_CNT puede pasar sin que se actualice RSSI_MEAN */

static long SRP_RSSI_LIST[SRP_RSSI_LIST_LEN];
static int srp_rssi_list_p = 0;
//static int SRP_RSSI_MEAN = SRP_RSSI_MEAN_INIT;

/**
 * Calcular el promedio del nivel de RSSI sobre la lista de datos disponiblea.
 * Retorna el valor actualizado al desechar mediciones antiguas
 *
 * @param new_value Nuevo valor para agregar a la lista y actualizar el promedio
 * @return Promedio del nivel de RSSI sobre los datos disponibles
 */
static int srp_update_rssi_mean(int new_value){
    srp_rssi_list_p = (srp_rssi_list_p < SRP_RSSI_LIST_LEN) ? srp_rssi_list_p : 0;
    SRP_RSSI_LIST[srp_rssi_list_p] = new_value;
    srp_rssi_list_p++; int res;

    #if (SCH_CMDSRP_VERBOSE >=2)
        itoa(buf, new_value, 10);
        con_printf("new_value= "); con_printf(buf); con_printf("\r\n");
        itoa(buf, rssi_list_p, 10);
        con_printf("rssi_list_p= "); con_printf(buf); con_printf("\r\n");
    #endif

    int i; long sum = 0;
    for(i=0; i<SRP_RSSI_LIST_LEN; i++)
    {
        sum+=SRP_RSSI_LIST[i];
    }

    res=sum/SRP_RSSI_LIST_LEN;

    #if (SCH_CMDSRP_VERBOSE >=2)
        itoa(buf, res,10);
        con_printf("res= "); con_printf(buf); con_printf("\r\n");
    #endif

    return res;
}
int srp_trx_rssi_mean(int arg){
    static int init;

    if(init==0){
        /* Inicializar el buffer de valores de RSSI */
        int i, rssi_list_init=SRP_RSSI_MEAN_INIT;
        for(i=0; i<SRP_RSSI_LIST_LEN; i++)
        {
            SRP_RSSI_LIST[i] = rssi_list_init;
        }
        init++;
    }

    int res=srp_update_rssi_mean(arg);
    return res;
}

void srp_debug4(void){
    unsigned int address=0;
    unsigned int data=0xA000;
    char buffer[10];

    con_printf("(Destructive) Testing memEEPROM\r\n");
    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++, data++){
        //con_printf("testing address j="); Hex16ToAscii( address); con_printf(buffer); con_printf("\n");

        con_printf("writing: ");
        writeIntEEPROM1(address, data);
        itoa(buffer, address,10); con_printf("value["); con_printf(buffer); con_printf("]=");
        itoa(buffer, data,10); con_printf(buffer); con_printf("    |    ");

        con_printf("reading: ");
        unsigned int res = readIntEEPROM1(address);
        itoa(buffer, address,10); con_printf("value["); con_printf(buffer); con_printf("]=");
        itoa(buffer, res,10); con_printf(buffer); con_printf("    |    ");

        con_printf("comparing: ");
        if(data==res){ con_printf("ok"); }
        else{ con_printf("fail"); }
        con_printf("\n");
    }
    srp_eraseAll_CubesatVar();

}

