/*
 * infrared.c
 *
 *  Created on: 03 Apr 2016
 *      Author: Daniel
 */

/*
 * As you can see, global variables are in the .h file, and local variables are in the .c file.
 * Also, I generally prefix variable names with something analogous to the file it is defined in.
 * In this case, it is ir_, short for infrared.
 */

/*
 * I put any other #includes in the relative .h header to this .c source file. Looks neater
 */
#include "infrared.h"

//default ir tx message
#define ir_txMessage_default 0x37C0

/*
 * I try to make names self explanatory
 */
#define ir_64bitMessage_inHalfBits_total 128
#define ir_14bitMessage_inHalfBits_total 28

// total receive ticks for a message
#define ir_14bitMessage_inQuarterBits_total 55

void ir_pulse_train(uint8_t enabled);

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
			ir_pulse_train(bit ^ (ir_14bitMessage_inHalfBits_counter%2));
			ir_14bitMessage_inHalfBits_counter--;
		}
		else{
			ir_pulse_train(1);
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
			R_INTC2_Start();
		}
	}
}

void ir_pulse_train(uint8_t enabled){
	IR_LED_TX = enabled;
}

/**
 * this interrupt service routine will receive an infra red message in
 * manchester RC-5 format (1 bit in 1778us).
 */
void ir_rxInterruptSR(){
	static volatile uint8_t ir_14bitMessage_inQuarterBits_counter = ir_14bitMessage_inQuarterBits_total;
	static volatile uint8_t double_half_bit;
	static volatile uint16_t ir_rxMessage_buffer; //ir rx message buffer
    //	double_half_bit = 0;

	// grab first half-bit
	if (!((ir_14bitMessage_inQuarterBits_counter + 1) % 4) && ir_14bitMessage_inQuarterBits_counter){
		double_half_bit = (double_half_bit | IR_RX) << 1;
	}

	// if at the end of a full bit
	if (!((ir_14bitMessage_inQuarterBits_counter - 1) % 4) && ir_14bitMessage_inQuarterBits_counter){
		double_half_bit |= IR_RX;
		if (double_half_bit == 0x01){
			// valid bit, also equal to 1
			ir_rxMessage_buffer = (ir_rxMessage_buffer | 1) << 1;
		}
		else if (double_half_bit == 0x10){
			// valid bit, also equal to 0
			ir_rxMessage_buffer = (ir_rxMessage_buffer | 0) << 1;
		}
		double_half_bit = 0;
	}
	ir_14bitMessage_inQuarterBits_counter--;
	if (!ir_14bitMessage_inQuarterBits_counter){
		ir_14bitMessage_inQuarterBits_counter = ir_14bitMessage_inQuarterBits_total;
		ir_rxMessage_buffer >>= 1;
		ir_rxMessage = ir_rxMessage_buffer;
		ir_rxMessage_buffer = 0;
		R_TMR_RJ0_Stop();
		R_INTC5_Start();
	}
}
