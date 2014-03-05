/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

#include "memFlash.h"

/*TODO: CAMBIAR EL NOMBRE A LAS FUNCIONES, EMPEZAR CON MEMFLASH*/
void memFlash_lock(void) {
    //  Abrir_SPI_1_default();PIC24FJ256GA110
    unsigned char d_in;
    unsigned char opcodeWD = 0x04; // Write disable
    SPI_nSS_1 = 0;
    d_in = SPI_1_transfer(opcodeWD);
    SPI_nSS_1 = 1;
}

void memFlash_unlock(void) {
    // Abrir_SPI_1_default();
    unsigned char result;
    unsigned char opcodeWE = 0x06; //Write Enable
    SPI_nSS_1 = 0; // Chip Select Activo!
    result = SPI_1_transfer(opcodeWE);
    SPI_nSS_1 = 1;
}

unsigned char memFlash_write(unsigned char* dir, char dato) {
    /*¡¡¡ Debe activarse la funcion "memFlash_Unlock" primero antes de usar esta funcion!!!*/
    unsigned char d_in;
    unsigned char opcode = 0x02; // Byte page programm command
    //unsigned char opcode=0xA2; // Dual-Byte page program command

    SPI_nSS_1 = 0; /* Chip select activo */
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    d_in = SPI_1_transfer(dir[3]);
    d_in = SPI_1_transfer(dato);
    SPI_nSS_1 = 1; /* Chip select desactivado */

    return d_in;
}

void memFlash_write_page(unsigned char* dir, unsigned char* dato, int len) {
    /*¡¡¡ Debe activarse la funcion "memFlash_Unlock" primero antes de usar esta funcion!!!*/
    unsigned char d_in;
    unsigned char opcode = 0x02; // Byte page programm command
    //unsigned char opcode=0xA2; // Dual-Byte page program command

    SPI_nSS_1 = 0; /* Chip select activo */
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    d_in = SPI_1_transfer(dir[3]);

    int i;
    for(i = 0; i< len; i++)
        d_in = SPI_1_transfer(dato[i]);

    SPI_nSS_1 = 1; /* Chip select desactivado */
}

void memFlash_readProtector(unsigned char * dir) {
    unsigned char result, d_in;
    unsigned char null = 0x00;
    unsigned char opcode = 0x3C;
    // Abrir_SPI_1_default();
    SPI_nSS_1 = 0; /* Chip Select Activo*/
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[1]); /*Se escriben los 3 bytes de la dirección*/
    d_in = SPI_1_transfer(dir[2]); //
    d_in = SPI_1_transfer(dir[3]); //
    result = SPI_1_transfer(null);
    SPI_nSS_1 = 1;
    if (result == 0x00)//if(!result)
    {
        con_printf("El sector esta desprotegido\r\n");
    }
    if (result == 0xFF) {
        con_printf("El sector esta protegido\r\n");
    }
}

void memFlash_readLockdownRegistrers(unsigned char * dir) {
    unsigned char null = 0xAA;
    unsigned char result, d_in;
    unsigned char opcode = 0x35; // Default
    SPI_nSS_1 = 0; //Chip Select Activo
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    result = SPI_1_transfer(null); /*Resultado aca!*/
    SPI_nSS_1 = 1;
    if (result == 0x00)//if(!result)
    {
        con_printf("El sector esta desprotegido\r\n");
    }
    if (result == 0xFF) {
        con_printf("El sector esta protegido\r\n"); // Pude haber retornado un 1 o 0 pero asi es mejor
    }

}

unsigned char memFlash_read(unsigned char* dir, unsigned char *lectura1) {
    unsigned char opcode = 0x0B; // Default
    //  unsigned char opcode=0x03;// lower frequency
    //  unsigned char opcode=0x1B;// Highest read performance posible
    SPI_nSS_1 = 0; //Chip select activo
    lectura1[0] = SPI_1_transfer(opcode);
    lectura1[0] = SPI_1_transfer(dir[0]);
    lectura1[0] = SPI_1_transfer(dir[1]);
    lectura1[0] = SPI_1_transfer(dir[2]);
    lectura1[0] = SPI_1_transfer(0x00);
    lectura1[1] = SPI_1_transfer(0x00); /* Resulado aca */
    SPI_nSS_1 = 1; //Chip select desactivo
    return lectura1[1];
}

