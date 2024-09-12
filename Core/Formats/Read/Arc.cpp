


//=============================
//		Arc.cpp - Sims 2
//=============================



#include <stdafx.h>
#include "Engine.h"
#include "Gui.h"

//==============================================================================

void Supported::ArcRead(FILE* in){

static struct
{
	uint	index[4];
	uchar	fileName[70];
}arc;


fseek(in, 0, 0);
fread((char*)arc.index, 4, 1, in);

fseek(in, *arc.index, 0);
fread(&CItem.Files, 4, 1, in);

long pos, offset[4], chunk[4];

// Offsets / chunks / names
pos = ftell(in) + 4;
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, pos, 0);
	fread(offset, 4, 1, in);
	fread(chunk, 4, 1, in);

	filex.PushOffset(*offset);
	filex.PushChunk(*chunk);

	pos = ftell(in) + 1;
	fgets((char*)arc.fileName, sizeof(arc.fileName), in);
	filex.PushFileName((char*)arc.fileName);
	pos += (long)strlen((char*)arc.fileName) + 12;
}


ushort sr[4];
uchar fmt[4];
uchar chan[2];

// Audio specs
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] + 4, 0);
	fread(sr, 4, 1, in);
	fread(fmt, 2, 1, in);
	if (*fmt == 105)
	{
		FItem.Offset[x] += 32;
		FItem.Chunk[x] -= 32;
		FItem.SampleRate.push_back(*sr);
		FItem.Format.push_back("xadpcm");
		FItem.Bits.push_back(4);
		FItem.Ext.push_back(".wav");
		fread(chan, 2, 1, in);
		FItem.Channels.push_back(*chan);
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

//==============================================================================


