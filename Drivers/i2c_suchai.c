/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2012, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

#include "i2c_suchai.h"

static void I2C1Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i);
static void I2C1WriteNextState(unsigned int *STA1, unsigned int n1, unsigned int *i1);
static void I2C1Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR);
static void I2C1ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE);


/*------------------------------------------------------------------------------------------
 *		 							CONFIG I2C
 *------------------------------------------------------------------------------------------
 * Function Prototype : void ConfigI2C(unsigned int BRG)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Confiugures I2C periferal to operate at user defined frecuency
 *						and disable interrupt
 * Arguments          : BRG - Computed value for the baud rate generator
 * Return Value      :  None
 * Remarks           :  This function configures the I2C Control register, I2C 
                     	Baud Rate Generator register and disable interrupts
 *----------------------------------------------------------------------------------------*/
 void ConfigI2C1(unsigned int BRG)
 {
	/* I2C Clock Rates:
	 *
	 * Req.F  -  FCY  - BRG
	 * 100kHz - 16MHz - 157
	 * 100kHz - 08MHz - 78
	 * 400kHz - 16MHz - 37
	 * 001MHz - 16MHz - 13
	 */
 
 	//ConfigIntI2C1(MI2C_INT_ON & MI2C_INT_PRI_6 & SI2C_INT_ON & SI2C_INT_PRI_5); /* Interrupt mode state trantition */
	ConfigIntI2C1(MI2C_INT_OFF & MI2C_INT_PRI_0 & SI2C_INT_OFF & SI2C_INT_PRI_0); /* Software mode state trantition */
	
	/* OPEN I2C OPTION EXPLANATION
	 *
	 * I2C_IPMI_DIS/EN: Intelligent Perih Mang. Interface to accept all adresses (DIS normally)
	 * I2C_SLW_DIS/EN: Slew Rate required at 400kHz only
	 * I2C_SM_DIS/EN: SMBus voltage specifications (DIS normally)
	 * I2C_GCALL_DIS/EN: When generall call is used, all enabled devices should respond with an Acknowledge
	 * NOTE: I2C module operates as Master and Slave simultaneously
	 */
	OpenI2C1(I2C_ON & I2C_IDLE_CON & I2C_CLK_REL & I2C_IPMI_DIS & I2C_7BIT_ADD & I2C_SLW_DIS & I2C_SM_DIS & I2C_GCALL_DIS & I2C_STR_DIS &
			I2C_ACK & I2C_ACK_DIS & I2C_RCV_DIS & I2C_STOP_DIS & I2C_RESTART_DIS & I2C_START_DIS,
			BRG) ; /* BRG according to I2C Clock Rate table*/

}
/*------------------------------------------------------------------------------------------
 *		 			I2C SEND CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2CSendChar(unsigned char ID, unsigned char AD, unsigned char DA)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Writes a data byte into an I2C EEPROM memory (as RAM)
 * Arguments          : ID - This contains the indentifier to select correpond I2C disp.
 *			AD - This contains the addres of the EERPOM will be write
 *			DATA - This contains the data byte will be write in the EERPOM	 
 *			N - The number of bytes to be read from *DATA an send
 * Return Value      :  None
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C1SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N)
{
	unsigned char Id_add[2];
	unsigned int State;
	unsigned int i;
	
	Id_add[0]=ID; Id_add[1]=AD;
	State=I2C_START;
	i=0;
	
	/* Return when transmission is completed */
        int max=0;
	while(State!=I2C_IDLE)
	{
		I2C1Write(&State, Id_add, DATA, i);
		I2C1WriteNextState(&State,N,&i);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}	
}
/*------------------------------------------------------------------------------------------
 *		 							I2C READ CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2C1ReadChar(unsigned char IDW, unsigned char AD,
 *                      unsigned char IDR, unsigned char *BUFF, unsigned int N,
 *                      unsigned char MODE)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Reads a data byte by byte from I2C and stores data in the
 *                      specified buffer
 * Arguments          : IDW - This contains the indentifier to select correpond I2C disp. (write)
 *						AD  - This contains the addres of the EERPOM will be read.
 *						IDR - This contains the indentifier to select correpond I2C disp. (read)
 *						N	- Number of bytes to read
 *						MODE- when MODE=0 send the IDW, AD and IDR, and when MODE=1 send just IDR
 *                      BUFF - Contais the byte read from EEPROM
 * Return Value      :  none
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C1ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE)
{
	unsigned char DataR[3];
	unsigned int StateR;
	unsigned int iR;
	
	DataR[0]=IDW; DataR[1]=AD; DataR[2]=IDR;
	StateR=I2C_START;
	iR=0;
	
	/* Return when transmission is completed */
        int max=0;
	while(StateR!=I2C_IDLE)
	{
		I2C1Read(&StateR, DataR, BUFF, N, iR);
		I2C1ReadNextState(&StateR,&iR,N,MODE);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}
}
/*------------------------------------------------------------------------------------------
 *		 							I2C SLAVE READY
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int I2C1SlaveReady(unsigned char ID)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Return 0 if Slave is busy (No ACK). Send a START condition and 
 * Arguments          : None
 * Return Value      :  1-> Slave send ACK, so is ready. 0-> Slave don't send ACK, not ready.
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
unsigned int I2C1SlaveReady(unsigned char ID)
{
	unsigned int val;
	IdleI2C1();
	StartI2C1();
	/* Esperar se complete secuencia START */
	while(!IFS1bits.MI2C1IF);
	MI2C1_Clear_Intr_Status_Bit;
	
	/* Esperar se desocupe registro TBF */
	while(I2C1STATbits.TBF);
	
	/* Tratar de enviar ID */
	I2C1TRN = ID;
	
	/* If write collision occurs,STOP */
	if(I2C1STATbits.IWCOL)
	{
   		I2C1STATbits.IWCOL=0;
   		/* Falló la comunicación */
   		val = 0;
	}
	else
	{ 
		/* Esperar a que termine la escritura */    		
		while(!IFS1bits.MI2C1IF);
		MI2C1_Clear_Intr_Status_Bit;
		
		/* Revisar ACK */
		/*ACKSTAT=0 -> ACK OK*/
		if(!I2C1STATbits.ACKSTAT)
			/* Dispositivo Preparado */
			val = 1;
		else
			/* Dispositivo no preparado */
			val = 0;
	}
	StopI2C1();
	/* Esperar a que se mande STOP*/    		
	while(!IFS1bits.MI2C1IF);
	MI2C1_Clear_Intr_Status_Bit;
				
	return val;	
}  

