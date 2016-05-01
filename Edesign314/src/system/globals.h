/*
 * globals.h
 *
 *  Created on: 01 May 2016
 *      Author: Daniel
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "adc.h"
#include "debug.h"
#include "delay.h"
#include "gate_controller.h"
#include "infrared.h"
#include "io.h"
#include "math.h"
#include "motor_driver.h"
#include "serial.h"
#include "time.h"
#include "utility.h"

uint8_t mode;

typedef enum status_b {
	status_base 		= 0b10000000,
	gate_closed 		= 0b10000001,
	gate_opened 		= 0b10000010,
	emergency_stopped 	= 0b10000100,
	remote_open 		= 0b10001000,
	remote_close 		= 0b10010000,
	card_request 		= 0b10100000,
	auto_close 			= 0b11000000
} status_b;

typedef enum datalog_cmd {
	remote_opening 			= 0xE1,
	remote_closing 			= 0xE2,
	card_opening 			= 0xE3,
	card_closing 			= 0xE4,
	ESTOP_pressed 			= 0xE5,
	mechanical_interference = 0xE6,
	autoclose_time 			= 0xE7,
	unspecified 			= 0xEF
} datalog_cmd;

typedef struct watch_t {
	uint8_t month;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} watch_t;

typedef struct datalog_t {
	uint8_t used;
	datalog_cmd cmd;
	watch_t time;
	status_b status;
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
