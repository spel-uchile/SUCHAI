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

#include "cmdDRP.h"

cmdFunction drpFunction[DRP_NCMD];
int drp_sysReq[DRP_NCMD];

void drp_onResetCmdDRP(){
    //De display
    drpFunction[(unsigned char)drp_id_print_dat_CubesatVar] = drp_print_dat_CubesatVar;
    drp_sysReq[(unsigned char)drp_id_print_dat_CubesatVar]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_TelecmdBuff] = drp_print_dat_TelecmdBuff;
    drp_sysReq[(unsigned char)drp_id_print_dat_TelecmdBuff]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_FlightPlan] = drp_print_dat_FlightPlan;
    drp_sysReq[(unsigned char)drp_id_print_dat_FlightPlan]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadIndxs] = drp_print_dat_PayloadIndxs;
    drp_sysReq[(unsigned char)drp_id_print_dat_PayloadIndxs]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadVar] = drp_print_dat_PayloadVar;
    drp_sysReq[(unsigned char)drp_id_print_dat_PayloadVar]  = CMD_SYSREQ_MIN;
    //De update en general en dat_CubesatVar
    drpFunction[(unsigned char)drp_id_updateAll_dat_CubesatVar] = drp_updateAll_dat_CubesatVar;
    drp_sysReq[(unsigned char)drp_id_updateAll_dat_CubesatVar]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_hoursAlive] = drp_update_dat_CubesatVar_hoursAlive;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_hoursAlive]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_hoursWithoutReset] = drp_update_dat_CubesatVar_hoursWithoutReset;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_hoursWithoutReset]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_nSended_tm] = drp_update_dat_CubesatVar_nSended_tm;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_nSended_tm]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_nReceived_tc] = drp_update_dat_CubesatVar_nReceived_tc;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_nReceived_tc]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_opMode] = drp_update_dat_CubesatVar_opMode;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_opMode]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_trx_rssi] = drp_update_dat_CubesatVar_trx_rssi;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_trx_rssi]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_update_dat_CubesatVar_trx_rssi_mean] = drp_update_dat_CubesatVar_trx_rssi_mean;
    drp_sysReq[(unsigned char)drp_id_update_dat_CubesatVar_trx_rssi_mean]  = CMD_SYSREQ_MIN;
    //De mem ext (EEPROM y SD)
    drpFunction[(unsigned char)drp_id_executeBeforeFlight] = drp_executeBeforeFlight;
    drp_sysReq[(unsigned char)drp_id_executeBeforeFlight]  = CMD_SYSREQ_MIN;
    //de debug
    drpFunction[(unsigned char)drp_id_debug] = drp_debug;
    drp_sysReq[(unsigned char)drp_id_debug]  = CMD_SYSREQ_MIN;
    //Flight Plan
    drpFunction[(unsigned char)drp_id_fpl_set_index] = drp_fpl_set_index;
    drp_sysReq[(unsigned char)drp_id_fpl_set_index]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_fpl_set_cmd] = drp_fpl_set_cmd;
    drp_sysReq[(unsigned char)drp_id_fpl_set_cmd]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_fpl_set_param] = drp_fpl_set_param;
    drp_sysReq[(unsigned char)drp_id_fpl_set_param]  = CMD_SYSREQ_MIN;
}

// comands to update Cubestat. They may take one, or no arguments
int drp_updateAll_dat_CubesatVar(void *param){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("drp_updateAll_dat_CubesatVar()..\r\n");
    #endif

    #if (SCH_EPS_ONBOARD==1)
        updateSOCEPS();
        updateStatusEPS();
    #else
        dat_setCubesatVar(dat_eps_soc, CMD_SYSREQ_MAX);
    #endif

    DAT_CubesatVar indxVar;
    for(indxVar=0;indxVar<dat_cubesatVar_last_one;indxVar++){
        drp_CubesatVar_update(indxVar);
    }

    return 1;
}

int drp_update_dat_CubesatVar_opMode(void *param){

    int arg = *((int *)param);
    dat_setCubesatVar(dat_ppc_opMode, arg);

    return 1;
}

int drp_update_dat_CubesatVar_hoursWithoutReset(void *param)
{
    /* En el futro esta funcion deber� ser llamada desde alguna interrupcion periodica del RTCC.
     * O leer del RTCC y comparar con su valor actual (get_ppc_hoursWithoutReset) y decidir
     * si debe incrementarlo y por cuanto */

    //int ar=*( (int *)param ); char ret[6];
    //con_printf("param= ");  Hex16ToAscii( ar, ret); con_printf(buffer); con_printf("\n");
    //solo debe ser llamada cada 1hora
    int arg = dat_getCubesatVar(dat_ppc_hoursWithoutReset)+1;
    dat_setCubesatVar(dat_ppc_hoursWithoutReset, arg);
    return 1;
}

int drp_update_dat_CubesatVar_hoursAlive(void *param){
    //solo debe ser llamada cada 1hora
    int arg = dat_getCubesatVar(dat_ppc_hoursAlive)+1;
    dat_setCubesatVar(dat_ppc_hoursAlive, arg);
    return 1;
}

int drp_update_dat_CubesatVar_nSended_tm(void *param){
    //solo debe ser llamada cada 1hora
    int arg = dat_getCubesatVar(dat_trx_count_tm)+1;
    dat_setCubesatVar(dat_trx_count_tm, arg);
    return 1;
}

int drp_update_dat_CubesatVar_nReceived_tc(void *param){
    //solo debe ser llamada cada 1hora
    int arg = dat_getCubesatVar(dat_trx_count_tc)+1;
    dat_setCubesatVar(dat_trx_count_tc, arg);
    return 1;
}

int drp_update_dat_CubesatVar_trx_rssi_mean(void *param)
{
    int new_value=*((int *)param);
    int res = drp_trx_rssi_mean(new_value);
    
    dat_setCubesatVar(dat_trx_rssi_mean, res);

    return 1;
}


/*------------------------------------------------------------------------------
 *                               UPDATE RSSI
 *------------------------------------------------------------------------------
 * Description        : Updates the rssi level, when no TC availible mode
 * Arguments          : none
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
int drp_update_dat_CubesatVar_trx_rssi(void *param)
{
    //Actualizar periodicamente el rssi, para modo sin telecomandos
    drp_trx_rssi();

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
int drp_trx_newTcFrame(void *param)
{
#if (SCH_CMDDRP_VERBOSE >= 1)
    char buffer[6];
    con_printf("trx_newTcFrame= ");
    itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newTcFrame), 10);
    con_printf(buffer);
    con_printf("\r\n");
#endif

    int flag=*( (int *)param );
    dat_setCubesatVar(dat_trx_newTcFrame, flag);     /* indico que llego un telecomando */
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
int drp_trx_newCmdBuff(void *param)
{
#if (SCH_CMDDRP_VERBOSE >= 1)
    char buffer[6];
    con_printf("trx_newCmdBuff= ");
    itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newCmdBuff), 10);
    con_printf(buffer);
    con_printf("\r\n");
