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

#include "cmdEPS.h"

cmdFunction epsFunction[EPS_NCMD];
int eps_sysReq[EPS_NCMD];

void eps_onResetCmdEPS(void){
    printf("        eps_onResetCmdEPS\n");

    int i;
    for(i=0; i<EPS_NCMD; i++){eps_sysReq[i] = CMD_SYSREQ_MIN;}
    
    epsFunction[(unsigned char)eps_id_isAlive] = eps_isAlive;
    epsFunction[(unsigned char)eps_id_initialize] = eps_initialize;
    epsFunction[(unsigned char)eps_id_print_hk] = eps_print_hk;
    epsFunction[(unsigned char)eps_id_read_vars] = eps_read_vars;
}

/**
 * Check if EPS is working ok. Communicates wih eps, reads and checks version
 * @param param *int 1-Verbose, 0-No verbose
 * @return 1-Ok, 0-Error
 */
int eps_isAlive(void *param2)
{
    printf("eps_isAlive .. \r\n");
    #if (SCH_EPS_ONBOARD == 0)
        return 0;
    #endif

    int arg = NODE_EPS;    
    //return trx_ping(&arg);
    void *param = &arg;
    uint16_t com_timeout = 5000; //average time is 3000ms
    
    int result;
    int node = *((int *)param);

    #if SCH_CMDTRX_VERBOSE
        printf("Sending test frame to node %d...\n", node);
    #endif

    result = csp_ping(node, com_timeout, 10, CSP_O_NONE);

    #if SCH_CMDTRX_VERBOSE
        printf("Ping to %d of size %d, took %d ms\n", node, 10, result);
    #endif
    
    result = result > 0 ? 1:0;

    return result;
    
}

/**
 * Initializes EPS and return current status
 * @param param *int 1-Verbose, 0-No verbose
 * @return 1-Ok, 0-Error
 */
int eps_initialize(void *param)
{
    //Nothing to do just test communication
    return 1;//eps_isAlive(NULL);
}

/**
 * Print EPS housekeeping parameters
 * @param param Type: 0 ALL, 1 HK1, 2 HK2
 * @return 1-Command Ok, 0-Error
 */
int eps_print_hk(void *param)
{
    int hk_type = *((int *)(param));
    int bintmp;
    
    if(hk_type == 0 || hk_type == 2)
    {
        printf("Requesting EPS HK2 data\r\n");
        chkparam2_t chkparam2;
        if (!eps_get_hk2(&chkparam2)) {
            printf("Error requesting HK2\r\n");
            return 0;
        }

        printf("SOC %d %%\r\n", chkparam2.SOC);
        printf("Icount %lu As\r\n", chkparam2.Icount);
        printf("Uptime %lu s\r\n", chkparam2.UpTime);
        printf("Mode %d \r\n", chkparam2.Mode);
        printf("TimeoutGndWdt %lu s\r\n", chkparam2.TimeoutGndWDT);
        int i;
        printf("SwitchOnTimer  ");
        for (i=0;i<6;i++)
        {
            printf(" %d", chkparam2.SwitchOnTimer[i]);
        }
        printf(" s \r\n ");
        printf("SwitchOffTimer  ");
        for (i=0;i<6;i++)
        {
            printf(" %d", chkparam2.SwitchOffTimer[i]);
        }
        printf(" s \r\n ");
        printf("PPT fixed ref %d %d %d mV\r\n", chkparam2.PPTFixedRef[0], chkparam2.PPTFixedRef[1], chkparam2.PPTFixedRef[2]);
        printf("I2C wdt timeouts %d, GND wdt timeouts %d\r\n", chkparam2.i2c_wdt_timeouts, chkparam2.gnd_wdt_timeouts);
        printf("I2C obc ping timeouts %d, I2C com i3c timeouts %d\r\n", chkparam2.i2c_ping_timeouts[0], chkparam2.i2c_ping_timeouts[0]);
    }// End hk_type 2
    
    if(hk_type == 0 || hk_type == 1)
    {
        printf("Requesting EPS HK data\r\n");
        chkparam_t chkparam;
        if (!eps_get_hk(&chkparam)) {
            printf("Error requesting HK\r\n");
            return 0;
        }

        printf("ADC sample:\r\n");
        printf("Temp 1 %d\r\n", chkparam.temp[0]);
        printf("Temp 2 %d\r\n", chkparam.temp[1]);
        printf("Temp 3 %d\r\n", chkparam.temp[2]);
        printf("Temp PCB %d\r\n", chkparam.temp[3]);
        printf("BV %u\r\n", chkparam.bv);
        printf("PC %u\r\n", chkparam.pc);
        printf("SC %u\r\n", chkparam.sc);
        printf("PV1 %u\r\n", chkparam.pv[0]);
        printf("PV2 %u\r\n", chkparam.pv[1]);
        printf("PV3 %u\r\n", chkparam.pv[2]);
        printf("Latch up 1 %u\r\n", chkparam.latchup[0]);
        printf("Latch up 2 %u\r\n", chkparam.latchup[1]);
        printf("Latch up 3 %u\r\n", chkparam.latchup[2]);
        printf("Latch up 4 %u\r\n", chkparam.latchup[3]);
        printf("Latch up 5 %u\r\n", chkparam.latchup[4]);
        printf("Latch up 6 %u\r\n", chkparam.latchup[5]);
        printf("User Channel Status %02X\r\n", chkparam.channel_status);
        bintmp = chkparam.channel_status;
        int n;
        for(n=0; n<8; n++)
           {
              if((bintmp & 0x80) !=0)
              {
                 printf("1");
              }
              else
              {
                 printf("0");
              }
              if (n==3)
              {
                 printf(" "); /* insert a space between nybbles */
              }
              bintmp = bintmp<<1;
           }
        printf("\r\n");
        printf("Battery temperature 1 %d\r\n", chkparam.batt_temp[0]);
        printf("Battery temperature 2 %d\r\n", chkparam.batt_temp[1]);
        printf("Reset %d   BootCount %d   SW Err %d   PPT mode %d\r\n",chkparam.reset,chkparam.bootcount,chkparam.sw_errors,chkparam.ppt_mode);
    }// End hk_type 1
    
	return 1;
    
}

