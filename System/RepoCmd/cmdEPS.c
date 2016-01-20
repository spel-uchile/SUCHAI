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
#include "eps_suchai.h"

cmdFunction epsFunction[EPS_NCMD];
int eps_sysReq[EPS_NCMD];

void eps_onResetCmdEPS(void){
    printf("        eps_onResetCmdEPS\n");

    int i;
    for(i=0; i<EPS_NCMD; i++) eps_sysReq[i] = CMD_SYSREQ_MIN;
    
    epsFunction[(unsigned char)eps_id_isAlive] = eps_isAlive;
    epsFunction[(unsigned char)eps_id_print_hk] = eps_print_hk;
    
    //Olds commands
    epsFunction[(unsigned char)eps_id_readreg] = eps_readreg;
    
    epsFunction[(unsigned char)eps_id_readreg] = eps_readreg;
    epsFunction[(unsigned char)eps_id_status_read] = eps_status_read;
    
    epsFunction[(unsigned char)eps_id_update_internal_vars] = eps_update_internal_vars;
    epsFunction[(unsigned char)eps_id_soc_estimation] = eps_soc_estimation;
    epsFunction[(unsigned char)eps_id_energy_estimation] = eps_energy_estimation;

    epsFunction[(unsigned char)eps_id_current_meassurement] = eps_current_meassurement;
    epsFunction[(unsigned char)eps_id_panel_pwr_meassuerement] = eps_panel_pwr_meassuerement;
    epsFunction[(unsigned char)eps_id_pdm_off] = eps_pdm_off;
    
    epsFunction[(unsigned char)eps_id_print_all_reg] = eps_print_all_reg;
}

/**
 * Check if EPS is working ok. Communicates wih eps, reads and checks version
 * @param param *int 1-Verbose, 0-No verbose
 * @return 1-Ok, 0-Error
 */
