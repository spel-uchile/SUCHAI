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
    printf("        drp_onResetCmdDRP\n");

    drpFunction[(unsigned char)drp_id_print_dat_TelecmdBuff] = drp_print_dat_TelecmdBuff;
    drp_sysReq[(unsigned char)drp_id_print_dat_TelecmdBuff]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_FlightPlan] = drp_print_dat_FlightPlan;
    drp_sysReq[(unsigned char)drp_id_print_dat_FlightPlan]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadIndxs] = drp_print_dat_PayloadIndxs;
    drp_sysReq[(unsigned char)drp_id_print_dat_PayloadIndxs]  = CMD_SYSREQ_MIN;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadVar] = drp_print_dat_PayloadVar;
    drp_sysReq[(unsigned char)drp_id_print_dat_PayloadVar]  = CMD_SYSREQ_MIN;

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

//******************************************************************************
int drp_print_dat_TelecmdBuff(void *param)
{
    char buffer[10];

    con_printf("===================================\r\n");
    con_printf("Reading sta_TelecmdBuff Buffer\r\n");
    con_printf("===================================\r\n");

    // Cmd buffer control
    itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newTcFrame), 10);
    con_printf("drp_trx_newTcFrame= "); con_printf(buffer); con_printf("\r\n");
    itoa(buffer,  (unsigned int)sta_getCubesatVar(sta_trx_newCmdBuff), 10);
    con_printf("drp_trx_newCmdBuff= "); con_printf(buffer); con_printf("\r\n");

    con_printf("Contenido del sta_telecmdBuff:\r\n");
    int i;
    for(i=0; i<DAT_MAX_BUFF_TELECMD; i++)
    {
        itoa(buffer,  (unsigned int)dat_get_TeleCmdBuff(i), 10);
        con_printf("sta_telecmdBuff["); itoa(buffer, (unsigned int)i, 10);
        con_printf(buffer); con_printf("]= "); con_printf(buffer); con_printf("\r\n");

    }
    return 1;
}

int drp_print_dat_FlightPlan(void *param){
    con_printf("===================================\r\n");
    con_printf("Reading sta_FligthPlan Buffer\r\n");
    con_printf("===================================\r\n");

    int i; char buffer[10];
    for(i=0; i<SCH_FLIGHTPLAN_N_CMD; i++)
    {
        DispCmd dcmd = dat_get_FlightPlan(i);
        con_printf("sta_FlightPlan[");
        itoa(buffer, (unsigned int)i, 10); con_printf(buffer); con_printf("]: cmdId=");
        itoa(buffer,  (unsigned int)dcmd.cmdId, 10); con_printf(buffer); con_printf(" param=");
        itoa(buffer,  (unsigned int)dcmd.param, 10); con_printf(buffer); con_printf("\r\n");
    }
    return 1;
}

