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

/*------------------------------------------------------------------------------
 *                              RTC_MB
 * -----------------------------------------------------------------------------
 * System ID:
 * Description: M41T81S - Real time clock
 * Includes: i2c.h
 * -----------------------------------------------------------------------------
 * Company: SPEL - SUCHAI
 * Autor:   Tomas Opazo Toro - tomas.opazo.t@gmail.com
 *          Carlos Gonzalez Cortes - carlgonz@ug.uchile.cl
 * ---------------------------------------------------------------------------*/

#include "rtc_mb.h"

//#define GET_BCD

/*------------------------------------------------------------------------------
 *                              WRITE REGISTER
 *------------------------------------------------------------------------------
 * Function Prototype : void WriteRegister(unsigned int reg, unsigned char val)
 * Include            : i2c_comm.h
 * Description        : Write some rtc's register by I2C interface
 * Arguments          : reg - register to be write
			val - value to be write
 * Return Value       : None
 * Remarks            : Use defines in header to find the register value
 *----------------------------------------------------------------------------*/
void RTC_WriteRegister(unsigned int reg, char val)
{
        char address[] = {RTC_ID, reg};
        i2c1_master_fputs(&val, 1, address, 2);
}

/*------------------------------------------------------------------------------
 *		 		READ REGISTER
 *------------------------------------------------------------------------------
 * Function Prototype : void ReadRegister(unsigned int reg, unsigned char val)
 * Include            : i2c_comm.h
 * Description        : Write some rtc's register by I2C interface
 * Arguments          : reg -register to be read
 * Return Value       : None
 * Remarks            : Use defines in header to find the register value
 *----------------------------------------------------------------------------*/
unsigned char RTC_ReadRegister(unsigned int reg)
{
	char ret;
        char address[] = {RTC_ID, (char)reg};
        i2c1_master_fgets(&ret, 1, address, 2);
	return (unsigned char)ret;
}

/*------------------------------------------------------------------------------
 *		 		INIT RTC_MB
 *------------------------------------------------------------------------------
 * Description        : initialize this hardware
 * Arguments          : none
 * Return Value       : 1 if succesfull 0 - Otherwise
 *----------------------------------------------------------------------------*/
int RTC_init(void)
{
    /*
     * OJO CON EL VALOR DE LOS FLAG 'OF' Y 'HT' AL INICIO LOS CUALES PUEDEN
     * IMPEDIR QUE SE ACTUALICEN LOS REGISTROS DEL RTC
     * 
     * VALORES DEFAULT:
     * h01:    ST = Stop Bit =0
     * h08:    FT= Frecuency Test = 0
     * OUT = Output level = 1
     * h09:    OFIE = Oscillator Fail Interrupt Enable = 0
     * h0A:    AFE = Alarm Flag Enable Flag=0
     * SQWE = Square Wave Enable=0
     * ABE = Alarm in Battery Back-up Mode Enable Bit=0,
     */

    /* Checking oscilator */
    unsigned char reg;
    reg = RTC_ReadRegister(RTC_SEC_REG);
    reg = reg & 0x80;
    
    /* Restarting the oscilator */
    if(reg)
    {
        printf(">>RTC Restarting cristal\n\r");
        RTC_WriteRegister(RTC_SEC_REG, 0x80);
        RTC_WriteRegister(RTC_SEC_REG, 0x00);

        /* Wait before clear the flag (See M41T81S Datasheet) */
        long i;
        for(i = 0xFFFFFF; i>0; i--);

        /* Clear OF flag */
        RTC_WriteRegister(RTC_FLAGS_REG,0x00);
    }

    /* Check HALT Bit */
    reg = RTC_ReadRegister(0x0C);
    reg = reg & 0x40;

    if(reg)
    {
#if _VERBOSE_
        con_printf(">>RTC cleaning HT\n\r");
#endif
        /* Reset HALT Bit (set if power-down) */
        RTC_WriteRegister(0x0C, 0x00);
    }

    return RTC_isAlive();
}
/**
 * Check if the RTC is working
 * @return 1 = is working 0 = not working
 */
int RTC_isAlive(void){
    //verificar correcto funcionamiento
    int seg1 = RTC_get_seconds();

    /* Wait some seconds  */
//    unsigned long i;
//    for(i = 0x004FFFFF; i>0; i--){}
    __delay_ms(3000);

    int seg2 = RTC_get_seconds();
    if(seg1!=seg2){
        return 1;
    }
    else{
        return 0;
    }
}

/*------------------------------------------------------------------------------
 *		 		RTC RESTART OSC
 *------------------------------------------------------------------------------
 * Description        : Check the OF flag. If OF==1 then restart the osc and 
 *                      clear the OF flag.
 * Arguments          : None
 * Return Value       : Initial OF flag bit
 * Note               : This function takes at least 4 second to restart the osc
 *                      and then clear the flag
 *----------------------------------------------------------------------------*/
