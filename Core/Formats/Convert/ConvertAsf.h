

#include <stdafx.h>
#pragma hdrstop
//#include "Main.h"
#include "Gui.h"

#ifndef CONVERT_ASF_H
#define CONVERT_ASF_H

//=============================
//		ConvertAsf.h
//=============================

typedef __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;
#define HINIBBLE(byte) ((byte) >> 4)
#define LONIBBLE(byte) ((byte) & 0x0F)

//------------------------------------------------------------------------------


#define CLAMP_TO_SHORT(value) \
if (value > 32767) \
	value = 32767; \
else if (value < -32768) \
	value = -32768; \


#define LE_16(x)  ((((uint8_t*)(x))[1] << 8) | ((uint8_t*)(x))[0])

#define LE_32(x)  ((((uint8_t*)(x))[3] << 24) | \
                   (((uint8_t*)(x))[2] << 16) | \
                   (((uint8_t*)(x))[1] << 8) | \
					((uint8_t*)(x))[0])



static int ea_adpcm_table[] = {
	0, 240, 460, 392, 0, 0, -208, -220, 0, 1,
	3, 4, 7, 8, 10, 11, 0, -1, -3, -4
};

static const int step_table[89] = {
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
	19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
	130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
	337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
	876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
	5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};



//------------------------------------------------------------------------------

extern long Clip16BitSample(LONG sample);

extern void AsfToPcm (FILE* in, FILE* out, u_int offset);
extern void AsfSplitToPcm (FILE* in, FILE* out, u_int offset);

extern void TestAsfToPcm (FILE* in, FILE* out, u_int offset);
extern int adpcm_decode_frame(void *data, int *data_size,
				uint8_t *buf, int buf_size);


#endif
