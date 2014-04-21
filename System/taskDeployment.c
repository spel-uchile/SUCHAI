/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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
#include "taskDeployment.h"

extern xTaskHandle taskComunicationsHandle;
extern xTaskHandle taskConsoleHandle;
extern xTaskHandle taskFlightPlanHandle;
extern xTaskHandle taskFlightPlan2Handle;
extern xTaskHandle taskHouskeepingHandle;
extern xQueueHandle dispatcherQueue;


void taskDeployment(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE)
        printf(">>[Deployment] Started\r\n");
        printf("\n[Deployment] INITIALIZING SUCHAI FLIGHT SOFTWARE\r\n");
    #endif

    /* Perifericos*/
    dep_init_hw(NULL);

    /* Repositorios */
    dep_init_Repos(NULL);

    /* Otras estructuras */
    dep_init_GnrlStrct(NULL);

    /* Toopazo: Ahora deberia ser un comando (el rpimero) llamado en taskFligthPlan o equivalente */
//    /* Antena */
//    #if (SCH_ANTENNA_ONBOARD==1)
//        int realTime2 = SCH_TASKDEPLOYMENT_ANTENNA_REALTIME; /* 1=Real Time, 0=Debug Time */
//        dep_deploy_antenna( (void *)&realTime2 );
//    #endif


    /* Initializing Transceiver */
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Setting TRX config\r\n");
    #endif
    #if (SCH_TRX_ONBOARD==1)
        trx_initialize(NULL);
    #endif
}

//------------------------------------------------------------------------------
extern cmdFunction trxFunction[];
extern cmdFunction ppcFunction[];
extern cmdFunction conFunction[];
extern cmdFunction epsFunction[];
extern cmdFunction drpFunction[];
extern cmdFunction payFunction[];
extern cmdFunction rtcFunction[];
extern cmdFunction tcmFunction[];
extern int trx_sysReq[];
extern int ppc_sysReq[];
extern int con_sysReq[];
extern int eps_sysReq[];
extern int drp_sysReq[];
extern int pay_sysReq[];
extern int rtc_sysReq[];
extern int tcm_sysReq[];
/**
 * Initializes all data repositories
 *
 * @param param Not used
 * @return 1
 */
int dep_init_Repos(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_Repos] Initializing status repositories...\r\n");
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Status rep.\r\n");
    #endif
    sta_onResetStatRepo();
//------------------------------------------------------------------------------
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_Repos] Initializing command repositories...\r\n");
    #endif

