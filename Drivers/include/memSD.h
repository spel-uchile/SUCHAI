#ifndef _MEM_SD_
#define _MEM_SD_


#include <spi.h>                    //uso ReadSPI2(), SPI2_Rx_Buf_Full
#include "DebugIncludes.h"          //para con_printf
#include "pic_pc104_config.h"       //para usar SPI_nSS_x


void msd_blockErase(unsigned long block_address);

BOOL msd_setVar_1BlockExtMem(unsigned long block, unsigned char indx, int value);
BOOL msd_getVar_1BlockExtMem(unsigned long block, unsigned char indx, int *value);
BOOL msd_setVar_256BlockExtMem(unsigned long block_i, unsigned int indx, int value);
BOOL msd_getVar_256BlockExtMem(unsigned long block_i, unsigned int indx, int *value);

//******************************************************************************************************
#define MSD_readSPI() SPI_2_transfer(0xFF)
#define MSD_clockSPI() SPI_2_transfer(0xFF)

#define MSD_mem_ReadPage(j, buff, i); SPI_2_transfer(0xFF)
#define MSD_mem_WritePage(j, buff, i); SPI_2_transfer(0xFF)

#define MSD_disableSD() SPI_nSS_2=1; MSD_clockSPI()
#define MSD_enableSD() SPI_nSS_2=0
//----------------------------------------------------------------------------------------------
//#define nSS _LATG9					// ~SS: slave select
//#define nSDCD _RE7					// ~SDCD: presencia de la tarjeta SD
//#define LED_R _LATC3				// led indicador
//#define LED_W _LATC4				// led indicador
//-----------------------------------------------------------------------------------------------

#define MSD_START_BLOCK_TOKEN	0xFE	// token response to CMD17.
#define MSD_DATA_ACCEPTED_TOKEN	0x05	// token send when a write operation is successful
#define MSD_COMMAND_ACCEPTED   	0x00	// Previouse command was accpeted (only valid for R1 response format).
#define MSD_VOLTAGE_OK		0x000001AA	// Respuesta R7 que indica que la SD soporta el voltaje del host
//-----------------------------------------------------------------------------------------------
typedef struct{
	unsigned char mid;	//numero identificador del fabricante (8-bits)
	char oid[3];		//(2-character String) parce que identifica el proposito del contenido de la tarjeta SD
	char pnm[6];		//product name (5-character String)
	float prv;			//product revision (version "x.y")
	unsigned long psn;	//product serial number
	int month;			//month of manufacturing
	int year;			//year of manufacturing
}MSD_CARD_ID;

typedef unsigned long MSD_ARG;
typedef struct{
	unsigned char command;          // 1 byte command
	MSD_ARG argument;		// 4 bytes argument
	unsigned char crc;		// 1 byte crc (Cyclic Redundancy Check)
}MSD_COMMAND;				//si no ocupo typedef deberia hacer "struct x nombre;" en vez de "COMMAND nombre;"
                                        //COMMAND cmd1; cmd1.command=c|0x40; cmd1.ADDR=0; cmd1.crc=0x95;
MSD_COMMAND create_COMMAND(unsigned char c, MSD_ARG a,unsigned char e);
//-----------------------------------------------------------------------------------------------------------------------------
//funciones internas
unsigned char get_R1_response(void);
unsigned char get_R1b_response(void);
unsigned int get_R2_response(void);

void send_COMMAND(MSD_COMMAND CMDx);	//envia comandos a la tarjeta SD
//-----------------------------------------------------------------------------------------------------------------------------
int SD_presence(void);		//devuelve 1 si la SD esta en el host, 0 si no esta.

unsigned char Set_Block_Length(MSD_ARG l);	//largo de los block para leer y escribir la SD. lmax =512bytes, lmin=1byte
                                                // Si tiene exito devuelve COMMAND_ACCEPTED (es un define)
                                                //si falla devuelve R1 (es una variable)

unsigned char SD_init2(void);		//Inicializacion bajo estandar 2.0 de Part I, Physical layer
                                        //si tiene exito devuelve COMMAND_ACCEPTED (es un define)
                                        //si falla devuelve el byte R1 response (es una variable)

