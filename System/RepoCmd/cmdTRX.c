/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

#include "cmdTRX.h"
#include "csp.h"
#include "csp_port.h"

static int trx_tm_send(uint16_t *data, int len);

/* Auxiliary variables */
nanocom_conf_t TRX_CONFIG; /*Stores TRX configuration*/
static uint16_t com_timeout = 5000; //average time is 3000ms

cmdFunction trxFunction[TRX_NCMD];
int trx_sysReq[TRX_NCMD];

void trx_onResetCmdTRX(void){
    printf("        trx_onResetCmdTRX\n");

    int i;
    for(i=0; i<TRX_NCMD; i++) trx_sysReq[i] = CMD_SYSREQ_MIN;
    
    trxFunction[(unsigned char)trx_id_isAlive] = trx_isAlive;
    trxFunction[(unsigned char)trx_id_get_operation_mode] = trx_get_operation_mode;
    trxFunction[(unsigned char)trx_id_set_operation_mode] = trx_set_operation_mode;
    trxFunction[(unsigned char)trx_id_get_count_tm] = trx_get_count_tm;
    trxFunction[(unsigned char)trx_id_set_count_tm] = trx_set_count_tm;
    trxFunction[(unsigned char)trx_id_get_count_tc] = trx_get_count_tc;
    trxFunction[(unsigned char)trx_id_set_count_tc] = trx_set_count_tc;
    trxFunction[(unsigned char)trx_id_get_day_last_tc] = trx_get_day_last_tc;
    trxFunction[(unsigned char)trx_id_set_day_last_tc] = trx_set_day_last_tc;
    trxFunction[(unsigned char)trx_id_get_tx_baud] = trx_get_tx_baud;
    trxFunction[(unsigned char)trx_id_set_tx_baud] = trx_set_tx_baud;
    trxFunction[(unsigned char)trx_id_get_rx_baud] = trx_get_rx_baud;
    trxFunction[(unsigned char)trx_id_set_rx_baud] = trx_set_rx_baud;
    trxFunction[(unsigned char)trx_id_get_beacon_level] = trx_get_beacon_level;
    trxFunction[(unsigned char)trx_id_set_beacon_level] = trx_set_beacon_level;
    trxFunction[(unsigned char)trx_id_get_beacon_period] = trx_get_beacon_period;
    trxFunction[(unsigned char)trx_id_set_beacon_period] = trx_set_beacon_period;

    trxFunction[(unsigned char)trx_id_set_conf] = trx_set_conf;
    trxFunction[(unsigned char)trx_id_read_conf] = trx_read_conf;
    trxFunction[(unsigned char)trx_id_ping] = trx_ping;
    trxFunction[(unsigned char)trx_id_getstatus] = trx_getstatus;
    trxFunction[(unsigned char)trx_id_set_beacon] = trx_set_beacon;
    trxFunction[(unsigned char)trx_id_initialize] = trx_initialize;

    //Power budget restriction
    // Ej. trx_sysReq[(unsigned char)trx_id_tm_trxstatus]  = CMD_SYSREQ_MIN+2;
}

/* Status repository commands */

/**
 * Check if the TRX is working normally
 * @param param not used
 * @return 1 Ok, 0 Fail
 */
int trx_isAlive(void *param){
    //int arg = NODE_COM;
    //return trx_ping(&arg);
    
    //return trx_getstatus(NULL);
    //return trx_read_conf(NULL);
    #if (SCH_TRX_ONBOARD == 1)
        return 1;
    #else
        return 0;
    #endif
}

/**
 * Returns TRX current operation mode
 * @param param Not used
 * @return 1(Normal), 0(ERROR)
 */
int trx_get_operation_mode(void *param)
{
    // TODO: Implement operation mode report (normal, silent, etc)
    // Probably don't need to store values in EEPROM
    return 1;
}

/**
 * Set TRX mode
 * @param param (0)RESET, (1)SYSRESET, (2)SILENT, (3)ONLYBEACON, (4)NOBEACON, (5)NOMINAL
 * @return 1 - OK; 0 - Fail
 */
int trx_set_operation_mode(void *param)
{
    int mode = *(int *)param;

    //TODO: Implement

    return mode;
}

/**
 * Get current TM count (from status repository)
 * @param param Not used
 * @return TM count
 */
int trx_get_count_tm(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_count_tm;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}

/**
 * Set current TM count to status repository
 * @param param (int *) current TM count
 * @return 1(Ok)
 */
int trx_set_count_tm(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_count_tm;
    int value = *((int*)param);
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}

/**
 * Get current TC count
 * @param param Not used
 * @return TC count
 */
