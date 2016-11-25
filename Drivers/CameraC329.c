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

#include "CameraC329.h"

//-----------------------------------------------------------------------------
//
// Constants
//
//-----------------------------------------------------------------------------

static const unsigned char CAM_CMD_PREFIX = 0xFF;
static const unsigned char CAM_CMD_INITIAL = 0x01;
static const unsigned char CAM_CMD_GETPICTURE = 0x04;
static const unsigned char CAM_CMD_SNAPSHOT = 0x05;
static const unsigned char CAM_CMD_RESET = 0x08;
static const unsigned char CAM_CMD_POWEROFF = 0x09;
static const unsigned char CAM_CMD_DATA = 0x0A;
static const unsigned char CAM_CMD_SYNC = 0x0D;
static const unsigned char CAM_CMD_ACK = 0x0E;
static const unsigned char CAM_CMD_NAK = 0x0F;
static const unsigned char CAM_CMD_QUALITY = 0x10;

/**
 * Constructor
 */
//CameraC329(int _spi_cs_pin):spi_cs_pin(_spi_cs_pin)
//{
//}

/**
 * Synchronize with the camera. Synchronization will be attempted up to
 * MAX_SYNC_ATTEMPTS. You must synchronize with the camera before you can
 * call other methods.
 *
 * @return True if successful, FALSE otherwise.
 */
BOOL c329_sync()
{
  uint8_t syncAttempts = 0;
  BOOL success = FALSE;

  c329_setOutputCommand(CAM_CMD_SYNC, 0, 0, 0, 0);

  while (syncAttempts < CAM_MAX_SYNC_ATTEMPTS)
  {
    c329_sendCommand();
    c329_delay(25);

    // Wait for ACK response
    success = c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_SYNC);

    // Make sure it is an ACK
    if (success)
    {
      // Now wait for SYNC from camera
      success = c329_waitForResponse(CAM_RESPONSE_DELAY);
      if (success && c329_inputCommand[3] == CAM_CMD_SYNC)
      {
        // All is good, flush the buffer
//        cameraPort.flush();

        // Send ACK
        c329_setOutputCommand(CAM_CMD_ACK, CAM_CMD_SYNC, 0, 0, 0);
        c329_sendCommand();

        return TRUE;
      }
    }

    syncAttempts++;
  }

  return FALSE;
}

/**
 * Sets the camera's initial baud rate, color type, and image sizes. Call this
 * method after synchronization to set up appropriate parameters before
 * taking a snapshot.
 *
 * @param baudRate The baud rate to use for future camera communication
 * @param colorType The color type
 * @param previewResolution The preview resolution
 * @param jpegResolution The JPEG resolution
 *
 * @return True if successful, FALSE otherwise
 */
BOOL c329_initialize(BaudRate baudRate, ColorType colorType,
    PreviewResolution previewResolution, JPEGResolution jpegResolution)
{
  c329_setOutputCommand(CAM_CMD_INITIAL, baudRate, colorType, previewResolution,
    jpegResolution);
  c329_sendCommand();

  if (c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_INITIAL))
    return TRUE;

  return FALSE;
}

/**
 * Set the quality level for compressed (JPEG) images.
 *
 * @param qualityLevel The desired quality level. Should be a value from the
 * QualityLevel enumeration.
 */
BOOL c329_setQuality(QualityLevel qualityLevel)
{
  c329_setOutputCommand(CAM_CMD_QUALITY, qualityLevel, 0, 0, 0);
  c329_sendCommand();

  if (c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_QUALITY))
    return TRUE;

  return FALSE;
}

/**
 * Takes a snapshot with the camera and stores it in the camera's internal
 * data buffer. Once a snapshot has been taken, use getPicture to retrieve
 * the photo data.
 *
 * @param pictureType The picture type to take. Should be a value from the
 * PictureType enumeration.
 *
 * @return True if successful, FALSE otherwise
 */
BOOL c329_takeSnapshot(PictureType pictureType)
{
  c329_setOutputCommand(CAM_CMD_SNAPSHOT, pictureType, 0, 0, 0);
  c329_sendCommand();

  if (c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_SNAPSHOT))
    return TRUE;

  return FALSE;
}

BOOL c329_getPicture(PictureType pictureType, void (*callback)(uint32_t pictureSize, uint16_t packetSize, uint32_t packetStartPosition, unsigned char* packet))
{
  uint32_t pictureSize = 0;

  c329_reset(RT_STATE);

//  while (!waitForACK(RESPONSE_DELAY, CAM_CMD_GETPICTURE) && ack_counter < 100) {
//    cam_delay(10);
//  }

  uint32_t get_pic_tries = 0;
  while(get_pic_tries++ < 10) {
    c329_setOutputCommand(CAM_CMD_GETPICTURE, pictureType, 0, 0, 0);
    c329_sendCommand();

    uint32_t max_tries = 0, total_tries = 10;
    while (!(c329_waitForResponse(CAM_RESPONSE_DELAY) && c329_inputCommand[3] == CAM_CMD_DATA) && max_tries++ < total_tries) {
      printf("not DATA (%d)\n", c329_inputCommand[3]);
      c329_delay(10);
    }
    
    if(max_tries < total_tries) break;
  }
  if(get_pic_tries >= 10) return FALSE;


  pictureSize = c329_inputCommand[7] << 8;
  pictureSize |= c329_inputCommand[6] << 8;
  pictureSize |= c329_inputCommand[5];

  uint32_t bytePosition = 0;
  uint8_t package[CAM_DEFAULT_PACKAGE_SIZE];

  while (bytePosition < pictureSize)
  {
    if (!c329_waitForResponse2(CAM_RESPONSE_DELAY, package, CAM_DEFAULT_PACKAGE_SIZE))
      return FALSE;

    callback(pictureSize, min(CAM_DEFAULT_PACKAGE_SIZE, pictureSize - bytePosition), bytePosition, package);
    bytePosition += CAM_DEFAULT_PACKAGE_SIZE;
  }

  return TRUE;
}

