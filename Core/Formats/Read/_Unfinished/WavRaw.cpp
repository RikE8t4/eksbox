

//=======================================================
//	WavRaw.cpp - Used For Test Purposes (Unfinished)
//=======================================================



#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Main.h"

//==============================================================================

void Supported::WavRawRead(FILE* in){

CItem.Files = 1;
FItem.Offset.push_back(0);
FItem.Chunk.push_back(CItem.FileSize);
FItem.FileName.push_back(CItem.DirName);
FItem.Ext.push_back(".wav");


if (Form1->ComboFormat->Text == "Auto")
{
	sys.PushAudioFormat("xadpcm");
}
else if (Form1->ComboFormat->Text == "PCM")
{
	sys.PushAudioFormat("pcm");
}
else if (Form1->ComboFormat->Text == "XADPCM")
{
	sys.PushAudioFormat("xadpcm");
}


if (Form1->ComboChan->Text == "Auto")
	FItem.Channels.push_back(2);
else if (Form1->ComboChan->Text == "Mono")
	FItem.Channels.push_back(1);
else if (Form1->ComboChan->Text == "Stereo")
	FItem.Channels.push_back(2);


if (Form1->ComboBits->Text == "Auto")
	FItem.Bits.push_back(4);
else
	FItem.Bits.push_back(Form1->ComboBits->Text.ToInt());


if (Form1->ComboSmpRate->Text == "Auto")
	FItem.SampleRate.push_back(44100);
else
	FItem.SampleRate.push_back(Form1->ComboSmpRate->Text.ToInt());


//sys.RiffSpecs(in, true);

}

//==============================================================================