void memFlash_read_array(unsigned char* dir, unsigned char *lectura, int len) {
    unsigned char result;
    unsigned char opcode = 0x0B; // Default
    //  unsigned char opcode=0x03;// lower frequency
    //  unsigned char opcode=0x1B;// Highest read performance posible
    SPI_nSS_1 = 0; //Chip select activo
    result = SPI_1_transfer(opcode);
    result = SPI_1_transfer(dir[0]);
    result = SPI_1_transfer(dir[1]);
    result = SPI_1_transfer(dir[2]);
    result = SPI_1_transfer(0x00); /* Dont care */

    int i;
    for(i = 0; i< len; i++)
        lectura[i] = SPI_1_transfer(0x00); /* Resulados aca */
    
    SPI_nSS_1 = 1; //Chip select desactivo
}

void memFlash_UnlockProtection(unsigned char * dir) {
    /* Debe activarse la funcion "memFlash_Unlock" primero antes
     * La memoria queda con escritura desactivada luego de esta operacion */
    unsigned char  d_in;
    unsigned char opcode = 0x39;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    SPI_nSS_1 = 1; // Chip select desactivado!
}

void memFlash_LockProtection(unsigned char * dir) {
    /*¡¡¡ Debe activarse la funcion "memFlash_Unlock" primero antes de usar esta funcion!!!*/
    unsigned char  d_in;
    unsigned char opcode = 0x36;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    SPI_nSS_1 = 1; // Chip select desactivado!
}


void memFlash_sectorLockdown(unsigned char * dir) {
    unsigned char d_in;
    unsigned char null = 0x00;
    unsigned char opcode = 0x33;
    unsigned char opcodeC = 0xD0;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    d_in = SPI_1_transfer(opcodeC);
    d_in = SPI_1_transfer(null);
    SPI_nSS_1 = 1; //Chip select no activo!

}
//SPI_nWP_1 write protect

void memFlash_WriteRegister1(void) {
    unsigned char d_in;
    unsigned char null = 0x00;
    unsigned char opcode = 0x01;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in=SPI_1_transfer(0);
    d_in=SPI_1_transfer(null);
    d_in=SPI_1_transfer(0);
    d_in=SPI_1_transfer(0);
    d_in=SPI_1_transfer(0);
    d_in=SPI_1_transfer(0);
    d_in = SPI_1_transfer(null);
    d_in = SPI_1_transfer(null);
    SPI_nSS_1 = 1; //Chip select descativado!
}

void memFlash_Erase(unsigned char * dir, unsigned char size) {
    unsigned char d_in,opcode;
    unsigned opcode1=0x20;// 4 kbytes
    unsigned opcode2=0x52;//32 kbytes
    unsigned opcode3=0xD8;//64 kbytes
   
    if (size==4){
        opcode=opcode1;
    }
    else if (size==32){
        opcode=opcode2;
    }
    else if (size==64){
        opcode=opcode3;
    }
    else
        return;

    d_in = SPI_1_transfer(opcode);
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    SPI_nSS_1 = 1;// Chip select desactivado!

     while(1)
     {
         unsigned int i;
         for(i=0;i<0xFFFF; i++);
         i = mem_ReadStatus(0);
         if( !(i & 0x0001) )
             return;
     }
}
void memFlash_Freeze(void){
    int i;
    unsigned char dir1[3];
    long dir;
    for (  i=0; i<2031;i++){
        dir= 0x0FFF + 0x1000*i;
        dir1[0]=(char)(dir>>0)&(0x000000FF);
        dir1[1]=(char)(dir>>8)&(0x000000FF);
        dir1[2]=(char)(dir>>16)&(0x000000FF);
        unsigned char a1[]={ dir1[0],dir1[1] ,dir1[2]};
        mem_FlashFreezeSector(a1);
    }
}