/*---------------------------------
 *		I2C WRITE SATATE MACHINE
 *---------------------------------*/
static void I2C1Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i)
{
	switch(*STA)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C1();
			StartI2C1();
			*STA=I2C_BUSY_START;
			break;
		
		case I2C_BUSY_START:
			break;
			
		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
			I2C1TRN = *ID_ADD;
			
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C1STATbits.IWCOL=0;
   			}     		
	    	else
	    		*STA=I2C_BUSY1;
			break;
			
		case I2C_BUSY1: /* Transmiting */
			break;
			
		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
			I2C1TRN = *(ID_ADD+1);
			
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */	
			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C1STATbits.IWCOL=0;
   			}     		
	    	else
	    		*STA=I2C_BUSY2;
				
			break;
			
		case I2C_BUSY2: /* Transmiting */
			break;
		
		/* Este estado escribe una serie de datos desde el buffer DATA */	
		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
			I2C1TRN = *(DATA+i);
			
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */	

			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C1STATbits.IWCOL=0;
   			}     		
	    	else
	    		*STA=I2C_BUSY3;
			
			break;
			
		case I2C_BUSY3: /* Transmiting */
			break;
			
		case I2C_STOP: /* Send a stop condition */
			StopI2C1();
			*STA=I2C_BUSY_STOP;
			break;
			
		case I2C_BUSY_STOP:
			break;
			
		case I2C_IDLE: /* Wait state */
	///* DEBUG INST */button=0;
			break;
	}
}

/*---------------------------------
 *	I2C WRITE NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C1WriteNextState(unsigned int *STA, unsigned int n, unsigned int *i)
{
	if(IFS1bits.MI2C1IF)
	{
		/* TRANSICION CON INTERRUPCION */       	
		switch(*STA)
		{
			case I2C_BUSY_START: /* Start condition ok */
				*STA=I2C_WRITE1;
				break;
	
			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA=I2C_WRITE2;
				else
					*STA=I2C_STOP;
				break;
			
			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/
                                    *STA=I2C_WRITE3;
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;
				
			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Se aumenta el puntero al dato */
				(*i)++;
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/

                                    /* ------------
                                     * Se permite escritura de página volviendo al estado WRITE3 hasta que
                                     * se enviaron todos los bytes deseados (máximo 16)
                                     * ----------- */
                                    if((*i)<n && (*i)<=15){
                                        *STA=I2C_WRITE3;
                                    }
                                    else{
                                        *STA=I2C_STOP;
                                    }
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;
				
			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C1_Clear_Intr_Status_Bit;	
	}			
}


/*---------------------------------
 *		I2C READ SATATE MACHINE
 *---------------------------------*/
