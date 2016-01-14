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
#include "interfaz_ADC.h"

#define _FISICA_VERBOSE_ITERATE      (0)
#define _FISICA_VERBOSE_TIMER4_ISR   (0)
#define _FISICA_VERBOSE_TIMER5_ISR   (0)
#define _FISICA_VERBOSE_TIMER4_CFG   (0)
#define _FISICA_VERBOSE_TIMER5_CFG   (0)
#define _FISICA_VERBOSE_ADC_CFG      (0)
#define _FISICA_VERBOSE_DAC_SPI      (0)

/*
 * Global parameters being used in the execution of this payload
 */
static unsigned int fis_state;    //working state
static unsigned int fis_signal_period_ind;    //Index of the  "fis_ADC_period" being executed 
//static const unsigned int* fis_signal_period; //Array with the values of "ADC_period"
//static unsigned int fis_signal_period[FIS_MAX_FREQS];
static unsigned int fis_signal_period;
static int fis_signal_period_len; //number of elements in "fis_ADC_period"
static int fis_rounds;   //number of repetitions of the payload for each "ADC_period" value
static unsigned int fis_current_round;   //index of the current waveform being executed
static unsigned int fis_point;  //counter for the total waveform points
static unsigned int fis_sample;  //total number of samples to be done
static unsigned int seed[FIS_SRAND_SEEDS];  //seeds array with the arguments for srand() calls
static unsigned int sens_buff[FIS_SENS_BUFF_LEN];   //temporary buffer where the measures are stored
static int sens_buff_ind;   //index used with sens_buff
static BOOL sync;

unsigned int fis_get_total_number_of_samples(void){
    return FIS_SIGNAL_POINTS*fis_rounds*FIS_SAMPLES_PER_POINT;
}

/* 
 * Return the size of the sens_buff
 */
unsigned int fis_get_sens_buff_size(void){
    return FIS_SENS_BUFF_LEN;
}

/* 
 * Return the state of the execution of the Payload (executin/wating/etc)
 */
unsigned int fis_get_state(void){
    return fis_state;
}

/* 
 * Initialize the buffer with the seeds values used with rand() in the DAC
 */
void fis_seed_init(void){
    unsigned int i;
    for (i=0; i < FIS_SRAND_SEEDS; i++){
        seed[i] = i+1000;        
    }
    srand(seed[0]);
    
}

/* 
 * This function waits until the current payload iteration is completed.
 * If "sens_buff" is not full takes too long to be filled, then this function
 * triggers a timeout and return a error return-code.
 */
int fis_wait_busy_wtimeout(unsigned int timeout){
    int seg_timeout = timeout;   

    while(!(fis_sens_buff_isFull())){
        __delay_ms(1000)
        seg_timeout--;
        if(seg_timeout<=0){ 
        #if _FISICA_VERBOSE_ITERATE > 0
            printf("fis_wait_busy_wtimeout: expFis timeout!\n");
        #endif
            return 0;   //timeout!
        }
    }
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_wait_busy_wtimeout: ok\n");
    #endif
    return 1;
}

/*
 * Prints all the elements inside the temporary buffer "sens_buff". Use for debuggoing
 * 
 */
void fis_print_sens_buff(void){
    int ind;
    for(ind=0; ind<FIS_SENS_BUFF_LEN; ind++){
        if(ind%2==0){
            printf("sens_buff[%02d]=%04d, ", ind, sens_buff[ind]);
        }
        else{
            printf("sens_buff[%02d]=%04d\n", ind, sens_buff[ind]);
        }
    }
}

/*
 * Erase all the values stored inside "sen_buff"
 */
void fis_sens_buff_init(void){
    printf("sens_buff reset \n");
    int ind;
    for(ind = 0; ind < FIS_SENS_BUFF_LEN ; ind++){
        sens_buff[ind] = 0;
    }
    sens_buff_ind = 0;
}

/*
 * Returns the value of sens_buff[ind]
 * @param ind Index of the element
 * @return value of sens_buff[ind] if it exist, else returns 0
 */
unsigned int fis_get_sens_buff_i(int ind){
    if(ind>=FIS_SENS_BUFF_LEN){return 0;}
    return sens_buff[ind];
}

/*
 * Asks if the temporary buffer "sens_buff" is full
 * @return
 *          TRUE if sens_buff is full
 *          FALSE if not
 */
BOOL fis_sens_buff_isFull(void){
//esperar mientras se termina de llenar el sens_buff (buffer intermedio)
    if( sens_buff_ind < FIS_SENS_BUFF_LEN ){
        return FALSE;
    }
    return TRUE;
}

/**
 * Return TRUE if the last round of the las ADC_period was completed, if so, the
 * experiment is complete
 * @return TRUE if the last round of the las ADC_period was completed, FALSE otherwise
 */
BOOL fis_iterate_isComplete(void){

    if(fis_state == FIS_STATE_DONE) { return TRUE; }
    else{ return FALSE; }
}

/**
 * Reset the iterate_config settings, a new call to "fis_iterate_config"
 * is nedded to use fis_iterate again. Not doing so will certainly end in
 * SEGMENTATOIN FAULTS !! So, don't call its a internal function
 */
static void fis_config_reset(void){
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_config_ reset...\n");
    #endif

    fis_signal_period = 0;
    fis_signal_period_len = fis_rounds = 0;
    fis_current_round = 0;
    fis_signal_period_ind = 0;
    fis_point = 0;
    fis_sample = 0;
    sync = FALSE;
    fis_state = FIS_STATE_OFF;  //ready for init the execution
    fis_sens_buff_init();  //reset the buffer and clears it
    fis_seed_init();  //reset the seeds used for rand()
}

