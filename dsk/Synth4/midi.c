#include "common.h"

//globals
MIDIEvent MIDIEventArray[QUEUE_SIZE];
int  MIDIQueueIndex_In = 0; //circular Queue/Buffer in index
int  MIDIQueueIndex_Out = 0; //circular Queue/Buffer out index

void QueueMIDIEvent( MIDIEvent fullEventAssignIn){
	MIDIEventArray[(MIDIQueueIndex_In)++] = fullEventAssignIn;
	if(MIDIQueueIndex_In == QUEUE_SIZE){MIDIQueueIndex_In = 0;}
}

void QueueMIDIEvent2( unsigned char sb, unsigned char db1, unsigned char db2) {
	MIDIEvent event;
	event.sb=sb;
	event.db1=db1;
	event.db2=db2;
	event.db3=0;
	QueueMIDIEvent(event);
}

MIDIEvent* De_QueueMIDIEvent(void){
	if(MIDIQueueIndex_Out == QUEUE_SIZE){MIDIQueueIndex_Out = 0;} //if count is more than QUEUE_SIZE, then wrap around back to 0
	if(MIDIQueueIndex_Out == MIDIQueueIndex_In){ return NULL; }
	return MIDIEventArray + MIDIQueueIndex_Out++;

}


#define NOTEON			     0x90
#define NOTEOFF			     0x80
#define PITCHBEND		     0xE0  //MIDI Controller pitch bend wheel
#define	CONTROLCHANGE     	 0xB0
#define VOLUME 	    		 0x07  //MIDI Controller slider F9
#define ANALOGINVOLUME		 0x54  //MIDI Controller slider F8
#define WAHPARAM		     0x01  //MIDI Controller modulation wheel
#define WAHWET		    	 0x5B  //MIDI Controller slider F3
#define PHASORPARAM	    	 0x48  //MIDI Controller slider F6
#define PHASORWET	    	 0x49  //MIDI Controller slider F3
#define PULSORWET			 0x4A  //MIDI Controller slider F1
#define PULSORFREQ			 0x47  //MIDI Controller slider F2
#define SCALEHARMONIZER	     0x19  //MIDI Controller button "record(circle)"
#define SCALEHARMONIZERKEY   0x5F  //MIDI Controller knob C17
//#define SCALEHARMKEYNEG      0x0D  //MIDI Controller knob C13
#define RANDARPEGGIATOR	     0x18  //MIDI Controller button "play"
#define NEGARPRANGE          0x02  //MIDI Controller knob C11
#define POSARPRANGE          0x5C  //MIDI Controller knob C16
#define RANDARPEGGIATORDELAY 0x0C  //MIDI Controller knob C12
#define RANDARPEGGIATORFREQ  0x5D  //MIDI Controller slider F4
#define SCALESELECTOR        0x0A  //MIDI Controller knob C10
#define LEFTSELECT           0X15  //MIDI Controller button "left arrow"
#define RIGHTSELECT          0X16  //MIDI Controller button "right arrow"
#define MUTE		         0x14  //MIDI Controller button "circular arrow" 
#define DELAY				 0x05

unsigned char channel=0;