/**
 * Init memSD and return status
 * @return 1=init Ok 0=init Failed
 */
unsigned int SD_init_memSD(void);

unsigned char SD_init(void);	//inicializacion alternativa para modo SPI de la tarjeta SD
                                //si tiene exito devuelve COMMAND_ACCEPTED (es un define)
                                //si falla devuelve el byte R1 response (es una variable)
//-----------------------------------------------------------------------------------------------------------------------------
//Operaciones de lectura escritura de la SD por block (512bytes)
unsigned char Single_Block_Read(MSD_ARG addr, unsigned char* buff);     //lee un block(512 bytes) y lo guarda en buff[]
                                                                        //si tiene exito devuelve START_BLOCK_TOKEN (es un define)
                                                                        //si falla devuelve R1 o DATA_ERROR_TOKEN (es una variable)
                                                                        //si dir de mem a leer es invalida
unsigned char get_CMD17_Response_Token(void);		//luego de tener R1=COMMAND_ACCEPPTED y
                                                        //antes de que el host lea un bloque, la SD envia el byte:
                                                        //Start Block Token si enviará datos, o Data Error Token si no los enviará
													
unsigned char Single_Block_Write(MSD_ARG addr, unsigned char* buff);	//graba un block(512 bytes) y lo guarda en buff[]
                                                                        //si tiene exito devuelve DATA_ACCEPTED_TOKEN (es un define)
                                                                        //si falla devuelve R1 o DATA_RESPONSE_TOKEN (es una variable)
unsigned char get_DRToken(void);		//luego de tener R1=COMMAND_ACCEPPTED y grabar un bloque
                                                //la SD responde con el byte: Data Response Token
//-----------------------------------------------------------------------------------------------------------------------------
// Hay 4 registros accesibles en modo SPI: SCR (ACMD51), OCR (CMD58), CID (CMD10), CSD (CMD9)
MSD_CARD_ID CID_register(void);		//Obtiene el registro CID (card identification).
                                        //Si tiene exito devuelve un type CARD_ID,que es una estructura en que cada campo es
                                        //algun type (String, int, float, unsigned long) con informacion de la tarjeta SD.
                                        //Si falla, no retorna R1 y la estructura contendra solo basura.

// 512bits CSD_register(void); demasiado pajero de implementar!!!!
// 64bits SCR_register(void); no tan pajero, pero igual que paja!!!!

MSD_ARG OCR_register(void);		// reads the Operating Condition Register (OCR). En una respuesta R3
                                        //Si tiene exito, devuelve 4bytes (type ARG) con la informacion de OCR
                                        //Si falla (error de comunicacion) devuelve R1 (queda en el byte inferior del return ARG)
//-----------------------------------------------------------------------------------------------------------------------------
//respuestas R2 (CMD13) y R7 (CMD8)
unsigned int STATUS_register(void);	//Envia CMD13 y recive la respuesta R2
                                        //Si tiene exito devuelve 16 bits: [15:8]=R1 y [7:0]=info extra
                                        //Si falla devuelve 16 bits: [7:0]=R1 y [15:8]=0x00
int memSD_isAlive(void);
MSD_ARG send_CMD8(void);        //Send CMD8 and get the rseposne R7
                                //indica los voltajes aceptados en la rutina de inicializacion de ls SD
                                // Si tiene exito devuelve VOLTAGE_OK (es un define)
                                //Si falla devuelve Un tipo ARG con un unsigned char R1 en su byte inferior
//-----------------------------------------------------------------------------------------------------------------------------

/*
------------------------------------------------------------------------------------------------
Type								Bits					Min					Max 
------------------------------------------------------------------------------------------------
char						 		 8 						-128				127 
unsigned char  						 8						 0					255 
short								 16  					-32768 				32767 
unsigned short  					 16  					 0 					65535 
int									 16 					-32768 				32767 
unsigned int  						 16 					 0					65535 
long 			 			 		 32						-2^31 				2^31 - 1 
unsigned long  						 32						 0  				2^32 - 1 
long long**						  	 64						-2^63  				2^63 - 1 
unsigned long long**  				 64 					 0					2^64 - 1 
------------------------------------------------------------------------------------------------
*/

#endif