unsigned int fis_iterate_config(unsigned int inputSignalPeriod, int rounds){
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_iterate_config..\n");
    #endif 
    fis_config_reset();
    
    int len = 1;
    fis_signal_period = inputSignalPeriod;
    fis_signal_period_len = len;   
    fis_rounds = rounds;

    fis_signal_period = inputSignalPeriod;
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("    fis_signal_period = %u\n",fis_signal_period);
    #endif

    fis_signal_period_ind = 0;
    fis_state = FIS_STATE_READY;
    return fis_state;
}

/*
 * Does ONE ITERATION of the experiment. This means the execution of one WAVEFORMwith 
 * the CURRENT value of "fis_ADC_period[fis_ADC_period_i]". 
 * Sequential calls to this function uses the same value of "fis_ADC_period[fis_ADC_period_i]" until
 * the number of "fis_round" for that specific "fis_ADC_period[fis_ADC_period_i]" were executed.
 * When that occurs, the next call to this function will execute with "fis_ADC_period[fis_ADC_period_i+1]"
 *  This function only returns when the buffer is full or when an error occurred (see rc)
 * @rc  return code
 * @return 
 *          TRUE if the iteration was successfull
 *          FALSE if not
 */void fis_iterate(unsigned int *rc, unsigned int timeout_seg){
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_iterate...\n");
    #endif
        
    int normal_wait;

    if(fis_state == FIS_STATE_DONE){
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("    expFis completed\n");
    #endif
        fis_state = FIS_STATE_OFF;  //shuts down the expFis
        *rc = 1;
        return;
    }
    else if(fis_state == FIS_STATE_OFF){ //error
        #if _FISICA_VERBOSE_ITERATE > 0
            printf("    expFis must be configured before calling fis_iterate\n");
        #endif
        *rc = 1;
        return;
    }
    else if (fis_state == FIS_STATE_READY){  //first time of execution
        #if _FISICA_VERBOSE_ITERATE > 0
            printf("    Configuring and starting expFis...\n");
            printf("    ADC period = %u\n", fis_signal_period);
            printf("    round = %u/%u\n",fis_current_round+1, fis_rounds);
            printf("    seed[%u] = %u\n",fis_current_round, seed[fis_current_round]);
            printf("    fis_points = %u/%u\n",fis_point, FIS_SIGNAL_POINTS);
            printf("    fis_samples = %u/%u\n",fis_sample, FIS_SIGNAL_SAMPLES);
            printf("    samples per point = %u\n", FIS_SAMPLES_PER_POINT);
            printf("    total samples (ADC) = %u\n", FIS_SIGNAL_SAMPLES);
            printf("    len( sens_buff ) = %u\n", FIS_SENS_BUFF_LEN);
        #endif
        fis_run(fis_signal_period);

    }
    else if(fis_state == FIS_STATE_WAITING){    //expFis is wating to resume its execution
        #if _FISICA_VERBOSE_ITERATE > 0
            printf("    Configuring and starting expFis...\n");
            printf("    ADC period = %u\n", fis_signal_period);
            printf("    round = %u/%u\n",fis_current_round+1, fis_rounds);
            printf("    seed[%u] = %u\n",fis_current_round, seed[fis_current_round]);
            printf("    fis_points = %u/%u\n",fis_point, FIS_SIGNAL_POINTS);
            printf("    fis_samples = %u/%u\n",fis_sample, FIS_SIGNAL_SAMPLES);
            printf("    samples per point = %u\n", FIS_SAMPLES_PER_POINT);
            printf("    total samples (ADC) = %u\n", FIS_SIGNAL_SAMPLES);
            printf("    len( sens_buff ) = %u\n", FIS_SENS_BUFF_LEN);
            //printf("    T4CONbits.TON %X\n",T4CONbits.TON);
            //printf("    T4CONbits.TON %X\n",T4CONbits.TON);
            //printf("    IEC1bits.T4IE %X\n",IEC1bits.T4IE);
            //printf("    IEC1bits.T5IE %X\n",IEC1bits.T5IE);
            //printf("    IFS1bits.T4IF %X\n",IFS1bits.T4IF);
            //printf("    IFS1bits.T5IF %X\n",IFS1bits.T5IF);
        #endif
        fis_iterate_resume();
    }
    else{
        #if _FISICA_VERBOSE_ITERATE > 0
            printf("    Invalid fis_state value\n");
        #endif
        *rc = -1;
        return;
    }

    normal_wait = fis_wait_busy_wtimeout(timeout_seg);

    if( normal_wait == 1 ){   

            *rc = 0;
        if(fis_state == FIS_STATE_DONE) {
            *rc = 1;
        }
    }
    else {  //timeout!
        *rc = -1;
    }
}

Fis_States fis_next_state_logic(Fis_States curr_state){
    printf("[fis_next_state_logic] curr_state is %d\r\n", curr_state);
    switch(curr_state){
        case FIS_OFF:
            curr_state = FIS_READY;
        break;
        case FIS_READY:
            curr_state = FIS_RUNNING;
        break;
        case FIS_RUNNING:
            curr_state = FIS_WAITING;
        break;
        case FIS_WAITING:
            curr_state = FIS_DONE;
        break;
        case FIS_DONE:
            curr_state = FIS_DONE;
        break;
        default:
            printf("[fis_next_state_logic] ERROR: curr_state in default state\r\n");
        break;
    }
    printf("[fis_next_state_logic] next_state is %d\r\n", curr_state);

    return curr_state;
}

