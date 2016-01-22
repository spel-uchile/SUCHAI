#include "cmdTCM.h"
#include "csp.h"
#include "cmdPayload.h"



cmdFunction tcmFunction[TCM_NCMD];
int tcm_sysReq[TCM_NCMD];

char beacon_buff[COM_MORSE_LEN];
char *p_beacon_buff = beacon_buff+9;//+strlen(beacon_buff);

extern nanocom_conf_t TRX_CONFIG; /** Global configuration var from cmdTRX.c*/

void tcm_onResetCmdTCM(void){
    printf("        tcm_onResetCmdTCM\n");

    int i;
    for(i=0; i<TCM_NCMD; i++) tcm_sysReq[i] = CMD_SYSREQ_MIN+SCH_TCTM_SYS_REQ;

    tcmFunction[(unsigned char)tcm_id_testframe] = tcm_testframe;
    tcmFunction[(unsigned char)tcm_id_resend] = tcm_resend;

    //sendTM Payload
    tcmFunction[(unsigned char)tcm_id_sendTM_all_pay_i] = tcm_sendTM_all_pay_i;
    tcmFunction[(unsigned char)tcm_id_sendTM_pay_i] = tcm_sendTM_pay_i;
    tcmFunction[(unsigned char)tcm_id_sendTM_battery] = tcm_sendTM_battery;

    //dat_stateVar
    tcmFunction[(unsigned char)tcm_id_sendTM_tmEstado] = tcm_sendTM_tmEstado;

    //Beacon
    tcmFunction[(unsigned char)tcm_id_update_beacon] = tcm_update_beacon;
    tcm_sysReq[(unsigned char)tcm_id_update_beacon]  = CMD_SYSREQ_MIN+SCH_BCN_SYS_REQ;

    tcmFunction[(unsigned char)tcm_id_set_sysreq] = tcm_set_sysreq;
    tcm_sysReq[(unsigned char)tcm_id_set_sysreq] = CMD_SYSREQ_MIN;
}

/**
 * Send an message for testing
 *
 * @param param (1)Verbose, (0)No Verbose
 * @return 1 - OK
 */
int tcm_testframe(void *param)
{
    int result;
    char *testframe = "Proyecto SUCHAI - suchai@ing.uchile.cl";
    //unsigned char testframe[80] = "Proyecto SUCHAI - suchai@ing.uchile.cl\n\0Proyecto SUCHAI - suchai@ing.uchile.cl\n\0";

    if(*(int *)param)
    {
        printf("Sending test frame: %s\n", testframe);
    }

    result = csp_transaction(CSP_PRIO_NORM, SCH_TRX_NODE_GND, SCH_TRX_PORT_TM,
                             2000, (void *)testframe, strlen(testframe), NULL, 0);
    return result;
}

/**
 * Resend the telemetry buffer
 *
 * @param param (1)Verbose, (0)No Verbose
 * @return 1 - OK; 0 - Fail
 * @deprecated
 */
int tcm_resend(void *param)
{
#if SCH_CMDTRX_VERBOSE
    printf("[DEPRECATED]\n");
#endif
    int result = 0;

//    /* Get position of last sent data */
//    unsigned char outl = TRX_ReadRegister(TRX_TMTF_OUT_L);
//    unsigned char outh = TRX_ReadRegister(TRX_TMTF_OUT_H);
//
//    /* Get position of last buffered data*/
//    unsigned char inl = TRX_ReadRegister(TRX_TMTF_IN_L);
//    unsigned char inh = TRX_ReadRegister(TRX_TMTF_IN_H);
//
//    /* Resend only if all buffered data was sent */
//    if((outl == inl) && (outh == inh))
//    {
//        /* Reset position of last telemtry */
//        if(*(int *)param) con_printf("Reseting buffer pointer...");
//        TRX_WriteRegister(TRX_TMTF_OUT_H, 0);
//        TRX_WriteRegister(TRX_TMTF_OUT_L, 0);
//
//        /* Now resend the telemetry buffer */
//        if(*(int *)param) con_printf("Resending telemetry buffer...");
//        result = TRX_SendTelemetry();
//    }

    return result;
}
/**
 * Envia la TM de TODOS los DAT_Payload pay_i usado como arumento (param)
 * Toma el buffer de dat_get_PayloadVar(pay_i, ..) y la transmite por el TRX
 *
 * @param param no usado
 * @return 1 succes, 0 failure
 */
