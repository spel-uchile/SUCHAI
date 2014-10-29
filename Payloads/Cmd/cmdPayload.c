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


cmdFunction payFunction[PAY_NCMD];
int pay_sysReq[PAY_NCMD];

#define _VERBOSE_ 1
#define TEST2_MSJS 1

void pay_onResetCmdPAY(void){
    printf("        pay_onResetCmdPAY\n");

    int i;
    for(i=0; i<PAY_NCMD; i++) pay_sysReq[i] = CMD_SYSREQ_MIN;

    payFunction[(unsigned char)pay_id_test_dataRepo] = pay_test_dataRepo;
    payFunction[(unsigned char)pay_id_FSM_default] = pay_FSM_default;

    payFunction[(unsigned char)pay_id_isAlive_tmEstado] = pay_isAlive_tmEstado;
    payFunction[(unsigned char)pay_id_get_state_tmEstado] = pay_get_state_tmEstado;
    payFunction[(unsigned char)pay_id_init_tmEstado] = pay_init_tmEstado;
    payFunction[(unsigned char)pay_id_take_tmEstado] = pay_take_tmEstado;
    payFunction[(unsigned char)pay_id_stop_tmEstado] = pay_stop_tmEstado;

    payFunction[(unsigned char)pay_id_isAlive_sensTemp] = pay_isAlive_sensTemp;
    payFunction[(unsigned char)pay_id_get_state_sensTemp] = pay_get_state_sensTemp;
    payFunction[(unsigned char)pay_id_init_sensTemp] = pay_init_sensTemp;
    payFunction[(unsigned char)pay_id_take_sensTemp] = pay_take_sensTemp;
    payFunction[(unsigned char)pay_id_stop_sensTemp] = pay_stop_sensTemp;
    payFunction[(unsigned char)pay_id_debug_sensTemp] = pay_debug_sensTemp;

    payFunction[(unsigned char)pay_id_isAlive_gyro] = pay_isAlive_gyro;
    payFunction[(unsigned char)pay_id_get_state_gyro] = pay_get_state_gyro;
    payFunction[(unsigned char)pay_id_init_gyro] = pay_init_gyro;
    payFunction[(unsigned char)pay_id_take_gyro] = pay_take_gyro;
    payFunction[(unsigned char)pay_id_stop_gyro] = pay_stop_gyro;
    payFunction[(unsigned char)pay_id_debug_gyro] = pay_debug_gyro;

    payFunction[(unsigned char)pay_id_isAlive_camera] = pay_isAlive_camera;
    payFunction[(unsigned char)pay_id_get_state_camera] = pay_get_state_camera;
    payFunction[(unsigned char)pay_id_init_camera] = pay_init_camera;
    payFunction[(unsigned char)pay_id_take_camera] = pay_take_camera;
    payFunction[(unsigned char)pay_id_stop_camera] = pay_stop_camera;
    payFunction[(unsigned char)pay_id_takePhoto_camera] = pay_takePhoto_camera;
    payFunction[(unsigned char)pay_id_debug_camera] = pay_debug_camera;

    payFunction[(unsigned char)pay_id_isAlive_gps] = pay_isAlive_gps;
    payFunction[(unsigned char)pay_id_get_state_gps] = pay_get_state_gps;
    payFunction[(unsigned char)pay_id_init_gps] = pay_init_gps;
    payFunction[(unsigned char)pay_id_take_gps] = pay_take_gps;
    payFunction[(unsigned char)pay_id_stop_gps] = pay_stop_gps;

    payFunction[(unsigned char)pay_id_isAlive_expFis] = pay_isAlive_expFis;
    payFunction[(unsigned char)pay_id_get_state_expFis] = pay_get_state_expFis;
    payFunction[(unsigned char)pay_id_init_expFis] = pay_init_expFis;
    payFunction[(unsigned char)pay_id_take_expFis] = pay_take_expFis;
    payFunction[(unsigned char)pay_id_stop_expFis] = pay_stop_expFis;
    payFunction[(unsigned char)pay_id_debug_expFis] = pay_debug_expFis;

    payFunction[(unsigned char)pay_id_isAlive_battery] = pay_isAlive_battery;
    payFunction[(unsigned char)pay_id_get_state_battery] = pay_get_state_battery;
    payFunction[(unsigned char)pay_id_init_battery] = pay_init_battery;
    payFunction[(unsigned char)pay_id_take_battery] = pay_take_battery;
    payFunction[(unsigned char)pay_id_stop_battery] = pay_stop_battery;

    payFunction[(unsigned char)pay_id_isAlive_debug] = pay_isAlive_debug;
    payFunction[(unsigned char)pay_id_get_state_debug] = pay_get_state_debug;
    payFunction[(unsigned char)pay_id_init_debug] = pay_init_debug;
    payFunction[(unsigned char)pay_id_take_debug] = pay_take_debug;
    payFunction[(unsigned char)pay_id_stop_debug] = pay_stop_debug;

    payFunction[(unsigned char)pay_id_isAlive_lagmuirProbe] = pay_isAlive_lagmuirProbe;
    payFunction[(unsigned char)pay_id_get_state_lagmuirProbe] = pay_get_state_lagmuirProbe;
    payFunction[(unsigned char)pay_id_init_lagmuirProbe] = pay_init_lagmuirProbe;
    payFunction[(unsigned char)pay_id_take_lagmuirProbe] = pay_take_lagmuirProbe;
    payFunction[(unsigned char)pay_id_stop_lagmuirProbe] = pay_stop_lagmuirProbe;
    payFunction[(unsigned char)pay_id_debug_langmuirProbe] = pay_debug_langmuir;
    payFunction[(unsigned char)pay_id_send_to_lagimur] = pay_send_to_lagimur;
}


