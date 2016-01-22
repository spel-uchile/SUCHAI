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
#include "cmdRTC.h"
#include "csp.h"

cmdFunction epsFunction[EPS_NCMD];
int eps_sysReq[EPS_NCMD];

void eps_onResetCmdEPS(void){
    printf("        eps_onResetCmdEPS\n");

    int i;
    for(i=0; i<EPS_NCMD; i++) eps_sysReq[i] = CMD_SYSREQ_MIN;
    
    epsFunction[(unsigned char)eps_id_isAlive] = eps_isAlive;
    epsFunction[(unsigned char)eps_id_initialize] = eps_initialize;
    epsFunction[(unsigned char)eps_id_print_hk] = eps_print_hk;
}

/**
 * Check if EPS is working ok. Communicates wih eps, reads and checks version
 * @param param *int 1-Verbose, 0-No verbose
 * @return 1-Ok, 0-Error
 */
int eps_isAlive(void *param)
{
    int ok = 0;
    ok = csp_ping(NODE_EPS, 1000, 4, CSP_O_NONE);
    ok = ok > 0 ? 1:0;
    
    #if(SCH_CMDEPS_VERBOSE)
    if(ok)
        printf("Ok\r\n");
    else
        printf("Error\r\n");
    #endif
    
	return ok;
}

/**
 * Initializes EPS and return current status
 * @param param *int 1-Verbose, 0-No verbose
 * @return 1-Ok, 0-Error
 */
int eps_initialize(void *param)
{
    //Nothing to do just test communication
    return eps_isAlive(NULL);
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
