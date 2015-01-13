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
#include "cmdPayload.h"

cmdFunction srpFunction[SRP_NCMD];
int srp_sysReq[SRP_NCMD];

void srp_onResetCmdSRP(){
    printf("        srp_onResetCmdSRP\n");

    int i;
    for(i=0; i<SRP_NCMD; i++) srp_sysReq[i] = CMD_SYSREQ_MIN;

    //De display
    srpFunction[(unsigned char)srp_id_print_STA_stateVar] = srp_print_STA_stateVar;
    //De update en general en sta_stateVar
    srpFunction[(unsigned char)srp_id_increment_STA_stateVar_hoursAlive] = srp_increment_STA_stateVar_hoursAlive;
    srpFunction[(unsigned char)srp_id_increment_STA_stateVar_hoursWithoutReset] = srp_increment_STA_stateVar_hoursWithoutReset;
    srpFunction[(unsigned char)srp_id_increment_STA_stateVar_nSended_tm] = srp_increment_STA_stateVar_nSended_tm;
    srpFunction[(unsigned char)srp_id_increment_STA_stateVar_nReceived_tc] = srp_increment_STA_stateVar_nReceived_tc;
    srpFunction[(unsigned char)srp_id_executeBeforeFlight] = srp_executeBeforeFlight;
    srpFunction[(unsigned char)srp_id_memEEPROM_initial_state] = srp_memEEPROM_initial_state;

}

//------------------------------------------------------------------------------

int srp_increment_STA_stateVar_hoursWithoutReset(void *param)
{
    /* En el futro esta funcion deberia ser llamada desde alguna interrupcion periodica del RTCC.
     * O leer del RTCC y comparar con su valor actual (get_ppc_hoursWithoutReset) y decidir
     * si debe incrementarlo y por cuanto */

    //int ar=*( (int *)param ); char ret[6];
    //con_printf("param= ");  Hex16ToAscii( ar, ret); con_printf(buffer); con_printf("\n");
    //solo debe ser llamada cada 1hora
    int arg = sta_get_stateVar(sta_ppc_hoursWithoutReset)+1;
    ppc_set_hoursWithoutReset(&arg);
    return 1;
}
int srp_increment_STA_stateVar_hoursAlive(void *param){
    //solo debe ser llamada cada 1hora
    int arg = sta_get_stateVar(sta_ppc_hoursAlive)+1;
    ppc_set_hoursAlive(&arg);
    return 1;
}
int srp_increment_STA_stateVar_nSended_tm(void *param){
    int arg = sta_get_stateVar(sta_trx_count_tm)+1;
    trx_set_count_tm(&arg);

    return 1;
}
int srp_increment_STA_stateVar_nReceived_tc(void *param){
    int arg = sta_get_stateVar(sta_trx_count_tc)+1;
    trx_set_count_tc(&arg);
    return 1;
}


int srp_executeBeforeFlight(void * param){
    printf("  srp_executeBeforeFlight()..\n");

    srp_memEEPROM_initial_state(NULL);

    return 1;
}
/**
 * Set every memEEPROM var to its initial value (to a Before Fligth condition)
 * @param param None
 * @return 1 = Ok, 0 = Fail
 */
int srp_memEEPROM_initial_state(void * param){
    int arg;

    //PPC => (C&DH subsystem)
    arg = STA_PPC_OPMODE_NORMAL;
    ppc_set_opMode(&arg);
    arg = 0;
    ppc_set_hoursAlive(&arg);
    arg = 0;
    ppc_set_hoursWithoutReset(&arg);
    arg = -1;   //always incremented onReset => to get 0 a -1 needs to be set
    ppc_set_resetCounter(&arg);

    //DEP => (C&DH subsystem)
    arg = -1;   //Reset Antenna DEP vars to a Before Fligth condition
    thk_deployment_registration(&arg);

    //Flight Plan
    arg = 0;
    drp_fpl_set_index(&arg);

    //PAYLOAD
    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_lagmuirProbe(&arg);
    #else
        sta_setstateVar(sta_pay_lagmuirarg = pay_xxx_state_inactive;
        pay_set_state_lagmuirProbe(&arg);
    #endif
    #if (SCH_PAY_SENSTEMP_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_sensTemp(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_sensTemp(&arg);
    #endif
    #if (SCH_PAY_GPS_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_gps(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_gps(&arg);
    #endif
    #if (SCH_PAY_FIS_ONBOARD==1)
        //arg = sta_pay_xxx_state_active;
        arg = pay_xxx_state_inactive;   //special case, not executed by FP2
        pay_set_state_expFis(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_expFis(&arg);
    #endif
    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
        //  arg = sta_pay_xxx_state_active;
        arg = pay_xxx_state_inactive;   //special case, not executed by FP2
        pay_set_state_camera(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_camera(&arg);
    #endif
    #if (SCH_PAY_GYRO_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_gyro(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_gyro(&arg);
    #endif
    #if (SCH_PAY_TMESTADO_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_tmEstado(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_tmEstado(&arg);
    #endif
    #if (SCH_PAY_BATTERY_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_battery(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_battery(&arg);
    #endif
    #if (SCH_PAY_DEBUG_ONBOARD==1)
        arg = pay_xxx_state_active;
        pay_set_state_debug(&arg);
    #else
        arg = pay_xxx_state_inactive;
        pay_set_state_debug(&arg);
    #endif

    //TRX
    arg = 0;
    trx_set_operation_mode(&arg);
    arg = 0;
    trx_set_count_tc(&arg);
    arg = 0;
    trx_set_count_tm(&arg);
    arg = 0;
    trx_set_day_last_tc(&arg);
    arg = SCH_TRX_BEACON_PERIOD;
    trx_set_beacon_period(&arg);
    arg = SCH_TRX_BEACON_BAT_LVL;
    trx_set_beacon_level(&arg);
    arg = SCH_TRX_RX_BAUD;
    trx_set_rx_baud(&arg);
    arg = SCH_TRX_TX_BAUD;
    trx_set_tx_baud(&arg);

    /* Save configuration to TRX, to make the available to the next TRX wakeup */
    arg = 0;    //not deployed
    trx_initialize(&arg);

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
int srp_print_STA_stateVar(void *param)
{
    printf("===============================\r\n");
    printf("StateVar content: \r\n");
    printf("===============================\r\n");

    STA_StateVar indxVar; int val;
    for(indxVar=0; indxVar<sta_stateVar_last_one; indxVar++){
        val = sta_get_stateVar(indxVar);
        printf("    * sta_get_stateVar(%s) = %d \r\n", sta_varToString(indxVar), val);
    }
    printf("===============================\r\n");

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
}

