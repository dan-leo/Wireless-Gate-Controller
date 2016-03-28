/*
 * utility.h
 *
 *  Created on: 28 Mar 2016
 *      Author: Daniel
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string.h>
#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

char *toString(char *message, uint8_t len);
char *strcat_v2(char *dest, const char *src);

#endif /* UTILITY_H_ */
