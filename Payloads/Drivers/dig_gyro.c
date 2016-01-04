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
Description            		:Function set the digital gyroscope
File                            :dig_gyro.c
Created by    	           	:Alex Becerra & Tomas Opazo
Project                    	:SUCHAI
Creation date        		:01/06/2012

Note(1)               		:Uses Carlos Gonzalez's I2C implementation
==========================================================================================*/

#include "dig_gyro.h"

// ============================================================================================================
// Write a register
// Arguments	: unsigned char reg Register to write
//                unsigned char value Value to write
// Return      	: None
// ============================================================================================================
void gyr_write_reg(unsigned char reg, unsigned char val){
    char address[] = {GYR_ADD, (char)reg};
    char _val = (char)val;
    i2c3_master_fputs(&_val, 1, address, 2);
}

// ============================================================================================================
// Write a register
// Arguments	: unsigned char reg Register to read
// Return      	: unsigned char Register value
// ============================================================================================================
unsigned char gyr_read_reg(unsigned char reg){
    char ret;
    char address[] = {GYR_ADD, (char)reg};
    i2c3_master_fgets(&ret, 1, address, 2);
    return (unsigned char)ret;
}

BOOL gyr_isAlive(void){
    unsigned char who_am_i = 0b11010011;
    unsigned char val = 0;
    val = gyr_read_reg(GYR_WHO_AM_I);
    if(val==who_am_i){ return TRUE; }
    else{ return FALSE; }
}
// ============================================================================================================
// Returns the identification of the device 
// Arguments	: None
// Return      	: None
// NOTE			: Returned value MUST be 0xD3 (Factory value for L3G4200D three axes gyro)
// ============================================================================================================
void whoami(void){

	unsigned char who = 0;
        who = gyr_read_reg(GYR_WHO_AM_I);
	con_printf("Who_am_I returned: ");
        char ret[10];
        //utoa(ret, (unsigned int)who, 16);
        sprintf (ret, "0x%X", (unsigned int)who);
        con_printf(ret); con_printf("\r\n");
}

