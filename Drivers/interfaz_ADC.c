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

#include "interfaz_ADC.h"	//uso mis definiciones 

void Abrir_ADC_default(void){
	unsigned int config1; unsigned int config2; unsigned int config3;
	unsigned int configportL,configportH, configscanL, configscanH;
	config1 = ADC_MODULE_ON & ADC_IDLE_CONTINUE & ADC_FORMAT_INTG & ADC_CLK_MANUAL & ADC_AUTO_SAMPLING_ON ;
	config2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_INTR_2_CONV & ADC_ALT_BUF_ON & ADC_ALT_INPUT_OFF;
	config3 = ADC_SAMPLE_TIME_10 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_1Tcy;
	configportL = ENABLE_AN4_ANA & ENABLE_AN5_ANA;
	configportH = 0xffff;   //1=digital 0=analog
	configscanL = ADC_SCAN_AN4 & ADC_SCAN_AN5; //1=scan 0=skip
	configscanH = 0x0000;
	//AD1CHS= ADC_CH0_NEG_SAMPLEA_VREFN & ADC_CH0_POS_SAMPLEB_AN15;
        AD1CHS0= 0x284; //neg=Vrefneg para A y B pos=4 A y 5 para B
	OpenADC10_v2(config1,config2,config3,configportL,configportH,configscanL,configscanH);

        //ConfigIntADC10(ADC_INT_ENABLE & ADC_INT_PRI_7 );
}
/*
void _ISR _ADC1Interrupt(void){
    int valor1=ReadADC10(0);
    int valor2=ReadADC10(1);

    //guardar(valor1, valor2);

    IFS0bits.AD1IF = 0;

}
*/
