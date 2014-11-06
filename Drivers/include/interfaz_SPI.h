/******************************************************************************
*
*                  LIBRERIA PARA LA INTERFAZ DEL PERIFERICO SPI
*
******************************************************************************
* FileName:        interfaz_SPI.c
* Dependencies:    p24FJ256GA110.h and spi.h
* Processor:       PIC24FJ256GA110
* Compiler:        MPLAB C30
* Autor:	       Tomas Opazo T.
*/
#ifndef _INTERFAZ_SPI_
#define _INTERFAZ_SPI_

#include <spi.h>

//Pines para SPI_1 configurado segun: Peripheral_init() en pic_pc104_config.c
#define SPI_SDI_1 _RD10		//SDO en nomenclatura Pumpkin's
#define SPI_SCK_1 _RD11		//SCK en nomenclatura Pumpkin's
#define SPI_SDO_1 _RD0		//SDI en nomenclatura Pumpkin's
#if (SCH_PAYCAM_nMEMFLASH_ONBOARD==1)
    #define SPI_nSS_1 _LATG9	//nCS para la  Camara
#elif (SCH_PAYCAM_nMEMFLASH_ONBOARD==0)
    #define SPI_nSS_1 _LATD13	//nCS en nomenclatura Pumpkin's
#endif
#define SPI_nWP_1 _LATD6	//nWP en nomenclatura Pumpkin's

//Pines para SPI_2 configurado segun: Peripheral_init() en pic_pc104_config.c
#define SPI_SDI_2 _RF7  //Para la memSD
#define SPI_SCK_2 _RD9
#define SPI_SDO_2 _RF8
#define SPI_nSS_2 _LATE5

//Pines para SPI_3 configurado segun: Peripheral_init() en pic_pc104_config.c
#define SPI_SDI_3 0 //no existe, no usado
#define SPI_SCK_3 _RB9
#define SPI_SDO_3 _RB8
#define SPI_nSS_3 _LATB12

