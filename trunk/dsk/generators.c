#include "common.h"

/////////////////////////////////////////////////////
//ADSR Generator
int Adsr_Init(Adsr *this) {
	this->state = 0;  //state 0 is the attack phase
	this->currentvalue = 0;
	this->inrelease = 0;
	return 0;
}

int Adsr_ProcessFrame(Adsr *this) {
	int index;
	float value, increment;

	float *outBuffer;		
	float attackgain;
	float sustaingain;
	unsigned int attacktime;
	unsigned int decaytime;	
	unsigned int releasetime;
	int state;	
	float currentvalue;

	outBuffer = this->outBuffer;
	attackgain  = this->attackgain;
	sustaingain = this->sustaingain;
	attacktime = this->attacktime;
	decaytime = this->decaytime;
	releasetime = this->releasetime;
	state = this->state;
	currentvalue = this->currentvalue;

	if (state == 0) { //attack phase
		//calculate slope
		increment = attackgain / attacktime;

		//calculate first value
		value = currentvalue;

		//fill buffer until frame ends or attack ends
		for (index = 0; (index<FRAME_SIZE) && (value<=attackgain); index++) {
			outBuffer[index] = value;
			value += increment;
		}

		//do decay if we didn't finish the frame
		if (index < FRAME_SIZE) {
			state = 1; //decay phase
			increment = (sustaingain - attackgain) / decaytime;
			value = attackgain + increment;
			//fill buffer until frame ends or attack ends
			for (; (index<FRAME_SIZE) && (value>=sustaingain); index++) {
				outBuffer[index] = value;
				value += increment;
			}

			//do sustain if we didn't finish the frame,  i.e. decay time is short
			if (index < FRAME_SIZE) {
				state = 2; //sustain phase
				value = sustaingain;
				//fill buffer until frame ends
				for (; index < FRAME_SIZE; index++) {
					outBuffer[index] = value;
				}
			}
		}

	} else if (state == 1) { //decay phase
		//calculate slope
		increment = (sustaingain - attackgain) / decaytime;

		//calculate first value
		value = currentvalue;

		//fill buffer until frame ends or decay ends
		for (index = 0; (index<FRAME_SIZE) && (value>=sustaingain); index++) {
			outBuffer[index] = value;
			value += increment;
		}

		//do sustain if we didn't finish the frame
		if (index < FRAME_SIZE) {
			state = 2; //sustain phase
			value = sustaingain;
			//fill buffer until frame ends
			for (; index < FRAME_SIZE; index++) {
				outBuffer[index] = value;
			}
		}

	} else if (state == 2) { //sustain phase
		//fill buffer until frame ends
		value = sustaingain;
		for (index = 0; index<FRAME_SIZE; index++) {
			outBuffer[index] = value;
		}

	} else if (state == 3) { //release phase
		if (this->inrelease == 0) { //if this is the first frame in release phase
			sustaingain = currentvalue;		//this fixes the case where the Adsr
			this->sustaingain = currentvalue;  //wasn't in sustain before release
			this->inrelease = 1;
		}
		
		increment = -sustaingain / releasetime;
		value = currentvalue + increment;
		
		//fill buffer until frame ends or release ends
		for (index = 0; (index<FRAME_SIZE) && (value>=0); index++) {
			outBuffer[index] = value;
			value += increment;
		}
		//fill with zeros if we didn't finish the frame
		if (index < FRAME_SIZE) {
			for (; index < FRAME_SIZE; index++) {
				outBuffer[index] = 0;
			}
			return 1; //Adsr should be destroyed
		}
	}

	this->state = state;
	this->currentvalue = value;

	return 0;
}


/////////////////////////////////////////////////////
//Oscillator Generator
int Oscillator_Init(Oscillator *this) {
	this->currentindex = 0;
	return 0;
}

int Oscillator_ProcessFrame(Oscillator *this) {
	float *outBuffer, *table, freq, index, increment, *gain;
	unsigned int tablesize, i, nindex;

	outBuffer = this->outBuffer;
	//gain = this->gain;
	table = this->table;
	tablesize = this->tablesize;
	freq = this->freq;
	
	increment = SAMPLING_PERIOD * freq * tablesize;
	index = this->currentindex;

	for (i = 0; i < FRAME_SIZE; i++) {
		if (index >= tablesize)
			index -= tablesize;
		//nindex = ((unsigned int)index)&TABLEMASK; 
		//outBuffer[i] = gain[i] * table[(int)(index)]; 
		outBuffer[i] = table[(int)(index)]; 
		index += increment;
	}

	this->currentindex = index;
	return 0;
}

/////////////////////////////////////////////////////
//Variable Oscillator Generator
int VariableOscillator_Init(VariableOscillator *this) {
	this->currentindex = 0;
	return 0;
}

int VariableOscillator_ProcessFrame(VariableOscillator *this) {
	float *outBuffer, *table, *freqTable, index, increment, *gain;
	unsigned int tablesize, i;

	outBuffer = this->outBuffer;
	table = this->table;
	//gain = this->gain;
	tablesize = this->tablesize;
	freqTable = this->freqTable;
	
	index = this->currentindex;

	for (i = 0; i < FRAME_SIZE; i++) {
		if (index >= tablesize)
			index -= tablesize;
		outBuffer[i] = table[(int)(index)]; 
		//outBuffer[i] = gain[i] * table[(int)(index)]; 
		increment = SAMPLING_PERIOD * freqTable[i] * tablesize;
		index += increment;
	}

	this->currentindex = index;
	return 0;
}

