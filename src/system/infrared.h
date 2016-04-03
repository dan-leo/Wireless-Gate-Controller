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


#define ir_half_bit_total 128
#define ir_half_bit_subtotal 28

// counter for whole message
volatile uint8_t ir_half_bit_total_counter;
// counter for message excluding idle time
volatile uint8_t ir_half_bit_subtotal_counter;

//default ir message
#define ir_message 0x37C0

void ir_InterruptSR();
void ir_pulse_train(uint8_t enabled);
void ir_reset_counters();

#endif /* INFRARED_H_ */
