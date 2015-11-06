#ifndef _FIS_PAYLOAD_
#define _FIS_PAYLOAD_

#include <adc.h>
#include <stdlib.h> //rand, itoa

#include "interfaz_SPI.h"
#include "DebugIncludes.h"  //para con_printf
#include "dataRepository.h"

// comand verbose
#define FIS_CMD_VERBOSE (1)

// expFis variables
#define FIS_STATE_OFF   (0)
#define FIS_STATE_READY (1)
#define FIS_STATE_WAITING (2)
#define FIS_STATE_WORKING   (3)
#define FIS_STATE_DONE (4)

//cantidad de waveforms a utilizar
#define FIS_ROUNDS_PER_PERIOD    (3UL)
//cantidad de puntos que conforman un waveform
#define FIS_WAVEFORM_SIZE (60UL)
//cantidad total de puntos a generar
//numero de muestras por punto generado por el DAC
#define FIS_SAMPLES_PER_POINT (2UL)
//total amount of points to be generated with the DAC
#define FIS_TOTAL_POINTS    (FIS_WAVEFORM_SIZE)
//#define FIS_TOTAL_POINTS    ((FIS_ROUNDS_PER_PERIOD)*(FIS_WAVEFORM_SIZE))
//total amount of samples to taken with the ADC
//#define FIS_TOTAL_SAMPLES (FIS_TOTAL_POINTS)*(FIS_SAMPLES_PER_POINT)
#define FIS_TOTAL_SAMPLES (FIS_TOTAL_POINTS)*(FIS_SAMPLES_PER_POINT)
//number of seed for the srand() function calls
#define FIS_SRAND_SEEDS (FIS_ROUNDS_PER_PERIOD)
//maximun size for the buffer
#define FIS_MAX_SENS_BUFF_LEN (100UL)
/*
#define FIS_REPEAT_PER_ROUND    (100UL*FIS_SENS_NUM)//(500UL*FIS_SENS_NUM)
//numero de frecuencias
#define FIS_SENS_NUM            (10UL)
//numero de muestras en el buffer
*/
#if (SCH_PAY_FIS_ONBOARD==1)
    #if ((FIS_WAVEFORM_SIZE)*(FIS_SAMPLES_PER_POINT)) <= (FIS_MAX_SENS_BUFF_LEN)
        //size of sens_buff is equal to the size of a waveform, times the samples for each point
        #define FIS_SENS_BUFF_LEN ((FIS_WAVEFORM_SIZE)*(FIS_SAMPLES_PER_POINT))
    #else
        #define FIS_SENS_BUFF_LEN FIS_MAX_SENS_BUFF_LEN
    #endif
#else
    #define FIS_SENS_BUFF_LEN   1UL
#endif

unsigned int fis_get_total_number_of_samples(void);
unsigned int fis_get_sens_buff_size(void);
BOOL fis_sens_buff_isFull(void);
int fis_wait_busy_wtimeout(unsigned int timeout);
void fis_print_sens_buff(void);
void fis_reset_sens_buff(void);
unsigned int fis_get_sens_buff_i(int ind);
//void fis_testDAC(void);
void fis_testDAC(unsigned int value);
void fis_Timer45_begin(void);
unsigned int fis_get_sens_buff_size(void);
unsigned int fis_get_state(void);

/**
 * Return TRUE if the last round of the las ADC_period was completed, if so, the
 * experiment is complete
 * @return TRUE if the last round of the las ADC_period was completed, FALSE otherwise
 */
BOOL fis_iterate_isComplete();
/**
 * Set the values for the expFis experiment to operate. It returns the
 * lenght of the internal buffer (FIS_SENS_BUFF_LEN)
 * @param _ADC_period Buffer containing the ADC_periods expFis must execute
 * @param _len Indicates how many values the ADC_period buffer has
 * @param _rounds_per_ADC_period How many rounds there will be for every ADC_period
 * @return the lenght of the internal buffer (FIS_SENS_BUFF_LEN)
 */
unsigned int fis_iterate_config(const unsigned int _ADC_period[], int _len, int _rounds_per_ADC_period);
/**
 * Helper to iterate ONE TIME over one of the "_rounds_per_ADC_period"-times 
 * a SINGLE ADC_period must execute
 * @return TRUE if success in execution, FALSE Otherwise
 */
//unsigned int fis_iterate(unsigned int previous_state);
void fis_iterate(unsigned int* rc, unsigned int timeout_seg);

//void fis_save_sens_buff_to_GPB(DAT_GnrlPurpBuff frec_i, int rst_gbp_indx);
//unsigned int fis_frec_i_to_ADC_period(DAT_GnrlPurpBuff pay_frec_i);

void fis_start_expFis(unsigned int period);
void fis_stop_expFis(void);

void fis_ADC_config(void);
void fis_Timer4_config(unsigned int period);
void fis_Timer5_config(unsigned int period);

void fis_payload_writeDAC(unsigned int arg);

void fis_pause_expFis(void);
void fis_iterate_resume(void);

#endif
