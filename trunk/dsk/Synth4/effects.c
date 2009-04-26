#include "common.h"


// /////////////////////////////////////////////////////
// //Wah Effect
// Wah wahEffect; //the WAH instance

// void Wah_Init(void){
	// wahEffect.lastvalue[0] = wahEffect.lastvalue[1] = 0;
	// wahEffect.lastinput[0] = wahEffect.lastinput[1] = 0;
	// wahEffect.wet = 0;
	// wahEffect.parameter = 0;
	// wahEffect.previousWets[0] = wahEffect.previousWets[1] = wahEffect.previousWets[2]
		// = wahEffect.previousWets[3] = 0;
	// wahEffect.previousParameters[0] = wahEffect.previousParameters[1] = wahEffect.previousParameters[2]
		// = wahEffect.previousParameters[3] = 0;
	// return;
// }

// void Wah_ProcessFrame(void){
	// float *outBuffer;
	// float *inBuffer;
	// float lastoutone, lastouttwo;
	// float lastinone, lastintwo;
	// float wet, parameter, anglcos, bw, B, B2, a_0, a2_0, b_0;
	// int i = 0;
	
	// outBuffer = wahEffect.outBuffer;
	// inBuffer = wahEffect.inBuffer;
	// lastoutone = wahEffect.lastvalue[0];
	// lastouttwo = wahEffect.lastvalue[1];
	// lastinone = wahEffect.lastinput[0];
	// lastintwo = wahEffect.lastinput[1];
	// wet = (wahEffect.wet + wahEffect.previousWets[0] + wahEffect.previousWets[1]
		 // + wahEffect.previousWets[2] + wahEffect.previousWets[3]) / 5;
	// wahEffect.previousWets[3]=wahEffect.previousWets[2];  //cheesy, simply, and inefficient lowpass filter
	// wahEffect.previousWets[2]=wahEffect.previousWets[1];
	// wahEffect.previousWets[1]=wahEffect.previousWets[0];
	// wahEffect.previousWets[0]=wahEffect.wet;
	
	// parameter = (wahEffect.parameter + wahEffect.previousParameters[0] + wahEffect.previousParameters[1]
		 // + wahEffect.previousParameters[2] + wahEffect.previousParameters[3]) / 5;
	// wahEffect.previousParameters[3]=wahEffect.previousParameters[2];  //cheesy, simply, and inefficient lowpass filter
	// wahEffect.previousParameters[2]=wahEffect.previousParameters[1];
	// wahEffect.previousParameters[1]=wahEffect.previousParameters[0];
	// wahEffect.previousParameters[0]=wahEffect.parameter;
		
	// anglcos = cosf(7 * PI / 64 * parameter + PI / 64);
	// bw = 2*32000/PI * (7 * PI / 64 * parameter + PI / 64);
	// B = .35*PI*bw*SAMPLING_PERIOD;
	// B2 = B*B;
 
	// a_0 = 1+B2/8-B*sqrt(B2+16)/8;
	// a2_0 = a_0*a_0;
	// b_0 = (1 - a_0*a_0)/2;
	
	// outBuffer[0] = b_0 * (inBuffer[0] - lastintwo) +
							// 2* a_0 * anglcos * lastoutone -
							// a2_0 * lastouttwo;
	// outBuffer[1] = b_0 * (inBuffer[1] - lastinone) +
							// 2* a_0 * anglcos * outBuffer[0] -
							// a2_0 * lastoutone;
	
	// for (i=2; i < FRAME_SIZE; i++) {
			// outBuffer[i] = b_0 * (inBuffer[i] - inBuffer[i-2]) +
							// 2* a_0 * anglcos * outBuffer[i-1] -
							// a2_0  * outBuffer[i-2];
	// }
	
	// wahEffect.lastvalue[0] = outBuffer[FRAME_SIZE - 1];
	// wahEffect.lastvalue[1] = outBuffer[FRAME_SIZE - 2];
	// wahEffect.lastinput[0] = inBuffer[FRAME_SIZE - 1];
	// wahEffect.lastinput[1] = inBuffer[FRAME_SIZE - 2];
	
	// for (i=0; i < FRAME_SIZE; i++) {
		// outBuffer[i] = 1.7*wet*outBuffer[i] + (1 - wet)*inBuffer[i];	
	// }
	
	// return;
// }

