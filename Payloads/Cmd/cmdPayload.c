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

    payFunction[(unsigned char)pay_id_isAlive_expFis] = pay_isAlive_expFis;
    payFunction[(unsigned char)pay_id_get_state_expFis] = pay_get_state_expFis;
    payFunction[(unsigned char)pay_id_set_state_expFis] = pay_set_state_expFis;
    payFunction[(unsigned char)pay_id_init_expFis] = pay_init_expFis;
    payFunction[(unsigned char)pay_id_take_expFis] = pay_take_expFis;
    payFunction[(unsigned char)pay_id_stop_expFis] = pay_stop_expFis;
    payFunction[(unsigned char)pay_id_debug_expFis] = pay_debug_expFis;

    payFunction[(unsigned char)pay_id_isAlive_battery] = pay_isAlive_battery;
    payFunction[(unsigned char)pay_id_get_state_battery] = pay_get_state_battery;
    payFunction[(unsigned char)pay_id_set_state_battery] = pay_set_state_battery;
    payFunction[(unsigned char)pay_id_init_battery] = pay_init_battery;
    payFunction[(unsigned char)pay_id_take_battery] = pay_take_battery;
    payFunction[(unsigned char)pay_id_stop_battery] = pay_stop_battery;

    payFunction[(unsigned char)pay_id_isAlive_debug] = pay_isAlive_debug;
    payFunction[(unsigned char)pay_id_get_state_debug] = pay_get_state_debug;
    payFunction[(unsigned char)pay_id_set_state_debug] = pay_set_state_debug;
    payFunction[(unsigned char)pay_id_init_debug] = pay_init_debug;
    payFunction[(unsigned char)pay_id_take_debug] = pay_take_debug;
    payFunction[(unsigned char)pay_id_stop_debug] = pay_stop_debug;

    payFunction[(unsigned char)pay_id_isAlive_lagmuirProbe] = pay_isAlive_lagmuirProbe;
    payFunction[(unsigned char)pay_id_get_state_lagmuirProbe] = pay_get_state_lagmuirProbe;
    payFunction[(unsigned char)pay_id_set_state_lagmuirProbe] = pay_set_state_lagmuirProbe;
    payFunction[(unsigned char)pay_id_init_lagmuirProbe] = pay_init_lagmuirProbe;
    payFunction[(unsigned char)pay_id_take_lagmuirProbe] = pay_take_lagmuirProbe;
    payFunction[(unsigned char)pay_id_stop_lagmuirProbe] = pay_stop_lagmuirProbe;
    payFunction[(unsigned char)pay_id_debug_langmuirProbe] = pay_debug_langmuir;
    payFunction[(unsigned char)pay_id_send_to_lagimur] = pay_send_to_lagimur;
}


/**
 *
 * @param param
 * @return
 */
