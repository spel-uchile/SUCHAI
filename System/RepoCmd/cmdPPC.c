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

#include "cmdPPC.h"

//Para ppc_rtos_debug
extern xTaskHandle taskDeploymentHandle;
extern xTaskHandle taskDispatcherHandle;
extern xTaskHandle taskExecuterHandle;
extern xTaskHandle taskComunicationsHandle;
extern xTaskHandle taskConsoleHandle;
extern xTaskHandle taskFlightPlanHandle;
extern xTaskHandle taskFlightPlan2Handle;
extern xTaskHandle taskHouskeepingHandle;

cmdFunction ppcFunction[PPC_NCMD];
int ppc_sysReq[PPC_NCMD];

void ppc_onResetCmdPPC(void){
    printf("        ppc_onResetCmdPPC\n");

    int i;
    for(i=0; i<PPC_NCMD; i++) ppc_sysReq[i] = CMD_SYSREQ_MIN;

    ppcFunction[(unsigned char)ppc_id_reset] = ppc_reset;
    ppcFunction[(unsigned char)ppc_id_get_lastResetSource] = ppc_get_lastResetSource;
    
    ppcFunction[(unsigned char)ppc_id_set_opMode] = ppc_set_opMode;
    ppcFunction[(unsigned char)ppc_id_get_opMode] = ppc_get_opMode;
    ppcFunction[(unsigned char)ppc_id_set_resetCounter] = ppc_set_resetCounter;
    ppcFunction[(unsigned char)ppc_id_get_hoursAlive] = ppc_get_hoursAlive;
    ppcFunction[(unsigned char)ppc_id_set_hoursWithoutReset] = ppc_set_hoursWithoutReset;
    ppcFunction[(unsigned char)ppc_id_get_resetCounter] = ppc_get_resetCounter;
    
    ppcFunction[(unsigned char)ppc_id_newosc] = ppc_newosc;
    ppcFunction[(unsigned char)ppc_id_get_osc] = ppc_get_osc;
    ppcFunction[(unsigned char)ppc_id_set_wdt_state] = ppc_set_wdt_state;
    ppcFunction[(unsigned char)ppc_id_get_wdt_state] = ppc_get_wdt_state;
    ppcFunction[(unsigned char)ppc_id_reactToSOC] = ppc_reactToSOC;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nOE_USB_nINT] = ppc_set_PPC_MB_nOE_USB_nINT;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nOE_MHX] = ppc_set_PPC_MB_nOE_MHX;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nON_MHX] = ppc_set_PPC_MB_nON_MHX;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nON_SD] = ppc_set_PPC_MB_nON_SD;
    ppcFunction[(unsigned char)ppc_id_get_PPC_MB_nOE_USB_nINT] = ppc_get_PPC_MB_nOE_USB_nINT;
    ppcFunction[(unsigned char)ppc_id_get_PPC_MB_nOE_MHX] = ppc_get_PPC_MB_nOE_MHX;
    ppcFunction[(unsigned char)ppc_id_get_PPC_MB_nON_MHX] = ppc_get_PPC_MB_nON_MHX;
    ppcFunction[(unsigned char)ppc_id_get_PPC_MB_nON_SD] = ppc_get_PPC_MB_nON_SD;
    ppcFunction[(unsigned char)ppc_id_rtos_debug] = ppc_rtos_debug;
    ppcFunction[(unsigned char)ppc_id_frozen] = ppc_frozen;
}