//    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
//        printf("    * Commands rep.\r\n");
//    #endif
//    repo_onResetCmdRepo();

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdPPC..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdPPC_handler;
    cmdPPC_handler.cmdOwn = SCH_CMD_PPC;
    cmdPPC_handler.nCmd = PPC_NCMD;
    cmdPPC_handler.p_xxxFunction = ppcFunction;
    cmdPPC_handler.p_xxxSysReq = ppc_sysReq;
    cmdPPC_handler.xxx_onReset = ppc_onResetCmdPPC;
    repo_set_cmdXXX_hanlder(cmdPPC_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdCON..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdCON_handler;
    cmdCON_handler.cmdOwn = SCH_CMD_CON;
    cmdCON_handler.nCmd = CON_NCMD;
    cmdCON_handler.p_xxxFunction = conFunction;
    cmdCON_handler.p_xxxSysReq = con_sysReq;
    cmdCON_handler.xxx_onReset = con_onResetCmdCON;
    repo_set_cmdXXX_hanlder(cmdCON_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdTRX..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdTRX_handler;
    cmdTRX_handler.cmdOwn = SCH_CMD_TRX;
    cmdTRX_handler.nCmd = TRX_NCMD;
    cmdTRX_handler.p_xxxFunction = trxFunction;
    cmdTRX_handler.p_xxxSysReq = trx_sysReq;
    cmdTRX_handler.xxx_onReset = trx_onResetCmdTRX;
    repo_set_cmdXXX_hanlder(cmdTRX_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdEPS..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdEPS_handler;
    cmdEPS_handler.cmdOwn = SCH_CMD_EPS;
    cmdEPS_handler.nCmd = EPS_NCMD;
    cmdEPS_handler.p_xxxFunction = epsFunction;
    cmdEPS_handler.p_xxxSysReq = eps_sysReq;
    cmdEPS_handler.xxx_onReset = eps_onResetCmdEPS;
    repo_set_cmdXXX_hanlder(cmdEPS_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdDRP..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdDRP_handler;
    cmdDRP_handler.cmdOwn = SCH_CMD_DRP;
    cmdDRP_handler.nCmd = DRP_NCMD;
    cmdDRP_handler.p_xxxFunction = drpFunction;
    cmdDRP_handler.p_xxxSysReq = drp_sysReq;
    cmdDRP_handler.xxx_onReset = drp_onResetCmdDRP;
    repo_set_cmdXXX_hanlder(cmdDRP_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdPAY..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdPAY_handler;
    cmdPAY_handler.cmdOwn = SCH_CMD_PAY;
    cmdPAY_handler.nCmd = PAY_NCMD;
    cmdPAY_handler.p_xxxFunction = payFunction;
    cmdPAY_handler.p_xxxSysReq = pay_sysReq;
    cmdPAY_handler.xxx_onReset = pay_onResetCmdPAY;
    repo_set_cmdXXX_hanlder(cmdPAY_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdRTC..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdRTC_handler;
    cmdRTC_handler.cmdOwn = SCH_CMD_RTC;
    cmdRTC_handler.nCmd = RTC_NCMD;
    cmdRTC_handler.p_xxxFunction = rtcFunction;
    cmdRTC_handler.p_xxxSysReq = rtc_sysReq;
    cmdRTC_handler.xxx_onReset = rtc_onResetCmdRTC;
    repo_set_cmdXXX_hanlder(cmdRTC_handler);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Attaching cmdTCM..\r\n");
    #endif
    CmdRepo_cmdXXX_handler cmdTCM_handler;
    cmdTCM_handler.cmdOwn = SCH_CMD_TCM;
    cmdTCM_handler.nCmd = TCM_NCMD;
    cmdTCM_handler.p_xxxFunction = tcmFunction;
    cmdTCM_handler.p_xxxSysReq = tcm_sysReq;
    cmdTCM_handler.xxx_onReset = tcm_onResetCmdTCM;
    repo_set_cmdXXX_hanlder(cmdTCM_handler);
   

//------------------------------------------------------------------------------
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_Repos] Initializing data repositories...\r\n");
    #endif

    #if (SCH_USE_FLIGHTPLAN == 1 )
        /* Initializing dataRepository */
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * FlighPlan\r\n");
        #endif
        dat_onResetFlightPlan();
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Telecommands buffer\r\n");
    #endif
    dat_onResetTelecmdBuff();

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Payloads data rep.\r\n");
    #endif
    dat_onResetPayloadVar();

    return 1;
}

/**
 * Initializes all data repositories
 *
 * @param param Not used
 * @return 1
 */
int dep_init_GnrlStrct(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_GnrlStruct] Initializing other structures...\r\n");
    #endif

    /* Initializing EPS struct */
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * init EPS structs\r\n");
    #endif
    setStateFlagEPS( (unsigned char)sta_getCubesatVar(sta_eps_state_flag) );

    return 1;
}

/**
 * Ends taskDeployment by deleting it.
 * @param param Not used
 * @return 1 succes
 */
int dep_suicide(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE)
        printf("[Deployment] ENDS\r\n");
        printf("[Deployment] Deleting task\r\n");
    #endif
    vTaskDelete(NULL);

    while(1)
    {
        printf("    vTaskDelete(NULL) did NOT work out...\r\n");
        __delay_ms(3000);
    }

    return 1;
}

/**
 * Starts all task.
 * @param param Not used
 * @return 1 success, 0 fails
 */
int dep_launch_tasks(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_launch_tasks] Starting all tasks...\r\n");
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Creating taskConsole\r\n");
    #endif
    xTaskCreate(taskConsole, (signed char *)"CON", 1.5*configMINIMAL_STACK_SIZE, NULL, 2, &taskConsoleHandle);

    #if (SCH_USE_HOUSEKEEPING==1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * Creating taskHousekeeping\r\n");
        #endif
        xTaskCreate(taskHouskeeping, (signed char *)"HKP", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskHouskeepingHandle);
    #endif
    
    #if (SCH_TRX_ONBOARD == 1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * Creating taskCommunications\r\n");
        #endif
        xTaskCreate(taskComunications, (signed char *)"COM", 3*configMINIMAL_STACK_SIZE, NULL, 2, &taskComunicationsHandle);
    #endif

    if( sta_getCubesatVar(sta_MemSD_isAlive) == 1 )
    {
        #if (SCH_USE_FLIGHTPLAN==0)
            //launch nothing..
        #elif (SCH_USE_FLIGHTPLAN==1)
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                    printf("    * Creating taskFlightPlan\r\n");
            #endif
            xTaskCreate(taskFlightPlan, (signed char *)"flightplan", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskFlightPlanHandle);
        #endif
        #if (SCH_USE_FLIGHTPLAN2==0)
            //launch nothing..
        #elif (SCH_USE_FLIGHTPLAN2==1)
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                    printf("    * Creating taskFlightPlan2\r\n");
            #endif
            xTaskCreate(taskFlightPlan2, (signed char *)"flightplan2", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskFlightPlan2Handle);
        #endif
    }
        
    return 1;
}

/**
 * Deploys satellite antennas
 * @param param 1 realime, 0 debug time
 * @return 1 success, 0 fails
 */
int dep_deploy_antenna(void *param)
{

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_deploy_antenna] Deploying TRX Antenna... \r\n");
    #endif

    if( sta_getCubesatVar(sta_dep_ant_deployed) == 0x0001 )
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
            printf("    * Antenna is already deployed\r\n");
        #endif
        return 1;
    }

    //Realtime=1 DebugTime=0
    unsigned int delay_dep_time, delay_rest_dep_time, delay_recheck_dep_time;
    int mode= *( (int *)param );
    if(mode)
    {
        delay_dep_time = (TDP_DEPLOY_TIME) / portTICK_RATE_MS;
        delay_rest_dep_time = (TDP_REST_DEPLOY_TIME) / portTICK_RATE_MS;
        delay_recheck_dep_time = (TDP_RECHECK_TIME) / portTICK_RATE_MS;
    }
    else
    {
        delay_dep_time = (600) / portTICK_RATE_MS;
        delay_rest_dep_time = (400) / portTICK_RATE_MS;
        delay_recheck_dep_time = (200) / portTICK_RATE_MS;
    }

    //Quemado del nylon
    int tries_indx = 0;

    #if(SCH_ANTENNA_ONBOARD == 1)
    {
        for(tries_indx=1; tries_indx<=TDP_TRY_DEPLOY; tries_indx++)
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


            if(PPC_ANT12_CHECK==0)   /* reviso */
            {
                vTaskDelay(delay_recheck_dep_time);   /* tiempo de RE-chequeo */
                if(PPC_ANT12_CHECK==0)   /* RE-reviso */
                {
                    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
                        printf("    ANTENNA DEPLOYED SUCCESSFULLY [%d TRIES]\r\n", tries_indx);
                    #endif

                    drp_dep_write_deployed(1, tries_indx);
                    return 1;
                }
            }
        }
    }
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    ANTENNA DEPLOY FAIL [%d TRIES]\r\n", TDP_TRY_DEPLOY);
    #endif

    drp_dep_write_deployed(0, tries_indx);

    return 0;
}

