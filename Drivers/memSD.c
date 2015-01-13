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

#include "memSD.h"	//uso mis definiciones SPI_2_transfer(), ReadSPI() y clockSPI()

#define SD_MB_MODE 1
//modo para lograr que la seÒÒal de SCK llegue a la memSD, a pesar de que nSS=1
//En la MB de Pumpkins se peude solo si nSS=0, producto del buffer intermedio

void msd_blockErase(unsigned long block_address){
    #if (SCH_DATAREPOSITORY_VERBOSE>=2)
        static int ind; char ret[10]; unsigned int ib;
        ind++;
        if(ind==1){ib=block_address;}
        if( (ind%32)==0 ){
            con_printf("    dat_memSD_BlockErase\r\n");
            con_printf("    erasing also.. block=");
            sprintf (ret, "%d", (unsigned int)block_address);
            con_printf(ret); con_printf("\r\n");
        }
        if(ind==256){
            con_printf("    Sucessfully erased 256Block (starting in block ");
            sprintf (ret, "%d", (unsigned int)ib);
            con_printf(ret); con_printf(")\r\n");
            ind=0;
        }
    #endif

    unsigned char buff[512]; int i;
    for(i=0;i<512;i++){buff[i]=0xFF;}
    unsigned char resp;

    resp = Single_Block_Write(block_address, buff);
}

//****************************************************************************
BOOL msd_setVar_256BlockExtMem(unsigned long block_i, unsigned int indx, int value){
    // MAX(indx)=2^16 => 2^16 = 65536 variables por setear. Si hay 256 variables por bloque, entonces
    // habr· que tener (2^16)/256=256 bloques para poder guardar 2^16 variables.

    unsigned int i; unsigned int indx2; BOOL resp;
    for(i=0;i<256;i++){
        indx2=indx-(256*i);
        if(indx2<256){
            resp = msd_setVar_1BlockExtMem(block_i+i, (int)indx2, value);
        }
    }

    return resp;
}
 BOOL msd_getVar_256BlockExtMem(unsigned long block_i, unsigned int indx, int *value){
    // MAX(indx)=2^16 => 2^16 = 65536 variables por setear. Si hay 256 variables por bloque, entonces
    // habr· que tener (2^16)/256=256 bloques para poder guardar 2^16 variables.

    unsigned int i; unsigned int indx2; BOOL resp;
    for(i=0;i<256;i++){
        indx2=indx-(256*i);
        if(indx2<256){
            resp = msd_getVar_1BlockExtMem(block_i+i, (unsigned char)indx2, value);
        }
    }

    return resp;
}
BOOL msd_setVar_1BlockExtMem(MSD_ARG block, unsigned char  indx, int value){
    //printf("msd_setVar_1BlockExtMem\n");
    unsigned char buff[512], resp;

    resp = Single_Block_Read(block, buff);
    if ( resp!=MSD_START_BLOCK_TOKEN ){
        #if SCH_MEMSD_VERBOSE
            printf("SingBlockRead !=0xFE\n");
        #endif
        return FALSE;
    }

    buff[indx]=(unsigned char)(value);             //LSByte
    buff[511-indx]=(unsigned char)(value>>8);      //MSByte

    resp = Single_Block_Write(block, buff);
    if(resp!=MSD_DATA_ACCEPTED_TOKEN){
        #if SCH_MEMSD_VERBOSE
            printf("SingBlockWrite !=0x05\n");
        #endif
        return FALSE;
    }
    else{
        return TRUE;
    }
}
BOOL msd_getVar_1BlockExtMem(unsigned long block, unsigned char indx, int *value){
    //printf("msd_getVar_1BlockExtMem\n");
    unsigned char buff[512]; int val=0;

    if ( Single_Block_Read(block, buff)!=MSD_START_BLOCK_TOKEN ){
        #if SCH_MEMSD_VERBOSE
            printf("SingBlockRead !=0xFE\n");
        #endif
        return FALSE;
    }

    val=buff[511-indx];                        //MSByte
    val=val<<8;
    val=(val | (unsigned int)buff[indx] );     //LSByte
    *value=val;

    return TRUE;
}

//************************************************************************************************************
//************************************************************************************************************
//************************************************************************************************************