//****************************************************************
int pay_debugPay(void *param){
    DAT_PayloadBuff pay_i = *((DAT_PayloadBuff*)param);
    int res;

    printf("debugPay:\r\n");
    switch(pay_i){
        case dat_pay_lagmuirProbe:
            printf("dat_pay_lagmuirProbe:\r\n");
        break;
        case dat_pay_debug:
            printf("dat_pay_debug\r\n");
        break;
        case dat_pay_gps:
            printf("dat_pay_gps\r\n");
        break;
        case dat_pay_camera:
            printf("dat_pay_camera\r\n");
            int issync;

            //encender camera
            PPC_CAM_SWITCH=1;
            printf("PPC_CAM_SWITCH = %d\n", PPC_CAM_SWITCH_CHECK);
            
            // Wait for the camera to be ready
            cam_wait_hold_wtimeout(TRUE);

            //initialize camera          
            issync = cam_sync(TRUE);
            printf("SYNC() arrojo: issync = 0x%X\n", issync);

            if(issync!=0x0000){
                printf("issync!=0x0000()\r\n");
                printf("Camera is broken (once again)\r\n");
            }

            //unsigned int larg= cam_photo(0x01, 0x00, 0x05);
            unsigned int larg= cam_photo(0x07, 0x00, 0x05);
            #if (_VERBOSE_>=1)
                char ret[6];
                printf("cam_photo() retorno ");
                sprintf (ret, "0x%X", (unsigned int)larg);
                printf(ret); printf("\r\n");
            #endif

            //int mode=CAM_MODE_BOTH;
            //int mode=CAM_MODE_SAVE_SD;
            int mode=CAM_MODE_VERBOSE;
            cam_receivePhoto(larg, mode);

            //apagar camera
            PPC_CAM_SWITCH=0;
            printf("PPC_CAM_SWITCH = %d\n", PPC_CAM_SWITCH_CHECK);
            
        break;
        case dat_pay_sensTemp:
            printf("dat_pay_sensTemp 1\r\n");
            sensTemp_init(ST1_ADDRESS);
            res = sensTemp_take(ST1_ADDRESS, TRUE);
            printf("dat_pay_sensTemp 2\r\n");
            sensTemp_init(ST2_ADDRESS);
            res = sensTemp_take(ST2_ADDRESS, TRUE);
            printf("dat_pay_sensTemp 3\r\n");
            sensTemp_init(ST3_ADDRESS);
            res = sensTemp_take(ST3_ADDRESS, TRUE);
            printf("dat_pay_sensTemp 4\r\n");
            sensTemp_init(ST4_ADDRESS);
            res = sensTemp_take(ST4_ADDRESS, TRUE);
        break;
        case dat_pay_gyro:
            printf("dat_pay_gyro\r\n");
            //config gyro
            gyr_init_config();
            GYR_DATA res_data;
            gyr_take_samples(FALSE, &res_data);
            printf ("a_x = %d\n", res_data.a_x);
            printf ("a_y = %d\n", res_data.a_y);
            printf ("a_z = %d\n", res_data.a_z);
        break;
        case dat_pay_tmEstado:
            printf("dat_pay_tmEstado\n");
        break;
        case dat_pay_battery:
            printf("dat_pay_test1\n");
        break;
        case dat_pay_expFis:
            printf("dat_pay_expFis\n");
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
            if(ft==0){
                ft = 1;
                static unsigned int ADC_period[] = {10000, 5000, 1000, 500, 100, 50, 10};
                int len = 7;
                int rounds_per_ADC_period = 3;
                fis_iterate_config(ADC_period, len, rounds_per_ADC_period);
            }
            if( fis_iterate() ){
                //fis_print_sens_buff();
                //fis_get_sens_buff_i(indx);
            }
            if( fis_iterate_isComplete() ){
                printf("fis_iterate_isComplete() return TRUE\n");
                ft = 0; //repeat it over
            }
        break;
        default:
            printf("dafault\n");
        break;
    }

//    printf("=>FIFO buffer check:\r\n");
//    unsigned int i, indx; int value=0xAA00, val; BOOL stat; char ret[10];
//    indx = dat_getNextPayIndx(pay_i);
//    printf("indx = dat_getNextPayIndx(pay_i);\r\nindx = ");
//    sprintf (ret, "0x%X", (unsigned int)indx);
//    printf(ret); printf("\r\n");
    return 1;
}
int pay_test_dataRepo(void *param){

    DAT_PayloadBuff pay_i=0;
    printf("=>FIFO buffer check:\r\n");
    unsigned int i, indx; int value=0xAA00, val; BOOL stat; char ret[10];
    for(i=0;i<0x000F;i++){
        indx = dat_get_NextPayIndx(pay_i);
        printf("indx = dat_getNextPayIndx(pay_i);\r\nindx = ");
        sprintf (ret, "0x%X", (unsigned int)indx);
        printf(ret); printf("\r\n");

        //escribo
        stat = dat_set_PayloadBuff( pay_i, value);
        printf("stat = dat_setPayloadVar( pay_i, value);\r\nstat =  ");
        if(stat==TRUE){ printf("TRUE\r\n"); }
        else{ printf("FALSE\nFin del test\r\n"); break; }

        //leo
        stat = dat_get_PayloadBuff( pay_i, indx, &val);
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

/**
 * Comando para manejar la ejecucion por default de un Payload indicado en el parametro.
 * Cada vez que se llama esta funcion, la FSM ejecuta el estado PAY_State correspondiente para pay_i
 * @param param Puntero a DAT_Payload con el pay_i que se quiere manejar
 * @return 1 success, 0 fail
 */
int pay_FSM_default(void *param){
    DAT_PayloadBuff pay_i = *((DAT_PayloadBuff*)param);

    static PAY_State pay_current_state_buff[dat_pay_last_one];
    static PAY_State pay_current_state;
    static int ini;

    //inicializacion luego de cada reset
    if(ini==0){
        for(ini=0;ini<dat_pay_last_one;ini++){
            pay_current_state_buff[ini]=0;
        }
        pay_current_state=0;
    }

    #if (_VERBOSE_>=1)
        printf("  FSM_default => pay_i = ");
        dat_print_pay_i_name(pay_i); printf("\r\n");
        printf("    ");
    #endif

    //asigna el estado actual al pay_i correspondiente
    pay_current_state = pay_current_state_buff[pay_i];

    //logica del cambio de estados_prox
    pay_current_state = pay_nextStateLogic(pay_current_state, pay_i);
    //guarda el estado proximo al pay_i correspondiente
    pay_current_state_buff[pay_i] = pay_current_state;

    //logica del estado_actual
    pay_currentStateLogic(pay_current_state, pay_i);

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
static int expFis_gpb_indx;
int pay_init_expFis(void *param){
    printf("pay_init_expFis\r\n");

    //configure PayloadBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_expFis;

    static unsigned int ADC_period[] = {10000, 5000, 1000, 500, 100, 50, 10};
    int len = 7;
    int rounds_per_ADC_period = 3;
    int buff_len = fis_iterate_config(ADC_period, len, rounds_per_ADC_period);

    lenBuff = (buff_len*len*rounds_per_ADC_period);
    dat_reset_PayloadBuff(pay_i, lenBuff, 0);

    //configure Payload
    expFis_gpb_indx = 0;    //Reset vars
    int res = 1;    //always alive

    //report isAlive status
    sta_setCubesatVar(sta_pay_expFis_isAlive, res);

    //debug Info
    printf("  sta_pay_expFis_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_expFis_isAlive) );

    return res;
}
int pay_take_expFis(void *param){
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
//    dat_resetPayloadBuffer(pay_i, lenBuff, 1);
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
static int battery_indx;
int pay_init_battery(void *param){
    printf("pay_init_test1\r\n");

    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_battery;
    lenBuff = (unsigned int)(500*4);   //desde 0x00 a 0xFF
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //Reiniciar test1
    battery_indx=0;

    return 1;
}
int pay_take_battery(void *param){
    printf("pay_take_test1()..\r\n");

    int i;
    for(i=0;i<50;i++){
        dat_set_PayloadBuff(dat_pay_battery, battery_indx);
        battery_indx++;
    }

    return 1;
}
int pay_stop_battery(void *param){
    printf("pay_stop_test1()..\r\n");
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
int pay_init_debug(void *param){
    printf("pay_init_debug\r\n");

    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_debug;
    lenBuff = (unsigned int)(500);    //payload en etapa experimental aun
    dat_reset_PayloadBuff(pay_i, lenBuff, 0);

    return 1;
}
int pay_take_debug(void *param){
    printf("pay_take_debug()..\r\n");

    //
    dat_set_PayloadBuff(dat_pay_debug, 0x01);
    dat_set_PayloadBuff(dat_pay_debug, 0x02);
    return 1;

    /*
    int tot_len=0;
    int ind=0; char ret[10];
    DAT_Payload pay_i; unsigned int maxIndx;
    pay_i = dat_pay_debug;

    dat_setPayloadVar(dat_pay_debug, ind);

    #if (TEST2_MSJS==1)
        //se definen mensajes
        char msj1[]="Pico pal que lo lee. Super Taldo\r\n";
        int len1=strlen(msj1);
        char msj2[]="En el cielo hay estrellas y yo digo que tu eres bella. Super Taldo\r\n";
        int len2=strlen(msj2);
//        char msj3[]="Hola mundo. \r\n";
//        int len3=strlen(msj3);

        //Resetea al buffer para adecuarse al tama�o correcto de TODOS los msjes
        tot_len=len1+len2;
        printf("tot_len = ");
        sprintf (ret, "%d", (unsigned int)tot_len);
        printf(ret);printf("\r\n");
        lenBuff = (unsigned int)(tot_len);
        dat_resetPayloadBuffer(pay_i, lenBuff, 0);
        
        //guarda msj1
        for(ind=0;ind<len1;ind++){
            dat_setPayloadVar(dat_pay_debug, msj1[ind]);
        }
        
        //guarda msj2
        for(ind=0;ind<len2;ind++){
            dat_setPayloadVar(dat_pay_debug, msj2[ind]);
        }

    #else
        //Resetea al buffer para adecuarse al tama�o correcto de TODOS los msjes
        tot_len=102;    //len1+len2;
        printf("tot_len = ");
        sprintf (ret, "%d", (unsigned int)tot_len);
        printf(ret);printf("\r\n");
        lenBuff = (unsigned int)(tot_len);
        dat_resetPayloadBuffer(pay_i, lenBuff, 0);
    #endif
    */


    return 1;
}
int pay_stop_debug(void *param){
    printf("pay_stop_debug()..\r\n");

    #if (_VERBOSE_>=2)
        printf("dat_pay_debug\r\n");

        char buffer[10];
        DAT_PayloadBuff pay_i=dat_pay_debug;

        unsigned int indx; unsigned int max = dat_getMaxPayIndx(pay_i); int val;
        for(indx=0; indx<=max; indx++)
        {
            dat_get_PayloadBuff(pay_i, indx, &val);

            printf("dat_getPayloadVar[");
            sprintf (buffer, "%d", (unsigned int)indx);
            printf(buffer); printf("]=");
            sprintf (buffer, "%c", (unsigned int)val);
            printf(buffer); printf("\r\n");

            ClrWdt();
        }
    #endif


    return 1;
}
//******************************************************************************
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

    //configure PayloadBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gyro;
    lenBuff = (unsigned int)(500*3);  //(1440*3)      //numero de 10-minutos en un dia
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //configure Payload
    int res;
    if( gyr_isAlive()==TRUE ){
        res = 1;
        gyr_init_config();
    }
    else{
        res = 0;
    }

    //report isAlive status
    sta_setCubesatVar(sta_pay_gyro_isAlive, res);

    //debug info
    printf("  sta_pay_gyro_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_gyro_isAlive) );
    printf("  PPC_GYRO_INT2_CHECK = %d \r\n", PPC_GYRO_INT2_CHECK );

    return res;
}
int pay_isAlive_gyro(void *param){
    return gyr_isAlive();
}
int pay_get_state_gyro(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_gyro_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_take_gyro(void *param){
    printf("pay_take_gyro()..\r\n");

    //in case of failure
    if( sta_getCubesatVar(sta_pay_gyro_isAlive)==0 ){
        unsigned int lenBuff;
        lenBuff = (unsigned int)(3);
        dat_reset_PayloadBuff(dat_pay_gyro, lenBuff, 0);
        dat_set_PayloadBuff(dat_pay_gyro ,0xFAFA);
        dat_set_PayloadBuff(dat_pay_gyro, 0xFAFA);
        dat_set_PayloadBuff(dat_pay_gyro, 0xFAFA);
        
        return 1;
    }

    GYR_DATA res_data;
    gyr_take_samples(FALSE, &res_data);
    dat_set_PayloadBuff(dat_pay_gyro, res_data.a_x);
    dat_set_PayloadBuff(dat_pay_gyro, res_data.a_y);
    dat_set_PayloadBuff(dat_pay_gyro, res_data.a_z);
    
    return 1;
}
int pay_stop_gyro(void *param){
    printf("pay_stop_gyro()..\r\n");
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
int pay_init_tmEstado(void *param){
    printf("pay_init_tmEstado\r\n");

    //configure PaylaodBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_tmEstado;
    lenBuff = (unsigned int)(40*sta_cubesatVar_last_one);  //(4*60/5=48)      //numero de 5-minutos en una orbita (4 horas)
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //configure Paylaod
    //nothing to do..
    int res = 1;

    //report isAlive status
    sta_setCubesatVar(sta_pay_tmEstado_isAlive, res);

    //debug info
    printf("  sta_pay_tmEstado_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_tmEstado_isAlive) );

    return res;
}
int pay_take_tmEstado(void *param){
    printf("pay_take_tmEstado()..\r\n");

    STA_CubesatVar indxVar; int var;
    for(indxVar=0; indxVar<sta_cubesatVar_last_one; indxVar++){
        var = sta_getCubesatVar(indxVar);
        dat_set_PayloadBuff(dat_pay_tmEstado, var);
        #if (_VERBOSE_>=2)
            printf("dat_CubesatVar[%d] = %d\r\n", indxVar, var);
        #endif
    }

    return 1;
}
int pay_stop_tmEstado(void *param){
    printf("pay_stop_tmEstado()..\r\n");
    return 1;
}
//******************************************************************************
int pay_debug_camera(void *param){
    int issync;

    //encender camera
    PPC_CAM_SWITCH=1;
    printf("PPC_CAM_SWITCH = %d\n", PPC_CAM_SWITCH_CHECK);

    // Wait for the camera to be ready
    cam_wait_hold_wtimeout(TRUE);

    //initialize camera
    issync = cam_sync(TRUE);
    printf("SYNC() arrojo: issync = 0x%X\n", issync);

    if(issync!=0x0000){
        printf("issync!=0x0000()\r\n");
        printf("Camera is broken (once again)\r\n");
    }

    //unsigned int larg= cam_photo(0x01, 0x00, 0x05);
    unsigned int larg= cam_photo(0x07, 0x00, 0x05);
    #if (_VERBOSE_>=1)
        char ret[6];
        printf("cam_photo() retorno ");
        sprintf (ret, "0x%X", (unsigned int)larg);
        printf(ret); printf("\r\n");
    #endif

    //int mode=CAM_MODE_BOTH;
    //int mode=CAM_MODE_SAVE_SD;
    int mode=CAM_MODE_VERBOSE;
    cam_receivePhoto(larg, mode);

    //apagar camera
    PPC_CAM_SWITCH=0;
    printf("PPC_CAM_SWITCH = %d\n", PPC_CAM_SWITCH_CHECK);

    return larg;
}
int pay_isAlive_camera(void *param){
    return cam_isAlive();
}
int pay_get_state_camera(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_camera_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_init_camera(void *param){
    printf("pay_init_camera\r\n");

    //configure PayloadBuff
    //nothing to do..

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

    //report isAlive status
    sta_setCubesatVar(sta_pay_camera_isAlive, res);

    //debug info
    printf("  PPC_CAM_HOLD_CHECK = %d\r\n", PPC_CAM_HOLD_CHECK);
    printf("  sta_pay_camera_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_camera_isAlive) );

    return res;
}
int pay_take_camera(void *param){
    printf("pay_take_camera()\r\n");

    //in case of failure
    if( sta_getCubesatVar(sta_pay_camera_isAlive)==0 ){
        printf("camera is not alive!..\r\n");

        unsigned int lenBuff;
        lenBuff = (unsigned int)(3);
        dat_reset_PayloadBuff(dat_pay_camera, lenBuff, 0);
        dat_set_PayloadBuff(dat_pay_camera ,0xFAFA);
        dat_set_PayloadBuff(dat_pay_camera, 0xFAFA);
        dat_set_PayloadBuff(dat_pay_camera, 0xFAFA);

        return 1;
    }

    int mode = *( (int*)param );
    //BOOL st = pay_cam_takeAndSave_photo(0x07, 0x00, 0x05, mode);
    //BOOL st = pay_cam_takeAndSave_photo(0x02, 0x00, 0x05, mode);
    BOOL st = pay_cam_takeAndSave_photo(0x02, 0x00, 0x05, mode);

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
BOOL pay_cam_takeAndSave_photo(int resolution, int qual, int pic_type, int mode){
    printf("pay_takeAndSave_photo()..\r\n");

    printf(" taking photo..\r\n");
    unsigned int length= cam_photo(resolution, qual, pic_type);
    #if (_VERBOSE_>=1)
        printf("    photo length = %u\r\n", length);
    #endif

    if(length==0){
        unsigned int lenBuff;
        lenBuff = (unsigned int)(3);
        dat_reset_PayloadBuff(dat_pay_camera, lenBuff, 0);
        dat_set_PayloadBuff(dat_pay_camera ,0xFAFA);
        dat_set_PayloadBuff(dat_pay_camera, 0xFAFA);
        dat_set_PayloadBuff(dat_pay_camera, 0xFAFA);
        return FALSE;
    }

    printf("  saving photo..\r\n");
    static int cnt; BOOL stat;
    int ml=length/2;    //se guardan 2byten en 1int

    //Inicializa la estructura de data payload
    dat_reset_PayloadBuff(dat_pay_camera, ml, 0);

    unsigned char respuesta; char ret[10];
    unsigned int resp;

    int i; cnt=0;
    for(i=0;i<ml;i++)
    {
        resp=0;
        SPI_nSS_1=0;
        respuesta=SPI_1_transfer(0x00);
        SPI_nSS_1=1;
        resp=(unsigned int)respuesta;

        SPI_nSS_1=0;
        respuesta=SPI_1_transfer(0x00);
        SPI_nSS_1=1;
        resp=( (resp<<8)|((unsigned int)respuesta) );


        if( (mode==CAM_MODE_VERBOSE) || (mode==CAM_MODE_BOTH) ){

            //imprime 0's que se comen sprintf y utoa ( a menos que este 1000% seguro, no toque esta parte !! )
            if( (resp&0xF000)==0x0000 ){ con_printf("0"); }
            if( (resp&0xFF00)==0x0000 ){ con_printf("0"); }
            if( (resp&0xFFF0)==0x0000 ){ con_printf("0"); }
            sprintf (ret, "%X", ((unsigned int)resp) );
            con_printf(ret);
        }
        if( (mode==CAM_MODE_SAVE_SD) || (mode==CAM_MODE_BOTH) ){
            stat = dat_set_PayloadBuff( dat_pay_camera, (int)resp );
            //__delay_ms(12);
            #if (SCH_CAMERA_VERBOSE>=1)
                cnt++;
                if(cnt%200==0){
                    con_printf("still writing memSD .. cnt=");
                    sprintf (ret, "%X", ((unsigned int)cnt) );
                    con_printf(ret); con_printf("\r\n");
                }
            #endif
        }

        ClrWdt();
    }

    //Si entro a este if es q tengo un error
    if( (mode==CAM_MODE_SAVE_SD) || (mode==CAM_MODE_BOTH) ){
        while(stat==TRUE){
            stat = dat_set_PayloadBuff( dat_pay_camera, (int)0x0000 );
            printf("rellenando\r\n");
            //__delay_ms(12);
        }
    }

    return TRUE;
}
int pay_takePhoto_camera(void *param){
    printf("pay_takePhoto_camera ..");

    //2) tomo foto
    pay_init_camera(NULL);
    //int arg = CAM_MODE_VERBOSE;
    //int arg = CAM_MODE_BOTH;
    int arg = CAM_MODE_SAVE_SD;
    int st = pay_take_camera(&arg);
    pay_stop_camera(NULL);
   //parar ciclo de Payload
    sta_setCubesatVar(sta_pay_camera_state, SRP_PAY_XXX_PERFORM_INACTIVE );
    return st;
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
int pay_init_gps(void *param){
    printf("pay_init_gps\r\n");

    //configure PayloadBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_gps;
    lenBuff = (unsigned int)(200);   //(1440)  //numero de 10-minutos en un dia
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //configure Payload
    int res;
    res = 0;    //not implemented yet => always dead ..

    //report isAlive status
    sta_setCubesatVar(sta_pay_gps_isAlive, res);

    //check SW and CHECK pins (not definitive)
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    PPC_GPS_SWITCH = 1;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_GPS_SWITCH = %d \r\n", PPC_GPS_SWITCH_CHECK );
    printf("  sta_pay_gps_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_gps_isAlive) );

    return 1;
}
int pay_take_gps(void *param){
    printf("pay_take_gps\r\n");

    //in case of failure
    if( sta_getCubesatVar(sta_pay_gps_isAlive)==0 ){
        printf("camera is not alive!..\r\n");

        unsigned int lenBuff;
        lenBuff = (unsigned int)(3);
        dat_reset_PayloadBuff(dat_pay_gps, lenBuff, 0);
        dat_set_PayloadBuff(dat_pay_gps ,0xFAFA);
        dat_set_PayloadBuff(dat_pay_gps, 0xFAFA);
        dat_set_PayloadBuff(dat_pay_gps, 0xFAFA);

        return 0;
    }

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
BOOL pay_deploy_langmuirProbe(void){
    printf("******************************\r\n");
    printf("Deployng LangmuirProbe\r\n");
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );
    PPC_LANGMUIR_DEP_SWITCH = 1;
    __delay_ms(50); //wait while port write takes effect
    printf("  PPC_LANGMUIR_DEP_SWITCH = %d \r\n", PPC_LANGMUIR_DEP_SWITCH_CHECK );

    __delay_ms(30000); //wait 30sec to burn nylon
    __delay_ms(30000); //wait 30sec to burn nylon
    ClrWdt();
    __delay_ms(30000); //wait 30sec to burn nylon

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
int pay_init_lagmuirProbe(void *param){
    printf("pay_init_lagmuirProbe\r\n");

    //configure PayloadBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_lagmuirProbe;
    lenBuff = (unsigned int)(20*1); /*TODO: SET VALUE*/ //(1440)      //numero de 10-minutos en un dia
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //configure Payload
    if( sta_getCubesatVar(sta_pay_lagmuirProbe_isDeployed)==0 ){
        BOOL st = pay_deploy_langmuirProbe();
        if( st==1 ){    //deploy succedded
            sta_setCubesatVar(sta_pay_lagmuirProbe_isDeployed, 1);
        }
    }
    int res = 0; //not fully implemented yet => always dead
    lag_erase_buffer();

    //report isAlive status
    sta_setCubesatVar(sta_pay_lagmuirProbe_isAlive, res);

    //debug info
    printf("  sta_pay_lagmuirProbe_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_lagmuirProbe_isAlive) );
    printf("  sta_pay_lagmuirProbe_isDeployed = %d \r\n", sta_getCubesatVar(sta_pay_lagmuirProbe_isDeployed) );

    return res;
}

int pay_take_lagmuirProbe(void *param){
    printf("pay_take_lagmuirProbe\r\n");

    dat_set_PayloadBuff(dat_pay_lagmuirProbe, 0x01);
    dat_set_PayloadBuff(dat_pay_lagmuirProbe, 0x02);

//    #if (SCH_PAY_LANGMUIR_ONBOARD==0)
//        dat_set_PayloadBuff(dat_pay_lagmuirProbe, 0x01);
//        dat_set_PayloadBuff(dat_pay_lagmuirProbe, 0x02);
//    #else
//
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
//            dat_set_PayloadBuff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
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
//            dat_set_PayloadBuff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
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
//            dat_set_PayloadBuff(dat_pay_lagmuirProbe, (int)lag_get_langmuir_buffer_i(i) );
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
    printf("pay_stop_lagmuirProbe()..\r\n");
    
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

    return 1;
}
int pay_isAlive_sensTemp(void *param){
    if( sensTemp_isAlive(ST1_ADDRESS) == FALSE ){return 0;}
    if( sensTemp_isAlive(ST2_ADDRESS) == FALSE ){return 0;}
    if( sensTemp_isAlive(ST3_ADDRESS) == FALSE ){return 0;}
    if( sensTemp_isAlive(ST4_ADDRESS) == FALSE ){return 0;}
    return 1;
}
int pay_get_state_sensTemp(void *param){
    MemEEPROM_Vars mem_eeprom_var = mem_pay_sensTemp_state;
    int res = readIntEEPROM1(mem_eeprom_var);
    return res;
}
int pay_init_sensTemp(void *param){
    printf("pay_init_sensTemp\r\n");

    //configure PayloadBuff
    DAT_PayloadBuff pay_i; unsigned int lenBuff;
    pay_i = dat_pay_sensTemp;
    lenBuff = (unsigned int)(500*4) + 4;    //numero de muestras*4 + estado de isAlive cada sensor
    dat_reset_PayloadBuff(pay_i, lenBuff, 1);

    //configure Payload
    BOOL s1, s2, s3, s4;
    s1 = sensTemp_init(ST1_ADDRESS);
    dat_set_PayloadBuff(dat_pay_sensTemp, (int)s1);

    s2 = sensTemp_init(ST2_ADDRESS);
    dat_set_PayloadBuff(dat_pay_sensTemp, (int)s2);

    s3 = sensTemp_init(ST3_ADDRESS);
    dat_set_PayloadBuff(dat_pay_sensTemp, (int)s3);

    s4 = sensTemp_init(ST4_ADDRESS);
    dat_set_PayloadBuff(dat_pay_sensTemp, (int)s4);

    //report isAlive status  (solo si todos fallan se reporta como perdido el payload)
    int res;
    if( s1==FALSE && s2==FALSE && s3==FALSE && s4==FALSE ){ res = 0; }
    else{ res = 1; }
    sta_setCubesatVar(sta_pay_sensTemp_isAlive, res);

    //debug info
    printf("  sta_pay_sensTemp_isAlive = %d \r\n", sta_getCubesatVar(sta_pay_sensTemp_isAlive) );

    return res;
}
int pay_take_sensTemp(void *param){
    printf("pay_take_sensTemp\r\n");

    //in case of failure
    if( sta_getCubesatVar(sta_pay_sensTemp_isAlive)==0 ){
        printf("sensTemp is not alive!..\r\n");

        unsigned int lenBuff;
        lenBuff = (unsigned int)(3);
        dat_reset_PayloadBuff(dat_pay_sensTemp, lenBuff, 0);
        dat_set_PayloadBuff(dat_pay_sensTemp ,0xFAFA);
        dat_set_PayloadBuff(dat_pay_sensTemp, 0xFAFA);
        dat_set_PayloadBuff(dat_pay_sensTemp, 0xFAFA);

        return 1;
    }

    int val;
    val=sensTemp_take(ST1_ADDRESS, FALSE);
    dat_set_PayloadBuff(dat_pay_sensTemp, val);
    val=sensTemp_take(ST2_ADDRESS, FALSE);
    dat_set_PayloadBuff(dat_pay_sensTemp, val);
    val=sensTemp_take(ST3_ADDRESS, FALSE);
    dat_set_PayloadBuff(dat_pay_sensTemp, val);
    val=sensTemp_take(ST4_ADDRESS, FALSE);
    dat_set_PayloadBuff(dat_pay_sensTemp, val);

    return 1;
}
int pay_stop_sensTemp(void *param){
    printf("pay_stop_sensTemp\r\n");
    return 1;
}
//******************************************************************************
PAY_State pay_nextStateLogic(PAY_State pay_sem, DAT_PayloadBuff pay_i){
    STA_CubesatVar dat_pay_xxx_perform;

    dat_pay_xxx_perform = sta_pay_i_to_performVar(pay_i);

    //logica del cambio de estados
    switch(pay_sem){
        case pay_state_pre_init:
            if( sta_getCubesatVar(dat_pay_xxx_perform)==SRP_PAY_XXX_PERFORM_ACTIVE ){
                pay_sem = pay_state_init;
            }
            else{
                pay_sem = pay_state_pre_init;
            }
        break;
        case pay_state_init:
            pay_sem = pay_state_take;
        break;
        case pay_state_take:
            if(dat_isFull_PayloadBuff(pay_i)==TRUE){
                pay_sem = pay_state_stop;
            }
            else{
                pay_sem = pay_state_take;
            }
        break;
        case pay_state_stop:
            pay_sem = pay_state_pre_init;
        break;
        default:
            printf("pay_nextStateLogic: en default !!!\r\n");
        break;
    }
    return pay_sem;
}
void pay_currentStateLogic(PAY_State pay_sem_state, DAT_PayloadBuff pay_i){
    int arg=0; //DAT_Payload arg_pay_i; /* WARNING, Revisar */

    //logica del estado
    switch(pay_sem_state){
        case pay_state_pre_init:
            //No se hacen cosas aca (independientemente del pay_i).
            //Esa es la idea de este estado
            #if (_VERBOSE_>=1)
                printf("pay_state_pre_init..\r\n");
            #endif
        break;
        case pay_state_init:
            //Init de Payloads
            switch(pay_i){
                case dat_pay_sensTemp:      
                    pay_init_sensTemp( (void*)&arg );
                break;
                case dat_pay_lagmuirProbe:
                    pay_init_lagmuirProbe( (void*)&arg );
                break;
                case dat_pay_gps:
                    pay_init_gps( (void*)&arg );
                break;
                case dat_pay_expFis:
                    pay_init_expFis( (void*)&arg );
                break;
                case dat_pay_camera:
                    pay_init_camera( (void*)&arg );
                break;
                case dat_pay_gyro:
                    pay_init_gyro( (void*)&arg );
                break;
                case dat_pay_tmEstado:
                    pay_init_tmEstado( (void*)&arg );
                break;
                case dat_pay_battery:
                    pay_init_battery( (void*)&arg );
                break;
                case dat_pay_debug:
                    pay_init_debug( (void*)&arg );
                break;
                default:
                break;
            }
        break;
        case pay_state_take:
            //ejecuccion de de Payloads
            switch(pay_i){
                case dat_pay_sensTemp:
                    pay_take_sensTemp( (void*)&arg );
                break;
                case dat_pay_lagmuirProbe:
                    pay_take_lagmuirProbe( (void*)&arg );
                break;
                case dat_pay_gps:
                    pay_take_gps( (void*)&arg );
                break;
                case dat_pay_expFis:
                    pay_take_expFis( (void*)&arg );
                break;
                case dat_pay_camera:
                    pay_take_camera( (void*)&arg );
                break;
                case dat_pay_gyro:
                    pay_take_gyro( (void*)&arg );
                break;
                case dat_pay_tmEstado:
                    pay_take_tmEstado( (void*)&arg );
                break;
                case dat_pay_battery:
                    pay_take_battery( (void*)&arg );
                break;
                case dat_pay_debug:
                    pay_take_debug( (void*)&arg );
                break;
                default:
                break;
            }
        break;
        case pay_state_stop:
            //termino de Payloads
            switch(pay_i){
                case dat_pay_sensTemp:
                    pay_stop_sensTemp( (void*)&arg );
                break;
                case dat_pay_lagmuirProbe:
                    pay_stop_lagmuirProbe( (void*)&arg );
                break;
                case dat_pay_gps:
                    pay_stop_gps( (void*)&arg );
                break;
                case dat_pay_expFis:
                    pay_stop_expFis( (void*)&arg );
                break;
                case dat_pay_camera:
                    pay_stop_camera( (void*)&arg );
                break;
                case dat_pay_gyro:
                    pay_stop_gyro( (void*)&arg );
                break;
                case dat_pay_tmEstado:
                    pay_stop_tmEstado( (void*)&arg );
                break;
                case dat_pay_battery:
                    pay_stop_battery( (void*)&arg );
                break;
                case dat_pay_debug:
                    pay_stop_debug( (void*)&arg );
                break;
                default:
                break;
            }
            //parar ciclo de Payload
            STA_CubesatVar dat_perfVar=sta_pay_i_to_performVar(pay_i);
            sta_setCubesatVar(dat_perfVar, SRP_PAY_XXX_PERFORM_INACTIVE );

            //debug
            #if (_VERBOSE_>=1)
                printf("pay_= %d ha pasado a estado: pay_state_stop\r\n", (int)(pay_i) );
            #endif
            #if (_VERBOSE_>=2)
                arg_pay_i=pay_i;
                drp_print_dat_PayloadVar( (void*)&arg_pay_i );
            #endif
        break;
        default:
            printf("pay_currentStateLogic: en default !!!\r\n");
        break;
    }
}