/////////////////////////////////////////////////////
//AutoWah Effect
Wah wahEffect; //the WAH instance
//#pragma DATA_SECTION(wahTable , ".sdram");
float wahTable[TABLE_SIZE];

void Wah_Init(void){
	wahEffect.lastvalue[0] = wahEffect.lastvalue[1] = 0;
	wahEffect.lastinput[0] = wahEffect.lastinput[1] = 0;
	wahEffect.wet = 0;
	wahEffect.parameter = 0;
	wahEffect.previousWets[0] = wahEffect.previousWets[1] = wahEffect.previousWets[2]
		= wahEffect.previousWets[3] = 0;
	wahEffect.previousParameters[0] = wahEffect.previousParameters[1] = wahEffect.previousParameters[2]
		= wahEffect.previousParameters[3] = 0;
	wahEffect.lfo.table = wahTable;
	wahEffect.lfo.tablesize = TABLE_SIZE;
	Oscillator_Init(&(wahEffect.lfo));
	return;
}

void Wah_ProcessFrame(void){
	float *outBuffer;
	float *inBuffer;
	float lastoutone, lastouttwo;
	float lastinone, lastintwo;
	float wet, parameter, anglcos, bw, B, B2, a_0, a2_0, b_0;
	int i = 0;
	
	outBuffer = wahEffect.outBuffer;
	inBuffer = wahEffect.inBuffer;
	lastoutone = wahEffect.lastvalue[0];
	lastouttwo = wahEffect.lastvalue[1];
	lastinone = wahEffect.lastinput[0];
	lastintwo = wahEffect.lastinput[1];
	wet = (wahEffect.wet + wahEffect.previousWets[0] + wahEffect.previousWets[1]
		 + wahEffect.previousWets[2] + wahEffect.previousWets[3]) / 5;
	wahEffect.previousWets[3]=wahEffect.previousWets[2];  //cheesy, simply, and inefficient lowpass filter
	wahEffect.previousWets[2]=wahEffect.previousWets[1];
	wahEffect.previousWets[1]=wahEffect.previousWets[0];
	wahEffect.previousWets[0]=wahEffect.wet;
	
	parameter = (wahEffect.parameter + wahEffect.previousParameters[0] + wahEffect.previousParameters[1]
		 + wahEffect.previousParameters[2] + wahEffect.previousParameters[3]) / 5;
	wahEffect.previousParameters[3]=wahEffect.previousParameters[2];  //cheesy, simply, and inefficient lowpass filter
	wahEffect.previousParameters[2]=wahEffect.previousParameters[1];
	wahEffect.previousParameters[1]=wahEffect.previousParameters[0];
	wahEffect.previousParameters[0]=wahEffect.parameter;
	
	wahEffect.lfo.freq = parameter;
	Oscillator_ProcessFrame(&(wahEffect.lfo));
	//anglcos = cosf(7 * PI / 64 * parameter + PI / 64);
	anglcos = cosf(7 * PI / 64 * wahEffect.lfo.outBuffer[0] + PI / 64);
	bw = 2*32000/PI * (7 * PI / 64 * parameter + PI / 64);
	B = .35*PI*bw*SAMPLING_PERIOD;
	B2 = B*B;
 
	a_0 = 1+B2/8-B*sqrt(B2+16)/8;
	a2_0 = a_0*a_0;
	b_0 = (1 - a_0*a_0)/2;
	
	outBuffer[0] = b_0 * (inBuffer[0] - lastintwo) +
							2* a_0 * anglcos * lastoutone -
							a2_0 * lastouttwo;
	outBuffer[1] = b_0 * (inBuffer[1] - lastinone) +
							2* a_0 * anglcos * outBuffer[0] -
							a2_0 * lastoutone;
	
	for (i=2; i < FRAME_SIZE; i++) {
			outBuffer[i] = b_0 * (inBuffer[i] - inBuffer[i-2]) +
							2* a_0 * anglcos * outBuffer[i-1] -
							a2_0  * outBuffer[i-2];
	}
	
	wahEffect.lastvalue[0] = outBuffer[FRAME_SIZE - 1];
	wahEffect.lastvalue[1] = outBuffer[FRAME_SIZE - 2];
	wahEffect.lastinput[0] = inBuffer[FRAME_SIZE - 1];
	wahEffect.lastinput[1] = inBuffer[FRAME_SIZE - 2];
	
	for (i=0; i < FRAME_SIZE; i++) {
		outBuffer[i] = 1.7*wet*outBuffer[i] + (1 - wet)*inBuffer[i];	
	}
	
	return;
}



