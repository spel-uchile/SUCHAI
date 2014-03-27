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

#define _CHECK_IF_EXECUTABLE_ SCH_TASKDISPATCHER_CHECK_IF_EXECUTABLE

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
    ppcFunction[(unsigned char)ppc_id_reset] = ppc_reset;
    ppc_sysReq[(unsigned char)ppc_id_reset]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_newosc] = ppc_newosc;
    ppc_sysReq[(unsigned char)ppc_id_newosc]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_osc] = ppc_osc;
    ppc_sysReq[(unsigned char)ppc_id_osc]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_enwdt] = ppc_enwdt;
    ppc_sysReq[(unsigned char)ppc_id_enwdt]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_reactToSOC] = ppc_reactToSOC;
    ppc_sysReq[(unsigned char)ppc_id_reactToSOC]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nOE_USB_nINT] = ppc_set_PPC_MB_nOE_USB_nINT;
    ppc_sysReq[(unsigned char)ppc_id_set_PPC_MB_nOE_USB_nINT]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nOE_MHX] = ppc_set_PPC_MB_nOE_MHX;
    ppc_sysReq[(unsigned char)ppc_id_set_PPC_MB_nOE_MHX]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nON_MHX] = ppc_set_PPC_MB_nON_MHX;
    ppc_sysReq[(unsigned char)ppc_id_set_PPC_MB_nON_MHX]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_set_PPC_MB_nON_SD] = ppc_set_PPC_MB_nON_SD;
    ppc_sysReq[(unsigned char)ppc_id_set_PPC_MB_nON_SD]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_rtos_debug] = ppc_rtos_debug;
    ppc_sysReq[(unsigned char)ppc_id_rtos_debug]  = CMD_SYSREQ_MIN;
    ppcFunction[(unsigned char)ppc_id_frozen] = ppc_frozen;
    ppc_sysReq[(unsigned char)ppc_id_frozen]  = CMD_SYSREQ_MIN;
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
    char *txt_result;
    int result;

    switch( i )
    {
		case 0:
                    result = PwrMgnt_OscSel(FRC_OSC);
                    txt_result = "new Oscillator is FRC_OSC\r\n";
                    break;
		case 1:
                    result = PwrMgnt_OscSel(FRC_OSC_WITH_POSTSCALER_PLL);
                    txt_result = "FRC_OSC_WITH_POSTSCALER_PLL\r\n";
                    break;
		case 2:
                    result = (PwrMgnt_OscSel(PRIMARY_OSC) );
                    txt_result = "PRIMARY_OSC\r\n";
                    break;
		case 3:
                    result = (PwrMgnt_OscSel(PRIMARY_OSC_WITH_PLL) );
                    txt_result = "PRIMARY_OSC_WITH_PLL\r\n";
                    break;
		case 4:
                    result = (PwrMgnt_OscSel(SECONDRY_OSC) );
                    txt_result = "SECONDRY_OSC\r\n";
                    break;
		case 5:
                    result = (PwrMgnt_OscSel(LOW_POWER_RC) );
                    txt_result = "LOW_POWER_RC\r\n";
                    break;
		case 7:
                    result = (PwrMgnt_OscSel(FRC_OSC_WITH_POSTSCALER) );
                    txt_result = "FRC_OSC_WITH_POSTSCALER\r\n";
                    break;
	}

#if SCH_CMDPPC_VERBOSE
    if(result){
        con_printf(txt_result);
    }
    else{
        con_printf("Failed to set new oscilator\r\n");
    }
#endif
        
    return result;
}

/*------------------------------------------------------------------------------
 *                                  PPC ENWDT
 *------------------------------------------------------------------------------
 * Description        : Activates/Deactivate WDT
 * Arguments          : int  on_off: 1=Active, 0=Inactive
 * Return Value       : int: 1=Success, 0=Failure
 *----------------------------------------------------------------------------*/
int ppc_enwdt(void *on_off)
{
    int result = 1;
    char *txt_result;
    int i = *(int *)on_off;

    if( i==0 )
    {
         EnableWDT( WDT_DISABLE );
         txt_result = "WDT_DISABLE\r\n";
         sta_setCubesatVar(sta_ppc_enwdt, 0);
    }
    else if( i==1 )
    {
         EnableWDT( WDT_ENABLE );
         txt_result = "WDT_ENABLE\r\n";
         sta_setCubesatVar(sta_ppc_enwdt, 1);
    }
    else
    {
        txt_result = "Not a valid argument\r\n";
        result = 0;
    }

    con_printf(txt_result);

    return result;
}

/*------------------------------------------------------------------------------
 *                                  PPC OSC
 *------------------------------------------------------------------------------
 * Description        : Returns current oscillator + 1
 * Arguments          : None
 * Return Value       : int: 1=FRC_OSC .. 8=FRC_OSC_WITH_POSTSCALER, 0ther=Failure
 *----------------------------------------------------------------------------*/
