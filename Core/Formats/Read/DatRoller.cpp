


//================================================
//	DatRoller.cpp - RollerCoaster Tycoon (-)
//================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::DatRollerRead(FILE* in){

fseek(in, 0, SEEK_SET);
fread(&CItem.Files, 4, 1, in);

for (int x = 0; x < CItem.Files; ++x)
	filex.PushOffset(in, 4);

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	filex.PushChunk(in, 4);

	fseek(in, FItem.Offset[x] + 6, 0);
	filex.PushChannel(in, 2);

	filex.PushSampleRate(in, 4);

	uchar bits;
	fseek(in, FItem.Offset[x] + 18, 0);
	fread(&bits, 1, 1, in);
	FItem.Bits.push_back(bits);

	if (bits == 16)
		filex.PushAudioFormat("pcm");
	else if (bits == 4)
		filex.PushAudioFormat("xadpcm");
	
	FItem.Ext.push_back(".wav");
	FItem.Offset[x] += 22;
}

}

//==============================================================================


