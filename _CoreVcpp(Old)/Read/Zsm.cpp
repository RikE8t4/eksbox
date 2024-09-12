


//==================================================
//	Zsm.cpp - Multi - (Audio container)
//==================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::ZsmRead(FILE* in){


#define ZSMFILENAMELEN 64
Supported file;

static struct
{
	uchar	fileName[ZSMFILENAMELEN];
	uint	totalIndexSize;
	uint	specsIndex;
	uint	filenameIndex;
}zsm;


//fseek(in, 12, 0);
//fread(&zsm.totalIndexSize, 4, 1, in);

fseek(in, 16, 0);
fread(&CItem.Files, 4, 1, in);

fseek(in, 36, 0);
fread(&zsm.specsIndex, 4, 1, in);

fseek(in, 48, 0);
fread(&zsm.filenameIndex, 4, 1, in);

file.pos = zsm.specsIndex + 4;
for (int x = 0; x < CItem.Files; ++x, file.pos += 28)
{
	fseek(in, file.pos, 0);
	sys.PushSampleRate(in, 4);
	FItem.Channels.push_back(1);
	sys.PushBits(4);
}

file.pos = zsm.filenameIndex;
for (int x = 0; x < CItem.Files; ++x, file.pos += 84)
{
	fseek(in, file.pos, 0);
	sys.PushOffset(in, 4);
	sys.PushChunk(in, 4);

	uint type;
	fread(&type, 2, 1, in);

//	if (type == 1)
//	{
		fseek(in, file.pos + 20, 0);
		sys.PushAudioFormat("xadpcm");
//	}
//	#ifdef DEBUG
//	else if (type == 106)
//	{
//		fseek(in, file.pos + 12, 0);
//		sys.PushAudioFormat("msadpcm");
//	}
//	#endif


	fread(zsm.fileName, ZSMFILENAMELEN, 1, in);
	FItem.FileName.push_back((char*)zsm.fileName);
	FItem.Ext.push_back(".wav");
}


}

