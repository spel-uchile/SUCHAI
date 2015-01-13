/*========================================================================================
Description		:Function set for the camera (header)
File			:camera.h
Created by 		:Alex Becerra
Project			:SUCHAI
Creation date	:16/01/2012

Note(1)			:Based in the user manual of Electronics123's C329 SPI camera (SPI)
Note(2)			:Made for the port SPI1
Note(3)			:Made for PIC24FJ96GA010 (Mikroelektronika's development board)

Revision history
v1.00 (16/01/2012)	:Initial release
v2.01 (01/06/2012)	:Updated from camera.c file
					 Port definition included
v2.02 (15/06/2012)	:Minor changes.
==========================================================================================*/
#include "DebugIncludes.h"  //para con_printf

#include "interfaz_SPI.h"   //SPI_nSS_1
#include "pic_pc104_config.h"   //PPC_CAM_HOLD_CHECK

#define CAM_MODE_VERBOSE    0
#define CAM_MODE_SAVE_SD    1
#define CAM_MODE_BOTH       2

void cam_delay_ms(int delay);
void cam_button_config(void);

// Camera functions
unsigned int cam_take_photo(void);
int cam_poweroff(void);
int cam_reset(int res_type);
int sync(BOOL verb);
int cam_sync(BOOL verb);
int cam_send_comm(unsigned char* cmd, int arg1, int arg2, int arg3, int arg4);
unsigned int cam_photo(int resolution, int qual, int pic_type);
void cam_recev_photo(unsigned int length);
unsigned int cam_receivePhoto(unsigned int length, int mode);
void cam_wait_hold_wtimeout(BOOL verb);
int cam_isAlive(void);

int send_comm(unsigned char* cmd, int arg1, int arg2, int arg3, int arg4);

