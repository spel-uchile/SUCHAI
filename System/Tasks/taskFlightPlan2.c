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

#include "taskFlightPlan2.h"

extern xQueueHandle dispatcherQueue;

void taskFlightPlan2(void *param)
{
#if (SCH_FLIGHTPLAN2_VERBOSE)
        printf(">>[FlightPlan2] Started\r\n");
#endif

#if SCH_FLIGHTPLAN2_REALTIME
    unsigned int min_check_period_ms = 10000;      /* check every x ms  */
    portTickType check_time = (min_check_period_ms) / portTICK_RATE_MS;
    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
#else
    unsigned int min_check_period_ms = 1000;      /* check every 2sec  */
    portTickType check_time = (min_check_period_ms) / portTICK_RATE_MS;
    portTickType check_deployment_time = (10000) / portTICK_RATE_MS;      /* check every 10sec  */
#endif

    DispCmd NewCmd;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2; /* Housekeeping */
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.param = 0;

    /*Avoid the acummulation of commands while the SUCHAI is still deploying.. */
    portTickType xLastWakeTime = xTaskGetTickCount();
    #if (SCH_USE_HOUSEKEEPING == 1)
        while( TRUE ){
            if( sta_get_stateVar(sta_dep_ant_deployed)==1 ){
                break;
            }
            vTaskDelayUntil(&xLastWakeTime, check_deployment_time);
        }
    #endif

    while(1)
    {
        /* min_check_period_ms actions */
        vTaskDelayUntil(&xLastWakeTime, check_time);
        //Add commands below ..

        #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
            printf("[FlightPlan2] min_check_period_ms (%d) actions ..\r\n", min_check_period_ms);
        #endif

        //fp2_pay_i_multiplexing(dispatcherQueue);
        if(sta_get_stateVar(sta_ppc_opMode)==STA_PPC_OPMODE_NORMAL){
            fp2_pay_i_simultaneous(dispatcherQueue);
        }

//            //ejecuto payload tmEstado
//            NewCmd.cmdId = pay_id_FSM_default;
//            NewCmd.param = dat_pay_tmEstado;
//            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
            
    }
}

void fp2_pay_i_multiplexing(xQueueHandle dispatcherQueue){
    printf("fp2_pay_i_multiplexing ..\r\n");

    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2;
    NewCmd.param = 0;

    static DAT_Payload_Buff current_pay_i;
    
    //multiplexo pay_i por turnos
    #if (SCH_FLIGHTPLAN2_VERBOSE>=2)
        printf("fp2_pay_i_multiplexing => pay_i = %d\r\n", (unsigned int)current_pay_i);
    #endif

    int pay_i_state = sta_get_stateVar(sta_DAT_Payload_Buff_to_STA_StateVar(current_pay_i));
    switch(pay_i_state){
        case SRP_PAY_XXX_STATE_INACTIVE:
            printf("  pay_i = "); dat_print_payload_name(current_pay_i);
            printf(", state = SRP_PAY_XXX_STATE_INACTIVE \r\n");
            break;
        case SRP_PAY_XXX_STATE_ACTIVE:
            printf("  pay_i = "); dat_print_payload_name(current_pay_i);
            printf(", state = SRP_PAY_XXX_STATE_ACTIVE \r\n");
            break;
        case SRP_PAY_XXX_STATE_RUN_INIT:
            printf("  pay_i = "); dat_print_payload_name(current_pay_i);
            printf(", state = SRP_PAY_XXX_STATE_RUNNING \r\n");
            break;
        case SRP_PAY_XXX_STATE_WAITING_TX:
            printf("  pay_i = "); dat_print_payload_name(current_pay_i);
            printf(", state = SRP_PAY_XXX_STATE_WAITING_TX \r\n");
            break;
        default:
            printf("  pay_i = "); dat_print_payload_name(current_pay_i);
            printf(", state = [ERROR] Unknown State \r\n");
            break;
    }
    return;

    switch(current_pay_i){
        //excluyo casos particulares
        case dat_pay_camera:
            //do nothing
            printf("dat_pay_tmEstado does not use FSM_default..\r\n");
            break;
        case dat_pay_expFis:
            //do nothing
            //printf("dat_pay_tmEstado does not use FSM_default..\r\n");
            break;
        default:
            NewCmd.cmdId = pay_id_FSM_default;
            NewCmd.param = current_pay_i;
            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
            break;
    }

    //Si el actual es el ultimo, el proximo sera el primero
    if( current_pay_i == (dat_pay_last_one-1) ){
        current_pay_i=0;
    }
    else{
        current_pay_i++;
    }
}

/**
 * Check the payloads "simultaneouslly" and execute it in reentrant calling multiples
 * @param dispatcherQueue
 */
