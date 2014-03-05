#ifndef _MEM_FLASH_
#define _MEM_FLASH_

#include "interfaz_SPI.h"
#include "DebugIncludes.h"  //para con_printf

#define MEM_DUMMYLOAD 0xFF
#define MEM_READ_STATUS 0x05
#define MEM_WRITE_STATUS1 0x01
#define MEM_WRITE_ENABLE 0x06
#define MEM_READ_SECTOR_PROTECT 0x3C
#define MEM_CHIP_ERASE 0x60

#define MEM_4K_BLOCK_ERASE 0x20    // (de 0 a 8388607-1)

#define MEM_WRITE_PAGE 0x02
#define MEM_READ_PAGE 0x0B

typedef struct{
    unsigned char a1, a2, a3;
}MEM_ADDR;

unsigned char memFlash_read(unsigned char* d_in,unsigned char *lectura1); // Lee en la memoria Flash
void memFlash_read_array(unsigned char* dir, unsigned char *lectura, int len);
unsigned char memFlash_write(unsigned char* dir, char dato);//Escribe en la memoria Flash
void memFlash_write_page(unsigned char* dir, unsigned char* dato, int len);
void memFlash_unlock(void);//Habilita la escritura (! Es importante ocuparlas en funciones que escriben en la memoria¡)
void memFlash_lock(void); // Deshabilita la escritura
void memFlash_readProtector(unsigned char * dir);// Leo el tipo de protección de la memoria Flash
void memFlash_UnlockProtection(unsigned char * dir);//Desbloquea el sector especfico de la memoria Flash
void memFlash_LockProtection(unsigned char * dir);// Bloquea el sector especifico de la memoria Flash
void memFlash_sectorLockdown(unsigned char * dir);// Bloquea la memFLash permanentemente
void memFlash_WriteRegister1(void);//Escribe el Status register
void memFlash_Erase(unsigned char * dir, unsigned char size);// Se borra toda la información del sector que quiero borrar
void mem_FlashFreezeSector(unsigned char * dir);//Desbloquea permanentemente la memoriaFlash
void memFlash_readLockdownRegistrers(unsigned char * dir);
void memFlash_Freeze(void);

//**** Funciones de servicio **************************************************
void mem_GlobalUnprotect(void);
void memF_UnlockProtection(MEM_ADDR *dir);//Desbloquea el sector especfico de la memoria Flash

unsigned int mem_ReadStatus(int mode);
void mem_WriteStatus1(unsigned char b1);

void mem_page_to_MEM_ADDR(unsigned int page, MEM_ADDR *dir);

/* The WEL bit must be set (is set using mem_WriteEnable() function) before a Byte/Page Program, erase, Protect
Sector, Unprotect Sector, Sector Lockdown, Freeze Sector Lockdown State, Program OTP
Security Register, or Write Status Register command can be executed */
void mem_WriteEnable(void);
//**** API **************************************************
int mem_init_memFlash(void);

/* Una pagina es de 256bytes y hay (2^23)/256=32768 paginas en la memFlash.
 * Por tanto, hay paginas de [0-32767]
 */
int mem_WritePage(unsigned int page, unsigned char* buff, unsigned char length);
int mem_ReadPage(unsigned int page, unsigned char* buff, unsigned char length);

void mem_chipErase_memFlash(void);// Borra todo el contenido de la memoria Flash
void mem_4k_block_erase_memFalsh(unsigned int block); // hay 2048 blocks = 127secotres*16blockes de 4KB

#endif
