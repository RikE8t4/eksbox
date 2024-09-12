


//=====================================================
//	Wpx.cpp - Capcom Vs Snk 2 EO - (Audio container)
//=====================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::WpxRead(FILE* in){

const int HEADERSIZE = 52;

FileX f;

fseek(in, 0, 0);
fread(&CItem.Files, 4, 1, in);

for (int x = 0; x < CItem.Files; ++x)
	f.PushOffset(in, 4);

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] +22, 0);
	f.PushChannel(in, 2);

	fseek(in, FItem.Offset[x] +24, 0);
	f.PushSampleRate(in, 4);

	fseek(in, FItem.Offset[x] +34, 0);
	fread(&f.bits, 2, 1, in);

	if (*f.bits == 16)
	{
		f.PushAudioFormat("pcm");
		f.PushBits(*f.bits);
		FItem.Ext.push_back(".wav");
	}
	else if (*f.bits == 4)
	{
		f.PushAudioFormat("xadpcm");
		f.PushBits(*f.bits);
		FItem.Ext.push_back(".wav");
	}
	else
	{
		f.PushAudioFormat("");
		f.PushBits(0);
		FItem.Ext.push_back(".unknown");
	}

	//fseek(in, FItem.Offset[x] +48, 0);
	fseek(in, FItem.Offset[x] +4, 0);
	f.PushChunk(in, 4);
	FItem.Chunk[x] -= 44;

	// Move offset to point to start of audio- straight after header
	FItem.Offset[x] += HEADERSIZE;
	FItem.Ext.push_back(".wav");
}

}

//==============================================================================

