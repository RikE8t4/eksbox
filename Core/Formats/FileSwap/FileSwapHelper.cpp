

//===============================
//		FileSwapHelper.cpp
//===============================


#include <stdafx.h>
#pragma hdrstop
//#include "FileSwapHelper.h"
#include "Gui.h"
#include "Strings.h"
#include "FormatHdrs.h"


//------------------------------------------------------------------------------

short GetWavHdrSize(FILE* in){

short wavHdrSize;

ushort fmt[2];
uchar 	fact[4];

fseek(in, 16, 0);
fread(&wavHdrSize, 2, 1, in);

fseek(in, 20, 0);
fread(fmt, 2, 1, in);

if (*fmt != 0x01)
{
	fseek(in, wavHdrSize + 20, 0);
	fread(fact, 4, 1, in);
	if ( !memcmp(fact, "fact", 4) )
		wavHdrSize += 36;
	else
		wavHdrSize += 24;
}
else
	wavHdrSize += 24;


return wavHdrSize + 4;

}// End GetWavHdrSize

//------------------------------------------------------------------------------

int GetWmaSpecs(ushort& channels, ushort& sampleRate,
							ushort& bits, FILE* in){

uchar hdr[16];

fseek(in, 0, 0);
fread(hdr, 16, 1, in);

if (!memcmp(hdr, wma.header, 16))
{

	uchar sig[8192];
	fseek(in, 0, 0);
	fread(sig, 8192, 1, in);

	int pos = 0;

	for(int y = 0; y < 8192; ++y)
		if ( (sig[y] == 0x53 && sig[y+1] == 0x65)
			&& (sig[y+2] == 0x72 || sig[y+2] == 0x73) && sig[y+3] == 0x00 )
		{
			pos = y;
			break;
		}

	if (!pos) return -1;

	fseek(in, pos + 66, 0);

	// Channels
	fread(&channels, 2, 1, in);

	// SampleRate
	fread(&sampleRate, 4, 1, in);

	// Bits
	fseek(in, ftell(in) + 6, 0);
	fread(&bits, 2, 1, in);

	return 0;
}
else
return -1;

}// End GetWmaSpecs

//------------------------------------------------------------------------------

int GetAudioFormat(const string& fileName,
		bool IsPcmOk, bool IsXadpcmOk, bool IsWmaOk,
		string& audioFmt, ushort& channels, ushort& sampleRate,
		ushort& bits, FILE* in)

{

// Wav
if (str.GetFileExt(fileName) == "wav")
{
	uchar wavHdr[4], fmt[2];
	fseek(in, 0, 0);
	fread(wavHdr, 4, 1, in);
	if (strncmp (wavHdr, "RIFF", 4) == 0)
	{
		fseek(in, 20, 0);
		fread(fmt, 2, 1, in);
		fread(&channels, 2, 1, in);
		fread(&sampleRate, 4, 1, in);
		fseek(in, 34, 0);
		fread(&bits, 2, 1, in);
		if (*fmt == 1) audioFmt = "pcm";
		else if (*fmt == 105) audioFmt = "xadpcm";
	}
	else
	{
		gui.MsgError("Invalid/Unsupported wav file");
		audioFmt = 0xff;
		return -1;
	}
}
// Wma
else if (str.GetFileExt(fileName) == "wma")
{
	uchar wmaHdr [16];
	fseek(in, 0, 0);
	fread(wmaHdr, 16, 1, in);
	if (strncmp (wmaHdr, wma.header, 16) == 0)
	{
		audioFmt = "wma";
		GetWmaSpecs(channels, sampleRate, bits, in);
	}
	else
	{
		gui.MsgError("This is not a valid wma file");
		audioFmt = 0xff;
		return -1;
	}
}
else
return -1;


if (audioFmt == "pcm" && !IsPcmOk)
{
	gui.MsgError("Pcm Is Not Supported");
	audioFmt = 0xff;
	return -1;
}
else if (audioFmt == "xadpcm" && !IsXadpcmOk)
{
	gui.MsgError("Xadpcm Is Not Supported");
	audioFmt = 0xff;
	return -1;
}
else if (audioFmt == "wma" && !IsWmaOk)
{
	gui.MsgError("Wma Is Not Supported");
	audioFmt = 0xff;
	return -1;
}

return 0;

} // End GetAudioFormat



