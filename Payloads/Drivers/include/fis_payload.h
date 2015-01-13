#ifndef _FIS_PAYLOAD_
#define _FIS_PAYLOAD_

#include <adc.h>
#include <stdlib.h> //rand, itoa

#include "interfaz_SPI.h"
#include "DebugIncludes.h"  //para con_printf
#include "dataRepository.h"

// expFis
#if(SCH_PAY_FIS_ONBOARD==1)
    #define FIS_SENS_BUFF_LEN      20UL//100UL
#else
    #define FIS_SENS_BUFF_LEN      1UL
#endif
//numero de muestras por ronda (a cierta frec)
#define FIS_REPEAT_PER_ROUND    (100UL*FIS_SENS_NUM)//(500UL*FIS_SENS_NUM)
//numero de frecuencias
#define FIS_SENS_NUM            10UL
//numero de muestras en el buffer



BOOL fis_sens_buff_isFull(void);
int fis_wait_busy_wtimeout(void);
void fis_print_sens_buff(void);
void fis_erase_sens_buff(void);
unsigned int fis_get_sens_buff_i(int ind);
void fis_testDAC(void);

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
int fis_iterate_config(const unsigned int _ADC_period[], int _len, int _rounds_per_ADC_period);
/**
 * Helper to iterate ONE TIME over one of the "_rounds_per_ADC_period"-times 
 * a SINGLE ADC_period must execute
 * @return TRUE if success in execution, FALSE Otherwise
 */
BOOL fis_iterate(void);

//void fis_save_sens_buff_to_GPB(DAT_GnrlPurpBuff frec_i, int rst_gbp_indx);
//unsigned int fis_frec_i_to_ADC_period(DAT_GnrlPurpBuff pay_frec_i);

void fis_start_expFis(unsigned int period);
void fis_stop_expFis(void);

void fis_ADC_config(void);
void fis_Timer4_config(unsigned int period);
void fis_Timer5_config(unsigned int period);

void fis_payload_writeDAC(unsigned int arg);

#endif