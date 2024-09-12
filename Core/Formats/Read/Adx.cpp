


//=============================
//		Adx.cpp - Multi
//=============================



#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "Convert/Adx2wav.h"

//==============================================================================

void Supported::AdxRead(FILE* in){

filex.PushFiles(1);
filex.PushAudioFormat("adx");
filex.PushOffset(0);
filex.PushChunkDefault();
filex.PushFileNameDefault();
filex.PushExtension(".adx");

uchar buf[16];
fseek(in, 0, 0);
fread(buf, 1, 16, in);

filex.PushChannel(buf[7]);
filex.PushSampleRate((ushort)read_long(buf +8));
filex.PushBits(16);

}

//==============================================================================




