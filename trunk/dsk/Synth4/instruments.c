#include "common.h"

int NewNote(unsigned char midiNote, unsigned char velocity) {	
	static unsigned char previouschannel = 0;
	
	if (previouschannel != channel) {
		ReleaseOrgan(128);
		ReleaseClarinet(128);
		ReleaseBrass(128);
		ReleaseRandomArpeggio(128);
	}
	previouschannel = channel;
	if (channel == 0x00) {
		return NewOrgan(midiNote, velocity); 
	} else if (channel == 0x01) {
		return NewClarinet(midiNote, velocity);
	} else if (channel == 0x02) {
		return NewBrass(midiNote, velocity);
	}
	return -1;
}

void ReleaseNote(unsigned char midiNote){
	static unsigned char previouschannel = 0;
	
	if (previouschannel != channel) {
		ReleaseOrgan(128);
		ReleaseClarinet(128);
		ReleaseBrass(128);
		ReleaseRandomArpeggio(128);
		previouschannel = channel;
		return;
	}
	previouschannel = channel;
	switch (channel) {
		case 0x00: ReleaseOrgan(midiNote); return;
		case 0x01: ReleaseClarinet(midiNote); return;
		case 0x02: ReleaseBrass(midiNote); return;
	}
}

void ReleaseNote2(int i){
	static unsigned char previouschannel = 0;
	
	if (previouschannel != channel) {
		ReleaseOrgan(128);
		ReleaseClarinet(128);
		ReleaseRandomArpeggio(128);
		previouschannel = channel;
		return;
	}
	previouschannel = channel;
	switch (channel) {
		case 0x00: organList[i].released = 1; return;
		case 0x01: clarinetList[i].released = 1; return;
	}
}

/////////////////////////////////////////////////////
//Organ Instrument
#pragma DATA_SECTION(organTable , ".sdram");
float	organTable[TABLE_SIZE];
//#pragma DATA_SECTION(organList , ".sdram");
#pragma DATA_ALIGN(organList, 8);
Organ organList[MAX_CHANNELS];

int Organ_Init(int i) {
	organList[i].released = 0;
	organList[i].active = 1;
	organList[i].finished = 0;
	organList[i].organOsc.table = organTable;
	organList[i].organOsc.tablesize = TABLE_SIZE;
	organList[i].envelope.attackgain = 1*organList[i].gain;
	organList[i].envelope.sustaingain = 0.6*organList[i].gain;
	organList[i].envelope.attacktime = 1000;
	organList[i].envelope.decaytime = 1250;
	organList[i].envelope.releasetime = 750;
	Oscillator_Init(&(organList[i].organOsc));
	Adsr_Init(&(organList[i].envelope));
	//organList[i].organOsc.gain = organList[i].envelope.outBuffer;
	return 0;
}

int Organ_ProcessFrame(int i) {
	float *outBuffer, *oscBuffer, *envelopeBuffer;
	int j, k;

	if (organList[i].active == 0) {
		return 0;
	}
	if (organList[i].finished == 1) {
		organList[i].active = 0;
		return 0;
	}
	
	outBuffer = organList[i].outBuffer;
	oscBuffer = organList[i].organOsc.outBuffer;
	envelopeBuffer = organList[i].envelope.outBuffer;

	organList[i].organOsc.freq = organList[i].freq * pitchBend;

	if (organList[i].released == 1) organList[i].envelope.state = 3;

	j = Adsr_ProcessFrame(&(organList[i].envelope));
	Oscillator_ProcessFrame(&(organList[i].organOsc));
	
	// for (k = 0; k < FRAME_SIZE; k++) {
		// outBuffer[k] = oscBuffer[k] * envelopeBuffer[k];
	// }
	DSPF_sp_vecmul( oscBuffer, envelopeBuffer, outBuffer, FRAME_SIZE);
	
	if (j == 1) organList[i].finished = 1;
	return j;
}


