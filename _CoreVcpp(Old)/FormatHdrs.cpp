


/******************************
*		FormatHdrs.cpp
******************************/

#include "stdafx.h"



// Container Formats

struct
{
	unsigned int	index[4];
	unsigned int	fileNameIndex[4];
	unsigned int   	fileNameIndexEnd;
	unsigned char	fileName[80];
	unsigned int	fileNameLength[4];
	unsigned long	padding[4];
}rcf;


struct
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
//} msadpcm = {
//"RIFF", 	// Header1
//0, 			// Size
//"WAVEfmt ", // Header2
//50, 		// SizeofFmtBlock
//0x02, 		// Format
//0, 			// Channels
//0, 			// SampleRate
//0, 			// BitRate
//0,			// BlockAlign
//4,			// Bits
//0x20,		// Unknown1  PROBLEM
//0,			// Unknown2
////0x07,		// Unknown3
////256,		// Unknown4
//16777223,
//0,
//512,	// Unknown5
//65280,		// Unknown6
//12582912,	// Unknown7
//15728704,	// Unknown8
//30146560,	// Unknown9
//25755440,	// Unknown10
//65304,	   	// Unknown11    PROBLEM
//
////"fact",	// Fact
////4,		// FactUnknown
////0,		// FactChunk
//
//"data", 	// Header3
//0			// SubChunk2Size
//};






// Xbox Adpcm
//#pragma pack(1)
struct 
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
} xadpcm = {
'R', 'I', 'F', 'F',						// Header1
0, 										// Size
'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', // Header2
20, 									// SizeofFmtBlock
0x69, 									// Format
0, 										// Channels
0, 										// SampleRate
0, 										// BitRate
0,										// BlockAlign
4,										// Bits
4194306,								// Unknown
'd', 'a', 't', 'a', 					// Header3
0										// SubChunk2Size
};
//#pragma pack()



// Pcm
struct
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
} pcm = {
'R', 'I', 'F', 'F',						// Header1
0,										// Size
'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', // Header2
16,										// SizeofFmtBlock
0x01,									// Format
0,										// Channels
0,										// SampleRate
0,										// BitRate
0,										// BlockSize
0,										// Bits
'd', 'a', 't', 'a', 					// Header3
0										// SubChunk2Size
};




// Rsd4 Pcm
struct
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
} rsd4 = {
'R', 'S', 'D', '4', 'P', 'C', 'M',// Header

// FullHeader
0x52, 0x53, 0x44, 0x34, 0x50, 0x43, 0x4D, 0x20, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
0xC0, 0x5D, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,

8,    // ChannelsOffset
12,   // BitsOffset
16    // SampleRateOffset
};





// Wma
struct
{
uchar header[16];
} wma = {
0x30, 0x26,  0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6,
0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C
};



// Video Formats


struct
{
unsigned char header[4];
}gxmv={0x47,0x58,0x4D,0x56};


struct
{
unsigned char header[4];
}m1v={0x00, 0x00, 0x01, 0xB3};


struct
{
unsigned char header[4];
}sfd={0x00, 0x00, 0x01, 0xBA};

//#pragma pack()	

//struct
//{
//	DWORD       format      : 2;	// 0 = Pcm, 1 = Xbadpcm, 2 = Wma
//	DWORD       channels	: 3;	// Channels (1 - 6)
//	DWORD       sampleRate  : 26;	// SamplesPerSec
//	DWORD       bits  		: 1;	// Bits per sample (8 / 16) [PCM Only]
//}xwbMagic;

