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

/**
 * takes in a binary coded decimal
 */
uint8_t toHex(uint8_t decimal){
	return (decimal & 0x0F) + ((decimal >> 4) * 10);
}

/**
 * converts from hex to binary coded decimal
 * max input/output is 99
 */
uint8_t toBCD(uint8_t hex){
	uint8_t nibble_l = hex % 10;
	uint8_t nibble_h = ((hex / 10) % 10) << 4;
	uint8_t byte = nibble_h | nibble_l;
	return byte;
	//return (hex % 10) | ((hex / 10) << 4);
}