int NewOrgan(unsigned char midiNote, unsigned char velocity) {	
	unsigned int i;
	
	i = 0;
	while ( (organList[i].active !=0) && (i < MAX_CHANNELS) ) {
		i++;	
	}

	if (i == MAX_CHANNELS) {
		return -1;
	}
	
	organList[i].midiNote = midiNote;
	organList[i].freq = 440 * expf(((float)midiNote - 69) * LN2DIV12); //formula for converting MIDI "notes" to frequency
	organList[i].gain = (float)(velocity) / 127; //scale velocity to between 0 and 1
	Organ_Init(i);
	
	return i;
}

void ReleaseOrgan(unsigned char midiNote){
	int i;
	
	for (i = 0; i < MAX_CHANNELS; i++) {
		if (organList[i].active == 1) {
			if ((organList[i].midiNote == midiNote)||(midiNote == 128)){
				organList[i].released = 1;
			}
		}
	}
}

/////////////////////////////////////////////////////
//Clarinet Instrument
//#pragma DATA_SECTION(sineTable , ".sdram");
float	sineTable[TABLE_SIZE];
//#pragma DATA_SECTION(clarinetList , ".sdram");
#pragma DATA_ALIGN(clarinetList, 8);
Clarinet clarinetList[MAX_CHANNELS];

int Clarinet_Init(int i) {
	clarinetList[i].released = 0;
	clarinetList[i].active = 1;
	clarinetList[i].finished = 0;
	clarinetList[i].f2Envelope.attacktime = 25;
	clarinetList[i].f2Envelope.decaytime = 975;
	clarinetList[i].f2Envelope.releasetime = 500;
	Adsr_Init(&(clarinetList[i].f2Envelope));
	clarinetList[i].secondOsc.table = sineTable;
	clarinetList[i].secondOsc.tablesize = TABLE_SIZE;
	//clarinetList[i].secondOsc.gain = clarinetList[i].f2Envelope.outBuffer;
	Oscillator_Init(&(clarinetList[i].secondOsc));
	clarinetList[i].f1Envelope.attackgain = 1 * clarinetList[i].gain;
	clarinetList[i].f1Envelope.sustaingain = 1 * clarinetList[i].gain;
	clarinetList[i].f1Envelope.attacktime = 950;
	clarinetList[i].f1Envelope.decaytime = 50;
	clarinetList[i].f1Envelope.releasetime = 500;
	Adsr_Init(&(clarinetList[i].f1Envelope));
	clarinetList[i].mainOsc.table = sineTable;
	clarinetList[i].mainOsc.tablesize = TABLE_SIZE;
	clarinetList[i].mainOsc.freqTable = clarinetList[i].secondOsc.outBuffer;
	//clarinetList[i].mainOsc.gain = clarinetList[i].f1Envelope.outBuffer;
	VariableOscillator_Init(&(clarinetList[i].mainOsc));
	return 0;
}

int Clarinet_ProcessFrame(int i) {
	float *outBuffer, *mainOscBuffer, *secondOscBuffer, *f1EnvelopeBuffer, *f2EnvelopeBuffer;
	float freq, fc, fm;
	int j, k;
	
	if (clarinetList[i].active == 0) {
		return 0;
	}
	if (clarinetList[i].finished == 1) {
		clarinetList[i].active = 0;
		return 0;
	}
	
	outBuffer = clarinetList[i].outBuffer;
	mainOscBuffer = clarinetList[i].mainOsc.outBuffer;
	secondOscBuffer = clarinetList[i].secondOsc.outBuffer;
	f1EnvelopeBuffer = clarinetList[i].f1Envelope.outBuffer;
	f2EnvelopeBuffer = clarinetList[i].f2Envelope.outBuffer;

	freq = clarinetList[i].freq * pitchBend;
	fc = 3*freq;
	fm = 2*freq + 2.13;
	
	if (clarinetList[i].released == 1) {
		clarinetList[i].f1Envelope.state = 3;
		clarinetList[i].f2Envelope.state = 3;
	}
	
	clarinetList[i].f2Envelope.attackgain = 4*fm;
	clarinetList[i].f2Envelope.sustaingain = 2*fm;
	Adsr_ProcessFrame(&(clarinetList[i].f2Envelope));
	
	clarinetList[i].secondOsc.freq = fm;	
	Oscillator_ProcessFrame(&(clarinetList[i].secondOsc));

	for (k = 0; k < FRAME_SIZE; k++) {
		secondOscBuffer[k] = secondOscBuffer[k]*f2EnvelopeBuffer[k] + fc;
		//secondOscBuffer[k] += fc;
	}
	
	j = Adsr_ProcessFrame(&(clarinetList[i].f1Envelope));
	VariableOscillator_ProcessFrame(&(clarinetList[i].mainOsc));
	
	// for (k = 0; k < FRAME_SIZE; k++) {
		// outBuffer[k] = mainOscBuffer[k] * f1EnvelopeBuffer[k];
	// }
	DSPF_sp_vecmul( mainOscBuffer, f1EnvelopeBuffer, outBuffer, FRAME_SIZE);
	
	
	if (j == 1) clarinetList[i].finished = 1;
	return j;
}

