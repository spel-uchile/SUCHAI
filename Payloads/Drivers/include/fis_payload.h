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

#define FIS_SIGNAL_POINTS (16000L)
#define FIS_SAMPLES_PER_POINT (4L)
#define FIS_SIGNAL_SAMPLES ((unsigned int)(FIS_SIGNAL_POINTS)*(FIS_SAMPLES_PER_POINT))
#define FIS_SENS_BUFF_LEN (400L)
#define FIS_POINTS_INB4 (500L)
#define fisRand()   ((rand())<<(1))

unsigned int fis_get_total_number_of_samples(void);
unsigned int fis_get_sens_buff_size(void);
BOOL fis_sens_buff_isFull(void);
int fis_wait_busy_wtimeout(unsigned int timeout);
void fis_print_sens_buff(void);
void fis_sens_buff_reset(void);
unsigned int fis_get_sens_buff_i(int ind);
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
unsigned int fis_set_seed(unsigned int seed, int rounds);
unsigned int fis_set_adcPeriod(unsigned int inputSignalPeriod, int rounds);

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
void fis_payload_print_seed(unsigned int seed);
void fis_payload_print_seed_full(unsigned int seed);
unsigned int fis_reset_iteration_variables(void);
#endif