/**
 * Resets the camera.
 *
 * @param resetType The type of reset to perform (ether a "soft" reset which
 * resets only the camera's internal state machine) or a "hard" reset.
 *
 * @return True if successful, FALSE otherwise.
 */
BOOL c329_reset(ResetType resetType)
{
  c329_setOutputCommand(CAM_CMD_RESET, resetType, 0, 0, 0xFF);
  c329_sendCommand();

  if (c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_RESET))
    return TRUE;

  return FALSE;
}

/**
 * Power off the camera. The camera will be unusable after calling this method
 * until a successful re-synchronization.
 *
 * @return True if successful, FALSE otherwise
 */
BOOL c329_powerOff()
{
  c329_setOutputCommand(CAM_CMD_POWEROFF, 0, 0, 0, 0);
  c329_sendCommand();

  if (c329_waitForACK(CAM_RESPONSE_DELAY, CAM_CMD_POWEROFF))
    return TRUE;

  return FALSE;
}

/**
 * @private
 *
 * Sends the command that is in the outputCommand buffer. Command bytes are
 * sent in big endian order.
 */
void c329_sendCommand()
{
  uint8_t i;
//printf("send command ");
  // Big endian
  for (i = 0; i < CAM_CMD_SIZE; i++)
  {
//printf(outputCommand[i]); printf(" ");
//    cameraPort.print(outputCommand[i], BYTE);
    //cameraPort.write((uint8_t)outputCommand[i]);
    SPI_nSS_1 = 0;
    SPI_1_transfer(c329_outputCommand[i]);
    SPI_nSS_1 = 1;
  }
//printfln();
}

BOOL c329_waitForACK(uint32_t timeout, uint8_t cmd)
{
  BOOL success = c329_waitForResponse(timeout);

  if (success && c329_inputCommand[3] == CAM_CMD_ACK && c329_inputCommand[4] == cmd)
    return TRUE;

  return FALSE;
}

/**
 * @private
 *
 * Waits for a response up to timeout, and stores the response in the
 * inputCommand array.
 *
 * @return True if response is received before the timeout, FALSE otherwise.
 */
BOOL c329_waitForResponse(uint32_t timeout)
{
  return c329_waitForResponse2(timeout, c329_inputCommand, CAM_CMD_SIZE);
}

/**
 * @private
 *
 * Waits for a response up to timeout, and stores the response in the buffer
 * array provided.
 *
 * @param timeout The timeout
 * @param buffer The buffer within which to store the response
 * @param bufferLength The length of the provided buffer
 *
 * @return True if response is received before timeout, FALSE otherwise
 */
BOOL c329_waitForResponse2(uint32_t timeout, unsigned char buffer[], uint16_t bufferLength)
{
  uint8_t byteCount = 0;
  unsigned long time = xTaskGetTickCount();

//printf("response: ");
  while (xTaskGetTickCount() - time <= timeout)
  {
//    while (cameraPort.available() > 0)
    {
      SPI_nSS_1 = 0;
      buffer[byteCount] = SPI_1_transfer(0x00); //cameraPort.read();
      SPI_nSS_1 = 1;
      
//printf(buffer[byteCount],DEC); printf(" ");
      byteCount++;

      if (byteCount == bufferLength) {
//printfln();
        return TRUE;
      }
    }
  }
//printfln();
  if (byteCount > 0)
    return TRUE;

  return FALSE;
}

/**
 * @private
 *
 * Utility method for populating the output command buffer.
 */
void c329_setOutputCommand(const unsigned char command, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4)
{
  c329_outputCommand[0] = CAM_CMD_PREFIX;
  c329_outputCommand[1] = CAM_CMD_PREFIX;
  c329_outputCommand[2] = CAM_CMD_PREFIX;
  c329_outputCommand[3] = command;
  c329_outputCommand[4] = param1;
  c329_outputCommand[5] = param2;
  c329_outputCommand[6] = param3;
  c329_outputCommand[7] = param4;
}

void c329_delay(int delay)
{
    unsigned int i,j,h=0;
    unsigned int delay2=0xFFFF;//(unsigned int)delay<<4;
    for(i=0;i<delay2;i++)
    {
        h=i*1;
    }
    j=h;
}