int NewClarinet(unsigned char midiNote, unsigned char velocity) {	
	unsigned int i;
	
	i = 0;
	while ( (clarinetList[i].active !=0) && (i < MAX_CHANNELS) ) {
		i++;	
	}

	if (i == MAX_CHANNELS)
	return -1;
	
	
	clarinetList[i].midiNote = midiNote;
	clarinetList[i].freq = 440 * expf(((float)midiNote - 69) * LN2DIV12); //formula for converting MIDI "notes" to frequency
	clarinetList[i].gain = (float)(velocity) / 127; //scale velocity to between 0 and 1
	Clarinet_Init(i);
	
	return i;
}

void ReleaseClarinet(unsigned char midiNote){
	int i;
	
	for (i = 0; i < MAX_CHANNELS; i++) {
		if (clarinetList[i].active == 1) {
			if ((clarinetList[i].midiNote == midiNote)||(midiNote == 128)){
				clarinetList[i].released = 1;
			}
		}
	}
}


/////////////////////////////////////////////////////
//Brass Instrument
//#pragma DATA_SECTION(brassList , ".sdram");
#pragma DATA_ALIGN(brassList, 8);
Brass brassList[MAX_CHANNELS];

int Brass_Init(int i) {
	brassList[i].released = 0;
	brassList[i].active = 1;
	brassList[i].finished = 0;
	brassList[i].f2Envelope.attacktime = 1000;
	brassList[i].f2Envelope.decaytime = 1000;
	brassList[i].f2Envelope.releasetime = 1000;
	Adsr_Init(&(brassList[i].f2Envelope));
	brassList[i].secondOsc.table = sineTable;
	brassList[i].secondOsc.tablesize = TABLE_SIZE;
	//brassList[i].secondOsc.gain = brassList[i].f2Envelope.outBuffer;
	Oscillator_Init(&(brassList[i].secondOsc));
	brassList[i].f1Envelope.attackgain = 1 * brassList[i].gain;
	brassList[i].f1Envelope.sustaingain = 0.8 * brassList[i].gain;
	brassList[i].f1Envelope.attacktime = 1000;
	brassList[i].f1Envelope.decaytime = 1000;
	brassList[i].f1Envelope.releasetime = 1000;
	Adsr_Init(&(brassList[i].f1Envelope));
	brassList[i].mainOsc.table = sineTable;
	brassList[i].mainOsc.tablesize = TABLE_SIZE;
	brassList[i].mainOsc.freqTable = brassList[i].secondOsc.outBuffer;
	//brassList[i].mainOsc.gain = brassList[i].f1Envelope.outBuffer;
	VariableOscillator_Init(&(brassList[i].mainOsc));
	return 0;
}