Fis_States fis_current_state_control(Fis_States curr_state){
    printf("[fis_current_state_control] curr_state is %d\r\n", curr_state);

    switch(curr_state){
        case FIS_OFF:
            // do nothing ..
        break;
        case FIS_READY:
            //
        break;
        case FIS_RUNNING:
            ///
        break;
        case FIS_WAITING:
            //
        break;
        case FIS_DONE:
            //
        break;
        default:
            printf("[fis_current_state_control] ERROR: curr_state in default state\r\n");
        break;
    }
    return curr_state;
}

/*
 * Tells the DAC to put a fixed voltage in the DAC output.
 * The voltage value its mapped from 0 to Vcc (0 to 3.3V)
 */
void fis_testDAC(unsigned int value){
    #if _FISICA_VERBOSE_DAC_SPI
        double analog_value = (value*(0.050813));
        printf("fis_testDAC ...\n");
        printf("    Sending a %x value to fis_writeDAC function\n", value);
        printf("    This value should be equivalent to a %f mV\n",analog_value);
    #endif

    unsigned int temp;
    for(temp=0;temp < value; temp++){
        fis_payload_writeDAC(temp);
    }
    #if _FISICA_VERBOSE_DAC_SPI > 0
        printf("    Ok\n");
    #endif
}
void fis_payload_print_seed(void){
    unsigned int temp;
    printf("    fis_payload_print_seed ...\n");
    fis_seed_init();
    printf("    srand is set, printing random values ...\n");
    
    for(temp = 0; temp < FIS_SIGNAL_POINTS; temp++){
        printf("    rand() = %d \n",rand());
    }
}


/*
 * Writes a Digital value in the input Port of this Payload, using the DAC
 */
void fis_payload_writeDAC(unsigned int arg){
    unsigned char r, firstByte, secondByte,thirdByte;
    //Bytes to be written in the SPI register
    unsigned int myarg = arg;
    firstByte = 0x00;
    secondByte = (unsigned char)(myarg>>8);
    thirdByte = (unsigned char) myarg;
    
    #if _FISICA_VERBOSE_DAC_SPI > 0
        printf("fis_payload_writeDAC\n");
        printf("    arg: %X \n", myarg);
        printf("SPI_3_transfer ...\n");
        printf("    first Byte sent %X\n", firstByte);
        printf("    second Byte sent %X\n", secondByte);
        printf("    third Byte sent %X\n", thirdByte);
    #endif

    SPI_nSS_3 = 0;  //SPI: Slave Select PIN active

    r = SPI_3_transfer(firstByte);
    r = SPI_3_transfer(secondByte);
    r = SPI_3_transfer(thirdByte);
    
    SPI_nSS_3 = 1;  //SPI: Slave Select PIN inactive
}

void fis_iterate_stop(void){
    
    T4CONbits.TON = 0;
    T5CONbits.TON = 0;
    IEC1bits.T4IE = 0;
    IEC1bits.T5IE = 0;

    #if (_FISICA_VERBOSE_ITERATE > 0)
        printf("expFis ISRs are down ...\r\n");
    #endif
    CloseADC10();
    
    #if (_FISICA_VERBOSE_ITERATE > 0)
        printf("expFis ADC is closed ...\r\n");
    #endif
    
    fis_state = FIS_STATE_DONE;
}

/*
 * Use only when the fis_sens_buff_isFull returns TRUE
 */
void fis_iterate_pause(void){
    fis_state = FIS_STATE_WAITING;
    T4CONbits.TON = 0;
    T5CONbits.TON = 0;
    IEC1bits.T4IE = 0;
    IEC1bits.T5IE = 0;
    
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_pause_expFis\n");
    //se han sacados todas las muestras, para todas las rondas, para cada una de las frecuencias
    //if(fis_sample == FIS_SIGNAL_SAMPLES && fis_current_round == fis_rounds && fis_signal_period_ind == (fis_signal_period_len-1)) {
        printf("fis_sample: %u\n", fis_sample);
        printf("fis_current_round: %u\n", fis_current_round);
    #endif
    if(fis_sample == FIS_SIGNAL_SAMPLES) {
        fis_sample = 0;
        if((fis_current_round) == fis_rounds) {
            fis_iterate_stop();  
        }
    }
}
/*  
 * Use only saving the data inside sens_buff into the Data Repository
 */
void fis_iterate_resume(void){
    sync = FALSE;
    sens_buff_ind = 0;
    T4CONbits.TON = 1;
    T5CONbits.TON = 1;
    IEC1bits.T4IE = 1;
    IEC1bits.T5IE = 1;
    fis_state = FIS_STATE_WORKING;
    #if _FISICA_VERBOSE_ITERATE > 0
        printf("fis_iterate_resume ok\n");
    #endif
    fis_run(fis_signal_period);

}

void fis_run(const unsigned int period){
    unsigned int period_DAC = period*(FIS_SAMPLES_PER_POINT);
    unsigned int period_ADC = period;    
    #if (_FISICA_VERBOSE_ITERATE > 0)
        printf("ADC_period (DAC_period=3*ADC_period) = %u\n", period);
        printf("period DAC= %u\n", period_DAC);            
        printf("period ADC= %u\n", period_ADC);
    #endif
    fis_ADC_config();   //configura los registros del ADC
    fis_Timer4_config(period_DAC);  //DAC
    fis_Timer5_config(period_ADC);  //ADC
    fis_state = FIS_STATE_WORKING;
    #if (_FISICA_VERBOSE_ITERATE > 0)
        printf("expFis ISRs are up..\r\n");
    #endif
    fis_Timer45_begin();
}

