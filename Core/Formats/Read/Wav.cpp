


//================================================
//			Wav.cpp - Multi ()
//================================================



#include <stdafx.h>
#include "Engine.h"
#include "Scx.h"

//==============================================================================

bool Supported::WavRead(FILE* in){

int size = fmtUtil.RiffSize(in, 0);

// Value of 512 = extra crap at end of file: looping, author etc
if (size + 512 < CItem.FileSize)// Scanfile
{
	fclose(in);

	Scx scx;
	if (scx.SilentMode(CItem.OpenFileName) != 0)
		return false;

	in = fopen(CItem.OpenFileName.c_str(), "rb");
}
else// Single wav- Don't scan
{
	CItem.Files = 1;
	FItem.Offset.push_back(0);
	FItem.Chunk.push_back(size);
	FItem.FileName.push_back(CItem.DirName);
	FItem.Ext.push_back(".wav");
}

fmtUtil.RiffSpecs(in, false);

return true;

}

//==============================================================================


