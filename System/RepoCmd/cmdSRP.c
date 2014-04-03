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
    srpFunction[(unsigned char)srp_id_print_sta_CubesatVar] = srp_print_sta_CubesatVar;
    srp_sysReq[(unsigned char)srp_id_print_sta_CubesatVar]  = CMD_SYSREQ_MIN;
    //De update en general en sta_CubesatVar
    srpFunction[(unsigned char)srp_id_updateAll_sta_CubesatVar] = srp_updateAll_sta_CubesatVar;
    srp_sysReq[(unsigned char)srp_id_updateAll_sta_CubesatVar]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_hoursAlive] = srp_update_sta_CubesatVar_hoursAlive;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_hoursAlive]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_hoursWithoutReset] = srp_update_sta_CubesatVar_hoursWithoutReset;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_hoursWithoutReset]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_nSended_tm] = srp_update_sta_CubesatVar_nSended_tm;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_nSended_tm]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_nReceived_tc] = srp_update_sta_CubesatVar_nReceived_tc;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_nReceived_tc]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_opMode] = srp_update_sta_CubesatVar_opMode;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_opMode]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_trx_rssi] = srp_update_sta_CubesatVar_trx_rssi;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_trx_rssi]  = CMD_SYSREQ_MIN;
    srpFunction[(unsigned char)srp_id_update_sta_CubesatVar_trx_rssi_mean] = srp_update_sta_CubesatVar_trx_rssi_mean;
    srp_sysReq[(unsigned char)srp_id_update_sta_CubesatVar_trx_rssi_mean]  = CMD_SYSREQ_MIN;

}

// comands to update Cubestat. They may take one, or no arguments
int srp_updateAll_sta_CubesatVar(void *param){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("srp_updateAll_sta_CubesatVar()..\r\n");
    #endif

    #if (SCH_EPS_ONBOARD==1)
        updateSOCEPS();
        updateStatusEPS();
    #else
        sta_setCubesatVar(sta_eps_soc, CMD_SYSREQ_MAX);
    #endif

    STA_CubesatVar indxVar;
    for(indxVar=0;indxVar<sta_cubesatVar_last_one;indxVar++){
        srp_CubesatVar_update(indxVar);
    }

    return 1;
}

int srp_update_sta_CubesatVar_opMode(void *param){

    int arg = *((int *)param);
    sta_setCubesatVar(sta_ppc_opMode, arg);

    return 1;
}

int srp_update_sta_CubesatVar_hoursWithoutReset(void *param)
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

int srp_update_sta_CubesatVar_hoursAlive(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_ppc_hoursAlive)+1;
    sta_setCubesatVar(sta_ppc_hoursAlive, arg);
    return 1;
}

int srp_update_sta_CubesatVar_nSended_tm(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_trx_count_tm)+1;
    sta_setCubesatVar(sta_trx_count_tm, arg);
    return 1;
}

int srp_update_sta_CubesatVar_nReceived_tc(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_getCubesatVar(sta_trx_count_tc)+1;
    sta_setCubesatVar(sta_trx_count_tc, arg);
    return 1;
}