int ppc_osc(void *param)
{
    unsigned int osc = Current_OSCILLATOR();

    switch( osc )
    {
        case 0:
            con_printf("Oscillator is FRC_OSC\r\n");
        break;
        case 1:
            con_printf("Oscillator is FRC_OSC_WITH_POSTSCALER_PLL\r\n");
        break;
        case 2:
            con_printf("Oscillator is PRIMARY_OSC\r\n");
        break;
        case 3:
            con_printf("Oscillator is PRIMARY_OSC_WITH_PLL\r\n");
        break;
        case 4:
            con_printf("Oscillator is SECONDRY_OSC\r\n");
        break;
        case 5:
            con_printf("Oscillator is LOW_POWER_RC\r\n");
        break;
        case 7:
            con_printf("Oscillator is FRC_OSC_WITH_POSTSCALER\r\n");
        break;
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
    if(*(int *)param)
        con_printf("Resetting system NOW!! ...\r\n");
    PPC_SWReset;
    
    return 1;
}

/*------------------------------------------------------------------------------
 *                                  PPC 
 *------------------------------------------------------------------------------
 * Description        : 
 * Arguments          : None
 * Return Value       : int: 1=Always successfull
 *----------------------------------------------------------------------------*/
int ppc_reactToSOC(void* param)
{
    #if (SCH_CMDPPC_VERBOSE>=1)
        con_printf("ppc_reactToSOC()\r\n");
    #endif
    //Check_FSCM();
    //Check_Reset_Status();
    //Check_Wakeup_Status();

    // Tomas Opazo: Esto aun no esta probado, por lo que no se usara para las pruebas de SUCHAIv1.
    // De todos modos con no ejecutar el comando a nivel de taskDispather es suficiente, por lo que
    //probablemente este comando quede sin ejecutar linea alguna
    /*
    #if (_CHECK_IF_EXECUTABLE_==1)
        int trx_mode, cu_soc; static int prev_soc;
        cu_soc = dat_getCubesatVar(dat_eps_soc);
        if( cu_soc < (CMD_SYSREQ_MIN+1) )
        {
            con_printf("get_eps_soc()<(CMD_SYSREQ_MIN+1) Nivel critico, TRX debe silenciarse\r\n");
            #if (SCH_TRX_TYPE_ONBOARD==1)
                trx_mode=2; trx_setmode( (void *)&trx_mode );   //silent
            #endif
            prev_soc=cu_soc;
        }
        else
        {
            if( (prev_soc!=0) && (prev_soc<cu_soc) ){
                con_printf("get_eps_soc()>=(CMD_SYSREQ_MIN+1) Nivel critico reestablecido, TRX vuevle a nominal\r\n");
            }
            #if (SCH_TRX_TYPE_ONBOARD==1)
                trx_mode=5; trx_setmode( (void *)&trx_mode );   //nominal
            #endif
        }
    #endif
    */

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

    char ascii_val[10];
    //utoa(ascii_val, r, 16);
    sprintf (ascii_val, "0x%X", r);
    con_printf("ppc_set_PPC_MB_nOE_USB_nINT= "); con_printf(ascii_val); con_printf("\r\n");

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

    char ascii_val[10];
    //itoa(ascii_val, r, 16);
    sprintf (ascii_val, "0x%X", r);
    con_printf("ppc_set_PPC_MB_nOE_MHX= "); con_printf(ascii_val); con_printf("\r\n");

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

    char ascii_val[10];
    //itoa(ascii_val, r, 16);
    sprintf (ascii_val, "0x%X", r);
    con_printf("ppc_set_PPC_MB_nON_MHX= "); con_printf(ascii_val); con_printf("\r\n");

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

    char ascii_val[4];
    //itoa(ascii_val,r,16);
    sprintf (ascii_val, "0x%X", r);
    con_printf("ppc_set_PPC_MB_nON_SD= "); con_printf(ascii_val); con_printf("\r\n");

    return 1;
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
    char buff[10];
    unsigned int water_mark = 0;

    con_printf("Task, Stak Highwatermark\n");
    
    /* cheking taskDispatcher stack */
    water_mark = uxTaskGetStackHighWaterMark(&taskDispatcherHandle);
    utoa(buff,water_mark,10);
    con_printf("taskDispatcher, "); con_printf(buff); con_printf("\n");
    
    /* cheking taskExecuter stack */
    water_mark = uxTaskGetStackHighWaterMark(&taskExecuterHandle);
    utoa(buff,water_mark,10);
    con_printf("taskExecuter, "); con_printf(buff); con_printf("\n");

    /* cheking taskConsole stack */
    water_mark = uxTaskGetStackHighWaterMark(&taskConsoleHandle);
    utoa(buff,water_mark,10);
    con_printf("taskConsole,"); con_printf(buff); con_printf("\n");

    /* cheking taskHousekeeping stack */
    water_mark = uxTaskGetStackHighWaterMark(&taskHouskeepingHandle);
    utoa(buff,water_mark,10);
    con_printf("taskHousekeeping, "); con_printf(buff); con_printf("\n");

    #if (SCH_TRX_ONBOARD==1)
    {
        /* cheking taskComunications stack */
        water_mark = uxTaskGetStackHighWaterMark(&taskComunicationsHandle);
        utoa(buff,water_mark,10);
        con_printf("taskComunications, "); con_printf(buff); con_printf("\n");
    }
    #endif

    #if (SCH_USE_FLIGHTPLAN==1)
    {
        /* cheking taskFlightPlan stack */
        water_mark = uxTaskGetStackHighWaterMark(&taskFlightPlanHandle);
        utoa(buff,water_mark,10);
        con_printf("taskFlightPlan, "); con_printf(buff); con_printf("\n");
    }
    #endif

    #if (SCH_USE_FLIGHTPLAN2==1)
    {
        /* cheking taskFlightPlan2 stack */
        water_mark = uxTaskGetStackHighWaterMark(&taskFlightPlan2Handle);
        utoa(buff,water_mark,10);
        con_printf("taskFlightPlan2, "); con_printf(buff); con_printf("\n");
    }
    #endif


    size_t mem_heap = xPortGetFreeHeapSize();
    itoa(buff,mem_heap,10);
    con_printf("Free heap size, "); con_printf(buff); con_printf("\n");

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
