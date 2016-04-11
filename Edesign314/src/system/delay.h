/*
 * time.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef TIME_H_
#define TIME_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

void delayNoInt(uint16_t delay);
void startTMR0(int delay);
void delay(uint16_t delay);


#endif /* TIME_H_ */
