#ifndef _INITIALIZATION_H_
#define _INITIALIZATION_H_
//TODO Audio In
// CSL dMAX Handle
extern CSL_DmaxHandle hDmaxDac;
extern CSL_DmaxHandle hDmaxAdc;

#define ADC_TCC 1 
#define DAC_TCC 2

//  Audio Buffer
#define LEFT		(0)
#define RIGHT		(1)
#define CH_0		(0)
#define CH_1		(1)
#define CH_2		(2)
#define CH_3		(3)
#define NUM_CHANNEL 4
#define STEREO      2
#define PINGPONG    2
extern int dmaxAdcBuffer[PINGPONG][STEREO][NUM_CHANNEL][FRAME_SIZE];
extern int dmaxDacBuffer[PINGPONG][STEREO][NUM_CHANNEL][FRAME_SIZE];
//the audio buffer must have this format for dMax unless we change other stuff


int SynthInit();


#endif //_INITIALIZATION_H_
