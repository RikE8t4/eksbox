

//=============================
//		Engine.cpp
//=============================

#include <stdafx.h>
#pragma hdrstop

#include "Engine.h"
#include "Strings.h"
#include "SystemX.h"
#include "Gui.h"
#include "FormatHdrs.h"

//==============================================================================

ContainerItem CItem;
FileItem FItem;
FormatUtil fmtUtil;
FileX filex;

//==============================================================================

//=============================
//		FormatUtil
//=============================

bool FormatUtil::DetectEaAsf(const string& fileName, const int& offset){

FILE* in = fopen(fileName.c_str(), "rb");

uint	hdrSize;
uchar 	scclhHdr[4];

// Header check
fseek(in, offset + 4, 0);
fread(&hdrSize, 4, 1, in);

fseek(in, offset + hdrSize, 0);
fread(scclhHdr, 4, 1, in);

fclose(in);

if (memcmp(scclhHdr, "SCCl", 4))
	return false;

return true;

}

//==============================================================================

uint FormatUtil::AsfSize(FILE* in, const uint& offset){

// Function not finished!!

uint hdrSize;
//uint size[4];

fseek(in, offset + 4, 0);
fread(&hdrSize, 4, 1, in);

fseek(in, offset + hdrSize + 12, 0);
fread(&hdrSize, 4, 1, in);

//for (int x = 0; x <
// End = SCEl

return 0;

}

//==============================================================================

void FormatUtil::DdsSize(FILE* in, const int& x){

uchar dxtType[4];
int blockSize = 0;
fseek(in, FItem.Offset[x] + 84, 0);
fread(&dxtType, 4, 1, in);

if (dxtType[0] == 'D' &&  dxtType[1] == 'X' &&
	dxtType[2] == 'T')
{
	if (dxtType[3] == '1')
		blockSize = 8;
	else if (dxtType[3] == '2' || dxtType[3] == '3' || dxtType[3] == '4' || dxtType[3] == '5')
		blockSize = 16;

	int height, width;
	fseek(in, FItem.Offset[x] + 12, 0);
	fread(&height, 4, 1, in);
	fread(&width, 4, 1, in);
	FItem.Chunk.push_back((height >> 2) * (width >> 2) * (blockSize) + 128);
}

else
	if (x < CItem.Files -1)
		FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
	else
		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);

}

//==============================================================================

// Based on delphi code by Alexande "Elbereth" Devilliers (Dragon UnPACKer)
void FormatUtil::OggSize(FILE* in, const int& x){

int size = 0;
bool isStart = true;

fseek(in, FItem.Offset[x], 0);

while(1)
{
	fread(&ogg, sizeof(ogg), 1, in);

	if (memcmp(ogg.id, "OggS", 4) || ogg.revision != 0)
		break;
	else if (isStart && !(ogg.bitFlags & 2)
			|| !isStart && (ogg.bitFlags & 2))
		break;

	int oggPageLen = 0;

	for(int j = 0; j < ogg.numPageSegments; ++j)
	{
		uchar tbyte;
		fread(&tbyte, 1, 1, in);
		oggPageLen += tbyte;
	}

	size += oggPageLen + sizeof(ogg) + ogg.numPageSegments;

	if((ogg.bitFlags & 4) == 4)
		break;

	fseek(in, FItem.Offset[x] + size, 0);
	isStart = false;
}

FItem.Chunk.push_back(size);

}

//==============================================================================

void FormatUtil::PngSize(FILE* in, const int& x){

// 4945 4E44 AE42 6082 / IEND.B`.
int off = sys.HeaderScan(FItem.Offset[x] + 80, CItem.FileSize, "IEND", 4, in);

if (off != -1)
	FItem.Chunk.push_back(off - FItem.Offset[x] + 8);
else
{
	if (x < CItem.Files -1)
		FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
	else
		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);
}

}

//==============================================================================

int FormatUtil::RiffHeaderSize(FILE* in, const uint& offset){

uchar fmt[4];
short hdrSize;
uchar fact[4];

fseek(in, offset + 16, 0);
fread(&hdrSize, 2, 1, in);

fseek(in, offset + 20, 0);
fread(fmt, 2, 1, in);

if (*fmt != 0x01)
{
	fseek(in, offset + hdrSize + 20, 0);
	fread(fact, 4, 1, in);
	if ( !memcmp(fact, "fact", 4) )
		hdrSize += 36;
	else
		hdrSize += 24;
}
else
	hdrSize += 24;

return hdrSize + 4;
}

//==============================================================================

uint FormatUtil::RiffSize(FILE* in, const uint& offset){

uchar fmt[4];
uint size[4];
short hdrSize;
uchar fact[4];

fseek(in, offset + 16, 0);
fread(&hdrSize, 2, 1, in);

fseek(in, offset + 20, 0);
fread(fmt, 2, 1, in);

if (*fmt != 0x01)
{
	fseek(in, offset + hdrSize + 20, 0);
	fread(fact, 4, 1, in);
	if ( !memcmp(fact, "fact", 4) )
		hdrSize += 36;
	else
		hdrSize += 24;
}
else
	hdrSize += 24;

fseek(in, offset + hdrSize, 0);
fread(size, 4, 1, in);
return *size + hdrSize + 4;

}