int pay_fp2_default_fsm(void *param){
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
        stat = dat_set_Payload_Buff(pay_i, value, DAT_PAYBUFF_MODE_NO_MAXINDX);
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

//******************************************************************************
int pay_debug_expFis(void *param){
        /*
     * Se usa T4 para escribir_DAC y T5 para leer_ADC,
     * para conservar Nyquist se hace que Periodo_T4=Periodo_T5*3
     * Asi se lee 3 veces por cada esritura del DAC.
     *
     * En resumen, la variable controlada es Periodo_T5 que es igual a
     * Periodo leer_ADC = Fosc/2*256*value = (1/16Mhz)/256*value [s],
     * con value una variable entera (unsigned int)
     */
//            fis_erase_sens_buff();
//            unsigned int ADC_period = 20000;
//            fis_start_expFis(ADC_period); //DAC is always 3*(pay_ADCperiod)
//            fis_wait_busy_wtimeout();
//            fis_print_sens_buff();

    static int ft;
    int buff_len;
    if(ft==0){
        ft = 1;
        static unsigned int ADC_period[] = {10000, 5000, 1000, 500, 100, 50, 10};
        int len = 7;
        int rounds_per_ADC_period = 3;
        buff_len = fis_iterate_config(ADC_period, len, rounds_per_ADC_period);
    }
    if( fis_iterate() ){
        //fis_print_sens_buff();
        //fis_get_sens_buff_i(indx);
    }
    if( fis_iterate_isComplete() ){
        printf("fis_iterate_isComplete() return TRUE\n");
        ft = 0; //repeat it over
    }

    return 1;
}
int pay_isAlive_expFis(void *param){
    /*
     * This Payload is mainly (DAC seems to basic to check isAlive with it)
     * Sw/PIC24 based, so it's assummed to always be aliwe.
     */
    return 1;
}
int pay_get_state_expFis(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_expFis_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_expFis(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_expFis_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
static int expFis_gpb_indx;
int pay_init_expFis(void *param){
    printf("pay_init_expFis\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_expFis;

    static unsigned int ADC_period[] = {10000, 5000, 1000, 500, 100, 50, 10};
    int len = 7;
    int rounds_per_ADC_period = 3;
    int buff_len = fis_iterate_config(ADC_period, len, rounds_per_ADC_period);

    lenBuff = (buff_len*len*rounds_per_ADC_period);
    dat_reset_Payload_Buff(pay_i, lenBuff, 0);

    //configure Payload
    expFis_gpb_indx = 0;    //Reset vars
    int res = 1;    //always alive

    //debug Info
    printf("  sta_pay_expFis_isAlive = %d \r\n", sta_get_stateVar(sta_pay_expFis_isAlive) );

    return res;
}
int pay_take_expFis(void *param){

    //save date_time in 2ints
    //pay_save_date_time_to_Payload_Buff(dat_pay_expFis);
    return 1;

    if( fis_iterate() ){
        //fis_print_sens_buff();
        //fis_get_sens_buff_i(indx);
        int ind;
        for(ind=0;ind<FIS_SENS_BUFF_LEN;ind++){
            //Set DAT_GnrlPurpBuff
            //dat_setGPB(dat_auxBuff_0, gpb_indx, fis_get_sens_buff_i(ind) );
            expFis_gpb_indx++;
        }
    }
    
    return 1;
}
int pay_stop_expFis(void *param){
    printf("pay_stop_expFis\r\n");

    if( fis_iterate_isComplete() ){
        printf("fis_iterate_isComplete() returned TRUE\n");
    }
    #if (_VERBOSE_>=2)
        char ret[10];
        DAT_GnrlPurpBuff aux_i;
        int j, val;
        unsigned int max=(FIS_REPEAT_PER_ROUND*FIS_SENS_BUFF_LEN);

        for(aux_i=dat_auxBuff_0; aux_i<=dat_auxBuff_9; aux_i++){

            printf("  aux_i=");
            sprintf (ret, "%d", (unsigned int)aux_i);
            printf(ret); printf("\r\n");

            for(j=0;j<max;j++){
                //Get
                printf("  dat_get_GPB[");
                sprintf (ret, "%d", (unsigned int)j);
                printf(ret); printf("]");
                val = dat_getGPB(aux_i, j);
                sprintf (ret, "%d", (unsigned int)val);
                printf(" = "); printf(ret); printf("\r\n");
            }
        }
    #endif

    return 1;
}
//static int expFis_fr;
//int pay_init_expFis(void *param){
//    printf("pay_init_expFis\r\n");
//
//    DAT_Payload pay_i; unsigned int maxIndx;
//    pay_i = dat_pay_expFis;
//    lenBuff = (unsigned int)(2); //valor exacto sera seteado en pay_take, o pay_stop
//    dat_resetPayload_Buffer(pay_i, lenBuff, 1);
//
//    //Reset expFis
//    expFis_fr=0;
//
//    return 1;
//}
//int pay_take_expFis(void *param){
//    char ret[10];
//    static unsigned int pay_round_cnt, pay_ADCperiod;
//    static DAT_GnrlPurpBuff pay_aux_i;
//    int rst_gbp_indx;
//
//    printf("  pay_take_expFis:\r\n");
//
//    //inicializacion de variables
//    if(expFis_fr==0){
//        expFis_fr++;
//        pay_aux_i=dat_auxBuff_0;
//        pay_round_cnt=0;
//        pay_ADCperiod=0;
//        rst_gbp_indx=0;
//    }
//
//    //si la frecuencia pay_aux_i debe parar y pasar a la sgte
//    if(pay_round_cnt == FIS_REPEAT_PER_ROUND){
//
//        #if (_VERBOSE_>=2)
//            int j, val; unsigned int max=(2*FIS_SENS_BUFF_LEN);//(FIS_REPEAT_PER_ROUND*FIS_SAMP_PER_ROUND);
//            for(j=0;j<max;j++){
//                printf("  dat_get_GPB[");
//                sprintf (ret, "%d", (unsigned int)j);
//                printf(ret); printf("]");
//                val = dat_getGPB(pay_aux_i, j);
//                sprintf (ret, "%d", (unsigned int)val);
//                printf(" = "); printf(ret); printf("\r\n");
//            }
//        #endif
//
//        pay_round_cnt=0;
//        rst_gbp_indx=1;
//        pay_aux_i++;
//
//        //si el ciclo debe terminar
//        if(pay_aux_i == (dat_auxBuff_9+1) ){
//            //lleno buffer para terminar el ciclo de pay_take_expFis()..
//            dat_setPayloadVar( dat_pay_expFis, 0xA00);
//            dat_setPayloadVar( dat_pay_expFis, 0xAA01);
//            return 1;
//        }
//    }
//
//    #if (_VERBOSE_>=1)
//        printf("  pay_round_cnt=");
//        sprintf (ret, "%d", (unsigned int)pay_round_cnt);
//        printf(ret); printf("\r\n");
//
//        printf("  pay_aux_i=");
//        sprintf (ret, "%d", (unsigned int)pay_aux_i);
//        printf(ret); printf("\r\n");
//    #endif
//
//    pay_ADCperiod = fis_frec_i_to_ADC_period( pay_aux_i);
//    fis_start_expFis(pay_ADCperiod); //DAC is always 3*(pay_ADCperiod)
//    fis_save_sens_buff_to_GPB(pay_aux_i, rst_gbp_indx );
//
//    //prepara la prox iteracion de la pay_aux_i
//    pay_round_cnt++;
//    rst_gbp_indx=0;
//
//    return 1;
//}
//int pay_stop_expFis(void *param){
//    printf("pay_stop_expFis\r\n");
//
//    #if (_VERBOSE_>=2)
//        char ret[10];
//        DAT_GnrlPurpBuff aux_i;
//        int j, val;
//        unsigned int max=(FIS_REPEAT_PER_ROUND*FIS_SENS_BUFF_LEN);
//
//        for(aux_i=dat_auxBuff_0; aux_i<=dat_auxBuff_9; aux_i++){
//
//            printf("  aux_i=");
//            sprintf (ret, "%d", (unsigned int)aux_i);
//            printf(ret); printf("\r\n");
//
//            for(j=0;j<max;j++){
//                //Get
//                printf("  dat_get_GPB[");
//                sprintf (ret, "%d", (unsigned int)j);
//                printf(ret); printf("]");
//                val = dat_getGPB(aux_i, j);
//                sprintf (ret, "%d", (unsigned int)val);
//                printf(" = "); printf(ret); printf("\r\n");
//            }
//        }
//    #endif
//
//    return 1;
//}
//******************************************************************************
int pay_isAlive_battery(void *param){
    //EPS based
    return eps_isAlive(FALSE);
}
int pay_get_state_battery(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_battery_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_battery(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_battery_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}

int pay_init_battery(void *param){
    printf("pay_init_battery() ..\r\n");

    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_battery;
    lenBuff = (unsigned int)(500*4);   //desde 0x00 a 0xFF
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);


    return pay_isAlive_battery(NULL);
}
int pay_take_battery(void *param){
    printf("pay_take_battery()  ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_battery);

    //save data
    int i, val;
    for(i=0; i<34; i++){
        val = eps_readreg( (void *)(&i) );
        dat_set_Payload_Buff(dat_pay_battery, val, DAT_PAYBUFF_MODE_NO_MAXINDX);
    }

    return 1;
}
int pay_stop_battery(void *param){
    printf("pay_stop_battery()  ..\r\n");
    return 1;
}
//******************************************************************************
int pay_isAlive_debug(void *param){
    /*
     * This Payload is Sw based, so it's assummed to always be aliwe.
     */
    return 1;
}
int pay_get_state_debug(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_debug_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_debug(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_debug_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
static unsigned int pay_debug_cnt;
int pay_init_debug(void *param){
    printf("pay_init_debug\r\n");

    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_debug;
    lenBuff = (unsigned int)(500);    //payload en etapa experimental aun
    dat_reset_Payload_Buff(pay_i, lenBuff, 0);

    //restart counter
    pay_debug_cnt = 0;

    return pay_isAlive_debug(NULL);
}
int pay_take_debug(void *param){
    printf("pay_take_debug()  ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_debug);

    //save data
    pay_debug_cnt++;
    dat_set_Payload_Buff(dat_pay_debug, pay_debug_cnt, DAT_PAYBUFF_MODE_NO_MAXINDX);

    return 1;
}
int pay_stop_debug(void *param){
    printf("pay_stop_debug()  ..\r\n");

    return 1;
}
//******************************************************************************
int pay_isAlive_gyro(void *param){
    return gyr_isAlive();
}
int pay_get_state_gyro(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gyro_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_gyro(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gyro_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_debug_gyro(void *param){

//    BOOL st = gyr_isAlive();
//    printf("dig_isAlive() = %d\n", (int)st);

    BOOL verb = *((int *)param);
    BOOL st = gyr_init_config();
    GYR_DATA res_data;
    gyr_take_samples(verb, &res_data);
    printf("pay_debug_gyro\r\n");
    printf("X axis : %d\n", (res_data).a_x );
    printf("Y axis : %d\n", (res_data).a_y );
    printf("Z axis : %d\n", (res_data).a_z );
    printf("************************\n");

    return (int)st;
}
int pay_init_gyro(void *param){
    printf("pay_init_gyro\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gyro;
    lenBuff = (unsigned int)(500*3);  //(1440*3)      //numero de 10-minutos en un dia
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);

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
    printf("  sta_pay_gyro_isAlive = %d \r\n", sta_get_stateVar(sta_pay_gyro_isAlive) );
    printf("  PPC_GYRO_INT2_CHECK = %d \r\n", PPC_GYRO_INT2_CHECK );

    return res;
}
int pay_take_gyro(void *param){
    printf("pay_take_gyro()  ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_gyro);

    //in case of failure
    if( pay_isAlive_gyro(NULL) == 0){
        dat_set_Payload_Buff(dat_pay_gyro ,0xFAFA, DAT_PAYBUFF_MODE_NO_MAXINDX);      
        return 1;
    }

    //save data
    GYR_DATA res_data;
    gyr_take_samples(FALSE, &res_data);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_x, DAT_PAYBUFF_MODE_NO_MAXINDX);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_y, DAT_PAYBUFF_MODE_NO_MAXINDX);
    dat_set_Payload_Buff(dat_pay_gyro, res_data.a_z, DAT_PAYBUFF_MODE_NO_MAXINDX);
    
    return 1;
}
int pay_stop_gyro(void *param){
    printf("pay_stop_gyro()  ..\r\n");
    return 1;
}
//******************************************************************************
int pay_isAlive_tmEstado(void *param){
    /*
     * This Payload is Sw based, so it's assummed to always be aliwe.
     */
    return 1;
}
int pay_get_state_tmEstado(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_tmEstado_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_tmEstado(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_tmEstado_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_init_tmEstado(void *param){
    printf("pay_init_tmEstado\r\n");

    //configure PaylaodBuff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_tmEstado;
    lenBuff = (unsigned int)(40*sta_stateVar_last_one);  //(4*60/5=48)      //numero de 5-minutos en una orbita (4 horas)
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);

    //isAlive
    int res_isAlive = sta_get_stateVar(sta_pay_tmEstado_isAlive);

    //debug info
    #if (_VERBOSE_>=2)
        printf("  sta_pay_tmEstado_isAlive = %d \r\n", res_isAlive );
    #endif

    return res_isAlive;
}

int pay_take_tmEstado(void *param){
    printf("pay_take_tmEstado()  ..\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_tmEstado);

    //save data
    STA_StateVar indxVar; int var;
    for(indxVar=0; indxVar<sta_stateVar_last_one; indxVar++){
        var = sta_get_stateVar(indxVar);
        //dat_set_Payload_Buff(dat_pay_tmEstado, var, DAT_PAYBUFF_MODE_NO_MAXINDX);
        #if (_VERBOSE_>=1)
            printf("sta_get_stateVar[%s] = %d\r\n", sta_varToString(indxVar), var);
        #endif
    }

    return 1;
}
int pay_stop_tmEstado(void *param){
    printf("pay_stop_tmEstado()  ..\r\n");
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
    return cam_isAlive();
}
int pay_get_state_camera(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_camera_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_camera(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_camera_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_init_camera(void *param){
    printf("pay_init_camera\r\n");

    //switch camera on
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    PPC_CAM_SWITCH=1;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    printf("  PPC_CAM_HOLD_CHECK = %d\r\n", PPC_CAM_HOLD_CHECK);

    // Wait for the camera to be ready
    __delay_ms(3000);
    
    //configure Payload
    int res;
    int issync = cam_sync(FALSE);
    if(issync == 0x0000){ res = 1; }
    else{ res = 0; }

    //debug info
    printf("  PPC_CAM_HOLD_CHECK = %d\r\n", PPC_CAM_HOLD_CHECK);
    printf("  sta_pay_camera_isAlive = %d \r\n", sta_get_stateVar(sta_pay_camera_isAlive) );

    return res;
}
int pay_take_camera(void *param){
    printf("pay_take_camera()\r\n");

    //save date time of the photo
    //pay_save_date_time_to_Payload_Buff(dat_pay_camera);   //save date_time in 2ints

    //in case of failure
    if( sta_get_stateVar(sta_pay_camera_isAlive)==0 ){
        printf("camera is not alive!..\r\n");
        dat_set_Payload_Buff(dat_pay_camera ,0xFAFA, DAT_PAYBUFF_MODE_NO_MAXINDX);
        return 1;
    }

    BOOL st = pay_cam_takeAndSave_photo(0x07, 0x00, 0x05);
    //BOOL st = pay_cam_takeAndSave_photo(0x02, 0x00, 0x05);
    //BOOL st = pay_cam_takeAndSave_photo(0x02, 0x00, 0x05);

    return st;
}
int pay_stop_camera(void *param){
    printf("pay_stop_camera()\r\n");

    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);
    PPC_CAM_SWITCH=0;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_CAM_SWITCH = %d\r\n", PPC_CAM_SWITCH_CHECK);

    return 1;
}
BOOL pay_cam_takeAndSave_photo(int resolution, int qual, int pic_type){
    printf("pay_takeAndSave_photo ..\r\n");

    printf(" Taking photo ..\r\n");
    unsigned int photo_byte_length= cam_photo(resolution, qual, pic_type);
    #if (_VERBOSE_>=1)
        printf("    Photo length = %u\r\n", photo_byte_length);
    #endif

    //in case of errors
    if(photo_byte_length == 0){
        printf(" Error: No photo was taken ..\r\n");
        dat_set_Payload_Buff(dat_pay_camera ,0xFAFA, DAT_PAYBUFF_MODE_NO_MAXINDX);
        return FALSE;
    }

    //calculate  length in ints
    unsigned int photo_int_length = photo_byte_length/2;    //se guardan 2byten en 1int
    printf("  Debug info: photo_int_length = %u, photo_byte_length = %d\r\n",
            photo_int_length, photo_byte_length);

    //Inicializa la estructura de data payload
    dat_reset_Payload_Buff(dat_pay_camera, photo_int_length, 0);

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

        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[0], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[1], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[2], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[3], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[4], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[5], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[6], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[7], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[8], DAT_PAYBUFF_MODE_NO_MAXINDX);
        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[9], DAT_PAYBUFF_MODE_NO_MAXINDX);

        ClrWdt();
//        printf("  Debug info: saving [%d/%d] ..\r\n",
//                            iter, num_10sections);
    }

    for(iter = 0; iter<rest_10sections; iter++)
    {
        //get 1 int out of 2 bytes
        int_r[0] = pay_camera_get_1int_from_2bytes();

        dat_set_Payload_Buff( dat_pay_camera, (int)int_r[0], DAT_PAYBUFF_MODE_NO_MAXINDX);

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
    int st = pay_take_camera(NULL);
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
    return 0;
}
int pay_get_state_gps(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gps_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_gps(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gps_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_init_gps(void *param){
    printf("pay_init_gps\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gps;
    lenBuff = (unsigned int)(200);   //(1440)  //numero de 10-minutos en un dia
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);

    return 0;
//    //configure Payload
//    int res;
//    res = 0;    //not implemented yet => always dead ..
//
//    //check SW and CHECK pins (not definitive)
//    PPC_GPS_SWITCH = 0;
//    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
//    PPC_GPS_SWITCH = 1;
//    __delay_ms(50); //wait while port write takes effect
//    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
//    printf("  sta_pay_gps_isAlive = %d \r\n", sta_get_stateVar(sta_pay_gps_isAlive) );
//
//    return pay_isAlive_gps(NULL);
}
int pay_take_gps(void *param){
    printf("pay_take_gps\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_gps);

    //in case of failure
    if( pay_isAlive_gps(NULL) == 0 ){
        printf("gps is not alive!..\r\n");
        dat_set_Payload_Buff(dat_pay_gps ,0xFAFA, DAT_PAYBUFF_MODE_NO_MAXINDX);
        //turn GPS off
        PPC_GPS_SWITCH = 0;
        return 0;
    }

    //save data

    return 1;
}
int pay_stop_gps(void *param){
    printf("pay_stop_gps\r\n");

    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    PPC_GPS_SWITCH = 0;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );

    return 1;
}
//******************************************************************************
BOOL pay_deploy_langmuirProbe(int realtime){
    printf("******************************\r\n");
    printf("Deployng LangmuirProbe\r\n");
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );
    PPC_LANGMUIR_DEP_SWITCH = 1;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    if(realtime==1){
        __delay_ms(30000); //wait 30sec to burn nylon
        __delay_ms(30000); //wait 30sec to burn nylon
        ClrWdt();
        __delay_ms(30000); //wait 30sec to burn nylon
    }
    else{
        __delay_ms(3000); //wait 30sec to burn nylon
    }

    PPC_LANGMUIR_DEP_SWITCH = 0;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    return TRUE;
}
int pay_isAlive_lagmuirProbe(void *param){
    return langmuir_isAlive();
}
int pay_get_state_lagmuirProbe(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_lagmuirProbe_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_lagmuirProbe(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_lagmuirProbe_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_init_lagmuirProbe(void *param){
    printf("pay_init_lagmuirProbe\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_lagmuirProbe;
    lenBuff = (unsigned int)(20*1); /*TODO: SET VALUE*/ //(1440)      //numero de 10-minutos en un dia
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);

    //configs
    int res_isAlive = pay_isAlive_lagmuirProbe(NULL); //not fully implemented yet => always dead
    lag_erase_buffer();

    //debug info
    printf("  sta_pay_lagmuirProbe_isAlive = %d \r\n", res_isAlive );
    printf("  sta_pay_lagmuirProbe_isDeployed = %d \r\n", sta_get_stateVar(sta_pay_lagmuirProbe_isDeployed) );

    return res_isAlive;
}

int pay_take_lagmuirProbe(void *param){
    printf("pay_take_lagmuirProbe\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_lagmuirProbe);

    //save data
    dat_set_Payload_Buff(dat_pay_lagmuirProbe, 0x01, DAT_PAYBUFF_MODE_NO_MAXINDX);

////    int verbose = *(int *)param;
//    int ok = 0;
//    int i = 0;
//    /* Tomar datos de calibracion */
//    ok = lag_read_cal_packet(FALSE);
//    if(ok)
//    {
//        /* Guardar datos a la SD (40) */
//        for(i=0; i<40; i++)
//        {
//            dat_set_Payload_Buff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
//        }
////        if(verbose) SendRS232(langmuir_buffer, 40, RS2_M_UART1);
//    }
//
//    /* Tomar datos de plasma */
//    ok = lag_read_plasma_packet(FALSE);
//    if(ok)
//    {
//        /* Guardar datos a la SD (10) */
//        for(i=0; i<10; i++)
//        {
//            dat_set_Payload_Buff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
//        }
////        if(verbose) SendRS232(langmuir_buffer, 10, RS2_M_UART1);
//    }
//
//    /* Tomar datos de sweep */
//    ok = lag_read_sweep_packet(FALSE);
//    if(ok)
//    {
//        /* Guardar datos a la SD (1096) */
//        for(i=0; i<1096; i++)
//        {
//            dat_set_Payload_Buff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
//        }
////        if(verbose) SendRS232(langmuir_buffer, 1096, RS2_M_UART1);
//    }
//    #endif


    return 1;
}

/**
 * Comando para debug de langmuir. Muestra en panatalla los datos solicitados
 * el instrumento, que puede ser: calibraciom, plasma o sweep.
 * @param param 0-calibracion, 1-plasma, 2-sweep
 * @return Resultado de la operacion: 1-exito, 0-error
 */
int pay_debug_langmuir(void *param)
{
    printf("pay_debug_lagmuir\r\n");
    int mode = *(int *)param;
    int ok = 0;

    switch (mode)
    {
        case 0:
            ok = lag_read_cal_packet(TRUE);
            break;
        case 1:
            ok = lag_read_plasma_packet(TRUE);
            break;
        case 2:
            ok = lag_read_sweep_packet(TRUE);
            break;
        default:
            ok = 0;
            break;
    }

    if(ok)
    {
        printf("[langmuir] OK");
    }
    else
    {
        printf("[langmuir] ERROR");
    }

    printf("\r\n");

    return ok;
}

int pay_stop_lagmuirProbe(void *param){
    printf("pay_stop_lagmuirProbe()  ..\r\n");
    
    lag_erase_buffer();

    return 1;
}

int pay_send_to_lagimur(void *param)
{
    printf("en pay_send_to_lagmuir\r\n");
    /*
    char c=*((char *)param);
    pay_lagimur_printf(&c);
     * */
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
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_set_state_sensTemp(void *param){
    int value = *( (int*)param );
    MemEEPROM_Vars mem_eeprom_var = mem_pay_sensTemp_state;
    writeIntEEPROM1(mem_eeprom_var, value);
    return 1;
}
int pay_init_sensTemp(void *param){
    printf("pay_init_sensTemp\r\n");

    //configure Payload_Buff
    DAT_Payload_Buff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_sensTemp;
    lenBuff = (unsigned int)(500*4) + 4;    //numero de muestras*4 + estado de isAlive cada sensor
    dat_reset_Payload_Buff(pay_i, lenBuff, 1);

    //configure Payload
    BOOL s1, s2, s3, s4;
    s1 = sensTemp_init(ST1_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s1, DAT_PAYBUFF_MODE_NO_MAXINDX);

    s2 = sensTemp_init(ST2_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s2, DAT_PAYBUFF_MODE_NO_MAXINDX);

    s3 = sensTemp_init(ST3_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s3, DAT_PAYBUFF_MODE_NO_MAXINDX);

    s4 = sensTemp_init(ST4_ADDRESS);
    dat_set_Payload_Buff(dat_pay_sensTemp, (int)s4, DAT_PAYBUFF_MODE_NO_MAXINDX);

    int res_isAlive = sta_get_stateVar(sta_pay_sensTemp_isAlive);

    //debug info
    printf("  sta_pay_sensTemp_isAlive = %d \r\n", res_isAlive );

    return res_isAlive;
}
int pay_take_sensTemp(void *param){
    printf("pay_take_sensTemp\r\n");

    //save date_time in 2ints
    pay_save_date_time_to_Payload_Buff(dat_pay_sensTemp);

    //in case of failure
    if( pay_isAlive_sensTemp(NULL) == 0){
        printf("sensTemp is not alive!..\r\n");
        dat_set_Payload_Buff(dat_pay_sensTemp ,0xFAFA, DAT_PAYBUFF_MODE_NO_MAXINDX);
        return 0;
    }

    //save data
    int val;
    val=sensTemp_take(ST1_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val, DAT_PAYBUFF_MODE_NO_MAXINDX);
    val=sensTemp_take(ST2_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val, DAT_PAYBUFF_MODE_NO_MAXINDX);
    val=sensTemp_take(ST3_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val, DAT_PAYBUFF_MODE_NO_MAXINDX);
    val=sensTemp_take(ST4_ADDRESS, FALSE);
    dat_set_Payload_Buff(dat_pay_sensTemp, val, DAT_PAYBUFF_MODE_NO_MAXINDX);

    return 1;
}
int pay_stop_sensTemp(void *param){
    printf("pay_stop_sensTemp\r\n");
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
    #if (SCH_FLIGHTPLAN2_VERBOSE>=2)
        printf("pay_fp2_i_multiplexing => pay_i = %d\r\n", (unsigned int)current_pay_i);
    #endif

    PAY_xxx_State pay_i_state = sta_get_stateVar(sta_DAT_Payload_Buff_to_STA_StateVar(current_pay_i));
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

    //print time
    rtc_print(NULL);

    for(pay_i = 0; pay_i < dat_pay_last_one; pay_i++)
    {
        //continue if it's no time for pay_i yet
        pay_i_tick_rate = pay_fp2_get_exec_rate(pay_i);
        if( exec_tick%pay_i_tick_rate != 0 ){continue;}

        #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
            printf("  pay_i = %d = %s \r\n", pay_i, dat_get_payload_name(pay_i) );
            printf("  exec_tick = %lu \r\n", exec_tick);
            printf("  pay_i_tick_rate = %d \r\n", pay_i_tick_rate);
        #endif

        pay_i_state = sta_get_stateVar(sta_DAT_Payload_Buff_to_STA_StateVar(pay_i));
        switch(pay_i_state){
        //**********************************************************************
            case pay_xxx_state_inactive:
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_inactive \r\n");
                #endif

                break;
        //**********************************************************************
            case pay_xxx_state_active:
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_active \r\n");
                #endif

                //change state to sta_pay_xxx_state_run_init
                pay_set_state(pay_i, pay_xxx_state_run_init);

                break;
        //**********************************************************************
            case pay_xxx_state_run_init:
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
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

                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
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
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
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
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = sta_pay_xxx_state_waiting_tx \r\n");
                #endif

                //change state to sta_pay_xxx_state_active only if pay_i TX is done
                if(FALSE){
                    pay_set_state(pay_i, pay_xxx_state_active);
                }

                break;
        //**********************************************************************
            default:
                #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
                    printf("  state = [ERROR] Unknown State \r\n");
                #endif
                break;
        //**********************************************************************
        }

        #if (SCH_FLIGHTPLAN2_VERBOSE>=1)
            printf("---------------------------------------------\r\n");
        #endif

    }

    //print time
    rtc_print(NULL);
}

/**
 * Return the number of tick before a pay_i is to be executed
 * @param pay_i
 * @return Return the number of tick before a pay_i is to be executed
 */
int pay_fp2_get_exec_rate(DAT_Payload_Buff pay_i){
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

/**
 * Return the number of times pay_i is to be executed
 * @param pay_i
 * @return Return TRUE = isDOne, FLASE = contiune pay_take
 */
unsigned int pay_fp2_get_run_take_num_exec_times(DAT_Payload_Buff pay_i){
    unsigned int max_exec_times;

    switch(pay_i){
        case dat_pay_tmEstado:
            max_exec_times = 9;
            break;
        case dat_pay_battery:
            max_exec_times = 8;
            break;
        case dat_pay_debug:
            max_exec_times = 7;
            break;
        case dat_pay_lagmuirProbe:
            max_exec_times = 6;
            break;
        case dat_pay_gps:
            max_exec_times = 5;
            break;
        case dat_pay_camera:
            max_exec_times = 4;
            break;
        case dat_pay_sensTemp:
            max_exec_times = 3;
            break;
        case dat_pay_gyro:
            max_exec_times = 2;
            break;
        case dat_pay_expFis:
            max_exec_times = 1;
            break;
        case dat_pay_last_one:
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
        case dat_pay_lagmuirProbe:
            switch (state){
                case pay_xxx_state_run_init:
                    arg = 0;
                    pay_init_lagmuirProbe(&arg);
                    break;
                case pay_xxx_state_run_take:
                    pay_take_lagmuirProbe(&arg);
                    break;
                case pay_xxx_state_run_stop:
                    pay_stop_lagmuirProbe(&arg);
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
        case dat_pay_lagmuirProbe:
            pay_i_state = pay_get_state_lagmuirProbe(NULL);
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
        case dat_pay_lagmuirProbe:
            pay_set_state_lagmuirProbe(&arg_state);
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
    dat_set_Payload_Buff(pay_i, dt1, DAT_PAYBUFF_MODE_NO_MAXINDX);
    dat_set_Payload_Buff(pay_i, dt2, DAT_PAYBUFF_MODE_NO_MAXINDX);
}