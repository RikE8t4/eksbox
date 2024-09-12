#ifndef CONVERT_STX_H
#define CONVERT_STX_H


//=============================
//		ConvertStx.h
//=============================

#include <fstream>


void StxToXbadpcm (FILE* in, FILE* out, const int& channels,
								int offset, const bool& surround,
								const bool& progressEnable, float& progress);

void XbadpcmToStx (FILE* in, FILE* out, const int& channels,
							const int& sampleRate,
							const int& wavHdrSize, const bool& looping);


extern struct{
	unsigned char	id[4];
	unsigned short	hdrSize;
	unsigned short	channels;
	unsigned short	unknown1;
	unsigned short	unknown2;
	unsigned short	unknown3;
	unsigned short	null1;
	unsigned short	pages;
	unsigned short	curPage;
	unsigned short	null2;
	unsigned short	interleaving; // Also related to loop/file start & end pos
	unsigned short	nextPage;
	unsigned short	loopStart;
	unsigned short	loopEnd;
	unsigned short	null3;
} stx;



#endif


