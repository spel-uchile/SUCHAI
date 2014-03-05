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

#include "fis_payload.h"


#define _FISICA_VERBOSE_ISR_T5     1

static unsigned int sens_buff[FIS_SAMP_PER_ROUND];
static int sens_buff_ind;

void fis_testDAC(void){
    int j;
    for(j=0x0FFF;j>0;j--){

        unsigned char c= rand();
        unsigned int arg=c;
        arg=(arg<<8);
        fis_payload_writeDAC(arg);
    }
}


unsigned int fis_frec_i_to_ADC_period(DAT_GnrlPurpBuff pay_frec_i){
    unsigned int ADC_period;
    switch(pay_frec_i){
        case dat_gpb_expFis_f0:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 20;
        break;
        case dat_gpb_expFis_f1:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 21;
        break;
        case dat_gpb_expFis_f2:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 22;
        break;
        case dat_gpb_expFis_f3:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 23;
        break;
        case dat_gpb_expFis_f4:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 24;
        break;
        case dat_gpb_expFis_f5:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 25;
        break;
        case dat_gpb_expFis_f6:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 26;
        break;
        case dat_gpb_expFis_f7:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 27;
        break;
        case dat_gpb_expFis_f8:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 28;
        break;
        case dat_gpb_expFis_f9:
            //              7654321076543210
            ADC_period = (0b0000000010110100);
            ADC_period = 29;
        break;
        default:
            ADC_period=0;
        break;
    }

    return ADC_period;
}
void fis_save_sens_buff_to_GPB(DAT_GnrlPurpBuff frec_i, int rst_gbp_indx){
    int i,j; unsigned int ind; unsigned long prom=0;

    //esperar mientras se termina de llenar el sens_buff (buffer intermedio)
    while( sens_buff_ind<(FIS_SAMP_PER_ROUND) ){
        for(i=0;i<0x00FF;i++){j=i*1;}
        i=j+2;
    }
    
    //calculo promedio sens1
    for(ind=0;ind<FIS_SAMP_PER_ROUND;ind=ind+2){
        prom=prom+sens_buff[ind];
    }
    prom=prom/FIS_SAMP_PER_ROUND;
    //resto el promedio sens1
    for(ind=0;ind<FIS_SAMP_PER_ROUND;ind=ind+2){
        sens_buff[ind]=sens_buff[ind]-prom;
    }
    
    //calculo promedio sens2
    for(ind=1;ind<FIS_SAMP_PER_ROUND;ind=ind+2){
        prom=prom+sens_buff[ind];
    }
    prom=prom/FIS_SAMP_PER_ROUND;
    //resto el promedio sens2
    for(ind=1;ind<FIS_SAMP_PER_ROUND;ind=ind+2){
        sens_buff[ind]=sens_buff[ind]-prom;
    }
    
    //guardo en memSD
    static unsigned int gpb_indx;
    if( rst_gbp_indx==1 ){
        gpb_indx=0;
    }
    for(ind=0;ind<FIS_SAMP_PER_ROUND;ind++){
        //Set DAT_GnrlPurpBuff
        dat_setGPB(frec_i, gpb_indx, sens_buff[ind] );
        gpb_indx++;
    }
}
void fis_print_sens_buff(void){
    int ind; char ret[10];
    for(ind=0;ind<FIS_SAMP_PER_ROUND;ind++){
        //utoa(ret,  (unsigned int)ind, 16);
        sprintf (ret, "0x%X", (unsigned int)ind);
        con_printf("sens_buff["); con_printf(ret); con_printf("]=");
        //utoa(ret,  (unsigned int)sens_buff[ind], 16);
        sprintf (ret, "0x%X", (unsigned int)sens_buff[ind]);
        con_printf(ret); con_printf("\r\n");
    }
}
void fis_payload_writeDAC(unsigned int arg){
    unsigned char r,msb,lsb;
    msb=(unsigned char)(arg>>8);
    lsb=(unsigned char)arg;
    //transmit
    SPI_nSS_3 = 0;
    r = SPI_3_transfer(0x00);
    r = SPI_3_transfer(msb);
    r = SPI_3_transfer(lsb);
    SPI_nSS_3 = 1;
}
void fis_stop_expFis(void){
    //ISR
    DisableIntT5;
    DisableIntT4;
    DisableIntADC1;

    #if (SCH_FISICA_VERBOSE>=2)
        con_printf("stoping expFis ISRs\r\n");
    #endif
    //Modules
    CloseTimer4();
    CloseTimer5();
    CloseADC10();
}
void fis_start_expFis(unsigned int period){
    char ret[10];
    #if (SCH_FISICA_VERBOSE>=1)
        itoa(ret,  (unsigned int)period, 10);
        con_printf("  period="); con_printf(ret); con_printf("\r\n");
    #endif  
    #if (SCH_FISICA_VERBOSE>=2)
        con_printf("  sens_buff_ind=0..\r\n");
        con_printf("  fis_ADC_config()..\r\n");
        con_printf("  fis_Timer4_config(period*3)..\r\n");
        con_printf("  fis_Timer5_config(period)..\r\n");
    #endif
    sens_buff_ind=0;
    fis_ADC_config();
    fis_Timer4_config(period*3);
    fis_Timer5_config(period);
}