/////////////////////////////////////////////////////
//Phasor Effect
Phasor phasorEffect; //the Phasor instance

#define NPITs -0.0000327249f

void Phasor_Init(void){
	phasorEffect.lastvalue[0] = phasorEffect.lastvalue[1] = 0;
	phasorEffect.lastinput[0] = phasorEffect.lastinput[1] = 0;
	phasorEffect.wet = 0;
	phasorEffect.parameter = 0;
	phasorEffect.previousWets[0] = phasorEffect.previousWets[1] = phasorEffect.previousWets[2]
		= phasorEffect.previousWets[3] = 0;
	phasorEffect.previousParameters[0] = phasorEffect.previousParameters[1] = phasorEffect.previousParameters[2]
		= phasorEffect.previousParameters[3] = 0;
	phasorEffect.bw = 1000;
	return;
}

void Phasor_ProcessFrame(void){
	float *outBuffer;
	float *inBuffer;
	float lastoutone, lastouttwo;
	float lastinone, lastintwo;
	float wet, parameter, anglcos;
	float a, d;
	int i = 0;
	
	outBuffer = phasorEffect.outBuffer;
	inBuffer = phasorEffect.inBuffer;
	lastoutone = phasorEffect.lastvalue[0];
	lastouttwo = phasorEffect.lastvalue[1];
	lastinone = phasorEffect.lastinput[0];
	lastintwo = phasorEffect.lastinput[1];
	wet = (phasorEffect.wet + phasorEffect.previousWets[0] + phasorEffect.previousWets[1]
		 + phasorEffect.previousWets[2] + phasorEffect.previousWets[3]) / 5;
	phasorEffect.previousWets[3]=phasorEffect.previousWets[2];  //cheesy, simply, and inefficient lowpass filter
	phasorEffect.previousWets[2]=phasorEffect.previousWets[1];
	phasorEffect.previousWets[1]=phasorEffect.previousWets[0];
	phasorEffect.previousWets[0]=phasorEffect.wet;
	
	parameter = (phasorEffect.parameter + phasorEffect.previousParameters[0] + phasorEffect.previousParameters[1]
		 + phasorEffect.previousParameters[2] + phasorEffect.previousParameters[3]) / 5;
	phasorEffect.previousParameters[3]=phasorEffect.previousParameters[2];  //cheesy, simply, and inefficient lowpass filter
	phasorEffect.previousParameters[2]=phasorEffect.previousParameters[1];
	phasorEffect.previousParameters[1]=phasorEffect.previousParameters[0];
	phasorEffect.previousParameters[0]=phasorEffect.parameter;
		
	anglcos = cosf(3.5 * PI / 32 * parameter + PI / 32);
	d = expf(NPITs * phasorEffect.bw);
	a = (1 + d*d)/2;
	
	outBuffer[0] = a * (inBuffer[0] -2*anglcos*lastinone + lastintwo + 2*anglcos*lastoutone)
							- d*d * lastouttwo;
	outBuffer[1] = a * (inBuffer[1] -2*anglcos*inBuffer[0] + lastinone + 2*anglcos*outBuffer[0])
							- d*d * lastoutone;
	
	for (i=2; i < FRAME_SIZE; i++) {
			outBuffer[i] = a * (inBuffer[i] -2*anglcos*inBuffer[i-1] + inBuffer[i-2] + 2*anglcos*outBuffer[i-1])
							- d*d * outBuffer[i-2];
	}
	
	phasorEffect.lastvalue[0] = outBuffer[FRAME_SIZE - 1];
	phasorEffect.lastvalue[1] = outBuffer[FRAME_SIZE - 2];
	phasorEffect.lastinput[0] = inBuffer[FRAME_SIZE - 1];
	phasorEffect.lastinput[1] = inBuffer[FRAME_SIZE - 2];
	
	for (i=0; i < FRAME_SIZE; i++) {
		outBuffer[i] = 1.2*wet*outBuffer[i] + (1 - wet)*inBuffer[i];	
	}
	
	return;
}


/////////////////////////////////////////////////////
//Pulsor Effect
Pulsor pulsorEffect; //the Pulsor instance
float pulsorTable[TABLE_SIZE];

