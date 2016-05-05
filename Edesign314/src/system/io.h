/*
 * io.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef IO_H_
#define IO_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

#include "../r_cg_rtc.h"

typedef enum status_b {
	status_base 				= 0b10000000,
	status_gate_closed 			= 0b10000001,
	status_gate_opened 			= 0b10000010,
	status_emergency_stopped 	= 0b10000100,
	status_remote_open 			= 0b10001000,
	status_remote_close 		= 0b10010000,
	status_card_request 		= 0b10100000,
	status_auto_close 			= 0b11000000
} status_b;

typedef enum datalog_cmd {
	cmd_remote_opening 			= 0xE1,
	cmd_remote_closing 			= 0xE2,
	cmd_card_opening 			= 0xE3,
	cmd_card_closing 			= 0xE4,
	cmd_ESTOP_pressed 			= 0xE5,
	cmd_mechanical_interference = 0xE6,
	cmd_autoclose_time 			= 0xE7,
	cmd_unspecified 			= 0xEF
} datalog_cmd;

typedef enum event_string {
	event_open,
	event_close,
	event_opened,
	event_closed,
	event_emergency_stopped,
	event_autoclose,
	event_autoclosed,
	event_mech_interference
} event_string;

static const char *event_string_array[] = {
	"    open",
	"   close",
	"  opened",
	"  closed",
	"estopped",
	"autclose",
	"auclosed",
	"  object"
};

typedef struct datalog_t {
	uint8_t used;
	datalog_cmd cmd;
	rtc_counter_value_t time;
	status_b status;
	event_string event;
} datalog_t;

void toggle_led();

#endif /* IO_H_ */
