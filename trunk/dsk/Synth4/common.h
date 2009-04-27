#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <soc.h>
#include <csl_chip.h>
#include <csl_intc.h>
#include <csl_dmax.h>
#include <csl_mcasp.h>
#include "DSPF_sp_vecmul.h"
#include "DSPF_sp_blk_move.h"


#define PI					(3.14159265358979f)
//#define SAMPLING_PERIOD 	(10.416667E-6)   // 96k
#define SAMPLING_PERIOD 	(0.0000208333)   // 48k
//#define SAMPLING_RATE		96000
#define SAMPLING_RATE		48000
#define FRAME_SIZE  		128
#define MAX_CHANNELS 		16
#define TABLE_SIZE			2048
#define QUEUE_SIZE 			32
#define GAIN				2147483647 //2^31-1
#define LN2DIV12 			0.057762265f
#define LN2DIV25			0.0277258872f

#include "PADK.h"

#include "initialization.h"
#include "generators.h"
#include "instruments.h"
#include "effects.h"
#include "mixer.h"
#include "midi.h"
#include "led.h"

//globals
extern float analogInBuffer[FRAME_SIZE];

#endif //_COMMON_H_