void mem_FlashFreezeSector(unsigned char * dir) {
    unsigned char opcode = 0x34; // Freeze Sector Lockdown State
    unsigned char d_in;
    unsigned char opcode1[] = {0x55, 0xAA, 0x40};
    unsigned char opcode2 = 0xD0;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(opcode1[0]);
    d_in = SPI_1_transfer(opcode1[1]);
    d_in = SPI_1_transfer(opcode1[2]);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    d_in = SPI_1_transfer(opcode2);
    SPI_nSS_1 = 1;
}


//****** Funciones de servicio *********************************************
void mem_4k_block_erase_memFalsh(unsigned int block)
{
    if(block>2047)  //page>0h7FF
    {
        return;   /* operation failed */
    }

    mem_WriteEnable();  /* set WEL  bit to 1 */

    SPI_nSS_1 = 0;//Chip Select activo
    SPI_1_transfer(MEM_4K_BLOCK_ERASE);

    MEM_ADDR dir;
    mem_page_to_MEM_ADDR(block, &dir);
    SPI_1_transfer(dir.a3);
    SPI_1_transfer(dir.a2);
    SPI_1_transfer(dir.a1);
    SPI_nSS_1 = 1;//Chip Select inactivo

    //bussy waiting
    while( (0x0001 & mem_ReadStatus(0) ) );
}

void mem_chipErase_memFlash(void){

    mem_WriteEnable();  /* set WEL  bit to 1 */

    /*
    // la función write enable debe estar en el main para poder usar esta función
    SPI_nSS_1 = 0;//Chip Select activo
    unsigned char d_in;
    unsigned char opcode1=0x60;
    unsigned char opcode2=0xC7;
    d_in=SPI_1_transfer(opcode1);
    d_in=SPI_1_transfer(opcode2);
    SPI_nSS_1= 1;//Chip Select desactivado
    */

    SPI_nSS_1 = 0;//Chip Select activo
    SPI_1_transfer(MEM_CHIP_ERASE);
    SPI_nSS_1 = 1;//Chip Select inactivo

    //bussy waiting
    while( (0x0001 & mem_ReadStatus(0) ) ){}
}

void mem_GlobalUnprotect(void)
{
    //mem_ReadStatus(1);
    //already GLobalUnprotected
    //if( mem_ReadStatus(1)==0x0010 ){con_printf("already GLobalUnprotected\r\n"); return;}

    SPI_nWP_1=1;        /* deassert nWP pin */
    //debiera ser 0b01000011 para Global unprotect
    mem_WriteStatus1(0b01000011);

    /*
    unsigned char null = 0x00;
    SPI_nSS_1 = 0; //Chip select activo!
    SPI_1_transfer(MEM_WRITE_STATUS1);
    SPI_1_transfer(0);
    SPI_1_transfer(null);
    SPI_1_transfer(0);
    SPI_1_transfer(0);
    SPI_1_transfer(0);
    SPI_1_transfer(0);
    SPI_1_transfer(null);
    SPI_1_transfer(null);
    SPI_nSS_1 = 1; //Chip select descativado!
    */
}

void mem_UnlockProtection(unsigned char * dir) {

    /*¡¡¡ Debe activarse la funcion "memFlash_Unlock" primero antes de usar esta funcion!!!*/
    unsigned char  d_in; // Unprotect Sector
    unsigned char opcode = 0x39;
//    unsigned char null = 0x00;
    SPI_nSS_1 = 0; //Chip select activo!
    d_in = SPI_1_transfer(opcode);
    d_in = SPI_1_transfer(dir[0]);
    d_in = SPI_1_transfer(dir[1]);
    d_in = SPI_1_transfer(dir[2]);
    SPI_nSS_1 = 1;
    con_printf("La memoria flash ahora esta desprotegida \r\n");
}

