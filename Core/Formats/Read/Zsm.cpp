


//==================================================
//	Zsm.cpp - Multi - (Audio container)
//==================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::ZsmRead(FILE* in){

const int ZSMFILENAMELEN = 64;
FileX f;

static struct
{
	uchar	fileName[ZSMFILENAMELEN];
	uint	totalIndexSize;
	uint	specsIndex;
	uint	filenameIndex;
}zsm;

//fseek(in, 12, 0);
//fread(&zsm.totalIndexSize, 4, 1, in);

fseek(in, 16, 0);
fread(&CItem.Files, 4, 1, in);

fseek(in, 36, 0);
fread(&zsm.specsIndex, 4, 1, in);

fseek(in, 48, 0);
fread(&zsm.filenameIndex, 4, 1, in);

f.pos = zsm.specsIndex + 4;
for (int x = 0; x < CItem.Files; ++x, f.pos += 28)
{
	fseek(in, f.pos, 0);
	f.PushSampleRate(in, 4);
	FItem.Channels.push_back(1);
	f.PushBits(4);
}

f.pos = zsm.filenameIndex;
for (int x = 0; x < CItem.Files; ++x, f.pos += 84)
{
	fseek(in, f.pos, 0);
	f.PushOffset(in, 4);
	f.PushChunk(in, 4);

	uint type;
	fread(&type, 2, 1, in);

//	if (type == 1)
//	{
		fseek(in, f.pos + 20, 0);
		f.PushAudioFormat("xadpcm");
//	}
//	else if (type == 106)
//	{
//		fseek(in, f.pos + 12, 0);
//		f.PushAudioFormat("msadpcm");
//	}

	fread(zsm.fileName, ZSMFILENAMELEN, 1, in);
	f.PushFileName((char*)zsm.fileName);
	f.PushExtension(".wav");
}

}

//==============================================================================

