


//==========================================================
//	Wma.cpp - Multi - (Unknown formats - use SCX to detect)
//==========================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"
#include "../Scx.h"

//------------------------------------------------------------------------------

bool Supported::MiscRead(){

Scx scx;
if (scx.SilentMode(CItem.OpenFileName) != 0)
{
	++CItem.IncompatileFmts;
	return false;
}

//ShowMessage(CItem.Files);
//if (!CItem.Files) {
	//++CItem.IncompatileFmts;
//	return false;
//}
bool ogg = false, wma = false, wav = false;

FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");

for (int x = 0; x < CItem.Files; ++x)
{                             
	if (FItem.Ext[x] == ".ogg"){
		if (!ogg)
		ogg = true;
		sys.PushAudioFormat("ogg");
	}
	else if (FItem.Ext[x] == ".wma"){
		if (!wma)
		wma = true;
		sys.PushAudioFormat("wma");
	}
	else if (FItem.Ext[x] == ".wav")
	{
		if (!wav)
		wav = true;
		uchar fmt[2];

		fseek(in, FItem.Offset[x] + 20, 0);
		fread(fmt, 2, 1, in);
		if (*fmt == 0x69)
			sys.PushAudioFormat("xadpcm");
		else if (*fmt == 0x01)
			sys.PushAudioFormat("pcm");
		else
			sys.PushAudioFormat("wav");
	}

	else
	{
		sys.PushAudioFormat("");
	}

	//fseek(in, Offset[x] + 40, 0);
	//sys.PushSampleRate(in, 4);
	//sys.PushBits(16);
	//sys.PushChannel(2);
}

//sys.WmaSpecs(in, false);
//ShowMessage((int)wav);
if (wav){
	sys.RiffSpecs(in, false);
	CItem.HasAudioInfo = true;
}
else if (wma){
	sys.WmaSpecs(in, false);
	CItem.HasAudioInfo = true;
}

//fclose(in);
return true;

}