void Pulsor_Init(void){
	pulsorEffect.lastinput1 = 0;
	//pulsorEffect.lastinput2 = 0;
	pulsorEffect.wet = 0;
	pulsorEffect.count = 0;
	pulsorEffect.freq = 1;
	pulsorEffect.previousWets[0] = pulsorEffect.previousWets[1] = pulsorEffect.previousWets[2]
		= pulsorEffect.previousWets[3] = 0;
	pulsorEffect.lfo.table = pulsorTable;
	pulsorEffect.lfo.tablesize = TABLE_SIZE;
	Oscillator_Init(&(pulsorEffect.lfo));
		return;
}

void Pulsor_ProcessFrame(void){
	float *outBuffer;
	float *inBuffer;
	float *lfoBuffer;
	float wet;
	//float a;
	int i, count;
	
	outBuffer = pulsorEffect.outBuffer;
	inBuffer = pulsorEffect.inBuffer;
	lfoBuffer = pulsorEffect.lfo.outBuffer;
	count = pulsorEffect.count;
	wet = (pulsorEffect.wet + pulsorEffect.previousWets[0] + pulsorEffect.previousWets[1]
		 + pulsorEffect.previousWets[2] + pulsorEffect.previousWets[3]) / 5;
	pulsorEffect.previousWets[3]=pulsorEffect.previousWets[2];  //cheesy, simply, and inefficient lowpass filter
	pulsorEffect.previousWets[2]=pulsorEffect.previousWets[1];
	pulsorEffect.previousWets[1]=pulsorEffect.previousWets[0];
	pulsorEffect.previousWets[0]=pulsorEffect.wet;

	pulsorEffect.lfo.freq = pulsorEffect.freq;
	Oscillator_ProcessFrame(&(pulsorEffect.lfo));
	outBuffer[0] = wet * 0.01401 * (lfoBuffer[0]*pulsorEffect.lastinput1 + (1-lfoBuffer[0])*inBuffer[0])
			+ (1-wet)*inBuffer[0];
	/*
	outBuffer[2] = wet * 0.01401 * (a*pulsorEffect.lastinput1 + (1-a)*inBuffer[1])
			+ (1-wet)*inBuffer[1];
	*/
	for (i=1; i < FRAME_SIZE; i++) {
		outBuffer[i] = wet * 0.01401 * (lfoBuffer[i]*inBuffer[i-1] + (1-lfoBuffer[i])*inBuffer[i])
				+ (1-wet)*inBuffer[i];
	}
	
	pulsorEffect.lastinput1 = inBuffer[FRAME_SIZE - 1];
	//pulsorEffect.lastinput2 = inBuffer[FRAME_SIZE - 2];
	pulsorEffect.count = count;
	
	return;
}


/////////////////////////////////////////////////////
//Mute Effect
Mute muteEffect; //the Mute instance

void Mute_Init(void){
	muteEffect.active = 0;
	muteEffect.previouslyActive = 0;
	return;
}
//TODO this clicks
void Mute_ProcessFrame(void){
	float *outBuffer;
	float *inBuffer;
	float increment;
	unsigned char active, previouslyActive;
	int i = 0;
	
	outBuffer = muteEffect.outBuffer;
	inBuffer = muteEffect.inBuffer;
	active = muteEffect.active;
	previouslyActive = muteEffect.previouslyActive;
	
	increment = 1 / FRAME_SIZE;
	
	if ((active == 0) && (previouslyActive == 0)) {
		for (i=0; i < FRAME_SIZE; i++) {
			outBuffer[i] = inBuffer[i];	
		}
	} else if ((active == 1) && (previouslyActive == 1)) {
		for (i=0; i < FRAME_SIZE; i++) {
			outBuffer[i] = 0;	
		}
	} else if ((active == 1) && (previouslyActive == 0)) {
		for (i=0; i < FRAME_SIZE; i++) {
			outBuffer[i] = (1 - i*increment)*inBuffer[i];	
			//outBuffer[i] = 1 / (1 + expf(12/FRAME_SIZE*i - 6)) * inBuffer[i];
		}
		muteEffect.previouslyActive = 1;
	} else if ((active == 0) && (previouslyActive == 1)) {
		for (i=0; i < FRAME_SIZE; i++) {
			outBuffer[i] = i*increment*inBuffer[i];	
			//outBuffer[i] = 1 / (1 + expf(-12/FRAME_SIZE*i + 6)) * inBuffer[i];
		}
		muteEffect.previouslyActive = 0;
	}
	
	return;
}