void processMIDIEvent(MIDIEvent *event) {
	int keyCheck,scaleCheck;
	
	channel = event->sb & 0x0F;
	

	if (((event->sb & 0xF0) == NOTEON)&&(event->db2 != 0)) {
		if (randArpEffectOnFlag){
			NewRandomArpeggio(event->db1, event->db2);
		} else {
			if (HarmonizerEffectOnFlag) {
				ScaleHarmonizer(event);
			}
			//if(event->db1 > 0x70){ event->db1 = 0x70;}//default to 0x70
			NewNote(event->db1, event->db2);
		}

	} else if (((event->sb & 0xF0) == NOTEOFF)||((event->sb & 0xF0) == NOTEON)){ //noteOff event
		if (randArpEffectOnFlag){
			ReleaseRandomArpeggio(event->db1);
		} else {
			if (HarmonizerEffectOnFlag) {
				ScaleHarmonizer(event);
			}
			//if(event->db1 > 0x70){ event->db1 = 0x70;}//default to 0x70
			ReleaseNote(event->db1);
		}

	} else if ((event->sb & 0xF0) == PITCHBEND) { //pitchbend
		if ((event->db1 == 0x00)||(event->db1 == 0x7F && event->db2 == 0x7F)) {
			pitchBend = expf((((float)event->db2-64)/16) * LN2DIV12);
		}
		
	} else if ((event->sb & 0xF0) == CONTROLCHANGE) {
		if ((event->db1) == VOLUME) {  //volume
			mixer.gain = (float)event->db2*(float)event->db2 / 16129;
			
		} else if ((event->db1) == ANALOGINVOLUME) {  
			mixer.analogInGain = 5*(float)event->db2*(float)event->db2 / 16129;
			
		} else if ((event->db1) == WAHPARAM) { //wah parameter
			//wahEffect.parameter = (float)event->db2 / 127;
			wahEffect.parameter = 0.15*expf((float)event->db2 / 127 * 2.590267165f);
			
		} else if ((event->db1) == WAHWET) {
			wahEffect.wet = (float)event->db2 / 127;
			
		} else if ((event->db1) == PHASORPARAM) {
			//phasorEffect.parameter = (float)event->db2 / 127;
			//vibratoEffect.lfo.freq = expf( (float)event->db2 * 0.0235884431f);
			vibratoEffect.lfo.freq = expf(4*event->db2/127*0.6931471806f);
			//if (event->db2 == 0) vibratoEffect.freq = 0;
			
		} else if ((event->db1) == PHASORWET) {
			//phasorEffect.wet = (float)event->db2 / 127;
			//L0(-1);
			vibratoEffect.intensity = expf(8.6*event->db2/127*0.6931471806f);
			if (event->db2 == 0) vibratoEffect.intensity = 0;
			
		} else if ((event->db1) == PULSORWET) {
			L1(-1);
			pulsorEffect.wet = (float)event->db2 / 127;
			
		} else if ((event->db1) == PULSORFREQ) {;
			pulsorEffect.freq = expf( (float)event->db2 * 0.0235884431f);
			if (event->db2 == 0) pulsorEffect.freq = 0;
			
		} else if ((event->db1) == SCALEHARMONIZER) {
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); ReleaseRandomArpeggio(128);
			if( HarmonizerEffectOnFlag == 0 ){HarmonizerEffectOnFlag = 1;}  //Turn on harmonizer effect 
			else{HarmonizerEffectOnFlag = 0;}  //Turn off Harmonizer effect
			
		} else if ((event->db1) == RANDARPEGGIATOR) {;
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); ReleaseRandomArpeggio(128);
			if( randArpEffectOnFlag == 0 ){randArpEffectOnFlag = 1;}  //Turn on random arp effect  
			else{randArpEffectOnFlag = 0; randArpHSnum = 1;}  //Turn off rand arp effect  //reset range to root note
			
		} else if ((event->db1) == RANDARPEGGIATORFREQ) {;
			newNoteFreq = expf( (float)event->db2 * LN2DIV25);
			if (event->db2 == 0) newNoteFreq = 0;

		} else if ((event->db1) == RANDARPEGGIATORDELAY) {;
			randArpDelay = event->db2;
			if (event->db2 == 0x7F) randArpDelay = 100000;
			
		} else if ( HarmonizerEffectOnFlag && (event->db1) == SCALESELECTOR) {
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); //ReleaseRandomArpeggio(128);
			harmonizerScaleSelect = (int)((float)(event->db2)*NUMOFHARM_SCALES/128);
			
		} else if ( HarmonizerEffectOnFlag && (event->db1) == LEFTSELECT) {
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); //ReleaseRandomArpeggio(128);
			if( (harmonizerScaleSelect -1) == -1){ harmonizerScaleSelect = NUMOFHARM_SCALES - 1; }
			else{harmonizerScaleSelect = (harmonizerScaleSelect - 1);}
			
		} else if (HarmonizerEffectOnFlag && (event->db1) == RIGHTSELECT) {
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); //ReleaseRandomArpeggio(128);
			harmonizerScaleSelect = (harmonizerScaleSelect +1)%NUMOFHARM_SCALES;
			
		} else if (HarmonizerEffectOnFlag && (event->db1) == SCALEHARMONIZERKEY ) {
			ReleaseOrgan(128); ReleaseClarinet(128); ReleaseBrass(128); //ReleaseRandomArpeggio(128);
			keyOffset = (int)(((float)event->db2-63)/5.25);			
			
		} else if ( randArpEffectOnFlag && (event->db1) == NEGARPRANGE) {
			if(event->db2 > 30){randArpHSnum = -30;}
			else if(event->db2 == 0){randArpHSnum = 1;}  //play only root note
			else{randArpHSnum = -1*event->db2;}	
			
		} else if ( randArpEffectOnFlag && (event->db1) == POSARPRANGE) {
		    if(event->db2 > 30){randArpHSnum = 30;}
			else if(event->db2 == 0){randArpHSnum = 1;}  //play only root note
			else{randArpHSnum = event->db2;}
			
		} else if ( randArpEffectOnFlag && (event->db1) == LEFTSELECT) {
			randArpHSnum = randArpHSnum - 1; 
			if(randArpHSnum == 0) randArpHSnum = -1;  
			
		} else if (randArpEffectOnFlag && (event->db1) == RIGHTSELECT) {
			randArpHSnum = randArpHSnum + 1; 
			if(randArpHSnum == 0) randArpHSnum = 1; 	
			
		} else if ((event->db1) == MUTE) {
			muteEffect.active = muteEffect.active ^ 0x01;
			ReleaseRandomArpeggio(128);
		} else if ((event->db1) == DELAY) {
//			delayEffect.delay = 384*event->db2;
		}
	}
	
}


void MIDI_Poll(void) {
	unsigned char data;
	static MIDIEvent event;
	
	MIDI_Read(&data, 1, 1);
	if (data >= 0x80) { //if status byte
		event.sb = data;
		return;
	}
	event.db1 = data;
	
	MIDI_Read(&data, 1, 1);
	if (data >= 0x80) { 
		event.sb = data;
		return;
	}
	event.db2 = data;
	
	QueueMIDIEvent(event);
	M1(-1);
}