//==============================================================================

uint FormatUtil::WmaSize(FILE* in, const uint& offset){

uchar sig[2048]; // Seh
int off = 0;
int size;

fseek(in, offset, 0);
fread(sig, 2048, 1, in);

for(int x = 0; x < 2048; ++x)
if (sig[x] == 0x53 && sig[x+1] == 0x65 && sig[x+2] == 0x68)
{
	off = x;
	break;
}

if (!off)
	return 0;


fseek(in, offset + off + 26, 0);
fread(&size, 4, 1, in);
return size;

}

//==============================================================================

void FormatUtil::XwbSize(FILE* in, const int&x){

// Get version
USHORT ver[2];
fseek(in, FItem.Offset[x] + 4, 0);
fread(ver, 2, 1, in);
if ( *ver == 0x02 || *ver == 0x03 )
{
	UINT size[4];
	fseek(in, FItem.Offset[x] + 36, 0);
	fread(size, 4, 1, in);
	FItem.Chunk.push_back(*size + 2048);
}
else
	FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);

}

//==============================================================================

// Riff Wav Audio specs
void FormatUtil::RiffSpecs(FILE* in, const bool& getSizes){

uchar fmt[4], bits[2];
short hdrSize;

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	fread(fmt, 4, 1, in);

	if (!strncmp((char*)fmt, "RIFF", 4))
	{
		fseek(in, FItem.Offset[x] + 16, 0);
		fread(&hdrSize, 2, 1, in);

		fseek(in, FItem.Offset[x] + 20, 0);
		fread(fmt, 2, 1, in);

		if (*fmt == 0x69)
			filex.PushAudioFormat("xadpcm");
		else if (*fmt == 0x01)
			filex.PushAudioFormat("pcm");
		else if (*fmt == 0x11)
			filex.PushAudioFormat("imaadpcm");
		else
			filex.PushAudioFormat("unknown");

		filex.PushChannel(in, 2);
		filex.PushSampleRate(in, 4);

		fseek(in, ftell(in) +6, 0);
		fread(bits, 2, 1, in);
		filex.PushBits(*bits);

		if (getSizes)
		{
			FItem.Chunk.push_back(RiffSize(in, FItem.Offset[x]));
		}

	}
	else
	{
		FItem.Format.push_back("");
		FItem.Bits.push_back(NULL);
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
	}

}

}// End RiffSpecs

//==============================================================================

// (Very much built on hacks: needs fixing)
void FormatUtil::WmaSpecs(FILE* in, const bool& getSizes){

uchar hdr[16];

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	fread(hdr, 16, 1, in);

	if (!memcmp(hdr, wma.header, 16))
	{
		if (getSizes)
			FItem.Chunk.push_back(WmaSize(in, FItem.Offset[x]));

		uchar sig[8192];

		fseek(in, FItem.Offset[x], 0);
		fread(sig, 8192, 1, in);

		int pos = 0;

		for(int y = 0; y < 8192; ++y)
			if ( (sig[y] == 0x53 && sig[y+1] == 0x65)
				&& (sig[y+2] == 0x72 || sig[y+2] == 0x73) && sig[y+3] == 0x00 )
			{
				pos = y;
				break;
			}


		fseek(in, FItem.Offset[x] + pos + 66, 0);

		// Channels
		filex.PushChannel(in, 2);

		// SampleRate
		filex.PushSampleRate(in, 4);

		// Bits
		uint bits[2];
		fseek(in, ftell(in) + 6, 0);
		fread(bits, 2, 1, in);
		filex.PushBits(*bits);

		filex.PushAudioFormat("wma");
	}
	else
	{
		FItem.Format.push_back(NULL);
		FItem.Bits.push_back(NULL);
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
	}

}

}// End WmaSpecs

//==============================================================================

void FormatUtil::WritePcmHeader(const ulong& fileSize,
const short& channels, const short& bits, const uint& sampleRate, FILE* out){

pcm.size = fileSize + 40;
pcm.channels = channels;
pcm.sampleRate = sampleRate;
pcm.bitRate = (sampleRate * channels * bits) >> 3; // >> 3 equivalent to / 8
pcm.blockSize = channels * (bits >> 3);
pcm.bits = bits;
pcm.subChunk2Size = fileSize;
fwrite(&pcm, sizeof (pcm), 1, out);

}

//==============================================================================

