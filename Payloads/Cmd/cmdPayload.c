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

#include "cmdPayload.h"
#include "taskFlightPlan2.h"


cmdFunction payFunction[PAY_NCMD];
int pay_sysReq[PAY_NCMD];

#define _VERBOSE_ 1
#define TEST2_MSJS 1

// 1 = time, samples, time, samples, .. | 0 = time, samples, samples, ..
#define PAY_TSPAIR_nSLIST   (1)

void pay_onResetCmdPAY(void){
    printf("        pay_onResetCmdPAY\n");

    int i;
    for(i=0; i<PAY_NCMD; i++) pay_sysReq[i] = CMD_SYSREQ_MIN;

    payFunction[(unsigned char)pay_id_test_dataRepo] = pay_test_dataRepo;
    payFunction[(unsigned char)pay_id_fp2_default_fsm] = pay_fp2_default_fsm;

    payFunction[(unsigned char)pay_id_isAlive_tmEstado] = pay_isAlive_tmEstado;
    payFunction[(unsigned char)pay_id_get_state_tmEstado] = pay_get_state_tmEstado;
    payFunction[(unsigned char)pay_id_set_state_tmEstado] = pay_set_state_tmEstado;
    payFunction[(unsigned char)pay_id_init_tmEstado] = pay_init_tmEstado;
    payFunction[(unsigned char)pay_id_take_tmEstado] = pay_take_tmEstado;
    payFunction[(unsigned char)pay_id_stop_tmEstado] = pay_stop_tmEstado;

    payFunction[(unsigned char)pay_id_isAlive_sensTemp] = pay_isAlive_sensTemp;
    payFunction[(unsigned char)pay_id_get_state_sensTemp] = pay_get_state_sensTemp;
    payFunction[(unsigned char)pay_id_set_state_sensTemp] = pay_set_state_sensTemp;
    payFunction[(unsigned char)pay_id_init_sensTemp] = pay_init_sensTemp;
    payFunction[(unsigned char)pay_id_take_sensTemp] = pay_take_sensTemp;
    payFunction[(unsigned char)pay_id_stop_sensTemp] = pay_stop_sensTemp;
    payFunction[(unsigned char)pay_id_debug_sensTemp] = pay_debug_sensTemp;

    payFunction[(unsigned char)pay_id_isAlive_gyro] = pay_isAlive_gyro;
    payFunction[(unsigned char)pay_id_get_state_gyro] = pay_get_state_gyro;
    payFunction[(unsigned char)pay_id_set_state_gyro] = pay_set_state_gyro;
    payFunction[(unsigned char)pay_id_init_gyro] = pay_init_gyro;
    payFunction[(unsigned char)pay_id_take_gyro] = pay_take_gyro;
    payFunction[(unsigned char)pay_id_stop_gyro] = pay_stop_gyro;
    payFunction[(unsigned char)pay_id_debug_gyro] = pay_debug_gyro;

    payFunction[(unsigned char)pay_id_isAlive_camera] = pay_isAlive_camera;
    payFunction[(unsigned char)pay_id_get_state_camera] = pay_get_state_camera;
    payFunction[(unsigned char)pay_id_set_state_camera] = pay_set_state_camera;
    payFunction[(unsigned char)pay_id_init_camera] = pay_init_camera;
    payFunction[(unsigned char)pay_id_take_camera] = pay_take_camera;
    payFunction[(unsigned char)pay_id_stop_camera] = pay_stop_camera;
    payFunction[(unsigned char)pay_id_takePhoto_camera] = pay_takePhoto_camera;
    payFunction[(unsigned char)pay_id_get_savedPhoto_camera] = pay_get_savedPhoto_camera;

    payFunction[(unsigned char)pay_id_get_state_gps] = pay_get_state_gps;
    payFunction[(unsigned char)pay_id_set_state_gps] = pay_set_state_gps;
    payFunction[(unsigned char)pay_id_isAlive_gps] = pay_isAlive_gps;
    pay_sysReq[(unsigned char)pay_id_isAlive_gps] =  CMD_SYSREQ_MIN + SCH_PAY_GPS_SYS_REQ;
    payFunction[(unsigned char)pay_id_init_gps] = pay_init_gps;
    pay_sysReq[(unsigned char)pay_id_init_gps] =  CMD_SYSREQ_MIN + SCH_PAY_GPS_SYS_REQ;
    payFunction[(unsigned char)pay_id_take_gps] = pay_take_gps;
    pay_sysReq[(unsigned char)pay_id_take_gps] =  CMD_SYSREQ_MIN + SCH_PAY_GPS_SYS_REQ;
    payFunction[(unsigned char)pay_id_stop_gps] = pay_stop_gps;
    pay_sysReq[(unsigned char)pay_id_stop_gps] =  CMD_SYSREQ_MIN + SCH_PAY_GPS_SYS_REQ;

    payFunction[(unsigned char)pay_id_gps_updateRTC] = pay_gps_updateRTC;
    pay_sysReq[(unsigned char)pay_id_gps_updateRTC] =  CMD_SYSREQ_MIN;
    payFunction[(unsigned char)pay_id_gps_serial] = pay_gps_serial;
    pay_sysReq[(unsigned char)pay_id_gps_serial] =  CMD_SYSREQ_MIN;
    payFunction[(unsigned char)pay_id_gps_senddn] = pay_gps_senddn;
    pay_sysReq[(unsigned char)pay_id_gps_senddn] =  CMD_SYSREQ_MIN;
    payFunction[(unsigned char)pay_id_gps_jmesg] = pay_gps_jmesg;
    pay_sysReq[(unsigned char)pay_id_gps_jmesg] =  CMD_SYSREQ_MIN;
    payFunction[(unsigned char)pay_id_gps_jsat] = pay_gps_jsat;
    pay_sysReq[(unsigned char)pay_id_gps_jsat] =  CMD_SYSREQ_MIN;

    payFunction[(unsigned char)pay_id_isAlive_expFis] = pay_isAlive_expFis;
    payFunction[(unsigned char)pay_id_get_state_expFis] = pay_get_state_expFis;
    payFunction[(unsigned char)pay_id_set_state_expFis] = pay_set_state_expFis;
    payFunction[(unsigned char)pay_id_init_expFis] = pay_init_expFis;
    payFunction[(unsigned char)pay_id_take_expFis] = pay_take_expFis;
    payFunction[(unsigned char)pay_id_stop_expFis] = pay_stop_expFis;
    payFunction[(unsigned char)pay_id_adhoc_expFis] = pay_adhoc_expFis;
    payFunction[(unsigned char)pay_id_testDAC_expFis] = pay_testDAC_expFis;
    payFunction[(unsigned char)pay_id_print_seed] = pay_print_seed;
    payFunction[(unsigned char)pay_id_testFreq_expFis] = pay_testFreq_expFis;
    
    payFunction[(unsigned char)pay_id_isAlive_battery] = pay_isAlive_battery;
    payFunction[(unsigned char)pay_id_get_state_battery] = pay_get_state_battery;
    payFunction[(unsigned char)pay_id_set_state_battery] = pay_set_state_battery;
    payFunction[(unsigned char)pay_id_init_battery] = pay_init_battery;
    payFunction[(unsigned char)pay_id_take_battery] = pay_take_battery;
    payFunction[(unsigned char)pay_id_stop_battery] = pay_stop_battery;
    payFunction[(unsigned char)pay_id_execute_experiment_battery] = pay_execute_experiment_battery;

    payFunction[(unsigned char)pay_id_isAlive_debug] = pay_isAlive_debug;
    payFunction[(unsigned char)pay_id_get_state_debug] = pay_get_state_debug;
    payFunction[(unsigned char)pay_id_set_state_debug] = pay_set_state_debug;
    payFunction[(unsigned char)pay_id_init_debug] = pay_init_debug;
    payFunction[(unsigned char)pay_id_take_debug] = pay_take_debug;
    payFunction[(unsigned char)pay_id_stop_debug] = pay_stop_debug;

    payFunction[(unsigned char)pay_id_isAlive_langmuirProbe] = pay_isAlive_langmuirProbe;
    payFunction[(unsigned char)pay_id_get_state_langmuirProbe] = pay_get_state_langmuirProbe;
    payFunction[(unsigned char)pay_id_set_state_langmuirProbe] = pay_set_state_langmuirProbe;
    payFunction[(unsigned char)pay_id_init_langmuirProbe] = pay_init_langmuirProbe;
    payFunction[(unsigned char)pay_id_take_langmuirProbe] = pay_take_langmuirProbe;
    payFunction[(unsigned char)pay_id_stop_langmuirProbe] = pay_stop_langmuirProbe;
    payFunction[(unsigned char)pay_id_adhoc_langmuirProbe] = pay_adhoc_langmuirProbe;
    payFunction[(unsigned char)pay_id_send_to_langmuirProbe] = pay_send_to_langmuirProbe;
}


