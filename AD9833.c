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
	
	pWavgen->_freq = 1000;
	pWavgen->_phase = 0;
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
	if(pWavgen->_freq == frequency) return;
	pWavgen->_freq = frequency;
 	pWavgen->_frequencyRegister = (uint32_t)frequency/pWavgen->_CLKratio;
	 
	pWavgen->_FSELECT = true;
	writeControlRegister(pWavgen);
	softwareSpiWrite(0x4000 | (pWavgen->_frequencyRegister & 0x3FFF));	//Reg 1 LSB
	softwareSpiWrite(0x4000 | ((pWavgen->_frequencyRegister >> 14) & 0x3FFF));	//Reg 1 MSB
	softwareSpiWrite(0x8000 | (pWavgen->_frequencyRegister & 0x3FFF));	//Reg 2 LSB
	softwareSpiWrite(0x8000 | ((pWavgen->_frequencyRegister >> 14) & 0x3FFF));	//Reg 2 MSB
	pWavgen->_FSELECT = false;
	writeControlRegister(pWavgen);
}

void setPhaseDegrees(WAVGEN_t *pWavgen, uint16_t phase){
	if(pWavgen->_phase == phase) return;
	pWavgen->_phase = phase;
	phase%=360;
	pWavgen->_phaseRegister = (phase==0)?(0):(uint16_t)phase/PHASEFORMULA;
	
	pWavgen->_PSELECT = true;
	writeControlRegister(pWavgen);
	softwareSpiWrite(0xC000 | (pWavgen->_phaseRegister & 0x1FFF));	//Reg 1
	softwareSpiWrite(0xE000 | (pWavgen->_phaseRegister & 0x1FFF));	//Reg 2
	pWavgen->_PSELECT = true;
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

