


//==========================================================
//	Misc.cpp - Multi - (Unknown formats - use SCX to detect)
//==========================================================



#include <stdafx.h>
#include "Engine.h"
#include "../Scx.h"

//==============================================================================

bool Supported::MiscRead(){

Scx scx;
if (scx.SilentMode(CItem.OpenFileName) != 0)
{
	++CItem.IncompatibleFmts;
	return false;
}

bool ogg = false, wma = false, wav = false;

FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");

for (int x = 0; x < CItem.Files; ++x)
{                             
	if (FItem.Ext[x] == ".ogg"){
		if (!ogg)
		ogg = true;
		filex.PushAudioFormat("ogg");
	}
	else if (FItem.Ext[x] == ".wma"){
		if (!wma)
		wma = true;
		filex.PushAudioFormat("wma");
	}
	else if (FItem.Ext[x] == ".wav")
	{
		if (!wav)
		wav = true;
		uchar fmt[2];

		fseek(in, FItem.Offset[x] + 20, 0);
		fread(fmt, 2, 1, in);
		if (*fmt == 0x69)
			filex.PushAudioFormat("xadpcm");
		else if (*fmt == 0x01)
			filex.PushAudioFormat("pcm");
		else
			filex.PushAudioFormat("wav");
	}

	else
	{
		filex.PushAudioFormat("");
	}

	//fseek(in, Offset[x] + 40, 0);
	//filex.PushSampleRate(in, 4);
	//filex.PushBits(16);
	//filex.PushChannel(2);
}

if (wav)
{
	fmtUtil.RiffSpecs(in, false);
	CItem.HasAudioInfo = true;
}
else if (wma)
{
	fmtUtil.WmaSpecs(in, false);
	CItem.HasAudioInfo = true;
}

return true;

}

//==============================================================================



