#include "common.h"

//#pragma DATA_SECTION(mixer , ".sdram");
Mixer mixer; //the one instance of the mixer

int Mixer_Init(void){
	mixer.gain = 1;
	mixer.analogInGain = 0;
	mixer.previousGains[0] = mixer.previousGains[1] 
		= mixer.previousGains[2] = mixer.previousGains[3] = 1;
	return 0;
}


int Mixer_ProcessFrame(void){
	int currentSample, currentChannel;
	float *outBuffer;
	float gain;

	outBuffer = mixer.outBuffer;
	gain = (mixer.gain+mixer.previousGains[0]+mixer.previousGains[1]
			+mixer.previousGains[2]+mixer.previousGains[3]) / 5/16; 
	mixer.previousGains[3]=mixer.previousGains[2];  //cheesy, simply, and inefficient lowpass filter
	mixer.previousGains[2]=mixer.previousGains[1];
	mixer.previousGains[1]=mixer.previousGains[0];
	mixer.previousGains[0]=mixer.gain;
	
	//zero samples, TODO change to memset
	// for (currentSample = 0; currentSample < FRAME_SIZE; currentSample++) {
		// outBuffer[currentSample] = 0;
	// }
	memset( outBuffer, 0, sizeof(float)*FRAME_SIZE );  
	
	for (currentChannel = 0; currentChannel < MAX_CHANNELS; currentChannel++) {
		if (clarinetList[currentChannel].active == 1) {
			for (currentSample = 0; currentSample < FRAME_SIZE; currentSample++) {
				outBuffer[currentSample] += clarinetList[currentChannel].outBuffer[currentSample];
			}
		}
	}
	for (currentChannel = 0; currentChannel < MAX_CHANNELS; currentChannel++) {
		if (organList[currentChannel].active == 1) {
			for (currentSample = 0; currentSample < FRAME_SIZE; currentSample++) {
				outBuffer[currentSample] += organList[currentChannel].outBuffer[currentSample];
			}
		}
	}
	for (currentChannel = 0; currentChannel < MAX_CHANNELS; currentChannel++) {
		if (brassList[currentChannel].active == 1) {
			for (currentSample = 0; currentSample < FRAME_SIZE; currentSample++) {
				outBuffer[currentSample] += brassList[currentChannel].outBuffer[currentSample];
			}
		}
	}
	
	
	//apply gain
	for (currentSample = 0; currentSample < FRAME_SIZE; currentSample++) {
		outBuffer[currentSample] += mixer.analogInGain * analogInBuffer[currentSample];
		outBuffer[currentSample] *= gain;
	}
	
	return 0;
}	
