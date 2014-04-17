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

#include "cmdRepository.h"

static CmdRepo_cmdXXX_handler buff_cmdXXX_handler[CMD_BUFF_CMDXX_LEN];
static int buff_cmdXXX_handler_indx;

void repo_set_cmdXXX_hanlder(CmdRepo_cmdXXX_handler cmdXXX_handler){
    if(buff_cmdXXX_handler_indx<CMD_BUFF_CMDXX_LEN){
        buff_cmdXXX_handler[buff_cmdXXX_handler_indx] = cmdXXX_handler;
        buff_cmdXXX_handler_indx++;

        //ejecuta onReset del cmdXX añadido
        cmdXXX_handler.xxx_onReset();
    }
    else{
        printf("repo_set_cmdXXX_hanlder\n ERROR: maximo valor de len_buff_cmdXXX_handler alcanzado !!! \n");
        printf("La presente incorporacion de cmdXXX_hanlder ya no tendra efecto (ni las posteriores)\n");
    }
}
cmdFunction repo_getFunction(int cmdID)
{
    unsigned char cmdOwn, cmdNum;
    cmdFunction result;

    cmdNum = (unsigned char)cmdID;
    cmdOwn = (unsigned char)(cmdID>>8);

    //printf("[repo_getFunction] cmdOwn: 0x%X | cmdNum: 0x%X\n", cmdOwn, cmdNum );

    int i,j;
    for(i=0;i<buff_cmdXXX_handler_indx;i++){
        if( buff_cmdXXX_handler[i].cmdOwn == cmdOwn ){
            for(j=0; j<buff_cmdXXX_handler[i].nCmd; j++){
                if(j==cmdNum){
                    result = buff_cmdXXX_handler[i].p_xxxFunction[j];
                    return result;
                }
            }
        }
    }
    
    return cmdNULL;
}

int repo_getsysReq(int cmdID)
{
    unsigned char cmdOwn, cmdNum;
    int result;

    cmdNum = (unsigned char)cmdID;
    cmdOwn = (unsigned char)(cmdID>>8);

    int i,j;
    for(i=0;i<buff_cmdXXX_handler_indx;i++){
        if( buff_cmdXXX_handler[i].cmdOwn == cmdOwn ){
            for(j=0; j<buff_cmdXXX_handler[i].nCmd; j++){
                if(j==cmdNum){
                    result = buff_cmdXXX_handler[i].p_xxxSysReq[j];
                    return result;
                }
            }
        }
    }

    return 0xFFFF;
}

int cmdNULL(void *param)
{
    int arg=*( (int *)param );
    printf("cmdNULL was used with param %d\n", arg );

    return 1;
}