/* 
 * Begin the timer counter
 * 
 */
void fis_Timer45_begin(void){
    T4CONbits.TON = 1;
    T5CONbits.TON = 1;
    #if _FISICA_VERBOSE_TIMER4_CFG > 0
        printf("fis_init_timers(): T4CON %X\n",T4CON);
        printf("fis_init_timers(): IFS1bits.T4IF %X\n",IFS1bits.T4IF);
        printf("fis_init_timers(): T5CON %X\n",T5CON);
        printf("fis_init_timers(): IFS1bits.T5IF %X\n",IFS1bits.T5IF);
    #endif
}

void fis_ADC_config(void){
    #if _FISICA_VERBOSE_ADC_CFG > 0
        printf("fis_ADC_config: ADC initializing...\n");
    #endif
    /*
     * list of AD/DA registers of pic24FJ256GA110
     * AD1CON1  OpenADC10_v2    
     * AD1CON2  OpenADC10_v2
     * AD1CON3  OpenADC10_v2
     * AD1CHS0  Manually
     * AD1PCFGL OpenADC10_v2
     * AD1PCFGH OpenADC10_v2
     * AD1CSSL  OpenADC10_v2
     * AD1CSSH  OpenADC10_v2
     */
    unsigned int config1; unsigned int config2; unsigned int config3;
    unsigned int configportL,configportH, configscanL, configscanH;
    /* 
     * @config1 This contains the parameters to be configured in the ADCON1 register 
     * @config2 This contains the parameters to be configured in the ADCON2 register 
     * @config3 This contains the parameters to be configured in the ADCON3 register 
     * @configportL This contains the pin select to be configured into ADPCFGL register 
     * @configportH This contains the pin select to be configured into ADPCFGH register 
     * @configscanL This contains the scan select parameter to be configured into the AD1CSSL 
     * @configscanH This contains the scan select parameter to be configured into the AD1CSSH
     */
    /* AD1CON1
     * config1 = 0x5c1f = 0b 0101 1100 0001 1111 
     * iwant   = 0x0004 = 0b 0000 0000 0000 01xx
     * AUTO_SAMPLING means that ADC is sampling all the time, 
     * but conversion has to be set manually.
     * After a conversion occurs, a new sampling process begins automatically.
     */
    config1 = ADC_MODULE_OFF & ADC_IDLE_CONTINUE & ADC_FORMAT_INTG & ADC_CLK_MANUAL & ADC_AUTO_SAMPLING_ON;
    /* AD1CON2
     * config2 = 0x0F84 = 0b 0000 1111 1000 0000
     * i want  = 0x0400 = 0b 0000 0100 0000 0000
     */
    config2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_INTR_EACH_CONV & ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF;
    /* AD1CON3
     * AD1CON2bits.SMPI controla los flag de interrupciones en el registro AD1IF
     * El flag de interrupcion se setea despu?s de la cantidad de conversiones correspondientes
     * por el valor en estos bits. SMPI tiene un maximo de 16, ya que hay 16 buffers del ADC
     * config3 = 0x6A00 = 0b 0110 1010 0000 0000
     * i want  = 0x0A00 = 0b 0000 1010 0000 0000
     */
    config3 = ADC_SAMPLE_TIME_10 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_1Tcy;
    /* AD1PCFGL and AD1PCFGH
     * registros ya configurados en pic_pc104_config.c
     * habilita los puertos AN13 y AN11 del PIC como pines analogos
     */
    configportL =  ENABLE_AN11_ANA & ENABLE_AN13_ANA;   //AN11 y AN13 son inputs analogicos
    configportH = 0xFFFF;       //1=digital 0=analog
    /* AD1CSSL and AD1CSSH
     * habilitar los puertos AN11 y AN 13 para el "input scan"
     */
    configscanL = 0b0010100000000000;   //1=scan 0=skip
    configscanH = 0x0000;
    /* AD1CHS0
     * i want 0x000B
     * input select register AD1CHS0. We will use MUXA
     * AN11 is the input pin (ADC)
     * AN13 is_ the output pin (DAC)
     */
    AD1CHS0bits.CH0NA = 0;
    AD1CHS0bits.CH0SA0= 1;
    AD1CHS0bits.CH0SA1= 1;
    AD1CHS0bits.CH0SA2= 0;
    AD1CHS0bits.CH0SA3= 1;
    AD1CHS0bits.CH0SA4= 0;
     //This function starts the A/D conversion and configures the ADC
    OpenADC10_v2(config1,config2,config3,configportL,configportH,configscanL,configscanH);
    EnableADC1; //set ADON to 0b1
    /* no se usaran las interrupciones del ADC, por que se usaran los timers para esto.
     * ConfigIntADC10(ADC_INT_DISABLE | ADC_INT_PRI_2 );   //Disable the interrup enable register IEC
     */
    IEC0bits.AD1IE = 0; //disable the ADC interrupts, cuz we'll not use it.
    IFS0bits.AD1IF = 0;   //clear the interrput flag for the ADC
    
    #if _FISICA_VERBOSE_ADC_CFG > 0
        printf("AD1CON1 : %X\n",AD1CON1);
        printf("AD1CON2 : %X\n",AD1CON2);
        printf("AD1CON3 : %X\n",AD1CON3);
        printf("AD1CHS0 : %X\n",AD1CHS0);
        printf("AD1PCFGL : %X\n",AD1PCFGL);
        printf("AD1CSSL : %X\n",AD1CSSL);
        printf("IEC0bits.AD1IE : %X\n",IEC0bits.AD1IE);
        printf("IFS0bits.AD1IF : %X\n",IFS0bits.AD1IF);
        printf("fis_ADC_config: ADC interruptions disabled...\n");
    #endif
}
/*  
 * Set the T4 control registers and the interruption register as well
 *  T4CON = T4_ON & T4_GATE_OFF & T4_IDLE_CON & T4_PS_1_64 & T4_SOURCE_INT
 */
