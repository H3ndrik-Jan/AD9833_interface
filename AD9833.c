/*
 * AD9833.c
 *
 * Created: 27-3-2021 11:39:50
 *  Author: Hendrik-Jan
 */ 
#include <math.h>
#include "AD9833.h"

void initGenerator(WAVGEN_t *pWavgen){
	initSoftwareSpi();
	pWavgen->_controlRegister = 0; 
	pWavgen->_B28BIT = true;
	pWavgen->_HLB = false;
	pWavgen->_reset = false;
	pWavgen->_OPBITEN = false;
	pWavgen->_DIV2 = true;
	
	pWavgen->_FSELECT = false;
	pWavgen->_PSELECT = false;
	pWavgen->_SLEEP1 = false;
	pWavgen->_SLEEP12 = false;
	
	pWavgen->_FMCLK = 25000000UL;
	
	
	pWavgen->_CLKratio = (double)pWavgen->_FMCLK/MAXDACVAL;
	writeControlRegister(pWavgen);
}

void setWaveform(WAVGEN_t *pWavgen, waveforms_t waveform){
	switch(waveform){
		case SINEWAVE: pWavgen->_OPBITEN = false; pWavgen->_mode = false; break;
		case SQUAREWAVE: pWavgen->_OPBITEN = true; pWavgen->_mode = false; break;
		case TRIANGLEWAVE: pWavgen->_OPBITEN = false; pWavgen->_mode = true; break;
	}
	writeControlRegister(pWavgen);
}

void setFrequency(WAVGEN_t *pWavgen, uint32_t frequency){
 	pWavgen->_frequencyRegister = (uint32_t)frequency/pWavgen->_CLKratio;
	 
	 pWavgen->_FSELECT = true;
	 writeControlRegister(pWavgen);
	 softwareSpiWrite(0x4000 | (pWavgen->_frequencyRegister & 0x3FFF));	//LSB
	 softwareSpiWrite(0x4000 | ((pWavgen->_frequencyRegister >> 14) & 0x3FFF));	//MSB
	softwareSpiWrite(0x8000 | (pWavgen->_frequencyRegister & 0x3FFF));	//LSB
	softwareSpiWrite(0x8000 | ((pWavgen->_frequencyRegister >> 14) & 0x3FFF));	//MSB
		pWavgen->_FSELECT = false;
		writeControlRegister(pWavgen);
}

void writeControlRegister(WAVGEN_t *pWavgen){
	pWavgen->_controlRegister = 0x0000 |
	(pWavgen->_mode << 1) | (pWavgen->_DIV2 << 3) |
	(pWavgen->_OPBITEN << 5) | (pWavgen->_SLEEP1 << 6) |
	(pWavgen->_SLEEP12 << 7) | (pWavgen->_reset << 8) |
	(pWavgen->_PSELECT << 10) |(pWavgen->_FSELECT << 11) |
	(pWavgen->_HLB << 12) | (pWavgen->_B28BIT << 13);
										
	softwareSpiWrite(pWavgen->_controlRegister);
}

