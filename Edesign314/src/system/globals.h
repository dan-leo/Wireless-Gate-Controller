/*
 * globals.h
 *
 *  Created on: 01 May 2016
 *      Author: Daniel
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "../r_cg_rtc.h"

#include "adc.h"
#include "debug.h"
#include "delay.h"
#include "gate_controller.h"
#include "infrared.h"
#include "io.h"
#include "lcd.h"
#include "math.h"
#include "motor_driver.h"
#include "serial.h"
#include "time.h"
#include "utility.h"

uint8_t mode;

#define def_event_open				0
#define def_event_close				1
#define def_event_opened			2
#define def_event_closed			3
#define def_event_emergency_stopped	4
#define def_event_autoclose			5
#define def_event_autoclosed		6
#define def_event_mech_interference	7

#define EVENT_STR(def_event_str) \
    (def_event_open       						== def_event_str ? "    open"  	: \
     (def_event_close     						== def_event_str ? "   close"  	: \
      (def_event_opened   						== def_event_str ? "  opened"  	: \
    	(def_event_closed   					== def_event_str ? "  closed"  	: \
    	  (def_event_emergency_stopped  	 	== def_event_str ? "  estopped"  : \
    		(def_event_autoclose   				== def_event_str ? "  autclose"  : \
    		  (def_event_autoclosed   			== def_event_str ? "  auclosed"  : \
    		    (def_event_mech_interference 	== def_event_str ? "object" : "unknown"))))


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

//typedef struct watch_t {
//	uint8_t month;
//	uint8_t days;
//	uint8_t hours;
//	uint8_t minutes;
//	uint8_t seconds;
//} watch_t;

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

// set time
// 04  10  08  01  05  F2

// read time
// F3 > F3 04  10  08  01  05
// Read_Clock = 16 Apr 8:1:5 (h:m:s)

// erase datalog
// F5

// read datalog
// F6

#endif /* GLOBALS_H_ */
