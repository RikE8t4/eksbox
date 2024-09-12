


//=============================================================
//	Mpk.cpp - Mission Impossible - (Simple audio container)
//=============================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"

//==============================================================================

void Supported::MpkRead(FILE* in){

FileX f;

static struct
{
	uchar	fileName[100];
	uint	fileNameIndex[4];
	uint	fileNameIndexLen[4];
}mpk;


// Num of files
fseek(in, 8, 0);
fread(&CItem.Files, 4, 1, in);

// FileName index length
fseek(in, 12, 0);
fread(mpk.fileNameIndexLen, 4, 1, in);

// FileName index
fseek(in, 24, 0);
fread(mpk.fileNameIndex, 4, 1, in);
*mpk.fileNameIndex += 16;


// Offsets / sizes
f.pos = 16;
for (int x = 0; x < CItem.Files; ++x, f.pos += 12)
{
	fseek(in, f.pos, 0);
	f.PushOffset(in, 4);
	f.PushChunk(in, 4);
}
	 
// FileNames
f.pos = *mpk.fileNameIndex;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, f.pos, 0);
	fgets((char*)mpk.fileName, sizeof(mpk.fileName), in);

	// Fix slashes so file can be saved
	f.PushFileName(str.ReplaceAll((char*)mpk.fileName, "/", "\\"));
	f.PushExtension("." + str.ToLower(str.ExtractFileExt((char*)mpk.fileName)));
	
	f.pos += (int)strlen((char*)mpk.fileName) + 1;
}

fmtUtil.RiffSpecs(in, false);

}

//==============================================================================


