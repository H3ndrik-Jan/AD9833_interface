/*
 * AD9833.h
 *
 * Created: 27-3-2021 11:40:00
 *  Author: Hendrik-Jan
 */ 
#include <stdbool.h>
#include "SoftSpi.h"

#define WG_FMCLK 25000000UL
#define MAXDACVAL 268435456UL

typedef enum {
	SINEWAVE, TRIANGLEWAVE, SQUAREWAVE
	}waveforms_t;

typedef struct AD9833_struct
{
	uint16_t _controlRegister;
	bool _B28BIT;
	bool _HLB;
	bool _reset;
	bool _OPBITEN;
	bool _DIV2;
	bool _mode;
	
	bool _FSELECT;
	bool _PSELECT;
	bool _SLEEP1;
	bool _SLEEP12;
	
	
	uint32_t _frequencyRegister;
	uint32_t _freq;
	
	uint32_t _FMCLK;
	double _CLKratio;

	
}WAVGEN_t;

void initGenerator(WAVGEN_t *pWavgen);
void setWaveform(WAVGEN_t *pWavgen, waveforms_t waveform);
void setFrequency(WAVGEN_t *pWavgen, uint32_t frequency);
void writeControlRegister(WAVGEN_t *pWavgen);