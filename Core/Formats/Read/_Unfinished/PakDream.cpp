


//============================================================================
//	PakDream.cpp - Dreamfall: The Longest Journey - (Multi format container)
//============================================================================

// Unfinished!!

#ifdef DEMO_ONLY

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"

//==============================================================================

void Supported::PakDreamRead(FILE* in){

FileX f;

fseek(in, 12, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 24;
for(int x = 0; x < CItem.Files; ++x, f.pos += 20)
{
	fseek(in, f.pos, 0);
	fread(f.offset, 4, 1, in);
	fread(f.chunk, 4, 1, in);
	//if (*f.chunk && *f.offset)
	//{
		FItem.Offset.push_back(*f.offset);
		FItem.Chunk.push_back(*f.chunk);
	//}
	//else
	//--Files;
}

for(int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	fread(f.fmt, 4, 1, in);

	if ( !memcmp(f.fmt, "RIFF", 4) )
	{
		fseek(in, FItem.Offset[x] + 20, 0);
		fread(f.fmt, 2, 1, in);

		if (*f.fmt == 0x69) FItem.Format.push_back("xadpcm");
		else if (*f.fmt == 0x01) FItem.Format.push_back("pcm");

		fread(f.chan, 2, 1, in); FItem.Channels.push_back(*f.chan);
		fread(f.sr, 4, 1, in); FItem.SampleRate.push_back(*f.sr);

		fseek(in, ftell(in) +6, 0);
		fread(f.bits, 2, 1, in); FItem.Bits.push_back(*f.bits);

		FItem.Ext.push_back(".wav");
	}
	else
	{
		if ( !memcmp(f.fmt, "ID3", 3) )
		{
			FItem.Format.push_back("mp3");
			FItem.Ext.push_back(".mp3");
		}
		else
		{
			FItem.Format.push_back("");
			FItem.Ext.push_back("");
		}
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
		FItem.Bits.push_back(NULL);
	}
}

}

//==============================================================================

#endif



