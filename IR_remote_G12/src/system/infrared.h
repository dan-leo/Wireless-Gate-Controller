/*
 * infrared.h
 *
 *  Created on: 11 Apr 2016
 *      Author: Daniel
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "../r_cg_userdefine.h"
#include "../r_cg_macrodriver.h"

#include "pins.h"

// ir tx message buffer (16 bits)
volatile uint16_t ir_txMessage;

void ir_txInterruptSR();
void button_interrupt(uint8_t button);

#endif /* INFRARED_H_ */
