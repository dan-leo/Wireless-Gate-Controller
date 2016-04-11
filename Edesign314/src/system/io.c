/*
 * io.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "io.h"

void toggle_led(){
	P7 ^= 0x80;
}