/**
 * Initializes all peripherals and subsystems.
 * @param param Not used.
 * @return 1 success, 0 fail.
 */
int dep_init_hw(void *param)
{
    int resp;
    STA_CubesatVar hw_isAlive;

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_hw] Initializig external hardware...\r\n");
    #endif

    #if (SCH_SYSBUS_ONBOARD==1)
    {
        #if (SCH_MEMEEPROM_ONBOARD==1)
        {
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("    * External MemEEPROM .. ");
            #endif
            resp = init_memEEPROM();
            hw_isAlive = sta_MemEEPROM_isAlive;
            sta_setCubesatVar(hw_isAlive, resp);
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                if(resp == 0x01){
                    printf("Ok\r\n");
                }
                else{
                    printf("Fail\r\n");
                }
            #endif
        }
        #endif
    }
    #endif

    #if (SCH_RTC_ONBOARD==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * External RTC .. ");
        #endif
        resp = RTC_init();
        hw_isAlive = sta_RTC_isAlive;
        sta_setCubesatVar(hw_isAlive, resp);
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            if(resp == 0x01){
                printf("Ok\r\n");
            }
            else{
                printf("Fail\r\n");
            }
        #endif
    }
    #endif

    #if (SCH_TRX_ONBOARD==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * External TRX .. ");
        #endif
        resp  = trx_initialize(NULL);
        hw_isAlive = sta_TRX_isAlive;
        sta_setCubesatVar(hw_isAlive, resp);
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            if(resp == 0x01){
                printf("Ok\r\n");
            }
            else{
                printf("Fail\r\n");
            }
        #endif
    }
    #endif

    #if (SCH_MEMSD_ONBOARD==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * External MemSD .. ");
        #endif
        resp = dat_sd_init();
        hw_isAlive = sta_MemSD_isAlive;
        sta_setCubesatVar(hw_isAlive, resp);
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            if(resp == 0x01){
                printf("Ok\r\n");
            }
            else{
                printf("Fail\r\n");
            }
        #endif
    }
    #endif

    return 1;
}