int eps_read_vars(void *param){
    int var2read = *((int *)param);
    #if (SCH_EPS_VERBOSE >= 2)
        printf("[eps_read_vars] Reading var %d \r\n", var2read);
    #endif
    
    chkparam_t sta_chkparam;
    if (!eps_get_hk(&sta_chkparam)) {
        printf("Error requesting HK\r\n");
        return 0;
    }
    
    switch(var2read){
        case EPS_ID_batt_temp_0 :
            return (int)sta_chkparam.batt_temp[0];
            break;
        case EPS_ID_batt_temp_1:
            return (int)sta_chkparam.batt_temp[1];
            break;
        case EPS_ID_bootcount :
            return (int)sta_chkparam.bootcount;
            break;        
        case EPS_ID_battery_voltage :
            return (int)sta_chkparam.bv;
            break;
        case EPS_ID_channel_status :
            return (int)sta_chkparam.channel_status;
            break;
        case EPS_ID_latchup_0 :
            return (int)sta_chkparam.latchup[0];
            break;
        case EPS_ID_panel_current :
            return (int)sta_chkparam.pc;
            break;
        case EPS_ID_ppt_mode :
            return (int)sta_chkparam.ppt_mode;
            break;
        case EPS_ID_panel_voltage_1:
            return (int)sta_chkparam.pv[0];
            break;
        case EPS_ID_panel_voltage_2:
            return (int)sta_chkparam.pv[1];
            break;
        case EPS_ID_panel_voltage_3:
            return (int)sta_chkparam.pv[2];
            break;
        case EPS_ID_reset:
            return (int)sta_chkparam.reset;
            break;
        case EPS_ID_system_current :
            return (int)sta_chkparam.sc;
            break;
        case EPS_ID_sw_errors:
            return (int)sta_chkparam.sw_errors;
            break;
        case EPS_ID_temp_conv1:
            return (int)sta_chkparam.temp[0];
            break;
        case EPS_ID_temp_conv2:
            return (int)sta_chkparam.temp[1];
            break;
        case EPS_ID_temp_conv3:
            return (int)sta_chkparam.temp[2];
            break;
        case EPS_ID_temp_board:
            return (int)sta_chkparam.temp[3];
            break;
        default :
            return 0;
            break;
    }
}
