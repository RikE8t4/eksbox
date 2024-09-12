


//=============================
//		Rez.cpp - Mojo!
//=============================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::RezRead(FILE* in){

fseek(in, 108514184, 0);   //108514184, 108502184
CItem.Files = 87;

for (int x = 0; x < CItem.Files; ++x)
{
	filex.PushOffset(in, 4);
	filex.PushChunk(in, 4);

	if (FItem.Chunk[x] <= 0)
	{
		FItem.Offset.pop_back(); FItem.Chunk.pop_back();
		--CItem.Files, --x;
	}
	else
		FItem.Chunk[x] -= 90;
	fseek(in, ftell(in) + 16, 0);
}

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	filex.PushChannel(in, 4);
	filex.PushSampleRate(in, 4);

	FItem.Offset[x] += 32;

	if (FItem.Channels[x] == 0)
		FItem.Channels[x] = 2;
	else
		FItem.Channels[x] <<= 1; // Multiply by 2 (shl 1)

	filex.PushAudioFormat("xadpcm");
	filex.PushExtension(".wav");
	filex.PushBits(16);
}   

}

//==============================================================================



