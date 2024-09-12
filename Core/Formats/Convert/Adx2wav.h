#ifndef ADX2WAV_H
#define ADX2WAV_H


//=============================
//		Adx2wav.h
//=============================

#include <stdio.h>

//#define	BASEVOL	0x11e0
//#define	BASEVOL	0x4000

#define	BASEVOL	0x4500
#define CLIP(x) if (x > 32767) x = 32767; else if (x < -32768) x = -32768
#define AND8(d) if (d &8) d -= 16


typedef struct 
{
	int s1, s2;
} PREV;


extern long read_long(unsigned char *p);
extern void adx2wav(const short channels, const int sampleRate, 
			 const int pos, FILE* in, FILE* out);


#endif