int Brass_ProcessFrame(int i) {
	float *outBuffer, *mainOscBuffer, *secondOscBuffer, *f1EnvelopeBuffer, *f2EnvelopeBuffer;
	float freq, fc, fm;
	int j, k;
	
	if (brassList[i].active == 0) {
		return 0;
	}
	if (brassList[i].finished == 1) {
		brassList[i].active = 0;
		return 0;
	}
	
	outBuffer = brassList[i].outBuffer;
	mainOscBuffer = brassList[i].mainOsc.outBuffer;
	secondOscBuffer = brassList[i].secondOsc.outBuffer;
	f1EnvelopeBuffer = brassList[i].f1Envelope.outBuffer;
	f2EnvelopeBuffer = brassList[i].f2Envelope.outBuffer;

	freq = brassList[i].freq * pitchBend;
	fc = freq;
	fm = freq + 0.03;
	
	if (brassList[i].released == 1) {
		brassList[i].f1Envelope.state = 3;
		brassList[i].f2Envelope.state = 3;
	}
	
	brassList[i].f2Envelope.attackgain = 5*fm;
	brassList[i].f2Envelope.sustaingain = 4*fm;
	Adsr_ProcessFrame(&(brassList[i].f2Envelope));
	
	brassList[i].secondOsc.freq = fm;	
	Oscillator_ProcessFrame(&(brassList[i].secondOsc));

	for (k = 0; k < FRAME_SIZE; k++) {
		secondOscBuffer[k] = secondOscBuffer[k]*f2EnvelopeBuffer[k] + fc;
		//secondOscBuffer[k] += fc;
	}
	
	j = Adsr_ProcessFrame(&(brassList[i].f1Envelope));
	VariableOscillator_ProcessFrame(&(brassList[i].mainOsc));
	
	// for (k = 0; k < FRAME_SIZE; k++) {
		// outBuffer[k] = mainOscBuffer[k] * f1EnvelopeBuffer[k];
	// }
	DSPF_sp_vecmul( mainOscBuffer, f1EnvelopeBuffer, outBuffer, FRAME_SIZE);
	
	if (j == 1) brassList[i].finished = 1;
	return j;
}

int NewBrass(unsigned char midiNote, unsigned char velocity) {	
	unsigned int i;
	
	i = 0;
	while ( (brassList[i].active !=0) && (i < MAX_CHANNELS) ) {
		i++;	
	}

	if (i == MAX_CHANNELS)
	return -1;
	
	
	brassList[i].midiNote = midiNote;
	brassList[i].freq = 440 * expf(((float)midiNote - 69) * LN2DIV12); //formula for converting MIDI "notes" to frequency
	brassList[i].gain = (float)(velocity) / 127; //scale velocity to between 0 and 1
	Brass_Init(i);
	
	return i;
}

void ReleaseBrass(unsigned char midiNote){
	int i;
	
	for (i = 0; i < MAX_CHANNELS; i++) {
		if (brassList[i].active == 1) {
			if ((brassList[i].midiNote == midiNote)||(midiNote == 128)){
				brassList[i].released = 1;
			}
		}
	}
}




///////////////////////////////////////////
// Arpeggio

unsigned int oneTwoThree [4] = {4,0,1,2};
unsigned int oneTwoThreeTwo [5] = {5,0,1,2,1};
unsigned int oneThree [3] = {3,0,2};
unsigned int oneFive [3] = {3,0,4};
unsigned int oneThreeFive [4] = {4,0,2,4};
unsigned int oneThreeFiveThree [5] = {5,0,2,4,2};
unsigned int oneThreeFiveThreeFiveThree [7] = {7,0,2,4,2,4,2};
//unsigned int currentScale = 0;
unsigned int *ArpScales[NUMARP_SCALES] = {oneTwoThree,oneTwoThreeTwo,oneThree,oneFive,oneThreeFive,oneThreeFiveThree,oneThreeFiveThreeFiveThree}; //keep updating



/************** SCALES Random Arpeggiator LUT *************************
****
** NOTES: 
** Middle C is dec: 60, hex: 3C
*************************************************/
unsigned char Chromatic[13] = {0x0D,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};
unsigned char HarmonicMinor[8]  = {0x08,0x00,0x02,0x03,0x05,0x07,0x08,0x0B};          // root node will be 0
unsigned char HirajoshiJapan[6] = {0x06,0x00,0x02,0x03,0x07,0x08};            //0x3C,0x3E,0x3F,0x43,0x44-0x48,0x4A,0x4B,0x4F,0x50
//0x54,0x56,0x57,0x5B,0x5C-0x60,0x62,0x63,0x67,0x68
unsigned char Chinese[6] = {0x06,0x00,0x02,0x04,0x07,0x09};
unsigned char C_Major[8] = {0x08,0x00,0x02,0x04,0x05,0x07,0x09,0x0B}; 
int keyOffset = 0; //default scale will be in the key of C -> range of keyOffset is 0 - 11
int harmonizerScaleSelect = 0;
unsigned char Key;
unsigned int scaleLUT_Index;
int HarmonizerEffectOnFlag = 0;



