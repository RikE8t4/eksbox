

// Total mess - needs to be cleaned up
//==================================
//	Rfa.cpp - RalliSport Challenge
//==================================



#include <stdafx.h>
#include "Engine.h"
#include "FormatHdrs.h"
#include "Strings.h"

//==============================================================================

void Supported::RfaRead(FILE* in){

static struct {
	ulong	index[4];
	uchar 	name[64];
	ushort	nameLen[4];
}rfa;

FileX file;

fseek(in, 28, 0);
fread(rfa.index, 4, 1, in);
fseek(in, *rfa.index, 0);
fread(&CItem.Files, 4, 1, in);

for (int x = 0; x < CItem.Files; ++x)
{
	fread(rfa.nameLen, 4, 1, in);
	memset(rfa.name, '\0', 64);
	fread(rfa.name, *rfa.nameLen, 1, in);
	// Fix slashes so file can be saved
	FItem.FileName.push_back(str.ReplaceAll((char*)rfa.name, "/", "\\"));
	FItem.Ext.push_back("." + str.ExtractFileExt(str.ToLower((char*)rfa.name)));

	fread(file.chunk, 4, 1, in);
	fseek(in, ftell(in) + 4, 0);
	fread(file.offset, 4, 1, in);
	fseek(in, ftell(in) + 12, 0);

	FItem.Offset.push_back(*file.offset);
	FItem.Chunk.push_back(*file.chunk);
}

for (int x = 0; x < CItem.Files; ++x)
{
	FItem.Format.push_back("");
	FItem.Channels.push_back(0);
	FItem.SampleRate.push_back(0);
	FItem.Bits.push_back(0);
	if (FItem.Ext[x] == ".ike")
	{
		fseek(in, FItem.Offset[x], 0);

		uchar riff[4];
		for (int i = 0; i < 600 ; ++i)
		{
			fseek(in, ftell(in)-3, 0);
			fread(riff, 4, 1, in);
			if (*riff == *pcm.header1)
			{
				FItem.Offset[x] = ftell(in)-4;
				break;
			}

		}

		fseek(in, FItem.Offset[x] + 20, 0);
		fread(file.fmt, 2, 1, in);

		if (*file.fmt == 105)
		{
			FItem.Format[x] = "xadpcm";
			FItem.Bits[x] = 4;
		}
		else
		{
			//FItem.Ext[x] = ".wav";
			continue;
		}

		fseek(in, FItem.Offset[x] + 22, 0);
		fread(file.chan, 2, 1, in);
		fread(file.sr, 4, 1, in);

		FItem.Channels[x] = *file.chan; FItem.SampleRate[x] = *file.sr;

		FItem.Ext[x] = ".wav";
		//Offset[x] += 48;
		//Chunk[x] -= 48;

	}
} // End loop

}

//==============================================================================

