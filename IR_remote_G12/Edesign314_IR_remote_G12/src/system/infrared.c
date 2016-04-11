/*
 * infrared.c
 *
 *  Created on: 11 Apr 2016
 *      Author: Daniel
 */

#include "infrared.h"

//default ir tx message
#define ir_txMessage_default 0x37C0

/*
 * I try to make names self explanatory :D
 */
#define ir_64bitMessage_inHalfBits_total 128
#define ir_14bitMessage_inHalfBits_total 28

void ir_enable_pulse(uint8_t enabled);

/**
 * this interrupt service routine will transmit an infra red message in
 * manchester RC-5 format (1 bit in 1778us).
 */
void ir_txInterruptSR(){
	static uint8_t setup = 1;
	if (setup){
		ir_txMessage = ir_txMessage_default;
		setup--;
	}
	// counter for whole message
	static volatile uint8_t ir_64bitMessage_inHalfBits_counter = 128;
	// counter for message excluding idle time
	static volatile uint8_t ir_14bitMessage_inHalfBits_counter = 28;
	// 1 bit buffer
	volatile uint8_t bit;
	if (ir_64bitMessage_inHalfBits_counter){
		if (ir_14bitMessage_inHalfBits_counter){
			// shift desired bit to lowest LSB
			bit = (ir_txMessage >> ((ir_14bitMessage_inHalfBits_counter + 1)/2 - 1));

			bit &= 0x1;  // select the lowest LSB only

			// enables pulse train if 1 else disable it
			// ir_enable_pulse_train = !bit ^ (ir_14bitMessage_inHalfBits_counter%2);
			ir_enable_pulse(!bit ^ (ir_14bitMessage_inHalfBits_counter%2));

			ir_14bitMessage_inHalfBits_counter--;
		}
		else{
//			ir_enable_pulse_train = 0;
			R_TAU0_Channel0_Stop();
		}
		ir_64bitMessage_inHalfBits_counter--;
	}
	else{
		ir_64bitMessage_inHalfBits_counter = ir_64bitMessage_inHalfBits_total;
		ir_14bitMessage_inHalfBits_counter = ir_14bitMessage_inHalfBits_total;
		if (!IR_BUTTON_TX){
			ir_txMessage |= 0x800;
		}
		else{
			ir_txMessage = ir_txMessage_default;
			R_TAU0_Channel3_Stop();
			R_INTC1_Start();
			R_INTC2_Start();
		}
	}
}

void button_interrupt(uint8_t button){
	ir_txMessage |= button;
	R_TAU0_Channel3_Start();
	if (button == 2) R_INTC2_Stop();
	else R_INTC1_Stop();
}

void ir_enable_pulse(uint8_t enabled){
	if (enabled) R_TAU0_Channel0_Start();
	else R_TAU0_Channel0_Stop();
}
