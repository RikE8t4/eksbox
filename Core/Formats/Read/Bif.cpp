


//===========================================================
//	Bif.cpp - Star Wars: KOTOR - (Simple container)
//===========================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::BifRead(FILE* in){

FileX f;

fseek(in, 8, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 24;

for (int x = 0; x < CItem.Files; ++x, f.pos += 16)
{
	fseek(in, f.pos, 0);
	f.PushOffset(in, 4);
	f.PushChunk(in, 4);

	// Check format
	memset(f.fmt, '\0', 4);
	fseek(in, FItem.Offset[x], 0);
	fread(f.fmt, 4, 1, in);

	if (f.fmt[0] == 'R' && f.fmt[1] == 'I' && f.fmt[2] == 'F' && f.fmt[3] == 'F')
		f.PushExtension(".wav");
	else
		f.PushExtension(".dat");
}            

fmtUtil.RiffSpecs(in, false);

}

//==============================================================================