int trx_get_count_tc(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_count_tc;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}

/**
 * Set current TC count to status repository
 * @param param (int *) TC count
 * @return 1 (Ok)
 */
int trx_set_count_tc(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_count_tc;
    int value = *((int*)param);
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}

/**
 * Get the day of the last received TC from status repo.
 * @param param Not used
 * @return Day of the last TC
 */
int trx_get_day_last_tc(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_day_last_tc;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}

/**
 * Set the day of the last received TC to status repo.
 * @param param (int *) Day of las TC
 * @return 1 (Ok)
 */
int trx_set_day_last_tc(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_trx_day_last_tc;
    int value = *((int*)param);
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}

/**
 * Sets baudrate for telemetry
 *
 * @param param RX Baurade 12=1200bps, 24=2400bps, 48=4800bps [48 default]
 * @return 1 - OK; 0 - Fail
 */
int trx_set_tx_baud(void *param)
{
    int result;
    int baud = *((int *)param);

    if(!((baud==12) || (baud==24) || (baud==48)))
        return 0;

    result = trx_read_conf(NULL);
    if(!result)
        return 0;

    TRX_CONFIG.tx_baud = (uint8_t)baud;

    // Save changes to status repo
    MemEEPROM_Vars mem_eeprom_var = mem_trx_tx_baud;
    writeIntEEPROM1(mem_eeprom_var, baud);

    // Save changes to device
    result = trx_set_conf(NULL);
    return result;

}

/**
 * Read tx baudrate from device config
 * @param param Not used
 * @return tx baudrate
 */
int trx_get_tx_baud(void *param)
{
//    int result = trx_read_conf(NULL);
//    if(!result)
//        return 0;

    int result = (int)TRX_CONFIG.tx_baud;
    return result;
}

/**
 * Sets baudrate for telecomand reception
 *
 * @param param RX Baurade 12=1200bps, 24=2400bps, 48=4800bps [48 default]
 * @return 1 - OK; 0 - Fail
 */
int trx_set_rx_baud(void *param)
{
    int result;
    int baud = *((int *)param);

    if(!((baud==12) || (baud==24) || (baud==48)))
        return 0;

    result = trx_read_conf(NULL);
    if(!result)
        return 0;

    TRX_CONFIG.rx_baud = (uint8_t)baud;

    // Save changes to status repo
    MemEEPROM_Vars mem_eeprom_var = mem_trx_rx_baud;
    writeIntEEPROM1(mem_eeprom_var, baud);

    // Save changes to device
    result = trx_set_conf(NULL);
    return result;
}

/**
 * Get rx baudrate from device config
 * @param param Not used
 * @return rx baudrate
 */
int trx_get_rx_baud(void *param)
{
//    int result = trx_read_conf(NULL);
//    if(!result)
//        return 0;

    int result = (int)TRX_CONFIG.rx_baud;
    return result;
}

/**
 * Sets beacon battery level. If battery is below this level, trx doesn't send
 * beacons
 * @param param *uint16_t Beacon battery level
 * @return 1 Ok, 0 Fail
 */
int trx_set_beacon_level(void* param)
{
    uint16_t beacon_level = *((uint16_t *)param);

    int result = trx_read_conf(NULL);
    if(!result)
        return 0;

    // Save changes to status repo
    MemEEPROM_Vars mem_eeprom_var = mem_trx_beacon_bat_lvl;
    writeIntEEPROM1(mem_eeprom_var, (int)beacon_level);

    // Save changes to device
    TRX_CONFIG.morse_bat_level = beacon_level;
    return trx_set_conf(NULL);
}

/**
 * Get beacon level from device config
 * @param param Not used
 * @return beacon level
 */
int trx_get_beacon_level(void *param)
{
//    int result = trx_read_conf(NULL);
//    if(!result)
//        return 0;

    int result = (int)TRX_CONFIG.morse_bat_level;
    return result;
}

/**
 * Sets beacon period in seconds.
 * @param param *uint16_t Beacon period in seconds
 * @return 1 Ok, 0 Fail
 */
int trx_set_beacon_period(void* param)
{
    uint16_t beacon_period = *((uint16_t *)param);

    int result = trx_read_conf(NULL);
    if(!result)
        return 0;

    // Save changes to status repo
    MemEEPROM_Vars mem_eeprom_var = mem_trx_beacon_period;
    writeIntEEPROM1(mem_eeprom_var, (int)beacon_period);

    // Save changes to device
    TRX_CONFIG.morse_inter_delay = beacon_period;
    return trx_set_conf(NULL);
}

