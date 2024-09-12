


//==================================================
//	Stx.cpp - Kakuto Chojin - (Non-Std Xbadpcm)
//==================================================

// TODO: Create a function in StxConvert to calc converted filesize

#include <stdafx.h>
#include "Engine.h"
#include "../Convert/ConvertStx.h"

//==============================================================================

void Supported::StxRead(FILE* in){

fseek(in, 0, 0);
fread(&stx, sizeof(stx), 1, in);

if (stx.channels == 4)
	CItem.Files = 2;
else
	CItem.Files = 1;

// Calc raw xbadpcm converted filesize (This is not done exactly yet)
uint convSize = stx.pages * 2048 - (stx.pages * 32);

//==============================================================================

if (stx.loopStart != 0xFFFF) // Calc filesize for looped stx's
{
	// Amount of data contained in page @ loop point
	ushort dataLoopStart;
	ushort dataLoopEnd;

	// Loop start datasize
	fseek(in, (stx.loopStart * 2048) - 2048 + 22, 0);
	fread(&dataLoopStart, sizeof(ushort), 1, in);

	// Loop end datasize
	fseek(in, (stx.loopEnd * 2048) - 2048 + 22, 0);
	fread(&dataLoopEnd, sizeof(ushort), 1, in);

	// Minus the blank data at the end of the loop points
	// to get exact raw xbadpcm converted filesize
	convSize -= 4096;

	ushort pageHdrSize = 32;
	convSize += ((dataLoopStart<<1) + (dataLoopEnd<<1) + (pageHdrSize<<1));

}

//==============================================================================

for (int x = 0; x < CItem.Files; ++x)
{
	if (stx.channels == 4 && x == 0)
	{
		FItem.Offset.push_back(2048);
		FItem.Chunk.push_back(convSize >> 1);
		FItem.FileName.push_back(CItem.DirName + " (1)");
	}
	else if (stx.channels == 4 && x == 1)
	{
		FItem.Offset.push_back(3056);
		FItem.Chunk.push_back(convSize >> 1);
		FItem.FileName.push_back(CItem.DirName + " (2)");
	}
	else
	{
		FItem.Offset.push_back(2048);
		FItem.Chunk.push_back(convSize);
		FItem.FileName.push_back(CItem.DirName);
	}

	fseek(in, 6, 0);
	fread(&stx.channels, 2, 1, in);

	if (stx.channels == 4)
		FItem.Channels.push_back(2);
	else
		FItem.Channels.push_back(stx.channels);

	fseek(in, 32, 0);
	filex.PushSampleRate(in, 4);
	filex.PushAudioFormat("xadpcm");
	filex.PushExtension(".wav");
	filex.PushBits(4);

} // End for loop

}

//==============================================================================




