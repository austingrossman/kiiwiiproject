#ifndef _GENERATORS_H_
#define _GENERATORS_H_

#include "common.h"

/////////////////////////////////////////////////////
//ADSR Generator
typedef struct Adsr {
	float outBuffer[FRAME_SIZE];
	float attackgain;
	float sustaingain;		//MUST be less than attackgain
	unsigned int attacktime;	//in samples
	unsigned int decaytime;		//in samples
	unsigned int releasetime;	//in samples
	int state;		//quasi-private, set to 3 to enter release phase
	float currentvalue;    //private
	unsigned int inrelease;		//private
} Adsr;

int Adsr_Init(Adsr *this);
int Adsr_ProcessFrame(Adsr *this); //return 0 normal, -1 on error, 1 signals deinit should be called

/////////////////////////////////////////////////////
//Oscillator Generator
typedef struct Oscillator {
	float outBuffer[FRAME_SIZE];
	float *table;			//array to waveform to be oscilated
	unsigned int tablesize;
	float freq;
	//float *gain;			//should be FRAME_SIZE long
	float currentindex;	//private
} Oscillator;

int Oscillator_Init(Oscillator *this);
int Oscillator_ProcessFrame(Oscillator *this);

/////////////////////////////////////////////////////
//Variable Oscillator Generator
typedef struct VariableOscillator {
	float outBuffer[FRAME_SIZE];
	float *freqTable; 		//should be FRAME_SIZE long
	float *table;			//array to waveform to be oscilated
	unsigned int tablesize;
	//float *gain;			//should be FRAME_SIZE long
	float currentindex;	//private
} VariableOscillator;

int VariableOscillator_Init(VariableOscillator *this);
int VariableOscillator_ProcessFrame(VariableOscillator *this);




#endif //_GENERATORS_H_
