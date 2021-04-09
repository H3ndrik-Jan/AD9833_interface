/*
 * SoftSpi.c
 *
 * Created: 27-3-2021 11:43:13
 *  Author: Hendrik-Jan
 *
 *	Software SPI implementation for Xmega AU microcontrollers
 */
#define F_CPU 2000000UL 

#include "SoftSpi.h"

void initSoftwareSpi(void){
	SCLKPORT.DIRSET = SCLKPIN;
	FSYNCPORT.DIRSET = FSYNCPIN;
	SCLKPORT.OUTSET = SCLKPIN;
	FSYNCPORT.OUTSET = FSYNCPIN;
	SDATPORT.DIRSET = SDATPIN;
}

void softwareSpiWrite(uint16_t data){
	FSYNCPORT.OUTCLR = FSYNCPIN;
	for(uint8_t i = 0; i<16; i++){
		if((data>>(15-i)) & 0x0001) SDATPORT.OUTSET = SDATPIN;
		else SDATPORT.OUTCLR = SDATPIN;
		
		SCLKPORT.OUTCLR = SCLKPIN;
		SCLKPORT.OUTSET = SCLKPIN;
	}
	FSYNCPORT.OUTSET = FSYNCPIN;
}