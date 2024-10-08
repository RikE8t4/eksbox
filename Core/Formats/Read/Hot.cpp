


//====================================================
//	Hot.cpp - Voodoo Vince - (Messy audio container)
//====================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::HotRead(FILE* in){

const int MAXHOTFILENAMELEN = 18;
FileX f;

static struct{
	int 	specsIndex;
	int 	filenameIndex;
	int 	firstOffset;
	uchar 	filename[MAXHOTFILENAMELEN];
	int 	filenameOffset;
	short	type;
}hot;


fseek(in, 8, 0);
fread(&hot.specsIndex, 4, 1, in);
fread(&hot.firstOffset, 4, 1, in);

//fseek(in, 32, 0);
//fread(&hot.type, 2, 1, in);

fseek(in, 20, 0);
fread(&hot.filenameIndex, 4, 1, in);
fread(&CItem.Files, 4, 1, in);


// Offsets
f.pos = 48;
for (int x = 0; x < CItem.Files; ++x, f.pos += 24)
{
	fseek(in, f.pos, 0);
	f.PushOffset(in, 4);
}


// Filenames

// First one
fseek(in, hot.filenameIndex, 0);
fgets((char*)hot.filename, MAXHOTFILENAMELEN, in);
f.PushFileName((char*)hot.filename);

// Rest
f.pos = 52;
for (int x = 0; x < CItem.Files -1; ++x, f.pos += 24)
{
	*hot.filename = 0x00;
	fseek(in, f.pos, 0);
	fread(&hot.filenameOffset, 4, 1, in);

	fseek(in, hot.filenameIndex + hot.filenameOffset, 0);
	fgets((char*)hot.filename, MAXHOTFILENAMELEN, in);
	f.PushFileName((char*)hot.filename);
}


// Audio specs
//if (hot.type == 0x72)
//{
f.pos = hot.specsIndex;
for (int x = 0; x < CItem.Files; ++x, f.pos += 72)
{

	fseek(in, f.pos + 68, 0);
	fread(f.chunk, 4, 1, in);
	FItem.Chunk.push_back(*f.chunk + 32 + 14);

	fseek(in, f.pos + 20, 0);
	fread(f.fmt, 2, 1, in);

	if (*f.fmt == 0x69)
		f.PushAudioFormat("xadpcm");
	else if (*f.fmt == 0x01)
		f.PushAudioFormat("pcm");

	f.PushChannel(in, 2);
	f.PushSampleRate(in, 4);

	fseek(in, ftell(in) +6, 0);
	fread(f.bits, 2, 1, in);
	f.PushBits(*f.bits);

	f.PushExtension(".wav");
}

//}
//else if (hot.type == 0x00)
//{
//
//for (int x = 0; x < Files; ++x)
//{
//Chunk.push_back(Offset[x+1] - Offset[x]);
//AudioFormat.push_back(NULL);
//Bits.push_back(NULL);
//Channels.push_back(NULL);
//SampleRate.push_back(NULL);
//filex.PushExtension(ExtractFileExt(FileName[x]));
//}
//Chunk.pop_back();
//Chunk.push_back(FileSize - Offset[Files - 1]);
//
//}


}

//==============================================================================