/************** SCALES LUT *************************
****
** NOTES: if it works with one test scale it will work with all?
** Middle C is dec: 60, hex: 3C
*************************************************/
// the Japanese scale
unsigned char JAPANESE[55] = {0x37, //dec 55 -> size of array
	0x00,0x01,0x05,0x07,0x08, //Octave -1
	0x0C,0x0D,0x11,0x13,0x14, //Octave 0
	0x18,0x19,0x1D,0x1F,0x20, //Octave 1
	0x24,0x25,0x29,0x2B,0x2C, //Octave 2
	0x30,0x31,0x35,0x37,0x38, //Octave 3
	0x3C,0x3D,0x41,0x43,0x44, //Octave 4
	0x48,0x49,0x4D,0x4F,0x50, //Octave 5
	0x54,0x55,0x59,0x5B,0x5C, //Octave 6
	0x60,0x61,0x65,0x67,0x68, //Octave 7
	0x6C,0x6D,0x71,0x73,0x74, //Octave 8
	0x78,0x79,0x7D,0x7F}; //Octave 9 

unsigned char CHINESE[55] = {0x37, //dec 55 -> size of array
	0x00,0x02,0x04,0x07,0x09, //Octave -1
	0x0C,0x0E,0x10,0x13,0x15, //Octave 0
	0x18,0x1A,0x1C,0x1F,0x21, //Octave 1
	0x24,0x26,0x28,0x2B,0x2D, //Octave 2
	0x30,0x32,0x34,0x37,0x39, //Octave 3
	0x3C,0x3E,0x40,0x43,0x45, //Octave 4
	0x48,0x4A,0x4C,0x4F,0x51, //Octave 5
	0x54,0x56,0x58,0x5B,0x5D, //Octave 6
	0x60,0x62,0x64,0x67,0x69, //Octave 7
	0x6C,0x6E,0x70,0x73,0x75, //Octave 8
	0x78,0x7A,0x7C,0x7F}; //Octave 9 

// the Harmonic Minor scale
unsigned char HARMONIC_MINOR[76] = {0x4C, //dec 76 -> size of array
	0x00,0x02,0x03,0x05,0x07,0x08,0x0B, //Octave -1
	0x0C,0x0E,0x0F,0x11,0x13,0x14,0x17, //Octave 0
	0x18,0x1A,0x1B,0x1D,0x1F,0x20,0x23, //Octave 1
	0x24,0x26,0x27,0x29,0x2B,0x2C,0x2F, //Octave 2
	0x30,0x32,0x33,0x35,0x37,0x38,0x3B, //Octave 3
	0x3C,0x3E,0x3F,0x41,0x43,0x44,0x47, //Octave 4
	0x48,0x4A,0x4B,0x4D,0x4F,0x50,0x53, //Octave 5
	0x54,0x56,0x57,0x59,0x5B,0x5C,0x5F, //Octave 6
	0x60,0x62,0x63,0x65,0x67,0x68,0x6B, //Octave 7
	0x6C,0x6E,0x6F,0x71,0x73,0x74,0x77, //Octave 8
	0x78,0x7A,0x7B,0x7D,0x7F}; //Octave 9 

unsigned char CHROMATIC[128] = {0x7F,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B, //Octave -1
	0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, //Octave  0 
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23, //Octave  1 
	0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F, //Octave  2
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B, //Octave  3 
	0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47, //Octave  4 
	0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53, //Octave  5
	0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F, //Octave  6
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B, //Octave  7
	0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77, //Octave  8
	0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F }; //Octave 9 

