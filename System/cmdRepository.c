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

//extern cmdFunction trxFunction[];
//extern cmdFunction ppcFunction[];
//extern cmdFunction conFunction[];
//extern cmdFunction epsFunction[];
//extern cmdFunction drpFunction[];
//extern cmdFunction payFunction[];
//extern cmdFunction rtcFunction[];
//extern cmdFunction tcmFunction[];
//
//extern int trx_sysReq[];
//extern int ppc_sysReq[];
//extern int con_sysReq[];
//extern int eps_sysReq[];
//extern int drp_sysReq[];
//extern int pay_sysReq[];
//extern int rtc_sysReq[];
//extern int tcm_sysReq[];

//int repo_getsysReq(int cmdID)
//{
//    int cmdOwn, cmdNum;
//    int result;
//
//    cmdNum = (unsigned char)cmdID;
//    cmdOwn = (unsigned char)(cmdID>>8);
//
//    switch (cmdOwn)
//    {
//        case CMD_PPC:
//            if(cmdNum>=PPC_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = ppc_sysReq[cmdNum];
//            }
//            break;
//        case CMD_TRX:
//            if(cmdNum>=TRX_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = trx_sysReq[cmdNum];
//            }
//            break;
//        case CMD_EPS:
//            if(cmdNum>=EPS_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = eps_sysReq[cmdNum];
//            }
//            break;
//        case CMD_PAY:
//            if(cmdNum>=PAY_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = pay_sysReq[cmdNum];
//            }
//            break;
//        case CMD_CON:
//            if(cmdNum>=CON_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = con_sysReq[cmdNum];
//            }
//            break;
//        case CMD_DRP:
//            if(cmdNum>=DRP_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = drp_sysReq[cmdNum];
//            }
//            break;
//        case CMD_RTC:
//            if(cmdNum>=RTC_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = rtc_sysReq[cmdNum];
//            }
//            break;
//        case CMD_TCM:
//            if(cmdNum>=TCM_NCMD){
//                result=CMD_SYSREQ_MIN;
//            }
//            else{
//                result = tcm_sysReq[cmdNum];
//            }
//            break;
//        default:
//            result = CMD_SYSREQ_MIN;
//            break;
//    }
//
//    return result;
//}


static CmdRepo_cmdXXX_handler buff_cmdXXX_handler[CMD_BUFF_CMDXX_LEN];
static int len_buff_cmdXXX_handler;
void repo_set_cmdXXX_hanlder(CmdRepo_cmdXXX_handler cmdXXX_handler){
    if(len_buff_cmdXXX_handler<CMD_BUFF_CMDXX_LEN){
        buff_cmdXXX_handler[len_buff_cmdXXX_handler] = cmdXXX_handler;
        len_buff_cmdXXX_handler++;

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

    printf("[repo_getFunction] cmdOwn: 0x%X | cmdNum: 0x%X\n", cmdOwn, cmdNum );

    int i,j;
    for(i=0;i<len_buff_cmdXXX_handler;i++){
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
    for(i=0;i<len_buff_cmdXXX_handler;i++){
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
//cmdFunction repo_getCmd(int cmdID)
//{
//    int cmdOwn, cmdNum;
//    cmdFunction result;
//
//    cmdNum = (unsigned char)cmdID;
//    cmdOwn = (unsigned char)(cmdID>>8);
//
//    switch (cmdOwn)
//    {
//        case CMD_PPC:
//            if(cmdNum>=PPC_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = ppcFunction[cmdNum];
//            }
//            break;
//        case CMD_TRX:
//            if(cmdNum>=TRX_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = trxFunction[cmdNum];
//            }
//            break;
//        case CMD_EPS:
//            if(cmdNum>=EPS_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = epsFunction[cmdNum];
//            }
//            break;
//        case CMD_PAY:
//            if(cmdNum>=PAY_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = payFunction[cmdNum];
//            }
//            break;
//        case CMD_CON:
//            if(cmdNum>=CON_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = conFunction[cmdNum];
//            }
//            break;
//        case CMD_DRP:
//            if(cmdNum>=DRP_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = drpFunction[cmdNum];
//            }
//            break;
//        case CMD_RTC:
//            if(cmdNum>=RTC_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = rtcFunction[cmdNum];
//            }
//            break;
//        case CMD_TCM:
//            if(cmdNum>=TCM_NCMD){
//                result=cmdNULL;
//            }
//            else{
//                result = tcmFunction[cmdNum];
//            }
//            break;
//        default:
//            result = cmdNULL;
//            break;
//    }
//
//    return result;
//}

/*
int repo_setCmd(int cmdID, cmdFunction function)
{
    int cmdOwn, cmdNum, result;

    cmdNum = (char)cmdID;
    cmdOwn = (char)(cmdID>>8);
    result = cmdID;

    switch (cmdOwn)
    {
        case CMD_PPC:
            ppcFunction[cmdNum] = function;
            break;
        case CMD_TRX:
            trxFunction[cmdNum] = function;
            break;
        case CMD_EPS:
            epsFunction[cmdNum] = function;
            break;
        case CMD_PAY:
            payFunction[cmdNum] = function;
            break;
        case CMD_CON:
            conFunction[cmdNum] = function;
            break;
        case CMD_DRP:
            drpFunction[cmdNum] = function;
            break;
        case CMD_RTC:
            rtcFunction[cmdNum] = function;
            break;
        default:
            result = 0;
            break;
    }

    return result;
}
*/

//int repo_onResetCmdRepo(void)
//{
//    ppc_onResetCmdPPC();
//    drp_onResetCmdDRP();
//    con_onResetCmdCON();
//    trx_onResetCmdTRX();
//    tcm_onResetCmdTCM();
//    eps_onResetCmdEPS();
//    pay_onResetCmdPAY();
//    rtc_onResetCmdRTC();
//
//    return 1;
//}

int cmdNULL(void *param)
{
    int arg=*( (int *)param );
    printf("cmdNULL was used with param %d\n", arg );

    return 1;
}