static void I2C1Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR)
{
	switch(*STA_R)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C1();
			StartI2C1();
			*STA_R=I2C_BUSY_START;
			break;
		
		case I2C_BUSY_START:
			break;
			
		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
			I2C1TRN = *ID_R; /* ID */
			
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
                            *STA_R=I2C_STOP;
                            I2C1STATbits.IWCOL=0;
   			}     		
                	else{
                            *STA_R=I2C_BUSY1;
                        }
			break;
			
		case I2C_BUSY1: /* Transmiting */
			break;
			
		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
				I2C1TRN = *(ID_R+1); /* WORD ADDRESS */
				
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C1STATbits.IWCOL=0;
   			}     		
	    	else
	    		*STA_R=I2C_BUSY2;
			
			break;
			
		case I2C_BUSY2: /* Transmiting */
			break;
			
		case I2C_START2: /* Can send a start bit */
			if(!I2C1CONbits.RSEN)
			{
				I2C1CONbits.RSEN=1;//RestartI2C1();
				*STA_R=I2C_BUSY_START2;
			}	
			break;
		
		case I2C_BUSY_START2:
			break;
			
		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C1STATbits.TBF);
				I2C1TRN = *(ID_R+2); /*|0x01 aseguro enviar siempre comando de lectura */
				
			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C1STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C1STATbits.IWCOL=0;
   			}     		
	    	else
	    		*STA_R=I2C_BUSY3;
	    		
			break;
			
		case I2C_BUSY3: /* Transmiting */
			break;
			
		case I2C_READ:
			if(!I2C1CONbits.RCEN)
				I2C1CONbits.RCEN = 1; /* Start read secuence */
			*STA_R=I2C_BUSY4;
			break;
			
		case I2C_BUSY4:
			break; /* Espero que lleguen los datos */
			
		case I2C_MODE_ACK:
			/* Se lee el i-esimo dato */
			*(DATA_R+iR)=I2C1RCV;
			I2C1STATbits.I2COV = 0;
			if((iR+1) < NR)
				AckI2C1(); /* ACK => Read next data byte */
			else
				NotAckI2C1(); /* No more data, THEN NACK AND GO TO STOP */
			*STA_R=I2C_BUSY_ACK;
			break;
		
		case I2C_BUSY_ACK:
			break;
			
		case I2C_STOP: /* Send a stop condition */
			StopI2C1();
			*STA_R=I2C_BUSY_STOP;
			break;
			
		case I2C_BUSY_STOP:
			break;	
			
		case I2C_IDLE: /* Wait state */
			break;
	}
}

/*---------------------------------
 *	I2C READ NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C1ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE)
{
	if(IFS1bits.MI2C1IF)
	{
		/* TRANSICION CON INTERRUPCION */       	
		switch(*STA_R1)
		{
			case I2C_BUSY_START: /* Start condition ok */
				if(MODE==0)
					*STA_R1=I2C_WRITE1;
				else
					*STA_R1=I2C_WRITE3;
				break;
	
			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_WRITE2;
				else
					*STA_R1=I2C_STOP;
				break;
			
			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_START2;
				else
					*STA_R1=I2C_STOP;
				break;
		
			case I2C_BUSY_START2:
				*STA_R1=I2C_WRITE3;
				break;
								
			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C1STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;
				
			case I2C_BUSY4://READ
				*STA_R1=I2C_MODE_ACK;
				break;
			
			case I2C_BUSY_ACK:
				(*iR1)++;
				if((*iR1) < NR1)
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;
				
			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA_R1=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C1_Clear_Intr_Status_Bit;	
	}			
}

//******************************************************************************
//******************************************************************************

