


//==================================================================
//			AsfMus.cpp - (Multi) - Experimental stage!!
//==================================================================



#include <stdafx.h>
#include "Engine.h"
#include "SystemX.h"
#include "Strings.h"
#include "Gui.h"
#include "LowLevel.h"

//==============================================================================

inline DWORD ReadBytes(FILE* file, const BYTE& count)
{
  BYTE	i, byte;
  DWORD result = 0L;

  for (i = 0; i < count ; ++i)
  {
	fread(&byte, sizeof(BYTE), 1, file);
	result <<= 8;
	result += byte;
  }

  return result;
}

bool Supported::AsfMusRead(FILE* in){

struct{
	uint 	hdrSize;
	uchar 	scclhHdr[4];
	uchar 	ptHdr[2];
	int 	ptPos;
	uchar 	asfType[2];
	byte  	bits;
	byte  	channels;
	byte  	compression;
	bool	isSplitBlockType;
}asf;


// Header check
fseek(in, 4, 0);
fread(&asf.hdrSize, 4, 1, in);

fseek(in, asf.hdrSize, 0);
fread(asf.scclhHdr, 4, 1, in);

if (memcmp(asf.scclhHdr, "SCCl", 4))
	return false;


// Prerequisites
filex.PushFiles(1);
filex.PushAudioFormat("mus");
filex.PushOffset(asf.hdrSize + 12);
filex.PushChunk(sys.GetFileSize(in, false) - (asf.hdrSize + 12));
filex.PushFileName(str.ExtractFileName(CItem.OpenFileName));
filex.PushExtension(".mus");
filex.PushChannel(2);
filex.PushSampleRate(22050);
filex.PushBits(16);



// Audioformat
fseek(in, 8, 0);

for (int y = 0; memcmp(asf.ptHdr, "PT", 2); ++y)
{
	fread(asf.ptHdr, 2, 1, in);

	if (y > 48){
		//gui.Status("Error: Incompatible/Corrupted Asf", 100, 0);
		return false;
	}
}
asf.ptPos = ftell(in);


fseek(in, 12, 0);
fread(asf.asfType, 2, 1, in);

if (asf.asfType[0] != 0x06 && asf.asfType[1] != 0x01){
	//gui.Status("Error: Incompatible/Corrupted Asf", 100, 0);
	return false;
}


BYTE buf = 0x00;

for(int x = 0; x < 24; ++x)
{

	fread(&buf, 1, 1, in);
							
	// Header end
	if (buf == 0xFF || x >= (int)asf.hdrSize)
		break;

	// SplitBlock field
	else if (buf == 0x80)
	{
		fread(&buf, 1, 1, in);
		if (buf == 0x01)
			asf.isSplitBlockType = true;
	}

	// SampleRate field
	else if (buf == 0x84)
	{                       //ShowMessage("sr");
		fread(&buf, 1, 1, in);
		FItem.SampleRate[0] = ReadBytes(in, buf);
	}
	// Bits field
	else if (buf == 0xA0)
	{                            //ShowMessage("bt");
		fseek(in, ftell(in) + 1, 0);
		fread(&buf, 1, 1, in);
		if (buf == 0x0A) // 16 bit
			FItem.Bits[0] = 16;
		else if (buf == 0x08) // 8 bit
			FItem.Bits[0] = 8;
	}

}

return true;

}

//==============================================================================