int tcm_sendTM_all_pay_i(void *param){

    DAT_Payload_Buff pay_i; PAY_xxx_State pay_state;
    for(pay_i=0; pay_i<dat_pay_last_one; pay_i++)
    {
        // Do not send TM if payload is INACTIVE
        pay_state = pay_get_state(pay_i);
        if(pay_state == pay_xxx_state_inactive){continue;}

        // Send TM
        printf("sending pay_i = %s \r\n", dat_get_payload_name(pay_i));
        tcm_sendTM_pay_i( (void *)(&pay_i) );
    }

    return 1;
}
/**
 * Envia la TM de un DAT_Payload pay_i usado como arumento (param)
 * Toma el buffer de dat_get_PayloadVar(pay_i, ..) y la transmite por el TRX
 *
 * @param param puntero a DAT_Payload con el pay_i del cual enviar su buffer
 * @return 1 succes, 0 failure
 */
int tcm_sendTM_pay_i(void *param){
    printf("tcm_sendTM_pay_i ..\r\n");
    
    DAT_Payload_Buff pay_i = *((DAT_Payload_Buff*)param);
    int mode=2;

    //send pay_i data, regardless of it's pay_i_state
    int res = tcm_sendTM_payload(mode, pay_i);

    //If successfull reinit payloads that were in waiting_tx state
    if(res!=0x0000){
        PAY_xxx_State pay_state;
        pay_state = pay_get_state(pay_i);
        if(pay_state == pay_xxx_state_waiting_tx ){
            pay_set_state(pay_i, pay_xxx_state_active);
        }        
    }
    return res;
}

int tcm_sendTM_battery(void *param){
    printf("tcm_sendTM_battery ..\r\n");
    //param minutos de descarga del payload battery

    int mode = 2;
    int aux = *(int*)param;
    int time = aux;
    //int time=(75*aux*60); /*descarga de 75 muestras por segundo por tiempo de descarga en segundos*/
    //75=velocidad de descarga:tamaño muestras=1200 bit/s : 16bits //

    int res = tcm_sendTM_payload_battery(mode,time);

    //If successfull reinit payloads that were in waiting_tx state
    if(res!=0x0000){
        PAY_xxx_State pay_state;
        pay_state = pay_get_state(dat_pay_battery);
        if(pay_state == pay_xxx_state_waiting_tx ){
            pay_set_state(dat_pay_battery, pay_xxx_state_active);
        }
    }

    printf("fin tcm_sendtm_battery");
    return res;
}


/**
 * Reads and transmit telemetry ralated to satellite's status
 *
 * Envia telemetria instantanea (en ese momento) del estado del SUCHAI.
 * Envia por TM todas las variables de estado de stateVar
 *
 * TMID : 0x0009
 *
 * @param param 0 - Only store, 1 - Only Send, 2 - Store and send @deprecated @sa trx_tm_addtoframe()
 * @return 0 (Tx fail) - 1 (Tx OK)
 */
int tcm_sendTM_tmEstado(void *param)
{
    int tm_id = 0x0009;

    /* Start a new session (Single or normal) */
    tm_id = dat_pay_tmEstado; /* TM ID */ 
    trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);   /* Close previos sessions */
    trx_tm_addtoframe(&tm_id, 1, CMD_ADDFRAME_START); /* New empty start frame */

    /* Read info and append to the frame */
    STA_BusStateVar indxVar;
    for(indxVar=0; indxVar<sta_busStateVar_last_one; indxVar++)
    {
        tm_id = sta_get_BusStateVar(indxVar);
        trx_tm_addtoframe(&tm_id, 1, CMD_ADDFRAME_ADD);
    }

    // Close session
    tm_id = trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);      /* End session */

    return tm_id;
}