int ppc_get_opMode(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_opMode;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int ppc_set_opMode(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_opMode;
    int value = *((int*)param);
    mem_setVar(mem_eeprom_var, value);
    return 1;   //se asume operacion exitosa
}

/**
 *
 * @return last Reset Source
 */
int ppc_get_lastResetSource(void* param){
    int verb = *((int*)param);
    static char first_time;
    static int lreset;
    if(first_time==0){
        lreset = PwrMgnt_ResetSource();
        first_time = 1; //enter only once
    }

    if(verb == 1){
        printf("        * ppc_get_lastResetSource: ");
        switch ( lreset )
        {
            case POWER_ON_Reset:
                printf("POWER_ON_Reset\r\n");      /* 0x00 Aun nose cuando salta */
                mPWRMGNT_Clear_PORbit();
            break;
            case BURN_OUT_Reset:
                printf("BURN_OUT_Reset\r\n");      /* 0x01 ver nota mas arriba*/
                mPWRMGNT_Clear_BORbit();
            break;
            case WATCHDOG_Reset:
                printf("WATCHDOG_Reset\r\n");      /* 0x02 al overflow del WDT, luego de aprox 2 min sin ejecutarse ClrWdt(); */
                mPWRMGNT_Clear_WDTObit();
            break;
            case SOFTWARE_Reset:
                printf("SOFTWARE_Reset\r\n");      /* 0x03 ocurre luego de ejecutar ppc_reset() */
                mPWRMGNT_Clear_SWRbit();
            break;
            case EXTERNAL_Reset:
                printf("EXTERNAL_Reset\r\n");      /* 0x04 Ocurre cuando se programa y cuando se energiza/desenergiza el PIC*/
                mPWRMGNT_Clear_EXTRbit();
            break;
            case CFG_WORD_MISMATCH_Reset:
                printf("CFG_WORD_MISMATCH_Reset\r\n");
                mPWRMGNT_Clear_CMbit();
            break;
            case ILLEGAL_INSTR_Reset:
                printf("ILLEGAL_INSTR_Reset\r\n");
                mPWRMGNT_Clear_IOPUWRbit();
            break;
            case TRAP_Reset:
                printf("TRAP_Reset\r\n");
                mPWRMGNT_Clear_TRAPRbit();
            break;
            default:
                printf("No new RESET\r\n");
                lreset= -1;
                /* ver nota mas arriba */
            break;
        }
    }

    return lreset;
}
int ppc_get_hoursAlive(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_hoursAlive;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int ppc_set_hoursAlive(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_hoursAlive;
    int value = *((int*)param);
    mem_setVar(mem_eeprom_var, value);
    return 1;   //se asume operacion exitosa
}

int ppc_get_resetCounter(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_resetCounter;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int ppc_set_resetCounter(void* param){
    MemEEPROM_Vars mem_eeprom_var = mem_ppc_resetCounter;
    int value = *((int*)param);
    mem_setVar(mem_eeprom_var, value);
    return 1;   //se asume operacion exitosa
}

/**
 * Static varible is always initialized to Zero => 0 hours without reset.
 * Wich is rigth on every onReset
 */
static int ppc_hoursWithoutReset = 0;
/**
 * get hours without reset
 * @param param ignored
 * @return hours without reset
 */
int ppc_get_hoursWithoutReset(void* param){
    return ppc_hoursWithoutReset;
}
int ppc_set_hoursWithoutReset(void* param){
    int value = *((int*)param);
    ppc_hoursWithoutReset = value;
    return 1;   //se asume operacion exitosa
}

/**
 * Selects a new oscilator source
 * @param osc Number of the oscilator source
 * 0 FRC_OSC
 * 1 FRC_OSC_WITH_POSTSCALER_PLL
 * 2 PRIMARY_OSC
 * 3 PRIMARY_OSC_WITH_PLL
 * 4 SECONDRY_OSC
 * 5 LOW_POWER_RC
 * 6 FRC_OSC_WITH_POSTSCALER
 * 
 * @return 1 Ok, 0 Fail
 */
int ppc_newosc(void * osc)
{
    int i = *(int *)osc;
    set_new_osc(i);

    return 1;
}


static int ppc_wdt_state = PPC_INITIAL_WDT_STATE;
/*Se usa variable estatica ya que no existe una funcion que permita leer
 * el estado actual del WDT*/

/**
 * Enable/disable WDT
 * @param on_off
 * @return 1 = Ok , 0 = fail
 */
int ppc_set_wdt_state(void *on_off)
{
    int result = 1;
    int i = *(int *)on_off;

    if( i==0 )
    {
         EnableWDT( WDT_DISABLE );
         ppc_wdt_state = 0;
         printf("WDT was disabled\r\n");
    }
    else if( i==1 )
    {
         EnableWDT( WDT_ENABLE );
         ppc_wdt_state = 1;
         printf("WDT was enabled\r\n");
    }
    else
    {
        printf("Invalid argument\r\n");
        result = 0;
    }

    return result;
}
int ppc_get_wdt_state(void* param){
    return ppc_wdt_state;
}
/*------------------------------------------------------------------------------
 *                                  PPC OSC
 *------------------------------------------------------------------------------
 * Description        : Returns current oscillator + 1
 * Arguments          : None
 * Return Value       : int: 1=FRC_OSC .. 8=FRC_OSC_WITH_POSTSCALER, 0ther=Failure
 *----------------------------------------------------------------------------*/
int ppc_get_osc(void *param)
{
    int verbose = *(int *)param;
    char * ps;

    unsigned int osc = Current_OSCILLATOR();

    switch( osc )
    {
        case 0:
            ps = "Oscillator is FRC_OSC\r\n";
        break;
        case 1:
            ps = "Oscillator is FRC_OSC_WITH_POSTSCALER_PLL\r\n";
        break;
        case 2:
            ps = "Oscillator is PRIMARY_OSC\r\n";
        break;
        case 3:
            ps = "Oscillator is PRIMARY_OSC_WITH_PLL\r\n";
        break;
        case 4:
            ps = "Oscillator is SECONDARY_OSC\r\n";
        break;
        case 5:
            ps = "Oscillator is LOW_POWER_RC\r\n";
        break;
        case 7:
            ps = "Oscillator is FRC_OSC_WITH_POSTSCALER\r\n";
        break;
        default:
            ps = "Oscillator is default\r\n";
        break;
    }
    if(verbose==1){
        printf("        * ppc_get_osc: ");
        printf(ps);
    }

    return (int)(osc+1);
}

/*------------------------------------------------------------------------------
 *                                  PPC RESET
 *------------------------------------------------------------------------------
 * Description        : Software resets the PIC
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
int ppc_reset(void* param)
{
    int arg = *((int *)param);
    sw_reset(arg);
    
    return 1;
}

/*------------------------------------------------------------------------------
 *                                  PPC 
 *------------------------------------------------------------------------------
 * Description        : 
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
static int prev_soc=-1;
int ppc_reactToSOC(void* param)
{
    #if (SCH_CMDPPC_VERBOSE>=1)
        printf("ppc_reactToSOC()\r\n");
    #endif
         
    #if( 0 ) // Deprecated (SCH_EPS_ONBOARD == 1 )
        int current_soc;
        current_soc = sta_get_BusStateVar(sta_eps_soc);
    #else
        int current_soc;
        current_soc = 0;
    #endif
    

    if(prev_soc == current_soc){
        //nothing new to do
        return 1;
    }

    switch(current_soc){
        case (CMD_SYSREQ_MIN+0):
            //turn off GPS
            PPC_GPS_SWITCH = 0;
            //set only beacon (no TM)
            break;
        case (CMD_SYSREQ_MIN+1):
            //turn off GPS
            PPC_GPS_SWITCH = 0;
            //set only beacon (no TM)
            break;
        case (CMD_SYSREQ_MIN+2):
            //turn off GPS
            PPC_GPS_SWITCH = 0;
            //set only beacon (no TM)
            break;
        case (CMD_SYSREQ_MIN+3):
            //allow TM
            break;
        case (CMD_SYSREQ_MIN+4):
            //allow TM
            break;
        case (CMD_SYSREQ_MIN+5):
            //allow TM
            break;
        case (CMD_SYSREQ_MIN+6):
            //allow TM
            break;
        case (CMD_SYSREQ_MIN+7):
            //allow TM
            break;
        case CMD_SYSREQ_MAX:
            //allow TM
            break;
        default:
            break;
    }

//    //Legacy code
//    //Check_FSCM();
//    //Check_Reset_Status();
//    //Check_Wakeup_Status();
//
//    // Tomas Opazo: Esto aun no esta probado, por lo que no se usara para las pruebas de SUCHAIv1.
//    // De todos modos con no ejecutar el comando a nivel de taskDispather es suficiente, por lo que
//    //probablemente este comando quede sin ejecutar linea alguna
//
//    if( current_soc < (CMD_SYSREQ_MIN+1) )
//    {
//        con_printf("get_eps_soc()<(CMD_SYSREQ_MIN+1) Nivel critico, TRX debe silenciarse\r\n");
//        #if (SCH_TRX_TYPE_ONBOARD==1)
//            trx_mode=2; trx_setmode( (void *)&trx_mode );   //silent
//        #endif
//    }
//    else
//    {
//        if( (prev_soc!=0) && (prev_soc<current_soc) ){
//            con_printf("get_eps_soc()>=(CMD_SYSREQ_MIN+1) Nivel critico reestablecido, TRX vuevle a nominal\r\n");
//        }
//        #if (SCH_TRX_TYPE_ONBOARD==1)
//            trx_mode=5; trx_setmode( (void *)&trx_mode );   //nominal
//        #endif
//    }

    prev_soc = current_soc;
    return 1;
}

/**
 *
 * @param param
 * @return
 */
int ppc_set_PPC_MB_nOE_USB_nINT(void* param)
{
    int a = *((int *)param); char r;
    if(a){
        r=1;
    }
    else{
        r=0;
    }

    PPC_MB_nOE_USB_nINT = r;

//    char ascii_val[10];
//    //utoa(ascii_val, r, 16);
//    sprintf (ascii_val, "0x%X", r);
    printf("ppc_set_PPC_MB_nOE_USB_nINT = %d \r\n", r); //con_printf(ascii_val); printf("\r\n");

    return 1;
}

/**
 *
 * @param param
 * @return
 */
int ppc_set_PPC_MB_nOE_MHX(void* param)
{
    int a = *((int *)param); char r;
    if(a){
        r=1;
    }
    else{
        r=0;
    }

    PPC_MB_nOE_MHX = r;

    printf("ppc_set_PPC_MB_nOE_MHX = %d \r\n", r);
    //char ascii_val[10];
    //itoa(ascii_val, r, 16);
    //sprintf (ascii_val, "0x%X", r);
    //con_printf("ppc_set_PPC_MB_nOE_MHX= "); con_printf(ascii_val); con_printf("\r\n");

    return 1;
}

/**
 *
 * @param param
 * @return
 */
int ppc_set_PPC_MB_nON_MHX(void* param)
{
    int a = *((int *)param); char r;
    if(a){
        r=1;
    }
    else{
        r=0;
    }

    PPC_MB_nON_MHX = r;

    printf("ppc_set_PPC_MB_nON_MHX = %d \r\n", r);
//    char ascii_val[10];
//    //itoa(ascii_val, r, 16);
//    sprintf (ascii_val, "0x%X", r);
//    con_printf("ppc_set_PPC_MB_nON_MHX= "); con_printf(ascii_val); con_printf("\r\n");

    return 1;
}

/**
 *
 * @param param
 * @return
 */
int ppc_set_PPC_MB_nON_SD(void* param)
{
    int a = *((int *)param); char r;
    if(a){
        r=1;
    }
    else{
        r=0;
    }

    PPC_MB_nON_SD = r;

    printf("ppc_set_PPC_MB_nON_SD = %d \r\n", r);
//    char ascii_val[4];
//    //itoa(ascii_val,r,16);
//    sprintf (ascii_val, "0x%X", r);
//    printf("ppc_set_PPC_MB_nON_SD= "); con_printf(ascii_val); printf("\r\n");

    return 1;
}
int ppc_get_PPC_MB_nOE_USB_nINT(void* param){
    return PPC_MB_nOE_USB_nINT_CHECK;
}
int ppc_get_PPC_MB_nOE_MHX(void* param){
    return PPC_MB_nOE_MHX_CHECK;
}
int ppc_get_PPC_MB_nON_MHX(void* param){
    return PPC_MB_nON_MHX_CHECK;
}
int ppc_get_PPC_MB_nON_SD(void* param){
    return PPC_MB_nON_SD_CHECK;
}


/**
 * Performs debug taks over current RTOS. Print task and rtos memory usage in
 * order to detect memory fails like stak overflow.
 * 
 * @param param Not used
 * @return 1-succes, 0-fails
 */
int ppc_rtos_debug(void *param)
{
//    char buff[10];
//    unsigned int water_mark = 0;
//
//    con_printf("Task, Stak Highwatermark\n");
//
//    /* cheking taskDispatcher stack */
//    water_mark = uxTaskGetStackHighWaterMark(&taskDispatcherHandle);
//    utoa(buff,water_mark,10);
//    con_printf("taskDispatcher, "); con_printf(buff); con_printf("\n");
//
//    /* cheking taskExecuter stack */
//    water_mark = uxTaskGetStackHighWaterMark(&taskExecuterHandle);
//    utoa(buff,water_mark,10);
//    con_printf("taskExecuter, "); con_printf(buff); con_printf("\n");
//
//    /* cheking taskConsole stack */
//    water_mark = uxTaskGetStackHighWaterMark(&taskConsoleHandle);
//    utoa(buff,water_mark,10);
//    con_printf("taskConsole,"); con_printf(buff); con_printf("\n");
//
//    /* cheking taskHousekeeping stack */
//    water_mark = uxTaskGetStackHighWaterMark(&taskHouskeepingHandle);
//    utoa(buff,water_mark,10);
//    con_printf("taskHousekeeping, "); con_printf(buff); con_printf("\n");
//
//    #if (SCH_TRX_ONBOARD==1)
//    {
//        /* cheking taskComunications stack */
//        water_mark = uxTaskGetStackHighWaterMark(&taskComunicationsHandle);
//        utoa(buff,water_mark,10);
//        con_printf("taskComunications, "); con_printf(buff); con_printf("\n");
//    }
//    #endif
//
//    #if (SCH_USE_FLIGHTPLAN==1)
//    {
//        /* cheking taskFlightPlan stack */
//        water_mark = uxTaskGetStackHighWaterMark(&taskFlightPlanHandle);
//        utoa(buff,water_mark,10);
//        con_printf("taskFlightPlan, "); con_printf(buff); con_printf("\n");
//    }
//    #endif
//
//    #if (SCH_USE_FLIGHTPLAN2==1)
//    {
//        /* cheking taskFlightPlan2 stack */
//        water_mark = uxTaskGetStackHighWaterMark(&taskFlightPlan2Handle);
//        utoa(buff,water_mark,10);
//        con_printf("taskFlightPlan2, "); con_printf(buff); con_printf("\n");
//    }
//    #endif
//
//
//    size_t mem_heap = xPortGetFreeHeapSize();
//    itoa(buff,mem_heap,10);
//    con_printf("Free heap size, "); con_printf(buff); con_printf("\n");

    return 1;
}

/**
 * Debug command that executes an infinite loop, causing system being frozen
 * and reset by watchdog timeout.
 * @param param Not used
 * @return Never return. 0 if fails.
 */
int ppc_frozen(void *param)
{
    while(1);
    return 0;
}
