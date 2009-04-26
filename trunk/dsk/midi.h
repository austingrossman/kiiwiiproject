#ifndef _MIDI_H_
#define _MIDI_H_


typedef struct{
	unsigned char sb;
	unsigned char db1;
	unsigned char db2;
	unsigned char db3;
} MIDIEvent;


void QueueMIDIEvent( MIDIEvent fullEventAssignIn);
void QueueMIDIEvent2(  unsigned char sb, unsigned char db1, unsigned char db2);
MIDIEvent* De_QueueMIDIEvent(void);
void ScaleHarmonizer(MIDIEvent *event);

extern unsigned char channel;
void processMIDIEvent(MIDIEvent *event);

void MIDI_Poll(void);

#endif //_MIDI_H_