// the WHITEKEYS values checked are db1 of a MIDI event
unsigned char WHITEKEYS[75] = {0x00,0x02,0x04,0x05,0x07,0x09,0x0B, //Octave -1
	0x0C,0x0E,0x10,0x11,0x13,0x15,0x17, //Octave 0
	0x18,0x1A,0x1C,0x1D,0x1F,0x21,0x23, //Octave 1
	0x24,0x26,0x28,0x29,0x2B,0x2D,0x2F, //Octave 2
	0x30,0x32,0x34,0x35,0x37,0x39,0x3B, //Octave 3
	0x3C,0x3E,0x40,0x41,0x43,0x45,0x47, //Octave 4
	0x48,0x4A,0x4C,0x4D,0x4F,0x51,0x53, //Octave 5
	0x54,0x56,0x58,0x59,0x5B,0x5D,0x5F, //Octave 6
	0x60,0x62,0x64,0x65,0x67,0x69,0x6B, //Octave 7
	0x6C,0x6E,0x70,0x71,0x73,0x75,0x77, //Octave 8
	0x78,0x7A,0x7C,0x7D,0x7F }; //Octave 9 


// the BLACKKEYS values checked are db1 of a MIDI event
unsigned char BLACKKEYS[53] = {0x01,0x03,0x06,0x08,0x0A, //Octave -1
	0x0D,0x0F,0x12,0x14,0x16, //Octave 0
	0x19,0x1B,0x1E,0x20,0x22, //Octave 1
	0x25,0x27,0x2A,0x2C,0x2E, //Octave 2
	0x31,0x33,0x36,0x38,0x3A, //Octave 3
	0x3D,0x3F,0x42,0x44,0x46, //Octave 4
	0x49,0x4B,0x4E,0x50,0x52, //Octave 5
	0x55,0x57,0x5A,0x5C,0x5E, //Octave 6
	0x61,0x63,0x66,0x68,0x6A, //Octave 7
	0x6D,0x6F,0x72,0x74,0x76, //Octave 8
	0x79,0x7B,0x7E }; //Octave 9 

/************** SCALES *************************
**
*************************************************/
unsigned char *harmonizerScales[NUMOFHARM_SCALES];    

unsigned char *rArpScales[NUMOFRANDARP_SCALES] = {CHROMATIC, HarmonicMinor, Chinese, HirajoshiJapan, C_Major}; //keep updating
RandomArpeggio rarpeggioList[MAX_CHANNELS];
float newNoteFreq = 1;
int randArpEffectOnFlag = 0;
int randArpScaleSelect = 0;
unsigned int randArpDelay = 0;
int randArpHSnum = -1; 

void RandomArpeggio_Init(int i) {
	int index;
	rarpeggioList[i].released = 0;
	rarpeggioList[i].active = 1;
	rarpeggioList[i].shouldCreate = 1;
	rarpeggioList[i].delay = randArpDelay;
	//if black key, then dont activate
	if (HarmonizerEffectOnFlag&&(harmonizerScaleSelect != 0)) {
		for(index = 0; index < NUM_BLCK_KEYS; index++){
			if(rarpeggioList[i].midiNote == BLACKKEYS[index]){
				rarpeggioList[i].active = 0;
			}
		}
	}
	return;
}

void RandomArpeggio_ProcessFrame(int i) {
	int currentNote;

	if (rarpeggioList[i].active == 0) {
		return;
	}
	
	currentNote = rarpeggioList[i].currentNote;
	if (rarpeggioList[i].released == 1) {
		if ((newNoteFreq == 0)||(rarpeggioList[i].delay == 0)){
			//if (currentNote != -1) organList[currentNote].released = 1;
			if (currentNote != -1) ReleaseNote2(currentNote);
			rarpeggioList[i].active = 0;
			return;
		}
	}
	
	
	if ((rarpeggioList[i].shouldCreate == 1)||(currentNote == -1)) {
		rarpeggioList[i].shouldCreate = 0;
		rarpeggioList[i].frameCounter = 0;
		rarpeggioList[i].currentNote = NewNote( randNote(rarpeggioList[i].midiNote),
		//rarpeggioList[i].currentNote = NewOrgan( randNote(rarpeggioList[i].midiNote),
			rarpeggioList[i].velocity);
	}
	
	rarpeggioList[i].frameCounter++;
	if (newNoteFreq == 0) { }
	else if (rarpeggioList[i].frameCounter
			>= (int)((float)(SAMPLING_RATE/FRAME_SIZE)/newNoteFreq)) {
		//if (currentNote != -1) organList[currentNote].released = 1;
		if (currentNote != -1) ReleaseNote2(currentNote);
		if (rarpeggioList[i].released == 1) rarpeggioList[i].delay--;
		rarpeggioList[i].shouldCreate = 1;
	}
	return;
}

