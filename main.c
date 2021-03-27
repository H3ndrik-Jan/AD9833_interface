/*
 * ad9833interface.c
 *
 * Created: 27-3-2021 11:07:04
 * Author : Hendrik-Jan
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "AD9833.h"

int main(void)
{
	_delay_ms(1);
	WAVGEN_t ad9833;
	initGenerator(&ad9833);
	
    while (1) 
    {
		_delay_ms(1000);
		setFrequency(&ad9833, 100);
		setWaveform(&ad9833, TRIANGLEWAVE);
		_delay_ms(1000);
		setFrequency(&ad9833, 1000);
		setWaveform(&ad9833, SQUAREWAVE);
		_delay_ms(1000);
		setFrequency(&ad9833, 10000);
		setWaveform(&ad9833, SINEWAVE);
    }
}

