/*
 * time.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "delay.h"

void startTMR0(int delay)
{
	TMIF00 = 0U; // Clear any interrupt flag
	TDR00 = delay; // Load register (in 1 usec units)
	TS0 = 1U; // Start timer
}

void delayNoInt(uint16_t delay){
	//	DI();
	//	for ( ; delay > 0 ; delay--);
	//	EI();
	startTMR0(delay); // Start timer with delay value
	while(TMIF00 == 0) // Wait for timer flag
		NOP();
	R_TAU0_Channel0_Stop(); // Stop timer and clear all flags
	EI();
}

void delay(uint16_t delay){
	// DI();
	for ( ; delay > 0 ; delay--);
	// EI();
}
