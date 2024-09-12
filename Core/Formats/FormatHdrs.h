#ifndef FORMAT_HDRS_H
#define FORMAT_HDRS_H

//=============================
//		FormatHdrs.h
//=============================

#include <stdafx.h>
#pragma hdrstop

// Container Formats

extern struct
{
	uint	index[4];
	uint	fileNameIndex[4];
	uint   	fileNameIndexEnd;
	uchar	fileName[80];
	uint	fileNameLength[4];
	ulong	padding[4];
}rcf;


extern struct
{
	ulong  	hdrPos;
	ushort	ver[2];
	ulong	padding[4];
	uint	headerSize[4];
	uint   	filenameDir[4];
	uchar	bankType[2];
	uchar	waveEntries[2];
	uchar	loopZeroTail[2];
	uchar	convertSample[2];
	uint 	detailsLength[4];
	uint 	offset1[4];
	bool			x360;
}xwb;


//==============================================================================

// Audio Formats

#ifdef DEBUG
//Microsoft Adpcm
extern struct
{
	uchar 	header1[4];
	ulong 	size;
	uchar 	header2[8];
	uint	sizeofFmtBlock;
	ushort	format;
	ushort	channels;
	ushort	sampleRate;
	uint	bitRate;
	ushort	blockAlign;
	ushort	bits;
	ushort	unknown1; //Error
	ushort	unknown2;
	uint 	unknown3;
	ushort 	unknown4;
	ushort	unknown5;
	ushort	unknown6;
	uint	unknown7;
	uint	unknown8;
	uint	unknown9;
	uint	unknown10;
	ushort	unknown11;
	//uchar 	fact[4];
	//uint	factUnknown;
	//uchar factChunk[4];
	uchar	header3[4];
	ulong	subChunk2Size;
} msadpcm;
#endif



// Xbox Adpcm
extern struct
{
	uchar 	header1[4];
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
} xadpcm;



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
} pcm;



// Rsd4 Pcm
extern struct
{
	uchar	header[7];
	uchar	fullHeader[128];
	ushort	channelsOffset;
	ushort	bitsOffset;
	ushort	sampleRateOffset;
	uchar	pcmPadding[1920]; // Not economical
	ushort	channels[2];
	ushort	bits[2];
	uint	sampleRate[4];
} rsd4;



// Ogg
#pragma pack(1)
extern struct{
	char		id [4];
	uchar		revision; 	// Should be 0
	uchar		bitFlags; 	// 0x01: unset = fresh packet
							// 		   set = continued packet
							// 0x02: unset = not first page of logical bitstream
							//         set = first page of logical bitstream (bos)
							// 0x04: unset = not last page of logical bitstream
							//         set = last page of logical bitstream (eos)
	__int64  	absoluteGranulePos;
	int			streamSerialNumber;
	int			pageSeqNum;
	int			pageChecksum;
	uchar		numPageSegments;
}ogg;
#pragma pack(8)



// Wma
extern struct
{
	uchar header[16];
} wma;


//==============================================================================


// Video Formats

extern struct
{
	uchar header[4];
}gxmv;


extern struct
{
	uchar header[4];
}m1v;


extern struct
{
	uchar header[4];
}sfd;


//==============================================================================
#endif