/**
 *
 * @param param
 * @return
 */
int pay_fp2_default_fsm(void *param){
    //unsigned int tick_ms = *( (unsigned int *)param );
    //int value = *( (int*)param );
    //pay_fp2_multiplexing();
    pay_fp2_simultaneous();
    return 1;
}
//******************************************************************************
int pay_test_dataRepo(void *param){

    DAT_Payload_Buff pay_i=0;
    printf("=>FIFO buffer check:\r\n");
    unsigned int i, indx; int value=0xAA00, val; BOOL stat; char ret[10];
    for(i=0;i<0x000F;i++){
        indx = dat_get_NextPayIndx(pay_i);
        printf("indx = dat_getNextPayIndx(pay_i);\r\nindx = ");
        sprintf (ret, "0x%X", (unsigned int)indx);
        printf(ret); printf("\r\n");

        //escribo
        stat = dat_set_Payload_Buff(pay_i, value);
        printf("stat = dat_setPayloadVar( pay_i, value);\r\nstat =  ");
        if(stat==TRUE){ printf("TRUE\r\n"); }
        else{ printf("FALSE\nFin del test\r\n"); break; }

        //leo
        stat = dat_get_Payload_Buff( pay_i, indx, &val);
        printf("stat = dat_getPayloadVar( pay_i, indx, &val);\r\nstat = ");
        if(stat==TRUE){ printf("TRUE\r\n"); }
        else{ printf("FALSE\nFin del test\r\n"); break; }

        //Confirmo
        printf("value=");
        sprintf (ret, "0x%X", (unsigned int)value);
        printf(ret); printf("  |  ");
        printf("val=");
        //itoa(ret, (unsigned int)val, 16);
        sprintf (ret, "0x%X", (unsigned int)val);
        printf(ret); printf("\r\n");

    }

    return 1;
}

int pay_conf_expFis(void *param){

    unsigned int adcPeriod = *((unsigned int *) param);

    int rounds = 1;    //number of iterations done for each ADC_period value
        //debug Info
    #if FIS_CMD_VERBOSE > 0
        printf("    rounds = %u\n", rounds);
        printf("    adc period = %u\n", adcPeriod);
    #endif
    //configure Payload
    if (!(fis_iterate_config(adcPeriod, rounds) == FIS_STATE_READY)) {
        return 0;
    }
    printf("    expFis is READY!\n");

    /* Reset Payload
     * This erases all previous contents in Buffer
     */
    DAT_Payload_Buff pay_i;
    pay_i = dat_pay_expFis;
    dat_reset_Payload_Buff(pay_i);

    int res = 1;    //always alive

    //debug Info
    #if FIS_CMD_VERBOSE > 0
        printf("  sta_pay_expFis_isAlive = %d \r\n", sta_get_PayStateVar(sta_pay_expFis_isAlive) );
    #endif
    return res;
}

int pay_exec_expFis(void *param){
    int ind;
    unsigned int temp;
    unsigned int timeout = 30;  //max time waiting to fill the sens_buffer
    unsigned int buff_size = fis_get_sens_buff_size();

    unsigned int fis_state = fis_get_state();    //get the initial state of the Payload

    unsigned int rc = 0;    //return code of "fis_iterate" function
    while(rc == 0){
        //executes the Payload and return
        //when it is time to save data in the Data Repository
        //so we must save the current data inside "sens_buff" into the Data Repository
        //and then resume the Payload execution
        fis_iterate(&rc, timeout);

        for(ind=0;ind<buff_size;ind++){
            //save the data into the Data Repository
            temp = fis_get_sens_buff_i(ind);
            dat_set_Payload_Buff(dat_pay_expFis, temp);

            #if FIS_CMD_VERBOSE > 0
                printf("    dat_set_Payload_Buff(%u)\n",temp);
            #endif

        }

        printf("Clearing WDT \n");
        ClrWdt();
    }

    //Payload end
    if(rc<0){   //fis_iterate finished with error
        fis_state = fis_get_state();    //use the state to see the cause of error
        #if FIS_CMD_VERBOSE
            printf("fis_iterate() finished with error ");
            printf("fis_state= 0x%X\n",fis_state);
        #endif
    }
    //return -1 if failed (rc < 0)
    //return +1 if succesfull (rc > 0)
    printf("pay_exec finished\n");
    return (rc < 0)? 0 : 1;

}

int pay_adhoc_expFis(void *param){
    
    unsigned int frec_array[] = {10, 50, 100, 500, 1000, 2000, 4000, 8000, 10000, 16000};
    int len_frec_array = 10;
    
    int res, i;
    unsigned int frec;
    for(i=0;i<len_frec_array;i++){
        frec = frec_array[i];
        res = pay_conf_expFis(&frec);
        res = pay_exec_expFis(0);
    }

    return res;
}

int pay_isAlive_expFis(void *param){
    /*
     * This Payload is mainly (DAC seems to basic to check isAlive with it)
     * Sw/PIC24 based, so it's assummed to always be aliwe.
     */
    if(SCH_PAY_FIS_ONBOARD == 0){return 0;}
    return 1;
}
/* 
 * Get the value of the MemEEPROM variable used by expFis
 * @param not used
 */
