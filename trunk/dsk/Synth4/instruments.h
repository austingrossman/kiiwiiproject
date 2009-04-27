#ifndef _INSTRUMENTS_H_
#define _INSTRUMENTS_H_

int NewNote(unsigned char midiNote, unsigned char velocity);
void ReleaseNote(unsigned char midiNote);
void ReleaseNote2(int i);


///////////////////////////////////////////
//Organ shit
typedef struct Organ {
	float outBuffer[FRAME_SIZE];
	float freq;
	float gain;				//scale the velocity of the note to between 0 and 1;
	Oscillator organOsc;		//private
	Adsr envelope;			//private
	unsigned char midiNote;
	unsigned char released;	//set to 1 when noteOff occurs
	unsigned char active; //0 if unactive, 1 if active
	unsigned char finished;
	int placeholder;	//to make the organ double word aligned
} Organ;

extern float organTable[TABLE_SIZE];
extern Organ organList[MAX_CHANNELS];
extern float pitchBend;

int Organ_Init(int i);
int Organ_ProcessFrame(int i);
int NewOrgan(unsigned char midiNote, unsigned char velocity);
void ReleaseOrgan(unsigned char midiNote);


///////////////////////////////////////////
//Clarinet shit
typedef struct Clarinet {
	float outBuffer[FRAME_SIZE];
	float freq;
	float gain;				//scale the velocity of the note to between 0 and 1;
	VariableOscillator mainOsc;		//private
	Oscillator secondOsc;	//private
	Adsr f1Envelope;			//private
	Adsr f2Envelope;			//private
	unsigned char midiNote;
	unsigned char released;	//set to 1 when noteOff occurs
	unsigned char active; //0 if unactive, 1 if active
	unsigned char finished;
	int placeholder;	//to make the organ double word aligned
} Clarinet;

extern float sineTable[TABLE_SIZE];
extern Clarinet clarinetList[MAX_CHANNELS];

int Clarinet_Init(int i);
int Clarinet_ProcessFrame(int i);
int NewClarinet(unsigned char midiNote, unsigned char velocity);
void ReleaseClarinet(unsigned char midiNote);

///////////////////////////////////////////
//Brass shit
typedef struct Brass {
	float outBuffer[FRAME_SIZE];
	float freq;
	float gain;				//scale the velocity of the note to between 0 and 1;
	VariableOscillator mainOsc;		//private
	Oscillator secondOsc;	//private
	Adsr f1Envelope;			//private
	Adsr f2Envelope;			//private
	unsigned char midiNote;
	unsigned char released;	//set to 1 when noteOff occurs
	unsigned char active; //0 if unactive, 1 if active
	unsigned char finished;
	int placeholder;	//to make the organ double word aligned
} Brass;

extern Brass brassList[MAX_CHANNELS];

int Brass_Init(int i);
int Brass_ProcessFrame(int i);
int NewBrass(unsigned char midiNote, unsigned char velocity);
void ReleaseBrass(unsigned char midiNote);


//////////////////////////
//Arpeggio shit

#define NUMARP_SCALES 7   //Keep updating
extern int ArpEffectOnFlag;
extern int ArpScaleSelect;

///////////////////////////////////////////
//Random Arpeggio and Harmonizer shit
#define NUMOFRANDARP_SCALES 5   //Keep updating

#define NUM_BLCK_KEYS              53
#define NUM_WHITE_KEYS             75
#define JAP_SCALE_NUM_NOTES        55
#define HAR_MIN_SCALE_NUM_NOTES    75
#define CHIN_SCALE_NUM_NOTES       55
#define CHROMATIC_SCALE_NUM_NOTES  128
#define NUMOFHARM_SCALES           4

extern unsigned int currentScale;
extern int HarmonizerEffectOnFlag;
extern int harmonizerScaleSelect;
extern int keyOffset;
extern unsigned char CHROMATIC[];
extern unsigned char *harmonizerScales[];
extern unsigned int scaleLUT_Index;

typedef struct RandomArpeggio {
	unsigned char midiNote;
	unsigned char velocity;	
	unsigned char released;	//set to 1 when noteOff occurs
	unsigned char active; //0 if unactive, 1 if active
	unsigned int delay;
	int currentNote;	//private
	int frameCounter;	//private
	unsigned char shouldCreate;	//private
} RandomArpeggio;

extern RandomArpeggio rarpeggioList[MAX_CHANNELS];
extern float newNoteFreq;
extern int randArpHSnum;
extern int randArpScaleSelect;
extern int randArpEffectOnFlag;
//extern unsigned int *rArpScales[NUMOFRANDARP_SCALES];
extern unsigned char *rArpScales[NUMOFRANDARP_SCALES];
extern unsigned int randArpDelay;

void RandomArpeggio_Init(int i);
void RandomArpeggio_ProcessFrame(int i);
unsigned char randNote(unsigned char root);
void NewRandomArpeggio(unsigned char midiNote, unsigned char velocity);
void ReleaseRandomArpeggio(unsigned char midiNote);


#endif //_INSTRUMENTS_H_
