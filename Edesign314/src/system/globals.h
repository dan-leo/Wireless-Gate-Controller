/*
 * globals.h
 *
 *  Created on: 01 May 2016
 *      Author: Daniel
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#define true 1
#define false 0

#include "../r_cg_rtc.h"

#include "adc.h"
#include "buzzer.h"
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

typedef enum mode_t {
	TEST_MODE,
	NORMAL_MODE
} mode_t;

volatile mode_t mode;

// boolean for if scrolling through menu.
volatile uint8_t scrolling;

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
