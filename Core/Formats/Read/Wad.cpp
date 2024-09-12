


//======================================================================
//	Wad.cpp - Disney's Extreme Skate Adventure - (Simple Wma container)
//======================================================================



#include <stdafx.h>
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Strings.h"

//==============================================================================

bool Supported::WadRead(){

FileX f;

FILE* in = fopen(str.ChangeFileExt(CItem.OpenFileName, ".dat").c_str(), "rb");

if (!in)
{
	gui.Error("Couldn't Find " + CItem.DirName + ".dat", sys.Delay(false), NULL);
	return false;
}

fread(&CItem.Files, 4, 1, in);

f.pos = 8;
for (int x = 0; x < CItem.Files; ++x, f.pos += 12)
{
	fseek(in, f.pos, 0);
	fread(f.offset, 4, 1, in);
	fread(f.chunk, 4, 1, in);
	FItem.Offset.push_back(*f.offset);
	FItem.Chunk.push_back(*f.chunk);
	FItem.Ext.push_back(".wma");
}

// Reset
fclose(in);
in = fopen(CItem.OpenFileName.c_str(), "rb");
if (!in)
{
	gui.Error("Couldn't Find " + CItem.DirName + ".wad", 1500, NULL);
	return false;
}

fmtUtil.WmaSpecs(in, false);

return true;

}

//==============================================================================


