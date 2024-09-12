


//===========================================================
//	Pod.cpp - BloodRayne 2 - (Simple multi-format container)
//===========================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"

//==============================================================================

void Supported::PodRead(FILE* in){

FileX f;

static struct
{
	uint 	index[4];
	uint 	fileNameIndexSize[4];
	uchar	fileName[70];
}pod;


// Num of files
fseek(in, 88, 0);
fread(&CItem.Files, 4, 1, in);

// Index
fseek(in, 264, 0);
fread(pod.index, 4, 1, in);

// FileName index size
fseek(in, 272, 0);
fread(pod.fileNameIndexSize, 4, 1, in);


// Offsets / sizes
f.pos = *pod.index + 4;
for (int x = 0; x < CItem.Files; ++x, f.pos += 20)
{                              
	fseek(in, f.pos, 0);
	f.PushChunk(in, 4);
	f.PushOffset(in, 4);
}

// FileNames
f.pos = CItem.FileSize - *pod.fileNameIndexSize;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, f.pos, 0);
	fgets((char*)pod.fileName, sizeof(pod.fileName), in);

	f.pos += (long)strlen((char*)pod.fileName) + 1;
	FItem.FileName.push_back((char*)pod.fileName);
	FItem.Ext.push_back(str.ToLower("." + str.ExtractFileExt((char*)pod.fileName)));
}

fmtUtil.RiffSpecs(in, false);

}

//==============================================================================
