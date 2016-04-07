/*
 * infrared.c
 *
 *  Created on: 03 Apr 2016
 *      Author: Daniel
 */

#include "infrared.h"
#include "pins.h"

volatile uint8_t ir_half_bit_total_counter = 128;
volatile uint8_t ir_half_bit_subtotal_counter = 28;
/**
 * this interrupt service routine will transmit an infra red message in
 * manchester RC-5 format (1 bit in 1778us).
 */
void ir_txInterruptSR(){
	volatile uint8_t bit;
	if (ir_half_bit_total_counter){
		if (ir_half_bit_subtotal_counter){
			// shift desired bit to lowest LSB
			bit = (ir_txMessage >> ((ir_half_bit_subtotal_counter + 1)/2 - 1));
			bit &= 0x1;  // select the lowest LSB only
			// enables pulse train if 1 else disables it
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

/**
 * this interrupt service routine will receive an infra red message in
 * manchester RC-5 format (1 bit in 1778us).
 */
void ir_rxInterruptSR(){
	static volatile uint8_t ir_quarterbit_total_counter = ir_quarterbit_total;
	volatile uint8_t callback = 0;
	static volatile uint8_t double_half_bit;
    //	double_half_bit = 0;
	// grab first half-bit
	if (!(ir_quarterbit_total_counter % 4) && ir_quarterbit_total_counter){
		double_half_bit = (double_half_bit | IR_RX) << 1;
	}
	// if at the end of a full bit
	if (!((ir_quarterbit_total_counter + 2) % 4) && ir_quarterbit_total_counter){
		double_half_bit |= IR_RX;
		if (double_half_bit == 0x01){
			// valid bit, also equal to 1
			ir_rxMessage = (ir_rxMessage | 1) << 1;
		}
		else if (double_half_bit == 0x10){
			// valid bit, also equal to 0
			ir_rxMessage = (ir_rxMessage | 0) << 1;
		}
		double_half_bit = 0;
	}
	if (!ir_quarterbit_total_counter){
		ir_quarterbit_total_counter = ir_quarterbit_total;
		ir_rxMessage >>= 1;
		ir_rxMessage = 0;
		callback = 1;
	}
	ir_quarterbit_total_counter--;
	if (callback){
		R_TMR_RJ0_Stop();
		R_INTC5_Start();
	}
}
