/*
 * eps.c
 *
 *  Created on: Sep 6, 2009
 *      Author: johan
 */

#include <string.h>

#include <nanopower.h>

//#include <util/error.h>
//#include <util/log.h>

#include <csp.h>
#include <csp_endian.h>

uint8_t node_eps = NODE_EPS;

int eps_set_node(uint8_t node) {

	node_eps = node;
	return 1;

}

int eps_get_hk(chkparam_t * chkparam) {

	int status = csp_transaction(CSP_PRIO_NORM, node_eps, EPS_PORT_HK, 5000, NULL, 0, chkparam, sizeof(chkparam_t));

	chkparam->pv[0] = csp_ntoh16(chkparam->pv[0]);
	chkparam->pv[1] = csp_ntoh16(chkparam->pv[1]);
	chkparam->pv[2] = csp_ntoh16(chkparam->pv[2]);
	chkparam->pc = csp_ntoh16(chkparam->pc);
	chkparam->bv = csp_ntoh16(chkparam->bv);
	chkparam->sc = csp_ntoh16(chkparam->sc);
	chkparam->temp[0] = csp_ntoh16(chkparam->temp[0]);
	chkparam->temp[1] = csp_ntoh16(chkparam->temp[1]);
	chkparam->temp[2] = csp_ntoh16(chkparam->temp[2]);
	chkparam->temp[3] = csp_ntoh16(chkparam->temp[3]);
	chkparam->batt_temp[0] = csp_ntoh16(chkparam->batt_temp[0]);
	chkparam->batt_temp[1] = csp_ntoh16(chkparam->batt_temp[1]);
	chkparam->latchup[0] = csp_ntoh16(chkparam->latchup[0]);
	chkparam->latchup[1] = csp_ntoh16(chkparam->latchup[1]);
	chkparam->latchup[2] = csp_ntoh16(chkparam->latchup[2]);
	chkparam->latchup[3] = csp_ntoh16(chkparam->latchup[3]);
	chkparam->latchup[4] = csp_ntoh16(chkparam->latchup[4]);
	chkparam->latchup[5] = csp_ntoh16(chkparam->latchup[5]);
	chkparam->sw_errors = csp_ntoh16(chkparam->sw_errors);
	chkparam->bootcount = csp_ntoh16(chkparam->bootcount);

	return status;

}

int eps_get_hk2(chkparam2_t * chkparam2) {
int i;
char var=1;
	int status = csp_transaction(CSP_PRIO_NORM, node_eps, EPS_PORT_HK, 5000, &var, 1, chkparam2, sizeof(chkparam2_t));

	chkparam2->Icount = csp_ntoh32(chkparam2->Icount);
	chkparam2->UpTime = csp_ntoh32(chkparam2->UpTime);
	chkparam2->TimeoutGndWDT = csp_ntoh32(chkparam2->TimeoutGndWDT);
		for (i=0;i<6;i++)
		{
			chkparam2->SwitchOnTimer[i] = csp_ntoh16(chkparam2->SwitchOnTimer[i]);
			chkparam2->SwitchOffTimer[i] = csp_ntoh16(chkparam2->SwitchOffTimer[i]);


		}
	chkparam2->PPTFixedRef[0] = csp_ntoh16(chkparam2->PPTFixedRef[0]);
	chkparam2->PPTFixedRef[1] = csp_ntoh16(chkparam2->PPTFixedRef[1]);
	chkparam2->PPTFixedRef[2] = csp_ntoh16(chkparam2->PPTFixedRef[2]);

	chkparam2->i2c_wdt_timeouts = csp_hton16(chkparam2->i2c_wdt_timeouts);
	chkparam2->gnd_wdt_timeouts = csp_hton16(chkparam2->gnd_wdt_timeouts);
	chkparam2->i2c_ping_timeouts[0] = csp_hton16(chkparam2->i2c_ping_timeouts[0]);
	chkparam2->i2c_ping_timeouts[1] = csp_hton16(chkparam2->i2c_ping_timeouts[1]);

	return status;

}

int eps_get_version(eps_version_t * version) {

	return csp_transaction(CSP_PRIO_NORM, node_eps, EPS_PORT_GET_VERSION, 5000, NULL, 0, version, sizeof(eps_version_t));
}



int eps_get_bootdelay(eps_bootdelay_t * bootdelay) {

	return csp_transaction(CSP_PRIO_NORM, node_eps, EPS_PORT_BOOTDELAY, 5000, NULL, 0, bootdelay, sizeof(eps_bootdelay_t));
}


int eps_set_bootdelay(eps_bootdelay_t * bootdelay) {

	return csp_transaction(CSP_PRIO_NORM, node_eps, EPS_PORT_BOOTDELAY, 5000, bootdelay, sizeof(eps_bootdelay_t), NULL, 0);
}





int eps_set_output(uint8_t output) {

	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_SET_OUTPUT, 5000, &output, 1, NULL, 0);

}

int eps_set_single_output(uint8_t channel, uint8_t mode, int16_t delay) {

	eps_switch_t eps_switch;
	eps_switch.channel=channel;
	eps_switch.mode = mode;
	eps_switch.delay = csp_hton16(delay);

	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_SET_SINGLE_OUTPUT, 5000, &eps_switch, 4, NULL, 0);
}

int eps_set_pv_volt(uint16_t pv1, uint16_t pv2, uint16_t pv3) {

	uint16_t pvolt[3];
	pvolt[0] = csp_hton16(pv1);
	pvolt[1] = csp_hton16(pv2);
	pvolt[2] = csp_hton16(pv3);

	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_SET_PV_VOLT, 5000, &pvolt, 6, NULL, 0);

}

int eps_set_ppt_mode(uint8_t mode) {

	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_SET_PV_AUTO, 5000, &mode, 1, NULL, 0);

}


int eps_reset_persistent(void) {

	uint8_t magic = 0x42;
	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_RESET_PERSISTENT, 0, &magic, 1, NULL, 0);

}


int eps_reset_gndwdt(void) {

	uint8_t magic = 0x78;
	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_RESET_GNDWDT, 0, &magic, 1, NULL, 0);

}

int eps_auto_heater(uint8_t mode, uint8_t * mode_reply) {

	return csp_transaction(CSP_PRIO_HIGH, node_eps, EPS_PORT_HEATER, 0, &mode, 1, mode_reply, 1);

}