void fp2_pay_i_simultaneous(xQueueHandle dispatcherQueue)
{
    DispCmd NewCmd;
    NewCmd.cmdId = CMD_CMDNULL;
    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2;
    NewCmd.param = 0;

    //reviso payloads "simultaneamente" y ejecuto en multiplos de cada llamada reentrante
    static long unsigned int exec_tick;
    exec_tick++;

    DAT_Payload_Buff pay_i; int pay_i_state, next_state, pay_i_tick_rate;
    for(pay_i = 0; pay_i < dat_pay_last_one; pay_i++)
    {
        //if( fp2_sim_exe_every_tick[pay_i]%exec_tick )
        pay_i_state = sta_get_stateVar(sta_DAT_Payload_Buff_to_STA_StateVar(pay_i));
        switch(pay_i_state){
            case SRP_PAY_XXX_STATE_INACTIVE:
//                printf("  pay_i = "); dat_print_payload_name(pay_i); printf("\r\n");
//                printf("  state = SRP_PAY_XXX_STATE_INACTIVE \r\n");
//                printf("---------------------------------------------\r\n");
                break;
            case SRP_PAY_XXX_STATE_ACTIVE:
                pay_i_tick_rate = fp2_get_pay_exec_rate(pay_i);
                if( exec_tick%pay_i_tick_rate == 0 ){
                    printf("  pay_i = %d = ", pay_i); dat_print_payload_name(pay_i); printf("\r\n");
                    printf("  state = SRP_PAY_XXX_STATE_ACTIVE \r\n");
                    printf("  pay_i_tick_rate = %d \r\n", pay_i_tick_rate);
                    printf("  exec_tick = %lu \r\n", exec_tick);
                    printf("---------------------------------------------\r\n");

//                    //change state to SRP_PAY_XXX_STATE_RUN_INIT
//                    next_state = SRP_PAY_XXX_STATE_RUN_INIT;
//                    fp2_set_pay_state(pay_i, next_state);
                }
                break;
            case SRP_PAY_XXX_STATE_RUN_INIT:
                printf("  pay_i = "); dat_print_payload_name(pay_i);
                printf(", state = SRP_PAY_XXX_STATE_RUN_INIT \r\n");
                break;
            case SRP_PAY_XXX_STATE_RUN_TAKE:
                printf("  pay_i = "); dat_print_payload_name(pay_i);
                printf(", state = SRP_PAY_XXX_STATE_RUN_TAKE \r\n");
                break;
            case SRP_PAY_XXX_STATE_RUN_STOP:
                printf("  pay_i = "); dat_print_payload_name(pay_i);
                printf(", state = SRP_PAY_XXX_STATE_RUN_STOP \r\n");
                break;
            case SRP_PAY_XXX_STATE_WAITING_TX:
                printf("  pay_i = "); dat_print_payload_name(pay_i);
                printf(", state = SRP_PAY_XXX_STATE_WAITING_TX \r\n");
                break;
            default:
                printf("  pay_i = "); dat_print_payload_name(pay_i);
                printf(", state = [ERROR] Unknown State \r\n");
                break;
        }
//        STA_StateVar dat_pay_xxx_state = sta_DAT_Payload_Buff_to_STA_StateVar(dat_pay_tmEstado);
//        if(sta_get_stateVar(dat_pay_xxx_perform) == 0x0001)
//        {
//            NewCmd.cmdId = pay_id_FSM_default;
//            NewCmd.param = pay_i;
//            xQueueSend(dispatcherQueue, &NewCmd, portMAX_DELAY);
//        }
        //__delay_ms(100);   //separate commands
    }
}

/**
 * Return the number of tick before a pay_i is to be executed
 * @param pay_i
 * @return Return the number of tick before a pay_i is to be executed
 */
int fp2_get_pay_exec_rate(DAT_Payload_Buff pay_i){
    switch(pay_i){
        case dat_pay_tmEstado:
            return 1;
            break;
        case dat_pay_battery:
            return 2;
            break;
        case dat_pay_debug:
            return 3;
            break;
        case dat_pay_lagmuirProbe:
            return 4;
            break;
        case dat_pay_gps:
            return 5;
            break;
        case dat_pay_camera:
            return 6;
            break;
        case dat_pay_sensTemp:
            return 7;
            break;
        case dat_pay_gyro:
            return 8;
            break;
        case dat_pay_expFis:
            return 9;
            break;
        case dat_pay_last_one:
            //ignore
            break;
    }
    return -1;
}

void fp2_set_pay_state(DAT_Payload_Buff pay_i, int state){
    //change state
    switch(pay_i){
        case dat_pay_tmEstado:
            pay_set_state_tmEstado(&state);
            break;
        case dat_pay_battery:
            pay_set_state_battery(&state);
            break;
        case dat_pay_debug:
            pay_set_state_debug(&state);
            break;
        case dat_pay_lagmuirProbe:
            pay_set_state_lagmuirProbe(&state);
            break;
        case dat_pay_gps:
            pay_set_state_gps(&state);
            break;
        case dat_pay_camera:
            pay_set_state_camera(&state);
            break;
        case dat_pay_sensTemp:
            pay_set_state_sensTemp(&state);
            break;
        case dat_pay_gyro:
            pay_set_state_gyro(&state);
            break;
        case dat_pay_expFis:
            pay_set_state_expFis(&state);
            break;
        case dat_pay_last_one:
            //ignore
//            state = state;
            break;
    }
}