


//=============================================================================
//	Bor.cpp - Beats Of Rage - (MS Adpcm Container, Unfinished / Non-Functional)
//=============================================================================

// Unfinished!!

#ifdef DEMO_ONLY

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"

#include "SystemX.h"

//==============================================================================

void Supported::BorRead(FILE* in){

struct{
	u_char name[64];
}bor;

CItem.Files = 1;

fseek(in, 16, 0);
fread(bor.name, 64, 1, in);
if (!strlen(bor.name))
	FItem.FileName.push_back(CItem.DirName);
else
{
	FItem.FileName.push_back((char*)bor.name);
	fread(bor.name, 64, 1, in);
	if (strlen(bor.name))
		FItem.FileName[0] = FItem.FileName[0] + " - " + (char*) bor.name;
}

fseek(in, 146, 0);
filex.PushChannel(in, 2);
filex.PushSampleRate(in, 4);
filex.PushOffset(160);
filex.PushChunk(CItem.FileSize - 160);
filex.PushBits(4);
filex.PushAudioFormat("adpcm");
filex.PushExtension(".wav");

}

//==============================================================================

#endif