int RTC_restart_osc(void)
{
    int ret;
    ret = RTC_ReadRegister(RTC_FLAGS_REG);

    /* Get the OF flag bit (bit number 3)*/
    ret = ret >> 2;
    ret = ret & 0x01;

    if(ret)
    {
        /* Restarting the clock */
        RTC_WriteRegister(RTC_SEC_REG,0x80);
        RTC_WriteRegister(RTC_SEC_REG,0x00);

        /* Wait before clear the flag (See M41T81S Datasheet) */
        long i;
        for(i = 0x00FFFFFF; i>0; i--);

        /* Clear OF flag */
        RTC_WriteRegister(RTC_FLAGS_REG,0x00);
    }

    return ret;
}

/*------------------------------------------------------------------------------
 *		 		RTC GET HOURS
 *------------------------------------------------------------------------------
 * Description        : Return current hours
 * Arguments          : none
 * Return Value       : hours as integer
 *----------------------------------------------------------------------------*/
int RTC_get_hours(void)
{
    unsigned char dat;
    unsigned int hours;

    dat=RTC_ReadRegister(RTC_HRS_REG);
    // Unidades
    hours = dat & 0x0F;
    // Decenas y sumo
    hours += ((dat & 0b00110000) >> 4)*10;


#ifdef GET_BCD
    return dat;
#else
    return hours;
#endif
}

/*------------------------------------------------------------------------------
 *		 		RTC GET WEEK DAY
 *------------------------------------------------------------------------------
 * Description        : Return current week day
 * Arguments          : none
 * Return Value       : Week day as integer
 *----------------------------------------------------------------------------*/
int RTC_get_week_day(void)
{
    unsigned char dat;
    dat=RTC_ReadRegister(RTC_DAY_NAME_REG);

    return dat;
}

/*------------------------------------------------------------------------------
 *		 		RTC GET DAY NUM
 *------------------------------------------------------------------------------
 * Description        : Return current day number
 * Arguments          : none
 * Return Value       : day of the month number as integer
 *----------------------------------------------------------------------------*/
int RTC_get_day_num(void)
{
    unsigned char dat;
    unsigned int day;

    dat=RTC_ReadRegister(RTC_DAY_NUM_REG);
    //obtengo decenas
    day = dat & 0x0F;
    day += ((dat & 0xF0) >> 4)*10;

#ifdef GET_BCD
    return dat;
#else
    return day;
#endif
}

/*------------------------------------------------------------------------------
 *		 		RTC GET YEAR
 *------------------------------------------------------------------------------
 * Description        : Return current year
 * Arguments          : none
 * Return Value       :  yer as integer
 *----------------------------------------------------------------------------*/
int RTC_get_year(void)
{
    unsigned char dat;
    unsigned int year;

    dat=RTC_ReadRegister(RTC_YEAR_REG);

    year = dat & 0x0F;
    year += ((dat & 0xF0) >> 4)*10;

#ifdef GET_BCD
    return dat;
#else
    return year;
#endif
}

/*------------------------------------------------------------------------------
 *		 		RTC GET MILLISECONDS
 *------------------------------------------------------------------------------
 * Description        : Return current milleseconds
 * Arguments          : none
 * Return Value       : Milliseconds as integer
 *----------------------------------------------------------------------------*/
int RTC_get_milliseconds(void)
{
    unsigned char value;
    unsigned int millisec;

    /* Read register */
    value = RTC_ReadRegister(RTC_MSEC_REG);     /* value format is: DDDDUUUU*/

    /* Obtener unidades */
    millisec = value & 0x0F;
    /* Obtener decenas */
    millisec += ((value & 0xF0)>>4)*10;

#ifdef GET_BCD
    return value;
#else
    return millisec;
#endif
    
}

/*------------------------------------------------------------------------------
 *		 		RTC_GET_SECONDS
 *------------------------------------------------------------------------------
 * Description        : Return curretn seconds
 * Arguments          : none
 * Return Value       : Seconds as integer
 *----------------------------------------------------------------------------*/
int RTC_get_seconds(void)
{
    unsigned char value;
    unsigned int second;

    /* Read register */
    value = RTC_ReadRegister(RTC_SEC_REG);     /* value format is: SDDDUUUU*/

    /* Obtener unidades */
    second = value & 0x0F;
    /* Obtener decenas */
    second += ((value & 0x70)>>4)*10;

#ifdef GET_BCD
    return value;
#else
    return second;
#endif
    
}

/*------------------------------------------------------------------------------
 *		 		RTC_GET_MINUTES
 *------------------------------------------------------------------------------
 * Description        : Return curretn minutes
 * Arguments          : none
 * Return Value       : minutes as integer
 *----------------------------------------------------------------------------*/
