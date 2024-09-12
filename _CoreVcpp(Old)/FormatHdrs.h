#ifndef FORMAT_HDRS_H
#define FORMAT_HDRS_H

#include "stdafx.h"

/******************************
*		FormatHdrs.h
******************************/

// Container Formats

extern struct
{
	unsigned int	index[4];
	unsigned int	fileNameIndex[4];
	unsigned int   	fileNameIndexEnd;
	unsigned char	fileName[80];
	unsigned int	fileNameLength[4];
	unsigned long	padding[4];
}rcf;


extern struct
{
	unsigned long  	hdrPos;
	unsigned short	ver[2];
	unsigned long	padding[4];
	unsigned int	headerSize[4];
	unsigned int   	filenameDir[4];
	unsigned char	bankType[2];
	unsigned char	waveEntries[2];
	unsigned char	loopZeroTail[2];
	unsigned char	convertSample[2];
	unsigned int 	detailsLength[4];
	unsigned int 	offset1[4];
	bool			x360;
}xwb;




// Audio Formats


////Microsoft Adpcm
//struct
//{
//uchar 	header1[4];
//ulong 	size;
//uchar 	header2[8];
//uint	sizeofFmtBlock;
//ushort	format;
//ushort	channels;
//ushort	sampleRate;
//uint	bitRate;
//ushort	blockAlign;
//ushort	bits;
//ushort	unknown1; //Error
//ushort	unknown2;
//uint 	unknown3;
//ushort unknown4;
//ushort	unknown5;
//ushort	unknown6;
//uint	unknown7;
//uint	unknown8;
//uint	unknown9;
//uint	unknown10;
//ushort	unknown11;
////uchar 	fact[4];
////uint	factUnknown;
////uchar factChunk[4];
//uchar	header3[4];
//ulong	subChunk2Size;
//};






// Xbox Adpcm
extern struct 
{
uchar	header1[4];
ulong 	size;
uchar 	header2[8];
uint 	sizeofFmtBlock;
ushort	format;
ushort	channels;
ushort	sampleRate;
uint	bitRate;
ushort	blockAlign;
ushort	bits;
uint	unknown;
uchar	header3[4];
ulong	subChunk2Size;
}xadpcm;



// Pcm
extern struct
{
uchar 	header1[4];
ulong 	size;
uchar 	header2[8];
ulong 	sizeofFmtBlock;
ushort	format;
ushort	channels;
ushort	sampleRate;
ulong	bitRate;
ushort	blockSize;
ushort	bits;
uchar 	header3[4];
ulong	subChunk2Size;								
}pcm;




// Rsd4 Pcm
extern struct
{
uchar	header[7];
uchar	fullHeader[128];
ushort	channelsOffset;
ushort	bitsOffset;
ushort	sampleRateOffset;
uchar	pcmPadding[1920];// Not economical
ushort	channels[2];
ushort	bits[2];
uint	sampleRate[4];
}rsd4;





// Wma
extern struct
{
uchar header[16];
} wma;



// Video Formats


extern struct
{
unsigned char header[4];
}gxmv;


extern struct
{
unsigned char header[4];
}m1v;


extern struct
{
unsigned char header[4];
}sfd;

//#pragma pack()	

//struct
//{
//	DWORD       format      : 2;	// 0 = Pcm, 1 = Xbadpcm, 2 = Wma
//	DWORD       channels	: 3;	// Channels (1 - 6)
//	DWORD       sampleRate  : 26;	// SamplesPerSec
//	DWORD       bits  		: 1;	// Bits per sample (8 / 16) [PCM Only]
//}xwbMagic;

#endif