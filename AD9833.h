/*
 * AD9833.h
 *
 * Created: 27-3-2021 11:40:00
 *  Author: Hendrik-Jan
 *
 *	These files contain some functions to interface with the AD9833 programmable waveform generator.
 *	All the functions take a pointer to a struct of the WAVGEN_t type.
 *	
 */ 
#include <stdbool.h>
#include "SoftSpi.h"

// Maximum horizontal value, namely 2^28
#define DDS_HORIZONTAL_RES 268435456UL

// 360°/4096. Dividing the wished phase in degrees will give the right phase-register value
#define PHASEFORMULA 0.087890625

typedef enum {
	SINEWAVE, TRIANGLEWAVE, SQUAREWAVE
	}waveforms_t;

// Struct containing settings of the DDS
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
	
	uint16_t _phaseRegister;
	uint16_t _phase;
	
	uint32_t _FMCLK;
	double _CLKratio;

}WAVGEN_t;

// Initializes the DDS by setting and writing default values to the mode register
// Second argument contains the AD9833 clock frequency in Hz.
void initGenerator(WAVGEN_t *pWavgen, uint32_t f_clk);

// Sets and writes the waveform, including the current settings of the mode register
void setWaveform(WAVGEN_t *pWavgen, waveforms_t waveform);

// Writes both frequency registers. Takes frequency in Hertz as second argument
void setFrequency(WAVGEN_t *pWavgen, uint32_t frequency);

// Writes both phase registers. Takes phase in degrees as second argument
void setPhaseDegrees(WAVGEN_t *pWavgen, uint16_t phase);

// Writes control register with the current values in the struct
void writeControlRegister(WAVGEN_t *pWavgen);