int RTC_get_minutes(void)
{
    unsigned char value;
    unsigned int minutes;

    /* Read register */
    value = RTC_ReadRegister(RTC_MIN_REG);     /* value format is: SDDDUUUU*/

    /* Obtener unidades */
    minutes = value & 0x0F;
    /* Obtener decenas */
    minutes += ((value & 0xF0)>>4)*10;

#ifdef GET_BCD
    return value;
#else
    return minutes;
#endif
}

/*------------------------------------------------------------------------------
 *		 		RTC_GET_MONTH
 *------------------------------------------------------------------------------
 * Description        : Return current month
 * Arguments          : none
 * Return Value       : month as integer
 *----------------------------------------------------------------------------*/
int RTC_get_month(void)
{
    unsigned char value;
    unsigned int month;

    /* Read register */
    value = RTC_ReadRegister(RTC_MONTH_REG);     /* value format is: DDDDUUUU*/

    /* Obtener unidades */
    month = value & 0x0F;
    /* Obtener decenas */
    month += ((value & 0xF0)>>4)*10;

#ifdef GET_BCD
    return value;
#else
    return month;
#endif
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_DATE_TIME
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC data and time
 * Arguments          : int yy: year 0-99
 *                      int mo: month 1-12
 *                      int dd: month day 1-31
 *                      int wd: day of the week 1-7
 *                      int hh: hours 0-23
 *                      int mi: minutes 0-59
 *                      int ss: seconds 0-59
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_date_time(int yy, int mo, int dd, int wd, int hh, int mi, int ss)
{
    int tmp;

    /* Revisar rango de argumentos */
    if((yy > 100) || (mo > 12) || (dd > 31) || (wd > 7) || (hh > 23) || (mi > 60) || (ss > 60))
        return 0;
    if((yy < 0) || (mo < 1) || (dd < 1) || (wd < 1) || (hh < 0) || (mi < 0) || (ss < 0))
        return 0;

    /* Setting year */
    tmp = yy / 10; /* Obtener decena */
    yy  = yy % 10; /* Obtener unidad */
    yy = yy | (tmp<<4);
    RTC_WriteRegister(RTC_YEAR_REG, (unsigned char)yy);

    /* Setting month */
    tmp = mo / 10;
    mo = mo % 10;
    mo = mo | (tmp<<4);
    RTC_WriteRegister(RTC_MONTH_REG, (unsigned char)mo);

    /* Setting month day */
    tmp = dd / 10;
    dd = dd % 10;
    dd = dd | (tmp<<4);
    RTC_WriteRegister(RTC_DAY_NUM_REG, (unsigned char)dd);

    /* Setting week day */
    RTC_WriteRegister(RTC_DAY_NAME_REG, (unsigned char)wd);

    /* Setting hour */
    tmp = hh / 10;
    hh = hh % 10;
    hh = hh | (tmp<<4);
    RTC_WriteRegister(RTC_HRS_REG, (unsigned char)hh);

    /* Setting minutes */
    tmp = mi / 10;
    mi = mi % 10;
    mi = mi | (tmp<<4);
    RTC_WriteRegister(RTC_MIN_REG, (unsigned char)mi);

    /* Setting seconds */
    tmp = ss / 10;
    ss = ss % 10;
    ss = ss | (tmp<<4);
    RTC_WriteRegister(RTC_SEC_REG, (unsigned char)ss);

    /* Setting milliseconds */
    RTC_WriteRegister(RTC_MSEC_REG, (unsigned char)0x00);
    
    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_YEAR
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC year
 * Arguments          : int yy: year 0-99
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_year(int yy)
{
    int tmp;

    /* Revisar rango de argumentos */
    if((yy > 100) || (yy < 0))
        return 0;

    /* Setting year */
    tmp = yy / 10; /* Obtener decena */
    yy  = yy % 10; /* Obtener unidad */
    yy = yy | (tmp<<4);
    RTC_WriteRegister(RTC_YEAR_REG, (unsigned char)yy);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_MONTH
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC month
 * Arguments          : int mo: month 1-12
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_month(int mo)
{
    int tmp;

    if((mo > 12) || (mo < 1))
        return 0;

    /* Setting month */
    tmp = mo / 10;
    mo = mo % 10;
    mo = mo | (tmp<<4);
    RTC_WriteRegister(RTC_MONTH_REG, (unsigned char)mo);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_DAY
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC day
 * Arguments          : int dd: day 1-31
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_day(int dd)
{
    int tmp;

    if((dd > 31) || (dd < 1))
        return 0;

    /* Setting month day */
    tmp = dd / 10;
    dd = dd % 10;
    dd = dd | (tmp<<4);
    RTC_WriteRegister(RTC_DAY_NUM_REG, (unsigned char)dd);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_WEEKDAY
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC weekday
 * Arguments          : int wd: day 1-7
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_weekday(int wd)
{
    if((wd > 7) || (wd < 1))
        return 0;

    /* Setting week day */
    RTC_WriteRegister(RTC_DAY_NAME_REG, (unsigned char)wd);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_HOUR
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC hour
 * Arguments          : int hh: day 0-23
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_hour(int hh)
{
    int tmp;

    if((hh > 23) || (hh < 0))
        return 0;

    /* Setting hour */
    tmp = hh / 10;
    hh = hh % 10;
    hh = hh | (tmp<<4);
    RTC_WriteRegister(RTC_HRS_REG, (unsigned char)hh);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_MINUTES
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC hour
 * Arguments          : int MI: day 0-60
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_minutes(int mi)
{
    int tmp;

    if((mi > 60) || (mi < 0))
        return 0;
    /* Setting minutes */
    tmp = mi / 10;
    mi = mi % 10;
    mi = mi | (tmp<<4);
    RTC_WriteRegister(RTC_MIN_REG, (unsigned char)mi);

    return 1;
}

/*------------------------------------------------------------------------------
 *		 		RTC_SET_SECONDS
 *------------------------------------------------------------------------------
 * Description        : Adjust RTC seconds
 * Arguments          : int hh: day 0-23
 * Return Value       : 1 - OK
 *                      0 - Arguments out of range
 *----------------------------------------------------------------------------*/
int RTC_set_seconds(int ss)
{
    int tmp;

    if((ss > 60) || (ss < 0))
        return 0;

    /* Setting seconds */
    tmp = ss / 10;
    ss = ss % 10;
    ss = ss | (tmp<<4);
    RTC_WriteRegister(RTC_SEC_REG, (unsigned char)ss);

    // Settings milliseconds
    RTC_WriteRegister(RTC_MSEC_REG, (unsigned char)0x00);

    return 1;
}


unsigned long int RTC_encode_datetime(int year, int month, int day, int hours, int min, int sec){

//    int year = RTC_get_year();
//    int month = RTC_get_month();
//    int day = RTC_get_day_num();
//    int hours = RTC_get_hours();
//    int minutes = RTC_get_minutes();
//    int seconds = RTC_get_seconds();

    //clean any extra bit
    //              7654321076543210
    year =   year&0b0000000000111111;
    //              7654321076543210
    month = month&0b0000000000001111;
    //              7654321076543210
    day =     day&0b0000000000011111;
    //              7654321076543210
    hours = hours&0b0000000000011111;
    //              7654321076543210
    min =     min&0b0000000000111111;
    //              7654321076543210
    sec =     sec&0b0000000000111111;

    unsigned long int tmp = 0, date_time = 0;
    //include year,  6 bits
    tmp = (unsigned long int)year;
    date_time = date_time<<0;
    date_time = date_time|tmp;
    //include month,  4 bits
    tmp  = (unsigned long int)month;
    date_time = date_time<<4;
    date_time = date_time|tmp;
    //include day,  5 bits
    tmp  = (unsigned long int)day;
    date_time = date_time<<5;
    date_time = date_time|tmp;
    //include hours,  5 bits
    tmp  = (unsigned long int)hours;
    date_time = date_time<<5;
    date_time = date_time|tmp;
    //include minutes, 6 bits
    tmp  = (unsigned long int)min;
    date_time = date_time<<6;
    date_time = date_time|tmp;
    //include seconds, 6 bits
    tmp  = (unsigned long int)sec;
    date_time = date_time<<6;
    date_time = date_time|tmp;
    

    return date_time;
}

int RTC_decode_datetime(unsigned long int date_time, int decode_mode){
    
    //                      76543210765432107654321076543210
    int yy   = (date_time&0b11111100000000000000000000000000)>>26 ;
    //                      76543210765432107654321076543210
    int mo   = (date_time&0b00000011110000000000000000000000)>>22 ;
    //                      76543210765432107654321076543210
    int dd   = (date_time&0b00000000001111100000000000000000)>>17 ;
    //                      76543210765432107654321076543210
    int hour = (date_time&0b00000000000000011111000000000000)>>12 ;
    //                      76543210765432107654321076543210
    int min  = (date_time&0b00000000000000000000111111000000)>>6 ;
    //                      76543210765432107654321076543210
    int sec  = (date_time&0b00000000000000000000000000111111)>>0 ;

    int resp;
    switch( decode_mode){
        case 1:
            resp = yy;
            break;
        case 2:
            resp = mo;
            break;
        case 3:
            resp = dd;
            break;
        case 4:
            resp = hour;
            break;
        case 5:
            resp = min;
            break;
        case 6:
            resp = sec;
            break;
        default:
            printf(">> date_time = %lu <=> %d/%d/%d %d:%d:%d\n", date_time, dd, mo, yy, hour, min, sec);
            resp = -1;
            break;
    }
    return resp;
}