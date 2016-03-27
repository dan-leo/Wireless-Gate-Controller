/*
 * math.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "math.h"

uint16_t scale_down(uint16_t value, uint8_t percentage){
	return ((value * percentage) / 100);
}

/**
 * a way to scale down without using floats
 * @param: the value to be scaled, and a fraction
 */
uint16_t scale_down_ratio(uint16_t value, uint16_t ratio_numerator, uint16_t ratio_denominator){
	return ((value * ratio_numerator) / ratio_denominator);
}
