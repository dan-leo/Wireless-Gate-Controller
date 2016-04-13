/*
 * infrared.c
 *
 *  Created on: 11 Apr 2016
 *      Author: Daniel
 */

#include "infrared.h"

//default ir tx message
#define ir_txMessage_default 0x3188

/* I try to make names self explanatory :D
 */
#define ir_64bitMessage_inHalfBits_total 128
#define ir_14bitMessage_inHalfBits_total 28

#define toggle_button_bounces 3

 /*  this counter will decrement while a button is held in until it triggers a toggle,
  *  in an effort to avoid bouncing in buttons.
  *  It triggers the toggle of the 3rd bit when equal to zero.
  *  Increase value to decrease sensitivity
  */
volatile uint8_t toggle_debounce_counter = toggle_button_bounces;

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
			// eg. if I have 01101 and I want the middle bit it becomes 00011
			bit = (ir_txMessage >> ((ir_14bitMessage_inHalfBits_counter + 1)/2 - 1));

			// eg. if I have 00011 it becomes 00001
			bit &= 0x1;  // select the lowest LSB only

			// enables pulse train if 1 else disable it
			ir_enable_pulse(!bit ^ (ir_14bitMessage_inHalfBits_counter%2));

			ir_14bitMessage_inHalfBits_counter--;
		}
		else{
			// disable pulse train
			R_TAU0_Channel0_Stop();
			// make sure the line is also low during idle time
			IR_TX_38kHz_CARRIER = 0;
		}
		ir_64bitMessage_inHalfBits_counter--;
	}
	else{
		ir_64bitMessage_inHalfBits_counter = ir_64bitMessage_inHalfBits_total;
		ir_14bitMessage_inHalfBits_counter = ir_14bitMessage_inHalfBits_total;
		if(toggle_debounce_counter) toggle_debounce_counter--;
		ir_txMessage = ir_txMessage_default;
		if (!IR_BUTTON_1){
			ir_txMessage |= 0x1;
		}
		if (!IR_BUTTON_2){
			ir_txMessage |= 0x2;
		}
		if (toggle_debounce_counter){
			return;
		}
		if (IR_BUTTON_1 && IR_BUTTON_2){
			ir_txMessage ^= 0x800;
			toggle_debounce_counter = toggle_button_bounces;
			R_TAU0_Channel3_Stop();
			R_INTC1_Start();
			R_INTC2_Start();
		}
	}
}

void button_interrupt(uint8_t button){
	ir_txMessage |= button;

	// start manchester timer
	R_TAU0_Channel3_Start();

	if (button == 2) R_INTC2_Stop();
	else {
		R_INTC1_Stop();
	}
}

void ir_enable_pulse(uint8_t enabled){
	if (enabled) R_TAU0_Channel0_Start();
	else R_TAU0_Channel0_Stop();
}
