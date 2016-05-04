/*
 * buzzer.c
 *
 *  Created on: 04 May 2016
 *      Author: Daniel
 */
#include "buzzer.h"

void beep(uint8_t number_of_beeps){
	while (number_of_beeps){
		R_PCLBUZ0_Start();
		delayMillis(20);
		R_PCLBUZ0_Stop();
		number_of_beeps--;
		if (number_of_beeps) delayMillis(40);
	}
}
