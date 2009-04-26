#ifndef _MIXER_H_
#define _MIXER_H_

#include "common.h"

typedef struct Mixer {
	float outBuffer[FRAME_SIZE];
	float gain;		//scale to between 0 and 1, this is what the overall volume controls
	float analogInGain;
	float previousGains[4]; //private, for low-pass filtering
} Mixer;

extern Mixer mixer;

int Mixer_Init(void);
int Mixer_ProcessFrame(void);

#endif //_MIXER_H_
