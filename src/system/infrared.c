/*
 * infrared.c
 *
 *  Created on: 03 Apr 2016
 *      Author: Daniel
 */

#include "infrared.h"

/**
 * this interrupt service routine will transmit an infra red message in
 * manchester RC-5 format (1 bit in 1778us).
 */
void ir_InterruptSR(){
	volatile uint8_t bit;
	if (ir_half_bit_total_counter){
		if (ir_half_bit_subtotal_counter){
			// shift desired bit to lowest LSB
			bit = (ir_message >> ((ir_half_bit_subtotal_counter + 1)/2 - 1));
			bit &= 0x1;  // select the lowest LSB only
			ir_pulse_train(!bit ^ (ir_half_bit_subtotal_counter%2));
			ir_half_bit_subtotal_counter--;
		}
		else{
			ir_pulse_train(1);
		}
		ir_half_bit_total_counter--;
	}
	else{
		ir_reset_counters();
	}
}

void ir_pulse_train(uint8_t enabled){
	IR_LED_TX = enabled;
}

void ir_reset_counters(){
	ir_half_bit_total_counter = ir_half_bit_total;
	ir_half_bit_subtotal_counter = ir_half_bit_subtotal;
}
