


//=======================================
//	BigCode.cpp - Colin McRae Rally 2005
//=======================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::BigCodeRead(FILE* in){

Supported f;

static struct
{
	uchar	fileName[16];
}big;


fseek(in, 4, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 52;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, f.pos, 0);
	fread(f.chunk, 4, 1, in);
	fread(f.offset, 4, 1, in);
	FItem.Offset.push_back(*f.offset);
	FItem.Chunk.push_back(*f.chunk);
	FItem.Offset[x] += 2048;
	f.pos += 24;
}

sys.RiffSpecs(in, false);

// Filenames
f.pos = 36;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, f.pos, 0);
	fread(big.fileName, 16, 1, in);
	f.pos += 24;
	FItem.FileName.push_back((char*)big.fileName);
	//FItem.FileName[x] = FItem.FileName[x].Tidy();
	FItem.Ext.push_back("." + ExtractFileExt(FItem.FileName[x]));
}

}

