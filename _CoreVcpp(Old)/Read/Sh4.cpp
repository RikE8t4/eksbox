


//=============================================================
//	Sh4.cpp - Silent Hill 4 - (Simple multi-format container)
//=============================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::Sh4Read(FILE* in){

static struct{
	int fileNameIndex;
	uchar fileName[16];
}sh4;

fseek(in, 4, 0);
sys.PushFiles(in, 4);

sh4.fileNameIndex = CItem.FileSize - (CItem.Files * 16);
int pos = sh4.fileNameIndex;

// Filename / extension
for (int x = 0; x < CItem.Files; ++x, pos += 16)
{
	fseek(in, pos, 0);
	fgets((char*)sh4.fileName, 16, in);
	FItem.FileName.push_back((char*)sh4.fileName);
	FItem.Ext.push_back("." + ExtractFileExt(ToLower((char*)sh4.fileName)));
	sys.PushAudioFormat((char*)&FItem.Ext[x]);
}

pos = 16;
for (int x = 0; x < CItem.Files; ++x, pos += 8)
{
	fseek(in, pos, 0);
	sys.PushOffset(in, 4);
	sys.PushChunk(in, 4);
	//fseek(in, pos + 50, 0);
	sys.PushSampleRate(0);
	sys.PushChannel(0);
	sys.PushBits(0);
}

}