int drp_print_dat_PayloadIndxs(void *param){
    con_printf("===================================\r\n");
    con_printf("Reading sta_PayloadIndxs Block\r\n");
    con_printf("===================================\r\n");

    char buffer[10];
    DAT_PayloadBuff pay_i;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++)
    {
        unsigned int max = dat_get_MaxPayIndx( pay_i);
        unsigned int next = dat_get_NextPayIndx( pay_i);
        
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
    con_printf("Reading sta_PayloadVar \r\n");
    con_printf("===================================\r\n");

    char buffer[10];
    DAT_PayloadBuff pay_i=pay_i2;
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

    unsigned int indx; unsigned int max = dat_get_MaxPayIndx(pay_i); int val;
    for(indx=0; indx<=max; indx++)
    {
        dat_get_PayloadBuff(pay_i, indx, &val);
        
        con_printf("sta_getPayloadVar[");
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
    sta_setCubesatVar(sta_fpl_index, index);

    return 1;
}

/*------------------------------------------------------------------------------
 *                              DRP_FLP_SET_CMD
 *------------------------------------------------------------------------------
 * @brief             : Agrega la id del comando dado al flight plan en la
 *                       posicion dado por el valor actual de sta_fpl_index
 * @return            : 1 - Success, 0 - Fail
 * ID                 : 0x5028
 *----------------------------------------------------------------------------*/
int drp_fpl_set_cmd(void *param)
{
    //Se recupera el comando y el indice
    int cmdid = *((int *)param);
    int index = sta_getCubesatVar(sta_fpl_index);

    //Se actualiza el flight plan
    int result = dat_set_FlightPlan_cmd(index, cmdid);

    return result;
}

/*------------------------------------------------------------------------------
 *                              DRP_FLP_SET_PARAM
 *------------------------------------------------------------------------------
 * @brief             : Actualiza en el flight plan el valor del parametro
 *                      asociado al indice que contiene actualmente la variable
 *                      sta_fpl_index.
 * @return            : 1 - Success, 0 - Fail
 * ID                 : 0x5029
 *----------------------------------------------------------------------------*/
int drp_fpl_set_param(void *param)
{
    //Se recupera el comando y el indice
    int var = *((int *)param);
    int index = sta_getCubesatVar(sta_fpl_index);

    //Se actualiza el flight plan
    int result = dat_set_FlightPlan_param(index, var);

    return result;
}

int drp_debug(void *param){
    int ind=*((int*)param);

    printf("calling drp_debug(%d)..\n", ind);

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
    int value=0, res=0;
    unsigned int index;
    unsigned long block;

    con_printf("(Destructive) Testing -256block r/w- memSD\r\n");
    for(block=0; block<1024; block=block+256){
        printf("testing 256Block = %lu\n", block);

        value=20000;
        for(index=0; index<=0xFFFF; index++, value++){

            con_printf("writing: ");
            msd_setVar_256BlockExtMem( block, index, value);
            printf("value[%u] = %d    |    ", index, value);

            printf("reading: ");
            msd_getVar_256BlockExtMem( block, index, &res);
            printf("value[%u] = %d    |    ", index, res);

            printf("comparing: ");
            if( value==res ){ printf("ok\n"); }
            else{ con_printf("fail\n"); return; }

            ClrWdt();
        }
    }
    //drp_memSD_BlockErase();
}
void drp_debug4(void){
    int value=0, res=0;
    unsigned int index;

    printf("DAT_TeleCmdBuff..\n");
    value=20000;
    for(index=0; index<SCH_DATAREPOSITORY_MAX_BUFF_TELECMD; index++, value++){

        printf("  writing: ");
        dat_set_TeleCmdBuff(index, value);
        printf("    DAT_TeleCmdBuff[%u] = %d    |    ", index, value);

        printf("  reading: ");
        res = dat_get_TeleCmdBuff(index);
        printf("    DAT_TeleCmdBuff[%u] = %d    |    ", index, res);

        printf("comparing: ");
        if( value==res ){ printf("ok\n"); }
        else{ con_printf("fail\n"); return; }

        ClrWdt();
    }

    printf("DAT_FlightPlanBuff..\n");
    value=1000; DispCmd NewCmd;
    for(index=0; index<(SCH_FLIGHTPLAN_N_CMD); index++, value++){

        printf("  writing: ");
        dat_set_FlightPlan_cmd(index, value);
        printf("    DAT_FlightPlanBuff[%u].cmd = %d    |    ", index, value);
        dat_set_FlightPlan_param(index, value+1);
        printf("    DAT_FlightPlanBuff[%u].param = %d    |    \n", index, value+1);
        

        printf("  reading: ");
        NewCmd = dat_get_FlightPlan(index);
        printf("    DAT_FlightPlanBuff[%u].cmd = %d    |    ", index, NewCmd.cmdId);
        printf("    DAT_FlightPlanBuff[%u].param = %d    |    \n", index, NewCmd.param);

        printf("  comparing: ");
        if( value==NewCmd.cmdId && (value+1)==NewCmd.param ){ printf("ok\n"); }
        else{ con_printf("fail\n"); return; }

        ClrWdt();
    }
    
    printf("DAT_NextPayBuff and DAT_MaxPayBuff..\n");
    int pay_i; int n=0, m=49, res1, res2;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++, m=m+10){
        printf("  writing: ");
        dat_set_NextPayIndx(pay_i, n);
        printf("    DAT_NextPayBuff[%u] = %d    |    ", pay_i, n);
        dat_set_MaxPayIndx(pay_i, m);
        printf("    DAT_NextPayBuff[%u] = %d    |    \n", pay_i, m);

        printf("  reading: ");
        res1 = dat_get_NextPayIndx(pay_i);
        printf("    DAT_NextPayBuff[%u] = %d    |    ", pay_i, res1);
        res2 = dat_get_MaxPayIndx(pay_i);
        printf("    DAT_MaxPayBuff[%u] = %d    |    \n", pay_i, res2);

        printf("  comparing: ");
        if( n==res1 && m==res2 ){ printf("ok\n"); }
        else{ printf("fail\n"); return; }

        ClrWdt();
    }

    printf("DAT_PayloadBuff..\n");
    int maxind;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++){
        value=20000; 
        maxind = dat_get_MaxPayIndx(pay_i);
        for(index=0; index<=maxind; index++, value++){

            printf("  writing: ");
            dat_set_PayloadBuff(pay_i, value);
            printf("    DAT_PayloadBuff[%u][%u] = %d    |    ",pay_i, index, value);
            printf("%d/%d [NextIndx/MaxIndx]   |    \n", dat_get_NextPayIndx(pay_i), dat_get_MaxPayIndx(pay_i) );

            printf("  reading: ");
            dat_get_PayloadBuff(pay_i, index, &res);
            printf("    DAT_PayloadBuff[%u][%u] = %d    |    \n",pay_i,  index, res);

            printf("  comparing: ");
            if( value==res ){ printf("ok\n"); }
            else{ con_printf("fail\n"); return; }

            ClrWdt();
        }
        printf("*******************************************\n");
    }

    printf("DAT_AuxBuff..\n");
    int aux_i; int maxind2;
    for(aux_i=0; aux_i<dat_aux_last_one; aux_i++){
        value=20000;
        maxind2 = 30;
        for(index=0; index<=maxind2; index++, value++){

            printf("  writing: ");
            dat_set_AuxBuff(aux_i, index, value);
            printf("    DAT_AuxdBuff[%u] = %d    |    ", index, value);

            printf("  reading: ");
            res = dat_get_AuxBuff(aux_i, index);
            printf("    DAT_AuxdBuff[%u] = %d    |    ", index, res);

            printf("  comparing: ");
            if( value==res ){ printf("ok\n"); }
            else{ con_printf("fail\n"); return; }

            ClrWdt();
        }
        printf("*******************************************\n");
    }
    printf("End of drp_debug4\n");
}

int drp_executeBeforeFlight(void *param){
    drp_dat_FlightPlan_EBF();
    drp_dat_TelecmdBuffer_EBF();

    int mode=*((int *)param);
    if(mode==1){return 1;}

    con_printf("****************************************************\r\n");
    con_printf("drp_executeBeforeFlight finalizo\r\n");
    con_printf("Para quedar en config de vuelo, se\r\n");
    con_printf("DEBE apagar el SUCHAI, hagalo ANTES de:\r\n");
    con_printf("****************************************************\r\n");
    
    int i;
    for(i=20;i>=1;i--){
        __delay_ms(1000);
        printf("%d segundos..\r\n", i);
    }

    return 1;
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
            //sta_setFlightPlan_cmd(i, 0x1002);
            //sta_setFlightPlan_param(i, 0x0000);
        }
    }
}

void drp_dat_TelecmdBuffer_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        con_printf("Setting TelecmdBuffer in launch configuration..\n");
    #endif

    dat_erase_TeleCmdBuff();
}