// ============================================================================================================
// Configures the FIFO_GYR_CTRL_REG
// Arguments	: unsigned char watermark to configure the watermark level of the memory (useful for INT2)
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void config_FIFO_GYR_CTRL_REG(unsigned char watermark){
    unsigned char dummy = gyr_read_reg(GYR_FIFO_CTRL_REG);

    unsigned char value = ( (dummy & 0b11100000) | (watermark) );
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_FIFO_CTRL_REG, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
        I2C3ReadChar(GYR_ADD_WRITE, GYR_FIFO_CTRL_REG, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
        // Read the register to verify the configuration
        con_printf("FIFO_GYR_CTRL_REG register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}


// ============================================================================================================
// Puts de FIFO in one of the possible modes
// Arguments	: unsigned char mode for FIFO mode selection
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_config_FIFO_mode(unsigned char mode){
    unsigned char dummy = gyr_read_reg(GYR_FIFO_CTRL_REG);

    unsigned char value = ( (dummy & 0b00011111) | (mode << 5) );
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_FIFO_CTRL_REG, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_FIFO_CTRL_REG, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
        // Read the register to verify the configuration
	con_printf("FIFO mode changed to \t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

// ============================================================================================================
// Puts the GYRO in powerdown mode
// Argument		: unsigned char powerdown to put the gyro in normal or power down feature
// Return      	: None
// ============================================================================================================
void gyr_powermode(unsigned char powerdown){
    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG1);

    unsigned char value = ((dummy & 0b11110111) | (powerdown << 3));
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG1, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG1, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
        // Read the register to verify the configuration
	con_printf("Gyro mode changed to \t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}


// ============================================================================================================
// Enables or disables a particular axis in gyro
// Argument		: unsigned char axis contains the information for axis enabling or disabling
// Return      	: None
// Note			: For axes configuration see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_enable_axis(unsigned char axis){
	
    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG1);;

    unsigned char value = ((dummy & 0b11111000) | (axis));
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG1, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG1, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
	con_printf("Axis enabled/disbled \t ");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

	

// ============================================================================================================
// Sees the remaining samples to be read
// Arguments	: None
// Return      	: None
// ============================================================================================================
void gyr_print_remain_samp(void){
    unsigned char dummy = gyr_read_reg(GYR_FIFO_SRC_REG);

    con_printf("FIFO remaining samples to be read :");
    unsigned char numero = dummy&0x1F;
    char ret[10];
    //itoa(ret, (unsigned int)numero, 16);
    sprintf (ret, "0x%X", (unsigned int)numero);
    con_printf(ret); con_printf("\r\n");
}


// ============================================================================================================
// Returns the cause of the FIFO interruption (INT2 must be configured for that)
// Arguments	: None
// Return      	: None
// NOTE			: Void until customization is implemented.
// ============================================================================================================
void gyr_print_FIFO_int_source(void){
    unsigned char fifo_stats = gyr_read_reg(GYR_FIFO_SRC_REG);

    if(fifo_stats&0x80){
        con_printf("WATERMARK interruption\r\n");
    }
    else{
        if(fifo_stats&0x40){
            con_printf("OVERRUN interruption\r\n");
        }
        else{
            if(fifo_stats&0x20){
                con_printf("EMPTY interruption\r\n");
            }
            else
                con_printf("UNKNOWN interruption\r\n");
        }
    }

}


// ============================================================================================================
// Reads the temperature of the sensor
// Arguments	: None
// Return      	: None
// ============================================================================================================
void gyr_print_temp(void){
    unsigned char dummy = gyr_read_reg(GYR_OUT_TEMP);
    con_printf("Sensor temperature: \t ");
    char ret[10];
    //itoa(ret, (unsigned int)dummy, 16);
    sprintf (ret, "0x%X", (unsigned int)dummy);
    con_printf(ret); con_printf("\r\n");
}

// ============================================================================================================
// Reads the FIFO content
// Arguments	: None
// Return      	: int direcc_b (Value of first address of data buffer)
// ============================================================================================================
void gyr_get_FIFO_samples(BOOL verb, GYR_DATA *res_data){
    #if (SCH_GYRO_VERBOSE>=2)
	con_printf("Entered to read FIFO\r\n");
    #endif
	
    unsigned char dummy = gyr_read_reg(GYR_FIFO_CTRL_REG);
    unsigned char muestras = (dummy & 0x1F)+1;
    // Plus one because the first sample is in the position zero of the array

    #if (SCH_GYRO_VERBOSE>=2)
	con_printf("SAMPLES (number) recovered \t");		// Samples for ONE axis
        char ret[6];
        itoa(ret,  (unsigned int)muestras, 10); con_printf(ret); con_printf("\r\n");
    #endif

    char address[] = {GYR_ADD, GYR_OUT_X_L_RB};
    unsigned char buffer[muestras*3*2]; // By 3 because there's 3 axes
                                        // By 2 because each sample is two bytes
    i2c3_master_fgets((char *)buffer, muestras*3*2, address, 2);

    #if (SCH_GYRO_VERBOSE>=2)
	con_printf("Samples read ... \r\n");	// for debugging
    #endif

    unsigned char *direcc_b;
    direcc_b = &buffer[0];
    gyr_get_data(verb, direcc_b, muestras, res_data);

}

// ============================================================================================================
// Prints (in console screen) the data stored in buffer
// Arguments	: int dir (value of the first buffer address)
//				  unsigned char muestras for the number of samples (for each axis)
// Return      	: None
// ============================================================================================================
void gyr_get_data(BOOL verb, unsigned char *dir, unsigned char muestras, GYR_DATA *res_data ){
    int i;
    unsigned char a,b,d,e,g,h;
    signed int c,f,z;
    signed long k,j,l;
    j = k = l = 0;

    if(verb){
        for(i=0; i<(3*2*muestras); i++){
            printf("dir[%d] = 0x%X\n", i, dir[i]);
            printf("************************\n");
        }
    }

    for(i=0; i<(3*2*muestras); i = i+6){
        a = dir[i+1];
        b = dir[i];
        c = ((a<<8) | b);
        d = dir[i+3];
        e = dir[i+2];
        f = ((d<<8) | e);
        g = dir[i+5];
        h = dir[i+4];
        z = ((g<<8) | h);
        j = j + (long) c;
        k = k + (long) f;
        l = l + (long) z;

        #if (SCH_GYRO_VERBOSE>=2)
            itoa(ret,  (unsigned int)c, 10); con_printf(ret); con_printf("\r\n");
            itoa(ret,  (unsigned int)f, 10); con_printf(ret); con_printf("\r\n");
            itoa(ret,  (unsigned int)z, 10); con_printf(ret); con_printf("\r\n");
        #endif
    }
    j = j/muestras;
    k = k/muestras;
    l = l/muestras;

    (*res_data).a_x=j;
    (*res_data).a_y=k;
    (*res_data).a_z=l;

    if(verb){
	printf("gyr_get_data\r\n");
	printf("X axis : %d\n", (int)j);
        printf("Y axis : %d\n", (int)k);
        printf("Z axis : %d\n", (int)l);
        printf("************************\n");
    }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

// ============================================================================================================
// Configures the GYR_CTRL_REG1
// Arguments	: unsigned char datarate for datarate selection
//				  unsigned char bandwidth for bandwidth selection
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_config_GYR_CTRL_REG1(unsigned char datarate, unsigned char bandwidth){
    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG1);

    unsigned char value = ( (dummy & 0b00000000) | (datarate << 6) | (bandwidth << 4) );
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG1, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG1, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
        // Read the register to verify the configuration
	con_printf("GYR_CTRL_REG1 register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

void gyr_config_GYR_CTRL_REG2(unsigned char HPF_mode, unsigned char HPF_CO){
    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG2);

    unsigned char value = ((dummy & 0b11000000) | (HPF_mode << 4) | (HPF_CO));		// Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG2, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
        I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG2, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);
        // Read the register to verify the configuration
        con_printf("GYR_CTRL_REG2 register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

// ============================================================================================================
// Configures the GYR_CTRL_REG3 register
// Arguments	: unsigned char INT2_config for INT2 behavior configuration
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_config_GYR_CTRL_REG3(unsigned char INT2_config){

    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG3);

    unsigned char value = ( (dummy & 0b11110000) | (INT2_config) );					// Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG3, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG3, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);						// Read the register to verify the configuration
	con_printf("GYR_CTRL_REG3 register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

// ============================================================================================================
// Configures the GYR_CTRL_REG4
// Arguments	: unsigned char BDU for block data update configuration
//				  unsigned char scale for measurement scale selection
//				  unsigned char self_test for self test selection (debugging purposes)
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_config_GYR_CTRL_REG4(unsigned char BDU, unsigned char scale, unsigned char self_test){

    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG4);

    unsigned char value = ( (dummy & 0b00000000) | (BDU << 7) | (scale << 4) | (self_test << 1) );
    // Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG4, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG4, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);											// Read the register to verify the configuration
	con_printf("GYR_CTRL_REG4 register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}


// ============================================================================================================
// Configures the GYR_CTRL_REG5
// Arguments	: unsigned char boot for boot mode selection
//				  unsigned char FIFO_en to enable the FIFO memory
//				  unsigned char HPF_en to enable the high pass filter
//				  unsigned char outsel for data filter selection (HPF and/or LPF)
// Return      	: None
// Note			: For argument values see the L3G4200D datasheet or application note.
// ============================================================================================================
void gyr_config_GYR_CTRL_REG5(unsigned char boot, unsigned char FIFO_en, unsigned char HPF_en, unsigned char outsel){
    unsigned char dummy = gyr_read_reg(GYR_CTRL_REG5);

    unsigned char value = ( (dummy & 0b00000000) | (boot << 7) | (FIFO_en << 6) | (HPF_en << 4) | (outsel) );		// Read the actual register value and modify only the arguments given
    gyr_write_reg(GYR_CTRL_REG5, value);
    //TODO: while(!I2C3SlaveReady(GYR_ADD_WRITE)){;}

    #if (SCH_GYRO_VERBOSE>=2)
	I2C3ReadChar(GYR_ADD_WRITE, GYR_CTRL_REG5, GYR_ADD_READ, &dummy, 1, I2C_MODE_STD);														// Read the register to verify the configuration
	con_printf("GYR_CTRL_REG5 register configured with value\t");
        char ret[6];
        itoa(ret,  (unsigned int)dummy, 10); con_printf(ret); con_printf("\r\n");
    #endif
}

int gyr_init_config(void){
    if( gyr_isAlive()==FALSE ){return 0;}

    // Register configuration (Register order suggested in application note)
    gyr_config_GYR_CTRL_REG2(0x00, 0x00);              // HPF Normal mode and 15 Hz cut off frequency (depends on datarate)
    gyr_config_GYR_CTRL_REG3(0x04);                    // Only Watermark interruption enabled
    gyr_config_GYR_CTRL_REG4(0x01, 0x03, 0x00);        // NO continous update, 2000 dps and self test disabled
    gyr_config_GYR_CTRL_REG5(0x01, 0x01, 0x01, 0x03);  // Reboot memory content, FIFO enable, HPF enabled and LPF anbled

    return 1;
}
void gyr_take_samples(BOOL verb, GYR_DATA *res_data){
    gyr_config_GYR_CTRL_REG1(0x01, 0x03);  // 200 Hz datarate and 70 cutoff
    gyr_enable_axis(0x07);                  // Enable three axes
    gyr_powermode(0x01);                    // Enable device
    __delay_ms(50);

    config_FIFO_GYR_CTRL_REG(0b00000111);   // configure FIFO_GYR_CTRL_REG with watermark
    if(verb){
        gyr_print_remain_samp();            // Obtain the number of remaining samples
    }
    gyr_config_FIFO_mode(0x01);         // Mode to FIFO mode

    int counts = 0;
    while(!GYR_INT2 && counts<10 ){
        if(verb){
            con_printf("GYR_INT2=0\r\n");
        }
        __delay_ms(1000);
        counts++;
    }
    __delay_ms(50);

    if(verb){
        gyr_print_FIFO_int_source();    // see FIFO interruption
        gyr_print_remain_samp();        // Samples to read
        gyr_print_temp();               // Read the temperature of the sensor
    }


    __delay_ms(50);
    if(verb){
        con_printf("showing samples from buffer\r\n");
    }

    gyr_get_FIFO_samples(verb, res_data);  // read FIFO data
    if(verb){
	printf("gyr_take_samples\r\n");
	printf("X axis : %d\n", (*res_data).a_x );
        printf("Y axis : %d\n", (*res_data).a_y );
        printf("Z axis : %d\n", (*res_data).a_z );
        printf("************************\n");
    }

    __delay_ms(50);

    gyr_config_FIFO_mode(0x00);     // go to bypass mode (in order to go
                                    // to FIFO mode again - device requirement)
    __delay_ms(50);


    //ClrWdt();
    // --------------------------------------------
    // Debugging purposes (Change scale to 250 dps)
    /*
    gyr_config_GYR_CTRL_REG4(0x01, 0x00, 0x00);
    con_printf("Scale changed to 250 dps\r\n");

    gyr_powermode(0x00);						// to powerdown
    */
}