int dat_sd_init(void){
    //apagar energia MemSD
    PPC_MB_nON_SD=1;
    /* Un delay para poder inicializar conrrectamente la SD si el PIC se resetea */
//    unsigned long i;
//    for(i = 0x004FFFFF; i>0; i--){}
    __delay_ms(3000);
    //encender energia MemSD
    PPC_MB_nON_SD=0;
    unsigned char r = SD_init();
    if(r == 0){
        dat_onReset_memSD(FALSE);
        return 1;
    }
    else{
        return 0;
    }
}


//Libcsp defines and functions
#define MY_ADDRESS (0)
void dep_csp_initialization(void)
{
    csp_debug_set_level(CSP_INFO, 1);
    csp_debug_set_level(CSP_PACKET, 0); /* Fails if activated */
    csp_debug_set_level(CSP_BUFFER, 0); /* Fails if activated */
    csp_debug_set_level(CSP_ERROR, 1);
    csp_debug_set_level(CSP_WARN, 1);

    /* Init buffer system with 3 packets of maximum I2C_MTU bytes each */
    csp_buffer_init(5, TRX_TMFRAMELEN8+5);

    /* Init CSP with address MY_ADDRESS */
    csp_init(MY_ADDRESS);
    csp_i2c_init(SCH_I2C1_ADDR, 0, 400);

    csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_i2c, CSP_NODE_MAC);
    csp_route_start_task(2*configMINIMAL_STACK_SIZE, 3);

    /* Create socket without any socket options */
    csp_socket_t *sock = csp_socket(CSP_SO_NONE);

    /* Bind all ports to socket */
    csp_bind(sock, CSP_ANY);

    /* Create connections backlog queue */
    csp_listen(sock, 5);

    //DEBUG
    printf("\n---- Conn table ----\n");
    csp_conn_print_table();
    printf("---- Route table ----\n");
    csp_route_print_table();
    printf("---- Interfaces ----\n");
    csp_route_print_interfaces();

//    xTaskCreate(taskServerCSP, (signed char *)"SRV", 2*configMINIMAL_STACK_SIZE, NULL, 3, NULL);
}