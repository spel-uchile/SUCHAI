/**
 * Copyright 2011
 * Sean Voisen <http://voisen.org>
 *
 * SPI port by Roy Shilkrot 2013 <http://www.morethantechnical.com>
 *
 * Based on the original library for the now-obsolete C328R camera, also by
 * Sean Voisen in collaboration with Beatriz da Costa 
 * <http://beatrizdacosta.net>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CAMERAC329_H_
#define CAMERAC329_H_

//#include "WProgram.h"
//#include "WConstants.h"
//#include <inttypes.h>
//#include <NewSoftSerial.h>
#include <spi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "interfaz_SPI.h"
#include "FreeRTOS.h"
#include "task.h"

// The byte size of UART commands
#define CAM_CMD_SIZE 8

// Number of sync attempts to try before giving up
#define CAM_MAX_SYNC_ATTEMPTS 100

// How long to wait for serial communication responses
#define CAM_RESPONSE_DELAY 200

// Size (in bytes) to split up picture data into packages for processing
#define CAM_DEFAULT_PACKAGE_SIZE 64

//class CameraC329
//{
//  public:
typedef enum
{
  BAUD7200 = 0x08,
  BAUD14400 = 0x07,
  BAUD28800 = 0x06,
  BAUD57600 = 0x05,
  BAUD115200 = 0x04,
  BAUD230400 = 0x03,
  BAUD460800 = 0x02,
  BAUD921600 = 0x01
}BaudRate;

typedef enum
{
  CT_Y4 = 0x02,
  CT_Y8 = 0x03,
  CT_RGB12 = 0x05,
  CT_RGB16 = 0x06,
  CT_RGB24 = 0x08,
  CT_YUV16 = 0x09,
  CT_JPEG = 0x07
} ColorType;

typedef enum
{
  PR_80x60 = 0x01,
  PR_88x72 = 0x02,
  PR_160x120 = 0x03,
  PR_176x144 = 0x04,
  PR_320x240 = 0x05,
  PR_352x288 = 0x06,
  PR_640x480 = 0x07,
  PR_80x64 = 0x08,
  PR_128x96 = 0x09,
  PR_128x128 = 0x0A,
  PR_160x128 = 0x0B
} PreviewResolution;

typedef enum 
{
  JR_80x60 = 0x01,
  JR_88x72 = 0x02,
  JR_160x120 = 0x03,
  JR_176x144 = 0x04,
  JR_320x240 = 0x05,
  JR_352x288 = 0x06,
  JR_640x480 = 0x07,
  JR_80x64 = 0x08,
  JR_128x96 = 0x09,
  JR_128x128 = 0x0A,
  JR_160x128 = 0x0B
}JPEGResolution;

typedef enum 
{
  PT_SNAPSHOT = 0x01,
  PT_PREVIEW = 0x02,
  PT_SERIAL_FLASH = 0x03,
  PT_JPEG_PREVIEW = 0x05,
  PT_PLAYBACK = 0x06
}PictureType;

typedef enum
{
  DT_REGISTER = 0x00,
  DT_SNAPSHOT = 0x01,
  DT_PREVIEW = 0x02,
  DT_JPEG_PREVIEW = 0x04,
  DT_PLAYBACK = 0x05
}DataType;

typedef enum 
{
  RT_ALL = 0x00,
  RT_STATE = 0x01
}ResetType;

typedef enum 
{
  QL_BEST = 0x00,
  QL_BETTER = 0x01,
  QL_NORMAL = 0x02
}QualityLevel;

void c329_CameraC329(int _spi_cs_pin);
BOOL c329_sync(void);
BOOL c329_initialize(BaudRate, ColorType, PreviewResolution, JPEGResolution);
BOOL c329_setQuality(QualityLevel);
BOOL c329_takeSnapshot(PictureType);
BOOL c329_getPicture(PictureType, void (*)(uint32_t, uint16_t, uint32_t, unsigned char*));
BOOL c329_reset(ResetType);
BOOL c329_powerOff();

//  private:
unsigned char c329_outputCommand[CAM_CMD_SIZE];
unsigned char c329_inputCommand[CAM_CMD_SIZE];
void c329_sendCommand();
BOOL c329_waitForACK(uint32_t, uint8_t);
BOOL c329_waitForResponse(uint32_t);
BOOL c329_waitForResponse2(uint32_t, unsigned char[], uint16_t);
void c329_setOutputCommand(const unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);

int spi_cs_pin;
//};


void c329_delay(int delay);

#endif // CAMERAC329_H_