static void I2C2Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i);
static void I2C2WriteNextState(unsigned int *STA1, unsigned int n1, unsigned int *i1);
static void I2C2Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR);
static void I2C2ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE);
/*------------------------------------------------------------------------------------------
 *		 							CONFIG I2C
 *------------------------------------------------------------------------------------------
 * Function Prototype : void ConfigI2C(unsigned int BRG)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Confiugures I2C periferal to operate at user defined frecuency
 *						and disable interrupt
 * Arguments          : BRG - Computed value for the baud rate generator
 * Return Value      :  None
 * Remarks           :  This function configures the I2C Control register, I2C
                     	Baud Rate Generator register and disable interrupts
 *----------------------------------------------------------------------------------------*/
 void ConfigI2C2(unsigned int BRG)
 {
	/* I2C Clock Rates:
	 *
	 * Req.F  -  FCY  - BRG
	 * 100kHz - 16MHz - 157
	 * 100kHz - 08MHz - 78
	 * 400kHz - 16MHz - 37
	 * 001MHz - 16MHz - 13
	 */

 	//ConfigIntI2C2(MI2C_INT_ON & MI2C_INT_PRI_6 & SI2C_INT_ON & SI2C_INT_PRI_5); /* Interrupt mode state trantition */
	ConfigIntI2C2(MI2C_INT_OFF & MI2C_INT_PRI_0 & SI2C_INT_OFF & SI2C_INT_PRI_0); /* Software mode state trantition */

	/* OPEN I2C OPTION EXPLANATION
	 *
	 * I2C_IPMI_DIS/EN: Intelligent Perih Mang. Interface to accept all adresses (DIS normally)
	 * I2C_SLW_DIS/EN: Slew Rate required at 400kHz only
	 * I2C_SM_DIS/EN: SMBus voltage specifications (DIS normally)
	 * I2C_GCALL_DIS/EN: When generall call is used, all enabled devices should respond with an Acknowledge
	 * NOTE: I2C module operates as Master and Slave simultaneously
	 */
	OpenI2C2(I2C_ON & I2C_IDLE_CON & I2C_CLK_REL & I2C_IPMI_DIS & I2C_7BIT_ADD & I2C_SLW_DIS & I2C_SM_DIS & I2C_GCALL_DIS & I2C_STR_DIS &
			I2C_ACK & I2C_ACK_DIS & I2C_RCV_DIS & I2C_STOP_DIS & I2C_RESTART_DIS & I2C_START_DIS,
			BRG) ; /* BRG according to I2C Clock Rate table*/

}
/*------------------------------------------------------------------------------------------
 *		 			I2C SEND CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2CSendChar(unsigned char ID, unsigned char AD, unsigned char DA)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Writes a data byte into an I2C EEPROM memory (as RAM)
 * Arguments          : ID - This contains the indentifier to select correpond I2C disp.
 *			AD - This contains the addres of the EERPOM will be write
 *			DATA - This contains the data byte will be write in the EERPOM
 *			N - The number of bytes to be read from *DATA an send
 * Return Value      :  None
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C2SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N)
{
	unsigned char Id_add[2];
	unsigned int State;
	unsigned int i;

	Id_add[0]=ID; Id_add[1]=AD;
	State=I2C_START;
	i=0;

	/* Return when transmission is completed */
        int max=0;
	while(State!=I2C_IDLE)
	{
		I2C2Write(&State, Id_add, DATA, i);
		I2C2WriteNextState(&State,N,&i);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}
}
/*------------------------------------------------------------------------------------------
 *		 							I2C READ CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2C2ReadChar(unsigned char IDW, unsigned char AD,
 *                      unsigned char IDR, unsigned char *BUFF, unsigned int N,
 *                      unsigned char MODE)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Reads a data byte by byte from I2C and stores data in the
 *                      specified buffer
 * Arguments          : IDW - This contains the indentifier to select correpond I2C disp. (write)
 *						AD  - This contains the addres of the EERPOM will be read.
 *						IDR - This contains the indentifier to select correpond I2C disp. (read)
 *						N	- Number of bytes to read
 *						MODE- when MODE=0 send the IDW, AD and IDR, and when MODE=1 send just IDR
 *                      BUFF - Contais the byte read from EEPROM
 * Return Value      :  none
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C2ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE)
{
	unsigned char DataR[3];
	unsigned int StateR;
	unsigned int iR;

	DataR[0]=IDW; DataR[1]=AD; DataR[2]=IDR;
	StateR=I2C_START;
	iR=0;

	/* Return when transmission is completed */
        int max=0;
	while(StateR!=I2C_IDLE)
	{
		I2C2Read(&StateR, DataR, BUFF, N, iR);
		I2C2ReadNextState(&StateR,&iR,N,MODE);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}
}
/*------------------------------------------------------------------------------------------
 *		 							I2C SLAVE READY
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int I2C2SlaveReady(unsigned char ID)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Return 0 if Slave is busy (No ACK). Send a START condition and
 * Arguments          : None
 * Return Value      :  1-> Slave send ACK, so is ready. 0-> Slave don't send ACK, not ready.
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
unsigned int I2C2SlaveReady(unsigned char ID)
{
	unsigned int val;
	IdleI2C2();
	StartI2C2();
	/* Esperar se complete secuencia START */
	while(!IFS3bits.MI2C2IF);
	MI2C2_Clear_Intr_Status_Bit;

	/* Esperar se desocupe registro TBF */
	while(I2C2STATbits.TBF);

	/* Tratar de enviar ID */
	I2C2TRN = ID;

	/* If write collision occurs,STOP */
	if(I2C2STATbits.IWCOL)
	{
   		I2C2STATbits.IWCOL=0;
   		/* Falló la comunicación */
   		val = 0;
	}
	else
	{
		/* Esperar a que termine la escritura */
		while(!IFS3bits.MI2C2IF);
		MI2C2_Clear_Intr_Status_Bit;

		/* Revisar ACK */
		/*ACKSTAT=0 -> ACK OK*/
		if(!I2C2STATbits.ACKSTAT)
			/* Dispositivo Preparado */
			val = 1;
		else
			/* Dispositivo no preparado */
			val = 0;
	}
	StopI2C2();
	/* Esperar a que se mande STOP*/
	while(!IFS3bits.MI2C2IF);
	MI2C2_Clear_Intr_Status_Bit;

	return val;
}

/*---------------------------------
 *		I2C WRITE SATATE MACHINE
 *---------------------------------*/
