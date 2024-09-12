


//=============================
//		Arc.cpp - Sims 2
//=============================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::ArcRead(FILE* in){

Supported f;

static struct
{
	uint	index[4];
	uchar	fileName[70];
}arc;


fseek(in, 0, 0);
fread((char*)arc.index, 4, 1, in);

fseek(in, *arc.index, 0);
fread(&CItem.Files, 4, 1, in);

// Offsets / chunks / names
f.pos = ftell(in) + 4;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, f.pos, 0);
	fread(f.offset, 4, 1, in);
	fread(f.chunk, 4, 1, in);
	FItem.Offset.push_back(*f.offset); FItem.Chunk.push_back(*f.chunk);

	f.pos = ftell(in) + 1;
	fgets((char*)arc.fileName, sizeof(arc.fileName), in);
	FItem.FileName.push_back((char*)arc.fileName);     //msg.info(FileName[x]);
	f.pos += (long)strlen((char*)arc.fileName) + 12;
}

// Audio specs
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] + 4, 0);
	fread(f.sr, 4, 1, in);
	fread(f.fmt, 2, 1, in);
	if (*f.fmt == 105)
	{
		FItem.Offset[x] += 32;
		FItem.Chunk[x] -= 32;
		FItem.SampleRate.push_back(*f.sr);
		FItem.Format.push_back("xadpcm");
		FItem.Bits.push_back(4);
		FItem.Ext.push_back(".wav");
		fread(f.chan, 2, 1, in);
		FItem.Channels.push_back(*f.chan);
		//Channels[x]=2;
	}
	else
	{
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
		FItem.Bits.push_back(NULL);
		FItem.Format.push_back("");
		FItem.Ext.push_back("");
	}
}

}