void fis_Timer4_config(unsigned int period){    //CONFIGURAR EL POSTSCALER A 64
    //                      7654321076543210
    unsigned int config = 0b0000000000100000; //T4_ON & T4_GATE_OFF & T4_IDLE_CON & T4_PS_1_64 & T4_SOURCE_INT & T4_32BIT_MODE_OFF;
   
    WriteTimer4(0x0000);
    OpenTimer4( config, period );
    EnableIntT4;
    #if _FISICA_VERBOSE_TIMER4_CFG > 0
        printf("t4_config configuration data\n");
        printf("IEC1bits.T4IE: %u\n",IEC1bits.T4IE);
        printf("IEC1bits.T5IE: %u\n",IEC1bits.T5IE);
        printf("IFS1bits.T4IF: %u\n", IFS1bits.T4IF);
        printf("IFS1bits.T5IF: %u\n",IFS1bits.T5IF);
        printf("IPC6bits.T4IP: %X\n",IPC6bits.T4IP);
        printf("IPC6bits.T5IP: %X\n",IPC7bits.T5IP);
        printf("T4CON : %X\n",T4CON);
        printf("T5CON : %X\n",T5CON);
        printf("TMR4 : %u\n",TMR4);
        printf("TMR5 : %u\n",TMR5);
        printf("PR4 : %u\n",PR4);
        printf("PR5 : %u\n",PR5);    
        printf("t4_config done\n");
    #endif
}

/*  
 * Set the T5 control registers and the interruption register as well
 * T5CON = T5_ON & T5_GATE_OFF & T5_IDLE_CON & T5_PS_1_64 & T5_SOURCE_INT
 */
void fis_Timer5_config(unsigned int period){//CONFIGURAR EL POSTSCALER A 64
    //                      7654321076543210
    unsigned int config = 0b0000000000100000; //T5_ON & T5_GATE_OFF & T5_IDLE_CON & T5_PS_1_64 & T5_SOURCE_INT;
    WriteTimer5(0x0000);
    OpenTimer5( config, period );
    EnableIntT5;
    #if _FISICA_VERBOSE_TIMER5_CFG > 0
        printf("t5_config configuration data\n");
        printf("IEC1bits.T4IE: %u\n",IEC1bits.T4IE);
        printf("IEC1bits.T5IE: %u\n",IEC1bits.T5IE);
        printf("IFS1bits.T4IF: %u\n", IFS1bits.T4IF);
        printf("IFS1bits.T5IF: %u\n",IFS1bits.T5IF);
        printf("IPC6bits.T4IP: %X\n",IPC6bits.T4IP);
        printf("IPC6bits.T5IP: %X\n",IPC7bits.T5IP);
        printf("T4CON : %X\n",T4CON);
        printf("T5CON : %X\n",T5CON);
        printf("TMR4 : %u\n",TMR4);
        printf("TMR5 : %u\n",TMR5);
        printf("PR4 : %u\n",PR4);
        printf("PR5 : %u\n",PR5);    
        printf("t5_config done\n");
    #endif
}

/*  
 * DAC ISR
 */
void __attribute__((__interrupt__, auto_psv)) _T4Interrupt(void){
    #if _FISICA_VERBOSE_TIMER4_ISR > 0
        printf("ISR T4\n");
        #if _FISICA_VERBOSE_TIMER4_ISR >= 2
            printf("IEC1bits.T4IE: %u\n",IEC1bits.T4IE);
            printf("IEC1bits.T5IE: %u\n",IEC1bits.T5IE);
            printf("IFS1bits.T4IF: %u\n", IFS1bits.T4IF);
            printf("IFS1bits.T5IF: %u\n",IFS1bits.T5IF);
            printf("IPC6bits.T4IP: %X\n",IPC6bits.T4IP);
            printf("IPC6bits.T5IP: %X\n",IPC7bits.T5IP);
            printf("T4CON : %X\n",T4CON);
            printf("T5CON : %X\n",T5CON);
            printf("TMR4 : %u\n",TMR4);
            printf("TMR5 : %u\n",TMR5);
            printf("PR4 : %u\n",PR4);
            printf("PR5 : %u\n",PR5);
        #endif
    #endif

    if(fis_point == FIS_SIGNAL_POINTS || sens_buff_ind == (FIS_SENS_BUFF_LEN)){ //last point of a waveform

        fis_point = 0;
        //printf("fis_current_round = %u\n", fis_current_round);
        //if(fis_current_round < FIS_ROUNDS){    //there are some waveforms left
            //fis_current_round++;
            //srand(seed[fis_current_round]);
            //falta introducir un metodo para cambiar el periodo de la se?al!!!
            #if _FISICA_VERBOSE_TIMER4_ISR > 0
                printf("    srand(%d)\n",seed[fis_current_round]);
            #endif
        //}
        //else{
            //all the points were send to the DAC
            //IFS1bits.T4IF = 0;
            //return;
        //}
    }                
    else{//T4_Clear_Intr_Status_Bit;
        unsigned int arg = rand();
        #if _FISICA_VERBOSE_TIMER4_ISR > 0
            printf("rand(): %X\n",arg);
        #endif
        fis_payload_writeDAC(arg);

        fis_point++;    //update the global counter 

        sync = TRUE;
    }
    IFS1bits.T4IF = 0;
}