/**
 * Get beacon period from device configuration
 * @param param Not used
 * @return beacon period
 */
int trx_get_beacon_period(void *param)
{
    int result;

//    result = trx_read_conf(NULL);
//    if(!result)
//        return 0;

    result = (int)TRX_CONFIG.morse_inter_delay;
    return result;
}



/* TRX commands */

/**
 * Upload current configuration into TRX
 * @param param Not used
 * @return 0 Fail, 1 OK
 */
int trx_set_conf(void *param)
{
    #if SCH_CMDTRX_VERBOSE >= 1
        printf("Uploading TRX configuration\n");
        com_print_conf(&TRX_CONFIG);
    #endif
    return com_set_conf(&TRX_CONFIG, NODE_COM, com_timeout);
}

/**
 * Get TRX current configuration and save to local variable
 *
 * @param param Not used
 * @return 1 - OK, 0 - Fail
 */
int trx_read_conf(void *param)
{
    int result;
    result = com_get_conf(&TRX_CONFIG, NODE_COM, com_timeout);

    #if (SCH_CMDTRX_VERBOSE>=1)
        printf("Current TRX configuration\n");
        com_print_conf(&TRX_CONFIG);
    #endif

    return result;
}

/**
 * Set the standart beacon: 00SUCHAI00
 * @sa tcm_send_beacon()
 *
 * @param param (0)SUCHAI beacon. (1)Test beacon 1. (2)Test beacon 2.
 * @return 1 - OK
 */
int trx_set_beacon(void *param)
{
    char stdbeacon[COM_MORSE_LEN] = "00SUCHAI00";

    switch (*(int *)param)
    {
        case 0:
            //USe default beacon
            break;
        case 1:
            strcpy(stdbeacon, "0123456789");
            break;

        case 2:
            strcpy(stdbeacon, "0000000000");
            break;

        default:
            strcpy(stdbeacon, "");
            break;
    }

    #if SCH_CMDTRX_VERBOSE > 2
        printf("Setting beacon: %s\n", stdbeacon);
    #endif

    int len = strlen(stdbeacon) + 1;
    if(len > COM_MORSE_LEN)
        return 0;

    memcpy(TRX_CONFIG.morse_text, stdbeacon, len);
    int result = trx_set_conf(NULL);

    return result;
}

/**
 * Send a frame for testing (ping) to desired node
 *
 * @param param int Node to transmit
 * @return 1 - OK
 */