static void I2C2Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i)
{
	switch(*STA)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C2();
			StartI2C2();
			*STA=I2C_BUSY_START;
			break;

		case I2C_BUSY_START:
			break;

		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
			I2C2TRN = *ID_ADD;

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C2STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY1;
			break;

		case I2C_BUSY1: /* Transmiting */
			break;

		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
			I2C2TRN = *(ID_ADD+1);

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C2STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY2;

			break;

		case I2C_BUSY2: /* Transmiting */
			break;

		/* Este estado escribe una serie de datos desde el buffer DATA */
		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
			I2C2TRN = *(DATA+i);

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */

			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C2STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY3;

			break;

		case I2C_BUSY3: /* Transmiting */
			break;

		case I2C_STOP: /* Send a stop condition */
			StopI2C2();
			*STA=I2C_BUSY_STOP;
			break;

		case I2C_BUSY_STOP:
			break;

		case I2C_IDLE: /* Wait state */
	///* DEBUG INST */button=0;
			break;
	}
}

/*---------------------------------
 *	I2C WRITE NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C2WriteNextState(unsigned int *STA, unsigned int n, unsigned int *i)
{
	if(IFS3bits.MI2C2IF)
	{
		/* TRANSICION CON INTERRUPCION */
		switch(*STA)
		{
			case I2C_BUSY_START: /* Start condition ok */
				*STA=I2C_WRITE1;
				break;

			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA=I2C_WRITE2;
				else
					*STA=I2C_STOP;
				break;

			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/
                                    *STA=I2C_WRITE3;
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;

			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Se aumenta el puntero al dato */
				(*i)++;
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/

                                    /* ------------
                                     * Se permite escritura de página volviendo al estado WRITE3 hasta que
                                     * se enviaron todos los bytes deseados (máximo 16)
                                     * ----------- */
                                    if((*i)<n && (*i)<=15){
                                        *STA=I2C_WRITE3;
                                    }
                                    else{
                                        *STA=I2C_STOP;
                                    }
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;

			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C2_Clear_Intr_Status_Bit;
	}
}


/*---------------------------------
 *		I2C READ SATATE MACHINE
 *---------------------------------*/
static void I2C2Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR)
{
	switch(*STA_R)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C2();
			StartI2C2();
			*STA_R=I2C_BUSY_START;
			break;

		case I2C_BUSY_START:
			break;

		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
			I2C2TRN = *ID_R; /* ID */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
                            *STA_R=I2C_STOP;
                            I2C2STATbits.IWCOL=0;
   			}
                	else{
                            *STA_R=I2C_BUSY1;
                        }
			break;

		case I2C_BUSY1: /* Transmiting */
			break;

		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
				I2C2TRN = *(ID_R+1); /* WORD ADDRESS */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C2STATbits.IWCOL=0;
   			}
	    	else
	    		*STA_R=I2C_BUSY2;

			break;

		case I2C_BUSY2: /* Transmiting */
			break;

		case I2C_START2: /* Can send a start bit */
			if(!I2C2CONbits.RSEN)
			{
				I2C2CONbits.RSEN=1;//RestartI2C2();
				*STA_R=I2C_BUSY_START2;
			}
			break;

		case I2C_BUSY_START2:
			break;

		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C2STATbits.TBF);
				I2C2TRN = *(ID_R+2); /*|0x01 aseguro enviar siempre comando de lectura */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C2STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C2STATbits.IWCOL=0;
   			}
	    	else
	    		*STA_R=I2C_BUSY3;

			break;

		case I2C_BUSY3: /* Transmiting */
			break;

		case I2C_READ:
			if(!I2C2CONbits.RCEN)
				I2C2CONbits.RCEN = 1; /* Start read secuence */
			*STA_R=I2C_BUSY4;
			break;

		case I2C_BUSY4:
			break; /* Espero que lleguen los datos */

		case I2C_MODE_ACK:
			/* Se lee el i-esimo dato */
			*(DATA_R+iR)=I2C2RCV;
			I2C2STATbits.I2COV = 0;
			if((iR+1) < NR)
				AckI2C2(); /* ACK => Read next data byte */
			else
				NotAckI2C2(); /* No more data, THEN NACK AND GO TO STOP */
			*STA_R=I2C_BUSY_ACK;
			break;

		case I2C_BUSY_ACK:
			break;

		case I2C_STOP: /* Send a stop condition */
			StopI2C2();
			*STA_R=I2C_BUSY_STOP;
			break;

		case I2C_BUSY_STOP:
			break;

		case I2C_IDLE: /* Wait state */
			break;
	}
}

