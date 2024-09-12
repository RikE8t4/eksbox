


//===============================================================================
//	Xwc.cpp - Multi (Xbox/Xbox360/PC) - (Xbadpcm/Ogg container/Xma)
//===============================================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
//#include "../Helper.h"

//------------------------------------------------------------------------------

bool Supported::XwcRead(FILE* in){


Supported file;

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

if (xwc.rootName[0] == 'W') // Xbox 360 - Darkness - Root should be 'WAVEDIRECTORY2'
{
	xwc.x360 = true;
	fseek(in, *xwc.index + 224, 0);
	fread(&xwc.x360OffsetsIndex, sizeof(int), 1, in);
}
else if (xwc.rootName[0] == 'V') // Xbox / PC - Root should be 'VERSION'
	xwc.x360 = false;
else
{ // Unsupported type
	return false;
}

fseek(in, *xwc.index + 88, 0);	fread(&CItem.Files, 2, 1, in);
fseek(in, *xwc.index + 128, 0);	fread(xwc.fileIndex, 4, 1, in);
fseek(in, *xwc.index + 132, 0); fread(xwc.fileIndexSize, 2, 1, in);
file.pos = *xwc.fileIndex;

switch (xwc.x360) {
	case false: // Offsets / chunks / names (Xbox/PC)

		for (int x = 0; x < CItem.Files; ++x)
		{
			fseek(in, file.pos, 0);
			sys.PushOffset(in, 4); sys.PushChunk(in, 4);
			fread(xwc.nameLen, 4, 1, in); sys.PushFileName(in, *xwc.nameLen);

			int	pad = 0; // Calc filename padding
			while ( (pad + *xwc.nameLen) %4 ) ++pad;
			file.pos = ftell(in) + pad;
		}
		break;

	case true: // Offsets / chunks / names (Xbox 360 - Darkness)

		char name[80];
		//CItem.Files += 2;
		for (int x = 0; x < CItem.Files; ++x)
		{
			fseek(in, file.pos, 0);
			fgets(name, 64, in);
			sys.PushFileName((char*)name);
			file.pos += (int)strlen(name)+1;
		}
		file.pos = xwc.x360OffsetsIndex + 12;
		for (int x = 0; x < CItem.Files; ++x, file.pos += 24)
		{
			fseek(in, file.pos, 0);
			sys.PushOffset(in, 4);
			sys.PushChunk(in, 4);
		}
		break;

	//default:
	//	break;

	}

// Audio specs
for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x] + 12, 0); sys.PushChannel(in, 2);

	// File format (Enclave)
	fseek(in, FItem.Offset[x] + 24, 0);
	fread(file.fmt, 4, 1, in);

	if (!memcmp(file.fmt, "MPEG", 4) )
	{
		sys.PushAudioFormat("mpeg");
		sys.PushExtension(".mpeg");
		sys.PushSampleRate(0);
		sys.PushBits(16);
		FItem.Offset[x] += 24; FItem.Chunk[x] -= 24;
		continue;
	}

	// File format (Pc Version: Ogg, Xbox: Xbadpcm)
	fseek(in, FItem.Offset[x] + 28, 0);
	fread(file.fmt, 4, 1, in);

	fseek(in, FItem.Offset[x] + 38, 0);
	fread(file.sr, 2, 1, in);
									   
	if (!memcmp(file.fmt, "VORB", 4) )
	{
		sys.PushAudioFormat("ogg");
		sys.PushExtension(".ogg");
		sys.PushBits(16);
		FItem.Offset[x] += 40; FItem.Chunk[x] -= 40;
	}
	else if (!memcmp(file.fmt, "XACM", 4) )
	{
		sys.PushAudioFormat("xadpcm");
		sys.PushExtension(".wav");
		sys.PushBits(4);
		// Remove 2048 bytes of padding from start
		FItem.Offset[x] += 2048;
		FItem.Chunk[x] -= 2048;
	}
	else if (!memcmp(file.fmt, "XMA", 3) )
	{
		sys.PushAudioFormat("xma");
		sys.PushExtension(".xma");
		sys.PushBits(16);
		// Remove 2048 bytes of padding from start
		FItem.Offset[x] += 28;
		FItem.Chunk[x] -= 28;
	}
	else //Unknown format types
	{
		FItem.SampleRate.push_back(0);
		sys.PushAudioFormat(".unknown");
		sys.PushExtension(".unknown");
		sys.PushBits(0);
		FItem.Offset[x] += 24;
		FItem.Chunk[x] -= 24;
		continue;
	}

	*file.sr <<= 2; // Multiply sampleRate by 4 (shl 2)
	if (*file.sr == 22048)
		*file.sr = 22050; // Strange sample rate fix

	FItem.SampleRate.push_back(*file.sr);
}

return true;

}