int pay_get_state_expFis(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_expFis_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
/*
 * Sets the value for the MemEEPROM variable used by expFis
 * @param value that will be set
 */
int pay_set_state_expFis(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_expFis_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}

int pay_testDAC_expFis(void *param){
    printf("pay_testDAC_expFis ...\r\n");

    unsigned int value = *((unsigned int *) param);
    #if FIS_CMD_VERBOSE
        printf("     parameter value = %u\n", value);
    #endif
    fis_testDAC(value);
    return 1;
}

int pay_print_seed(void* param) {
    printf("pay_print_seed ...\r\n");

    fis_payload_print_seed();
    
    printf("pay_print_seed ... finished \r\n");
    return 1;
}

int pay_testFreq_expFis(void *param){

    unsigned int value = *((unsigned int *) param);
    unsigned int frec_array[] = {value};
    int len_frec_array = 1;
    
    int res, i;
    unsigned int frec;
    for(i=0;i<len_frec_array;i++){
        frec = frec_array[i];
        res = pay_conf_expFis(&frec);
        res = pay_exec_expFis(0);
    }

    return res;
}

int pay_init_expFis(void *param){
    printf("pay_init_expFis ..\r\n");
    
    return 0;
}

int pay_take_expFis(void *param){
    printf("pay_take_expFis ..\r\n");
    
    return 0;
}
int pay_stop_expFis(void *param){
    printf("pay_stop_expFis ..\r\n");

    if( fis_iterate_isComplete() ){
        printf("fis_iterate_isComplete() returned TRUE\n");
    }

    return 1;
}

// Battery data payloads

int pay_isAlive_battery(void *param){
    return eps_isAlive(param);
}

int pay_get_state_battery(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_battery_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}

int pay_set_state_battery(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_battery_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}

int pay_init_battery(void *param){
    printf("pay_init_battery ..\r\n");

    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_battery;
    lenBuff = (unsigned int)(500*4);   //desde 0x00 a 0xFF
    dat_reset_Payload_Buff(pay_i);
    //agrego un comentario

    return pay_isAlive_battery(NULL);
}
int pay_take_battery(void *param){
    printf("pay_take_battery ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_battery);
    
    //Read EPS variables
    chkparam_t chkparam;
    if (!eps_get_hk(&chkparam)) {
        printf("Error requesting HK\r\n");
        return 0;
    }

    //Save EPS variables
    //Save voltage, panel current, system current, temperature 1 and 2
    dat_set_Payload_Buff(dat_pay_battery, (int)chkparam.bv);
    dat_set_Payload_Buff(dat_pay_battery, (int)chkparam.pc);
    dat_set_Payload_Buff(dat_pay_battery, (int)chkparam.sc);
    dat_set_Payload_Buff(dat_pay_battery, (int)chkparam.batt_temp[0]);
    dat_set_Payload_Buff(dat_pay_battery, (int)chkparam.batt_temp[1]);

    printf("writing ..\r\n");
    printf("dat_pay_battery[Voltge] = %d \r\n", (int)chkparam.bv);
    printf("dat_pay_battery[Panel curr.] = %d \r\n", (int)chkparam.pc);
    printf("dat_pay_battery[System curr.] = %d \r\n", (int)chkparam.sc);
    printf("dat_pay_battery[Temp 1] = %d \r\n", (int)chkparam.batt_temp[0]);
    printf("dat_pay_battery[Temp 2] = %d \r\n", (int)chkparam.batt_temp[1]);
    printf("-----------------------------\r\n");

    return 1;
}

int pay_execute_experiment_battery(void *param){ //comando para iniciar medicion bateria
    printf("pay_execute_experiment_battery ..\r\n");

    unsigned int timemed = *(int*)param; //95*60*10 = 57000
    unsigned int i;

    /* Reset Payload
     * This erases all previous contents in the Buffer
     */
    dat_reset_Payload_Buff(dat_pay_battery);

    //una orbita=95minutos, 60segundos, 10 muestras por segundo (sensor de 100ms)
    
    //prueba
    int val;
//    int j;
    int indx=0;

    //fin prueba
    for (i=0; i<timemed ; i++)
    {
        pay_take_battery((void*)&indx);
        __delay_ms(100); //muestras cada 100ms
        ClrWdt();
        //prueba
//        for (j=0; j<3; j++)
//        {
//            dat_get_Payload_Buff(dat_pay_battery, indx , &val);
//            printf("dat_pay_battery[%d] = %d \r\n", indx, val);
//            indx++;
//        }
        printf("reading ..\r\n");
        dat_get_Payload_Buff(dat_pay_battery, indx , &val);
        printf("dat_pay_battery[%d] = %d \r\n", indx, val);
        indx++;
        dat_get_Payload_Buff(dat_pay_battery, indx , &val);
        printf("dat_pay_battery[%d] = %d \r\n", indx, val);
        indx++;
        dat_get_Payload_Buff(dat_pay_battery, indx , &val);
        printf("dat_pay_battery[%d] = %d \r\n", indx, val);
        indx++;
        printf("-----------------------------\r\n");
        //fin prueba
    }
    printf("END battery %i..\r\n",i);
    return 1;
}


int pay_stop_battery(void *param){
    printf("pay_stop_battery ..\r\n");
    return 1;
}
//******************************************************************************
int pay_isAlive_debug(void *param){
    /*
     * This Payload is Sw based
     */
    if(SCH_PAY_DEBUG_ONBOARD == 0){return 0;}
    return 1;
}
int pay_get_state_debug(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_debug_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_debug(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_debug_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
static unsigned int pay_debug_cnt;
int pay_init_debug(void *param){
    printf("pay_init_debug ..\r\n");

    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_debug;
    lenBuff = (unsigned int)(500);    //payload en etapa experimental aun
    dat_reset_Payload_Buff(pay_i);

    //restart counter
    pay_debug_cnt = 0;

    #if(PAY_TSPAIR_nSLIST == 0)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_debug);
    #endif

    return pay_isAlive_debug(NULL);
}
int pay_take_debug(void *param){
    printf("pay_take_debug ..\r\n");

    #if(PAY_TSPAIR_nSLIST == 1)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_debug);
    #endif

    //save data
    unsigned int i, num_per_take = 1;
    for(i=0;i<num_per_take;i++){
        pay_debug_cnt++;
        dat_set_Payload_Buff(dat_pay_debug, pay_debug_cnt);
    }

    return 1;
}
int pay_stop_debug(void *param){
    printf("pay_stop_debug ..\r\n");

    return 1;
}
//******************************************************************************
int pay_isAlive_gyro(void *param){
    if(SCH_PAY_GYRO_ONBOARD == 0){return 0;}
    return gyr_isAlive();
}
int pay_get_state_gyro(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gyro_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_gyro(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gyro_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
int pay_debug_gyro(void *param){
    printf("pay_debug_gyro ..\r\n");
//    BOOL st = gyr_isAlive();
//    printf("dig_isAlive() = %d\n", (int)st);

    BOOL verb = *((int *)param);
    BOOL st = gyr_init_config();
    GYR_DATA res_data;
    gyr_take_samples(verb, &res_data);
    printf("X axis : %d\n", (res_data).a_x );
    printf("Y axis : %d\n", (res_data).a_y );
    printf("Z axis : %d\n", (res_data).a_z );
    printf("************************\n");

    return (int)st;
}
int pay_init_gyro(void *param){
    printf("pay_init_gyro ..\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gyro;
    lenBuff = (unsigned int)(500*3);  //(1440*3)      //numero de 10-minutos en un dia
    dat_reset_Payload_Buff(pay_i);

    //configure Payload
    int res;
    if( gyr_isAlive()==TRUE ){
        res = 1;
        gyr_init_config();
    }
    else{
        res = 0;
    }

    //debug info
    printf("  sta_pay_gyro_isAlive = %d \r\n", sta_get_PayStateVar(sta_pay_gyro_isAlive) );
    printf("  PPC_GYRO_INT2_CHECK = %d \r\n", PPC_GYRO_INT2_CHECK );

    #if(PAY_TSPAIR_nSLIST == 0)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_gyro);
    #endif

    return res;
}
int pay_take_gyro(void *param){
    printf("pay_take_gyro ..\r\n");

    #if(PAY_TSPAIR_nSLIST == 1)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_gyro);
    #endif

    //in case of failure
    if( pay_isAlive_gyro(NULL) == 0){
        dat_set_Payload_Buff(dat_pay_gyro ,0xFAFA);
        printf("pay_take_gyro failure\r\n");
        return 1;
    }

    //take and save data
    GYR_DATA res_data;
    gyr_init_config();
    gyr_take_samples(FALSE, &res_data);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_x);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_y);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_z);

    printf("pay_take_gyro a_x = %d, a_y = %d, a_z = %d \r\n", res_data.a_x, res_data.a_y, res_data.a_z);
    
    return 1;
}
int pay_stop_gyro(void *param){
    printf("pay_stop_gyro ..\r\n");
    return 1;
}
//******************************************************************************
int pay_isAlive_tmEstado(void *param){
    /*
     * This Payload is Sw based, so it's assummed to always be aliwe.
     */
    if(SCH_PAY_TMESTADO_ONBOARD == 0){return 0;}
    return 1;
}
int pay_get_state_tmEstado(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_tmEstado_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_tmEstado(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_tmEstado_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
int pay_init_tmEstado(void *param){
    printf("pay_init_tmEstado ..\r\n");

    //configure PaylaodBuff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_tmEstado;
    lenBuff = (unsigned int)(40*sta_busStateVar_last_one);  //(4*60/5=48)      //numero de 5-minutos en una orbita (4 horas)
    dat_reset_Payload_Buff(pay_i);

    //isAlive
    int res_isAlive = sta_get_PayStateVar(sta_pay_tmEstado_isAlive);

    //debug info
    #if (_VERBOSE_>=2)
        printf("  sta_pay_tmEstado_isAlive = %d \r\n", res_isAlive );
    #endif

    #if(PAY_TSPAIR_nSLIST == 0)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_tmEstado);
    #endif


    return res_isAlive;
}

int pay_take_tmEstado(void *param){
    printf("pay_take_tmEstado ..\r\n");
    int verbose = *( (int*)param );

    #if(PAY_TSPAIR_nSLIST == 1)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_tmEstado);
    #endif

    //save data
    STA_BusStateVar indxVar; int var;
    for(indxVar=0; indxVar<sta_busStateVar_last_one; indxVar++){
        var = sta_get_BusStateVar(indxVar);
        dat_set_Payload_Buff(dat_pay_tmEstado, var);
        //__delay_ms(300);
        if (verbose>=1)
            printf("sta_get_stateVar[%s] = %d\r\n", sta_BusStateVarToString(indxVar), var);
    }

    return 1;
}
int pay_stop_tmEstado(void *param){
    printf("pay_stop_tmEstado ..\r\n");
    return 1;
}
//******************************************************************************
int pay_get_savedPhoto_camera(void *param){
    unsigned int length_indx = dat_get_NextPayIndx(dat_pay_camera);
    unsigned int index, value;

    //Chequeo que pay langth sea la mitad que el
    printf("[pay_get_savedPhoto_camera]: NextPayIndx = %u \r\n", length_indx);

    // print rtc time
    rtc_print(NULL);

    for(index = 0; index<length_indx; index++)
    {
        dat_get_Payload_Buff(dat_pay_camera, index, (int* )&value);
        printf("%04X", (unsigned int)value);    //print also left-side zeros
        
        ClrWdt();
    }
    printf("\r\n");

    // print rtc time
    rtc_print(NULL);

    return 1;
}
int pay_isAlive_camera(void *param){
    if(SCH_PAY_CAM_nMEMFLASH_ONBOARD == 0){return 0;}
    return cam_isAlive();
}
int pay_get_state_camera(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_camera_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_camera(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_camera_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
int pay_init_camera(void *param){
    printf("pay_init_camera ..\r\n");

    //switch camera on
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    PPC_CAM_SWITCH=1;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    printf("  PPC_CAM_HOLD_CHECK = %d\r\n", PPC_CAM_HOLD_CHECK);

    // Wait for the camera to boot
    cam_wait_hold_wtimeout(TRUE);
    // Wait for PPC_CAM_HOLD_CHECK low signal
    __delay_ms(3000);

    //debug info
    printf("  PPC_CAM_HOLD_CHECK = %d\r\n", PPC_CAM_HOLD_CHECK);
    int res = sta_get_PayStateVar(sta_pay_camera_isAlive);
    printf("  sta_pay_camera_isAlive = %d \r\n", res);
    
    return res;
}
int pay_take_camera(void *param){
    printf("pay_take_camera ..\r\n");
    
    int resol = *((int *)param);

    //save date time of the photo
    //pay_save_date_time_to_Payload_Buff(dat_pay_camera);   //save date_time in 2ints

    //in case of failure
    if( sta_get_PayStateVar(sta_pay_camera_isAlive)==0 ){
        printf("camera is not alive!..\r\n");
        dat_set_Payload_Buff(dat_pay_camera ,0xFAFA);
        return 1;
    }

    //BOOL st = pay_cam_takeAndSave_photo(0x07, 0x00, 0x05);
    BOOL st = pay_cam_takeAndSave_photo(resol, 0x00, 0x05);
    //BOOL st = pay_cam_takeAndSave_photo(0x02, 0x00, 0x05);

    return st;
}
int pay_stop_camera(void *param){
    printf("pay_stop_camera ..\r\n");

    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    PPC_CAM_SWITCH=0;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);

    return 1;
}
BOOL pay_cam_takeAndSave_photo(int resolution, int qual, int pic_type){
    printf("pay_takeAndSave_photo ..\r\n");
    
    printf("Sync camera ..\r\n");
    int status = cam_sync(TRUE);
    printf("Sync status (0 => successful) = %d \r\n", status);
    
    printf(" Taking photo ..\r\n");
    printf(" cam_photo(%d, %d, %d) \r\n", resolution, qual, pic_type);
    unsigned int photo_byte_length= cam_photo(resolution, qual, pic_type);
    #if (_VERBOSE_>=1)
        printf("    Photo length = %u\r\n", photo_byte_length);
    #endif

    //in case of errors
    if(photo_byte_length == 0){
        printf(" Error: No photo was taken ..\r\n");
        dat_set_Payload_Buff(dat_pay_camera ,0xFAFA);
        return FALSE;
    }

    //calculate  length in ints
    unsigned int photo_int_length = photo_byte_length/2;    //se guardan 2byten en 1int
    printf("  Debug info: photo_int_length = %u, photo_byte_length = %d\r\n",
            photo_int_length, photo_byte_length);

    //Inicializa la estructura de data payload
    dat_reset_Payload_Buff(dat_pay_camera);

    //prepara variables para guardar foto
    unsigned int int_r[10];
    unsigned int num_10sections, rest_10sections;

    num_10sections = photo_int_length/10;
    rest_10sections = photo_int_length%10;
    
    printf("  Debug info: num_10sections = %d, rest_10sections = %d\r\n",
            num_10sections, rest_10sections);

    //warn about duration
    printf("    Saving data (this might take up tp 15 min) ..\r\n");
    rtc_print(NULL);
    
    unsigned int iter;
    for(iter = 0; iter<num_10sections; iter++)
    {
        //get 1 int out of 2 bytes
        int_r[0] = pay_camera_get_1int_from_2bytes();
        int_r[1] = pay_camera_get_1int_from_2bytes();
        int_r[2] = pay_camera_get_1int_from_2bytes();
        int_r[3] = pay_camera_get_1int_from_2bytes();
        int_r[4] = pay_camera_get_1int_from_2bytes();
        int_r[5] = pay_camera_get_1int_from_2bytes();
        int_r[6] = pay_camera_get_1int_from_2bytes();
        int_r[7] = pay_camera_get_1int_from_2bytes();
        int_r[8] = pay_camera_get_1int_from_2bytes();
        int_r[9] = pay_camera_get_1int_from_2bytes();

        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[0]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[1]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[2]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[3]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[4]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[5]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[6]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[7]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[8]);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[9]);

        ClrWdt();
        //printf("      saving [%d/%d] ..\r\n", iter, num_10sections);
    }

    for(iter = 0; iter<rest_10sections; iter++)
    {
        //get 1 int out of 2 bytes
        int_r[0] = pay_camera_get_1int_from_2bytes();

        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[0]);

        ClrWdt();
    }

    // print rtc time
    rtc_print(NULL);

    //Chequeo que pay langth sea la mitad que el
    printf(" Debug info: NextPayIndx = %u \r\n",
            dat_get_NextPayIndx(dat_pay_camera));

    return TRUE;
}
int pay_takePhoto_camera(void *param){
    printf("pay_takePhoto_camera ..\r\n");

    //int asd = *( (int *)param );
    //__delay_ms(asd);
    //int asd = STA_PPC_OPMODE_CAMERA;
    //ppc_set_opMode(&asd);
    //return 1;

    pay_init_camera(NULL);
    __delay_ms(10000);
    int st = pay_take_camera(param);
    __delay_ms(1000);
    pay_stop_camera(NULL);

    //parar Payload (por si acaso)
    PAY_xxx_State cam_state = pay_xxx_state_inactive;
    pay_set_state_camera(&cam_state);
            
    return st;
}
//aux
int pay_camera_get_1int_from_2bytes(void){
    unsigned char byte_r1;
    unsigned int int_r1;
    //get 1 int out of 2 bytes
    int_r1 = 0;
    SPI_nSS_1 = 0;
    byte_r1 = SPI_1_transfer(0x00);
    SPI_nSS_1 = 1;
    int_r1 = (unsigned int)byte_r1;

    SPI_nSS_1 = 0;
    byte_r1 = SPI_1_transfer(0x00);
    SPI_nSS_1 = 1;
    int_r1 = ( (int_r1<<8)|((unsigned int)byte_r1) );
    
    return int_r1;
}
//******************************************************************************
int pay_isAlive_gps(void *param){
    unsigned char *gps_buff = gps_exec_cmd(5);    // gps_cmd_rcv_model
    unsigned int gps_buff_len = strlen((const char*)gps_buff);
    if(gps_buff_len != 0){return 1;}
    return 0;
}
int pay_get_state_gps(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gps_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_gps(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gps_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
int pay_init_gps(void *param){
    printf("pay_init_gps ..\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gps;
    lenBuff = (unsigned int)(200);   //(1440)  //numero de 10-minutos en un dia
    dat_reset_Payload_Buff(pay_i);

//    #if(PAY_TSPAIR_nSLIST == 0)
//        //save date_time in 2ints
//        pay_save_date_time_to_Payload_Buff(dat_pay_gps);
//    #endif

    //Power GPS on
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    PPC_GPS_SWITCH = 1;
    __delay_ms(100); //wait while port write takes effect
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );

    //check isAlive status
    printf("  sta_pay_gps_isAlive = %d \r\n", sta_get_PayStateVar(sta_pay_gps_isAlive) );
    __delay_ms(4000); //wait GPS boots
    printf("  sta_pay_gps_isAlive = %d \r\n", sta_get_PayStateVar(sta_pay_gps_isAlive) );

    return 1;
}

/**
 * Takes samples from GPS, executing any of available commands
 * @param param Command sent to GPS see gps_exec_cmd
 * @return null, but print gps buffer
 */
int pay_take_gps(void *param){
    printf("pay_take_gps ..\r\n");

    //Power GPS on
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    PPC_GPS_SWITCH = 1;
    __delay_ms(100); //wait while port write takes effect
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );

//    #if(PAY_TSPAIR_nSLIST == 0)
//        //save date_time in 2ints
//        pay_save_date_time_to_Payload_Buff(dat_pay_gps);
//    #endif

    unsigned int gps_cmdnum = *((unsigned int *)param);
    unsigned char *gps_buff;
    unsigned int i=0, gps_buff_len = 0;

    // Get "gps_cmdnum" command
    // gps_cmdnum = 25 => RMC nmea sentence
    gps_buff = gps_exec_cmd(gps_cmdnum);
    gps_buff_len = strlen((const char*)gps_buff);
    printf("gps_buff: %s", gps_buff);
    for(i=0; i<gps_buff_len; i++){
        dat_set_Payload_Buff(dat_pay_gps, gps_buff[i]);
        //printf("gps_buff[%d] = %c \r\n", i, gps_buff[i]);
    }

    //Erase gps_buff
    gps_clear_buffer();
    gps_clearUARTbuffer();

    return 1;
}
int pay_stop_gps(void *param){
    printf("pay_stop_gps ..\r\n");

    //Save time from RTC and GPS to compare
    pay_save_date_time_to_Payload_Buff(dat_pay_gps);
    unsigned char *gps_buff;
    unsigned int i=0, gps_buff_len = 0;
    gps_buff = gps_exec_cmd(25);    // RMC nmea sentence
    gps_buff_len = strlen((const char*)gps_buff);
    printf("gps_buff: %s", gps_buff);
    for(i=0; i<gps_buff_len; i++){
        dat_set_Payload_Buff(dat_pay_gps, gps_buff[i]);
        //printf("gps_buff[%d] = %c \r\n", i, gps_buff[i]);
    }
    
    //update RTC time using GPS time
    //0123456789012345678901234567890123456789
    //$GNRMC,150957.00,V,,,,,,,311215,,,N*69
    unsigned int gps_hh = (gps_buff[7] - '0')*10 + (gps_buff[8] - '0')*1;
    unsigned int gps_mm = (gps_buff[9] - '0')*10 + (gps_buff[10] - '0')*1;
    unsigned int gps_ss = (gps_buff[11] - '0')*10 + (gps_buff[12] - '0')*1;
        
    if((gps_hh==00) && (gps_mm==00) && (gps_ss==00)){
        // $GNRMC,000000.00,V,,,,,,,,,,N*63
        //GPS not locked, don't update RTC
    }
    else{
        printf("Updating RTC => gps_hh = %d gps_mm = %d gps_ss = %d \r\n", gps_hh, gps_mm, gps_ss);
        //rtc_adjust_year(&gps_hh);
        //rtc_adjust_month(&gps_hh);
        //rtc_adjust_day(&gps_hh);
        //rtc_adjust_weekday(&gps_hh);
        rtc_adjust_hour(&gps_hh);
        rtc_adjust_minutes(&gps_mm);
        rtc_adjust_seconds(&gps_ss);
        rtc_print(NULL);
    }

    //Power GPS off
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    PPC_GPS_SWITCH = 0;
    __delay_ms(100); //wait while port write takes effect
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );

    //Erase gps_buff
    gps_clear_buffer();
    gps_clearUARTbuffer();

    return 1;
}
//       // FUNCTIONAL
//       if((strcmp(con_cmd, "model") == 0) )
int pay_gps_updateRTC(void *param){
    printf("pay_gps_updateRTC ..\r\n");

    //Get time from GPS
    unsigned char *gps_buff;
    unsigned int gps_buff_len = 0;
    gps_buff = gps_exec_cmd(25);    // RMC nmea sentence
    gps_buff_len = strlen((const char*)gps_buff);
    printf("gps_buff: %s", gps_buff);

    //update RTC time using GPS time
    //01234567890123456789
    //$GNRMC,150957.00,V,,,,,,,311215,,,N*69
    unsigned int gps_hh = (gps_buff[7] - '0')*10 + (gps_buff[8] - '0')*1;
    unsigned int gps_mm = (gps_buff[9] - '0')*10 + (gps_buff[10] - '0')*1;
    unsigned int gps_ss = (gps_buff[11] - '0')*10 + (gps_buff[12] - '0')*1;
    printf("gps_hh = %d gps_mm = %d gps_ss = %d \r\n", gps_hh, gps_mm, gps_ss);
    //rtc_adjust_year(&gps_hh);
    //rtc_adjust_month(&gps_hh);
    //rtc_adjust_day(&gps_hh);
    //rtc_adjust_weekday(&gps_hh);
    rtc_adjust_hour(&gps_hh);
    rtc_adjust_minutes(&gps_mm);
    rtc_adjust_seconds(&gps_ss);
    rtc_print(NULL);

    //Erase gps_buff
    gps_clear_buffer();
    gps_clearUARTbuffer();

    return 1;
}

int pay_gps_serial(void *param){
    return 1;
}

//       // FUNCTIONAL
//       if(strcmp(con_cmd, "senddm") == 0)
int pay_gps_senddn(void *param){
    return 1;
}

//       // BETA (NOT TOTALLY FUNCTIONAL)
//       if(strcmp(con_cmd, "jmesg") == 0)
int pay_gps_jmesg(void *param){
    return 1;
}

//       // BETA (NOT TOTALLY FUNCTIONAL)
//       if(strcmp(con_cmd, "jsat") == 0)
int pay_gps_jsat(void *param){
    return 1;
}

//******************************************************************************
BOOL pay_deploy_langmuirProbe(int realtime){
    printf("******************************\r\n");
    printf("Deployng LangmuirProbe\r\n");
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        PPC_LANGMUIR_DEP_SWITCH = 1;
    #endif
    ClrWdt();
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    if(realtime==1){

        __delay_ms(45311);
        ClrWdt();
        __delay_ms(45311);

//        ClrWdt();
//        __delay_ms(30000); //wait 30sec to burn nylon
//        __delay_ms(30000); //wait 30sec to burn nylon
//        ClrWdt();
//        __delay_ms(30000); //wait 30sec to burn nylon
    }
    else{
        ClrWdt();
        __delay_ms(3000); //wait 30sec to burn nylon
    }

    #if (SCH_PAY_LANGMUIR_ONBOARD==1)
        PPC_LANGMUIR_DEP_SWITCH = 0;
    #endif
    ClrWdt();
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    return TRUE;
}
int pay_isAlive_langmuirProbe(void *param){
    if(SCH_PAY_LANGMUIR_ONBOARD == 0){return 0;}

    return langmuir_isAlive();
}
int pay_get_state_langmuirProbe(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_langmuirProbe_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_langmuirProbe(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_langmuirProbe_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}

/**
 * Init. langmuir probe including deployment
 * @param param 1-Force deploymeny, 0-Deploy only first time
 * @return Bool, langmuir is alive
 */
static int first_time_langmuirProbe = 0;
int pay_init_langmuirProbe(void *param){
    printf("pay_init_langmuirProbe ..\r\n");
    
    int arg = *((int *)param);
    if(first_time_langmuirProbe == 0 && arg != (-1)){
        first_time_langmuirProbe = 1;
        
        ClrWdt();
        /////////////// There is no better place than this one /////////////////////
        /* Deploy langmuir should NOT be here, but there is no way
         * to check deployment, so its included here */
        #if (SCH_ANTENNA_ONBOARD==1 && SCH_PAY_LANGMUIR_ONBOARD==1)
            int rt_mode = SCH_THOUSEKEEPING_ANT_DEP_REALTIME; /* 1=Real Time, 0=Debug Time */
            pay_deploy_langmuirProbe(rt_mode);    //realtime mode
            //set var lang dep = 1 b
        #endif
        ClrWdt();
    }

    //configure Payload_Buff
    DAT_Payload_Buff pay_i;
    pay_i = dat_pay_langmuirProbe;
    dat_reset_Payload_Buff(pay_i);

    //configs
    int res_isAlive = pay_isAlive_langmuirProbe(NULL); //not fully implemented yet => always dead
    lag_erase_buffer();

    //debug info
    printf("  sta_pay_langmuirProbe_isAlive = %d \r\n", res_isAlive );
    printf("  sta_pay_langmuirProbe_isDeployed = %d \r\n", sta_get_PayStateVar(sta_pay_langmuirProbe_isDeployed) );

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_langmuirProbe);
    //save iniial data
    int i;
    __delay_ms(15000);  //wait 15sec for particle counter (LangmuirProbe)
    int lenbuff_cal = lag_read_cal_packet(FALSE);
    printf("  lenbuff_cal = %d \r\n", lenbuff_cal);
    for(i=0;i<lenbuff_cal;i++){
        dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(i));
    }

    return res_isAlive;
}

int pay_take_langmuirProbe(void *param){
    printf("pay_take_langmuirProbe ..\r\n");

    #if(PAY_TSPAIR_nSLIST == 1)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_langmuirProbe);
    #endif

    //(15 secs delay between commands with an increased delay)
    //save data
    int i;

    int lenbuff_pla = lag_read_plasma_packet(FALSE);
    for(i=0;i<lenbuff_pla;i++){
        dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(i));
    }

    return 1;
}

/**
 * Comando para debug de langmuir. Muestra en panatalla los datos solicitados
 * el instrumento, que puede ser: calibraciom, plasma o sweep.
 * @param param 0-calibracion, 1-plasma, 2-sweep
 * @return Resultado de la operacion: 1-exito, 0-error
 */
int pay_adhoc_langmuirProbe(void* param)
{
    printf("pay_adhoc_langmuirProbe ..\r\n");

    /* Reset Payload
     * This erases all previous contents in dat_pay_langmuirProbe Buffer
     */
    DAT_Payload_Buff pay_i;
    pay_i = dat_pay_langmuirProbe;
    dat_reset_Payload_Buff(pay_i);

    unsigned int times_per_min = 4;
    unsigned int delay = (1000U*60U)/times_per_min;
    unsigned int times = (24U*60U)*times_per_min;
    int i, j;

    //save initial data
    pay_save_date_time_to_Payload_Buff(dat_pay_langmuirProbe);
    printf("    calling lag_read_cal_packet ..\r\n");
    int lenbuff_cal = lag_read_cal_packet(FALSE);
    for(i=0; i<lenbuff_cal; i++){
        dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(i));
    }
    
    for(i=1;i<=times;i++){
        printf("    %d/%d[i/times] ..\r\n", i, times);
        __delay_ms(delay);

        ClrWdt();

        printf("        calling lag_read_plasma_packet ..\r\n");
        int lenbuff_pla = lag_read_plasma_packet(FALSE);
        for(j=0; j<lenbuff_pla; j++){
            dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(j));
        }
    }

    //save final data
    __delay_ms(15000);  //wait 15sec for particle counter (LangmuirProbe)
    lenbuff_cal = lag_read_cal_packet(FALSE);
    for(i=0;i<lenbuff_cal;i++){
        dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(i));
    }

    return 1;
}