/*---------------------------------
 *	I2C READ NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C2ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE)
{
	if(IFS3bits.MI2C2IF)
	{
		/* TRANSICION CON INTERRUPCION */
		switch(*STA_R1)
		{
			case I2C_BUSY_START: /* Start condition ok */
				if(MODE==0)
					*STA_R1=I2C_WRITE1;
				else
					*STA_R1=I2C_WRITE3;
				break;

			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_WRITE2;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_START2;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY_START2:
				*STA_R1=I2C_WRITE3;
				break;

			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C2STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY4://READ
				*STA_R1=I2C_MODE_ACK;
				break;

			case I2C_BUSY_ACK:
				(*iR1)++;
				if((*iR1) < NR1)
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA_R1=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C2_Clear_Intr_Status_Bit;
	}
}

//******************************************************************************
//******************************************************************************

static void I2C3Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i);
static void I2C3WriteNextState(unsigned int *STA1, unsigned int n1, unsigned int *i1);
static void I2C3Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR);
static void I2C3ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE);
/*------------------------------------------------------------------------------------------
 *		 							CONFIG I2C
 *------------------------------------------------------------------------------------------
 * Function Prototype : void ConfigI2C(unsigned int BRG)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Confiugures I2C periferal to operate at user defined frecuency
 *						and disable interrupt
 * Arguments          : BRG - Computed value for the baud rate generator
 * Return Value      :  None
 * Remarks           :  This function configures the I2C Control register, I2C
                     	Baud Rate Generator register and disable interrupts
 *----------------------------------------------------------------------------------------*/
 void ConfigI2C3(unsigned int BRG)
 {
	/* I2C Clock Rates:
	 *
	 * Req.F  -  FCY  - BRG
	 * 100kHz - 16MHz - 157
	 * 100kHz - 08MHz - 78
	 * 400kHz - 16MHz - 37
	 * 001MHz - 16MHz - 13
	 */

 	//ConfigIntI2C3(MI2C_INT_ON & MI2C_INT_PRI_6 & SI2C_INT_ON & SI2C_INT_PRI_5); /* Interrupt mode state trantition */
	ConfigIntI2C3(MI2C_INT_OFF & MI2C_INT_PRI_0 & SI2C_INT_OFF & SI2C_INT_PRI_0); /* Software mode state trantition */

	/* OPEN I2C OPTION EXPLANATION
	 *
	 * I2C_IPMI_DIS/EN: Intelligent Perih Mang. Interface to accept all adresses (DIS normally)
	 * I2C_SLW_DIS/EN: Slew Rate required at 400kHz only
	 * I2C_SM_DIS/EN: SMBus voltage specifications (DIS normally)
	 * I2C_GCALL_DIS/EN: When generall call is used, all enabled devices should respond with an Acknowledge
	 * NOTE: I2C module operates as Master and Slave simultaneously
	 */
	OpenI2C3(I2C_ON & I2C_IDLE_CON & I2C_CLK_REL & I2C_IPMI_DIS & I2C_7BIT_ADD & I2C_SLW_DIS & I2C_SM_DIS & I2C_GCALL_DIS & I2C_STR_DIS &
			I2C_ACK & I2C_ACK_DIS & I2C_RCV_DIS & I2C_STOP_DIS & I2C_RESTART_DIS & I2C_START_DIS,
			BRG) ; /* BRG according to I2C Clock Rate table*/

}
/*------------------------------------------------------------------------------------------
 *		 			I2C SEND CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2CSendChar(unsigned char ID, unsigned char AD, unsigned char DA)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Writes a data byte into an I2C EEPROM memory (as RAM)
 * Arguments          : ID - This contains the indentifier to select correpond I2C disp.
 *			AD - This contains the addres of the EERPOM will be write
 *			DATA - This contains the data byte will be write in the EERPOM
 *			N - The number of bytes to be read from *DATA an send
 * Return Value      :  None
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C3SendChar(unsigned char ID, unsigned char AD, unsigned char *DATA, unsigned int N)
{
	unsigned char Id_add[2];
	unsigned int State;
	unsigned int i;

	Id_add[0]=ID; Id_add[1]=AD;
	State=I2C_START;
	i=0;

	/* Return when transmission is completed */
        int max=0;
	while(State!=I2C_IDLE)
	{
		I2C3Write(&State, Id_add, DATA, i);
		I2C3WriteNextState(&State,N,&i);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}
}
/*------------------------------------------------------------------------------------------
 *		 							I2C READ CHAR
 *------------------------------------------------------------------------------------------
 * Function Prototype : void I2C3ReadChar(unsigned char IDW, unsigned char AD,
 *                      unsigned char IDR, unsigned char *BUFF, unsigned int N,
 *                      unsigned char MODE)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Reads a data byte by byte from I2C and stores data in the
 *                      specified buffer
 * Arguments          : IDW - This contains the indentifier to select correpond I2C disp. (write)
 *						AD  - This contains the addres of the EERPOM will be read.
 *						IDR - This contains the indentifier to select correpond I2C disp. (read)
 *						N	- Number of bytes to read
 *						MODE- when MODE=0 send the IDW, AD and IDR, and when MODE=1 send just IDR
 *                      BUFF - Contais the byte read from EEPROM
 * Return Value      :  none
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
void I2C3ReadChar(unsigned char IDW, unsigned char AD, unsigned char IDR, unsigned char *BUFF, unsigned int N, unsigned char MODE)
{
	unsigned char DataR[3];
	unsigned int StateR;
	unsigned int iR;

	DataR[0]=IDW; DataR[1]=AD; DataR[2]=IDR;
	StateR=I2C_START;
	iR=0;

	/* Return when transmission is completed */
        int max=0;
	while(StateR!=I2C_IDLE)
	{
		I2C3Read(&StateR, DataR, BUFF, N, iR);
		I2C3ReadNextState(&StateR,&iR,N,MODE);
                max++;
                if(max==0x0FFF){break;} //evita que el pic se cuelgue
	}
}
/*------------------------------------------------------------------------------------------
 *		 							I2C SLAVE READY
 *------------------------------------------------------------------------------------------
 * Function Prototype : unsigned int I2C3SlaveReady(unsigned char ID)
 * Include            : i2c.h - i2c_suchai.h
 * Description        : Return 0 if Slave is busy (No ACK). Send a START condition and
 * Arguments          : None
 * Return Value      :  1-> Slave send ACK, so is ready. 0-> Slave don't send ACK, not ready.
 * Remarks           :  Designed for a 24LC04B 4Kb I2C Serial EEPROM
 *----------------------------------------------------------------------------------------*/
