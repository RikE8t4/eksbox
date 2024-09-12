


//====================================================================
//  Gta5.cpp - GTA: San Andreas - (Simple Bulk Format Ogg container)
//====================================================================



#include <stdafx.h>
#include "Engine.h"
#include "Scx.h"

//==============================================================================

bool Supported::Gta5Read(){

Scx scx;
if ( scx.SilentMode(CItem.OpenFileName) == -1 )
{
	return false;
}

FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");
CItem.HasAudioInfo = 1;

for (int x = 0; x < CItem.Files; ++x)
{
	filex.PushAudioFormat("ogg");

	fseek(in, FItem.Offset[x] + 40, 0);
	filex.PushSampleRate(in, 4);
	filex.PushBits(16);
	filex.PushChannel(2);
}

return true;

}

//==============================================================================


