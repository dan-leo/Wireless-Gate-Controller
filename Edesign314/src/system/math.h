/*
 * math.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef MATH_H_
#define MATH_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

uint16_t scale_down(uint16_t value, uint8_t percentage);
uint16_t scale_down_ratio(uint16_t value, uint16_t ratio_numerator, uint16_t ratio_denominator);

#endif /* MATH_H_ */
