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

    int i;
    for(i=0; i<DRP_NCMD; i++) drp_sysReq[i] = CMD_SYSREQ_MIN;

    drpFunction[(unsigned char)drp_id_print_dat_FlightPlan] = drp_print_dat_FlightPlan;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadIndxs] = drp_print_dat_PayloadIndxs;
    drpFunction[(unsigned char)drp_id_print_dat_PayloadVar] = drp_print_dat_PayloadVar;
    //De mem ext (EEPROM y SD)
    drpFunction[(unsigned char)drp_id_executeBeforeFlight] = drp_executeBeforeFlight;
    //de debug
    drpFunction[(unsigned char)drp_id_debug] = drp_debug;
    //Flight Plan
    drpFunction[(unsigned char)drp_id_fpl_get_index] = drp_fpl_get_index;
    drpFunction[(unsigned char)drp_id_fpl_set_index] = drp_fpl_set_index;
    drpFunction[(unsigned char)drp_id_fpl_set_cmd] = drp_fpl_set_cmd;
    drpFunction[(unsigned char)drp_id_fpl_set_param] = drp_fpl_set_param;
    drpFunction[(unsigned char)drp_id_fpl_check_and_exec] = drp_fpl_check_and_exec;
}

int drp_print_dat_FlightPlan(void *param){
    con_printf("===================================\r\n");
    con_printf("Reading sta_FligthPlan Buffer\r\n");
    con_printf("===================================\r\n");

    int i;
//    char buffer[10];
    for(i=0; i<SCH_FLIGHTPLAN_N_CMD; i++)
    {
        DispCmd dcmd = dat_get_FlightPlan(i);
        printf("sta_FlightPlan[%d]: cmdId = 0x%X, param = %d", i , dcmd.cmdId, dcmd.param);
//        itoa(buffer, (unsigned int)i, 10); con_printf(buffer); con_printf("]: cmdId=");
//        itoa(buffer,  (unsigned int)dcmd.cmdId, 10); con_printf(buffer); con_printf(" param=");
//        itoa(buffer,  (unsigned int)dcmd.param, 10); con_printf(buffer); con_printf("\r\n");
    }
    return 1;
}

int drp_print_dat_PayloadIndxs(void *param){
    printf("===================================\r\n");
    printf("Reading sta_PayloadIndxs Block\r\n");
    printf("===================================\r\n");

//    char buffer[10];
    DAT_Payload_Buff pay_i;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++)
    {
        unsigned int max = dat_get_MaxPayIndx( pay_i);
        unsigned int next = dat_get_NextPayIndx( pay_i);
        
        printf("pay_i = %u | maxIndx = %d | next = %d \r\n", (unsigned int)pay_i, max, next);
//        itoa(buffer, (unsigned int)pay_i,10);
//        con_printf(buffer); con_printf("\r\n");
//        con_printf("    MaxIndx=");
//        itoa(buffer, (unsigned int)max,10);
//        sprintf (buffer, "0x%X", (unsigned int)max);
//        con_printf(buffer); con_printf("\r\n");
//        con_printf("    NextIndx=");
//        //itoa(buffer, (unsigned int)next,10);
//        sprintf (buffer, "0x%X", (unsigned int)next);
//        con_printf(buffer); con_printf("\r\n");
    }
    return 1;
}