// ADC ISR
void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void){
    if (sync == TRUE){
        #if _FISICA_VERBOSE_TIMER5_ISR > 0
            printf("ISR T5\n");
            #if _FISICA_VERBOSE_TIMER5_ISR >= 2
                //printf("count: %u \n",count++);
                printf("IEC1bits.T4IE: %u\n",IEC1bits.T4IE);
                printf("IEC1bits.T5IE: %u\n",IEC1bits.T5IE);
                printf("IFS1bits.T4IF: %u\n", IFS1bits.T4IF);
                printf("IFS1bits.T5IF: %u\n",IFS1bits.T5IF);
                printf("IPC6bits.T4IP: %X\n",IPC6bits.T4IP);
                printf("IPC6bits.T5IP: %X\n",IPC7bits.T5IP);
                printf("T4CON : %X\n",T4CON);
                printf("T5CON : %X\n",T5CON);
                printf("TMR4 : %u\n",TMR4);
                printf("TMR5 : %u\n",TMR5);
                printf("PR4 : %u\n",PR4);
                printf("PR5 : %u\n",PR5);
            #endif
        #endif

        ConvertADC10(); //stop sampling and begins the conversion
        while(!AD1CON1bits.DONE);
        sens_buff[sens_buff_ind] = ReadADC10(0);
        
        #if _FISICA_VERBOSE_TIMER5_ISR > 0
            printf("sens_buff[%d] = %X\n", sens_buff_ind, sens_buff[sens_buff_ind]);
        #endif
    
        sens_buff_ind = sens_buff_ind+1;    //updates the index of the buffer
        fis_sample = fis_sample+1; //updates the global counter of samples
        
        if(sens_buff_ind == (FIS_SENS_BUFF_LEN)){ 
            #if _FISICA_VERBOSE_TIMER5_ISR > 0
                printf("ISR T5: sens_buff_ind == FIS_SENS_BUFF_LEN\r\n");
            #endif
            //sens_buff_ind = 0;
            if(fis_sample == FIS_SIGNAL_SAMPLES){
                fis_current_round++;
                srand(seed[fis_current_round]);
            }
            fis_iterate_pause(); //there are some work to do, we make a pause only
        }
    }
    IFS1bits.T5IF = 0;
}

////ISR del T4
////void _ISR _T4Interrupt(void){
//void __attribute__((__interrupt__, auto_psv)) _T4Interrupt(void){
//    printf("ISR T4\n");
//    //unsigned char c= rand();    //generate a random value for test only
//    //unsigned int arg=c;-
//    //arg=(arg<<8);
//    unsigned int arg = 0xFFFF;
//    printf("Voy a mandar %d por el DAC \n", arg);
//    fis_payload_writeDAC(arg);
//
//    //ClrWdt();
//    T4_Clear_Intr_Status_Bit;   //clear sthe interruption flag bit
//}
////ISR del T5
////void _ISR _T5Interrupt(void){
//void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void){
//    printf("ISR Timer5\n");
//    unsigned long i;
//    unsigned int sens1 = 0;//, sens2;
//    //unsigned int aux1 = 0;
//    
//    //convert ADC_SCAN_AN11
//    ConvertADC10();   //Starts conversion by clearing sample bit (SAMP)
//    for(i=0;i<0x0000FFFF;i++){}//busy waiting ? 
//    
//    //aux1 = ADC1BUF0;
//    //printf("ConvertADC10() : %d\n",aux1);
//    
//    //convert ADC_SCAN_AN13
//    //ConvertADC10();   //Starts conversion by clearing sample bit (SAMP)
//    //for(i=0;i<0x0000000F;i++){} //busy waiting ?
//    
//    //aux1 = ADC1BUF0;
//    //printf("ConvertADC10() : %d\n",aux1);
//    
//    //sens2=ReadADC10(0);
//    //printf("ReadADC10() : %d\n", sens2);
//    sens1=ReadADC10(0); //reads ADC1BUF0 (the A-D conversion value)
//    printf("ReadADC10() : %d\n", sens1);
//
//    #if (_FISICA_VERBOSE_TIMER_ISR>=2)
//        printf("sens1 = %d\n", sens1);
//        //printf("sens2 = %d\n", sens2);
//    #endif
//
//    sens_buff[sens_buff_ind]=sens1; //stores the value measured
//    sens_buff_ind++;    //updates the index of the intermediate buffer
//    //sens_buff[sens_buff_ind]=sens2;
//    //sens_buff_ind++;
//
//    #if (_FISICA_VERBOSE_TIMER_ISR>=2)
//        printf("sens_buff_ind=%d\n", sens_buff_ind);
//    #endif
//
//    if(sens_buff_ind>=(FIS_SENS_BUFF_LEN)){
//        #if (_FISICA_VERBOSE_TIMER_ISR>=2)
//            con_printf("ISR T5: sens_buff_ind == FIS_SENS_BUFF_LENGTH\r\n");
//        #endif
//        fis_stop_expFis();
//        //fis_save_sens_buff();
//    }
//    
//    T5_Clear_Intr_Status_Bit;   //clears the interruption flag bit
//}
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


