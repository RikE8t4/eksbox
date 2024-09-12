


//==================================================
//	Piz.cpp - Mashed - (Simple non audio container)
//==================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"

//==============================================================================

void Supported::PizRead(FILE* in){

static struct
{
	uchar fileName[116];
}piz;

FileX f;

f.pos = 8;
fseek(in, f.pos, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 2048;
for (int x = 0; x < CItem.Files; ++x, f.pos += 128)
{
	fseek(in, f.pos, 0);
	fread(piz.fileName, 116, 1, in);
	fread(f.offset, 4, 1, in);
	fread(f.chunk, 4, 1, in);
	// Fix slashes so file can be saved
	FItem.FileName.push_back(str.ReplaceAll((char*)piz.fileName, "/", "\\"));
	FItem.Ext.push_back("." + str.ExtractFileExt((char*)piz.fileName));
	FItem.Offset.push_back(*f.offset); 
	FItem.Chunk.push_back(*f.chunk);
	FItem.Format.push_back("");
}

}

//==============================================================================