/**
 * Este comando actualiza la info del el beacon del satelite. Dependiendo del
 * parametro, construye diferentes beacons que contienen el identificador del
 * proyecto e informacion base del sistema.
 *
 * @note Para una descripcion detallada de cada beacon, revisar la planilla en
 * linea.
 * https://docs.google.com/spreadsheet/ccc?key=0AlJNKX_r8AXcdFZwNURWdmZUVjNtM1RpdTlTRG9LV0E#gid=6
 *
 * @param param Tipo de beacon a construir y enviar.
 *          0 - Simple
 *          1 - OBC
 *          2 - Estado hardware
 *          3 - Estado TRX
 *          4 - Estado EPS
 *
 * @return 0-Fallo, 1-Exito
 * TODO: Actualizar campos para calzar con nuevo tamano de beacon
 */
int tcm_update_beacon(void *param)
{
    int mode = *((int *)param);
    int ok = 0;
    int val = -1;
    static char buff[10];

    memset(beacon_buff, '\0', COM_MORSE_LEN);
    strcpy(beacon_buff, "00SUCHAI0");
    char *p_buff = beacon_buff + strlen(beacon_buff);

    //SIMPLE
    if(mode==0)
    {
        /* Tipo */
        itoa(buff, mode, 10);
        strcpy(p_buff++, buff);
        ok = 1;
    }

    //OBC
    else if(mode == 1)
    {
        /* Tipo */
        itoa(buff, mode, 10);
        strcpy(p_buff++, buff);

        /* hoursWithoutReset */
        val = sta_get_BusStateVar(sta_ppc_hoursWithoutReset);
        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);
        itoa(buff,val%10,10);
        strcpy(p_buff++, buff);

        /* resetCounter */
        val = sta_get_BusStateVar(sta_ppc_resetCounter);
        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);
        itoa(buff,val%10,10);
        strcpy(p_buff++, buff);

        /* Last reset source */
        val = sta_get_BusStateVar(sta_ppc_lastResetSource);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* ant_deployed */
        val = sta_get_BusStateVar(sta_dep_ant_deployed);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* opMode */
        val = sta_get_BusStateVar(sta_ppc_opMode);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* ppc_osc */
        val = sta_get_BusStateVar(sta_ppc_osc);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* ppc_osc */
        val = sta_get_BusStateVar(sta_ppc_wdt);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);
        
        ok = 1;
    }

    //HARDWARE
    else if(mode == 2)
    {
        /* Tipo */
        itoa(buff, mode, 10);
        strcpy(p_buff++, buff);

        val = sta_get_BusStateVar(sta_RTC_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_BusStateVar(sta_TRX_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_BusStateVar(sta_EPS_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_BusStateVar(sta_MemEEPROM_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_BusStateVar(sta_MemSD_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_PayStateVar(sta_pay_lagmuirProbe_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_PayStateVar(sta_pay_sensTemp_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_PayStateVar(sta_pay_gps_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        val = sta_get_PayStateVar(sta_pay_camera_isAlive);
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        ok = 1;
    }

    //TRX
    else if(mode == 3)
    {
        /* Tipo */
        itoa(buff, mode, 10);
        strcpy(p_buff++, buff);

        /* count_tm */
        #if (SCH_TRX_ONBOARD==1)
            val = sta_get_BusStateVar(sta_trx_count_tm);
        #else
            val = 0;
        #endif

        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);
        itoa(buff,val%10,10);
        strcpy(p_buff++, buff);

        /* count_tc */
        #if (SCH_TRX_ONBOARD==1)
            val = sta_get_BusStateVar(sta_trx_count_tc);
        #else
            val = 0;
        #endif

        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);
        itoa(buff,val%10,10);
        strcpy(p_buff++, buff);

        /* tx baud */
        val = TRX_CONFIG.tx_baud;
        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);

        /* rx baud */
        val = TRX_CONFIG.rx_baud;
        itoa(buff,val/10,10);
        strcpy(p_buff++, buff);

        /* do rs */
        val = TRX_CONFIG.do_rs;
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* do random */
        val = TRX_CONFIG.do_random;
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        /* do viterbi */
        val = TRX_CONFIG.do_viterbi;
        itoa(buff,val,10);
        strcpy(p_buff++, buff);

        ok = 1;
    }

    else
    {
        ok = 0;
    }

    
    ok = trx_read_conf(NULL);
    
#if SCH_CMDTCM_VERBOSE
    printf(">>Beacon: %s  (%d)\n\r", beacon_buff, strlen(beacon_buff));
    if(!ok)
    {
        printf("Unable to read TRX config! \n\r");
        return 0;
    }
#endif

    memcpy(TRX_CONFIG.morse_text, beacon_buff, COM_MORSE_LEN);
    ok = trx_set_conf(NULL);

    return ok;
}

/**
 * Debug command that set the SysReq field to all TCM commands. If -1 is used
 * as argument then default values are restored.
 *
 * @param param new SysReq value. Use -1 to restore default values
 * @return 1 success.
 */
int tcm_set_sysreq(void *param)
{
    int new_sysreq = *((int *)param);
    int i = 0;

    // Resto to default values
    if(new_sysreq < 0)
    {
        //Default
        for(i=0; i<TCM_NCMD; i++) tcm_sysReq[i] = CMD_SYSREQ_MIN+SCH_TCTM_SYS_REQ;

        //Special cases
        tcm_sysReq[(unsigned char)tcm_id_update_beacon]  = CMD_SYSREQ_MIN+SCH_BCN_SYS_REQ;
        tcm_sysReq[(unsigned char)tcm_id_set_sysreq] = CMD_SYSREQ_MIN;
    }
    //Set new value
    else
    {
        for(i=0; i<TCM_NCMD; i++)
        {
            tcm_sysReq[i] = new_sysreq;
        }

        //Do not change the sysReq of this command!
        tcm_sysReq[(unsigned char)tcm_id_set_sysreq] = CMD_SYSREQ_MIN;
    }

    return 1;
}

/* Funciones auxiliares */

/**
 * Reads and transmit telemetry ralated to payloads
 * @note param mode is deprecated
 * @param mode 0 - Only store, 1 - Only Send, 2 - Store and send @deprecated @sa trx_tm_addtoframe()
 * @param pay_i Payload id
 * @return 0 (Tx fail) - 1 (Tx OK)
 */
int tcm_sendTM_payload(int mode, DAT_Payload_Buff pay_i){
    printf("tcm_sendTM_payload ..\r\n");

    int tm_id, nfrm;

    /* Start a new session (Single or normal) */
    tm_id = (int)pay_i; /* TM ID */
    nfrm = trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);   /* Close previos sessions */
    nfrm = trx_tm_addtoframe(&tm_id, 1, CMD_ADDFRAME_START); /* New empty start frame */

    /* Read info and append to the frame */

    //Add pay_i metadata
    //unsigned int maxIndx = dat_get_MaxPayIndx( pay_i);
    unsigned int nextIndx = dat_get_NextPayIndx(pay_i);
    int pay_i_state = pay_get_state(pay_i);

    //nfrm = trx_tm_addtoframe( (int *)&maxIndx, 1, CMD_ADDFRAME_ADD);
    nfrm = trx_tm_addtoframe( (int *)&nextIndx, 1, CMD_ADDFRAME_ADD);
    nfrm = trx_tm_addtoframe( (int *)&pay_i_state, 1, CMD_ADDFRAME_ADD);

    #if (SCH_CMDTCM_VERBOSE>=1)
        printf("    pay_i = %s, pay_i_state = %d, nextIndx = %u \r\n", dat_get_payload_name(pay_i), pay_i_state, nextIndx);
    #endif

    //Add pay_i data
    unsigned int indx; int val;
    for(indx=0; indx<nextIndx; indx++)
    {
        dat_get_Payload_Buff(pay_i, indx, &val);
        nfrm = trx_tm_addtoframe(&val, 1, CMD_ADDFRAME_ADD);

        #if (SCH_CMDTCM_VERBOSE>=1)
            printf("    pay_i= %02d = %s | indx=%05u | &val=0x%04X | &val=%05d \r\n", pay_i, dat_get_payload_name(pay_i), indx, val, val);
        #endif

        ClrWdt();
    }

    /* Close session */
    // data = trx_tm_addtoframe(&data, 0, CMD_ADDFRAME_STOP);     /* Empty stop frame */
    //nfrm = trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);      /* End session */
    trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);      /* End session */

    return nfrm;
}

int tcm_sendTM_payload_battery(int mode, int num_samples)
{
    printf("tcm_sendTM_payload_battery ..\r\n");

    int tm_id, nfrm;

    /* Start a new session (Single or normal) */
    tm_id = (int)dat_pay_battery; /* TM ID */
    nfrm = trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);   /* Close previos sessions */
    nfrm = trx_tm_addtoframe(&tm_id, 1, CMD_ADDFRAME_START); /* New empty start frame */

    /* Read info and append to the frame */

    //Add pay_i metadata
    //unsigned int maxIndx = dat_get_MaxPayIndx( pay_i);
    unsigned int nextIndx = dat_get_NextPayIndx(dat_pay_battery); //valor de la  ultima posicion
    int pay_battery_state = pay_get_state(dat_pay_battery);

    //nfrm = trx_tm_addtoframe( (int *)&maxIndx, 1, CMD_ADDFRAME_ADD);
    nfrm = trx_tm_addtoframe( (int *)&nextIndx, 1, CMD_ADDFRAME_ADD);
    nfrm = trx_tm_addtoframe( (int *)&pay_battery_state, 1, CMD_ADDFRAME_ADD);

    #if (SCH_CMDTCM_VERBOSE>=1)
        printf("    pay_i = %d, pay_i_state = %d, nextIndx = %u \r\n", (unsigned int)dat_pay_battery, pay_battery_state, nextIndx);
    #endif

    unsigned int indx=0; //indice
    unsigned int set_indx;
    int valNULL = 0; // varable int con valor cero
    printf("indx= %i , nextIndx = %i, ", indx , nextIndx);
    int val = 0;
    printf("val=%d \r\n", val);

    while(val==0)
    {
        printf("en while.. val = %d \r\n", val);
        dat_get_Payload_Buff(dat_pay_battery, indx, &val);
        indx++;
        ClrWdt();
    }

    printf("termina iteracion while \r\n");
    int next_indx = indx;
    printf("next_indx=%d \r\n", indx);

    for(indx=0; indx<num_samples; indx++)
    {
        set_indx = (next_indx + indx)-1;
        dat_get_Payload_Buff(dat_pay_battery, set_indx , &val);
        nfrm = trx_tm_addtoframe(&val, 1, CMD_ADDFRAME_ADD);
        printf("set_indx = %i \r\n",set_indx);

        #if (SCH_CMDTCM_VERBOSE>=1)
            printf("    dat_get_Payload_Buff(pay_i=%d, indx=%u, val=%d) \r\n", dat_pay_battery, indx, val);
        #endif

        // aca va una funcion que guarde NULL en la posicion indx del lugar donde esta guardado los valores de la bateria
        //converse con tomas y esta funcion la hara el


        dat_set_Payload_Buff_at_indx(dat_pay_battery, valNULL, set_indx);
        //dat_get_Payload_Buff(dat_pay_battery, set_indx , &val);
        //printf("    dat_get_Payload_Buff(indx=%u, val=%d) \r\n", indx, val);
        ClrWdt();

    }

    /* Close session */
    // data = trx_tm_addtoframe(&data, 0, CMD_ADDFRAME_STOP);     /* Empty stop frame */
    //nfrm = trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);      /* End session */
    trx_tm_addtoframe(&tm_id, 0, CMD_ADDFRAME_FIN);      /* End session */
    printf("fin tcm_sendTM_payload_battery\n");

    return nfrm;
}