#endif

    int flag = *(int *)param;
    dat_setCubesatVar(dat_trx_newCmdBuff, flag);     /* indico que llegaron Comandos al  buff interno */
    return 1;
}

// functions to "read" Cubestat
/*------------------------------------------------------------------------------
 *                                  DRP READALL DAT CUBESAT
 *------------------------------------------------------------------------------
 * Description        : Read/Prints all Cubestat structure
 * Arguments          : None
 * Return Value       : int
 * ID                 : 0x501D
 *----------------------------------------------------------------------------*/
int drp_print_dat_CubesatVar(void *param)
{
    char buffer[10];
    int arg=*((int *)param);

    con_printf("===================================\r\n");
    con_printf("Contenido de CubestatVar: \r\n");
    con_printf("===================================\r\n");

    if(arg==1)
    {
        DAT_CubesatVar indxVar;
        for(indxVar=0; indxVar<dat_cubesatVar_last_one; indxVar++)
        {
            con_printf("dat_CubesatVar[");
            //itoa(buffer, (unsigned int)indxVar, 10);
            sprintf( buffer, "%d", (unsigned int)indxVar );
            con_printf(buffer); con_printf("]=");
            //itoa(buffer,(unsigned int)dat_getCubesatVar(indxVar), 10);
            sprintf( buffer, "0x%X", (unsigned int)dat_getCubesatVar(indxVar) );
            con_printf(buffer); con_printf("\r\n");
        }
    }
    else
    {
        /* Get mPPC */
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_opMode), 10);
        con_printf("ppc_opMode= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_lastResetSource), 10);
        con_printf("ppc_lastResetSource= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_hoursAlive), 10);
        con_printf("ppc_hoursAlive= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_hoursWithoutReset), 10);
        con_printf("ppc_hoursWithoutReset= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_resetCounter), 10);
        con_printf("ppc_resetCounter= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_enwdt), 10);
        con_printf("ppc_enwdt= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_osc), 10);
        con_printf("ppc_osc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_MB_nOE_USB_nINT_stat), 10);
        con_printf("ppc_MB_nOE_USB_nINT_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_MB_nOE_MHX_stat), 10);
        con_printf("ppc_MB_nOE_MHX_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_MB_nON_MHX_stat), 10);
        con_printf("ppc_MB_nON_MHX_stat= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_ppc_MB_nON_SD_stat), 10);
        con_printf("ppc_MB_nON_SD_stat= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mDEP */
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_ant_deployed), 10);
        con_printf("dep_ant_deployed= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_ant_tries), 10);
        con_printf("dep_ant_tries= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_year), 10);
        con_printf("dep_year= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_month), 10);
        con_printf("dep_month= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_week_day), 10);
        con_printf("dep_week_day= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_day_number), 10);
        con_printf("dep_day_number= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_hours), 10);
        con_printf("dep_hours= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_minutes), 10);
        con_printf("dep_minutes= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_dep_seconds), 10);
        con_printf("dep_seconds= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mRTC */
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_year), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_year), 10);
        con_printf("rtc_year= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_month), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_month), 10);
        con_printf("rtc_month= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_week_day), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_week_day), 10);
        con_printf("rtc_week_day= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_day_number), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_day_number), 10);
        con_printf("rtc_day_number= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_hours), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_hours), 10);
        con_printf("rtc_hours= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_minutes), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_minutes), 10);
        con_printf("rtc_minutes= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_rtc_seconds), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_rtc_seconds), 10);
        con_printf("rtc_seconds= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mEPS */

        itoa(buffer,   (unsigned int)dat_getCubesatVar(dat_eps_bat0_voltage), 10);
        con_printf("eps_bat0_voltage= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_bat0_current), 10);
        con_printf("eps_bat0_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_bus5V_current), 10);
        con_printf("eps_bus5V_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_bus3V_current), 10);
        con_printf("eps_bus3V_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_bus_battery_current), 10);
        con_printf("eps_bus_battery_current= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_bat0_temp), 10);
        con_printf("eps_bat0_temp= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_panel_pwr), 10);
        con_printf("eps_panel_pwr= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_status), 10);
        con_printf("eps_status= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_soc), 10);
        con_printf("eps_soc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_socss), 10);
        con_printf("eps_socss= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_state_flag), 10);
        con_printf("eps_state_flag= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_eps_charging), 10);
        con_printf("eps_charging= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        /* Get mTRX */
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_frec_tx), 10);
        con_printf("trx_frec_tx= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_frec_rx), 10);
        con_printf("trx_frec_rx= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_opmode), 10);
        con_printf("trx_opmode= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_temp_hpa), 10);
        con_printf("trx_temp_hpa= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_temp_mcu), 10);
        con_printf("trx_temp_mcu= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)dat_getCubesatVar(dat_trx_rssi), 10); //itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_rssi), 10);
        con_printf("trx_rssi= "); con_printf(buffer); con_printf(" dBm\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_beacon_pwr), 10);
        con_printf("trx_beacon_pwr= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_telemetry_pwr), 10);
        con_printf("trx_telemetry_pwr= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_status_tc), 10);
        con_printf("trx_status_tc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_count_tm), 10);
        con_printf("trx_count_tm= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_count_tc), 10);
        con_printf("trx_count_tc= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_lastcmd_day), 10);
        con_printf("trx_lastcmd_day= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");

        // Cmd buffer control
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newTcFrame), 10);
        con_printf("trx_newTcFrame= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newCmdBuff), 10);
        con_printf("trx_newCmdBuff= "); con_printf(buffer); con_printf("\r\n");        
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_fpl_index), 10);
        con_printf("fpl_index= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_msd_status), 10);
        con_printf("msd_status= "); con_printf(buffer); con_printf("\r\n");
        con_printf("-----------------------------------\r\n");
    }

    return 1;
}

int drp_print_dat_TelecmdBuff(void *param)
{
    char buffer[10];

    con_printf("===================================\r\n");
    con_printf("Reading dat_TelecmdBuff Buffer\r\n");
    con_printf("===================================\r\n");

    // Cmd buffer control
    itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newTcFrame), 10);
    con_printf("drp_trx_newTcFrame= "); con_printf(buffer); con_printf("\r\n");
    itoa(buffer,  (unsigned int)dat_getCubesatVar(dat_trx_newCmdBuff), 10);
    con_printf("drp_trx_newCmdBuff= "); con_printf(buffer); con_printf("\r\n");

    con_printf("Contenido del dat_telecmdBuff:\r\n");
    int i;
    for(i=0; i<DAT_MAX_BUFF_TELECMD; i++)
    {
        itoa(buffer,  (unsigned int)dat_getTelecmdBuff(i), 10);
        con_printf("dat_telecmdBuff["); itoa(buffer, (unsigned int)i, 10);
        con_printf(buffer); con_printf("]= "); con_printf(buffer); con_printf("\r\n");

    }
    return 1;
}

int drp_print_dat_FlightPlan(void *param){
    con_printf("===================================\r\n");
    con_printf("Reading dat_FligthPlan Buffer\r\n");
    con_printf("===================================\r\n");

    int i; char buffer[10];
    for(i=0; i<SCH_FLIGHTPLAN_N_CMD; i++)
    {
        DispCmd dcmd = dat_getFlightPlan(i);
        con_printf("dat_FlightPlan[");
        itoa(buffer, (unsigned int)i, 10); con_printf(buffer); con_printf("]: cmdId=");
        itoa(buffer,  (unsigned int)dcmd.cmdId, 10); con_printf(buffer); con_printf(" param=");
        itoa(buffer,  (unsigned int)dcmd.param, 10); con_printf(buffer); con_printf("\r\n");
    }
    return 1;
}

int drp_print_dat_PayloadIndxs(void *param){
    con_printf("===================================\r\n");
    con_printf("Reading dat_PayloadIndxs Block\r\n");
    con_printf("===================================\r\n");

    char buffer[10];
    DAT_Payload pay_i;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++)
    {
        unsigned int max = dat_getMaxPayIndx( pay_i);
        unsigned int next = dat_getNextPayIndx( pay_i);
        
        con_printf("pay_i=");
        itoa(buffer, (unsigned int)pay_i,10);
        con_printf(buffer); con_printf("\r\n");
        con_printf("    MaxIndx=");
        //itoa(buffer, (unsigned int)max,10);
        sprintf (buffer, "0x%X", (unsigned int)max);
        con_printf(buffer); con_printf("\r\n");
        con_printf("    NextIndx=");
        //itoa(buffer, (unsigned int)next,10);
        sprintf (buffer, "0x%X", (unsigned int)next);
        con_printf(buffer); con_printf("\r\n");
    }
    return 1;
}

int drp_print_dat_PayloadVar(void *param){

    int pay_i2 = *((int *)param);
    
    con_printf("===================================\r\n");
    con_printf("Reading dat_PayloadVar \r\n");
    con_printf("===================================\r\n");

    char buffer[10];
    DAT_Payload pay_i=pay_i2;
    if(pay_i>=dat_pay_last_one){
        con_printf("pay_i=");
        itoa(buffer, (unsigned int)pay_i,10);
        con_printf(buffer);
        con_printf(" NO existe. Nada que imprimir\r\n");
        return 0;
    }

    con_printf("pay_i=");
    itoa(buffer, (unsigned int)pay_i,10);
    con_printf(buffer); con_printf("\r\n");

    unsigned int indx; unsigned int max = dat_getMaxPayIndx(pay_i); int val;
    for(indx=0; indx<=max; indx++)
    {
        dat_getPayloadVar(pay_i, indx, &val);
        
        con_printf("dat_getPayloadVar[");
        //itoa(buffer, (unsigned int)indx,10);
        sprintf (buffer, "%d", (unsigned int)indx);
        con_printf(buffer); con_printf("]=");
        //itoa(buffer, (unsigned int)val,10);
        sprintf (buffer, "0x%X", (unsigned int)val);
        con_printf(buffer); con_printf("\r\n");

        ClrWdt();
    }
    return 1;
}

/*------------------------------------------------------------------------------
 *                              DRP_FLP_SET_INDEX
 *------------------------------------------------------------------------------
 * @brief             : Configura la variable del sistema que indica el indice
 *                      del flight plan que sera modificado. Este comando debe
 *                      utilizarse antes de los comandos drp_fpl_set_command y
 *                      drp_fpl_set_param, ya que estos comandos modifican el
 *                      indice indicado por esta variable.
 * @arg                 index - Nuevo valor del puntero a flight plan
 * @return            : 1 - Success, 0 - Fail
 * ID                 : 0x5027
 *----------------------------------------------------------------------------*/
int drp_fpl_set_index(void *param)
{
    int index = *((int *)param);
    dat_setCubesatVar(dat_fpl_index, index);

    return 1;
}

/*------------------------------------------------------------------------------
 *                              DRP_FLP_SET_CMD
 *------------------------------------------------------------------------------
 * @brief             : Agrega la id del comando dado al flight plan en la
 *                       posicion dado por el valor actual de dat_fpl_index
 * @return            : 1 - Success, 0 - Fail
 * ID                 : 0x5028
 *----------------------------------------------------------------------------*/
int drp_fpl_set_cmd(void *param)
{
    //Se recupera el comando y el indice
    int cmdid = *((int *)param);
    int index = dat_getCubesatVar(dat_fpl_index);

    //Se actualiza el flight plan
    int result = dat_setFlightPlan_cmd(index, cmdid);

    return result;
}

/*------------------------------------------------------------------------------
 *                              DRP_FLP_SET_PARAM
 *------------------------------------------------------------------------------
 * @brief             : Actualiza en el flight plan el valor del parametro
 *                      asociado al indice que contiene actualmente la variable
 *                      dat_fpl_index.
 * @return            : 1 - Success, 0 - Fail
 * ID                 : 0x5029
 *----------------------------------------------------------------------------*/
int drp_fpl_set_param(void *param)
{
    //Se recupera el comando y el indice
    int var = *((int *)param);
    int index = dat_getCubesatVar(dat_fpl_index);

    //Se actualiza el flight plan
    int result = dat_setFlightPlan_param(index, var);

    return result;
}

int drp_debug(void *param){
    int ind=*((int*)param);

    char ret[10];
    itoa(ret, (unsigned int)ind, 10); con_printf("calling drp_debug"); con_printf(ret); con_printf("()...\r\n");

    switch(ind){
        case 1:
            drp_debug1();
            break;
        case 2:
            drp_debug2();
            break;
        case 3:
            drp_debug3();
            break;
        case 4:
            drp_debug4();
            break;
        default:
            //
            break;
    }

    return 1;
}

int drp_executeBeforeFlight(void *param){
    drp_dat_CubesatVar_EBF();
    drp_dat_FlightPlan_EBF();
    drp_dat_TelecmdBuffer_EBF();

    int mode=*((int *)param);
    if(mode==1){return 1;}

    con_printf("****************************************************\r\n");
    con_printf("drp_executeBeforeFlight finalizo\r\n");
    con_printf("Para quedar en config de vuelo, se\r\n");
    con_printf("DEBE apagar el SUCHAI, hagalo ANTES de:\r\n");
    con_printf("****************************************************\r\n");
    
    const unsigned int _1sec_check = (1000) / portTICK_RATE_MS;
    int i;
    for(i=20;i>=1;i--){
        vTaskDelay(_1sec_check);
        char buffer[10];
        itoa(buffer, (unsigned int)i,10); con_printf(buffer); con_printf(" segundos..\r\n");
    }

    return 1;
}

void drp_dat_CubesatVar_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("Setting CubesatVar in launch configuration\n");
    #endif

    // Setea TODAS las variables a 0xFFFF
    drp_memEEPROM_erase();

    //settings particulares:

    //Operation mode partir�a en modo RSSI
    dat_setCubesatVar(dat_ppc_opMode, DAT_PPC_OPMODE_RSSI);
    // hoursAive debe partir en cero, para que Housekeeping la incremente cada 1 hora
    dat_setCubesatVar(dat_ppc_hoursAlive, 0x0000);  
    // hoursWithoutReset debe partir en cero, para que Housekeeping la incremente cada 1 hora
    dat_setCubesatVar(dat_ppc_hoursWithoutReset, 0x0000);
    //memSD parte como inactiva
    dat_setCubesatVar(dat_msd_status, 0);
    //Telecomand Buffer parte vacios
    dat_setCubesatVar(dat_trx_newCmdBuff, 0x0000);      // Doesn't exist unproccessed TC Frames in TRX
    dat_setCubesatVar(dat_trx_newTcFrame, 0x0000);      // Doues't exist unproccessed TC in internal buffer
    //Todos los Payloads dat_pay_xxx_perform parten activos
    DAT_Payload pay_i; DAT_CubesatVar pay_i_perfVar;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++)
    {
        pay_i_perfVar = dat_pay_i_to_performVar(pay_i);
        dat_setCubesatVar(pay_i_perfVar, 0x0001);
    }

    //Se considera a la antena como NO-Desplegada
    dat_setCubesatVar(dat_dep_ant_deployed, 0);

    //Al desplegarse el SUCHAI, se parte bloqueando comandos de TCM hasta que la bateria se cargue
    dat_setCubesatVar(dat_eps_state_flag, 1 );
}

void drp_memEEPROM_erase(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("    Erasing memEEPROM\n");
    #endif

    unsigned int address=0;
    unsigned int data=0xFFFF;

    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++){
        writeIntEEPROM1(address, data);
    }
}

void drp_dat_FlightPlan_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("Setting FligthPlan in launch configuration..\n");
    #endif

    dat_erase_FlightPlanBuff();
    
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("Setting initial commands in FligthPlan..\n");
    #endif
    //aca debe ir la configuracion inicial del FligthPlan
    //esta es la configuracion a cargar antes del lanzamiento
    int i;
    for(i=0;i<1440;i++){
        if(i%2==0){
            //dat_setFlightPlan_cmd(i, 0x1002);
            //dat_setFlightPlan_param(i, 0x0000);
        }
    }
}

void drp_dat_TelecmdBuffer_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("Setting TelecmdBuffer in launch configuration..\n");
    #endif

    dat_erase_TeleCmdBuff();
}

void drp_CubesatVar_update(int indxVar){
    
    #if (SCH_CMDDRP_VERBOSE >=2)
        con_printf("drp_CubesatVar_update(");
        utoa(ret, (unsigned int)indxVar, 10);
        //sprintf (buffer, "%d", (unsigned int)indx)Var;
        con_printf(ret); con_printf(")\r\n");
    #endif

    switch(indxVar){
        case dat_ppc_lastResetSource:
            //drp_ppc_lastResetSource();
            //Variables modificada solamente en dat_onResetCubesatVar
            break;
        case dat_ppc_hoursAlive:
            //drp_ppc_hoursAlive();
            //Solo ee llama (actualiza) por taskHousekeeping cada 1hora
            break;
        case dat_ppc_hoursWithoutReset:
            //drp_ppc_hoursWithoutReset();
            //Solo ee llama (actualiza) por taskHousekeeping cada 1hora
            break;
        case dat_ppc_resetCounter:
            //drp_ppc_resetCounter();
            // resetCounter no se actualiza durante la marcha, solo en dat_init_Cubestat
            break;
        case dat_ppc_enwdt: // 1=WDT Active, 0=WDT Inactive
            drp_ppc_enwdt();
            break;
        case dat_ppc_osc: // holds Current Oscillator
            drp_ppc_osc();
            break;
        case dat_ppc_MB_nOE_USB_nINT_stat:
            drp_ppc_MB_nOE_USB_nINT_stat();
            break;
        case dat_ppc_MB_nOE_MHX_stat:
            drp_ppc_MB_nOE_MHX_stat();
            break;
        case dat_ppc_MB_nON_MHX_stat:
            drp_ppc_MB_nON_MHX_stat();
            break;
        case dat_ppc_MB_nON_SD_stat:
            drp_ppc_MB_nON_SD_stat();
            break;

    //DEP => (C&DH subsystem)
        case dat_dep_ant_deployed: // 1=already deployed, 0=not deployed yet
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_ant_tries: // Number of tries to deploy antenna
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_year:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_month:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_week_day:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_day_number:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_hours:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_minutes:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case dat_dep_seconds:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;

    //RTC => (C&DH subsystem)
        case dat_rtc_year:
            drp_rtc_year();
            break;
        case dat_rtc_month:
            drp_rtc_month();
            break;
        case dat_rtc_week_day:
            drp_rtc_week_day();
            break;
        case dat_rtc_day_number:
            drp_rtc_day_number();
            break;
        case dat_rtc_hours:
            drp_rtc_hours();
            break;
        case dat_rtc_minutes:
            drp_rtc_minutes();
            break;
        case dat_rtc_seconds:
            drp_rtc_seconds();
            break;

    //EPS => (Energy subsystem)

    #if SCH_EPS_ONBOARD==1            
        case dat_eps_bat0_voltage:          
            drp_eps_bat0_voltage();
            break;
        case dat_eps_bat0_current:
            drp_eps_bat0_current();
            break;
        case dat_eps_bus5V_current:
            drp_eps_bus5V_current();
            break;
        case dat_eps_bus3V_current:
            drp_eps_bus3V_current();
            break;
        case dat_eps_bus_battery_current:
            drp_eps_bus_battery_current();
            break;
        case dat_eps_bat0_temp:
            drp_eps_bat0_temp();
            break;
        case dat_eps_panel_pwr:
            drp_eps_panel_pwr();
            break;
        case dat_eps_status:
            drp_eps_status();
            break;
        case dat_eps_soc:
            drp_eps_soc();
            break;
        case dat_eps_socss:
            drp_eps_socss();
            break;
        case dat_eps_state_flag:
            drp_eps_state_flag();
            break;
        case dat_eps_charging:
            drp_eps_charging();
            break;
    #endif

    #if (SCH_TRX_TYPE_ONBOARD==1 || SCH_TRX_TYPE_ONBOARD==2)
        //TRX => (Communication subsystem)
        case dat_trx_frec_tx:
            drp_trx_frec_tx();
            break;          // TX Freq
        case dat_trx_frec_rx:
            drp_trx_frec_rx();
            break;          // RX Freq
        case dat_trx_opmode:
            drp_trx_opmode();
            break;           // Operation mode
        case dat_trx_temp_hpa:
            drp_trx_temp_hpa();
            break;         // Temp of HPA
        case dat_trx_temp_mcu:
            drp_trx_temp_mcu();
            break;         // Temp of MCU
        case dat_trx_rssi:
            drp_trx_rssi();
            break;             // RSSI        case :             break; Signal level
        case dat_trx_rssi_mean:
            //drp_trx_rssi_mean();
            //lo hace taskCommunication
            break;
        case dat_trx_beacon_pwr:
            drp_trx_beacon_pwr();
            break;       // Beacon power
        case dat_trx_telemetry_pwr:
            drp_trx_telemetry_pwr();
            break;    // Telemetry Power
        case dat_trx_status_tc:
            drp_trx_status_tc();
            break;        // Status Register of TC
        case dat_trx_lastcmd_day:
            /* Se actualiza cuando se reciben TC */
            break;
        case dat_trx_count_tc:
            /* Se actualiza cuando se reciben TC */
            break;
        case dat_trx_count_tm:
            /* Se actualiza cuando se envia TM */
            break;
    #endif
    // Cmd buffer control
        case dat_trx_newTcFrame:
            //Variables modificada solamente en taskComunications
            break;       // Exist any unprocessed TcFrame
        case dat_trx_newCmdBuff:
            //Variables modificada solamente en taskComunications
            break;       // Exist unprocessed CMD in the internal buffer

    //FLIGHT PLAN
        case dat_fpl_index:
            //Solo se modifica no se actualiza
            break;            // Indice del flight plan que sera editado


    //On default (para aquellos que no tienen un update asociado)
        default:
            //do nothing on default
            #if (SCH_CMDDRP_VERBOSE >=2)
                con_printf("default  => drp_CubesatVar_update(");
                utoa(ret, (unsigned int)indxVar, 10);
                // (buffer, "%d", (unsigned int)indxVar);
                con_printf(ret); con_printf(")\r\n");
            #endif
            break;
    }

    
}


/*
void drp_ppc_hoursAlive(void){
    int arg = dat_getCubesatVar(dat_ppc_hoursAlive)+1;
    dat_setCubesatVar(dat_ppc_hoursAlive, arg);
}
void drp_ppc_hoursWithoutReset(void)
{
    // En el futro esta funcion deber� ser llamada desde alguna interrupcion periodica del RTCC.
    // O leer del RTCC y comparar con su valor actual (get_ppc_hoursWithoutReset) y decidir
    // si debe incrementarlo y por cuanto 


    //int ar=*( (int *)param ); char ret[6];
    //con_printf("param= ");  Hex16ToAscii( ar, ret); con_printf(ret); con_printf("\n");
    int arg = dat_getCubesatVar(dat_ppc_hoursWithoutReset)+1;
    dat_setCubesatVar(dat_ppc_hoursWithoutReset, arg);
    
}
*/

/*------------------------------------------------------------------------------
 *                                  DRP PPC ENWDT
 *------------------------------------------------------------------------------
 * Description        : Update the state of the WDT
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_enwdt(void)
{
    /* Leer el registro de los CONFIGWORD no es directo
     * (esta "beyond user visibility pages"). Asi es que la 
     * implementacion de este comando es asumir que se parte en WDT_ENABLE
     *  y luego registrar las desactivaciones/activaciones del WDT a trav�s
     * de ppc_enwdt (ojo: no es lo mismo que drp_ppc_enwdt )*/

    
}

/*------------------------------------------------------------------------------
 *                                  DRP PPC OSC
 *------------------------------------------------------------------------------
 * Description        : Update current oscillator
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_osc(void)
{
    /* FRC_OSC                           0x0000 //Internal oscillator
    *  FRC_OSC_WITH_POSTSCALER_PLL       0x0001 //Internal oscillator with Postscalar PLL enabled
    *  PRIMARY_OSC                       0x0002 //Primary oscillator
    *  PRIMARY_OSC_WITH_PLL              0x0003 //Primary oscillator with PLL
    *  SECONDRY_OSC                      0x0004 //Secondary oscillator
    *  LOW_POWER_RC                      0x0005 //Low Power RC oscillator
    *  FRC_OSC_WITH_POSTSCALER           0x0007 //Internal oscillator with postscalar
     */

    dat_setCubesatVar(dat_ppc_osc, (int)Current_OSCILLATOR() );

    
}

/*------------------------------------------------------------------------------
 *                                  PPC LAST RESET SOURCE
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_MB_nOE_USB_nINT_stat(void)
{
    if(PPC_MB_nOE_USB_nINT_CHECK)
    {
        dat_setCubesatVar(dat_ppc_MB_nOE_USB_nINT_stat, 1);
    }
    else
    {
        dat_setCubesatVar(dat_ppc_MB_nOE_USB_nINT_stat, 0);
    }
    
}

/*------------------------------------------------------------------------------
 *                                  drp_ppc_MB_nOE_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_MB_nOE_MHX_stat(void)
{
    if(PPC_MB_nOE_MHX_CHECK)
    {
        dat_setCubesatVar(dat_ppc_MB_nOE_MHX_stat, 1);
    }
    else
    {
        dat_setCubesatVar(dat_ppc_MB_nOE_MHX_stat, 0);
    }
    
}

/*------------------------------------------------------------------------------
 *                                  drp_ppc_MB_nON_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_MB_nON_MHX_stat(void)
{
    if(PPC_MB_nON_MHX_CHECK)
    {
        dat_setCubesatVar(dat_ppc_MB_nON_MHX_stat, 1);
    }
    else
    {
        dat_setCubesatVar(dat_ppc_MB_nON_MHX_stat, 0);
    }
    
}

/*------------------------------------------------------------------------------
 *                                  drp_ppc_MB_nON_SD_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_ppc_MB_nON_SD_stat(void)
{
    if(PPC_MB_nON_SD_CHECK)
    {
        dat_setCubesatVar(dat_ppc_MB_nON_SD_stat, 1);
    }
    else
    {
        dat_setCubesatVar(dat_ppc_MB_nON_SD_stat, 0);
    }
    
}

/*
 * Aca deberian ir comandos para DEP, pero no son necesarios, pues se setean una
 * sola vez y nunca son actualizados
 * La unica funcion es para resetear DEP en la mem Flash
 */

/*------------------------------------------------------------------------------
 *                                  drp_rtc_year
 *------------------------------------------------------------------------------
 * Description        : Updates the RTC structure inside Cubestat
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_rtc_year(void)
{
    dat_setCubesatVar(dat_rtc_year, RTC_get_year() );
    
}

void drp_rtc_month(void)
{
    dat_setCubesatVar(dat_rtc_month, RTC_get_month() );
    
}

void drp_rtc_week_day(void)
{
    dat_setCubesatVar(dat_rtc_week_day, RTC_get_week_day() );
    
}

void drp_rtc_day_number(void)
{
    dat_setCubesatVar(dat_rtc_day_number, RTC_get_day_num() );
    
}

void drp_rtc_hours(void)
{
    dat_setCubesatVar(dat_rtc_hours, RTC_get_hours() );
    
}

void drp_rtc_minutes(void)
{
    dat_setCubesatVar(dat_rtc_minutes, RTC_get_minutes() );
    
}

void drp_rtc_seconds(void)
{
    dat_setCubesatVar(dat_rtc_seconds, RTC_get_seconds() );
    
}


/*------------------------------------------------------------------------------
 *                                  drp_eps_bat0_voltage
 *------------------------------------------------------------------------------
 * Description        : Updates the battery voltage based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bat0_voltage(void)
{
    dat_setCubesatVar(dat_eps_bat0_voltage, readEPSvars(EPS_ID_bat0_voltage));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_bat0_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bat0_current(void)
{
    dat_setCubesatVar(dat_eps_bat0_current, readEPSvars(EPS_ID_bat0_current));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_bus5V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 5V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bus5V_current(void)
{
    dat_setCubesatVar(dat_eps_bus5V_current, readEPSvars(EPS_ID_bus5V_current));
    
}

/*------------------------------------------------------------------------------
 *                                   drp_eps_bus3V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 3V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bus3V_current(void)
{
    dat_setCubesatVar(dat_eps_bus3V_current, readEPSvars(EPS_ID_bus3V_current));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_bus_battery_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bus_battery_current(void)
{
    dat_setCubesatVar(dat_eps_bus_battery_current, readEPSvars(EPS_ID_bus_battery_current));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_bat0_temp
 *------------------------------------------------------------------------------
 * Description        : Updates the battery �temp based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_bat0_temp(void)
{
    dat_setCubesatVar(dat_eps_bat0_temp, readEPSvars(EPS_ID_bat0_temp));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_panel_pwr
 *------------------------------------------------------------------------------
 * Description        : Updates the power sourced by the panels based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_panel_pwr(void)
{
    dat_setCubesatVar(dat_eps_panel_pwr, readEPSvars(EPS_ID_panel_pwr));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_status
 *------------------------------------------------------------------------------
 * Description        : Updates the EPS status based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_status(void)
{
    dat_setCubesatVar(dat_eps_status, readEPSvars(EPS_ID_status));
    
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_soc
 *------------------------------------------------------------------------------
 * Description        : Updates the battery soc estimation based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_soc(void)
{
    int soc = readEPSvars(EPS_ID_soc);
    dat_setCubesatVar(dat_eps_soc, soc);
    
    #if (SCH_CMDDRP_VERBOSE >=1)
        char buf[10];
        itoa(buf, soc, 10);
        con_printf("SOC= "); con_printf(buf); con_printf("\r\n");
    #endif
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_socss
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "steady state" soc estimation based
 *                      on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_socss(void)
{
    dat_setCubesatVar(dat_eps_socss, readEPSvars(EPS_ID_socss));
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_state_flag
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
void drp_eps_state_flag(void)
{
    dat_setCubesatVar(dat_eps_state_flag, readEPSvars(EPS_ID_state_flag));
}

/*------------------------------------------------------------------------------
 *                                  drp_eps_charging
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "charging state"
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void drp_eps_charging(void)
{
    int i_value = readEPSvars(EPS_ID_current_dir);
    i_value = i_value < 256 ? 1:0;
    dat_setCubesatVar(dat_eps_charging, i_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX FREC TX
 *------------------------------------------------------------------------------
 * Description        : Update TRX FTX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5011
 * NOTE               : DEPRECATED
 *----------------------------------------------------------------------------*/
void drp_trx_frec_tx(void)
{
    /* Reading FTX */
    long long_value = -1;
//    long_value = (long)TRX_ReadRegister(TRX_FTX_H)<<16 ;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FTX_M)<<8;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FTX_L);
    /* Writing TRX_FTX to repo */
    dat_setCubesatVar(dat_trx_frec_tx, (int)long_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX FREC RX
 *------------------------------------------------------------------------------
 * Description        : Update TRX FRX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5012
 *----------------------------------------------------------------------------*/
void drp_trx_frec_rx(void)
{
    /* Reading FRX */
    long long_value = -1;
//    long_value = (long)TRX_ReadRegister(TRX_FRX_H)<<16 ;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FRX_M)<<8;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FRX_L);
    /* Writing TRX_FRX to repo */
    dat_setCubesatVar(dat_trx_frec_rx, (int)long_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX OPMODE
 *------------------------------------------------------------------------------
 * Description        : Update TRX Operation Mode status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5013
 *----------------------------------------------------------------------------*/
void drp_trx_opmode(void)
{
    /* TODO: Reading OP_MODE */
    char char_value = 0;
//    char_value = TRX_ReadRegister(TRX_MODE);
    /* Writing TRX_FTX to repo */
    dat_setCubesatVar(dat_trx_opmode, (int)char_value);

    
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
void drp_trx_temp_hpa(void)
{
    int int_value = -1;

    /*TODO: Reading TEMP_HPA */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPHPA_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPHPA_L);
    /* Writing TEMP_HPA to repo */
    dat_setCubesatVar(dat_trx_temp_hpa, int_value);

    
}

void drp_trx_temp_mcu(void)
{
    int int_value = -1;

    /* TODO: Reading TEMP_MCU */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPMCU_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPMCU_L);
    /* Writing TEMP_MCU to repo */
    dat_setCubesatVar(dat_trx_temp_mcu, int_value);

    
}


#define DRP_RSSI_LIST_LEN   20  /*!< Largo del buffer de datos para calcular rssi promedio */
#define DRP_RSSI_MEAN_INIT  99 /*|< Aca hay que tener cuidado de que (RSSI_MEAN_INIT)*(RSSI_LIST_LEN)
                            no porvoque overflow, por eso extendi el tipo de RSSI_LIST[] a long */

#define DRP_RSSI_CNT_MAX   600  /*!< Segundos que RSSI_CNT puede pasar sin que se actualice RSSI_MEAN */

/* Funcion auxiliar para mentener la media de rssi actualiada */
static int drp_update_rssi_mean(int new_value);

long DRP_RSSI_LIST[DRP_RSSI_LIST_LEN];
int drp_rssi_list_p = 0;
int DRP_RSSI_MEAN = DRP_RSSI_MEAN_INIT;

int drp_trx_rssi_mean(int arg){
    static int init;

    if(init==0){
        /* Inicializar el buffer de valores de RSSI */
        int i, rssi_list_init=DRP_RSSI_MEAN_INIT;
        for(i=0; i<DRP_RSSI_LIST_LEN; i++)
        {
            DRP_RSSI_LIST[i] = rssi_list_init;
        }
        init++;
    }

    int res=drp_update_rssi_mean(arg);
    return res;
}

/**
 * Calcular el promedio del nivel de RSSI sobre la lista de datos disponiblea.
 * Retorna el valor actualizado al desechar mediciones antiguas
 *
 * @param new_value Nuevo valor para agregar a la lista y actualizar el promedio
 * @return Promedio del nivel de RSSI sobre los datos disponibles
 */
static int drp_update_rssi_mean(int new_value){
    drp_rssi_list_p = (drp_rssi_list_p < DRP_RSSI_LIST_LEN) ? drp_rssi_list_p : 0;
    DRP_RSSI_LIST[drp_rssi_list_p] = new_value;
    drp_rssi_list_p++; int res;

    #if (SCH_CMDDRP_VERBOSE >=2)
        itoa(buf, new_value, 10);
        con_printf("new_value= "); con_printf(buf); con_printf("\r\n");
        itoa(buf, rssi_list_p, 10);
        con_printf("rssi_list_p= "); con_printf(buf); con_printf("\r\n");
    #endif

    int i; long sum = 0;
    for(i=0; i<DRP_RSSI_LIST_LEN; i++)
    {
        sum+=DRP_RSSI_LIST[i];
    }

    res=sum/DRP_RSSI_LIST_LEN;

    #if (SCH_CMDDRP_VERBOSE >=2)
        itoa(buf, res,10);
        con_printf("res= "); con_printf(buf); con_printf("\r\n");
    #endif

    return res;
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX RSSI
 *------------------------------------------------------------------------------
 * Description        : Update TRX RSSI status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5015
 *----------------------------------------------------------------------------*/
void drp_trx_rssi(void)
{
    /* TODO: Reading RSSI */
    int int_value = -1;
//    int_value = (int)TRX_ReadRegister(TRX_RSSI_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_RSSI_L);

#ifdef TRX_RSSI_DBM
    /* Convert to dBm */
    double dob_value =  (double)TRX_RSSI_GAIN * (double)int_value +
                        (double)TRX_RSSI_OFFSET;
    int_value = (int)dob_value;
#endif

    /* Writing RSSI to repo */
    dat_setCubesatVar(dat_trx_rssi, int_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX BEACON PWR
 *------------------------------------------------------------------------------
 * Description        : Update TRX BEACON PWR status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5016
 * NOTE               : DEPRECATED
 *----------------------------------------------------------------------------*/
void drp_trx_beacon_pwr(void)
{
    /* Reading BEACON_PWR */
    char char_value = 0;
//    char_value = TRX_ReadRegister(TRX_BEACONPOWER);
    /* Writing BEACON_PWR to repo */
    dat_setCubesatVar(dat_trx_beacon_pwr, (int)char_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX TM PWR
 *------------------------------------------------------------------------------
 * Description        : Update TRX TM PWR status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5017
 * NOTE               : DEPRECATED
 *----------------------------------------------------------------------------*/
void drp_trx_telemetry_pwr(void)
{
    char char_value = 0;
    /* Reading TELEMETRY_PWR */
//    char_value = TRX_ReadRegister(TRX_TMPOWER);
    /* Writing BEACON_PWR to repo */
    dat_setCubesatVar(dat_trx_telemetry_pwr, (int)char_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX STATUS TC
 *------------------------------------------------------------------------------
 * Description        : Update TRX FRX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5018
 *----------------------------------------------------------------------------*/
void drp_trx_status_tc(void)
{
    int value = 0;
    /* TODO: Reading STATUS_TC */
//    value = TRX_CheckNewTC();
    /* Writing BEACON_PWR to repo */
    dat_setCubesatVar(dat_trx_status_tc, value);
}

/*------------------------------------------------------------------------------
 *                              DRP_DEP_WRITE_DEPLOYED
 *------------------------------------------------------------------------------
 * Description        : Updates the dep_ant_deployed flag in persistent memory
 * Arguments          : flag value
 * Return Value       : 1 - Success, 0 - Fail
 * ID                 : 0x5026
 *----------------------------------------------------------------------------*/
void drp_dep_write_deployed(int dep_state, int dep_tries)
{
    dat_setCubesatVar(dat_dep_ant_deployed, dep_state);
    dat_setCubesatVar(dat_dep_ant_tries, dep_tries);

    if(dep_state)
    {
        dat_setCubesatVar(dat_dep_year, RTC_get_year() );
        dat_setCubesatVar(dat_dep_month, RTC_get_month() );
        dat_setCubesatVar(dat_dep_day_number, RTC_get_day_num() );
        dat_setCubesatVar(dat_dep_week_day, RTC_get_week_day() );
        dat_setCubesatVar(dat_dep_hours, RTC_get_hours() );
        dat_setCubesatVar(dat_dep_minutes, RTC_get_minutes() );
        dat_setCubesatVar(dat_dep_seconds, RTC_get_seconds() );
    }
}

void drp_debug1(void){
    int i;

    con_printf("Testing rand()\r\n");
    for(i=0;i<0xFFFF;i++){
        unsigned char c= rand();
        con_printf("rand()=");
        char buffer[6];
        itoa(buffer,  (unsigned int)c, 10);
        con_printf(buffer); con_printf("\r\n");
        ClrWdt();
    }

}

void drp_debug2(void){
    char buffer[10];
    int value=0, res=0;
    unsigned char index;
    long block;
    
    con_printf("(Destructive) Testing -1block r/w- memSD\r\n");
    for(block=0;block<1024;block++){
        con_printf("testing block j="); itoa(buffer, block,10); con_printf(buffer); con_printf("\n");

        value=0xA000;
        for(index=0;index<=0xFF;index++, value++){

            con_printf("writing: ");
            msd_setVar_1BlockExtMem( block, index, value);
            itoa(buffer, index,10); con_printf("value["); con_printf(buffer); con_printf("]=");
            itoa(buffer, value,10); con_printf(buffer); con_printf("    |    ");

            con_printf("reading: ");
            msd_getVar_1BlockExtMem( block, index, &res);
            itoa(buffer, index,10); con_printf("value["); con_printf(buffer); con_printf("]=");
            itoa(buffer, res,10); con_printf(buffer); con_printf("    |    ");

            con_printf("comparing: ");
            if(value==res){ con_printf("ok"); }
            else{ con_printf("fail"); }
            con_printf("\n");

            //con_printf("ClrWdt()\r\n");
            ClrWdt();
        }
    }
    //drp_memSD_BlockErase();

    
}

void drp_debug3(void){
    char buffer[10];
    int value=0, res=0;
    unsigned int index;
    long block;

    con_printf("(Destructive) Testing -256block r/w- memSD\r\n");
    for(block=0;block<1024;block=block+256){
        con_printf("testing block j="); itoa(buffer, block,10); con_printf(buffer); con_printf("\n");

        value=0xA000;
        for(index=0;index<=0xFFFF;index++, value++){

            con_printf("writing: ");
            msd_setVar_256BlockExtMem( block, index, value);
            itoa(buffer, index,10); con_printf("value["); con_printf(buffer); con_printf("]=");
            itoa(buffer, value,10); con_printf(buffer); con_printf("    |    ");

            con_printf("reading: ");
            msd_getVar_256BlockExtMem( block, index, &res);
            itoa(buffer, index,10); con_printf("value["); con_printf(buffer); con_printf("]=");
            itoa(buffer, res,10); con_printf(buffer); con_printf("    |    ");

            con_printf("comparing: ");
            if(value==res){ con_printf("ok"); }
            else{ con_printf("fail"); }
            con_printf("\n");

            //con_printf("ClrWdt()\r\n");
            ClrWdt();
        }
    }
    //drp_memSD_BlockErase();

}

void drp_debug4(void){
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
    drp_memEEPROM_erase();

}

void drp_debug5(void){
    con_printf("=>Testing Payload API\r\n");
    char buffer[10];

    con_printf("=>Initial settings:\r\n");
    DAT_Payload pay_i; unsigned int maxIndx, nextIndx, indx;
    pay_i = dat_pay_expFis;
    maxIndx = 0x0005;   //numero de 10-minutos en un dia
    nextIndx = 0x0000;
    con_printf("pay_i="); itoa(buffer,(unsigned int)pay_i,10); con_printf(buffer); con_printf("\r\n");
    con_printf("maxIndx="); itoa(buffer,(unsigned int)maxIndx,10); con_printf(buffer); con_printf("\r\n");
    con_printf("nextIndx="); itoa(buffer,(unsigned int)nextIndx,10); con_printf(buffer); con_printf("\r\n");

    //Seteo
    dat_setMaxPayIndx(pay_i, maxIndx);
    con_printf("dat_setMaxPayIndx( pay_i, maxIndx);\r\n");
    dat_setNextPayIndx(pay_i, nextIndx);
    con_printf("dat_setNextPayIndx( pay_i, nextIndx);\r\n");

    //Compruebo
    indx = dat_getMaxPayIndx(pay_i);
    con_printf("indx = dat_getMaxPayIndx(pay_i);\r\nindx = ");
    itoa(buffer, (unsigned int)indx,10); con_printf(buffer); con_printf("\r\n");
    indx = dat_getNextPayIndx(pay_i);
    con_printf("indx = dat_getNextPayIndx(pay_i);\r\nindx = ");
    itoa(buffer, (unsigned int)indx,10); con_printf(buffer); con_printf("\r\n");

    con_printf("=>FIFO buffer check:\r\n");
    unsigned int i; int value=0xAA00, val; BOOL stat;
    for(i=0;i<0x000F;i++,value++){
        indx = dat_getNextPayIndx(pay_i);
        con_printf("indx = dat_getNextPayIndx(pay_i);\r\nindx = ");
        itoa(buffer, (unsigned int)indx,10); con_printf(buffer); con_printf("\r\n");

        //escribo
        stat = dat_setPayloadVar( pay_i, value);
        con_printf("stat = dat_setPayloadVar( pay_i, value);\r\nstat =  ");
        if(stat==TRUE){ con_printf("TRUE\r\n"); }
        else{ con_printf("FALSE\nFin del test\r\n"); break; }

        //leo
        stat = dat_getPayloadVar( pay_i, indx, &val);
        con_printf("stat = dat_getPayloadVar( pay_i, indx, &val);\r\nstat = ");
        if(stat==TRUE){ con_printf("TRUE\r\n"); }
        else{ con_printf("FALSE\nFin del test\r\n"); break; }

        //Confirmo
        con_printf("value=");
        itoa(buffer, (unsigned int)value,10); con_printf(buffer); con_printf("  |  ");
        con_printf("val=");
        itoa(buffer, (unsigned int)val,10); con_printf(buffer); con_printf("\r\n");

    }
}