#ifdef DEBUG
void FormatUtil::WriteMsadpcmHeader(const ulong& fileSize,
const short& channels, const uint& sampleRate, FILE* out){

msadpcm.size = fileSize + 40;
msadpcm.channels = channels;
msadpcm.sampleRate = sampleRate;

if (sampleRate == 8000)
{
	msadpcm.bitRate = 4096 * channels;
	msadpcm.blockAlign = 256 * channels;
	msadpcm.unknown2 = 500;
}
else if (sampleRate == 11025)
{
	msadpcm.bitRate = 5644 * channels;
	msadpcm.blockAlign = 256 * channels;
	msadpcm.unknown2 = 500;
}
else if (sampleRate == 22050)
{
	msadpcm.bitRate = 11155 * channels;
	msadpcm.blockAlign = 512 * channels;
	msadpcm.unknown2 = 1012;
}
else if (sampleRate == 44100)
{
	msadpcm.bitRate = 22179 * channels;
	msadpcm.blockAlign = 1024 * channels;
	msadpcm.unknown2 = 2036;
}


msadpcm.subChunk2Size = fileSize;
fwrite(&msadpcm, sizeof(msadpcm), 1, out);

}
#endif

//==============================================================================

void FormatUtil::WriteXadpcmHeader(const ulong &fileSize,
const short& channels, const uint& sampleRate, FILE* out){

xadpcm.size = fileSize + 40;
xadpcm.channels = channels;
xadpcm.sampleRate = sampleRate;
xadpcm.blockAlign = channels * 36;
xadpcm.bitRate = (sampleRate * xadpcm.blockAlign) >> 6; // >> 6 equivalent to / 64
xadpcm.subChunk2Size = fileSize;
fwrite(&xadpcm, sizeof(xadpcm), 1, out);

}

//==============================================================================

//=============================
//		FileX
//=============================


void FileX::PushFiles(const int &files){

CItem.Files = files;

}

//==============================================================================

void FileX::PushFiles(FILE* in, const int& size){

fread(&CItem.Files, size, 1, in);

}

//==============================================================================

void FileX::PushOffset(FILE* in, const ushort& size){

static ulong offset[4];
fread((char*)&offset, size, 1, in);
FItem.Offset.push_back(*offset);

}

//==============================================================================

void FileX::PushOffset(const ulong& offset){

FItem.Offset.push_back(offset);

}

//==============================================================================

void FileX::PushChunk(FILE* in, const ushort& size){

static ulong chunk[4];
fread((char*)&chunk, size, 1, in);
FItem.Chunk.push_back(*chunk);

}

//==============================================================================

void FileX::PushChunk(const ulong& chunk){

FItem.Chunk.push_back(chunk);

}

//==============================================================================

void FileX::PushChunkDefault(){

FItem.Chunk.push_back(CItem.FileSize);

}

//==============================================================================

void FileX::PushFileName(FILE* in, const ushort& size){

static char name[100];
fread(&name, size, 1, in);
string((char*)name).resize(size);
FItem.FileName.push_back(name);

}

//==============================================================================

void FileX::PushFileName(const string& name){

FItem.FileName.push_back(name);

}

//==============================================================================

void FileX::PushFileNameDefault(){

FItem.FileName.push_back(str.ExtractFileName(CItem.OpenFileName));

}


//==============================================================================

void FileX::PushExtension(const string& ext){

FItem.Ext.push_back(ext);

}

//==============================================================================

void FileX::PushAudioFormat(const char* audioFmt){

FItem.Format.push_back((char*)audioFmt);

}

//==============================================================================

void FileX::PushBits(const short& bits){

FItem.Bits.push_back(bits);

}

//==============================================================================

void FileX::PushChannel(FILE* in, const uint& size){

static uint channels[4];
fread(&channels, size, 1, in);
FItem.Channels.push_back(*channels);

}

//==============================================================================

void FileX::PushChannel(const short& channel){

FItem.Channels.push_back(channel);

}

//==============================================================================

void FileX::PushSampleRate(const ushort& sampleRate){

FItem.SampleRate.push_back(sampleRate);

}

//==============================================================================

void FileX::PushSampleRate(FILE* in, const uint& size){

static uint sampleRate[4];
fread(&sampleRate, size, 1, in);
FItem.SampleRate.push_back(*sampleRate);

}

//==============================================================================

void FileX::ClearVariables(){

CItem.Finished = false;
CItem.Loading = false;
CItem.ScxMode = false;
CItem.Extracting = false;
CItem.IniMatch = false;
CItem.HasNames = false;
CItem.HasAudioInfo = false;
CItem.HasFolders = false;
CItem.NeedsWavHeader = false;
CItem.Error = false;
CItem.Files = 0;
//CItem.IncompatileFmts = 0;
CItem.FileSize = 0;
CItem.DirName = "";
//CItem.DirPath = "";
//CItem.OpenFileName = "";
CItem.Format = "";

FItem.Offset.clear();
FItem.OffsetXtra.clear();
FItem.Chunk.clear();
FItem.ChunkXtra.clear();
FItem.Chunk1st.clear();
FItem.Parts.clear();
FItem.SampleRate.clear();
FItem.Channels.clear();
FItem.Bits.clear();
FItem.Format.clear();
FItem.Ext.clear();
FItem.FileName.clear();
FItem.FileNameBak.clear();

}

//==============================================================================