int drp_print_dat_PayloadVar(void *param){

    int pay_i2 = *((int *)param);
    
    con_printf("===================================\r\n");
    con_printf("Reading sta_PayloadVar \r\n");
    con_printf("===================================\r\n");

    char buffer[10];
    DAT_Payload_Buff pay_i=pay_i2;
    if(pay_i>=dat_pay_last_one){
        printf("pay_i= %d NO existe, nada que imprimir\r\n", pay_i);
//        itoa(buffer, (unsigned int)pay_i,10);
//        con_printf(buffer);
//        con_printf(" NO existe. Nada que imprimir\r\n");
        return 0;
    }

    printf("pay_i = %d\r\n", pay_i);
//    itoa(buffer, (unsigned int)pay_i,10);
//    con_printf(buffer); con_printf("\r\n");

    unsigned int indx; unsigned int max = dat_get_MaxPayIndx(pay_i); int val;
    for(indx=0; indx<=max; indx++)
    {
        dat_get_Payload_Buff(pay_i, indx, &val);
        
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

int drp_fpl_get_index(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_fpl_index;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
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
    MemEEPROM_Vars mem_eeprom_var = mem_fpl_index;
    int value = *((int*)param);
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;   //se asume operacion exitosa
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
    int index = sta_get_stateVar(sta_fpl_index);

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
    int index = sta_get_stateVar(sta_fpl_index);

    //Se actualiza el flight plan
    int result = dat_set_FlightPlan_param(index, var);

    return result;
}

int drp_fpl_check_and_exec(void *param){
    //FP, programmed actions
    unsigned int index, current_hour, current_mins;
    static unsigned int last_index; //to enter the first try, then check every time

    DispCmd NewCmd;
    NewCmd.idOrig = 0;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    ExeCmd exeCmd; /* The cmd to executer */
    int cmdParam;
    int cmdResult;

    /* Map hh:mm to MM minutues of the day to obtain the
     * index of the next command to read from fligh plan */
    current_hour = sta_get_stateVar(sta_rtc_hours);
    current_mins = sta_get_stateVar(sta_rtc_minutes);
    index = current_hour*60 + current_mins;
    index = index / SCH_FLIGHTPLAN_RESOLUTION;
    
    #if SCH_TASKFLIGHTPLAN_VERBOSE
        /* Debug info */
        printf("  [drp_fpl_check_and_exec] index = %d = (%d*60+%d)/SCH_FP_RESOLUTION \n  last_index=%d | SCH_FP_N_CMD=%d\r\n",
                index, current_hour, current_mins, last_index, SCH_FLIGHTPLAN_N_CMD);
    #endif

    /* If check time is less than flight plan resolution (as it should be)
     *  we need to prevent an index repetition */
    if(last_index != index)
    {
        /* Update last_index */
        last_index = index;

        /* Get the next command from flight plan */
        NewCmd = dat_get_FlightPlan(index); //get cmdId and param
        //NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN3;

        /* Check if valid cmd */
        if(NewCmd.cmdId == CMD_CMDNULL){
            #if SCH_TASKFLIGHTPLAN_VERBOSE
                printf("    [drp_fpl_check_and_exec] Se extrae CMD_CMDNULL, se omite\r\n");
            #endif
            return 0;
        }

        #if SCH_TASKFLIGHTPLAN_VERBOSE
            /* Print the command code */
            printf("    [drp_fpl_check_and_exec] Se extrae cmdId = 0x%X, param = %d, ejecutando .. \r\n",
                    (unsigned int)NewCmd.cmdId, (unsigned int)NewCmd.param);
        #endif

        /* Queue NewCmd - Blocking */
        //xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);

        /* Check if command is executable */
        //not performed

        /* Fill the executer command */
        exeCmd.fnct = repo_getFunction(NewCmd.cmdId);
        exeCmd.param = NewCmd.param;

        /* Execute the command */
        cmdParam = exeCmd.param;
        cmdResult = exeCmd.fnct((void *)&cmdParam);
    }
    else{
        #if(SCH_TASKFLIGHTPLAN_VERBOSE >= 1)
            /* Print the command code */
            printf("    [drp_fpl_check_and_exec] (last_index==index) => NO se genera comando\r\n");
        #endif
        return 1;
    }

    return cmdResult;
}

int drp_debug(void *param){
    int ind=*((int*)param);

    printf("drp_debug(%d)..\n", ind);

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
        case 5:
            drp_debug5();
            break;
        default:
            //
            break;
    }

    return 1;
}
void drp_debug1(void){
//    int i;
//    con_printf("Testing rand()\r\n");
//    for(i=0;i<0xFFFF;i++){
//        unsigned char c= rand();
//        con_printf("rand()=");
//        char buffer[6];
//        itoa(buffer,  (unsigned int)c, 10);
//        con_printf(buffer); con_printf("\r\n");
//        ClrWdt();
//    }

}

void drp_debug2(void){
//    char buffer[10];
    int value=0, res=0;
    unsigned char index;
    long block;

    con_printf("(Destructive) Testing -1block r/w- memSD\r\n");
    for(block=0;block<1024;block++){
        printf("testing block = %lu \r\n",block);

        value=0xA000;
        for(index=0;index<=0xFF;index++, value++){

            con_printf("writing: ");
            msd_setVar_1BlockExtMem( block, index, value);
            printf("value[%d] = 0x%X | ", index, value);
//            itoa(buffer, index,10); con_printf(buffer); con_printf("]=");
//            itoa(buffer, value,10); con_printf(buffer); con_printf("    |    ");

            con_printf("reading: ");
            msd_getVar_1BlockExtMem( block, index, &res);
            printf("value[%d] = 0x%X | ", index, res);
//            itoa(buffer, index,10); con_printf("value["); con_printf(buffer); con_printf("]=");
//            itoa(buffer, res,10); con_printf(buffer); con_printf("    |    ");

            printf("comparing: ");
            if(value==res){ printf("ok"); }
            else{ printf("fail"); }
            printf("\n");

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
        dat_set_TeleCmd_Buff(index, value);
        printf("    DAT_TeleCmdBuff[%u] = %d    |    ", index, value);

        printf("  reading: ");
        res = dat_get_TeleCmd_Buff(index);
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

    printf("DAT_Payload_Buff..\n");
    int maxind;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++){
        value=20000; 
        maxind = dat_get_MaxPayIndx(pay_i);
        for(index=0; index<=maxind; index++, value++){

            printf("  writing: ");
            dat_set_Payload_Buff(pay_i, value, DAT_PAYBUFF_MODE_NO_MAXINDX);
            printf("    DAT_Payload_Buff[%u][%u] = %d    |    ",pay_i, index, value);
            printf("%d/%d [NextIndx/MaxIndx]   |    \n", dat_get_NextPayIndx(pay_i), dat_get_MaxPayIndx(pay_i) );

            printf("  reading: ");
            dat_get_Payload_Buff(pay_i, index, &res);
            printf("    DAT_Payload_Buff[%u][%u] = %d    |    \n",pay_i,  index, res);

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
            dat_set_Aux_Buff(aux_i, index, value);
            printf("    DAT_AuxdBuff[%u] = %d    |    ", index, value);

            printf("  reading: ");
            res = dat_get_Aux_Buff(aux_i, index);
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
void drp_debug5(void){
    int value=0, res=0;
    unsigned int index;

    printf("dat_reset_Payload_Buff()..\n");
    int pay_i; int lenBuff=10, r_nextIndx, r_MaxIndx;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++, lenBuff=lenBuff+1){
        printf("  writing: ");
        dat_reset_Payload_Buff(pay_i, lenBuff, 0);
        printf("    dat_reset_Payload_Buff(%d, %d)\r\n", pay_i, lenBuff);

        printf("  reading: ");
        r_nextIndx = dat_get_NextPayIndx(pay_i);
        printf("    dat_get_NextPayIndx(%u) = %d    |    ", pay_i, r_nextIndx);
        r_MaxIndx = dat_get_MaxPayIndx(pay_i);
        printf("    dat_get_MaxPayIndx(%u) = %d    |    \n", pay_i, r_MaxIndx);

        printf("  comparing: ");
        if( r_nextIndx==0 && r_MaxIndx==(lenBuff-1) ){ printf("ok\n"); }
        else{ printf("fail\n"); return; }

        ClrWdt();
    }

    printf("DAT_Payload_Buff..\n");
    int maxind; BOOL st;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++){
        value=20000;
        st = TRUE;
        maxind = dat_get_MaxPayIndx(pay_i);
        for(index=0; st==TRUE; index++, value++){

            printf("  writing: ");
            dat_set_Payload_Buff(pay_i, value, DAT_PAYBUFF_MODE_NO_MAXINDX);
            printf("    DAT_Payload_Buff[%u][%u] = %d    |    ",pay_i, index, value);
            printf("%d/%d [NextIndx/MaxIndx]   |    \n", dat_get_NextPayIndx(pay_i), dat_get_MaxPayIndx(pay_i) );

            printf("  reading: ");
            dat_get_Payload_Buff(pay_i, index, &res);
            printf("    DAT_Payload_Buff[%u][%u] = %d    |    \n",pay_i,  index, res);

            printf("  comparing: ");
            if( value==res ){ printf("ok\n"); }
            else{ printf("fail\n"); return; }

            if( dat_isFull_Payload_Buff(pay_i)==TRUE){
                printf("    DAT_Payload_Buff[%u] esta lleno\r\n", pay_i);
                st = FALSE;
            }

            ClrWdt();
        }
        printf("*******************************************\n");
    }
    printf("End of drp_debug5\n");
}

int drp_executeBeforeFlight(void *param){
    #if (SCH_CMDDRP_VERBOSE>=1)
        printf("  drp_executeBeforeFlight()..\n");
    #endif

    drp_DAT_FlightPlan_EBF();
    //drp_DAT_Payload_Buff_EBF();

    return 1;
}
void drp_DAT_Payload_Buff_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        printf("    Setting Payload_Buff in launch configuration..\n");
    #endif
    //nothing to do..
}
void drp_DAT_FlightPlan_EBF(void){
    #if (SCH_CMDDRP_VERBOSE>=1)
        printf("    Setting FligthPlan in launch configuration..\n");
    #endif
    
    #if (SCH_CMDDRP_VERBOSE>=1)
        printf("    Setting initial commands in FligthPlan..\n");
    #endif

    //aca debe ir la configuracion inicial del FligthPlan
    //esta es la configuracion a cargar antes del lanzamiento
    int index;
    for(index=0; index<SCH_FLIGHTPLAN_N_CMD; index++){
        if(index%2==0){
            dat_set_FlightPlan_cmd(index, ppc_id_get_osc);
            dat_set_FlightPlan_param(index, index);
        }
        else{
            dat_set_FlightPlan_cmd(index, rtc_id_print);
            dat_set_FlightPlan_param(index, index);
        }
    }
}
