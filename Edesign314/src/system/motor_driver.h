/*
 * motor_driver.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"
#include "pins.h"

void pwm_change_duty_cycle(uint8_t percentage);
void motor_power(uint8_t percentage);
void motor_power_ratio(uint16_t ratio_numerator, uint16_t ratio_denominator);

#endif /* MOTOR_DRIVER_H_ */
