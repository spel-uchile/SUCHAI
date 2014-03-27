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
    dep_init_Peripherals(NULL);

    /* Repositorios */
    dep_init_Repos(NULL);

    /* Otras estructuras */
    dep_init_GnrlStrct(NULL);

    /* Initializing Transceiver */
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Setting TRX config\r\n");
    #endif
    trx_initialize(NULL);
}

/**
 * Initializes all data repositories
 *
 * @param param Not used
 * @return 1
 */
int dep_init_Repos(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_Repos] Initializing data repositories...\r\n");
    #endif

    #if (SCH_FLIGHTPLAN_TYPE == 1) || (SCH_FLIGHTPLAN_TYPE == 3)
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
        printf("    * Payloads status rep.\r\n");
    #endif
    dat_onResetPayloadVar();

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Status rep.\r\n");
    #endif
    dat_onResetCubesatVar();

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * Commands rep.\r\n");
    #endif
    /* Initializing cmdRepository */
    repo_onResetCmdRepo();

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
    setStateFlagEPS( (unsigned char)dat_getCubesatVar(dat_eps_state_flag) );

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
        const unsigned long Delayms = 0xFFFF / portTICK_RATE_MS;
        vTaskDelay(Delayms);
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
    xTaskCreate(taskConsole, (signed char *)"console", 1.5*configMINIMAL_STACK_SIZE, NULL, 2, &taskConsoleHandle);

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
//        printf("    * Creating taskHousekeeping\r\n");
    #endif
//    xTaskCreate(taskHouskeeping, (signed char *)"housekeeping", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskHouskeepingHandle);
    
    #if (SCH_TRX_ONBOARD == 1)
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * Creating taskCommunications\r\n");
        #endif
//        xTaskCreate(taskComunications, (signed char *)"comunications", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskComunicationsHandle);
        xTaskCreate(taskServerCSP, (signed char *)"SRV", 3*configMINIMAL_STACK_SIZE, NULL, 2, &taskComunicationsHandle);
        xTaskCreate(taskRxI2C, (signed char *)"I2C", 3*configMINIMAL_STACK_SIZE, NULL, 2, &taskComunicationsHandle);
    #endif

    if( dat_getCubesatVar(dat_msd_status) == 1 )
    {
        #if (SCH_FLIGHTPLAN_TYPE==0)
            //launching nothing..
        #elif (SCH_FLIGHTPLAN_TYPE==1)
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                    printf("    * Creating taskFlightPlan\r\n");
            #endif
            xTaskCreate(taskFlightPlan, (signed char *)"flightplan", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskFlightPlanHandle);
        #elif (SCH_FLIGHTPLAN_TYPE==2)
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                    printf("    * Creating taskFlightPlan2\r\n");
            #endif
            xTaskCreate(taskFlightPlan2, (signed char *)"flightplan2", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskFlightPlan2Handle);
        #elif (SCH_FLIGHTPLAN_TYPE==3)
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("    * Creating taskFlightPlan\r\n");
                printf("    * Creating taskFlightPlan2\r\n");
            #endif
            xTaskCreate(taskFlightPlan, (signed char *)"flightplan", 2*configMINIMAL_STACK_SIZE, NULL, 2, &taskFlightPlanHandle);
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

    if( dat_getCubesatVar(dat_dep_ant_deployed) == 0x0001 )
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
int dep_init_Peripherals(void *param)
{
    #if (SCH_TASKDEPLOYMENT_VERBOSE>=1)
        printf("\n[dep_init_Peripherals] Initializig external pheripherals...\r\n");
    #endif

    #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
        printf("    * External RTC\r\n");
    #endif
    RTC_init();

    #if (SCH_PAYCAM_nMEMFLASH_ONBOARD == 0) /* Camara no abordo*/
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * External flash memory\r\n");
        #endif
//        mem_init_memFlash();
    }
    #endif

    #if (SCH_TRX_ONBOARD ==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * Setting TRX\r\n");
        #endif
        /* Initializing Transceiver */
        trx_initialize(NULL);
    }
    #endif

    #if (SCH_SYSBUS_ONBOARD==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * EEPROM Memories\r\n");
        #endif
        dat_onReset_memEEPROM();
    }
    #endif

    #if (SCH_EXTMEM_SD_ONBOARD==1)
    {
        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * SD Memory  [delay]\r\n");
        #endif
        PPC_MB_nON_SD=1;

        /* Un delay para poder inicializar conrrectamente la SD si el PIC se resetea */
        const unsigned long DelaySd = 3000 / portTICK_RATE_MS;
        vTaskDelay(DelaySd);

        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * SD Memory [init ");
        #endif
        PPC_MB_nON_SD=0;
        unsigned char r = SD_init();
        if(r == 0)
        {
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("OK]\r\n");
            #endif
            dat_setCubesatVar(dat_msd_status, 1);
        }
        else
        {
            #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
                printf("FAIL]\r\n");
            #endif
            dat_setCubesatVar(dat_msd_status, 0);
        }

        #if (SCH_TASKDEPLOYMENT_VERBOSE>=2)
            printf("    * SD Memory [onReset]");
        #endif
        dat_onReset_memSD();
    }
    #endif

    return 1;
}

//Libcsp defines and functions
#define MY_ADDRESS (0)
void dep_csp_initialization(void)
{
    csp_debug_set_level(CSP_INFO, 1);
    csp_debug_set_level(CSP_PACKET, 1);
    csp_debug_set_level(CSP_BUFFER, 1);
    csp_debug_set_level(CSP_ERROR, 1);
    csp_debug_set_level(CSP_WARN, 1);

    /* Init buffer system with 3 packets of maximum 256 bytes each */
    csp_buffer_init(3, I2C_MTU+5);

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
    csp_debug_set_level(CSP_ERROR, 1);
    printf("\n---- Conn table ----\n");
    csp_conn_print_table();
    printf("---- Route table ----\n");
    csp_route_print_table();
    printf("---- Interfaces ----\n");
    csp_route_print_interfaces();

//    xTaskCreate(taskServerCSP, (signed char *)"SRV", 2*configMINIMAL_STACK_SIZE, NULL, 3, NULL);
}