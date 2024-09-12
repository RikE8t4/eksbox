


//=======================================================
//	Msx.cpp - MK Deception - (Simple audio container)
//=======================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::MsxRead(FILE* in){

FileX f;

static struct{
	ushort	headerSize[4];
}msx;

fseek(in, 4, 0);
fread(&CItem.Files, 4, 1, in);
--CItem.Files;

fread(msx.headerSize, 4, 1, in);

// Offsets / chunks
f.pos = 56;
for (int x = 0; x < CItem.Files; ++x, f.pos += 24)
{
	fseek(in, f.pos, 0);
	f.PushOffset(in, 4);
	f.PushChunk(in, 4);
}

// Set offset 1
FItem.Offset[0] = *msx.headerSize;

// Audio specs
f.pos = ftell(in) + 12 + 20;
for (int x = 0; x < CItem.Files; ++x, f.pos += 40)
{
	fseek(in, f.pos, 0);
	fread(f.fmt, 2, 1, in);
	if (*f.fmt == 0x69)
		f.PushAudioFormat("xadpcm");
	else
		f.PushAudioFormat("pcm");

	f.PushChannel(in, 2);
	f.PushSampleRate(in, 4);

	fseek(in, ftell(in) +6, 0);
	fread(f.bits, 2, 1, in);
	f.PushBits(*f.bits);
	f.PushExtension(".wav");
}

}

//==============================================================================

