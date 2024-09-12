


//===========================================================
//	Wmapack.cpp - Forza Motorsport - (Simple wma container)
//===========================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::WmapackRead(FILE* in){

const int MAXPACKFILENAME = 44;

FileX f;

fseek(in, 8, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 20;

for (int x = 0; x < CItem.Files; ++x, f.pos += 64)
{
	fseek(in, f.pos, 0);
	f.PushFileName(in, MAXPACKFILENAME);
	f.PushOffset(in, 4);
	f.PushChunk(in, 4);
	f.PushExtension(".wma");
}

fmtUtil.WmaSpecs(in, 0);

}

//==============================================================================

