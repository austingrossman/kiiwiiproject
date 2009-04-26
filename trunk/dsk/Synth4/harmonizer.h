#ifndef _HARMONIZER_H_
#define _HARMONIZER_H_

#define NUM_BLCK_KEYS              53
#define NUM_WHITE_KEYS             75
#define JAP_SCALE_NUM_NOTES        55
#define HAR_MIN_SCALE_NUM_NOTES    75
#define CHIN_SCALE_NUM_NOTES       55
#define CHROMATIC_SCALE_NUM_NOTES  128
#define NUMOFHARM_SCALES           4



extern unsigned int currentScale;
extern int HarmonizerEffectOnFlag;
extern int firstNoteFlag;
extern int harmonizerScaleSelect;
extern unsigned char keyOffset;
extern unsigned char CHROMATIC[];
extern unsigned char *harmonizerScales[];
extern unsigned int scaleLUT_Index;

#endif //_HARMONIZER_H_