int pay_stop_langmuirProbe(void *param){
    printf("pay_stop_langmuirProbe ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_langmuirProbe);
    
    //save final data
    int i;
    __delay_ms(15000);  //wait 15sec for particle counter (LangmuirProbe)
    int lenbuff_cal = lag_read_cal_packet(FALSE);
    for(i=0;i<lenbuff_cal;i++){
        dat_set_Payload_Buff(dat_pay_langmuirProbe, (int)lag_get_langmuir_buffer_i(i));
    }

    lag_erase_buffer();

    //avoid extra writes from FSM
    PAY_xxx_State state;
    state = pay_xxx_state_waiting_tx;
    pay_set_state_langmuirProbe((void *)&state);

    return 1;
}

int pay_send_to_langmuirProbe(void *param)
{
    printf("pay_send_to_lagmuir ..\r\n");

    int c=*((int *)param);
    switch(c){
        case 1:
            lag_read_plasma_packet(TRUE);
        break;
        case 2:
            lag_read_cal_packet(TRUE);
        break;
        case 3:
            lag_read_sweep_packet(TRUE);
        break;
    }

    return 1;
}

//******************************************************************************
int pay_debug_sensTemp(void *param){
    int res;
    sensTemp_init(ST1_ADDRESS);
    res = sensTemp_take(ST1_ADDRESS, TRUE);

    sensTemp_init(ST2_ADDRESS);
    res = sensTemp_take(ST2_ADDRESS, TRUE);

    sensTemp_init(ST3_ADDRESS);
    res = sensTemp_take(ST3_ADDRESS, TRUE);

    sensTemp_init(ST4_ADDRESS);
    res = sensTemp_take(ST4_ADDRESS, TRUE);

    return pay_isAlive_sensTemp(NULL);
}
int pay_isAlive_sensTemp(void *param){
    if(SCH_PAY_SENSTEMP_ONBOARD == 0){return 0;}

    int res, rest = 0;
    res = (int)sensTemp_isAlive(ST1_ADDRESS);
    rest = rest<<1;
    rest = rest|res;
    res = (int)sensTemp_isAlive(ST2_ADDRESS);
    rest = rest<<1;
    rest = rest|res;
    res = (int)sensTemp_isAlive(ST3_ADDRESS);
    rest = rest<<1;
    rest = rest|res;
    res = (int)sensTemp_isAlive(ST4_ADDRESS);
    rest = rest<<1;
    rest = rest|res;

    //                   76543210
    //if all Alive res = 00001111 = 0x0F = 15

    return rest;
}
int pay_get_state_sensTemp(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_sensTemp_state;
    int res = mem_getVar(mem_eeprom_var);
    return res;
}
int pay_set_state_sensTemp(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_sensTemp_state;
    mem_setVar(mem_eeprom_var, value);
    return 1;
}
int pay_init_sensTemp(void *param){
    printf("pay_init_sensTemp ..\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_sensTemp;
    lenBuff = (unsigned int)(500*4) + 4;    //numero de muestras*4 + estado de isAlive cada sensor
    dat_reset_Payload_Buff(pay_i);

    //configure Payload
    BOOL s1, s2, s3, s4;
    s1 = sensTemp_init(ST1_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s1);

    s2 = sensTemp_init(ST2_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s2);

    s3 = sensTemp_init(ST3_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s3);

    s4 = sensTemp_init(ST4_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s4);

    int res_isAlive = sta_get_PayStateVar(sta_pay_sensTemp_isAlive);

    //debug info
    printf("  sta_pay_sensTemp_isAlive = %d \r\n", res_isAlive );

    #if(PAY_TSPAIR_nSLIST == 0)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_sensTemp);
    #endif

    return res_isAlive;
}
int pay_take_sensTemp(void *param){
    printf("pay_take_sensTemp ..\r\n");

    #if(PAY_TSPAIR_nSLIST == 1)
        //save date_time in 2ints
        pay_save_date_time_to_Payload_Buff(dat_pay_sensTemp);
    #endif

    //in case of failure
    if( pay_isAlive_sensTemp(NULL) == 0){
        printf("sensTemp is not alive!..\r\n");
        dat_set_Payload_Buff(dat_pay_sensTemp ,0xFAFA);
        return 0;
    }

    //save data
    int val;
    val=sensTemp_take(ST1_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val);
    printf("pay_take_sensTemp t1 = %d \r\n", val);

    val=sensTemp_take(ST2_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val);
    printf("pay_take_sensTemp t2 = %d \r\n", val);

    val=sensTemp_take(ST3_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val);
    printf("pay_take_sensTemp t3 = %d \r\n", val);

    val=sensTemp_take(ST4_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val);
    printf("pay_take_sensTemp t4 = %d \r\n", val);

    return 1;
}
int pay_stop_sensTemp(void *param){
    printf("pay_stop_sensTemp ..\r\n");
    return 1;
}
//******************************************************************************
//Aux function for FP2


void pay_fp2_multiplexed(){
    printf("pay_fp2_i_multiplexing ..\r\n");

//    DispCmd NewCmd;
//    NewCmd.cmdId = CMD_CMDNULL;
//    NewCmd.idOrig = CMD_IDORIG_TFLIGHTPLAN2;
//    NewCmd.param = 0;

    static DAT_Payload_Buff current_pay_i;

    //multiplexo pay_i por turnos
    #if (SCH_TFLIGHTPLAN2_VERBOSE>=2)
        printf("pay_fp2_i_multiplexing => pay_i = %d\r\n", (unsigned int)current_pay_i);
    #endif

    PAY_xxx_State pay_i_state = sta_get_BusStateVar(sta_DAT_Payload_Buff_to_STA_PayStateVar(current_pay_i));
    switch(pay_i_state){
        case pay_xxx_state_inactive:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_inactive \r\n");
            break;
        case pay_xxx_state_active:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_active \r\n");
            break;
        case pay_xxx_state_run_init:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_run_init \r\n");
            break;
        case pay_xxx_state_run_take:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_run_take \r\n");
            break;
        case pay_xxx_state_run_stop:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_run_stop \r\n");
            break;
        case pay_xxx_state_waiting_tx:
            printf("  pay_i = %s", dat_get_payload_name(current_pay_i));
            printf(", state = sta_pay_xxx_state_waiting_tx \r\n");
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
//            NewCmd.cmdId = pay_id_fp2_default_fsm;
//            NewCmd.param = current_pay_i;
//            xQueueSend(dispatcherQueue, (const void *) &NewCmd, portMAX_DELAY);
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
void pay_fp2_simultaneous(void)
{
    //reviso payloads "simultaneamente" y ejecuto en multiplos de cada llamada reentrante
    static long unsigned int exec_tick;
    exec_tick++;
    static unsigned int run_take_times_executed[dat_pay_last_one];    //all initialized to zero

    DAT_Payload_Buff pay_i;
    PAY_xxx_State pay_i_state;
    int pay_i_tick_rate;

    #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
        printf("[pay_fp2_simultaneous] running ..\r\n");
        //print time
        rtc_print(NULL);
    #endif

    for(pay_i = 0; pay_i < dat_pay_last_one; pay_i++)
    {
        //continue if it's no time for pay_i yet
        pay_i_tick_rate = pay_fp2_get_exec_rate(pay_i);
        if( exec_tick%pay_i_tick_rate != 0 ){continue;}
        

        #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
            printf("  pay_i = %d = %s \r\n", pay_i, dat_get_payload_name(pay_i) );
            printf("  exec_tick = %lu \r\n", exec_tick);
            printf("  pay_i_tick_rate = %d \r\n", pay_i_tick_rate);
        #endif

        STA_PayStateVar pay_i_state_var = sta_DAT_Payload_Buff_to_STA_PayStateVar(pay_i);
        pay_i_state = sta_get_PayStateVar(pay_i_state_var);
        switch(pay_i_state){
        //**********************************************************************
            case pay_xxx_state_inactive:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_inactive \r\n");
                #endif

                break;
        //**********************************************************************
            case pay_xxx_state_active:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_active \r\n");
                #endif

                //change state to sta_pay_xxx_state_run_init
                pay_set_state(pay_i, pay_xxx_state_run_init);

                break;
        //**********************************************************************
            case pay_xxx_state_run_init:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_run_init \r\n");
                #endif

                //execute pay_xxx_init
                printf("  pay_xxx_init \r\n");
                pay_fp2_exec_run_xxx(pay_i, pay_xxx_state_run_init);


                //change state to sta_pay_xxx_state_run_take
                pay_set_state(pay_i, pay_xxx_state_run_take);

                break;
        //**********************************************************************
            case pay_xxx_state_run_take:
                //increment
                run_take_times_executed[pay_i]++;

                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_run_take \r\n");
                    printf("  run_take_times_executed[pay_i = %d] = %u \r\n", pay_i, run_take_times_executed[pay_i] );
                    printf("  pay_fp2_get_run_take_num_exec_times(pay_i = %u) = %d \r\n", pay_i, pay_fp2_get_run_take_num_exec_times(pay_i) );
                #endif

                //execute pay_xxx_take
                printf("  pay_xxx_take \r\n");
                pay_fp2_exec_run_xxx(pay_i, pay_xxx_state_run_take);

                //change state to sta_pay_xxx_state_run_stop if current exec is the last
                if( run_take_times_executed[pay_i] >= pay_fp2_get_run_take_num_exec_times(pay_i) ){
                    pay_set_state(pay_i, pay_xxx_state_run_stop);
                    //reset times_executed[pay_i] for the next time
                    run_take_times_executed[pay_i] = 0;
                }
                    
                break;
        //**********************************************************************
            case pay_xxx_state_run_stop:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_run_stop \r\n");
                #endif

                //execute pay_xxx_stop
                printf("  pay_xxx_stop \r\n");
                pay_fp2_exec_run_xxx(pay_i, pay_xxx_state_run_stop);

                //change state to sta_pay_xxx_state_waiting_tx
                pay_set_state(pay_i, pay_xxx_state_waiting_tx);

                break;
        //**********************************************************************
            case pay_xxx_state_waiting_tx:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_waiting_tx \r\n");
                #endif

                //change state to sta_pay_xxx_state_active only if pay_i TX is done
                if(FALSE){
                    pay_set_state(pay_i, pay_xxx_state_active);
                }

                break;
        //**********************************************************************
            default:
                #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = [ERROR] Unknown State \r\n");
                #endif
                break;
        //**********************************************************************
        }

        #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
            printf("---------------------------------------------\r\n");
        #endif

    }
    
    #if (SCH_TFLIGHTPLAN2_VERBOSE>=1)
        //print time
        rtc_print(NULL);
    #endif
}

/**
 * Return the number of tick before a pay_i is executed
 * @param pay_i
 * @return Return the number of ticks before a pay_i is executed
 */
int pay_fp2_get_exec_rate(DAT_Payload_Buff pay_i){

    /* Se asume que hay un tick cada 10[sec]
     * Por lo tanto un tick de 6 => exec cada 60sec
     */

    switch(pay_i){
        case dat_pay_tmEstado:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_battery:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_debug:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_langmuirProbe:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_gps:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_camera:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_sensTemp:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_gyro:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_expFis:
            return 6;       //tick_rate in taskHousekeeping is 10sec => every 60 sec = 1min
            break;
        case dat_pay_last_one:
            //ignore
            break;
    }
    return -1;
}

/**
 * Return the number of times pay_i is to be executed
 * @param pay_i
 * @return Return TRUE = isDOne, FLASE = contiune pay_take
 */
unsigned int pay_fp2_get_run_take_num_exec_times(DAT_Payload_Buff pay_i){
    unsigned int max_exec_times;

    /* Se asume que hay un tick cada 1[min]
     * 
     * Luego:
     * 1) Con 95 min completo una orbita
     * 2) Si una "vuelta al mundo" dura 24*60 = 1440[min]
     * Si cada orbita dura 95[min] => 1440/95 = 15.1578947 [orbitas/vuelta al mundo]
     * Por lo tanto con aproximadamente 16 orbitas doy "la vuelta al mundo"
     */
    unsigned int pay_exec_times = 95*2;
    //unsigned int pay_exec_times = 95*16;
    //unsigned int pay_exec_times = 1440 + 20;
    switch(pay_i){
        case dat_pay_tmEstado:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_battery:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_debug:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_langmuirProbe:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_gps:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_camera:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_sensTemp:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_gyro:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        case dat_pay_expFis:
            max_exec_times = pay_exec_times;    // if tick is 1 min => 1 orbit of 95 min => 95 execution_times
            break;
        default:
            //ignore
            max_exec_times = 0;
            break;
    }
    return max_exec_times;
}

/**
 * Exectutes the correspnding Cmd for each pay_i and sta_pay_xxx_state
 * @param pay_i
 * @param run_state
 */
void pay_fp2_exec_run_xxx(DAT_Payload_Buff pay_i, PAY_xxx_State state){
    int arg;
    switch(pay_i){
        case dat_pay_tmEstado:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_tmEstado(&arg);
                    break;
                case pay_xxx_state_run_take:
                    arg = 1;
                    pay_take_tmEstado(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_tmEstado(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_battery:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_battery(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_battery(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_battery(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_debug:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_debug(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_debug(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_debug(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_langmuirProbe:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_langmuirProbe(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_langmuirProbe(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_langmuirProbe(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_gps:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_gps(&arg);
                    break;
                case pay_xxx_state_run_take:
                    arg = 25;   //gps_cmdnum => RMC nmea sentence
                    pay_take_gps(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_gps(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_camera:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_camera(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_camera(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_camera(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_sensTemp:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_sensTemp(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_sensTemp(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_sensTemp(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_gyro:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_gyro(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_gyro(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_gyro(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_expFis:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_expFis(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_expFis(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_expFis(&arg);
                    break;
                //ignore the rest of states
                case pay_xxx_state_active:
                case pay_xxx_state_inactive:
                case pay_xxx_state_waiting_tx:
                    break;
            }
            break;
        case dat_pay_last_one:
            //ignore
            break;
    }
}

/**
 * Get pay_i execution state
 * @param pay_i
 * @param state
 */
PAY_xxx_State pay_get_state(DAT_Payload_Buff pay_i){
    PAY_xxx_State pay_i_state;
    switch(pay_i){
        case dat_pay_tmEstado:
            pay_i_state = pay_get_state_tmEstado(NULL);
            break;
        case dat_pay_battery:
            pay_i_state = pay_get_state_battery(NULL);
            break;
        case dat_pay_debug:
            pay_i_state = pay_get_state_debug(NULL);
            break;
        case dat_pay_langmuirProbe:
            pay_i_state = pay_get_state_langmuirProbe(NULL);
            break;
        case dat_pay_gps:
            pay_i_state = pay_get_state_gps(NULL);
            break;
        case dat_pay_camera:
            pay_i_state = pay_get_state_camera(NULL);
            break;
        case dat_pay_sensTemp:
            pay_i_state = pay_get_state_sensTemp(NULL);
            break;
        case dat_pay_gyro:
            pay_i_state = pay_get_state_gyro(NULL);
            break;
        case dat_pay_expFis:
            pay_i_state = pay_get_state_expFis(NULL);
            break;
        case dat_pay_last_one:
            //ignore
            pay_i_state = -1;
            break;
    }
    return pay_i_state;
}

/**
 * Set the state of pay_i. Used to control execution, by FP2 and others
 * @param pay_i
 * @param state
 */
void pay_set_state(DAT_Payload_Buff pay_i, PAY_xxx_State state){
    //change state
    int arg_state = state;
    switch(pay_i){
        case dat_pay_tmEstado:
            pay_set_state_tmEstado(&arg_state);
            break;
        case dat_pay_battery:
            pay_set_state_battery(&arg_state);
            break;
        case dat_pay_debug:
            pay_set_state_debug(&arg_state);
            break;
        case dat_pay_langmuirProbe:
            pay_set_state_langmuirProbe(&arg_state);
            break;
        case dat_pay_gps:
            pay_set_state_gps(&arg_state);
            break;
        case dat_pay_camera:
            pay_set_state_camera(&arg_state);
            break;
        case dat_pay_sensTemp:
            pay_set_state_sensTemp(&arg_state);
            break;
        case dat_pay_gyro:
            pay_set_state_gyro(&arg_state);
            break;
        case dat_pay_expFis:
            pay_set_state_expFis(&arg_state);
            break;
        case dat_pay_last_one:
            //ignore
//            state = state;
            break;
    }

}

/**
 * 
 * @param pay_i
 */
void pay_save_date_time_to_Payload_Buff(DAT_Payload_Buff pay_i){
    int sec, min, hour, mo, dd, yy;
    sec = RTC_get_seconds();
    min = RTC_get_minutes();
    hour = RTC_get_hours();
    mo = RTC_get_month();
    dd = RTC_get_day_num();
    yy = RTC_get_year();
    unsigned long int date_time = RTC_encode_datetime(yy, mo, dd, hour, min, sec);

    //print date time
    RTC_decode_datetime(date_time, 0);

    unsigned int dt1, dt2;
    dt1 = (unsigned int )(date_time>>0);
    dt2 = (unsigned int )(date_time>>16);
    dat_set_Payload_Buff(pay_i, dt1);
    dat_set_Payload_Buff(pay_i, dt2);
}
