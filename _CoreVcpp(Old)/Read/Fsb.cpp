

// FMod Sound Bank
//============================================================================
//	Fsb.cpp - Flatout 2,  BioShock (PC) - (Audio container)
//===========================================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::FsbRead(FILE* in){

#define FSB_FILENAME_LEN 30

static struct{
	int files;
	int totHdrSize;
	int totCompSmplSize;
}fsb;

uchar 	fsbFileName[FSB_FILENAME_LEN];
short 	fsbEachHdrSize;

Supported f;

fseek(in, 4, 0);
fread(&fsb, sizeof(fsb), 1, in);
CItem.Files = fsb.files;

// 1st offset
FItem.Offset.push_back(fsb.totHdrSize + 24);
f.pos = 24;


for (int x = 0; x < CItem.Files; ++x, f.pos += fsbEachHdrSize)
{

memset(fsbFileName, '\0', FSB_FILENAME_LEN);
fseek(in, f.pos, 0);
fread(&fsbEachHdrSize, 2, 1, in);

fgets((char*)fsbFileName, FSB_FILENAME_LEN , in);
FItem.FileName.push_back((char*)fsbFileName);
									  
fseek(in, f.pos + 36, 0);
sys.PushChunk(in, 4);

// Get current offset by adding last chunk to last offset (No padding between files)
FItem.Offset.push_back(FItem.Offset[x] + FItem.Chunk[x]);

fseek(in, f.pos + 52, 0);
sys.PushSampleRate(in, 4);

fseek(in, f.pos +62, 0);
sys.PushChannel(in, 2);

sys.PushAudioFormat("xadpcm");
sys.PushBits(4);
sys.PushExtension(".wav");
}

}



