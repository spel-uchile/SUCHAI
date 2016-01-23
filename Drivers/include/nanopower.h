/*
 * eps.h
 *
 *  Created on: Sep 6, 2009
 *      Author: johan
 */

#ifndef EPS_H_
#define EPS_H_

#include <stdint.h>
#include "SUCHAI_config.h"

/* Converted house keeping parameters  */
typedef struct __attribute__ ((__packed__)) chousekeeping_parameters {
	uint16_t pv[3];			//Photo-voltaic input voltage [mV]
	uint16_t pc;			//Total photo current [mA]
	uint16_t bv;			//Battery voltage [mV]
	uint16_t sc;			//Total system current [mA]
	int16_t  temp[4];	    //Temp. of boost converters and on-board battery [degC] (Conv 1, Conv 2, Conv 3, Board)
	int16_t  batt_temp[2];	//External board battery temperatures [degC];
	uint16_t latchup[6];	//Number of latch-ups on each output 5V and +3V3 channel [5V1 5V2 5V3 3.3V1 3.3V2 3.3V3]
	uint8_t	 reset;		    //Cause of last EPS reset
	uint16_t bootcount;     //Number of EPS reboots
	uint16_t sw_errors;		//Number of errors in the eps software
	uint8_t  ppt_mode;		//0 = Hardware, 1 = MPPT, 2 = Fixed SW PPT.
	uint8_t  channel_status;//Mask of output channel status, 1=on, 0=off [MSB NC NC 5V1 5V2 5V3 3.3V1 3.3V2 3.3V3 LSB]
} chkparam_t;
extern chkparam_t chkparam;


typedef struct __attribute__ ((__packed__)) chousekeeping2_parameters {
uint8_t  SOC;               //State of charge [%]
int32_t  Icount;            //Coloumb count [mAs] AmpereSeconds
uint32_t UpTime;            //Uptime [s]
uint8_t  Mode;              // Mode byte for EPS, [x x x x x x HighVoltProtect LowVoltProtect]
uint32_t TimeoutGndWDT;     // Seconds ?
int16_t  SwitchOnTimer[6];  //Seconds*10 ?
int16_t  SwitchOffTimer[6]; //Seconds*10 ?
uint16_t PPTFixedRef[3];    //mV
uint16_t i2c_wdt_timeouts;  //
uint16_t gnd_wdt_timeouts;   //
uint16_t i2c_ping_timeouts[2]; //
} chkparam2_t;
extern chkparam2_t chkparam2;


typedef struct __attribute__((packed)) this_eps_version {
	char type[4];
	char ver[5];
	char date[12];
	char time[9];
} eps_version_t;


typedef struct __attribute__((packed)) this_eps_uid {
	uint32_t customer;
	uint64_t uid;
} eps_uid_t;


typedef struct __attribute__((packed)) {
	uint8_t channel;
	uint8_t mode;
	int16_t delay;
} eps_switch_t;

typedef struct __attribute__((packed)) {
	int16_t delay[6];
} eps_bootdelay_t;

/* CSP Address */
#define NODE_EPS	(SCH_TRX_NODE_EPS)

/* CSP Port numbers */
#define EPS_PORT_RESET  7
#define EPS_PORT_HK	8
#define EPS_PORT_SET_OUTPUT 9
#define EPS_PORT_SET_SINGLE_OUTPUT 10
#define EPS_PORT_SET_PV_VOLT 11
#define EPS_PORT_SET_PV_AUTO 12
#define EPS_PORT_HEATER 13
#define EPS_PORT_GET_VERSION 14
#define EPS_PORT_RESET_PERSISTENT 15
#define EPS_PORT_RESET_GNDWDT 16
#define EPS_PORT_BOOTDELAY 17

/* EPS magic reset number */
#define EPS_RESET_MAGIC 0xA4B8F2D9;

int eps_set_node(uint8_t node);
int eps_get_hk(chkparam_t * hk);
int eps_get_hk2(chkparam2_t * hk2);
int eps_set_output(uint8_t output);
int eps_set_single_output(uint8_t channel, uint8_t mode, int16_t delay);
int eps_set_pv_volt(uint16_t pv1, uint16_t pv2, uint16_t pv3);

/**
 * Set PPT mode
 * @param mode 0 = OFF, 1 = AUTO, 2 = FIXED
 * @return
 */
int eps_set_ppt_mode(uint8_t mode);
int eps_reset(void);
int eps_get_version(eps_version_t * version);
int eps_reset_persistent(void);
int eps_reset_gndwdt(void);
int eps_auto_heater(uint8_t mode, uint8_t * mode_reply);
int eps_get_bootdelay(eps_bootdelay_t * bootdelay);
int eps_set_bootdelay(eps_bootdelay_t * bootdelay);

#endif /* EPS_H_ */