//void hist(void){
//
//    DAT_GnrlPurpBuff gpb_frec_i;
//    unsigned int i,j; unsigned int max;
//    long v1, v2, mul, ran, prev; unsigned long v1u, v2u;
//
//    //realiza multiplicacion para cada frecuencia
//    max=(2*FIS_SENS_BUFF_LEN);//(FIS_REPEAT_PER_ROUND*FIS_SAMP_PER_ROUND);
//    for(gpb_frec_i=dat_gpb_expFis_f0; gpb_frec_i<=dat_gpb_expFis_f9; gpb_frec_i++){
//        for(i=0;i<max;i=i+2){
//            v1 = (long)dat_getGPB(gpb_frec_i, i);
//            v2 = (long)dat_getGPB(gpb_frec_i, i+1);
//            mul=v1*v2;
//            dat_setGPB(gpb_frec_i, i, (unsigned int)mul);
//            dat_setGPB(gpb_frec_i, i+1, (unsigned int)(mul>>16) );
//        }
//    }
//
//    //inicializa buffer del hist
//    for(i=0;i<(512*2*2);i++){
//        dat_setGPB(dat_gpb_expFis_hist, i, 0xFFFF);
//    }
//
//    //realiza histograma para cada frecuencia
//    max=(2*FIS_SENS_BUFF_LEN);//(FIS_REPEAT_PER_ROUND*FIS_SAMP_PER_ROUND);
//    for(gpb_frec_i=dat_gpb_expFis_f0; gpb_frec_i<=dat_gpb_expFis_f9; gpb_frec_i++){
//
//        for(i=0;i<max;i=i+2){
//            v1u = (unsigned long)dat_getGPB(gpb_frec_i, i);
//            v2u = (unsigned long)dat_getGPB(gpb_frec_i, i+1);
//            mul = (long)( (v2u<<16)|v1u );
//
//            //esto da aca ya no sirve :/
//            for(j=0;TRUE;j++){
//
//                if(mul < 0 ){
//                    ran=mul/2;
//                    if( ran==0 || ran==(-1) ){
//                        //rango j-esimo negativo
//                        dat_setGPB(dat_gpb_expFis_hist, j, j);
//                        //tiene una frecuencia de
//                        prev = dat_getGPB(dat_gpb_expFis_hist, (10000)+j);
//                        prev++;
//                        dat_setGPB(dat_gpb_expFis_hist, (10000)+j, prev);
//                        break;
//                    }
//                }
//                else{
//                    ran=mul/2;
//                    if( ran==0 || ran==1 ){
//                        //rango j-esimo positivo
//                        dat_setGPB(dat_gpb_expFis_hist, (10000*2)+j, j);
//                        //tiene una frecuencia de
//                        prev = dat_getGPB(dat_gpb_expFis_hist, (10000*3)+j);
//                        prev++;
//                        dat_setGPB(dat_gpb_expFis_hist, (10000*3)+j, prev);
//                        break;
//                    }
//                }
//
//            }
//        }
//    }
//
//    /*
//    int b_dat[50.000];
//    int b_cntn[512];
//    int b_cntp[512];
//    int i,j, ran;
//    for(i=0;i<50.000;i++){
//
//        for(j=0;TRUE;j++){
//
//            if(b_dat[i] < 0 ){
//                ran=b_dat[i]/2;
//                if( ran==0 || ran==(-1) ){
//                    b_cntn[j]=b_cntn[j]+1;
//                    break;
//                }
//            }
//            else{
//                ran=b_dat[i]/2;
//                if( ran==0 || ran==1 ){
//                    b_cntp[j]=b_cntp[j]+1;
//                    break;
//                }
//            }
//        }
//    }
//    */
//}

