


//========================================================================
//	Hwx.cpp - Star Wars Episode III: Revenge Of The Sith - (xadpcm files)
//========================================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

void Supported::HwxRead(){

filex.PushFiles(1);
filex.PushAudioFormat("xadpcm");
filex.PushBits(4);
filex.PushChannel(2);
filex.PushSampleRate(44100);
filex.PushOffset(0);
filex.PushChunkDefault();
filex.PushFileNameDefault();
filex.PushExtension(".wav");

}

//==============================================================================



