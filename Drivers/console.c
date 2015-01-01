/*                            SERIAL CONSOLE
 *                              For PIC24F
 *
 *      Copyright 2012, Francisco Reyes Aspe, komodotas@gmail.com
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

#include "console.h"
#include "csp.h"
extern xSemaphoreHandle consolePrintfSem;


char con_buf[CON_BUF_WIDTH];
char con_buf_index;

char con_entry[CON_BUF_WIDTH];
char con_entry_index;
BOOL con_entry_flag; //TRUE if a new attemp of command has arrived

char con_cmd[CON_BUF_WIDTH];
char con_cmd_index;

char con_args[CON_ARG_QTY][CON_ARG_WIDTH];

char con_arg_count;

BOOL con_arg_toolong;

char con_prev_char;

BOOL con_escape_flag;
BOOL con_arrow_flag;

char con_newchar;
BOOL con_init_flag;
BOOL con_active;

BOOL con_cmd_toolong_flag;

/*------------------------------------------------------------------------------
 *		 	CONSOLE COMMAND HANDLER
 *------------------------------------------------------------------------------
 * Description        : Analyze command and chose the correct to be evaluated
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
DispCmd con_cmd_handler(void)
{
    DispCmd newCmd;
    newCmd.idOrig = CMD_IDORIG_TCONSOLE;
    newCmd.sysReq = CMD_SYSREQ_MIN;
    newCmd.cmdId = CMD_CMDNULL;  /* cmdNULL */
    newCmd.param = 0;

    if(con_entry_flag && con_entry_index > 0 )
    {
        con_cmd_from_entry(con_entry);
        if(con_arg_toolong)
        {
            newCmd.cmdId = con_id_error_invalid_arg;
            con_arg_toolong = FALSE;
            con_entry_flag = FALSE;

            return newCmd;
        }
        /* ------------ DRP COMMANDS ---------------*/
        if(strcmp(con_cmd, "thk_EBF") == 0){
            int arg;
            switch(con_arg_count){
                case 1:
                    arg=atoi(con_args[0]);
                    newCmd.cmdId = thk_id_executeBeforeFlight;
                    newCmd.param = arg;
                break;
                default:
                    newCmd.cmdId = con_id_error_count_arg;
                break;
            }

            con_arg_toolong = FALSE;
            con_entry_flag = FALSE;
            return newCmd;
        }
        /* ------------ OTHERS COMMANDS ---------------*/
        if(strcmp(con_cmd, "exe_cmd") == 0)
        {
            int cmd, param;
            char* end;

            switch(con_arg_count)
            {
               case 2:
                  cmd=(unsigned int)strtoul(con_args[0], &end, 0);
                  param = atoi(con_args[1]);

                  newCmd.cmdId = cmd;
                  newCmd.param = param;        /* Argument to the command */
                  break;
              default:
                  newCmd.cmdId = con_id_error_count_arg;  /* con_error_count_arg */
              break;
           }
           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;
           return newCmd;
        }

       /* ------------ PPC COMMANDS ---------------*/
        if(strcmp(con_cmd, "ppc_reset") == 0){
            switch(con_arg_count){
                case 0:
                    newCmd.cmdId= ppc_id_reset;
                    break;
                default:
                    newCmd.cmdId = con_id_error_count_arg;
                    break;
            }

            con_arg_toolong = FALSE;
            con_entry_flag = FALSE;
            return newCmd;
        }

        if(strcmp(con_cmd, "ppc_newosc") == 0){
            switch(con_arg_count){
                int i;
                case 1:
                   i=atoi(con_args[0]);
                   if( i==0 || i==1 || i==2 || i==3 || i==4 || i==5 || i==7 ){
                        newCmd.cmdId=ppc_id_newosc;
                        newCmd.param = i;
                   }
                   else{
                        newCmd.cmdId = con_id_error_invalid_arg;
                   }
                   break;

                default:
                   newCmd.cmdId = con_id_error_count_arg;
                   break;
            }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;
           return newCmd;
        }

        if(strcmp(con_cmd, "ppc_osc") == 0){
            switch(con_arg_count){
                case 0:
                    newCmd.cmdId = ppc_id_get_osc;
                    break;

                default:
                    newCmd.cmdId = con_id_error_count_arg;
                    break;
            }

            con_arg_toolong = FALSE;
            con_entry_flag = FALSE;
            return newCmd;
        }

        if(strcmp(con_cmd, "ppc_enwdt") == 0){
           switch(con_arg_count)
               {
                       int i;
               case 1:
                   i=atoi(con_args[0]);
                   newCmd.cmdId = ppc_id_set_wdt_state;
                   newCmd.param = i;
                   break;

               default:
                   newCmd.cmdId = con_id_error_count_arg;
                   break;
           }
           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;
           return newCmd;
        }

       /*----------------------- TRX COMMAND ------------------------*/
       /*TRX READ REG*/
       if(strcmp(con_cmd, "trx_cg") == 0)
       {
           if(con_arg_count == 0)
           {
                newCmd.cmdId=trx_id_read_conf;
                newCmd.param = 0;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

        /*TRX PING <PARAM>*/
        if(strcmp(con_cmd, "trx_ping") == 0)
        {
           if(con_arg_count == 1)
           {
               newCmd.cmdId=trx_id_ping;
               newCmd.param = atoi(con_args[0]); /* To node X */;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg; /*invalid count arg*/
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;
           return newCmd;
       }

        /*TRX send <PARAM>*/
        if(strcmp(con_cmd, "trx_send") == 0)
        {
           if(con_arg_count > 0)
           {
               int len = strlen(con_args[0]);
               csp_transaction(1, 10, 10, 5000, con_args[0], len, NULL, 0);
               
           }
           else
           {
               newCmd.cmdId = tcm_id_testframe; /*invalid count arg*/
               newCmd.param = 0;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;
           return newCmd;
       }

       /*TRX GET STATUS */
       if(strcmp(con_cmd, "trx_gs") == 0)
       {
           if(con_arg_count == 0)
           {
                newCmd.cmdId=trx_id_getstatus;//0x3004;
                newCmd.param = 1;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
               newCmd.param = 0;
           }
           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*TRX SET BEACON */
       if(strcmp(con_cmd, "trx_mtext") == 0)
       {
           if(con_arg_count == 0)
           {
                newCmd.cmdId = trx_id_set_beacon;
                newCmd.param = 1;
           }
           else if(con_arg_count == 1)
           {
                char* end;
                newCmd.cmdId = trx_id_set_beacon;
                newCmd.param = (int)strtol(con_args[0], &end, 0);
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

        /*TRX SET BEACON LEVEL*/
       if(strcmp(con_cmd, "trx_set_beacon_lvl") == 0)
       {
           if(con_arg_count == 1)
           {
                char* end;
                newCmd.cmdId = trx_id_set_beacon_level;
                newCmd.param = (uint16_t)strtol(con_args[0], &end, 0);
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*TRX INITIALIZE */
       if(strcmp(con_cmd, "trx_init") == 0)
       {
           if(con_arg_count == 0)
           {
                newCmd.cmdId=trx_id_initialize;//0x3007;
                newCmd.param = 1;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*----------------------- EPS COMMANDS ------------------------*/
       if(strcmp(con_cmd, "eps_readreg") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               if(StrIsInt(con_args[0]) && i<36){
                   newCmd.cmdId=0x4000;
                   newCmd.param = i;
               }
               else{
                   newCmd.cmdId = con_id_error_invalid_arg;
               }
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*----------------------- RTC COMMANDS ------------------------*/
       /*RTC SET YEAR */
       if(strcmp(con_cmd, "rtc_set_year") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId=0x7000; //Adjust year
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

        /*RTC SET MONTH */
       if(strcmp(con_cmd, "rtc_set_month") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId=rtc_id_adjust_month; //Adjust month
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC SET DAY */
       if(strcmp(con_cmd, "rtc_set_day") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId=rtc_id_adjust_day; //Adjust day
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC SET WEEKDAY */
       if(strcmp(con_cmd, "rtc_set_weekday") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId = rtc_id_adjust_weekday; //Adjust weekday
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC SET HOUR */
       if(strcmp(con_cmd, "rtc_set_hour") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId = rtc_id_adjust_hour; //Adjust hour
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC SET MIN */
       if(strcmp(con_cmd, "rtc_set_minutes") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId = rtc_id_adjust_minutes; //Adjust minutes
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC SET SECONDS */
       if(strcmp(con_cmd, "rtc_set_seconds") == 0)
       {
           if(con_arg_count == 1)
           {
               int i;
               i=atoi(con_args[0]);
               newCmd.cmdId = rtc_id_adjust_minutes; //Adjust seconds
               newCmd.param = i;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*RTC PRINT*/
       if(strcmp(con_cmd, "rtc_print") == 0)
       {
           if(con_arg_count == 0)
           {
               newCmd.cmdId = rtc_id_print; //Adjust seconds
               newCmd.param = 1;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /* RTC RESET */
       if(strcmp(con_cmd, "rtc_reset") == 0)
       {
           if(con_arg_count == 0)
           {
               newCmd.cmdId=0x7008; //Adjust seconds
               newCmd.param = 1;
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /*EEPROM_AVLIVE PRINT*/
       if(strcmp(con_cmd, "test_eeprom") == 0)
       {
           if(con_arg_count == 0)
           {
               memEEPROM_isAlive(); //TODO: FIXME
           }
           else
           {
               newCmd.cmdId = con_id_error_count_arg;
           }

           con_arg_toolong = FALSE;
           con_entry_flag = FALSE;

           return newCmd;
       }

       /* ------ To get here the cmd has to fail all the comparisons  ------*/
       newCmd.cmdId = con_id_error_unknown_cmd; /* con_error_unknown_cmd */

    }else
        if(con_cmd_toolong_flag){
            newCmd.cmdId = con_id_error_cmd_toolong;

            con_cmd_toolong_flag = FALSE;
        }

    con_arg_toolong = FALSE;
    con_entry_flag = FALSE;

    return newCmd;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE INITIALIZATION
 *------------------------------------------------------------------------------
 * Description        : Initialize the arrays and peripheral necessary to run
 *                      the console.
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_init(void)
{
    unsigned char i;

    for(i = 0; i < CON_BUF_WIDTH; i++){
        con_buf[i] = '\0';
    }
    con_buf_index = 0;
    con_prev_char = 0;

    con_escape_flag = FALSE;
    con_arrow_flag = FALSE;

    con_init_flag = FALSE;
    con_active = FALSE;

    con_cmd_toolong_flag = FALSE;

    return;
}
/*------------------------------------------------------------------------------
 *		 	CONSOLE CHAR HANDLER
 *------------------------------------------------------------------------------
 * Description        : Process any new character entered in the console
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_char_handler(char newchar){

    if((newchar>=0x20 || newchar == 0x0D || newchar == 0x08 || newchar == 0x0A)
            && !con_escape_flag){
        con_buf[(int)con_buf_index] = newchar;
        con_buf_index++;
    }

    switch(newchar){
        case 0x08:      //Backspace
            if(con_buf_index>=2 && !con_escape_flag){
                con_buf_index -= 2; //Erase last char
                con_putc(&newchar);
                con_putc(" ");
                con_putc(&newchar);
            }else{
                con_buf_index = 0;
            }

            break;
        case '\r':      //Carriage Return
            if(!con_entry_flag && con_buf_index > 1 && !con_escape_flag){
                memcpy(con_entry, con_buf, con_buf_index - 1);
                con_entry[con_buf_index - 1] = '\0';//convert in string
                con_entry_index = con_buf_index - 1;
                con_buf_index = 0;
                con_entry_flag = TRUE;
            }
            break;
        case '\n':      //newline
            if(!con_entry_flag && con_buf_index > 1 && !con_escape_flag){

                memcpy(con_entry, con_buf, con_buf_index - 1);
                con_entry[con_buf_index - 1] = '\0';//convert in string
                con_entry_index = con_buf_index - 1;
                con_buf_index = 0;
                con_entry_flag = TRUE;
            }
            break;
        case '\e':
            con_escape_flag = TRUE;
            break;
        default:
            if(con_escape_flag){ //space key pressed
                if(con_arrow_flag){ //arrow
                    if(newchar == 'A'){
//                        con_hist_up();
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'B'){
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'C'){
                        //con_hist_right();NOT IMPLEMENTED
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'D'){
                        //con_hist_down();NOT IMPLEMENTED
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                }
                else{
                    if(newchar == '['){
                        con_arrow_flag = TRUE;
                    }
                }
            }
            else{
                con_putc(&newchar);
            }
            break;
    }

    if(con_buf_index >= CON_BUF_WIDTH){
        con_buf_index = 0;
        con_cmd_toolong_flag = TRUE;
        //con_printf("\r\n Command too long \r\n>>");
    }
    con_prev_char = newchar;
    return;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE COMMAND FROM ENTRY
 *------------------------------------------------------------------------------
 * Description        : Parse the entry to recognize the command and the args
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_cmd_from_entry(char *entry){
    char * pch1;
    char * pch2;
    pch2 = strchr(entry, ' ');

    con_arg_count = 0;

    if(pch2 != NULL){
        con_strncpy(con_cmd, entry, pch2-entry);

        while(pch2 != NULL){

            pch1 = pch2;

            while(*(pch1+1)==' '){
                pch1++;
            }

            pch2 = strchr(pch1+1,' ');

            if( (pch2 != NULL && pch2-pch1-1>CON_ARG_QTY )|| ( pch2 == NULL && strlen(entry)-(pch1-entry+1)>CON_ARG_QTY) ){
                con_arg_toolong = TRUE;
                return;
            }

            if(pch2 != NULL){
                con_strncpy(con_args[(int)con_arg_count], pch1+1,pch2-pch1-1);
            }
            else{
                strcpy(con_args[(int)con_arg_count],pch1+1);

            }

            con_arg_count++;
        }

    }
    else{
        strcpy(con_cmd,entry);
    }

}

/*------------------------------------------------------------------------------
 *		 			String is Int?
 *------------------------------------------------------------------------------
 * Description        : Check if a string is a representation of an integer
 * Arguments          : str - String to be check
 * Return Value       : BOOL - TRUE or FALSE
 * Example            :
 *----------------------------------------------------------------------------*/

BOOL StrIsInt(char *str){

    char *ch;

    ch = str;
    while(*ch!='\0'){
        if(*ch < 48 || *ch > 57){
            return FALSE;
        }
        ch++;
    }
    return TRUE;
}

/**
 * Set con_active flag. If true console works normally, if false console just
 * is inactive and don't respond
 * @param on_off Activate or deactivate console
 * @return current value for con_active flag
 */
BOOL con_set_active(BOOL on_off)
{
    con_active = on_off;
    return con_active;
}

/**
 * Print a string by serial console.
 * Blocking, uses mutex
 * 
 * @param c String to print
 * @deprecated Use printf instead
 */
void con_printf(char* c)
{
    printf(c);
}

/*------------------------------------------------------------------------------
 *                              UART1 INTERRUPT
 *------------------------------------------------------------------------------
 * Description        : Each character is recived by UART1 Interrupt. Console
 *                      does an echo of the character and stores the command
 * Arguments          : None
 * Return Value       : None
 * Example            :
 *----------------------------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    con_newchar = con_getc();

    if(con_active)
    con_char_handler(con_newchar);

    if(!con_init_flag)
    {
        con_init_flag = TRUE;
        con_buf_index=0;
        con_active = TRUE;
    }

    IFS0bits.U1RXIF = 0;
}
