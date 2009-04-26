#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include "common.h"

// /////////////////////////////////////////////////////
// //Wah Effect
// typedef struct Wah {
	// float outBuffer[FRAME_SIZE];
	// float *inBuffer;		//should be FRAME_SIZE long
	// float parameter;
	// float wet;
	// float previousParameters[4];	//private
	// float previousWets[4];			//private
	// float lastvalue[2];		//private
	// float lastinput[2];		//private
// } Wah;

// extern Wah wahEffect;

// void Wah_Init(void);
// void Wah_ProcessFrame(void);

/////////////////////////////////////////////////////
//AutoWah Effect
typedef struct Wah {
	float outBuffer[FRAME_SIZE];
	float *inBuffer;		//should be FRAME_SIZE long
	float parameter;
	float wet;
	float previousParameters[4];	//private
	float previousWets[4];			//private
	float lastvalue[2];		//private
	float lastinput[2];		//private
	Oscillator lfo;
} Wah;

extern Wah wahEffect;
extern float wahTable[TABLE_SIZE];

void Wah_Init(void);
void Wah_ProcessFrame(void);


/////////////////////////////////////////////////////
//Phasor Effect
typedef struct Phasor {
	float outBuffer[FRAME_SIZE];
	float *inBuffer;		//should be FRAME_SIZE long
	float parameter;
	float wet;
	float bw;
	float previousParameters[4];	//private
	float previousWets[4];			//private
	float lastvalue[2];		//private
	float lastinput[2];		//private
} Phasor;

extern Phasor phasorEffect;

void Phasor_Init(void);
void Phasor_ProcessFrame(void);

/////////////////////////////////////////////////////
//Pulsor Effect
typedef struct Pulsor {
	float outBuffer[FRAME_SIZE];
	float *inBuffer;		//should be FRAME_SIZE long
	float freq;
	float wet;
	float previousWets[4];			//private
	float lastinput1;		//private
	//float lastinput2;		//private
	Oscillator lfo;
	unsigned int count;		//private
} Pulsor;

extern Pulsor pulsorEffect;

void Pulsor_Init(void);
void Pulsor_ProcessFrame(void);


/////////////////////////////////////////////////////
//Mute Effect
typedef struct Mute {
	float outBuffer[FRAME_SIZE];
	float *inBuffer;		//should be FRAME_SIZE long
	unsigned char active;
	unsigned char previouslyActive;	//private
} Mute;

extern Mute muteEffect;

void Mute_Init(void);
void Mute_ProcessFrame(void);


#endif //_EFFECTS_H_
