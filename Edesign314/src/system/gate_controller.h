/*
 * gate_controller.h
 *
 *  Created on: 13 Apr 2016
 *      Author: Daniel
 */

#ifndef GATE_CONTROLLER_H_
#define GATE_CONTROLLER_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"
#include "pins.h"

enum gate_position {
	CLOSED = 0x81,
	OPEN = 0x82,
	UNKNOWN = 0x84
} gate_pos_t;

volatile uint8_t gate_is_moving;
volatile uint8_t gate_position;
volatile uint8_t last_gate_position;

void gate_stop_handler();
void gate_close();
void gate_open();

#endif /* GATE_CONTROLLER_H_ */
