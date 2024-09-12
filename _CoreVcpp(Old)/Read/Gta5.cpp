


//=======================================================
//  Gta5.cpp - GTA: San Andreas - (Simple Ogg container)
//=======================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"
#include "../Scx.h"

//------------------------------------------------------------------------------

bool Supported::Gta5Read(){


Scx scx;
if ( scx.SilentMode(CItem.OpenFileName) == -1 )
{
	return false;
}

FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");
CItem.HasAudioInfo = 1;

for (int x = 0; x < CItem.Files; ++x)
{
	sys.PushAudioFormat("ogg");

	fseek(in, FItem.Offset[x] + 40, 0);
	sys.PushSampleRate(in, 4);
	sys.PushBits(16);
	sys.PushChannel(2);
}

//FileName.push_back(DirName);
//Ext.push_back(".ogg");

//fseek(in, 40, 0);
//ContainsAudioInfo = 1;
//sys.PushSampleRate(in, 4);
fclose(in);
return true;
}