unsigned char randNote(unsigned char root) {
	MIDIEvent tempevent;
	unsigned char randNote = root;
	int numNotesInScale, index;
	int randArpIndex;


	if((harmonizerScaleSelect == 0) || (HarmonizerEffectOnFlag == 0)){   //if scale is chromatic just do this fn
		if(randArpHSnum >= 0){
			randNote = (root + (rand()%randArpHSnum));
		}
		else if(randArpHSnum < 0){
			randNote = (root - (rand()%(-1*randArpHSnum)));		   
		}
		tempevent.db1 = randNote;
		if (HarmonizerEffectOnFlag) ScaleHarmonizer(&tempevent);
		randNote = tempevent.db1;
		return randNote;
	} else {
		for(index = 0; index < NUM_WHITE_KEYS; index++){
			if(root == WHITEKEYS[index]){
				if(randArpHSnum >= 0){
					randNote = WHITEKEYS[index + (rand()%randArpHSnum)];
				}
				else if(randArpHSnum < 0){
					randNote = WHITEKEYS[index - (rand()%randArpHSnum)];		   
				}
				tempevent.db1 = randNote;
				if (HarmonizerEffectOnFlag) ScaleHarmonizer(&tempevent);
				randNote = tempevent.db1;
				return randNote;
			}
		}
	}
	
	
	//else{
		/*	
		if(randArpHSnum >= 0){
		randNote = (harmonizerScales[harmonizerScaleSelect][(scaleLUT_Index + (rand()%randArpHSnum))]);
		}
		else if(randArpHSnum < 0){
		randNote = (root - (rand()%(-1*randArpHSnum)));			   
		}  
	
	
	// numNotesInScale = rArpScales[randArpScaleSelect][0] - 1;	  
	// randArpIndex = rand()%randArpHSnum;
	
		// if(randArpHSnum >= 0 && randArpHSnum < numNotesInScale ){
			// randNote = root + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1];//   randNum will be value 1 to size of scale - 1
			// if(randNote > 0x7F){ randNote = root;}  

	// } else if(randArpHSnum >= numNotesInScale && randArpHSnum < 2*numNotesInScale){
			// randNote = root + 12 + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1]; //previously had randArpHSnum in last []
			// if(randNote > 0x7F){ randNote = root;}  
			
	// }  else if(randArpHSnum >= 2*numNotesInScale && randArpHSnum < 3*numNotesInScale){
			// randNote = root + 2*12 + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1];
			// if(randNote > 0x7F){ randNote = root;}  
			
	// }  else if(randArpHSnum >= 3*numNotesInScale && randArpHSnum < 4*numNotesInScale){
			// randNote = root + 3*12 + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1];
			// if(randNote > 0x7F){ randNote = root;}  
			
	// }  else if(randArpHSnum >= 4*numNotesInScale && randArpHSnum < 5*numNotesInScale){
			// randNote = root + 4*12 + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1];
			// if(randNote > 0x7F){ randNote = root;}  
			
	// }  else if(randArpHSnum >= 5*numNotesInScale && randArpHSnum < 6*numNotesInScale){
			// randNote = root + 5*12 + rArpScales[randArpScaleSelect][(randArpIndex%numNotesInScale) + 1];
			// if(randNote > 0x7F){ randNote = root;}  

	// }   else if(randArpHSnum >= 5*numNotesInScale){
			// randNote = root; //default to root

	// }  else if(randArpHSnum < 0){
			// randNote = (root - (rand()%(-1*randArpHSnum)));
	// }  

		*/
	//}
	if(randNote > 0x7F){ randNote = root;}  //default back to root if hex value is out of bounds
	return randNote;
}