int trx_ping(void *param)
{
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
 * Read and show TRX status. Debug only
 *
 * @param param Not used
 * @return 1 - OK, 0 - Fail
 */
int trx_getstatus(void *param)
{
    nanocom_data_t status;
    int result;

    result = com_get_status(&status, NODE_COM, com_timeout);

#if SCH_CMDTRX_VERBOSE
    com_printf_status(&status);
#endif

    return result;
}

/**
 * Initializes TRX main parameters keeping modifications
 * 
 * @param param Is deployed?
 * @return 1 - OK; 0 - Fail
 */
int trx_initialize(void *param)
{
    int deployed;
    if(param)
        deployed = *((int *)param);
    else
        deployed = 0;

    // Reading settings from status repo
    MemEEPROM_Vars mem_eeprom_var;
    mem_eeprom_var = mem_trx_beacon_period;
    uint16_t morse_inter_delay = (uint16_t)readIntEEPROM1(mem_eeprom_var);
    mem_eeprom_var = mem_trx_beacon_bat_lvl;
    uint16_t morse_bat_level = (uint16_t)readIntEEPROM1(mem_eeprom_var);
    mem_eeprom_var = mem_trx_rx_baud;
    uint8_t rx_baud = (uint8_t)readIntEEPROM1(mem_eeprom_var);
    mem_eeprom_var = mem_trx_tx_baud;
    uint8_t tx_baud = (uint8_t)readIntEEPROM1(mem_eeprom_var);

    // Generating trx settings
    TRX_CONFIG.do_random = 1;
    TRX_CONFIG.do_rs = 1;
    TRX_CONFIG.do_viterbi = 1;
    TRX_CONFIG.hk_interval = 5;
    TRX_CONFIG.morse_bat_level = morse_bat_level;
    TRX_CONFIG.morse_cycle = 1;
    TRX_CONFIG.morse_en_rf_err = 1;
    TRX_CONFIG.morse_en_rssi = 1;
    TRX_CONFIG.morse_en_rx_count = 1;
    TRX_CONFIG.morse_en_temp_a = 1;
    TRX_CONFIG.morse_en_temp_b = 1;
    TRX_CONFIG.morse_en_tx_count = 1;
    TRX_CONFIG.morse_en_voltage = 1;
    TRX_CONFIG.morse_enable = 1;
    TRX_CONFIG.morse_inter_delay = morse_inter_delay;
    TRX_CONFIG.morse_mode = SCH_TRX_BEACON_MODE;
    TRX_CONFIG.morse_pospone = SCH_TRX_BEACON_POSPONE_NOM;
//    TRX_CONFIG.morse_text = "00SUCHAI00"; //Use command
    TRX_CONFIG.morse_wpm = SCH_TRX_BEACON_WPM;
    TRX_CONFIG.preamble_length = 75;
    TRX_CONFIG.rx_baud = rx_baud;
    TRX_CONFIG.tx_baud = tx_baud;
    TRX_CONFIG.tx_max_temp = 60;

    if(!deployed)
    {
        TRX_CONFIG.morse_pospone = SCH_TRX_BEACON_POSPONE_PRE;
    }

    /* Save configuration to TRX */
    int beacon = 0; // Set suchai beacon
    int result = trx_set_beacon((void *)(&beacon)); //Also call trx_set_conf

    return result;
}

/**
 * Organize temletry data in frames and load this frames in TRX. Each telemetry
 * frame has the following format.
 * @code
 *          |--- Control fields ---||----        Data fields        ---|
 *           __________________________________________________________
 *          | Type (2) | Frame# (2)|| TM-ID(2) |       DATA(LEN)      ||
 *          |__________|___________||__________||INT16|___...___|INT16||
 * @endcode
 *
 *
 * @param data Pointer to data that will be append
 * @param len Lenght of the data array
 * @param mode Mode of append  CMD_ADDFRAME_START -> Start new frame
 *                              CMD_ADDFRAME_CONT  -> Inter frame
 *                              CMD_ADDFRAME_STOP  -> Stop frame
 *                              CMD_ADDFRAME_FIN   -> Finish current session
 *                              CMD_ADDFRAME_ADD   -> Add new data
 *                              CMD_ADDFRAME_SINGL -> Create a single frame
 *
 * @return 0, TX Fail. 1, TX Success. 2, No TX operation performe
 */
int trx_tm_addtoframe(int *data, int len, int mode)
{
    static uint16_t tmframe[TRX_TMFRAMELEN16];
    static int int16_counter = 0;
    static int frame_counter = 0;
    static int tm_type = CMD_STOP;
    int send_stat = 2;

    while(mode != CMD_ADDFRAME_EXIT)
    {
        switch (mode)
        {
            /* A single frame that contains max TRX_TMFRAMELEN bytes of data */
            case CMD_ADDFRAME_SINGL:
                /* A new frame being configured */
                int16_counter = 0;
                /* Append control field  */
                tmframe[int16_counter++] = CMD_TMFRAME_TSINGL;   /* Type (1) */
                tmframe[int16_counter++] = (uint16_t)(frame_counter);     /* Frame# (2) */

                /* Adding data to the current frame. Max TRX_TMFRAMELEN bytes */
                while((len > 0) && (int16_counter < TRX_TMFRAMELEN16))
                {
                    tmframe[int16_counter++] = (uint16_t)(*data);     /* Data */
                    data++;
                    len--;
                }

                /* Loading frame to TRX and exiting */
                for(;int16_counter<TRX_TMFRAMELEN16;int16_counter++)
                {
                    tmframe[int16_counter] = (uint16_t)CMD_STOP;
                }

                /* Load and transmit TM */
                trx_tm_send(tmframe, TRX_TMFRAMELEN16);
                
                frame_counter++;
                int16_counter = 0;
                mode = CMD_ADDFRAME_EXIT;
                break;

            /* A new start frame for more than TRX_TMFRAMELEN bytes, so we need
             * to load several frames cotaining all the telemetry */
            case CMD_ADDFRAME_START:
                /* A new frame being configured */
                /* Load all remaining frames before */
                if(int16_counter > 0)
                {
                    for(;int16_counter<TRX_TMFRAMELEN16;int16_counter++)
                    {
                        tmframe[int16_counter] = (uint16_t)CMD_STOP;
                    }
                    
                    /* Load and transmit TM */
                    trx_tm_send(tmframe, TRX_TMFRAMELEN16);
                }

                int16_counter = 0;
                /* Append control field  */
                tmframe[int16_counter++] = CMD_TMFRAME_TSTART;   /* Type (2) */
                tmframe[int16_counter++] = (uint16_t)(frame_counter);     /* Frame# (2) */

                frame_counter++;
                
                /* Adding current tm type */
                if(len > 0)
                {
                    tm_type = *data;
                    tmframe[int16_counter++] = (uint16_t)(tm_type);     /* DataL */

                    /* Update the remaining data counter */
                    data++;
                    len--;
                }

                mode = CMD_ADDFRAME_ADD;
                break;

            case CMD_ADDFRAME_CONT:
                /* A new frame being configured */
                int16_counter = 0;
                /* Append control field  */
                tmframe[int16_counter++] = CMD_TMFRAME_TCONT;   /* Type (2) */
                tmframe[int16_counter++] = (char)(frame_counter);     /* Frame# (2) */

                /* Add tm type */
                tmframe[int16_counter++] = (uint16_t)(tm_type);     /* DataL */

                /* Add data if needed */
                frame_counter++;
                mode = CMD_ADDFRAME_ADD;
                break;

            /* The last frame with the current telemetry */
            case CMD_ADDFRAME_STOP:
                /* Load all remaining frames before */
                if(int16_counter > 0)
                {
                    for(;int16_counter<TRX_TMFRAMELEN16;int16_counter++)
                    {
                        tmframe[int16_counter] = (uint16_t)CMD_STOP;
                    }

                    /* Load and transmit TM */
                    trx_tm_send(tmframe, TRX_TMFRAMELEN16);
                }

                /* A new frame stop being configured */
                int16_counter = 0;
                /* Append control field */
                tmframe[int16_counter++] = CMD_TMFRAME_TSTOP;   /* Type (2) */
                tmframe[int16_counter++] = (uint16_t)(frame_counter);     /* Frame# (2) */

                /* Add data to this frame */
                frame_counter++;
                mode = CMD_ADDFRAME_ADD;
                break;

            /* Load to TRX all reamaining frames and reset the control field
             * counters to start a new session. No new data will be append */
            case CMD_ADDFRAME_FIN:
                /* Load to trx the remaining frames */
                if(int16_counter > 0)
                {
                    for(;int16_counter<TRX_TMFRAMELEN16;int16_counter++)
                    {
                        tmframe[int16_counter] = (uint16_t)CMD_STOP;
                    }

                    /* Load and transmit TM */
                    trx_tm_send(tmframe, TRX_TMFRAMELEN16);
                }

                int16_counter = 0;
                frame_counter = 0;
                tm_type = CMD_STOP;
                mode = CMD_ADDFRAME_EXIT;
                break;

            /* Append all the requiered data to the current frame or adds more
             * CONT frame if needed */
            case CMD_ADDFRAME_ADD:
                if(len > 0)
                {
                    /* Current frame is not initilized */
                    if(int16_counter == 0 )
                    {
                        mode = CMD_ADDFRAME_EXIT;
                    }
                    /* Current frame is not full */
                    else if(int16_counter < TRX_TMFRAMELEN16)
                    {
                        /* Adding data to the current frame*/
                        tmframe[int16_counter++] = (uint16_t)(*data);     /* DataL */
                        /* Update the remaining data counter */
                        data++;
                        len--;
                        mode = CMD_ADDFRAME_ADD;
                    }
                    else /* Current frame is full */
                    {
                        /* This frame is ready to be loaded into TRX buffer */
                        /* First fill tmframe's unused fields */
                        for(;int16_counter<TRX_TMFRAMELEN16;int16_counter++)
                        {
                            tmframe[int16_counter] = (uint16_t)CMD_STOP;
                        }

                        /* Load and transmit TM */
                        trx_tm_send(tmframe, TRX_TMFRAMELEN16);

                        /* Keep adding data in a new frame */
                        int16_counter = 0;
                        mode = CMD_ADDFRAME_CONT;
                    }
                }
                else /* No more data to load */
                {
                    /* Exit */
                    mode = CMD_ADDFRAME_EXIT;
                }
                break;

            /* Invalid state */
            default:
                mode = CMD_ADDFRAME_EXIT;
                break;
        }
    }
    //return frame_counter;

    return send_stat;
}

/**
 * Auxiliary function. Send a frame by TRX
 * 
 * @param data Pointer to data buffer (**int16 buffer**)
 * @param len Lenght of **int16** buffer to transmit
 * @return 1 Ok, 0 fail.
 */
static int trx_tm_send(uint16_t *data, int len)
{
#if SCH_CMDTRX_VERBOSE > 1
    printf("Sending TM frame...\n");
#endif

    int result;
    result = csp_transaction(CSP_PRIO_NORM, SCH_TRX_NODE_GND, SCH_TRX_PORT_TM,
                             com_timeout, (void *)data, len*2, NULL, 0);

    return result;
}