#ifndef _FIS_PAYLOAD_
#define _FIS_PAYLOAD_

#include <adc.h>

#include "interfaz_SPI.h"
#include "DebugIncludes.h"  //para con_printf
#include "dataRepository.h"

// expFis
#define FIS_SAMP_PER_ROUND      100UL//100UL
//numero de muestras por ronda (a cierta frec)
#define FIS_REPEAT_PER_ROUND    (100UL*FIS_SENS_NUM)//(500UL*FIS_SENS_NUM)
//numero de frecuencias
#define FIS_SENS_NUM            10UL
//numero de muestras en el buffer

void fis_start_expFis(unsigned int period);
void fis_stop_expFis(void);

void fis_ADC_config(void);
void fis_Timer4_config(unsigned int period);
void fis_Timer5_config(unsigned int period);

void fis_testDAC(void);
void fis_payload_writeDAC(unsigned int arg);
void fis_print_sens_buff(void);
void fis_save_sens_buff_to_GPB(DAT_GnrlPurpBuff frec_i, int rst_gbp_indx);
unsigned int fis_frec_i_to_ADC_period(DAT_GnrlPurpBuff pay_frec_i);

#endif