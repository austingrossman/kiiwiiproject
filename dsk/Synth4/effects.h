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
extern float pulsorTable[TABLE_SIZE];

void Pulsor_Init(void);
void Pulsor_ProcessFrame(void);


/////////////////////////////////////////////////////
//Delay line
/*typedef struct DelayLine {
	float outBuffer[FRAME_SIZE];
	float *inBuffer;		//should be FRAME_SIZE long, double word aligned
	float *buffer;
	unsigned int bufferSize;			//must be a power of two
	unsigned int delay;				//must be even number
	unsigned int dp;			//private
} DelayLine;

extern DelayLine delayEffect;
extern float delayBuffer[65536];

void DelayLine_Init(DelayLine *dl);
void DelayLine_ProcessFrame(DelayLine *dl);
*/

/////////////////////////////////////////////////////
//Vibrato Effect
typedef struct Vibrato {
	float outBuffer[FRAME_SIZE];
	Oscillator lfo;			//private
	float *inBuffer;		//should be FRAME_SIZE long, double word aligned
	float freq;				
	float intensity;			
	unsigned int dp;		//private
} Vibrato;

extern Vibrato vibratoEffect;
extern float vibratoBuffer[8192];

void Vibrato_Init(void);
void Vibrato_ProcessFrame(void);


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
