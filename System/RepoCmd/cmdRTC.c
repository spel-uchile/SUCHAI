/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
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

#include "cmdRTC.h"

extern xSemaphoreHandle rtcPrintSem;

cmdFunction rtcFunction[RTC_NCMD];
int rtc_sysReq[RTC_NCMD];

void rtc_onResetCmdRTC(void){
    printf("        rtc_onResetCmdRTC\n");

    rtcFunction[(unsigned char)rtc_id_adjust_year] = rtc_adjust_year;
    rtc_sysReq[(unsigned char)rtc_id_adjust_year]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_month] = rtc_adjust_month;
    rtc_sysReq[(unsigned char)rtc_id_adjust_month]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_day] = rtc_adjust_day;
    rtc_sysReq[(unsigned char)rtc_id_adjust_day]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_weekday] = rtc_adjust_weekday;
    rtc_sysReq[(unsigned char)rtc_id_adjust_weekday]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_hour] = rtc_adjust_hour;
    rtc_sysReq[(unsigned char)rtc_id_adjust_hour]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_minutes] = rtc_adjust_minutes;
    rtc_sysReq[(unsigned char)rtc_id_adjust_minutes]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_adjust_seconds] = rtc_adjust_seconds;
    rtc_sysReq[(unsigned char)rtc_id_adjust_seconds]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_print] = rtc_print;
    rtc_sysReq[(unsigned char)rtc_id_print]  = CMD_SYSREQ_MIN;
    rtcFunction[(unsigned char)rtc_id_reset] = rtc_reset;
    rtc_sysReq[(unsigned char)rtc_id_reset]  = CMD_SYSREQ_MIN;
}

/**
 * Adjust RTC year
 * @param param Year 0-99
 * @return 1 - Ok
 */
int rtc_adjust_year(void *param)
{
    return RTC_set_year(*(int *)param);
}

/**
 * Adjust RTC moth
 * @param param month 0-12
 * @return 1-OK
 */
int rtc_adjust_month(void *param)
{
    return RTC_set_month(*(int *)param);
}

/**
 * Adjust RTC day
 * @param param day 1-31
 * @return 1-Ok
 */
int rtc_adjust_day(void *param)
{
    return RTC_set_day(*(int *)param);
}

/**
 * Adjust RTC week day
 * @param param day of the week 1-7
 * @return 1-Ok
 */
int rtc_adjust_weekday(void *param)
{
    return RTC_set_weekday(*(int *)param);
}

/**
 * Adjust RTC hour
 * @param param hour 0-23
 * @return 1-Ok
 */
int rtc_adjust_hour(void *param)
{
    return RTC_set_hour(*(int *)param);
}

/**
 * Adjust RTC minutes
 * @param param minutes 0-59
 * @return 1-Ok
 */
int rtc_adjust_minutes(void *param)
{
    return RTC_set_minutes(*(int *)param);
}

/**
 * Adjust RTC seconds
 * @param param seconds 0-59
 * @return 1-Ok
 */
int rtc_adjust_seconds(void *param)
{
    return RTC_set_seconds(*(int *)param);
}

/**
 * Print current RCT value in a friendly way
 * @param param Not used
 * @return 1-Ok
 */
int rtc_print(void *param)
{
    int sec, min, hour, mo, dd, yy;

    /* Critical seccion */
    //TODO: Check this, probabbly is working as desired
    xSemaphoreTake(rtcPrintSem, portMAX_DELAY);
    sec = RTC_get_seconds();
    min = RTC_get_minutes();
    hour = RTC_get_hours();
    mo = RTC_get_month();
    dd = RTC_get_day_num();
    yy = RTC_get_year();
    xSemaphoreGive(rtcPrintSem);

    RTC_ReadRegister(RTC_FLAGS_REG);
    printf(">> %d/%d/%d %d:%d:%d\n", dd, mo, yy, hour, min, sec);

//    unsigned long int date_time = RTC_encode_datetime(yy, mo, dd, hour, min, sec);
//    RTC_decode_datetime(date_time, 0);

    return 1;
}

/**
 * Reset and restart the clock in case of mal function
 * @param param Not used
 * @return 1-Ok
 */
int rtc_reset(void *param)
{
    return RTC_init();
}

