/******************************************************************************
*
*                  LIBRERIA PARA LA INTERFAZ DEL PERIFERICO ADC
*
******************************************************************************
* FileName:        interfaz_ADC.c
* Dependencies:    adc.h
* Processor:       PIC24FJ256GA110
* Compiler:        MPLAB C30
* Autor:	       Tomas Opazo T.
*/

#include <adc.h>
/*
La conversion de ADC se realiza por medio de alguno de los 16 pines AN0-AN15.
Los pines elegidos para la conversion, reciben voltajes analogicos, y pasan al S/H (sample/hold), que lee el voltaje de un pin
y luego lo mantiene hasta que pasa al modulo SAR (successive approximation register) que los transforma a digital, finalmente 
el valor se guarda en forma sucesiva en el ADC1BUFF.

El tiempo de conversion esta compuesto por:
sample time 	= nTda, pero tal que Tda>75ns=0,075us
conversion time = 12Tad
Ya que, Tda=Tcy(ADCS+1). Si Fcy=16Mhz => Tcy=0,0625us => ADCSmin=1 => Tda=2*Tcy=2*(62,5ns)=125ns
=> MIN sample time		=10*Tda_min=750ns


Existen dos formas de seleccionar que pines pasaran al modulo S/H  y luego al SAR
1)MUXA y MUXB que selccionan dos grupos de pines que escanearán de manera ciclica
2)AD1CSSL y AD1CSSH que selecciona que pines se escanearán

Para congifurar y habilitar el AD1 se debe usar OpenADC10_v2(ad1con1,ad1con2,ad1con3,ad1pcfgl,ad1pcfgh,ad1cssl,ad1cssh); 
y ademas setChanADC10(ad1chs0);
Donde:

AD1CON1 control register definition:
ADC_MODULE_ON               A/D Converter on
ADC_MODULE_OFF              A/D Converter off

ADC_IDLE_CONTINUE           A/D Operate in Idle mode
ADC_IDLE_STOP               A/D Stop in Idle mode

ADC_FORMAT_SIGN_FRACT       A/D data format signed fractional
ADC_FORMAT_FRACT            A/D data format fractional
ADC_FORMAT_SIGN_INT         A/D data format signed integer
ADC_FORMAT_INTG             A/D data format integer

ADC_CLK_AUTO                Internal counter ends sampling and starts conversion (Auto convert) 
*/
#define ADC_CLK_CTMU_                0xFF9F // CTMU event ends sampling and starts conversion
#define ADC_CLK_TMR5_                0xFF7F // Timer5 compare ends sampling and starts conversion
/*
ADC_CLK_TMR3                Timer3 compare ends sampling and starts conversion
ADC_CLK_INT0                Active transition on INT0 ends sampling and starts conversion
ADC_CLK_MANUAL              Clearing sample (SAMP) bit ends sampling and starts conversion

ADC_AUTO_SAMPLING_ON        Sampling begins immediately after last conversion
ADC_AUTO_SAMPLING_OFF       Sampling begins when SAMP bit is set

ADC_SAMP_ON                 sample / hold amplifiers are sampling
ADC_SAMP_OFF                sample / hold amplifiers are holding

AD1CON2 control register definition:
ADC_VREF_AVDD_AVSS          A/D Voltage reference configuration Vref+ is AVdd and Vref- is AVss
ADC_VREF_EXT_AVSS           A/D Voltage reference configuration Vref+ external and Vref- is AVss
ADC_VREF_AVDD_EXT           A/D Voltage reference configuration Vref+ AVdd and Vref- external
ADC_VREF_EXT_EXT            A/D Voltage reference configuration both Vref+ and Vref- are external

ADC_SCAN_ON                 A/D Scan Input Selections for CH0 during SAMPLE A
ADC_SCAN_OFF                A/D Do notScan Input Selections for CH0+ during SAMPLE A

ADC_INTR_EACH_CONV          Interrupts at the completion of conversion of each sample
ADC_INTR_2_CONV             Interrupts at the completion of conversion of 2 samples
ADC_INTR_3_CONV             Interrupts at the completion of conversion of 3 samples
ADC_INTR_4_CONV             Interrupts at the completion of conversion of 4 samples
...
ADC_INTR_15_CONV            Interrupts at the completion of conversion of 15 samples
ADC_INTR_16_CONV            Interrupts at the completion of conversion of 16 samples

ADC_ALT_BUF_ON              Buffer configured as 2 8-word buffers
ADC_ALT_BUF_OFF             Buffer configured as 1 16-word buffer --------> elija esta y no discuta!

ADC_ALT_INPUT_ON            alternate between MUXA and MUXB
ADC_ALT_INPUT_OFF           use MUXA only

AD1CON3 register definition:
ADC_CONV_CLK_INTERNAL_RC    A/D Conversion Clock Source internal RC Clock
ADC_CONV_CLK_SYSTEM         A/D Conversion Clock Source Clock derived from system clock

ADC_SAMPLE_TIME_0           A/D Auto Sample Time 0 Tad--------> NO elija esta
ADC_SAMPLE_TIME_1           A/D Auto Sample Time 1 Tad
...
ADC_SAMPLE_TIME_30          A/D Auto Sample Time 30 Tad
ADC_SAMPLE_TIME_31          A/D Auto Sample Time 31 Tad

ADC_CONV_CLK_1Tcy          A/D Conversion Clock is Tcy
ADC_CONV_CLK_2Tcy          A/D Conversion Clock is 2*Tcy
...
ADC_CONV_CLK_255Tcy        A/D Conversion Clock is 255*Tcy
ADC_CONV_CLK_256Tcy        A/D Conversion Clock is 256*Tcy

AD1PCFGL register definition (x=0...15):
ENABLE_ANx_DIG              Enable ANx in digital mode
ENABLE_ANx_ANA              Enable ANx in analog mode

AD1PCFGH register definition:
ENABLE_AN16_DIG             Enable AN16 in digital mode. Band gap Voltage reference/2 is enabled
ENABLE_AN16_ANA             Enable AN16 in analog mode. Band gap Voltage reference/2 is disabled
ENABLE_AN17_DIG             Enable AN17 in digital mode. Band gap Voltage reference is disabled
ENABLE_AN17_ANA             Enable AN17 in analog mode. Band gap voltage reference is enabled

AD1CSSL control register definition (x=0...15):
ADC_SCAN_ANx                    Enable Input Scan ANx
ADC_SKIP_SCAN_ANx      		Disable Input Scan ANx

AD1CSSH register definition:
ADC_SCAN_AN16     			Enable Input Scan AN16. Internal Vbg/2 channel selected for input scan
ADC_SKIP_SCAN_AN16    		Disable Input Scan AN16. Analog channel omitted from input scan
ADC_SCAN_AN17     			Enable Input Scan AN17. Internal Vbg channel selected for input scan
ADC_SKIP_SCAN_AN17    		Disable Input Scan AN17. Analog channel omitted from input scan

Y fianlmente el registro AD1CHS:
ADC_CH0_NEG_SAMPLEB_AN1     CH0 negative input is AN1
ADC_CH0_NEG_SAMPLEB_VREFN   CH0 negative input is VREF-
*/
#define ADC_CH0_POS_SAMPLEB_VBG_	 0xFFFF // A/D CH0 pos i/p sel for SAMPLE B is VBG
#define ADC_CH0_POS_SAMPLEB_VBGDIV2_ 0xFEFF // A/D CH0 pos i/p sel for SAMPLE B is VBG/2
#define ADC_CH0_POS_SAMPLEB_AN15_    0xEFFF // A/D CH0 pos i/p sel for SAMPLE B is AN12
#define ADC_CH0_POS_SAMPLEB_AN14_    0xEEFF // A/D CH0 pos i/p sel for SAMPLE B is AN12
#define ADC_CH0_POS_SAMPLEB_AN13_    0xEDFF // A/D CH0 pos i/p sel for SAMPLE B is AN12
#define ADC_CH0_POS_SAMPLEB_AN12_    0xECFF // A/D CH0 pos i/p sel for SAMPLE B is AN12
#define ADC_CH0_POS_SAMPLEB_AN11_    0xEBFF // A/D CH0 pos i/p sel for SAMPLE B is AN11
#define ADC_CH0_POS_SAMPLEB_AN10_    0xEAFF // A/D CH0 pos i/p sel for SAMPLE B is AN10
#define ADC_CH0_POS_SAMPLEB_AN9_     0xE9FF // A/D CH0 pos i/p sel for SAMPLE B is AN5
#define ADC_CH0_POS_SAMPLEB_AN8_     0xE8FF // A/D CH0 pos i/p sel for SAMPLE B is AN12
#define ADC_CH0_POS_SAMPLEB_AN7_     0xE7FF // A/D CH0 pos i/p sel for SAMPLE B is AN11
#define ADC_CH0_POS_SAMPLEB_AN6_     0xE6FF // A/D CH0 pos i/p sel for SAMPLE B is AN10
#define ADC_CH0_POS_SAMPLEB_AN5_     0xE5FF // A/D CH0 pos i/p sel for SAMPLE B is AN5
#define ADC_CH0_POS_SAMPLEB_AN4_     0xE4FF // A/D CH0 pos i/p sel for SAMPLE B is AN4
#define ADC_CH0_POS_SAMPLEB_AN3_     0xE3FF // A/D CH0 pos i/p sel for SAMPLE B is AN3
#define ADC_CH0_POS_SAMPLEB_AN2_     0xE2FF // A/D CH0 pos i/p sel for SAMPLE B is AN2
#define ADC_CH0_POS_SAMPLEB_AN1_     0xE1FF // A/D CH0 pos i/p sel for SAMPLE B is AN1
#define ADC_CH0_POS_SAMPLEB_AN0_     0xE0FF // A/D CH0 pos i/p sel for SAMPLE B is AN0
/*
ADC_CH0_NEG_SAMPLEA_AN1     A/D CH0 neg I/P sel for SAMPLE A is AN1
ADC_CH0_NEG_SAMPLEA_VREFN   A/D CH0 neg I/P sel for SAMPLE A is Vrefn
*/
#define ADC_CH0_POS_SAMPLEA_VBG_	 0xFFFF // A/D CH0 pos i/p sel for SAMPLE A is VBG
#define ADC_CH0_POS_SAMPLEA_VBGDIV2_ 0xFFFE // A/D CH0 pos i/p sel for SAMPLE A is VBG/2
#define ADC_CH0_POS_SAMPLEA_AN15_    0xFFEF // A/D CH0 pos i/p sel for SAMPLE A is AN12
#define ADC_CH0_POS_SAMPLEA_AN14_    0xFFEE // A/D CH0 pos i/p sel for SAMPLE A is AN12
#define ADC_CH0_POS_SAMPLEA_AN13_    0xFFED // A/D CH0 pos i/p sel for SAMPLE A is AN12
#define ADC_CH0_POS_SAMPLEA_AN12_    0xFFEC // A/D CH0 pos i/p sel for SAMPLE A is AN12
#define ADC_CH0_POS_SAMPLEA_AN11_    0xFFEB // A/D CH0 pos i/p sel for SAMPLE A is AN11
#define ADC_CH0_POS_SAMPLEA_AN10_    0xFFEA // A/D CH0 pos i/p sel for SAMPLE A is AN10
#define ADC_CH0_POS_SAMPLEA_AN9_     0xFFE9 // A/D CH0 pos i/p sel for SAMPLE A is AN5
#define ADC_CH0_POS_SAMPLEA_AN8_     0xFFE8 // A/D CH0 pos i/p sel for SAMPLE A is AN12
#define ADC_CH0_POS_SAMPLEA_AN7_     0xFFE7 // A/D CH0 pos i/p sel for SAMPLE A is AN11
#define ADC_CH0_POS_SAMPLEA_AN6_     0xFFE6 // A/D CH0 pos i/p sel for SAMPLE A is AN10
#define ADC_CH0_POS_SAMPLEA_AN5_     0xFFE5 // A/D CH0 pos i/p sel for SAMPLE A is AN5
#define ADC_CH0_POS_SAMPLEA_AN4_     0xFFE4 // A/D CH0 pos i/p sel for SAMPLE A is AN4
#define ADC_CH0_POS_SAMPLEA_AN3_     0xFFE3 // A/D CH0 pos i/p sel for SAMPLE A is AN3
#define ADC_CH0_POS_SAMPLEA_AN2_     0xFFE2 // A/D CH0 pos i/p sel for SAMPLE A is AN2
#define ADC_CH0_POS_SAMPLEA_AN1_     0xFFE1 // A/D CH0 pos i/p sel for SAMPLE A is AN1
#define ADC_CH0_POS_SAMPLEA_AN0_     0xFFE0 // A/D CH0 pos i/p sel for SAMPLE A is AN0
/*
//----------------------------------------------------------------------------------------------------------------------
ConfigIntADC10(ADC_INT_ENABLE & ADC_INT_PRI_7 );
*/

//void _ISR _ADC1Interrupt(void);

