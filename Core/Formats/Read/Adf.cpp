


//=====================================
//		Adf.cpp - GTA: Vice City (PC)
//=====================================



#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"

//==============================================================================

void Supported::AdfRead(FILE* in){
           
filex.PushFiles(1);
filex.PushAudioFormat("mp3");
filex.PushOffset(0);
filex.PushChunkDefault();
filex.PushFileNameDefault();
filex.PushExtension(".mp3");
filex.PushChannel(2);
filex.PushSampleRate(44100);
filex.PushBits(16);   

}

//==============================================================================