unsigned int mem_ReadStatus(int mode)
{
    unsigned int stat;

    stat=0;
    SPI_nSS_1 = 0;
    SPI_1_transfer(MEM_READ_STATUS);

    stat = SPI_1_transfer(MEM_DUMMYLOAD);                      //Byte 1
    stat= ( SPI_1_transfer(MEM_DUMMYLOAD)<<8 ) | ( stat );     //Byte 2
    SPI_nSS_1 = 1;

    if(mode)
    {
        char buff[10];
        con_printf("mem_ReadStatus: ");
        //itoa(buff, stat, 16);
        sprintf (buff, "0x%X", (unsigned int)stat);
        con_printf( buff); con_printf(" \r\n");
    }
    
    return stat;
}
void mem_WriteStatus1(unsigned char b1)
{
    //b1 debiera ser b01000011 para Global unprotect

    mem_WriteEnable();  /* set WEL  bit to 1 */
    SPI_nSS_1 = 0;
    SPI_1_transfer(MEM_WRITE_STATUS1);
    SPI_1_transfer(b1);
    SPI_nSS_1 = 1;
}

void mem_WriteEnable(void)
{
    SPI_nSS_1 = 0;
    SPI_1_transfer(MEM_WRITE_ENABLE);
    SPI_nSS_1 = 1;
}

void mem_page_to_MEM_ADDR(unsigned int page, MEM_ADDR *dir)
{
    /*
     * Esta funcion aun no esta lista, por lo que de la mem Flash solo se pueden escribir/leer
     * las primeras 2^8=256 paginas, desde la 0x0000 hasta la 0x00FF (cada pagina es de 256bytes).
     */
    /*
    page 1: 0001FF h ? 000100h
    page 2: 0000FF h ? 000000h
    */
    if(dir==NULL){return;}

    dir->a1=0x00;
    dir->a2=(unsigned char)page;
    dir->a3=0x00; //(unsigned char)(page>>8);

}

//****** API *********************************************
int mem_init_memFlash(void)
{
    //mem_ReadStatus(1);


    SPI_nWP_1=1;                /* deassert nWP pin */
    mem_GlobalUnprotect();

    //mem_ReadStatus(1);

    return 1;   /* succeeded */
}

int mem_WritePage(unsigned int page, unsigned char* buff, unsigned char length)
{
    if(page>32767)  //page>0h7FFF
    {
        return 0;   /* operation failed */
    }

    //mem_chipErase_memFlash();
    //mem_4k_block_erase_memFalsh(0x0000);

    //bussy waiting while saving the new data
    while( (0x0001 & mem_ReadStatus(0) ) ){}

    mem_WriteEnable();

    SPI_nSS_1 = 0; /* Chip Select Activo*/
    SPI_1_transfer(MEM_WRITE_PAGE);

    MEM_ADDR dir;
    mem_page_to_MEM_ADDR(page, &dir);
    SPI_1_transfer(dir.a3);
    SPI_1_transfer(dir.a2);
    SPI_1_transfer(dir.a1);
    
    unsigned char i;
    for(i=0; i<length; i++)
    {
        SPI_1_transfer(buff[i]);
    }
    
    SPI_nSS_1 = 0;

    //bussy waiting while saving the new data
    while( (0x0001 & mem_ReadStatus(0) ) ){}
    for(i=0xFF;i>0;i--){}
    for(i=0xFF;i>0;i--){}
    for(i=0xFF;i>0;i--){}

    return 1;
}

int mem_ReadPage(unsigned int page, unsigned char* buff, unsigned char length)
{
    if(page>32767)  //page>0x7FFF
    {
        return 0;   /* operation failed */
    }

    //mem_WriteEnable(); no es necesario

    SPI_nSS_1 = 0;
    SPI_1_transfer(MEM_READ_PAGE);

    MEM_ADDR dir;
    mem_page_to_MEM_ADDR(page, &dir);
    SPI_1_transfer(dir.a3);
    SPI_1_transfer(dir.a2);
    SPI_1_transfer(dir.a1);

    SPI_1_transfer(MEM_DUMMYLOAD);
    
    unsigned char i;
    for(i=0; i<length; i++)
    {
        buff[i]=SPI_1_transfer(MEM_DUMMYLOAD);
        //if(buff[i]==0xFF){con_printf("x");}
        //char ret[6];
        //Hex16ToAscii( (unsigned int)buff[i], ret);
        //con_printf("FLASH: ReadPage= "); con_printf(ret); con_printf("\r\n");
    }
    SPI_nSS_1 = 0;

    //bussy waiting
    while( (0x0001 & mem_ReadStatus(0) ) ){}

    return 1;
}


