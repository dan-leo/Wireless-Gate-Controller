/*
 * utilities.c
 *
 *  Created on: 28 Mar 2016
 *      Author: Daniel
 */
#include "utility.h"
// this place should be called irony.c


//char *toString(char *message, uint8_t len){
//	uint8_t string[len + 1];
//	string = message;
//	string[len] = '\0';
//	return string;
//}

/**
 * still resets board unfortunately...
 */
//char *strcat_v2(char *dest, const char *src)
//{
//    size_t i,j;
//    for (i = 0; dest[i] != '\0'; i++)
//        ;
//    for (j = 0; src[j] != '\0'; j++)
//        dest[i+j] = src[j];
//    dest[i+j] = '\0';
//    return dest;
//}

void word_to_ascii(uint16_t word, uint8_t *lcd_word){
	volatile uint8_t k;
	for (k = 0; k < 16; k++){
		uint8_t shift = word >> (15 - k);
		uint8_t number = shift & 0x1;
		lcd_word[k] = number + '0';
	}
}
