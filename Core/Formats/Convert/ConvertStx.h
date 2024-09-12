#ifndef CONVERT_STX_H
#define CONVERT_STX_H


//=============================
//		ConvertStx.h
//=============================

#include <fstream>

#define ushort unsigned short
#define uchar unsigned char

void StxToXbadpcm (FILE* in, FILE* out, const int& channels,
								int offset, const bool& surround,
								const bool& progressEnable, float& progress);

void XbadpcmToStx (FILE* in, FILE* out, const int& channels,
							const int& sampleRate,
							const int& wavHdrSize, const bool& looping);


extern struct{
	uchar	id[4];
	ushort	hdrSize;
	ushort	channels;
	ushort	unknown1;
	ushort	unknown2;
	ushort	unknown3;
	ushort	null1;
	ushort	pages;
	ushort	curPage;
	ushort	null2;
	ushort	interleaving; // Also related to loop/file start & end pos
	ushort	nextPage;
	ushort	loopStart;
	ushort	loopEnd;
	ushort	null3;
} stx;



#endif