int srp_update_sta_CubesatVar_trx_rssi_mean(void *param)
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
int srp_update_sta_CubesatVar_trx_rssi(void *param)
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
int srp_trx_newTcFrame(void *param)
{
#if (SCH_CMDDRP_VERBOSE >= 1)
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
int srp_trx_newCmdBuff(void *param)
{
#if (SCH_CMDDRP_VERBOSE >= 1)
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

// functions to "read" Cubestat
/*------------------------------------------------------------------------------
 *                                  DRP READALL DAT CUBESAT
 *------------------------------------------------------------------------------
 * Description        : Read/Prints all Cubestat structure
 * Arguments          : None
 * Return Value       : int
 * ID                 : 0x501D
 *----------------------------------------------------------------------------*/
int srp_print_sta_CubesatVar(void *param)
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
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_frec_tx), 10);
        con_printf("trx_frec_tx= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_frec_rx), 10);
        con_printf("trx_frec_rx= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_opmode), 10);
        con_printf("trx_opmode= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_temp_hpa), 10);
        con_printf("trx_temp_hpa= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_temp_mcu), 10);
        con_printf("trx_temp_mcu= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer, (unsigned int)sta_getCubesatVar(sta_trx_rssi), 10); //itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_rssi), 10);
        con_printf("trx_rssi= "); con_printf(buffer); con_printf(" dBm\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_beacon_pwr), 10);
        con_printf("trx_beacon_pwr= "); con_printf(buffer); con_printf("\r\n");
        itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_telemetry_pwr), 10);
        con_printf("trx_telemetry_pwr= "); con_printf(buffer); con_printf("\r\n");
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

void srp_memEEPROM_erase(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("    Erasing memEEPROM\n");
    #endif

    unsigned int address=0;
    unsigned int data=0xFFFF;

    for(address=MEP_FIRST_ADDR;address<=MEP_LAST_ADDR;address++){
        writeIntEEPROM1(address, data);
    }
}


void srp_CubesatVar_update(int indxVar){
    
    #if (SCH_CMDDRP_VERBOSE >=2)
        con_printf("srp_CubesatVar_update(");
        utoa(ret, (unsigned int)indxVar, 10);
        //sprintf (buffer, "%d", (unsigned int)indx)Var;
        con_printf(ret); con_printf(")\r\n");
    #endif

    switch(indxVar){
        case sta_ppc_lastResetSource:
            //srp_ppc_lastResetSource();
            //Variables modificada solamente en sta_onResetCubesatVar
            break;
        case sta_ppc_hoursAlive:
            //srp_ppc_hoursAlive();
            //Solo ee llama (actualiza) por taskHousekeeping cada 1hora
            break;
        case sta_ppc_hoursWithoutReset:
            //srp_ppc_hoursWithoutReset();
            //Solo ee llama (actualiza) por taskHousekeeping cada 1hora
            break;
        case sta_ppc_resetCounter:
            //srp_ppc_resetCounter();
            // resetCounter no se actualiza durante la marcha, solo en sta_init_Cubestat
            break;
        case sta_ppc_enwdt: // 1=WDT Active, 0=WDT Inactive
            srp_ppc_enwdt();
            break;
        case sta_ppc_osc: // holds Current Oscillator
            srp_ppc_osc();
            break;
        case sta_ppc_MB_nOE_USB_nINT_stat:
            srp_ppc_MB_nOE_USB_nINT_stat();
            break;
        case sta_ppc_MB_nOE_MHX_stat:
            srp_ppc_MB_nOE_MHX_stat();
            break;
        case sta_ppc_MB_nON_MHX_stat:
            srp_ppc_MB_nON_MHX_stat();
            break;
        case sta_ppc_MB_nON_SD_stat:
            srp_ppc_MB_nON_SD_stat();
            break;

    //DEP => (C&DH subsystem)
        case sta_dep_ant_deployed: // 1=already deployed, 0=not deployed yet
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_ant_tries: // Number of tries to deploy antenna
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_year:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_month:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_week_day:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_day_number:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_hours:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_minutes:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;
        case sta_dep_seconds:
            /* Ya que solo se seteara una vez taskDeployment no tiene sentido crear
            * comandos para actualizarlo */
            break;

    //RTC => (C&DH subsystem)
        case sta_rtc_year:
            srp_rtc_year();
            break;
        case sta_rtc_month:
            srp_rtc_month();
            break;
        case sta_rtc_week_day:
            srp_rtc_week_day();
            break;
        case sta_rtc_day_number:
            srp_rtc_day_number();
            break;
        case sta_rtc_hours:
            srp_rtc_hours();
            break;
        case sta_rtc_minutes:
            srp_rtc_minutes();
            break;
        case sta_rtc_seconds:
            srp_rtc_seconds();
            break;

    //EPS => (Energy subsystem)

    #if SCH_EPS_ONBOARD==1            
        case sta_eps_bat0_voltage:
            srp_eps_bat0_voltage();
            break;
        case sta_eps_bat0_current:
            srp_eps_bat0_current();
            break;
        case sta_eps_bus5V_current:
            srp_eps_bus5V_current();
            break;
        case sta_eps_bus3V_current:
            srp_eps_bus3V_current();
            break;
        case sta_eps_bus_battery_current:
            srp_eps_bus_battery_current();
            break;
        case sta_eps_bat0_temp:
            srp_eps_bat0_temp();
            break;
        case sta_eps_panel_pwr:
            srp_eps_panel_pwr();
            break;
        case sta_eps_status:
            srp_eps_status();
            break;
        case sta_eps_soc:
            srp_eps_soc();
            break;
        case sta_eps_socss:
            srp_eps_socss();
            break;
        case sta_eps_state_flag:
            srp_eps_state_flag();
            break;
        case sta_eps_charging:
            srp_eps_charging();
            break;
    #endif

    #if (SCH_TRX_ONBOARD==1)
        //TRX => (Communication subsystem)
        case sta_trx_frec_tx:
            srp_trx_frec_tx();
            break;          // TX Freq
        case sta_trx_frec_rx:
            srp_trx_frec_rx();
            break;          // RX Freq
        case sta_trx_opmode:
            srp_trx_opmode();
            break;           // Operation mode
        case sta_trx_temp_hpa:
            srp_trx_temp_hpa();
            break;         // Temp of HPA
        case sta_trx_temp_mcu:
            srp_trx_temp_mcu();
            break;         // Temp of MCU
        case sta_trx_rssi:
            srp_trx_rssi();
            break;             // RSSI        case :             break; Signal level
        case sta_trx_rssi_mean:
            //srp_trx_rssi_mean();
            //lo hace taskCommunication
            break;
        case sta_trx_beacon_pwr:
            srp_trx_beacon_pwr();
            break;       // Beacon power
        case sta_trx_telemetry_pwr:
            srp_trx_telemetry_pwr();
            break;    // Telemetry Power
        case sta_trx_status_tc:
            srp_trx_status_tc();
            break;        // Status Register of TC
        case sta_trx_lastcmd_day:
            /* Se actualiza cuando se reciben TC */
            break;
        case sta_trx_count_tc:
            /* Se actualiza cuando se reciben TC */
            break;
        case sta_trx_count_tm:
            /* Se actualiza cuando se envia TM */
            break;
    #endif
    // Cmd buffer control
        case sta_trx_newTcFrame:
            //Variables modificada solamente en taskComunications
            break;       // Exist any unprocessed TcFrame
        case sta_trx_newCmdBuff:
            //Variables modificada solamente en taskComunications
            break;       // Exist unprocessed CMD in the internal buffer

    //FLIGHT PLAN
        case sta_fpl_index:
            //Solo se modifica no se actualiza
            break;            // Indice del flight plan que sera editado


    //On default (para aquellos que no tienen un update asociado)
        default:
            //do nothing on default
            #if (SCH_CMDDRP_VERBOSE >=2)
                con_printf("default  => srp_CubesatVar_update(");
                utoa(ret, (unsigned int)indxVar, 10);
                // (buffer, "%d", (unsigned int)indxVar);
                con_printf(ret); con_printf(")\r\n");
            #endif
            break;
    }

    
}


/*
void srp_ppc_hoursAlive(void){
    int arg = sta_getCubesatVar(sta_ppc_hoursAlive)+1;
    sta_setCubesatVar(sta_ppc_hoursAlive, arg);
}
void srp_ppc_hoursWithoutReset(void)
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
void srp_ppc_enwdt(void)
{
    /* Leer el registro de los CONFIGWORD no es directo
     * (esta "beyond user visibility pages"). Asi es que la 
     * implementacion de este comando es asumir que se parte en WDT_ENABLE
     *  y luego registrar las desactivaciones/activaciones del WDT a traves
     * de ppc_enwdt (ojo: no es lo mismo que srp_ppc_enwdt )*/

    
}

/*------------------------------------------------------------------------------
 *                                  DRP PPC OSC
 *------------------------------------------------------------------------------
 * Description        : Update current oscillator
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_ppc_osc(void)
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
void srp_ppc_MB_nOE_USB_nINT_stat(void)
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
 *                                  srp_ppc_MB_nOE_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_ppc_MB_nOE_MHX_stat(void)
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
 *                                  srp_ppc_MB_nON_MHX_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_ppc_MB_nON_MHX_stat(void)
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
 *                                  srp_ppc_MB_nON_SD_stat
 *------------------------------------------------------------------------------
 * Description        : Updates the state of the PIC-controlled signals of Pumkins's MB
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_ppc_MB_nON_SD_stat(void)
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

/*
 * Aca deberian ir comandos para DEP, pero no son necesarios, pues se setean una
 * sola vez y nunca son actualizados
 * La unica funcion es para resetear DEP en la mem Flash
 */

/*------------------------------------------------------------------------------
 *                                  srp_rtc_year
 *------------------------------------------------------------------------------
 * Description        : Updates the RTC structure inside Cubestat
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_rtc_year(void)
{
    sta_setCubesatVar(sta_rtc_year, RTC_get_year() );
    
}

void srp_rtc_month(void)
{
    sta_setCubesatVar(sta_rtc_month, RTC_get_month() );
    
}

void srp_rtc_week_day(void)
{
    sta_setCubesatVar(sta_rtc_week_day, RTC_get_week_day() );
    
}

void srp_rtc_day_number(void)
{
    sta_setCubesatVar(sta_rtc_day_number, RTC_get_day_num() );
    
}

void srp_rtc_hours(void)
{
    sta_setCubesatVar(sta_rtc_hours, RTC_get_hours() );
    
}

void srp_rtc_minutes(void)
{
    sta_setCubesatVar(sta_rtc_minutes, RTC_get_minutes() );
    
}

void srp_rtc_seconds(void)
{
    sta_setCubesatVar(sta_rtc_seconds, RTC_get_seconds() );
    
}


/*------------------------------------------------------------------------------
 *                                  srp_eps_bat0_voltage
 *------------------------------------------------------------------------------
 * Description        : Updates the battery voltage based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bat0_voltage(void)
{
    sta_setCubesatVar(sta_eps_bat0_voltage, readEPSvars(EPS_ID_bat0_voltage));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_bat0_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bat0_current(void)
{
    sta_setCubesatVar(sta_eps_bat0_current, readEPSvars(EPS_ID_bat0_current));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_bus5V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 5V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bus5V_current(void)
{
    sta_setCubesatVar(sta_eps_bus5V_current, readEPSvars(EPS_ID_bus5V_current));
    
}

/*------------------------------------------------------------------------------
 *                                   srp_eps_bus3V_current
 *------------------------------------------------------------------------------
 * Description        : Updates the 3V bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bus3V_current(void)
{
    sta_setCubesatVar(sta_eps_bus3V_current, readEPSvars(EPS_ID_bus3V_current));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_bus_battery_current
 *------------------------------------------------------------------------------
 * Description        : Updates the battery bus current based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bus_battery_current(void)
{
    sta_setCubesatVar(sta_eps_bus_battery_current, readEPSvars(EPS_ID_bus_battery_current));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_bat0_temp
 *------------------------------------------------------------------------------
 * Description        : Updates the battery temp based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_bat0_temp(void)
{
    sta_setCubesatVar(sta_eps_bat0_temp, readEPSvars(EPS_ID_bat0_temp));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_panel_pwr
 *------------------------------------------------------------------------------
 * Description        : Updates the power sourced by the panels based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_panel_pwr(void)
{
    sta_setCubesatVar(sta_eps_panel_pwr, readEPSvars(EPS_ID_panel_pwr));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_status
 *------------------------------------------------------------------------------
 * Description        : Updates the EPS status based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_status(void)
{
    sta_setCubesatVar(sta_eps_status, readEPSvars(EPS_ID_status));
    
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_soc
 *------------------------------------------------------------------------------
 * Description        : Updates the battery soc estimation based on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_soc(void)
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
 *                                  srp_eps_socss
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "steady state" soc estimation based
 *                      on the last meassurement
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_socss(void)
{
    sta_setCubesatVar(sta_eps_socss, readEPSvars(EPS_ID_socss));
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_state_flag
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
void srp_eps_state_flag(void)
{
    sta_setCubesatVar(sta_eps_state_flag, readEPSvars(EPS_ID_state_flag));
}

/*------------------------------------------------------------------------------
 *                                  srp_eps_charging
 *------------------------------------------------------------------------------
 * Description        : Updates the battery "charging state"
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
void srp_eps_charging(void)
{
    int i_value = readEPSvars(EPS_ID_current_dir);
    i_value = i_value < 256 ? 1:0;
    sta_setCubesatVar(sta_eps_charging, i_value);
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
void srp_trx_frec_tx(void)
{
    /* Reading FTX */
    long long_value = -1;
//    long_value = (long)TRX_ReadRegister(TRX_FTX_H)<<16 ;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FTX_M)<<8;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FTX_L);
    /* Writing TRX_FTX to repo */
    sta_setCubesatVar(sta_trx_frec_tx, (int)long_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX FREC RX
 *------------------------------------------------------------------------------
 * Description        : Update TRX FRX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5012
 *----------------------------------------------------------------------------*/
void srp_trx_frec_rx(void)
{
    /* Reading FRX */
    long long_value = -1;
//    long_value = (long)TRX_ReadRegister(TRX_FRX_H)<<16 ;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FRX_M)<<8;
//    long_value = long_value | (long) TRX_ReadRegister(TRX_FRX_L);
    /* Writing TRX_FRX to repo */
    sta_setCubesatVar(sta_trx_frec_rx, (int)long_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX OPMODE
 *------------------------------------------------------------------------------
 * Description        : Update TRX Operation Mode status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5013
 *----------------------------------------------------------------------------*/
void srp_trx_opmode(void)
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
void srp_trx_temp_hpa(void)
{
    int int_value = -1;

    /*TODO: Reading TEMP_HPA */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPHPA_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPHPA_L);
    /* Writing TEMP_HPA to repo */
    sta_setCubesatVar(sta_trx_temp_hpa, int_value);

    
}

void srp_trx_temp_mcu(void)
{
    int int_value = -1;

    /* TODO: Reading TEMP_MCU */
//    int_value = (int)TRX_ReadRegister(TRX_TEMPMCU_H)<<8;
//    int_value = int_value | (int)TRX_ReadRegister(TRX_TEMPMCU_L);
    /* Writing TEMP_MCU to repo */
    sta_setCubesatVar(sta_trx_temp_mcu, int_value);

    
}


#define DRP_RSSI_LIST_LEN   20  /*!< Largo del buffer de datos para calcular rssi promedio */
#define DRP_RSSI_MEAN_INIT  99 /*|< Aca hay que tener cuidado de que (RSSI_MEAN_INIT)*(RSSI_LIST_LEN)
                            no porvoque overflow, por eso extendi el tipo de RSSI_LIST[] a long */

#define DRP_RSSI_CNT_MAX   600  /*!< Segundos que RSSI_CNT puede pasar sin que se actualice RSSI_MEAN */

/* Funcion auxiliar para mentener la media de rssi actualiada */
static int srp_update_rssi_mean(int new_value);

long DRP_RSSI_LIST[DRP_RSSI_LIST_LEN];
int srp_rssi_list_p = 0;
int DRP_RSSI_MEAN = DRP_RSSI_MEAN_INIT;

int srp_trx_rssi_mean(int arg){
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

    int res=srp_update_rssi_mean(arg);
    return res;
}

/**
 * Calcular el promedio del nivel de RSSI sobre la lista de datos disponiblea.
 * Retorna el valor actualizado al desechar mediciones antiguas
 *
 * @param new_value Nuevo valor para agregar a la lista y actualizar el promedio
 * @return Promedio del nivel de RSSI sobre los datos disponibles
 */
static int srp_update_rssi_mean(int new_value){
    srp_rssi_list_p = (srp_rssi_list_p < DRP_RSSI_LIST_LEN) ? srp_rssi_list_p : 0;
    DRP_RSSI_LIST[srp_rssi_list_p] = new_value;
    srp_rssi_list_p++; int res;

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
void srp_trx_rssi(void)
{
    uint8_t count;
    int result;
    int rssi_value;

    result = com_get_log_rssi(rssi_data, &count, NODE_COM, 1000);
    if(result)
    {
        rssi_value = rssi_data[count].rssi; //Get last RSSI measure
    }

    /* Writing RSSI to repo */
    sta_setCubesatVar(sta_trx_rssi, rssi_value);
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
void srp_trx_beacon_pwr(void)
{
    /* Reading BEACON_PWR */
    char char_value = 0;
//    char_value = TRX_ReadRegister(TRX_BEACONPOWER);
    /* Writing BEACON_PWR to repo */
    sta_setCubesatVar(sta_trx_beacon_pwr, (int)char_value);
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
void srp_trx_telemetry_pwr(void)
{
    char char_value = 0;
    /* Reading TELEMETRY_PWR */
//    char_value = TRX_ReadRegister(TRX_TMPOWER);
    /* Writing BEACON_PWR to repo */
    sta_setCubesatVar(sta_trx_telemetry_pwr, (int)char_value);
}

/*------------------------------------------------------------------------------
 *		 	DRP TRX STATUS TC
 *------------------------------------------------------------------------------
 * Description        : Update TRX FRX status in data repository
 * Arguments          : void
 * Return Value       : 1 - OK, 0 - FAIL
 * ID                 : 0x5018
 *----------------------------------------------------------------------------*/
void srp_trx_status_tc(void)
{
    int value = 0;
    /* TODO: Reading STATUS_TC */
//    value = TRX_CheckNewTC();
    /* Writing BEACON_PWR to repo */
    sta_setCubesatVar(sta_trx_status_tc, value);
}

/*------------------------------------------------------------------------------
 *                              DRP_DEP_WRITE_DEPLOYED
 *------------------------------------------------------------------------------
 * Description        : Updates the dep_ant_deployed flag in persistent memory
 * Arguments          : flag value
 * Return Value       : 1 - Success, 0 - Fail
 * ID                 : 0x5026
 *----------------------------------------------------------------------------*/
void srp_dep_write_deployed(int dep_state, int dep_tries)
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
    srp_memEEPROM_erase();

}