/*
SPI es un tipo de comunicacion, en donde el objetivo es el intercambio de registros entre el Master y el Slave.
En modo normal, el Master selecciona un esclavo y luego intercambia un registro, habilitando la señal clock.
Luego des-selecciona al esclavo y deja de transmitir la seañal clock.

En el PIC24FJ:
El modulo SPI posee 8 modos de operacion, al combinar los sgtes:
8-17 BIT MODE						=> casi siempre se usa 8bit
MASTER-SLAVE MODE					=> el PIC24 siempre será master.
ENHANCED BUFFER-NORMAL BUFFER MODE	=> enhanced mode(activa un buffer de salida y entrada) NO IMPLEMENTADO. 
FRAMEED SPI-NORMAL MODE				=> enhanced mode NO IMPLEMENTADO. Tarjetas SD y la camara C329 usan normal mode
//-------------------------------------------------------------------------------------------------------------------------------
Para congifurar y habilitar el SPIx, se debe usar OpenSPIx(unsigned int spi1con1, unsigned int spi1con2, unsigned int spi1stat); 
Donde spi1con1=SPIxCON1 REGISTER, spi1con2=SPIxCON2 REGISTER y spi1stat=SPIxSTAT REGISTER

SPIxCON1 REGISTER:
DISABLE_SCK_PIN		Internal SPI clock is diabled, pin functions as I/O
ENABLE_SCK_PIN      Internal SPI clock is enabled

DISABLE_SDO_PIN		  SDO pin is not used by module (PIC24FJ solo escucha)
ENABLE_SDO_PIN        SDO pin is  used by module (transmite y escucha)

SPI_MODE16_ON         Communication is word wide (16 bit)
SPI_MODE16_OFF        Communication is byte wide  (8 bit)

SPI_SMP_ON            Input data sampled at end of data output time
SPI_SMP_OFF           Input data sampled at middle of data output time

SPI_CKE_ON            Transmite un clock antes del 1er edge. Samplea en el 1er edge
SPI_CKE_OFF           Transmite en el  1er edge. Samplea en el 2do edge

SLAVE_ENABLE_ON       Slave Select enbale
SLAVE_ENABLE_OFF      Slave Select not used by module----->tienen que elegir esta y luego configurar nSS como un pin I/O en el PPS_init

CLK_POL_ACTIVE_LOW    En idle (SPI no transmite) clock esta en high=1.
CLK_POL_ACTIVE_HIGH   En idle (SPI no transmite) clock esta en low=0.

MASTER_ENABLE_ON	  Master Mode
MASTER_ENABLE_OFF     Slave Mode

SEC_PRESCAL_1_1       Secondary Prescale 1:1----->F_SCK=Fcy/(PRI_PRESCAL*SEC_PRESCAL)
SEC_PRESCAL_2_1       Secondary Prescale 2:1
SEC_PRESCAL_3_1       Secondary Prescale 3:1
SEC_PRESCAL_4_1       Secondary Prescale 4:1
SEC_PRESCAL_5_1       Secondary Prescale 5:1
SEC_PRESCAL_6_1       Secondary Prescale 6:1
SEC_PRESCAL_7_1       Secondary Prescale 7:1
SEC_PRESCAL_8_1       Secondary Prescale 8:1

PRI_PRESCAL_1_1       Primary Prescale 1:1
PRI_PRESCAL_4_1       Primary Prescale 4:1
PRI_PRESCAL_16_1      Primary Prescale 16:1
PRI_PRESCAL_64_1      Primary Prescale 64:1

SPIXCON2 REGISTER:
FRAME_ENABLE_ON        		Frame SPI support enable
FRAME_ENABLE_OFF       		Frame SPI support Disable----->tienen que elegir esta

FRAME_SYNC_INPUT       		Frame sync pulse Input (slave)
FRAME_SYNC_OUTPUT     		Frame sync pulse Output (master)

FRAME_POL_ACTIVE_HIGH  		Frame sync pulse is active-high
FRAME_POL_ACTIVE_LOW   		Frame sync pulse is active-low

FRAME_SYNC_EDGE_COINCIDE 	Frame sync pulse coincides with first bit clock
FRAME_SYNC_EDGE_PRECEDE  	Frame sync pulse precedes first bit clock

FIFO_BUFFER_ENABLE  		FIFO buffer enabled
FIFO_BUFFER_DISABLE			FIFO buffer enabled----->tienen que elegir esta

SPIxSTAT REGISTER:
SPI_ENABLE             Enable module
SPI_DISABLE            Disable module

SPI_IDLE_CON           Continue module operation in idle mode
SPI_IDLE_STOP          Discontinue module operation in idle mode
//-------------------------------------------------------------------------------------------------------------------------------
Para configurar la ISR del SPIx se debe configurar:
ConfigIntSPI2( register );
SPI_INT_EN             SPI Interrupt Enable
SPI_INT_DIS            SPI Interrupt Disable

SPI_INT_PRI_0          SPI Interrupt Prior Level_0
SPI_INT_PRI_1          SPI Interrupt Prior Level_1
SPI_INT_PRI_2          SPI Interrupt Prior Level_2
SPI_INT_PRI_3          SPI Interrupt Prior Level_3
SPI_INT_PRI_4          SPI Interrupt Prior Level_4
SPI_INT_PRI_5          SPI Interrupt Prior Level_5
SPI_INT_PRI_6          SPI Interrupt Prior Level_6
SPI_INT_PRI_7          SPI Interrupt Prior Level_7

ISR de _SPIxIF (x=1,2 o 3)
void __attribute__((__interrupt__)) _SPI2Interrupt(void){
	....
	..rutina..
	....
    SPI2_Clear_Intr_Status_Bit;	//_SPI2IF
}
*/
//-------------------------------------------------------------------------------------------------------------------------------
int overflow_SPI_1(void);						//devuelve 1 si hubo overflow, 0 si no.
void clear_overflow_SPI_1(void);				//borra el bit SPIROV para proximos seteos (cuando haya overflow nuevamente)
int TXB_SPI_1(void);							//1=>TXB is full, transmit not yet started. 0=>TXB is empty, transmit started
int RXB_SPI_1(void);							//1=>RXB is full, receive complete. 0=>RXB is empty, receive  is not complete
unsigned char SPI_1_transfer(unsigned char a);	//transmite "a" y devuelve el registro leido
void Open_SPI_1_default(void);					//Abre SPI1 con la configuracion "mas estandar"
//-------------------------------------------------------------------------------------------------------------------------------
int overflow_SPI_2(void);						//devuelve 1 si hubo overflow, 0 si no.
void clear_overflow_SPI_2(void);
int TXB_SPI_2(void);							//1=>TXB is full, transmit not yet started. 0=>TXB is empty, transmit started
int RXB_SPI_2(void);							//1=>RXB is full, receive complete. 0=>RXB is empty, receive  is not complete
unsigned char SPI_2_transfer(unsigned char a);	//transmite "a" y devuelve el registro leido
void Open_SPI_2_default(void);					//Abre SPI1 con la configuracion "mas estandar"
//-------------------------------------------------------------------------------------------------------------------------------
int overflow_SPI_3(void);						//devuelve 1 si hubo overflow, 0 si no.
void clear_overflow_SPI_3(void);
int TXB_SPI_3(void);							//1=>TXB is full, transmit not yet started. 0=>TXB is empty, transmit started
int RXB_SPI_3(void);							//1=>RXB is full, receive complete. 0=>RXB is empty, receive  is not complete
unsigned char SPI_3_transfer(unsigned char a);	//transmite "a" y devuelve el registro leido
void Open_SPI_3_default(void);					//Abre SPI1 con la configuracion "mas estandar"
//-------------------------------------------------------------------------------------------------------------------------------

#endif