void fis_ADC_config(void){
    unsigned int config1; unsigned int config2; unsigned int config3;
    unsigned int configportL,configportH, configscanL, configscanH;
    config1 = ADC_MODULE_OFF & ADC_IDLE_CONTINUE & ADC_FORMAT_INTG & ADC_CLK_MANUAL & ADC_AUTO_SAMPLING_ON;
    config2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_INTR_2_CONV & ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF;
    config3 = ADC_SAMPLE_TIME_10 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_1Tcy;

    //registros ya configurados en pic_pc104_config.c
    configportL =  ENABLE_AN11_ANA & ENABLE_AN13_ANA;
    //_PCFG11=0; 		//pin 0-ADC 1- I/O
    //_PCFG13=0; 		//pin 0-ADC 1- I/O
    configportH = 0xFFFF;       //1=digital 0=analog
    //              7654321076543210
    configscanL = 0b0010100000000000;//ADC_SCAN_AN11 & ADC_SCAN_AN13; //1=scan 0=skip
    configscanH = 0x0000;
    //_CSSL11=1;
    //_CSSL13=1;

    AD1CHS0= ADC_CH0_NEG_SAMPLEA_VREFN & ADC_CH0_POS_SAMPLEA_AN11;

    OpenADC10_v2(config1,config2,config3,configportL,configportH,configscanL,configscanH);
    EnableADC1;
    //AD1CON1bits.SAMP = 1;

    ConfigIntADC10(ADC_INT_DISABLE & ADC_INT_PRI_2 );
    //IFS0bits.AD1IF = 0;
}

void fis_Timer4_config(unsigned int period){
    //                      7654321076543210
    unsigned int config = 0b1000000000110000; //T4_ON & T4_GATE_ON & T4_IDLE_CON & T4_PS_1_1 & T4_SOURCE_INT;
    //                      7654321076543210
    //unsigned int period = 0b0000000000001011;
    WriteTimer4(0x0000);
    OpenTimer4( config, period );

    ConfigIntTimer4(T4_INT_ON & T4_INT_PRIOR_1);
}
void fis_Timer5_config(unsigned int period){
    //                      7654321076543210
    unsigned int config = 0b1000000000110000; //T4_ON & T4_GATE_ON & T4_IDLE_CON & T4_PS_1_1 & T4_SOURCE_INT;
    //                      7654321076543210
    //unsigned int period = 0b0000000000000111;
    WriteTimer5(0x0000);
    OpenTimer5( config, period );

    ConfigIntTimer5(T5_INT_ON & T5_INT_PRIOR_1);
}

//ISR del T4
//void _ISR _T4Interrupt(void){
void __attribute__((__interrupt__, auto_psv)) _T4Interrupt(void){
    //con_printf("en ISR T4\r\n");
    
    unsigned char c= rand();
    unsigned int arg=c;
    arg=(arg<<8);
    fis_payload_writeDAC(arg);

    //ClrWdt();
    T4_Clear_Intr_Status_Bit;
}
//ISR del T5
//void _ISR _T5Interrupt(void){
void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void){
    //con_printf("ISR T5\r\n");
  
    ConvertADC10();
    unsigned long i,j;
    for(i=0;i<0x00000001;i++){j=i*1;}
    i=j+2;
    ConvertADC10();
    for(i=0;i<0x00000001;i++){j=i*1;}
    i=j+2;
    //static int asd;
    //if( BusyADC10()==0 && asd==0){con_printf("ADC10 is ready\r\n"); asd=2;}

    int sens1,sens2;
    sens2=ReadADC10(0);
    sens1=ReadADC10(1);

    #if (_FISICA_VERBOSE_ISR_T5>=2)
        char ret[6];
        con_printf("sens[1"); con_printf(ret); con_printf("]=");
        Hex16ToAscii(  (unsigned int)sens1, ret); con_printf(ret); con_printf("\r\n");
        con_printf("sens[2"); con_printf(ret); con_printf("]=");
        Hex16ToAscii(  (unsigned int)sens2, ret); con_printf(ret); con_printf("\r\n");
    #endif

    sens_buff[sens_buff_ind]=sens1;
    sens_buff_ind++;
    sens_buff[sens_buff_ind]=sens2;
    sens_buff_ind++;
    
    #if (_FISICA_VERBOSE_ISR_T5>=2)
        Hex16ToAscii(  (unsigned int)sens_buff_ind, ret);
        con_printf("sens_buff_ind="); con_printf(ret); con_printf("\r\n");
    #endif
    
    if(sens_buff_ind>=(FIS_SAMP_PER_ROUND)){
        #if (_FISICA_VERBOSE_ISR_T5>=2)
            con_printf("ISR T5: sens_buff_ind == FIS_SENS_BUFF_LENGTH\r\n");
        #endif
        fis_stop_expFis();
        //fis_save_sens_buff();
    }
    
    T5_Clear_Intr_Status_Bit;
}
//ISR del ADC
/*
void _ISR _ADC1Interrupt(void){

    con_printf("ISR ADC1\r\n");

    int ind, sens; char ret[6];
    for(ind=0;ind<=1;ind++){
        sens=ReadADC10(ind);

        Hex16ToAscii(  (unsigned int)ind, ret);
        con_printf("sens["); con_printf(ret); con_printf("]=");
        Hex16ToAscii(  (unsigned int)sens, ret); con_printf(ret); con_printf("\r\n");
    }

    sem_expFis++;
    if(sem_expFis==0x00FF){
        
    }
    //unsigned long i,j;
    //for(i=0;i<0x006FFFFF;i++){j=i*1;}
    //i=j+2;
    
    con_printf("fin ISR ADC1\r\n");
    
    IFS0bits.AD1IF = 0;
}
*/


