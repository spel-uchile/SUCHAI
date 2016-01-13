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

//cantidad de puntos de de cada se?al generada (largo)
#define FIS_SIGNAL_POINTS (1000) //(10000)
//numero de muestras por punto generado de la se?al (minimo 2 para cumplir con Nyquist)
#define FIS_SAMPLES_PER_POINT (4)
//cantidad de muestras que tiene una se?al/waveform
#define FIS_SIGNAL_SAMPLES ((FIS_SIGNAL_POINTS)*(FIS_SAMPLES_PER_POINT))
//number of seed for the srand() function calls
#define FIS_SRAND_SEEDS     (4)//(FIS_ROUNDS)
//maximun size for the buffer
#define FIS_MAX_SENS_BUFF_LEN (1000)

#define FIS_NUM_ROUNDS  (1)

#define FIS_SENS_BUFF_LEN (200)
unsigned int fis_get_total_number_of_samples(void);
unsigned int fis_get_sens_buff_size(void);
BOOL fis_sens_buff_isFull(void);
int fis_wait_busy_wtimeout(unsigned int timeout);
void fis_print_sens_buff(void);
void fis_sens_buff_init(void);
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
unsigned int fis_iterate_config(unsigned int inputSignalPeriod, int rounds);
//unsigned int fis_iterate_config(unsigned int inputSignalPeriod[], int len, int rounds);

/**
 * Helper to iterate ONE TIME over one of the "_rounds_per_ADC_period"-times 
 * a SINGLE ADC_period must execute
 * @return TRUE if success in execution, FALSE Otherwise
 */
void fis_iterate(unsigned int* rc, unsigned int timeout_seg);

typedef enum{
    FIS_OFF,
    FIS_READY,
    FIS_RUNNING,
    FIS_WAITING,
    FIS_DONE
} Fis_States;

Fis_States fis_next_state_logic(Fis_States curr_state);
Fis_States fis_current_state_control(Fis_States curr_state);

void fis_run(unsigned int period);
void fis_iterate_stop(void);

void fis_ADC_config(void);
void fis_Timer4_config(unsigned int period);
void fis_Timer5_config(unsigned int period);

void fis_payload_writeDAC(unsigned int arg);

void fis_iterate_pause(void);
void fis_iterate_resume(void);
void fis_payload_print_seed(void);

#endif
