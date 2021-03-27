/*
 * ad9833interface.c
 *
 * Created: 27-3-2021 11:07:04
 * Author : Hendrik-Jan
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "AD9833.h"
#include "serialF0.h"

void initADC(void);

#define AVGSAMPLES 100
#define MAXMISMATCHES 250

int main(void)
{
	_delay_ms(1);
	WAVGEN_t ad9833;
	initGenerator(&ad9833);
	setWaveform(&ad9833, TRIANGLEWAVE);
	initADC();
	
	static uint16_t lastMeasurements[AVGSAMPLES];
    while (1) 
    {
		
		if((ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm)){
			lastMeasurements[0] = ADCA.CH0.RES;
			ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm;
			
			uint32_t average = 0;
			for(uint8_t i = 0; i< AVGSAMPLES; i++){
				average+=lastMeasurements[i];
			}
			average/=AVGSAMPLES;
			
			static uint8_t misMatchCount;
			static uint16_t dacValue = 0;
			
			if(average != dacValue){
				misMatchCount++;
			} else{
				misMatchCount = 0;
			}
			
			if(misMatchCount>MAXMISMATCHES){
				setFrequency(&ad9833, (average-190)*2);
				dacValue = average;
			}

			for(uint8_t i = AVGSAMPLES-1; i>0; i--){
				lastMeasurements[i] = lastMeasurements[i-1];
			}
		}
    }
}

void initADC(void){
	PORTA.DIRCLR = PIN0_bm; 
	ADCA.CTRLB = ADC_FREERUN_bm | ADC_RESOLUTION_12BIT_gc;
	ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;
	ADCA.EVCTRL = ADC_SWEEP0_bm;

	ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CTRLA = ADC_ENABLE_bm;
}