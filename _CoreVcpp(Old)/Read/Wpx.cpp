


//==================================================
//	Wpx.cpp - Capcom Vs Snk 2 EO - (Audio container)
//==================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::WpxRead(FILE* in){

#define HEADERSIZE 52

Supported file;

fseek(in, 0, 0);
fread(&CItem.Files, 4, 1, in);

for (int x = 0; x < CItem.Files; ++x)
	sys.PushOffset(in, 4);

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] +22, 0);
	sys.PushChannel(in, 2);

	fseek(in, FItem.Offset[x] +24, 0);
	sys.PushSampleRate(in, 4);

	fseek(in, FItem.Offset[x] +34, 0);
	fread(&file.bits, 2, 1, in);

	if (*file.bits == 16)
	{
		sys.PushAudioFormat("pcm");
		sys.PushBits(*file.bits);
		FItem.Ext.push_back(".wav");
	}
	else if (*file.bits == 4)
	{
		sys.PushAudioFormat("xadpcm");
		sys.PushBits(*file.bits);
		FItem.Ext.push_back(".wav");
	}
	else
	{
		sys.PushAudioFormat("");
		sys.PushBits(0);
		FItem.Ext.push_back(".unknown");
	}

	//fseek(in, FItem.Offset[x] +48, 0);
	fseek(in, FItem.Offset[x] +4, 0);
	sys.PushChunk(in, 4);
	FItem.Chunk[x] -= 44;

	// Move offset to point to start of audio- straight after header
	FItem.Offset[x] += HEADERSIZE;
	FItem.Ext.push_back(".wav");
}

//fseek(in, 4, 0);
//int pos;
//for (int x = 0; x < CItem.Files; ++x){
//	fread(&pos, 4, 1, in);
//	//ShowMessage(x);
//	//ShowMessage((FItem.Offset[x+1]) - (pos + FItem.Chunk[x]));
//}


}

