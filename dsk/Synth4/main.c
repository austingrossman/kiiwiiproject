#include "common.h"

void processGenerators(void);
void processEffects(void);

//globals
//#pragma DATA_SECTION(analogInBuffer , ".sdram");
float analogInBuffer[FRAME_SIZE];
float *lastBuffer;

float pitchBend = 1;

int main( int argc, char *argv[] ) {
	unsigned int i;
	
	
	//set up tables
	for (i=0; i<TABLE_SIZE; i++) {
		organTable[i] = 0.78 * sinf(2 * PI * i / TABLE_SIZE);
		organTable[i] += 0.2 * sinf(4 * PI * i / TABLE_SIZE);
		organTable[i] += 0.15 * sinf(6 * PI * i / TABLE_SIZE);
		organTable[i] += 0.2 * sinf(12 * PI * i / TABLE_SIZE);
		organTable[i] += 0.12 * sinf(14 * PI * i / TABLE_SIZE);
		organTable[i] += 0.05 * sinf(16 * PI * i / TABLE_SIZE);
		organTable[i] += 0.09 * sinf(18 * PI * i / TABLE_SIZE);
		organTable[i] += 0.004 * sinf(20 * PI * i / TABLE_SIZE);
		organTable[i] += 0.008 * sinf(22 * PI * i / TABLE_SIZE);
		pulsorTable[i] = fabsf(777.6 * cosf(2 * PI * i / TABLE_SIZE));
		wahTable[i] = sinf(2 * PI * i / TABLE_SIZE)/2 + 0.5;
		sineTable[i] = sinf(2 * PI * i / TABLE_SIZE);
	}
	
	//initialize organs
	for (i=0; i<MAX_CHANNELS; i++) {
		organList[i].active = 0;
		clarinetList[i].active = 0;
		brassList[i].active = 0;
		rarpeggioList[i].active = 0;
	}
	
	//setup the mixer
	Mixer_Init();

	Pulsor_Init();
	pulsorEffect.inBuffer = mixer.outBuffer;

	//Phasor_Init();
	//phasorEffect.inBuffer = pulsorEffect.outBuffer;

	Wah_Init();
	wahEffect.inBuffer = pulsorEffect.outBuffer;
	
	Vibrato_Init();
	vibratoEffect.inBuffer = wahEffect.outBuffer;
	
	Mute_Init();
	muteEffect.inBuffer = vibratoEffect.outBuffer;
	
	// memset(delayBuffer, 0, sizeof(float)*65536);
	// delayEffect.inBuffer = muteEffect.outBuffer;
	// delayEffect.buffer = delayBuffer; delayEffect.bufferSize = 65536;
	// delayEffect.delay = 0;
	// DelayLine_Init(&delayEffect);
	
	lastBuffer = muteEffect.outBuffer;
	
	
	SynthInit();

	//burn time until interrupts occur
	while(1) {
		MIDI_Poll();
	}
}


//this routine executes everytime a buffer going to the DAC empties
interrupt void dmax_isr( void ) {
	volatile unsigned *GPTransferEntry;
	static int prevpp=0;
	int pp;
	static unsigned int counter = 0;
	unsigned int i;
	MIDIEvent *AnalyzeMIDIEvent;

	if( hDmaxAdc->regs->DTCR0 & (1<<ADC_TCC) ) {
		hDmaxAdc->regs->DTCR0 = (1<<ADC_TCC);
		GPTransferEntry  = (unsigned *)&hDmaxAdc->regs->HiPriorityEventTable;
		GPTransferEntry += ((*(hDmaxAdc->hiTableEventEntryPtr)>>8)&0x07F);
		pp = GPTransferEntry[2] >> 31;     	
		
		if (pp == prevpp) L1(-1); //an odd number of dropped frames
		prevpp = pp;
		
		for (i=0; i<FRAME_SIZE; i++) {
			analogInBuffer[i] = (float)dmaxAdcBuffer[!pp][LEFT][CH_0][i] / GAIN; 
		}
	}
	
	if( hDmaxDac->regs->DTCR0 & (1<<DAC_TCC) ) { 
		hDmaxDac->regs->DTCR0 = (1<<DAC_TCC); 
		GPTransferEntry  = (unsigned *)&hDmaxDac->regs->HiPriorityEventTable;
		GPTransferEntry += ((*(hDmaxDac->hiTableEventEntryPtr)>>8)&0x07F);
		pp = GPTransferEntry[2] >> 31;


		if (++counter == 200) {
			//L7(-1);
			counter = 0;
		}
		
		AnalyzeMIDIEvent = De_QueueMIDIEvent();
		if (AnalyzeMIDIEvent != NULL) {
			processMIDIEvent(AnalyzeMIDIEvent);		
		}
		
		//process each generator Instance in turn
		processGenerators();

		//process the mixer Instance
		Mixer_ProcessFrame();

		//process each effects Instance in turn
		processEffects();
		
		//fill the buffer	
		for( i=0; i<FRAME_SIZE; i++ ) {
			dmaxDacBuffer[!pp][LEFT][CH_0][i]  = (int)(GAIN * lastBuffer[i]);
			dmaxDacBuffer[!pp][RIGHT][CH_0][i] = (int)(GAIN * lastBuffer[i]);
		}

	}
}

void processGenerators(void) {
	int i;
	for (i=0; i<MAX_CHANNELS; i++) {
		RandomArpeggio_ProcessFrame(i);
	}
	for (i=0; i<MAX_CHANNELS; i++) {
		Organ_ProcessFrame(i);
	}
	for (i=0; i<MAX_CHANNELS; i++) {
		Clarinet_ProcessFrame(i);
	}
	for (i=0; i<MAX_CHANNELS; i++) {
		Brass_ProcessFrame(i);
	}
}

void processEffects(void) {
	Pulsor_ProcessFrame();
	//Phasor_ProcessFrame();
	Wah_ProcessFrame();
	Vibrato_ProcessFrame();
	Mute_ProcessFrame();
	//DelayLine_ProcessFrame(&delayEffect);
}