unsigned int I2C3SlaveReady(unsigned char ID)
{
	unsigned int val;
	IdleI2C3();
	StartI2C3();
	/* Esperar se complete secuencia START */
	while(!IFS5bits.MI2C3IF);
	MI2C3_Clear_Intr_Status_Bit;

	/* Esperar se desocupe registro TBF */
	while(I2C3STATbits.TBF);

	/* Tratar de enviar ID */
	I2C3TRN = ID;

	/* If write collision occurs,STOP */
	if(I2C3STATbits.IWCOL)
	{
   		I2C3STATbits.IWCOL=0;
   		/* Falló la comunicación */
   		val = 0;
	}
	else
	{
		/* Esperar a que termine la escritura */
		while(!IFS5bits.MI2C3IF);
		MI2C3_Clear_Intr_Status_Bit;

		/* Revisar ACK */
		/*ACKSTAT=0 -> ACK OK*/
		if(!I2C3STATbits.ACKSTAT)
			/* Dispositivo Preparado */
			val = 1;
		else
			/* Dispositivo no preparado */
			val = 0;
	}
	StopI2C3();
	/* Esperar a que se mande STOP*/
	while(!IFS5bits.MI2C3IF);
	MI2C3_Clear_Intr_Status_Bit;

	return val;
}

/*---------------------------------
 *		I2C WRITE SATATE MACHINE
 *---------------------------------*/
static void I2C3Write(unsigned int *STA, unsigned char *ID_ADD, unsigned char *DATA, unsigned int i)
{
	switch(*STA)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C3();
			StartI2C3();
			*STA=I2C_BUSY_START;
			break;

		case I2C_BUSY_START:
			break;

		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
			I2C3TRN = *ID_ADD;

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C3STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY1;
			break;

		case I2C_BUSY1: /* Transmiting */
			break;

		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
			I2C3TRN = *(ID_ADD+1);

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C3STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY2;

			break;

		case I2C_BUSY2: /* Transmiting */
			break;

		/* Este estado escribe una serie de datos desde el buffer DATA */
		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
			I2C3TRN = *(DATA+i);

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */

			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA=I2C_STOP;
        		I2C3STATbits.IWCOL=0;
   			}
	    	else
	    		*STA=I2C_BUSY3;

			break;

		case I2C_BUSY3: /* Transmiting */
			break;

		case I2C_STOP: /* Send a stop condition */
			StopI2C3();
			*STA=I2C_BUSY_STOP;
			break;

		case I2C_BUSY_STOP:
			break;

		case I2C_IDLE: /* Wait state */
	///* DEBUG INST */button=0;
			break;
	}
}

/*---------------------------------
 *	I2C WRITE NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C3WriteNextState(unsigned int *STA, unsigned int n, unsigned int *i)
{
	if(IFS5bits.MI2C3IF)
	{
		/* TRANSICION CON INTERRUPCION */
		switch(*STA)
		{
			case I2C_BUSY_START: /* Start condition ok */
				*STA=I2C_WRITE1;
				break;

			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA=I2C_WRITE2;
				else
					*STA=I2C_STOP;
				break;

			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/
                                    *STA=I2C_WRITE3;
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;

			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Se aumenta el puntero al dato */
				(*i)++;
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT){ /*ACKSTAT=0 -> ACK OK*/

                                    /* ------------
                                     * Se permite escritura de página volviendo al estado WRITE3 hasta que
                                     * se enviaron todos los bytes deseados (máximo 16)
                                     * ----------- */
                                    if((*i)<n && (*i)<=15){
                                        *STA=I2C_WRITE3;
                                    }
                                    else{
                                        *STA=I2C_STOP;
                                    }
                                }
                                else{
                                    *STA=I2C_STOP;
                                }
				break;

			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C3_Clear_Intr_Status_Bit;
	}
}


