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

#include "interfaz_SPI.h"

unsigned char SPI_1_transfer(unsigned char a){
	WriteSPI1(a);
	while(!SPI1_Rx_Buf_Full){}	// espera mientras llega la trama. 
                                        /*
                                        SPIxIF (interrupt flag de transferenecia completa) se levanta cuando
                                        se shiftea (envia y captura) el ultimo bit -b0- de SPIxSR.
                                        SPIxSTAT.SPIxRBF se levanta cuando SPIxSR pasa la trama a SPIxRxBUF.
                                        Pero ambos (SPIxSTAT.SPIxRBF y SPIxIF) se levantan UN CICLO DE fcy DESPUES del sampling y no
                                        una vez terminado el clock SCK.

                                         Si SPIxRBF=1 => hay un dato a leer, si no se lee antes de recibir otra trama se
                                         produce overflow y se levanta el flag SPIxSTAT.SPIROV
                                         */
	while(SPI_SCK_1){}				// espera mientras SCK no baje de su ultimo clock
	unsigned char b= ReadSPI1();
	return b;
}
int TXB_SPI_1(void){
	return (SPI1STAT&0b00000010);
}
int RXB_SPI_1(void){
	return (SPI1STAT&0b00000001);
}
int overflow_SPI_1(void){
	return (SPI1STAT&0b00100000);
}
void clear_overflow_SPI_1(void){
	SPI1STAT=SPI1STAT&0xffbf; //borra el bit SPIROV para proximos setes (cuando haya overflow nuevamente)
}
void Open_SPI_1_default(void){
    unsigned int spi1con1, spi1con2, spi1stat;
    spi1con1=ENABLE_SCK_PIN&ENABLE_SDO_PIN&SPI_MODE16_OFF&SPI_SMP_OFF&SPI_CKE_ON&SLAVE_ENABLE_OFF&CLK_POL_ACTIVE_HIGH&
    MASTER_ENABLE_ON&SEC_PRESCAL_8_1&PRI_PRESCAL_1_1 ;//SPIxCON1
    spi1con2=FRAME_ENABLE_OFF&FRAME_SYNC_OUTPUT&FRAME_POL_ACTIVE_LOW&FRAME_SYNC_EDGE_PRECEDE&FIFO_BUFFER_DISABLE ; //SPIxCON2
    spi1stat=SPI_ENABLE&SPI_IDLE_CON&SPI_RX_OVFLOW_CLR ; //SPIxSTAT
    //spi1con1 = 0x0329;
    //spi1con2 = 0x0000;
    //zpi1stat = 0x0000;
    OpenSPI1(spi1con1, spi1con2, spi1stat);
}
//-------------------------------------------------------------------------------------------------------------------------------
unsigned char SPI_2_transfer(unsigned char a){
	WriteSPI2(a);
	while(!SPI2_Rx_Buf_Full){}	// espera mientras llega la trama. 
                                        /*
                                        SPIxIF (interrupt flag de transferenecia completa) se levanta cuando
                                        se shiftea (envia y captura) el ultimo bit -b0- de SPIxSR.
                                        SPIxSTAT.SPIxRBF se levanta cuando SPIxSR pasa la trama a SPIxRxBUF.
                                        Pero ambos (SPIxSTAT.SPIxRBF y SPIxIF) se levantan UN CICLO DE fcy DESPUES del sampling y no
                                        una vez terminado el clock SCK.

                                         Si SPIxRBF=1 => hay un dato a leer, si no se lee antes de recivir otra trama se
                                         produce overflow y se levanta el flag SPIxSTAT.SPIROV
                                         */
	while(SPI_SCK_2){}				// espera mientras SCK no baje de su ultimo clock
	unsigned char b= ReadSPI2();
	return b;
}
int TXB_SPI_2(void){
	return (SPI2STAT&0b00000010);
}
int RXB_SPI_2(void){
	return (SPI2STAT&0b00000001);
}
int overflow_SPI_2(void){
	return (SPI2STAT&0b00100000);
}
void clear_overflow_SPI_2(void){
	SPI2STAT=SPI2STAT&0xffbf; //borra el bit SPIROV para proximos setes (cuando haya overflow nuevamente)
}
void Open_SPI_2_default(void){
	unsigned int spi1con1, spi1con2, spi1stat;
	spi1con1=ENABLE_SCK_PIN&ENABLE_SDO_PIN&SPI_MODE16_OFF&SPI_SMP_OFF&SPI_CKE_ON&SLAVE_ENABLE_OFF&CLK_POL_ACTIVE_HIGH&
	MASTER_ENABLE_ON&SEC_PRESCAL_3_1&PRI_PRESCAL_1_1 ;//SPIxCON1
	spi1con2=FRAME_ENABLE_OFF&FRAME_SYNC_OUTPUT&FRAME_POL_ACTIVE_LOW&FRAME_SYNC_EDGE_PRECEDE&FIFO_BUFFER_DISABLE ; //SPIxCON2
	spi1stat=SPI_ENABLE&SPI_IDLE_CON&SPI_RX_OVFLOW_CLR ; //SPIxSTAT
	OpenSPI2(spi1con1, spi1con2, spi1stat);
}
//-------------------------------------------------------------------------------------------------------------------------------
unsigned char SPI_3_transfer(unsigned char a){
	WriteSPI3(a);
	while(!SPI3_Rx_Buf_Full){}
            // espera mientras llega la trama.
            /*
            SPIxIF (interrupt flag de transferenecia completa) se levanta cuando
            se shiftea (envia y captura) el ultimo bit -b0- de SPIxSR.
            SPIxSTAT.SPIxRBF se levanta cuando SPIxSR pasa la trama a SPIxRxBUF.
            Pero ambos (SPIxSTAT.SPIxRBF y SPIxIF) se levantan UN CICLO DE fcy
            DESPUES del sampling y no una vez terminado el clock SCK.

             Si SPIxRBF=1 => hay un dato a leer, si no se lee antes de recivir
             otra trama se produce overflow y se levanta el flag SPIxSTAT.SPIROV
             */
	while(SPI_SCK_3){}	//espera mientras SCK no baje de su ultimo clock
	unsigned char b= ReadSPI3();
	return b;
}
int TXB_SPI_3(void){
	return (SPI3STAT&0b00000010);
}
int RXB_SPI_3(void){
	return (SPI3STAT&0b00000001);
}
int overflow_SPI_3(void){
	return (SPI3STAT&0b00100000);
}
void clear_overflow_SPI_3(void){
	SPI3STAT=SPI3STAT&0xffbf; //borra el bit SPIROV para proximos setes (cuando haya overflow nuevamente)
}
void Open_SPI_3_default(void){
	unsigned int spi1con1, spi1con2, spi1stat;
	spi1con1=ENABLE_SCK_PIN&ENABLE_SDO_PIN&SPI_MODE16_OFF&SPI_SMP_OFF&SPI_CKE_ON&SLAVE_ENABLE_OFF&CLK_POL_ACTIVE_HIGH&
	MASTER_ENABLE_ON&SEC_PRESCAL_1_1&PRI_PRESCAL_1_1 ;//SPIxCON1
	spi1con2=FRAME_ENABLE_OFF&FRAME_SYNC_OUTPUT&FRAME_POL_ACTIVE_LOW&FRAME_SYNC_EDGE_PRECEDE&FIFO_BUFFER_DISABLE ; //SPIxCON2
	spi1stat=SPI_ENABLE&SPI_IDLE_CON&SPI_RX_OVFLOW_CLR ; //SPIxSTAT
	OpenSPI3(spi1con1, spi1con2, spi1stat);
}
