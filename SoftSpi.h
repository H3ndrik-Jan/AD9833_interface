/*
 * SoftSpi.h
 *
 * Created: 27-3-2021 11:43:22
 *  Author: Hendrik-Jan
 *
 *	Software SPI implementation for Xmega AU microcontrollers
 */ 
#include <avr/io.h>
#define FSYNCPIN PIN2_bm
#define FSYNCPORT PORTD

#define SDATPIN PIN0_bm
#define SDATPORT PORTD

#define SCLKPIN PIN1_bm
#define SCLKPORT PORTD

void softwareSpiWrite(uint16_t data);
void initSoftwareSpi(void);