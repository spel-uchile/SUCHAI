/**
 * NanoCom Network Interface
 *
 * @author Johan De Claville Christiansen
 * Copyright 2011 GomSpace ApS. All rights reserved.
 */

#ifndef NANOCOM_H_
#define NANOCOM_H_

#include <stdint.h>

//includes for void com_csp_initialization(void);
#include "SUCHAI_config.h"
#include "csp.h"
#include "csp_if_i2c.h"
#include "taskTest.h"

/** Default node address */
#define NODE_COM				SCH_TRX_NODE_TRX
#define NODE_TNC				SCH_TRX_NODE_TNC
#define NANOCOM_ADD             NODE_COM

/** Port numbers */
#define COM_PORT_CONF			7
#define COM_PORT_STATUS			8
#define COM_PORT_LOG_HK			9
#define COM_PORT_LOG_RSSI		10
#define COM_PORT_CALIBRATE		13
#define COM_PORT_RESTORE		14
#define COM_PORT_TX_INHIBIT		15

/** Fixed values*/
#define COM_MORSE_LEN                   20 /* DO NOT CHANGE FROM 20 */
#define TRX_TMFRAMELEN8                 64 /** Max frame lenght in bytes */
#define TRX_TMFRAMELEN16                (TRX_TMFRAMELEN8/2) /** Max frame lenght in int16*/

/** Shared data-structures */
typedef struct __attribute__((packed)) {
	uint8_t do_rs;					// 1 = turn on reed-solomon FEC, 0 = off
	uint8_t do_random;				// 1 = turn on CCSDS randomization, 0 = off
	uint8_t do_viterbi;				// 1 = turn on viterbi K=7 FEC, 0 = off
	uint8_t tx_baud;				// TX baud, [12,24,48]
	uint8_t rx_baud;				// RX baud, [12,24,48]
	int16_t tx_max_temp;			// Automatic shutoff TX at temp deg C [0 = dont check temp]
	uint16_t preamble_length;		// Time in miliseconds
	uint8_t morse_enable;			// Turn morse ON/OFF, 0 = off, other = on
	uint8_t morse_mode;				// CW = 0, FM = 1;
	uint8_t morse_cycle;			// Turn parameter cycling 1 = on, 0 = off
	uint8_t morse_en_voltage;		// Enable voltage output
	uint8_t morse_en_rx_count;		// Enable RX count output
	uint8_t morse_en_tx_count;		// Enable TX count output
	uint8_t morse_en_temp_a;		// Enable Temp A output
	uint8_t morse_en_temp_b;		// Enable Temp B output
	uint8_t morse_en_rssi;			// Enable last RSSI output
	uint8_t morse_en_rf_err;		// Enable last RF error output
	uint16_t morse_inter_delay;		// Delay between beacons, in seconds.
	uint16_t morse_pospone;			// Delay before first beacon after activity
	uint8_t morse_wpm;			// WPM after paris standard, dfl = 20
	uint8_t morse_text[COM_MORSE_LEN];	// Text to morse, terminate with '\0'
	uint16_t morse_bat_level;		// Minimum battery level, int 500 = 5.00V
	uint16_t hk_interval;			// Interval between HK sampling in seconds
} nanocom_conf_t;

typedef struct __attribute__((packed)) {
	uint32_t bit_corr_tot;			// Total bits corrected by viterbi
	uint32_t byte_corr_tot;			// Total bytes corrected by reed-solomon
	uint32_t rx;					// Total packets detected
	uint32_t rx_err;				// Total packets with error
	uint32_t tx;					// Total packets transmitted
	int16_t last_temp_a;			// Last temperature A in [C]
	int16_t last_temp_b;			// Last temperature B in [C]
	int16_t last_rssi;				// Last detected RSSI [dBm]
	int16_t last_rferr;				// Last detected RF-error [Hz]
	uint16_t last_batt_volt;		// Last sampled battery voltage [mV/10]
	uint16_t last_txcurrent;		// Last TX current [mA]
	uint32_t bootcount;				// Total bootcount
} nanocom_data_t;

