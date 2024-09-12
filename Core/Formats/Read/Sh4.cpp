


//=============================================================
//	Sh4.cpp - Silent Hill 4 - (Simple multi-format container)
//=============================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"

//==============================================================================

void Supported::Sh4Read(FILE* in){

static struct{
	int fileNameIndex;
	uchar fileName[16];
}sh4;

fseek(in, 4, 0);
filex.PushFiles(in, 4);

sh4.fileNameIndex = CItem.FileSize - (CItem.Files * 16);
int pos = sh4.fileNameIndex;

// Filename / extension
for (int x = 0; x < CItem.Files; ++x, pos += 16)
{
	fseek(in, pos, 0);
	fgets((char*)sh4.fileName, 16, in);
	FItem.FileName.push_back((char*)sh4.fileName);
	FItem.Ext.push_back("." + str.ExtractFileExt(str.ToLower((char*)sh4.fileName)));
	filex.PushAudioFormat((char*)&FItem.Ext[x]);
}

pos = 16;
for (int x = 0; x < CItem.Files; ++x, pos += 8)
{
	fseek(in, pos, 0);
	filex.PushOffset(in, 4);
	filex.PushChunk(in, 4);
	//fseek(in, pos + 50, 0);
	filex.PushSampleRate(0);
	filex.PushChannel(0);
	filex.PushBits(0);
}

}

//==============================================================================