MSD_COMMAND create_COMMAND(unsigned char c, MSD_ARG a,unsigned char e){
	MSD_COMMAND cmd;
	cmd.command=(c|0x40);		// 0x40=0100 0000 <= b7=0(Start bit), b6=1(tramsition bit), b5..b0 (command)
	cmd.argument=a;
	cmd.crc=(e|0x01);			//b0=1 (end bit)
	return cmd;
}	
void send_COMMAND(MSD_COMMAND CMDx){

    #if (SD_MB_MODE==1)
        int del,k,l;
        for(del=0;del<0x0FFF;del++){k=del*1;}
        l=k;
        //Ya que en la MB de pumpkins, al hacer nSS=0 se desconecta el buffer
        //y no llegan mas seÒales hacia la tarjeta. Por lo que habre que hacer
        //~SS=0 para permitir que los pulsos lleguen a la SD
        SPI_nSS_2=0;
        //algunas tarjetas necesitan ciclos extra de reloja para
        //terminar de procesar el cmd anterior
        MSD_clockSPI();     
        MSD_clockSPI();
        MSD_clockSPI();
        SPI_nSS_2=1;
        for(del=0;del<0x0FFF;del++){k=del*1;}
        l=k;
    #else
        MSD_clockSPI();     //algunas tarjetas necesitan ciclos extra de reloja para terminar de procesar el cmd anterior
        MSD_clockSPI();     //algunas tarjetas necesitan ciclos extra de reloja para terminar de procesar el cmd anterior
        MSD_clockSPI();     //algunas tarjetas necesitan ciclos extra de reloja para terminar de procesar el cmd anterior
    #endif

    MSD_enableSD();				//selecciono el esclavo ~SS=0
    unsigned char com=CMDx.command;		// 1 byte command
    MSD_ARG arg=CMDx.argument;			// 4 bytes argument
    unsigned char cyclic_rc=CMDx.crc;		// 1 byte crc (Cyclic Redundancy Check)
    SPI_2_transfer(com);
    SPI_2_transfer(arg>>24);			//MSbyte de ARG address
    SPI_2_transfer(arg>>16);
    SPI_2_transfer(arg>>8);
    SPI_2_transfer(arg);			//LSbyte de ARG address
    SPI_2_transfer(cyclic_rc);
    
}
unsigned char SD_init2(void){
    char ret[6]; ret[0]=0;ret[1]=0;ret[2]=0;ret[3]=0;ret[4]=0;ret[5]=0;

    MSD_COMMAND CMD0 = create_COMMAND(0x00,0x00000000,0x95);	// solo para este comando importa un crc valido (0x95)
    MSD_COMMAND ACMD41= create_COMMAND(0x29,0x40000000,0x95);	// initialize card, and send Host Capcity Support=1
    MSD_COMMAND CMD55= create_COMMAND(0x37,0x00000000,0x95);	// next command shall be interpreted as Aplication-Command

    unsigned char r; int i;
    for(i=0;i<10;i++){		// mientras ~SS=1, emito 80 pulsos por SCK para el start-up
            MSD_disableSD();		// ~SS=1 y clockSPI() envia 8 pulsos
    }

    send_COMMAND(CMD0);		//~SS=0 and then send a command to enter SPI mode
    r=get_R1_response();
    SPI_nSS_2=1;					//~SS=1

    #if (SCH_MEMSD_VERBOSE>=2)
        printf("SD_init2()\r\n");
        Hex8ToAscii( r, ret); printf("R1_response(CMD0)="); printf(ret); printf("\n");
    #endif
    if(r!=0x01){return r;} 	// r=0x01=> command received, card in "idle" after "reset". Si la SD no paso a "idle"
    MSD_ARG r7=0;
    r7=send_CMD8();			//Send CMD8 and get the rseposne R7
                                            //indica los voltajes aceptados en la rutina de inicializacion de ls SD
                                            // Si tiene exito devuelve VOLTAGE_OK (es un define)
                                            //Si falla devuelve un tipo ARG con un unsigned char R1 en su byte inferior

    #if (SCH_MEMSD_VERBOSE>=2)
        Hex8ToAscii( r7, ret); printf("R7_response(CMD8)="); printf(ret); printf("\n");
    #endif
    if(r7!=MSD_VOLTAGE_OK){return r=(unsigned char)r7;}

    MSD_ARG r3=OCR_register();			// reads the Operating Condition Register (OCR). En una respuesta R3
                                                                    //Si tiene exito, devuelve 4bytes (type ARG) con la informacion de OCR
                                                                    //Si falla (error de comunicacion) devuelve R1 (queda en el byte inferior del return ARG)
    //if(r3=voltajes de OCR corresponden){sigo}
    //else{no sigo}

    i=10000;
    do{
            send_COMMAND(CMD55);
            r=get_R1_response();
            SPI_nSS_2=1;						//~SS=1
            if(r!=0x01) {break;}		//si hubo erroes en la transmision de CMD55
            send_COMMAND(ACMD41);
            r=get_R1_response();
            SPI_nSS_2=1;						//~SS=1
            if(r==0x00) break;
    }while(--i>0);					//intenta 10.000 que la SD pase de estado "idle" al "ready" (luego de salir del "reset")

    if(r!=0x00) return r; 			//SD time-out

    r3=OCR_register();				//vuelvo a leer OCr, para conocer CCS (1=>High Capacity SD  0=>Standard Capacity SD)
    //if(r3&0xC0000000){card is High Capacity}
    //else{card is Standard Capacity}

    return r;
}