typedef struct __attribute__((packed)) {
	uint32_t time;					// CPU Timestamp (processor Ticks since boot)
	int16_t rssi;					// Measured RSSI [dBm]
	int16_t rferr;					// Measured RFerr [Hz]
} nanocom_rssi_t;

typedef struct __attribute__((packed)) {
	uint32_t time;					// CPU Timestamp (processor Ticks since boot)
	int16_t temp_a;					// Temperature of A in [C]
	int16_t temp_b;					// Temperature of B in [C]
	uint16_t batt_volt;				// Battery voltage, int 712 = 7.12 V
} nanocom_hk_t;

typedef struct __attribute__((packed)) {
	int16_t raw_rssi;				// The RAW ADC value of the RSSI
	int16_t con_rssi;				// The converted value of the RSSI [dbm]
	int16_t raw_rferr;				// The RAW ADC valued of the RF-ERR
	int16_t con_rferr;				// The converted value of the RF-ERR [Hz]
} nanocom_calibrate_t;

/**
 * Send a SET_CONF message to a NanoCom or TNC
 * @param config pointer to config structure
 * @param node address of nanocom/tnc
 * @param timeout in ms
 * @return result of csp_transaction
 */
int com_set_conf(nanocom_conf_t * config, uint8_t node, uint32_t timeout);

/**
 * Send a GET_CONF message to a NanoCom or TNC,
 * and wait for a response before returning.
 * The timeout is 5 seconds.
 * @param config pointer to where retrieved config will be stored
 * @param node address of nanocom/tnc
 * @param timeout in ms
 * @return result of csp_transaction
 */
int com_get_conf(nanocom_conf_t * config, uint8_t node, uint32_t timeout);

/**
 * Send a SET_CONF_RESTORE message
 * This will delete the current stored config from the NanoCom/TNC
 * and return the device to factory settings. This is useful in the
 * event that an invalid configuration was sent to the node.
 * @param node address of nanocom/tnc
 * @return result of csp_transaction
 */
int com_restore_conf(uint8_t node);

/**
 * Send a GET_STATUS message
 * And wait for a response. Timeout is 5 seconds.
 * @param data pointer to where data will be stored
 * @param node address of nanocom/tnc
 * @param timeout in ms
 * @return result of csp_transaction
 */
int com_get_status(nanocom_data_t * data, uint8_t node, uint32_t timeout);

/**
 * Send a GET_RSSI message
 * And wait for a response. Timeout is 5 seconds
 * @param data pointer to where data will be stored, should be array of nanocom_rssi_t (with size >= 10)
 * @param count pointer to where the number of com_rssi_t structs received is stored
 * @param node address of nanocom/tnc
 * @param timeout in ms
 * @return result of csp_transaction
 */
int com_get_log_rssi(nanocom_rssi_t * data, uint8_t * count, uint8_t node, uint32_t timeout);

/**
 * Send a GET_HK message
 * And wait for a response. Timeout is 5 seconds
 * @param data pointer to where data will be stored (array size >= 8)
 * @param count poitner to number of hk entries received
 * @param node address of nanocom/tnc
 * @param timeout in ms
 * @return result of csp_transaction
 */
int com_get_hk(nanocom_hk_t * data, uint8_t * count, uint8_t node, uint32_t timeout);

/**
 * Set TX inhibit flag
 * @param node address of nanocom/tnc
 * @param value 1 = TX inhibited, 0 = TX allowed
 * @param timeout number of ms to wait for reply
 * @return 0 if ok, -1 if err
 */
int com_set_tx_inhibit(uint8_t node, uint8_t value, uint32_t timeout);

/**
 * Request calibration data
 * @param node address of nanocom/tnc
 * @param com_calibrate pointer to struct where data is returend
 * @param timeout in ms
 * @return -1 if err
 */
int com_get_calibration(uint8_t node, nanocom_calibrate_t * com_calibrate, uint32_t timeout);

/**
 * Pretty print of config structure
 * @param config
 */
void com_print_conf(nanocom_conf_t * config);

/**
 * Pretty printf of status structure
 * @param com_stat
 */
void com_printf_status(nanocom_data_t * com_stat);

/**
 * Initialize de CPS library for the SUCHAI Sw (call only once)
 */
void com_csp_initialization(void);

#endif /* NANOCOM_H_ */