int eps_isAlive(void *param)
{
    int ok = 0;
    int verbose = *((int *)(param));
    eps_version_t version;
    
    ok = eps_get_version(&version);
    
    if(ok)
    {
        //TODO: Check correct values
        ok = version.type == version.type ? 0:1;
        if(verbose)
            printf("EPS Version  %s %s %s %s\r\n", version.type, version.ver, version.date, version.time);
    }
    
    #if(SCH_CMDEPS_VERBOSE)
    if(ok)
        printf("Error\r\n");
    else
        printf("Ok\r\n");
    #endif
    
	return ok;
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

/**
 * Update various EPS structures (SOC, nergy estimation, etc)
 * @param param
 * @return 
 */
int eps_update_internal_vars(void *param){
    updateSOCEPS();
    updateStatusEPS();
    updateENERGYEPS();

    return 1;
}

/**
 * Muestra por consola los valores de todos los ADC de la EPS.
 * @param param ignorado, puede ser NULL
 * @return  siempre retorn 1
 */
int eps_print_all_reg(void *param){
    int i;

    printf("Reading EPS regs ..\r\n");
    rtc_print(NULL);

    for(i=0;i<34;i++){
        printf("reg[%d] = %d \r\n", i, eps_readreg( (void *)(&i) ) );
    }

    return 1;
}
/*------------------------------------------------------------------------------
 *                              EPS READ REGISTER
 *------------------------------------------------------------------------------
 * Description        : Display the reading of a EPS variable
 * Arguments          : param = 0-33
 *                              0-31 Are the 32 ADC channels (based on the
 *                                   CS-1UEPS2-NB-10 data sheet)
 *                              32   Display the EPS status register
 *                              33   Display the EPS version
 * Return Value       : readed value
 *----------------------------------------------------------------------------*/
int  eps_readreg(void *param)
{
    unsigned char argu = *((unsigned char *)(param));
    int lectura;

    if(argu<32){
        lectura = (int)ADCReadEPS(argu);
        #if(SCH_CMDEPS_VERBOSE > 1)
            printf("Lectura ADC: reg[%d]=", argu);
        #endif
    }
    else if(argu==32){
            lectura = (int)StatusReadEPS();
            #if(SCH_CMDEPS_VERBOSE > 1)
                printf("Lectura Status:");
            #endif
    }
    else if(argu==33){
        lectura = (int)VersionReadEPS();
        #if(SCH_CMDEPS_VERBOSE > 1)
            printf("Lectura Version:");
        #endif
    }
    else {
        lectura = (int)readEPSvars(EPS_ID_current_dir_est);
        #if(SCH_CMDEPS_VERBOSE > 1)
            printf("Estimacion Current Direction (0-Ch, 1-Disch):");
        #endif
    }

    #if(SCH_CMDEPS_VERBOSE > 1)
        printf("%d (dec), 0x%X (hex)\r\n", lectura, lectura);
    #endif
    
    return lectura;

}

/*------------------------------------------------------------------------------
 *		 	EPS SOC ESTIMATION
 *------------------------------------------------------------------------------
 * Description        : Reads the battery variables, and calculates a SOC estimation
 *                      The readings and estimation are saved internally. Proper
 *                      Data Repository Command must be used to save the variables
 *                      in the data repository
 * Arguments          : void
 * Return Value       : 1 - OK
 *----------------------------------------------------------------------------*/
int eps_soc_estimation(void *param)
{
    updateSOCEPS();
    return 1;
}

/*------------------------------------------------------------------------------
 *		 	EPS ENERGY ESTIMATION
 *------------------------------------------------------------------------------
 * Description        : Reads the battery variables, and calculates a ENERGY estimation
 *                      The readings and estimation are saved internally. Proper
 *                      Data Repository Command must be used to save the variables
 *                      in the data repository
 * Arguments          : void
 * Return Value       : 1 - OK
 *----------------------------------------------------------------------------*/
int eps_energy_estimation(void *param)
{
    updateENERGYEPS();
    return 1;
}

/*------------------------------------------------------------------------------
 *		 	EPS BUSES CURRENTS MEASSUREMENTS
 *------------------------------------------------------------------------------
 * Description        : Reads the buses currents.
 *                      The readings and estimation are saved internally. Proper
 *                      Data Repository Command must be used to save the variables
 *                      in the data repository
 * Arguments          : void
 * Return Value       : 1 - OK
 *----------------------------------------------------------------------------*/
int eps_current_meassurement(void *param)
{
    BusCurrEPS();
    return 1;
}

/*------------------------------------------------------------------------------
 *		 	EPS PANEL POWER MEASSUREMENT
 *------------------------------------------------------------------------------
 * Description        : Reads the panels voltages and currents, and calculates
 *                      the power sourced by the panels.
 *                      The readings and estimation are saved internally. Proper
 *                      Data Repository Command must be used to save the variables
 *                      in the data repository
 * Arguments          : void
 * Return Value       : 1 - OK
 *----------------------------------------------------------------------------*/
int  eps_panel_pwr_meassuerement(void *param)
{
    PnlMesEPS();
    return 1;
}

/*------------------------------------------------------------------------------
 *                              EPS STATUS READ
 *------------------------------------------------------------------------------
 * Description        : Reads the EPS status.
 *                      The readings and estimation are saved internally. Proper
 *                      Data Repository Command must be used to save the variables
 *                      in the data repository
 * Arguments          : void
 * Return Value       : 1 - OK
 *----------------------------------------------------------------------------*/
int  eps_status_read(void *param)
{
    updateStatusEPS();
    return 1;
}

/**
 * Turn off an EPS energy bus by a limited time performing a hard reset
 *
 * @param param Bus
 *  b0 - batery bus
 *  b1 - 5V bus
 *  b2 - 3.3V bus
 * @return 1 - OK, 0 - Invalid param
 */
int eps_pdm_off(void *param)
{
    int bus = *(int *)param;

    if ((bus >= 0) && (bus <= 7))
    {
        #if SCH_EPS_SUCHAI_VERBOSE
            con_printf("[EPS] Turning of energy bus\n");
        #endif

        PdmOffEPS((char)bus);
        return 1;
    }
    else
    {
        return 0;
    }
}