//void fis_Timer4_config(unsigned int period){
//    #if _FISICA_VERBOSE_TIMER_CFG > 0
//        printf("fis_Timer4_config: Timer4 initializing...\n");
//    #endif
//    //Timer 4 as 16-bit synchronous
//    //PR4 is the period register for Timer4
//    //                      7654321076543210
//    //unsigned int config = 0b1000000000110000; //T4_ON & T4_GATE_ON & T4_IDLE_CON & T4_PS_1_1 & T4_SOURCE_INT;
//    //unsigned int config = T4_OFF & T4_IDLE_CON & T4_GATE_OFF & T4_PS_1_256 & T4_SOURCE_INT & T4_32BIT_MODE_OFF;
//    //                      7654321076543210
//    //config = T4_ON & T4_GATE_OFF & T4_IDLE_CON & T4_PS_1_8 & T4_SOURCE_INT & T4_32BIT_MODE_OFF;
//    //unsigned int period2 = 0b0000000001001011;
//    //WriteTimer4(0x0000);    //clears the  TMR4 count register
//    //ClrWdt();
//    //ConfigIntTimer4(T4_INT_ON & T4_INT_PRIOR_1);
//    //OpenTimer4(config, period);  //loads T4CON and PR4
//    T4CON = 0b0000000000000000;
//    TMR4 = 0b00000z00000000000;
//    PR4 = period;
//    IFS1bits.T4IF = 0b0;
//    IEC1bits.T4IE = 0b1;
//    IPC6bits.T4IP = 6;
//    
//    #if _FISICA_VERBOSE_TIMER_CFG > 0
//        unsigned int tmp = 0;
//        tmp = IEC1bits.T4IE;    //enable
//        //tmp = IEC1 & (1<<11);   
//        printf("IEC1bits.T4IE: %d\n",tmp);
//        tmp = IFS1bits.T4IF;    //flag
//        //tmp = IFS1 & (1<<11);
//        printf("IFS1bits.T4IF: %d\n",tmp);
//        tmp = IPC6bits.T4IP;    //priority
//        //tmp = IPC6 & ((1<<12) | (1<<13) | (1<<14));
//        printf("IPC6bits.T4IP: %d\n",tmp);
//        tmp = T4CON;    //register control
//        printf("T4CON : %X\n",tmp);
//        tmp = TMR4; //count
//        printf("TMR4 : %u\n",tmp);
//        tmp = PR4;  //period
//        printf("PR4 : %u\n",tmp);
//        printf("fis_Timer4_config: done\n");
//    #endif
//}
//void fis_Timer5_config(unsigned int period){
//    #if _FISICA_VERBOSE_TIMER_CFG > 0
//        printf("fis_Timer5_config: Timer5 initializing...\n");
//    #endif
//    //Timer5 is the only timer that offers ADC Trigger Event
//    //PR5 is the period register for Timer5
//    //                        7654321076543210
//    //unsigned int config = T5_OFF & T5_IDLE_CON & T5_GATE_OFF & T5_PS_1_256 & T5_SOURCE_INT;
//    //WriteTimer5(0x0000);    //clears the TMR5 count register
//    //ClrWdt();
//    //ConfigIntTimer5(T5_INT_ON & T5_INT_PRIOR_1);    //Clears the T5IF flag, enables the 
//    //OpenTimer5(config, period);   //loads T5CON and PR5
//    T5CON = 0b0000000000000000;
//    TMR5 = 0b0000000000000000;
//    PR5 = period;
//    IFS1bits.T5IF = 0b0;
//    IEC1bits.T5IE = 0b1;
//    IPC7bits.T5IP = 7;
//    #if _FISICA_VERBOSE_TIMER_CFG > 0
//        unsigned int tmp = 0;
//        tmp = IEC1bits.T5IE;    //enable
//        //tmp = IEC1 & (1<<12);
//        //AQUI SE QUEDA PEGADO Y SE RESETEA!!
//        
//        printf("IEC1bits.T5IE: %d\n",tmp);
//        tmp = IFS1bits.T5IF;    //flag
//        //tmp = IFS1 & (1<<12);
//        printf("IFS1bits.T5IF: %d\n",tmp);
//        tmp = IPC7bits.T5IP;    //priority
//        //tmp = IPC7 & ((1<<0) | (1<<1) | (1<<2));
//        printf("IPC7bits.T5IP: %d\n",tmp);
//        tmp = T5CON;    //register control
//        printf("T5CON : %X\n",tmp);
//        tmp = TMR5; //count
//        printf("TMR5 : %u\n",tmp);
//        tmp = PR5;  //period
//        printf("PR5 : %u\n",tmp);
//        printf("fis_Timer5_config: done\n");
//    #endif
//}
//unsigned int fis_frec_i_to_ADC_period(DAT_GnrlPurpBuff pay_frec_i){
//    unsigned int ADC_period;
//    switch(pay_frec_i){
//        case dat_gpb_expFis_f0:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 20;
//        break;
//        case dat_gpb_expFis_f1:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 21;
//        break;
//        case dat_gpb_expFis_f2:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 22;
//        break;
//        case dat_gpb_expFis_f3:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 23;
//        break;
//        case dat_gpb_expFis_f4:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 24;
//        break;
//        case dat_gpb_expFis_f5:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 25;
//        break;
//        case dat_gpb_expFis_f6:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 26;
//        break;
//        case dat_gpb_expFis_f7:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 27;
//        break;
//        case dat_gpb_expFis_f8:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 28;
//        break;
//        case dat_gpb_expFis_f9:
//            //              7654321076543210
//            ADC_period = (0b0000000010110100);
//            ADC_period = 29;
//        break;
//        default:
//            ADC_period=0;
//        break;
//    }
//
//    return ADC_period;
//}

//void fis_save_sens_buff_to_GPB(DAT_GnrlPurpBuff frec_i, int rst_gbp_indx){
//    unsigned int ind; unsigned long prom=0;
//
//    //esperar mientras se termina de llenar el sens_buff (buffer intermedio)
//    while( sens_buff_ind<(FIS_SENS_BUFF_LEN) ){
//        __delay_ms(1000);
//    }
//
//    //calculo promedio sens1
//    for(ind=0;ind<FIS_SENS_BUFF_LEN;ind=ind+2){
//        prom=prom+sens_buff[ind];
//    }
//    prom=prom/FIS_SENS_BUFF_LEN;
//    //resto el promedio sens1
//    for(ind=0;ind<FIS_SENS_BUFF_LEN;ind=ind+2){
//        sens_buff[ind]=sens_buff[ind]-prom;
//    }
//
//    //calculo promedio sens2
//    for(ind=1;ind<FIS_SENS_BUFF_LEN;ind=ind+2){
//        prom=prom+sens_buff[ind];
//    }
//    prom=prom/FIS_SENS_BUFF_LEN;
//    //resto el promedio sens2
//    for(ind=1;ind<FIS_SENS_BUFF_LEN;ind=ind+2){
//        sens_buff[ind]=sens_buff[ind]-prom;
//    }
//
//    //guardo en memSD
//    static unsigned int gpb_indx;
//    if( rst_gbp_indx==1 ){
//        gpb_indx=0;
//    }
//    for(ind=0;ind<FIS_SENS_BUFF_LEN;ind++){
//        //Set DAT_GnrlPurpBuff
//        dat_set_GPB(frec_i, gpb_indx, sens_buff[ind] );
//        gpb_indx++;
//    }
//}
//******************************************************************************
