/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *      Copyright 2013, Alex Becerra
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

/*========================================================================================
Description		:Function set for the camera
File			:camera.c
Created by 		:Alex Becerra
Project			:SUCHAI
Creation date	:16/01/2012

Note(1)			:Based in the user manual of Electronics123's C329 SPI camera 
Note(2)			:Made for the port SPI1
Note(3)			:Made for PIC24FJ96GA010 (Mikroelektronika's development board)
==========================================================================================*/

#include "camera.h"

// Avalaible commands for C329 camera (For more information see the C329's User Manual)
unsigned char CAM_INITIAL[]     ={0xff, 0xff, 0xff, 0x01};
// [5] FIXED. [6],[7] for resolution
unsigned char CAM_GETPIC[]      ={0xff, 0xff, 0xff, 0x04};
// [4] for picture type
unsigned char CAM_SNAPSHOT[]    ={0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0x00};
unsigned char CAM_RESET[]       ={0xff, 0xff, 0xff, 0x08};
// [7] for reset type
unsigned char CAM_POWEROFF[]    ={0xff, 0xff, 0xff, 0x09};
unsigned char CAM_SYNC[]        ={0xff, 0xff, 0xff, 0x0d, 0x00, 0x00, 0x00, 0x00};
unsigned char CAM_ACK[]         ={0xff, 0xff, 0xff, 0x0e, 0x0d, 0x00, 0x00, 0x00};
unsigned char CAM_QUALITY[]     ={0xff, 0xff, 0xff, 0x10};
// [4] for quality
unsigned char CAM_DUMMY[]       ={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


// ============================================================================================================
// Puts the camera in power down mode (For more information see the C329 UM) 
// Arguments	:None
// Return		:int succes (0 for succes, 1 for fail)
// NOTE			:[[NOT FUNCTIONAL]] (Camera doesn't enter power mode despite an ACK comm is received for this feature)
// ============================================================================================================

int poweroff(void){
    int success = 1;					// Fail as default
    int i;

    for(i=0; i<5; i++){					// Maximum retry: 5
        success = send_comm(CAM_POWEROFF, 0x00, 0x00, 0x00, 0x00);
        if (!success) {break;}
    }

    return success;
}

// ============================================================================================================
// Resets the camera (For more information see the C329 UM) 
// Arguments	: int res_type (0x00 for reset all, 0x01 for state machine reset)
// Return		: 0 for success, 1 for fail.
// NOTE			: [[NOT TESTED]]
// ============================================================================================================
int reset(int res_type){

    int success = 1;					// Fail as default
    int i;

    for(i=0; i<5; i++){					// Maximum retries: 5
            success = send_comm(CAM_RESET, 0x00, 0x00, 0x00, res_type);
            if (!success) {break;}
    }

    return success;
}


// ============================================================================================================
// Synchronization of the camera with the master MCU (For more information see the C329's UM)
// Arguments	: None
// Return		: int rsync (0 for successfull sync, 1 for failed sync)
// ============================================================================================================
int sync(BOOL verb){

	unsigned int i,j,k;					// Variables for indexes
	unsigned char recev[8];					// Received data temp buffer
	unsigned int rsync = 1;				// Rsync status flag 1 (Fail by default)

	for(j=0; j<=60; j++){					// Max 60 retries

            SPI_nSS_1 = 0;        						// Select the camera
            for(i=0; i<8; i++){
                    recev[i] = SPI_1_transfer(CAM_SYNC[i]); 	// Write SYNC command
            }
            SPI_nSS_1 = 1;						// Deselect the camera
            __delay_ms(12);					// Delay of 12 us (Camera requirement)


            cam_wait_hold_wtimeout(verb);
            // Wait for the camera to be ready

            if((recev[2]==0xff) && (recev[3]==0x0e) && (recev[4]==0x0d)){ break;}	// If data received is ACK, break if cycle (*)
	}

	while(PPC_CAM_HOLD_CHECK){;}				// Wait for HOLD low signal
	if((recev[2]==0xff) && (recev[3]==0x0e) && (recev[4]==0x0d)){		// If data received is sync, continue. (Same procedure as *)
																		// (We need to check if the exit was due to a correct ACK data)

        SPI_nSS_1 = 0;						// Select the camera
        for(i=0; i<8; i++){
                recev[i] = SPI_1_transfer(CAM_DUMMY[i]); 	// Write DUMMY data, read data
        }
        SPI_nSS_1 = 1;						// Deselect camera

        __delay_ms(12);					// Delay of 12 us (Camera requirement)

		while(PPC_CAM_HOLD_CHECK){;}                 	// Wait for HOLD low signal

        if ((recev[2]==0xff) && (recev[3]==0x0d) && (recev[4]==0x00)){		// If received comm is a SYNC, continue
                SPI_nSS_1 = 0;														// Select the camera
                for(k=0; k<8; k++){
                        recev[i] = SPI_1_transfer(CAM_ACK[i]); 	// Write an ACK
                }
                SPI_nSS_1 = 1;														// Deselect the camera
                __delay_ms(12);												// Delay of 12 us (Camera requirement)

				rsync = 0;													// Rsync status flag 0 (Successful)

				while(PPC_CAM_HOLD_CHECK){;}                                      		// Wait for HOLD low signal

       } // If (ACK)
	}// If SYNC

	return rsync;															// Return the status flag
}


int cam_sync(BOOL verb){
    //printf("cam_sync..\r\n");
    
    unsigned int i,j,k;		// Variables for indexes
    unsigned char recev[8];	// Received data temp buffer
    unsigned int rsync = 1;	// Rsync status flag 1 (Fail by default)

    for(j=0; j<60; j++){	// Max 60 retries
        printf("try = %d \r\n", j);

        SPI_nSS_1=0;
        //__delay_ms(30); // Camera requirement
        for(i=0; i<8; i++){
            recev[i] = SPI_1_transfer(CAM_SYNC[i]);     // Write SYNC command
        }
        SPI_nSS_1=1;
        //__delay_ms(30); // Camera requirement

        cam_wait_hold_wtimeout(verb);
        // Wait for PPC_CAM_HOLD_CHECK low signal

        if((recev[2]==0xff) && (recev[3]==0x0e) && (recev[4]==0x0d)){ printf("received ACK \r\n"); break;}
        // If data received is ACK, break if cycle (*)
    }

    cam_wait_hold_wtimeout(verb);
    // Wait for PPC_CAM_HOLD_CHECK low signal

    if((recev[2]==0xff) && (recev[3]==0x0e) && (recev[4]==0x0d)){
        // If data received is sync, continue. (Same procedure as *)
        SPI_nSS_1 = 0;
        // Select the camera
        for(i=0; i<8; i++){
                recev[i] = SPI_1_transfer(CAM_DUMMY[i]);
                // Write DUMMY data, read data
        }
        SPI_nSS_1 = 1;

        __delay_ms(12);
        // Delay of 12 us (Camera requirement)

        cam_wait_hold_wtimeout(verb);
        // Wait for PPC_CAM_HOLD_CHECK low signal

        if ((recev[2]==0xff) && (recev[3]==0x0d) && (recev[4]==0x00)){
            // If received comm is a SYNC, continue
            SPI_nSS_1 = 0;
            for(k=0; k<8; k++){
                    recev[i] = SPI_1_transfer(CAM_ACK[i]);
                    // Write an ACK
            }
            SPI_nSS_1 = 1;
            __delay_ms(12);
            // Delay of 12 us (Camera requirement)

            rsync = 0;
            // Rsync status flag 0 (Successful)

            cam_wait_hold_wtimeout(verb);
            // Wait for PPC_CAM_HOLD_CHECK low signal

        } // If (ACK)
    }// If SYNC

    return rsync;															// Return the status flag
}

// ============================================================================================================
// Function to send a generic command
// Arguments	:char cmd (desired commands), int arg1, arg2, arg3, arg4 (arguments - max 4 - of each function)
// Return		:int rsend (0 if successfull [ACK received], 1 if doesn't)
// ============================================================================================================
int send_comm(unsigned char* cmd, int arg1, int arg2, int arg3, int arg4){
    unsigned char recev[8];
    unsigned char comm_renew[8] = {cmd[0], cmd[1], cmd[2], cmd[3], arg1, arg2, arg3, arg4};
    unsigned int i;
    int rsend = 1;

    SPI_nSS_1=0;
    for(i=0; i<8; i++){
            recev[i] = SPI_1_transfer(comm_renew[i]);
            // Send the desired command
    }
    SPI_nSS_1=1;
    __delay_ms(12);
    // Delay of 12 us (Camera requirement)

    while(PPC_CAM_HOLD_CHECK){;}

    SPI_nSS_1=0;
    for(i=0; i<8; i++){
        recev[i] = SPI_1_transfer(CAM_DUMMY[i]);
        // Write DUMMY data, read the response
    }
    SPI_nSS_1=1;
    __delay_ms(12);

    if((recev[2]==0xff) && (recev[3]==0x0e) && (recev[4]==cmd[3])){
        // If received comm is ACK
            rsend = 0;
            // Rsend status flag set to 0 (Successful)
    }

    return rsend;							// Return the status flag rsend
}	


// ============================================================================================================
// This function sends the necessary commands to take a photo in the C329 camera (For more information see C329's UM)
// Arguments	:The following are DEFAULTS values that WORK well
//				 int resolution (resolution of the photo (0x07 for 640x480 pixels))
//				 int qual (quality of the photo (0x00 for best))
//				 int pic_type (type of the photo (0x05 for compression))
// Return		:int length  (0 is there's no photo or any ACK failed, value (nonzero) if not)
// NOTE			:[[Length variable type MUST be confirmed - Can be overflown by a big photo]]
// ============================================================================================================
unsigned int cam_photo(int resolution, int qual, int pic_type){
    unsigned int i;
    unsigned char recev[8];

    unsigned int length = 0;
    int rinit = 1;
    int rquality = 1;
    int rphoto = 1;

    #if (SCH_CAMERA_VERBOSE>=2)
        printf("send_comm(INITIAL, 0x00, 0x87, resolution, resolution);\r\n");
    #endif
    rinit = send_comm(CAM_INITIAL, 0x00, 0x87, resolution, resolution);
    // Send an INITIAL command and store the status flag received
    if(!rinit){

        #if (SCH_CAMERA_VERBOSE>=2)
            printf("INITIAL was successful\r\n");
        #endif

        __delay_ms(12);
        // Delay 12 us (Camera requirement)
        while(PPC_CAM_HOLD_CHECK){;}
        // Wait for low PPC_CAM_HOLD_CHECK signal

        rquality = send_comm(CAM_QUALITY, qual, 0x00, 0x00, 0x00);
        // Send a QUALITY command and store the status flag received
        if(!rquality){

            #if (SCH_CAMERA_VERBOSE>=2)
                printf("QUALITY was successful\r\n");
            #endif

            __delay_ms(12);
            // Delay 12 us (Camera requirement)
            while(PPC_CAM_HOLD_CHECK){;}
            // Wait for low PPC_CAM_HOLD_CHECK signal

            //rphoto = send_comm(SNAPSHOT);
            // This command is not necessary
            //delay_ms(12);
            //while(PPC_CAM_HOLD_CHECK){;}

            rphoto = send_comm(CAM_GETPIC, pic_type, 0x00, 0x00, 0x00);
            // Send a PHOTO command and store the received status flag
            if(!rphoto){

                #if (SCH_CAMERA_VERBOSE>=2)
                    printf("GETPIC was successful\r\n");
                #endif

                __delay_ms(12);
                // Delay 12 us (Camera requirement)
                while(PPC_CAM_HOLD_CHECK){;}
                // Wait for low PPC_CAM_HOLD_CHECK signal

                SPI_nSS_1=0;
                // Select the camera
                for(i=0; i<8; i++){
                    recev[i] = SPI_1_transfer(CAM_DUMMY[i]);
                    // Write a DUMMY data to receive a DATA command
                }
                SPI_nSS_1=1;
                // Deselect the camera

                __delay_ms(12);
                // Delay 12 us (Camera requierement)
                //puthex8(recev, 8);

                if( ( (recev[2]==0xff) && (recev[3]==0x0a) ) ){
                    // If received comm is DATA
                    length = recev[5] + recev[6]*0x100 + recev[7]*0x10000;
                    // Compute the length of the photo
                }
            }
        }
    }

    return length;
}

// ============================================================================================================
// This function uses the length given by PHOTO functions to receive the corresponding data of the picture
// Arguments	:int length (length of the photo)
// Return		:None
// ============================================================================================================
void cam_recev_photo(unsigned int length){
    unsigned int l,m;
    unsigned int numb = length/8;		// Number of blocks of eight bytes each
    if((length % 8) > 0) numb++;		// If lefts some bytes (lees than eigth), add one to variable numb

    char picture; char ret[10];      	// Variable for the received data
    printf("\r\n");
    for(l=0; l<numb; l++){			// Variable l goes from 0 to block number (numb)
        SPI_nSS_1=0;								// Select the camera (NOTE: COULD BE BEFORE THE FOR)
        for(m=0; m<8; m++){			// Read eigth bytes each time
            picture = SPI_1_transfer(0x00);	// Send a dummy byte to read one from the camera
            //utoa(ret, (unsigned int)(0x00FF&picture), 16);
            if( picture==0x00 ){
                printf("00");
            }
            else{
                if( (picture&0xF0)==0x00 ){
                    printf("0");
                    sprintf (ret, "%X", ((unsigned char)picture) );
                    printf(ret);
                }
                else{
                    sprintf (ret, "%X", ((unsigned char)picture) );
                    printf(ret);
                }
            }
        }
        SPI_nSS_1=1;			// Deselect the camera
        __delay_ms(12);               // Delay of 12 us (Camera requierement)
    }
    printf("\r\n");
}

unsigned int cam_take_photo(void)
{
    printf("photo(1,0,1);\r\n");
    return cam_photo(1,0,1);//Init camera with 80x60 preview and compression resolution
                       // with the " best " Quality
                        // with mode get picture " snapshot picture"
}

void cam_wait_hold_wtimeout(BOOL verb){
    if(verb){
        printf("[cam_wait_hold_wtimeout] initial state PPC_CAM_HOLD_CHECK = %d \r\n", PPC_CAM_HOLD_CHECK);
    }
    unsigned long int max_msec_hold_wait=0;
    while(TRUE){
        if(max_msec_hold_wait >= 10000){break;}
        if(PPC_CAM_HOLD_CHECK==0){break;}
        //update data
        max_msec_hold_wait += 100;
        __delay_ms(100);
    }
    if(verb){
        printf("[cam_wait_hold_wtimeout] final state PPC_CAM_HOLD_CHECK = %d \r\n", PPC_CAM_HOLD_CHECK);
        printf("[cam_wait_hold_wtimeout] max_msec_hold_wait = %lu \r\n", max_msec_hold_wait);
    }
}

int cam_isAlive(void){
    /*
     * Is assummed that the camera isAlive if HOLD is not set (HOILD == 0)
     */
    return !PPC_CAM_HOLD_CHECK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int cam_takePhoto_v2(int resolution, int qual, int pic_type){
    printf("cam_takePhoto_v2 .. \r\n");
            
    int status; 
    
    status = cam_sync(TRUE);
    printf("status = %d \r\n", status);
    
    status = cam_sync_v2();
    printf("status = %d \r\n", status);
    
    return 1;
}

unsigned int cam_sync_v2(void){
    printf("cam_sync_v2 .. \r\n");
  
    int status, j;
    unsigned char cmd_recev[8];
    for(j=0; j<=5; j++){
        cam_send_cmd_v2(cmd_recev, CAM_SYNC, 0, 0, 0, 0);
        //__delay_ms(500);
        status = cam_check_cmd_recev_v2(cmd_recev, CAM_ACK, CAM_SYNC[3], 0, 0, 0);
        //__delay_ms(500);
        
        if(status >= 1){break;}
    }
        
        status = cam_check_cmd_recev_v2(cmd_recev, CAM_SYNC, 0, 0, 0, 0);
        //__delay_ms(500);
        cam_send_cmd_v2(cmd_recev, CAM_ACK, CAM_SYNC[3], 0, 0, 0);
        //__delay_ms(500);
    
    return status;
}

int cam_send_cmd_v2(unsigned char* cmd_recev, unsigned char* cmd, int arg1, int arg2, int arg3, int arg4){
    unsigned char comm_renew[8] = {cmd[0], cmd[1], cmd[2], cmd[3], arg1, arg2, arg3, arg4};
    unsigned int i;

    cam_wait_hold_wtimeout(TRUE);
    
    // Send command
    SPI_nSS_1=0;
    _LATG9 = 0;
    //__delay_ms(30); //camera requirement
    for(i=0; i<8; i++){
            cmd_recev[i] = SPI_1_transfer(comm_renew[i]);
    }
    SPI_nSS_1=1;
    _LATG9 = 1;
    //__delay_ms(30); //camera requirement
    
    //cam_wait_hold_wtimeout(TRUE);
    
    //debug info
    printf("Cmd sended is = 0x%02X %02X %02X %02X \r\n", comm_renew[0], comm_renew[1], comm_renew[2], comm_renew[3]);
    printf("  param1 = 0x%02X \r\n", comm_renew[4]);
    printf("  param2 = 0x%02X \r\n", comm_renew[5]);
    printf("  param3 = 0x%02X \r\n", comm_renew[6]);
    printf("  param4 = 0x%02X \r\n", comm_renew[7]);
    
    return 1;
}	

int cam_check_cmd_recev_v2(unsigned char* cmd_recev, unsigned char* cmd, int arg1, int arg2, int arg3, int arg4){
//    unsigned char recev[8];
//    unsigned int i;
    int rsend = 0;
//
//    cam_wait_hold_wtimeout(TRUE);
//    
//    SPI_nSS_1=0;
//    __delay_ms(30); //camera requirement
//    for(i=0; i<8; i++){
//        recev[i] = SPI_1_transfer(CAM_DUMMY[i]);
//        // Write DUMMY data, read the response
//    }
//    SPI_nSS_1=1;
//    __delay_ms(30); //camera requirement
    
    //cam_wait_hold_wtimeout(TRUE);

    //debug info
    printf("Cmd received is = 0x%02X %02X %02X %02X \r\n", cmd_recev[0], cmd_recev[1], cmd_recev[2], cmd_recev[3]);
    printf("  param1 = 0x%02X \r\n", cmd_recev[4]);
    printf("  param2 = 0x%02X \r\n", cmd_recev[5]);
    printf("  param3 = 0x%02X \r\n", cmd_recev[6]);
    printf("  param4 = 0x%02X \r\n", cmd_recev[7]);
    
    //Check received Cmd
    if((cmd_recev[0]==cmd[0]) && (cmd_recev[1]==cmd[1]) && (cmd_recev[2]==cmd[2]) && (cmd_recev[3]==cmd[3])){
        printf("Cmd received is correct \r\n");
        rsend = 1;
        if((cmd_recev[4]==arg1) && (cmd_recev[5]==arg2) && (cmd_recev[6]==arg3) && (cmd_recev[7]==arg4)){
            printf("Parameters received are correct \r\n");
            rsend = 2;
        }
    }
    else{
        printf("Cmd received is wrong \r\n");
        printf("Cmd expected was = 0x%02X %02X %02X %02X \r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
        rsend = 0;
    }

    return rsend;
}