unsigned int SD_init_memSD(void){
    //apagar energia MemSD
    PPC_MB_nON_SD=1;
    /* Un delay para poder inicializar conrrectamente la SD si el PIC se resetea */
//    unsigned long i;
//    for(i = 0x004FFFFF; i>0; i--){}
    __delay_ms(3000);
    //encender energia MemSD
    PPC_MB_nON_SD=0;
    unsigned char r = SD_init();
    if(r == 0){
        return 1;
    }
    else{
        return 0;
    }
}

unsigned char SD_init(void){
	MSD_COMMAND CMD0 = create_COMMAND(0x00,0x00000000,0x95);	// solo para este comando importa un crc valido (0x95)
	MSD_COMMAND CMD1 = create_COMMAND(0x01,0x40000000,0x95);	// initialize card, and send Host Capcity Support
								// (HCS,b30=1)=> host soporta High Capacity SD
        
	int i;
	unsigned char r;
	for(i=0;i<10;i++){		// mientras ~SS=1, emito 80 pulsos por SCK para el start-up
            MSD_disableSD();		// ~SS=1 y clockSPI() envia 8 pulsos
	}
	
	send_COMMAND(CMD0);		//~SS=0 and then send a command to enter SPI mode
	r=get_R1_response();
	SPI_nSS_2=1;			//~SS=1

        #if (SCH_MEMSD_VERBOSE>=2)
            Hex8ToAscii( r, ret); printf("R1_response(CMD0)="); printf(ret); printf("\n");
        #endif
	if(r!=0x01) return r; 	// r=0x01=> command received, card in "idle" after "reset". Si la SD no paso a "idle"
	
	i=10000;	
	do{
		send_COMMAND(CMD1);
		r=get_R1_response();
		SPI_nSS_2=1;			//~SS=1
		if(r==0x00) break;
	}while(--i>0);			//intenta 10.000 que la SD pase de estado "idle" al "ready" (luego de salir del "reset")

        #if (SCH_MEMSD_VERBOSE>=2)
            Hex8ToAscii( r, ret); printf("R1_response(CMD1)="); printf(ret); printf("\n");
        #endif
	if(r!=0x00) return r; 	//SD time-out

        //card is ready now


	/*
        CloseSPI2(); 	//deshabilito modulo e interrupcion
	unsigned int spi2con1, spi2con2, spi2stat;
	spi2con1=ENABLE_SCK_PIN&ENABLE_SDO_PIN&SPI_MODE16_OFF&SPI_SMP_OFF&SPI_CKE_ON&SLAVE_ENABLE_OFF&CLK_POL_ACTIVE_HIGH&
	MASTER_ENABLE_ON&SEC_PRESCAL_2_1&PRI_PRESCAL_1_1 ;//SPIxCON1
	spi2con2=FRAME_ENABLE_OFF&FRAME_SYNC_OUTPUT&FRAME_POL_ACTIVE_LOW&FRAME_SYNC_EDGE_PRECEDE&FIFO_BUFFER_DISABLE ; //SPIxCON2
	spi2stat=SPI_ENABLE&SPI_IDLE_CON&SPI_RX_OVFLOW_CLR ; //SPIxSTAT
	OpenSPI2(spi2con1, spi2con2, spi2stat);//aumento velocidad de 250Khz (16Mhz/64) a 8Mhz (16Mhz/2)
        */
        

	return r;	//si r=0x00 =>SD entro a estado "ready"
	/*
	0xFF	=> time-out. No answer
	0x01	=> command received. Card in idle state (after reset)
	0x00	=> command accepted
	*/
}
unsigned char Set_Block_Length(MSD_ARG l){
	MSD_COMMAND CMD16= create_COMMAND(0x10,0x00000200,0x95);	// block lenght -for read, write and LOCK_UNLOCK command-
															// is 512 byte (default and standard for hard disks)
	CMD16.argument=l;
	send_COMMAND(CMD16);
	unsigned int r=get_R1_response();
	SPI_nSS_2=1;			//~SS=1
	return r;
}
MSD_ARG send_CMD8(void){
	MSD_COMMAND CMD8 = create_COMMAND(0x08,0x000001AA,0x87);	// suplyy voltaje is 2.7-3.6 V=>0001 and Check Pattern is 0xAA
	send_COMMAND(CMD8);
	
	//response to CMD8 is R7
	unsigned char r1;
	MSD_ARG resp, r[4];
	
	r1=get_R1_response();
	
	int i,j=0;
        for(i=0;i<4;i++){r[i]=0;}
        i=0;
	while(j<4){
		i=19;
		do{
			r[j]=MSD_readSPI();
			if(r[j]!=0xFF)break;			//veo 9 veces si es que hay respuesta de la tarjeta SD
		}while(--i>0);
		j++;
	}
	resp=(r[0]<<24)|(r[1]<<16)|(r[2]<<8)|(r[3]);			//resp= r[0][31:24] | r[1][23:16] | r[2][15:8] | r[3][7:0]
	
	SPI_nSS_2=1;			//~SS=1

        #if (SCH_MEMSD_VERBOSE>=2)
        printf("Resp a CMD8 es:\n");
            char ret[6];
            Hex8ToAscii( r1, ret); printf("r1="); printf(ret); printf("\n");
            Hex8ToAscii( r[0], ret); printf("r[0]="); printf(ret); printf("\n");
            Hex8ToAscii( r[1], ret); printf("r[1]="); printf(ret); printf("\n");
            Hex8ToAscii( r[2], ret); printf("r[2]="); printf(ret); printf("\n");
            Hex8ToAscii( r[3], ret); printf("r[3]="); printf(ret); printf("\n");
        #endif

	resp=resp&0x00000FFF;		//resp deberia ser: xxxxxxxx xxxxxxxx xxxx0001 10101010	=> resp deberia ser: 0x000001AA
	if( (resp!=0x000001AA) | (r1!=0x01) ){
            #if (SCH_MEMSD_VERBOSE>=2)
                printf("resp a CMD8 es r1\n");
            #endif
            return (resp=r1);}	//si hay errores devuelve r1
	else return resp;
	/*	
	5 bytes. de respuesta [byte1, byte2, byte3, byte4, byte5]
	byte1: R1 response
	byte2: command version
	byte3: reserved bits (commonly 0 )
	byte4: voltage accepted
	byte5: 'echo-back' check pattern (yo us√© 0xAA)
	
	la funcion devuelve [byte2, byte3, byte4, byte5]
	*/
}
//--------------------------------------------------------------------------------------------------------------------
MSD_CARD_ID CID_register(void){
	MSD_COMMAND CMD10= create_COMMAND(0x0A,0x00000000,0x95);	// ask the card to send its Card Identification (CID)
	send_COMMAND(CMD10);
	MSD_CARD_ID cid_reg;
	
	get_R1_response();
	
	int i=0; unsigned char dat[16];
	for(i=0;i<16;i++){
		dat[i]=MSD_readSPI();
	}
	cid_reg.mid=dat[0];
	cid_reg.oid[0]=dat[1];		
	cid_reg.oid[1]=dat[2];
	cid_reg.oid[2]='\0';						// '\0'=0=NULL => fin del String
	cid_reg.pnm[0]=dat[3];
	cid_reg.pnm[1]=dat[4];
	cid_reg.pnm[2]=dat[5];
	cid_reg.pnm[3]=dat[6];
	cid_reg.pnm[4]=dat[7];
	cid_reg.pnm[5]='\0';						// '\0'=0=NULL => fin del String
	int a,b; a=(dat[8]>>4); b=(dat[8]&0x0F);
	cid_reg.prv=(a*10+b)/10;
	cid_reg.psn=dat[12]; 
	cid_reg.psn=(cid_reg.psn<<8)|(dat[11]);
	cid_reg.psn=(cid_reg.psn<<16)|(dat[10]);
	cid_reg.psn=(cid_reg.psn<<24)|(dat[9]);		//cid_reg.psn=dat[12],dat[11],dat[10],dat[9];
	cid_reg.month=(dat[13]&0xF0);
	cid_reg.year=dat[14];
	
	SPI_nSS_2=1;			//~SS=1
	return cid_reg;
	/*
	cid_reg.mid		=> numero identificador del fabricante (8-bits)
	cid_reg.oid[3]	=> (2-character String) parce que identifica el proposito del contenido de la tarjeta SD
	cid_reg.pnm[5]	=> product name (5-character String)
	cid_reg.prv		=> product revision (version "x.y")
	cid_reg.psn		=> product serial number (32bits)
	cid_reg.year	=> year of manufacturing 
	cid_reg.month	=> month of manufacturing (january=1)
*/
}
MSD_ARG OCR_register(void){
	MSD_COMMAND CMD58= create_COMMAND(0x3A,0x00000000,0x95);	// reads the Operating Condition Register (OCR).
															// The accepted voltaje range for Vdd, OCR[15...23]
															// Card Capacity Status bit (CCS) is OCR[30]
															// Power-up Routine bit (busy) is OCR[31]. 
	send_COMMAND(CMD58);
	
	unsigned char r1; 
	MSD_ARG resp,r[4];
	
	r1=get_R1_response();	
	
	int i,j=0;
	while(j<4){
		i=9;
		do{
			r[j]=MSD_readSPI();
			if(r[j]!=0xFF)break;					//veo 9 veces si es que hay respuesta de la tarjeta SD
		}while(--i>0);
		j++;
	}
	resp=(r[0]<<24)|(r[1]<<16)|(r[2]<<8)|(r[3]);	//resp= r[0][31:24] | r[1][23:16] | r[2][15:8] | r[3][7:0]
	
	SPI_nSS_2=1;			//~SS=1
	if(r1!=0x00) return (resp=r1);					//si hay errores devuelve r1
	else return resp;
	/*
	5 bytes de respuesta, R1=1byte y R3=4bytes
	b0...b6	= reserved
	b7		= reserved for low voltaje range
	b8...14	= reserved
	b15		= 2,7-2,8
	b16		= 2,8-2,9
	b17		= 2,9-3,0
	b18		= 3,0-3,1
	b19		= 3,1-3,2
	b20		= 3,2-3,3
	b21		= 3,3-3,4
	b22		= 3,4-3,5
	b23		= 3,5-3,6
	b24...29= reserved	
	b30		= Card Capacity Status (CCS)		Valido solo cuando busy=1
	b31		= Card power-up status bit (busy)	Seteado a 1 cuando el proceso de power-up esta completo
	la funcion solo devuelve R3
	*/
}
//--------------------------------------------------------------------------------------------------------------------
unsigned int STATUS_register(void){
	MSD_COMMAND CMD13= create_COMMAND(0x0D,0x00000000,0x95);	// ask the card to send its Status Register
	send_COMMAND(CMD13);

	unsigned int r1,r2,r3;

	r1=get_R1_response();
	
	int i=9;
	do{
		r2=MSD_readSPI();
		if(r2!=0xFF)break;	//veo 9 veces si es que hay respuesta de la tarjeta SD
	}while(--i>0);
	
	r3=(r1<<8)| r2;			//r3= r1[15:8] | r2[7:0]
	
	SPI_nSS_2=1;			//~SS=1
	if(r1!=0x00) return r1;		//si hay errores devuelve r1
	else return r3;
	/*
	R2 response format	
	b0: Card is locked		=> Set when the card is locked by the user. Reset when is unlocked
	b1: wp erase skip | lock/unlock cmd failed		=> It is set when the host attempts to erase a write-protceted block or
													   makes a sequence OR password errors during card lock/unlock operation
	b2:	error				=> A general or an unknown error ocurred during the operation
	b3:	CC error			=> Interal Card Controller error
	b4:	card ECC failed                 => Card internal ECC was applied but failed to correct the data
	b5:	wp violation                    => The command tried to write a write-protected block
	b6:	erase param			=> An invalid selection for erase, block or groups
	b7:	out of range | csd overwrite				=>
	
	b8: in idle state		=> the card is in idle state and running the initializing process
	b9: erase reset			=> an erase sequence was cleared before executing because an out of erase sequence command was received
	b10: illegal command 		=> an illegal command code was detected
	b11: communication CRC error    => the CRC check of the last command failed
	b12: erase sequence error	=> an error in the sequence of erase commands occured
	b13: addres error		=> a misaligned address that did not match the block lenght (512bytes) was used in the command
	b14: parameter error		=> the command's argument (e.g. address, block lenght) was outside the allowed range for this card
	b15: 0				=> Always set to zero
	*/
}
int memSD_isAlive(void){
    MSD_CARD_ID cid = CID_register();
    int cid_mid = cid.mid;
    //printf("cid.mid = %d\r\n", cid.mid);    // siempre es 255
    //printf("cid.psn = %lu\r\n", cid.psn);   // siempre es 117440583

    MSD_ARG block = 0;
    unsigned char indx = 254;
    int value = 1235;
    int get_val;
    msd_getVar_1BlockExtMem(block, indx, &get_val);
    //printf("msd_getVar_1BlockExtMem(%lu, %d) = %d \r\n", (unsigned long)block, indx, get_val);
    msd_setVar_1BlockExtMem(block, indx, value);
    //printf("msd_setVar_1BlockExtMem(%lu, %d, %d) \r\n", (unsigned long)block, indx, value);
    msd_getVar_1BlockExtMem(block, indx, &get_val);
    //printf("msd_getVar_1BlockExtMem(%lu, %d) = %d \r\n", (unsigned long)block, indx, get_val);

    if(cid_mid == 255 && get_val == value){
        return 1;
    }
    return 0;
}
unsigned char get_R1b_response(void){		//response to CMD12, CMD28, CMD29, CMD38
	unsigned char r1,r2; 

	r1=get_R1_response();
	
	while(1){
		r2=MSD_readSPI();
		if(r2!=0x00) break;		//card is not busy any more
	}
	if(r1!=0x00) return r1;		//si hay errores devuelve r1
	else return r2;
}
unsigned char get_R1_response(void){
	unsigned char r;
	int i=9;
	do{
		r=MSD_readSPI();	//trunca al byte inferior de unsigned int readSPI()
		if(r!=0xFF) break;	//veo 9 veces si es que hay respuesta de la tarjeta SD
	}while(--i>0);
	return r;
	/*
	Si la tarjeta SD no responde (time-out)	devuelve 0xFF. Si no.
	
	R1 response format:
	b0: in idle state			=> the card is in idle state and running the initializing process
	b1: erase reset				=> an erase sequence was cleared before executing because an out of erase sequence command was received
	b2: illegal command 		=> an illegal command code was detected
	b3: communication CRC error => the CRC check of the last command failed
	b4: erase sequence error	=> an error in the sequence of erase commands occured
	b5: addres error			=> a misaligned address that did not match the block lenght (512bytes) was used in the command
	b6: parameter error			=> the command's argument (e.g. address, block lenght) was outside the allowed range for this card
	b7: 0						=> Always set to zero
	*/
}
unsigned char Single_Block_Read(MSD_ARG addr, unsigned char* buff){
	MSD_COMMAND CMD17= create_COMMAND(0x11,0x00000001,0x95);	// CMD17.argument=addres to be read
									// SD card respond sending a block the size of CMD16.argument
                                                                        // CMD16.argument es por default 512
	CMD17.argument=(addr<<9);	// hace que la unidad de medida de las direcciones
					// de memoria sean blockes (multiplos de 512bytes)
	int i; unsigned char r;
	send_COMMAND(CMD17);					//envio comando de lectura y ~SS=0
	r=get_R1_response();

        #if (SCH_MEMSD_VERBOSE>=2)
            char ret[6]; Hex8ToAscii( r, ret); printf("R1 Resp="); printf(ret); printf("\n");
        #endif
	if(r==0x00){						// Si r=0x00=>command accepted
		r=get_CMD17_Response_Token();
		if(r==MSD_START_BLOCK_TOKEN){			//si el token es positivo (no hay errores) se leer√°
			for(i=0; i<512; i++){
				*buff=MSD_readSPI();
				buff++;				//graba en el buffer los 512 bytes
			}
			MSD_readSPI();
			MSD_readSPI();				// get and then ignore the 2 CRC bytes
		}
	}
	MSD_disableSD();					// ~SS=1 y 8 pulsos por SCK

        #if (SCH_MEMSD_VERBOSE>=2)
            Hex8ToAscii( r, ret); printf("SingSectRead Resp="); printf(ret); printf("\n");
        #endif
	return r;						// START_BLOCK_TOKEN byte si funciono, DATA_ERROR_TOKEN byte si hay errores
}
unsigned char get_CMD17_Response_Token(void){	//get Start Block Token (SBT) or Data Error Token (DET)
	unsigned char r;
	int i=10000;
	do{
		r=MSD_readSPI();
		if(r!=0xFF) break;				//Espera hasta que empiesa transmision
	}while(--i>0);
	if(r==MSD_START_BLOCK_TOKEN) return MSD_START_BLOCK_TOKEN;
	else return r;				//	Devuelve START_BLOCK_TOKEN si tiene exito, DATA_ERROR_TOKEN si hay errores
	/*
	START_BLOCK_TOKEN format:
	0xFE= 1111 1110	
	DATA_ERROR_TOKEN format:
	b0: Error
	b1: CC Error
	b2: Card ECC failed
	b3: Out of range
	b4: 0
	b5: 0
	b6: 0
	b7: 0
	*/
}
unsigned char Single_Block_Write(MSD_ARG addr, unsigned char* buff){
	MSD_COMMAND CMD24= create_COMMAND(0x18,0x00000001,0x95);	// CMD24.argument=addres to be written
								// SD card respond sending a block the size of CMD16.argument
								// CMD16.argument es por default 512
	CMD24.argument=(addr<<9);	// hace que la unidad de medida de las direcciones
								// de memoria sean blockes (multiplos de 512bytes)
	int i; unsigned char r;
	send_COMMAND(CMD24);					//envio comando de escritura y ~SS=0
	r=get_R1_response();
	if(r==0x00){						// Si r=0x00=>command accepted
		SPI_2_transfer(MSD_START_BLOCK_TOKEN);
		for(i=0; i<512; i++){
			r=SPI_2_transfer(*buff);
			buff++;					// envia los 512 bytes del buffer en la SD
		}
		SPI_2_transfer(0xFF);
		SPI_2_transfer(0xFF);				// write the 2 CRC bytes (su contenido es irrelevante)
		
		r=get_DRToken();
		if(r==MSD_DATA_ACCEPTED_TOKEN){			// si no hay errores en Data Response Token se grabar√°
			while(1){
				if(MSD_readSPI() !=0x00) break;	// mientras se graba la SD, envia el Busy Token =0x00
			}
		}
	}
	MSD_disableSD();						// ~SS=1 y 8 pulsos por SCK

        //checkeo si escribio correctamente
        MSD_COMMAND CMD13= create_COMMAND(0x0D,0x00000000,0x95);	// CMD24.argument=addres to be written
        send_COMMAND(CMD13);					//envio comando de escritura y ~SS=0
	unsigned int ri=get_R2_response();
        MSD_disableSD();

        if( ri!=0x0000 ){

            #if (SCH_MEMSD_VERBOSE>=2)
                char ret[6]; Hex16ToAscii( ri, ret); printf("R2 Resp="); printf(ret); printf("\n");
            #endif
            return 0xFF;
        }
        
	return r;						//devuelve DATA_RESPONSE_TOKEN byte (DATA_ACCEPTED_TOKEN =0x05 si tuvo exito)
}
unsigned char get_DRToken(void){		// Data Response Token
	unsigned char r;
	int i=9;
	do{
		r=MSD_readSPI();					// trunca al byte inferior de unsigned int readSPI()
		if(r!=0xFF) break;				// veo 9 veces si es que hay respuesta de la tarjeta SD
	}while(--i>0);
	return (r&0x1F);					// (r&0x1F)=> apaga bits 5, 6 y 7 => "010"=>0x05; "101"=>0x0B ;"110"=0x0D;
	/*
	b0: 1		=> Always set to one
	b1: Status	|
	b2: Status	|=> Status: "010" Data accpeted; "101" Data rejected due to a CRC error; "110" Data rejected due to a Write Error
	b3: Status	|
	b4:	0		=> Always set to zero
	b5: X 		=> Don't care
	b6: X 		=> Don't care
	b7: X 		=> Don't care
	*/
}
unsigned int get_R2_response(void){
    unsigned char r; unsigned int re=0;
    int i;
    i=9;
    do{
        r=MSD_readSPI();            //trunca al byte inferior de unsigned int readSPI()
        if(r!=0xFF) break;		//veo 9 veces si es que hay respuesta de la tarjeta SD
    }while(--i>0);
    re=(unsigned int)r;
    re=(re<<8);

    i=9;
    do{
        r=MSD_readSPI();            //trunca al byte inferior de unsigned int readSPI()
        if(r!=0xFF) break;		//veo 9 veces si es que hay respuesta de la tarjeta SD
    }while(--i>0);
    re=(re)|((unsigned int)r);

    return re;
}
int SD_presence(void){
    return 1;//!nSDCD;		//devuelve 1 si la SD esta, 0 si no esta.
}
/*
Comandos para manejo de la SD:
//----------------------------------------------------------------------------------------------------------------------------
	COMMAND CMD0 = create_COMMAND(0x00,0x00000000,0x95);	// solo para este comando importa un crc valido (0x95)
	COMMAND CMD1 = create_COMMAND(0x01,0x40000000,0x95);	// initialize card, and send Host Capcity Support
															// (HCS,b30=1)=> host soporta High Capacity SD
	COMMAND CMD6 = create_COMMAND(0x06,0x00000000,0x95);	// switch function in check mode 
															// and group 1 and 2 functions are 0x0
	COMMAND CMD8 = create_COMMAND(0x08,0x000001AA,0x95);	// suplyy voltaje is 3.3V and Check Pattern is 0xAA
	COMMAND CMD9 = create_COMMAND(0x09,0x00000000,0x95);	// ask the card to send Card Specific Data (CSD)
	COMMAND CMD10= create_COMMAND(0x0A,0x00000000,0x95);	// ask the card to send its Card Identification (CID)
	COMMAND CMD12= create_COMMAND(0x0C,0x00000000,0x95);	// stop card transmission in multiple block read operation
	COMMAND CMD13= create_COMMAND(0x0D,0x00000000,0x95);	// ask the card to send its Status Register
	
	//address unit= [byte] for standar capacity SD or [512*byte]=[block] for high capacity SD
	COMMAND CMD16= create_COMMAND(0x10,0x00000200,0x95);	// block lenght -for read, write and LOCK_UNLOCK command-
															// is 512 byte (default and standard for hard disks)
	COMMAND CMD17= create_COMMAND(0x11,0x00000001,0x95);	// CMD17.argument=addres to be read 
															// SD card respond sending a block the size of CMD16.argument
	COMMAND CMD18= create_COMMAND(0x12,0x00000001,0x95);	// same as CMD17 but in continuous blocks until CMD12 is send
	COMMAND CMD24= create_COMMAND(0x18,0x00000001,0x95);	// same as CMD17 but for write operations
	COMMAND CMD25= create_COMMAND(0x19,0x00000001,0x95);	// same as CMD18 but for write operations

	COMMAND CMD27= create_COMMAND(0x1B,0x00000000,0x95);	// programming of the programable bits of CSD
	
	// high capacity cards do not support write protection features
	COMMAND CMD28= create_COMMAND(0x1C,0x00000001,0x95);	// write protection bit is set for the addressed group.
															// Each bit of CMD28.argumment is certain group
	COMMAND CMD29= create_COMMAND(0x1D,0x00000001,0x95);	// the opposite of CMD28
	COMMAND CMD30= create_COMMAND(0x1E,0x00000001,0x95);	// write protection status, each bit of CMD30.argument 
															// is an addressed grupo
															
	//address unit= [byte] for standar capacity SD or [512*byte]=[block] for high capacity SD
	COMMAND CMD32= create_COMMAND(0x20,0x00000001,0x95);	// address of the FIRST write block to be erased
	COMMAND CMD33= create_COMMAND(0x21,0x00000001,0x95);	// address of the LAST write block to be erased
	COMMAND CMD38= create_COMMAND(0x26,0x00000000,0x95);	// erase all previouslly selected write blocks

	COMMAND CMD42= create_COMMAND(0x2A,0x00000000,0x95);	// starts a data block transfer (the size of CMD16.argument)
															// to set/reset the password or lock/unlock the card

	COMMAND CMD55= create_COMMAND(0x37,0x00000000,0x95);	// next command shall be interpreted as Aplication-Command
	COMMAND CMD56= create_COMMAND(0x38,0x00000000,0x95);	// b0=R/~W a data block the size of CMD16.argument
															// Creo que es el comando enviado en cada transferencia R/~W
															// luego de los comandos CMD17, 18, 24 o CMD25.
	COMMAND CMD58= create_COMMAND(0x3A,0x00000000,0x95);	// reads the Operating Condition Register (OCR). 
															// Card Capacity Status bit (CCS) is OCR[30]
															// Power-up Routine bit (busy) is OCR[31]. 
	COMMAND CMD59= create_COMMAND(0x3B,0x00000000,0x95);	// CRC byte on/off. 1=on 0=off
	
	//Aplication-Command
	COMMAND ACMD13= create_COMMAND(0x0D,0x00000000,0x95);	// command to receive-back the SD status (512bit long )
	COMMAND ACMD22= create_COMMAND(0x16,0x00000000,0x95);	// command to receive-back the number of well written blocks
	COMMAND ACMD23= create_COMMAND(0x2A,0x00000001,0x95);	// number of write blocks to be pre-erased before writing
	COMMAND ACMD41= create_COMMAND(0x29,0x40000000,0x95);	// initialize card, and send Host Capcity Support
															// (HCS,b30=1)=> host soporta High Capacity SD
	COMMAND ACMD42= create_COMMAND(0x2A,0x00000000,0x95);	// dis-connect the 50kOhm pull-up resistor on ~CS line
	COMMAND ACMD51= create_COMMAND(0x33,0x00000000,0x95);	// command to receive-back the SD Configuration Register(SCR)
	//ACMD 18, ACMD25, ACMD26, ACMD38 and finally ACMD43 up to ACMD49 are reserved for security operations.
//----------------------------------------------------------------------------------------------------------------------------
*/


