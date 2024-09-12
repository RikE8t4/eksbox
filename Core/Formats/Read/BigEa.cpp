


//==================================================================
//	BigEa.cpp - (Multi)
//==================================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"
#include "LowLevel.h"

//==============================================================================

bool Supported::BigEaRead(FILE* in){

FileX f;

static struct{
	uchar name[48];
}big;


fseek(in, 11, 0);
fread(&CItem.Files, 1, 1, in);

f.pos = 16;
uint offset = 0, chunk = 0;
for(int x = 0; x < CItem.Files; ++x)
{
	
	fseek(in, f.pos, 0);
	fread(&offset, 4, 1, in);
	fread(&chunk, 4, 1, in);

	BSWAPC(offset);
	BSWAPC(chunk);

	FItem.Offset.push_back(offset);
	FItem.Chunk.push_back(chunk);

	fgets((char*)big.name, 48, in);

	string tmpName = (char*)big.name;

	tmpName = str.ExtractFileName(tmpName);
	//tmpName.Delete(1, tmpName.LastDelimiter("\\"));

	FItem.FileName.push_back(tmpName);
	FItem.Ext.push_back(str.ExtractFileExt(FItem.FileName[x]));
	f.pos += 8 + (long)strlen((char*)big.name) + 1;

}

// Audioformat
for(int x = 0; x < CItem.Files; ++x)
{
	uchar hdr[4];
	fseek(in, FItem.Offset[x], 0);

	fread(hdr, 4, 1, in);

	if (hdr[0] == 'B' && hdr[1] == 'N' && hdr[2] == 'K' && hdr[3] == 'l')
	{
		f.PushAudioFormat("bnk");
	}
	else if (hdr[0] == 'S' && hdr[1] == 'C' && hdr[2] == 'H' && hdr[3] == 'l')
	{
		f.PushAudioFormat("mus");
		FItem.Ext[x] = ".mus";
	}
	else if (hdr[0] == 'x' && hdr[1] == 'D' && hdr[2] == 'F' && hdr[3] == 'P')
	{
		f.PushAudioFormat("mpf");
	}
	else if (hdr[0] == 'M' && hdr[1] == 'I' && hdr[2] == 'D' && hdr[3] == 'x')
	{
		f.PushAudioFormat("midx");
	}
	else
	{
		f.PushAudioFormat("unknown");
	}


f.PushChannel(0);
f.PushSampleRate(0);
f.PushBits(0);

}


//// Audioformat
////bool isSupported = false;
//for(int x = 0; x < CItem.Files; ++x)
//{
//
//	if (FItem.Format[x] == "mus")
//	{
//		uchar ptHdr[2];
//		fseek(in, FItem.Offset[x] + 8, 0);
//
//		for (int y = 0; ptHdr[0] != 'P' && ptHdr[1] != 'T'; ++y)
//		{
//			fread(ptHdr, 2, 1, in);
//
//			if (y > 48)
//			{
//				//isSupported = false;
//				continue;
//			}
//		}
//
//		if (ptHdr [0] != 'P' && ptHdr [1] != 'T')
//		{
//			uchar asfType[2];
//			fseek(in, FItem.Offset[x] + 12, 0);
//			fread(asfType, 2, 1, in);
//
//			if (asfType[0] == 0x06 && asfType[1] == 0x01)
//			{
//
//
//			}
//
//
//		}
//	}
//
//
//
//}


return true;

}

//==============================================================================

