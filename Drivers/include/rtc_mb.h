#ifndef _RTC_MB_H
#define _RTC_MB_H

#include "i2c_comm.h"
#include "DebugIncludes.h"  //para con_printf

//#define RTC_ID    (0b11010000)
#define RTC_ID      (0b01101000)
#define RTC_IDW     (0b11010000)
#define RTC_IDR     (0b11010001)

#define RTC_MSEC_REG 0x00
#define RTC_SEC_REG 0x01
#define RTC_MIN_REG 0x02
#define RTC_HRS_REG 0x03
#define RTC_DAY_NAME_REG 0x04
#define RTC_DAY_NUM_REG 0x05
#define RTC_MONTH_REG 0x06
#define RTC_YEAR_REG 0x07

#define RTC_FLAGS_REG   0x0F

/*
1. Tenths/Hundredths of a Second Register
2. Seconds Register
3. Minutes Register
4. Century/Hours Register
5. Day Register
6. Date Register
7. Month Register
8. Year Register
9. Calibration Register
10. Watchdog Register
11 - 15. Alarm Registers
16. Flags Register
17 - 19. Reserved
20. Square Wave Register
 */

void RTC_WriteRegister(unsigned int reg, char val);
unsigned char RTC_ReadRegister(unsigned int reg);


int RTC_init(void);
int RTC_restart_osc(void);
int RTC_isAlive(void);

int RTC_get_milliseconds(void);
int RTC_get_seconds(void);
int RTC_get_minutes(void);
int RTC_get_hours(void);
int RTC_get_week_day(void);
int RTC_get_day_num(void);
int RTC_get_month(void);
int RTC_get_year(void);

int RTC_set_date_time(int yy, int mo, int dd, int wd, int hh, int mi, int ss);

int RTC_set_year(int yy);
int RTC_set_month(int mo);
int RTC_set_day(int dd);
int RTC_set_weekday(int wd);
int RTC_set_hour(int hh);
int RTC_set_minutes(int mi);
int RTC_set_seconds(int ss);

//unsigned long int RTC_encode_datetime(void);
unsigned long int RTC_encode_datetime(int year, int month, int day, int hours, int minutes, int seconds);
int RTC_decode_datetime(unsigned long int date_time, int decode_mode);

#endif