void NewRandomArpeggio(unsigned char midiNote, unsigned char velocity) {	
	unsigned int i;
	
	i = 0;
	while ( (rarpeggioList[i].active !=0) && (i < MAX_CHANNELS) ) {
		i++;	
	}

	if (i == MAX_CHANNELS)
	return;
	
	rarpeggioList[i].midiNote = midiNote;
	rarpeggioList[i].velocity = velocity;
	RandomArpeggio_Init(i);
	
	return;
}

void ReleaseRandomArpeggio(unsigned char midiNote){
	int i;
	
	for (i = 0; i < MAX_CHANNELS; i++) {
		if (rarpeggioList[i].active == 1) {
			if ((rarpeggioList[i].midiNote == midiNote)){
				rarpeggioList[i].released = 1;
			}
			if (midiNote == 128) {
				rarpeggioList[i].released = 1;
				rarpeggioList[i].delay = 0;
			}
		}
	}
}


void ScaleHarmonizer(MIDIEvent *event){
	unsigned char tempDB1,boundsCheck; 
	unsigned int index = 0;

	harmonizerScales[0] = CHROMATIC;
	harmonizerScales[1] = JAPANESE;
	harmonizerScales[2] = HARMONIC_MINOR;
	harmonizerScales[3] = CHINESE;

	//if( event->db1 > 0x7F ){ event->db1 = 0x01;} //make it a black key which wont sound
	
	
	//if(harmonizerScales[harmonizerScaleSelect][0] != 0x7F){   //if we are using the Chromatic scale we dont care about this fn
	if(harmonizerScaleSelect != 0){   //if we are using the Chromatic scale we dont care about this fn
		
		//make black keys silent
		for(index = 0; index < NUM_BLCK_KEYS; index++){
			if(event->db1 == BLACKKEYS[index]){
				event->db2 = 0x01; //vel 1  -> almost no sound
			}
		}
				
		tempDB1 = event->db1;
		for(index = 0; index < NUM_WHITE_KEYS; index++){
			if(tempDB1 == WHITEKEYS[index]){
				scaleLUT_Index = index; //use the index of found white key to play "correct" note in specified scale
				if(scaleLUT_Index < harmonizerScales[harmonizerScaleSelect][0]){ // < size of scale array 
					//boundsCheck = harmonizerScales[harmonizerScaleSelect][(scaleLUT_Index + (rand()%randArpHSnum))] + keyOffset;
					boundsCheck = harmonizerScales[harmonizerScaleSelect][scaleLUT_Index] + keyOffset;
					if( boundsCheck >  0x7F){event->db1 = 0x00; event->db2 = 0x01;}
					// else {if(randArpHSnum >= 0){ event->db1 = keyOffset + (harmonizerScales[harmonizerScaleSelect][(scaleLUT_Index + (rand()%randArpHSnum))]);}
						// else if(randArpHSnum < 0){ event->db1 = (event->db1 - (rand()%(-1*randArpHSnum)));	}//sevent->db1 = harmonizerScales[harmonizerScaleSelect][scaleLUT_Index] + keyOffset;}
					// }
					else {event->db1 = harmonizerScales[harmonizerScaleSelect][scaleLUT_Index] + keyOffset;}
				}else{event->db1 = 0x00; event->db2 = 0x01; } //default to note 0 velocity 0  //was 0x00				
			}
		}
		//if(harmonizerScales[harmonizerScaleSelect][0] == 0x37){event->db1 = event->db1 - 23;}
	}
	//event->db1 = event->db1 + keyOffset;
} 

//if(randArpHSnum >= 0){ event->db1 = keyOffset + (harmonizerScales[harmonizerScaleSelect][(scaleLUT_Index + (rand()%randArpHSnum))]);}
//else if(randArpHSnum < 0){ event->db1 = (event->db1 - (rand()%(-1*randArpHSnum)));	}
