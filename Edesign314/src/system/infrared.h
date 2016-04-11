/*
 * infrared.h
 *
 *  Created on: 03 Apr 2016
 *      Author: Daniel
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"
#include "pins.h"

// timer variable to be put inside a 444us timer
volatile uint8_t ir_rec_timer_interrupt;

// hardware interrupt detecting the beginning of an infrared data stream
volatile uint8_t ir_start_of_rx_interrupt;

// ir tx message buffer (16 bits)
volatile uint16_t ir_txMessage;

// latest ir message
volatile uint16_t ir_rxMessage;

// should be put in a timer interrupt service routine at 444us
void ir_txInterruptSR();
// should be put in a timer interrupt service routine at 889us
void ir_rxInterruptSR();

#endif /* INFRARED_H_ */
