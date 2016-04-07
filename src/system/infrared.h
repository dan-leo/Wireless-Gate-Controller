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

// timer variable to be put inside a 444us timer
volatile uint8_t ir_rec_timer_interrupt;

// hardware interrupt detecting the beginning of an infrared data stream
volatile uint8_t ir_start_of_rx_interrupt;

//default ir tx message
//#define ir_txMessage 0x37C0
#define ir_txMessage 0x3002

// total receive ticks for a message
#define ir_quarterbit_total 56

//ir rx message buffer
volatile uint16_t ir_rxMessage;


void ir_txInterruptSR();
void ir_pulse_train(uint8_t enabled);
void ir_reset_counters();
void ir_rxInterruptSR();

#endif /* INFRARED_H_ */