/*---------------------------------
 *		I2C READ SATATE MACHINE
 *---------------------------------*/
static void I2C3Read(unsigned int *STA_R, unsigned char *ID_R, unsigned char *DATA_R, unsigned int NR, unsigned int iR)
{
	switch(*STA_R)
	{
		case I2C_START: /* Can send a start bit */
			IdleI2C3();
			StartI2C3();
			*STA_R=I2C_BUSY_START;
			break;

		case I2C_BUSY_START:
			break;

		case I2C_WRITE1: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
			I2C3TRN = *ID_R; /* ID */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
                            *STA_R=I2C_STOP;
                            I2C3STATbits.IWCOL=0;
   			}
                	else{
                            *STA_R=I2C_BUSY1;
                        }
			break;

		case I2C_BUSY1: /* Transmiting */
			break;

		case I2C_WRITE2: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
				I2C3TRN = *(ID_R+1); /* WORD ADDRESS */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C3STATbits.IWCOL=0;
   			}
	    	else
	    		*STA_R=I2C_BUSY2;

			break;

		case I2C_BUSY2: /* Transmiting */
			break;

		case I2C_START2: /* Can send a start bit */
			if(!I2C3CONbits.RSEN)
			{
				I2C3CONbits.RSEN=1;//RestartI2C3();
				*STA_R=I2C_BUSY_START2;
			}
			break;

		case I2C_BUSY_START2:
			break;

		case I2C_WRITE3: /* Transmiting => can send a data byte */
			while(I2C3STATbits.TBF);
				I2C3TRN = *(ID_R+2); /*|0x01 aseguro enviar siempre comando de lectura */

			/* SE CAMBIA INMEDIATAMENTE DE ESTADO */
			if(I2C3STATbits.IWCOL)        /* If write collision occurs,STOP */
			{
        		*STA_R=I2C_STOP;
        		I2C3STATbits.IWCOL=0;
   			}
	    	else
	    		*STA_R=I2C_BUSY3;

			break;

		case I2C_BUSY3: /* Transmiting */
			break;

		case I2C_READ:
			if(!I2C3CONbits.RCEN)
				I2C3CONbits.RCEN = 1; /* Start read secuence */
			*STA_R=I2C_BUSY4;
			break;

		case I2C_BUSY4:
			break; /* Espero que lleguen los datos */

		case I2C_MODE_ACK:
			/* Se lee el i-esimo dato */
			*(DATA_R+iR)=I2C3RCV;
			I2C3STATbits.I2COV = 0;
			if((iR+1) < NR)
				AckI2C3(); /* ACK => Read next data byte */
			else
				NotAckI2C3(); /* No more data, THEN NACK AND GO TO STOP */
			*STA_R=I2C_BUSY_ACK;
			break;

		case I2C_BUSY_ACK:
			break;

		case I2C_STOP: /* Send a stop condition */
			StopI2C3();
			*STA_R=I2C_BUSY_STOP;
			break;

		case I2C_BUSY_STOP:
			break;

		case I2C_IDLE: /* Wait state */
			break;
	}
}

/*---------------------------------
 *	I2C READ NEXT SATATE MACHINE
 *---------------------------------*/
static void I2C3ReadNextState(unsigned int *STA_R1, unsigned int *iR1, unsigned int NR1, unsigned char MODE)
{
	if(IFS5bits.MI2C3IF)
	{
		/* TRANSICION CON INTERRUPCION */
		switch(*STA_R1)
		{
			case I2C_BUSY_START: /* Start condition ok */
				if(MODE==0)
					*STA_R1=I2C_WRITE1;
				else
					*STA_R1=I2C_WRITE3;
				break;

			case I2C_BUSY1: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_WRITE2;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY2: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_START2;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY_START2:
				*STA_R1=I2C_WRITE3;
				break;

			case I2C_BUSY3: /* Se enviaron 8 bits + ack */
				/* Revisar ACK */
				if(!I2C3STATbits.ACKSTAT) /*ACKSTAT=0 -> ACK OK*/
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY4://READ
				*STA_R1=I2C_MODE_ACK;
				break;

			case I2C_BUSY_ACK:
				(*iR1)++;
				if((*iR1) < NR1)
					*STA_R1=I2C_READ;
				else
					*STA_R1=I2C_STOP;
				break;

			case I2C_BUSY_STOP: /* Se ha enviado Stop Cond */
				*STA_R1=I2C_IDLE; /* GO TO WAIT STATE */
				break;
		}
		MI2C3_Clear_Intr_Status_Bit;
	}
}