void hist(void){

    DAT_GnrlPurpBuff gpb_frec_i;
    unsigned int i,j; unsigned int max; 
    long v1, v2, mul, ran, prev; unsigned long v1u, v2u;

    //realiza multiplicacion para cada frecuencia
    max=(2*FIS_SAMP_PER_ROUND);//(FIS_REPEAT_PER_ROUND*FIS_SAMP_PER_ROUND);
    for(gpb_frec_i=dat_gpb_expFis_f0; gpb_frec_i<=dat_gpb_expFis_f9; gpb_frec_i++){
        for(i=0;i<max;i=i+2){
            v1 = (long)dat_getGPB(gpb_frec_i, i);
            v2 = (long)dat_getGPB(gpb_frec_i, i+1);
            mul=v1*v2;
            dat_setGPB(gpb_frec_i, i, (unsigned int)mul);
            dat_setGPB(gpb_frec_i, i+1, (unsigned int)(mul>>16) );
        }
    }

    //inicializa buffer del hist
    for(i=0;i<(512*2*2);i++){
        dat_setGPB(dat_gpb_expFis_hist, i, 0xFFFF);
    }

    //realiza histograma para cada frecuencia
    max=(2*FIS_SAMP_PER_ROUND);//(FIS_REPEAT_PER_ROUND*FIS_SAMP_PER_ROUND);
    for(gpb_frec_i=dat_gpb_expFis_f0; gpb_frec_i<=dat_gpb_expFis_f9; gpb_frec_i++){
        
        for(i=0;i<max;i=i+2){
            v1u = (unsigned long)dat_getGPB(gpb_frec_i, i);
            v2u = (unsigned long)dat_getGPB(gpb_frec_i, i+1);
            mul = (long)( (v2u<<16)|v1u );

            //esto da aca ya no sirve :/
            for(j=0;TRUE;j++){

                if(mul < 0 ){
                    ran=mul/2;
                    if( ran==0 || ran==(-1) ){
                        //rango j-esimo negativo
                        dat_setGPB(dat_gpb_expFis_hist, j, j);
                        //tiene una frecuencia de
                        prev = dat_getGPB(dat_gpb_expFis_hist, (10000)+j);
                        prev++;
                        dat_setGPB(dat_gpb_expFis_hist, (10000)+j, prev);
                        break;
                    }
                }
                else{
                    ran=mul/2;
                    if( ran==0 || ran==1 ){
                        //rango j-esimo positivo
                        dat_setGPB(dat_gpb_expFis_hist, (10000*2)+j, j);
                        //tiene una frecuencia de
                        prev = dat_getGPB(dat_gpb_expFis_hist, (10000*3)+j);
                        prev++;
                        dat_setGPB(dat_gpb_expFis_hist, (10000*3)+j, prev);
                        break;
                    }
                }
                
            }
        }
    }

    /*
    int b_dat[50.000];
    int b_cntn[512];
    int b_cntp[512];
    int i,j, ran;
    for(i=0;i<50.000;i++){
        
        for(j=0;TRUE;j++){

            if(b_dat[i] < 0 ){
                ran=b_dat[i]/2;
                if( ran==0 || ran==(-1) ){
                    b_cntn[j]=b_cntn[j]+1;
                    break;
                }
            }
            else{
                ran=b_dat[i]/2;
                if( ran==0 || ran==1 ){
                    b_cntp[j]=b_cntp[j]+1;
                    break;
                }
            }
        }
    }
    */
}

