


//========================================================================
//	Xwc.cpp - Multi (Xbox/Xbox360/PC) - (Xbadpcm/Ogg container/Xma)
//========================================================================



#include <stdafx.h>
#include "Engine.h"

//==============================================================================

bool Supported::XwcRead(FILE* in){


FileX f;

static struct
{
	int		index[4];
	char	rootName[14];
	bool	x360;
	int		x360OffsetsIndex;
	int		fileIndex[4];
	int		fileIndexSize[2];
	int		nameLen[4];
}xwc;

		
fseek(in, 24, 0);
fread(xwc.index, 4, 1, in);
fseek(in, *xwc.index, 0); 
fread(xwc.rootName, 14, 1, in);

// Xbox 360 - Darkness - Root should be 'WAVEDIRECTORY2'
if (xwc.rootName[0] == 'W')
{
	xwc.x360 = true;
	fseek(in, *xwc.index + 224, 0);
	fread(&xwc.x360OffsetsIndex, sizeof(int), 1, in);
}
// Xbox / PC - Root should be 'VERSION'
else if (xwc.rootName[0] == 'V')
	xwc.x360 = false;
// Unsupported type
else
{
	return false;
}

fseek(in, *xwc.index + 88, 0);	fread(&CItem.Files, 2, 1, in);
fseek(in, *xwc.index + 128, 0);	fread(xwc.fileIndex, 4, 1, in);
fseek(in, *xwc.index + 132, 0); fread(xwc.fileIndexSize, 2, 1, in);
f.pos = *xwc.fileIndex;

switch (xwc.x360)
{

	// Xbox / PC - Offsets / chunks / names
	case false:

		for (int x = 0; x < CItem.Files; ++x)
		{
			fseek(in, f.pos, 0);
			f.PushOffset(in, 4);
			f.PushChunk(in, 4);
			fread(xwc.nameLen, 4, 1, in);
			f.PushFileName(in, *xwc.nameLen);

			int	pad = 0; // Calc filename padding
			while ( (pad + *xwc.nameLen) %4 ) ++pad;
			f.pos = ftell(in) + pad;
		}

		break;

	// Xbox 360 (Darkness) - Offsets / chunks / names
	case true:

		char name[80];
		//CItem.Files += 2;
		for (int x = 0; x < CItem.Files; ++x)
		{
			fseek(in, f.pos, 0);
			fgets(name, 64, in);
			f.PushFileName((char*)name);
			f.pos += (int)strlen(name)+1;
		}

		f.pos = xwc.x360OffsetsIndex + 12;
		for (int x = 0; x < CItem.Files; ++x, f.pos += 24)
		{
			fseek(in, f.pos, 0);
			f.PushOffset(in, 4);
			f.PushChunk(in, 4);
		}

		break;

} // End switch


// Audio specs
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] + 12, 0);
	f.PushChannel(in, 2);

	// File format (Enclave)
	fseek(in, FItem.Offset[x] + 24, 0);
	fread(f.fmt, 4, 1, in);

	if (!memcmp(f.fmt, "MPEG", 4) )
	{
		f.PushAudioFormat("mpeg");
		f.PushExtension(".mpeg");
		f.PushSampleRate(0);
		f.PushBits(16);
		FItem.Offset[x] += 24;
		FItem.Chunk[x] -= 24;
		continue;
	}

	// File format (PC Version: Ogg, Xbox: Xbadpcm)
	fseek(in, FItem.Offset[x] + 28, 0);
	fread(f.fmt, 4, 1, in);

	fseek(in, FItem.Offset[x] + 38, 0);
	fread(f.sr, 2, 1, in);
									   
	if (!memcmp(f.fmt, "VORB", 4) )
	{
		f.PushAudioFormat("ogg");
		f.PushExtension(".ogg");
		f.PushBits(16);
		FItem.Offset[x] += 40;
		FItem.Chunk[x] -= 40;
	}
	else if (!memcmp(f.fmt, "XACM", 4) )
	{
		f.PushAudioFormat("xadpcm");
		f.PushExtension(".wav");
		f.PushBits(4);
		// Remove 2048 bytes of padding from start
		FItem.Offset[x] += 2048;
		FItem.Chunk[x] -= 2048;
	}
	else if (!memcmp(f.fmt, "XMA", 3) )
	{
		f.PushAudioFormat("xma");
		f.PushExtension(".xma");
		f.PushBits(16);
		// Remove 2048 bytes of padding from start
		FItem.Offset[x] += 28;
		FItem.Chunk[x] -= 28;
	}
	else // Unknown format types
	{
		FItem.SampleRate.push_back(0);
		f.PushAudioFormat(".unknown");
		f.PushExtension(".unknown");
		f.PushBits(0);
		FItem.Offset[x] += 24;
		FItem.Chunk[x] -= 24;
		continue;
	}

	*f.sr <<= 2; // Multiply sampleRate by 4 (shl 2)
	if (*f.sr == 22048)
		*f.sr = 22050; // Strange sample rate fix

	FItem.SampleRate.push_back(*f.sr);
}

return true;

}

//==============================================================================

