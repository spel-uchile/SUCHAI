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

cmdFunction epsFunction[EPS_NCMD];
int eps_sysReq[EPS_NCMD];

void eps_onResetCmdEPS(void){
    printf("        eps_onResetCmdEPS\n");

    int i;
    for(i=0; i<EPS_NCMD; i++) eps_sysReq[i] = CMD_SYSREQ_MIN;

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
    #if(SCH_CMDEPS_VERBOSE > 1)
        char buffer[40];
    #endif

    if(argu<32){
        lectura = (int)ADCReadEPS(argu);
        #if(SCH_CMDEPS_VERBOSE > 1)
            sprintf (buffer, "Lectura ADC: reg[%d]=", argu);
            con_printf(buffer);
        #endif
    }
    else if(argu==32){
            lectura = (int)StatusReadEPS();
            #if(SCH_CMDEPS_VERBOSE > 1)
                con_printf("Lectura Status:");
            #endif
    }
    else if(argu==33){
        lectura = (int)VersionReadEPS();
        #if(SCH_CMDEPS_VERBOSE > 1)
            con_printf("Lectura Version:");
        #endif
    }
    else {
        lectura = (int)readEPSvars(EPS_ID_current_dir_est);
        #if(SCH_CMDEPS_VERBOSE > 1)
            con_printf("Estimacion Current Direction (0-Ch, 1-Disch):");
        #endif
    }

    #if(SCH_CMDEPS_VERBOSE > 1)
        sprintf (buffer, "%d (dec), 0x%X (hex)\r\n", lectura, lectura);
        con_printf(buffer);
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