


//=============================
//		Xwb.cpp - Multi
//=============================



#include <stdafx.h>
#include "Engine.h"
#include "FormatHdrs.h"
#include "Strings.h"
#include "Gui.h"
#include "SystemX.h"
#include "LowLevel.h" 		// For byteSwapping
#include "../Zlib/CZlib.h" 	// For zwb's

void ShowXwbSpecs();

//==============================================================================

// XWB INFO
//----------

// ======================================
// Known Xbox Xwb versions:  1, 2, 3, 37
// Known PC Xwb versions:  41, 43
// ======================================
/*
	V1 (Xbox1)
	----------
	1 padding/alignment type: (4 bytes), no Wma.

	V2 / V3 (Xbox1)
	----------------
	3 padding/alignment types: Ram (4 bytes), Hd (512 bytes), Dvd (2048)

	V37 (Non-Std) (Xbox1)
	---------------------
	Close in spec to v1's, used in Blazing Angels Squadrons Of WWII

	V41/43 (PC)
	-----------
	Close in spec to v2/3 Xbox Xwb's (PCM / AIFF support)
	2 padding/alignment types: Ram (0 bytes), Hd/Streaming (2048 bytes)
*/
//==============================================================================

// XSB INFO
//----------
/*
	V6: Used with v2 xwb's
	V11: Used with v3 xwb's
*/

//==============================================================================

bool Supported::XwbRead(FILE* in, ulong startPos, const bool& xwb360){

static struct
{
	DWORD       format      : 2;	// 0 = Pcm, 1 = Xbadpcm, 2 = Wma
	DWORD       channels	: 3;	// Channels (1 - 6)
	DWORD       sampleRate  : 26;	// SamplesPerSec
	DWORD       bits  		: 1;	// Bits per sample (8 / 16) [PCM Only]
}xwbMagic;


memset(&xwb, 0x00, sizeof(xwb));

xwb.hdrPos = startPos;

if (CItem.Format == "zwb")
{
	FILE* out = fopen(str.ChangeFileExt(CItem.OpenFileName, ".xwb").c_str(),"wb");

	if (!out){
		gui.Error("Error writing output file", sys.Delay(false), out);
		return false;
	}

	fseek(in, startPos, 0);
	startPos = 0;
	gui.Status("Decompressing, Wait..", 0, 0);


	CZlib zl; zl.DecompStream(in, out);
	fclose(in);
	fclose(out);
	CItem.OpenFileName = str.ChangeFileExt(CItem.OpenFileName, ".xwb");
	in = fopen(CItem.OpenFileName.c_str(), "rb");
	CItem.FileSize = sys.GetFileSize(in, true);
}



if (xwb360)
	xwb.x360 = true;



// Xwb version
bool pcVer = false;
fseek(in, startPos + 4, 0);
fread(xwb.ver, 2, 1, in);
if (xwb.x360) ByteSwap(*xwb.ver);
else if (*xwb.ver != 41 || *xwb.ver != 43) 
	pcVer = true;
else
if ( ( (*xwb.ver != 37) && *xwb.ver > 3 || *xwb.ver < 1) 
	&& !(xwb.x360) )
{
	gui.Error("Incompatible Xwb Type", 1500, in);
	return false;
}



// Padding size
if (*xwb.ver == 1) *xwb.padding = 4;
else if (xwb.x360) *xwb.padding = 0; // Incorrect?
else if (*xwb.ver == 37) *xwb.padding = 0;
else if (pcVer) *xwb.padding = 0; // Haven't checked
else
{
	fseek(in, startPos + 72, 0);
	fread(xwb.padding, 4, 1, in);
}



// Headersize
ulong pos = 16;
//if (xwb.x360) pos = 18;
if (*xwb.ver == 43) pos = 20; // PC

if (*xwb.ver != 1)
{
	fseek(in, startPos + pos, 0);
	fread(&xwb.headerSize, 4, 1, in);
}

if (*xwb.ver == 1) *xwb.headerSize = 76;
else if (xwb.x360) ByteSwap(*xwb.headerSize);




// Exclusive ver.2 & 3 specs
if (*xwb.ver != 1 && !xwb.x360 && !pcVer)
{
	// Offset to filename dir
	pos = 24;
	fseek(in, startPos + pos, 0);
	fread(&xwb.filenameDir, 4, 1, in);

	// Bank type
	fseek(in, startPos + 40, 0);
	fread(xwb.bankType, 2, 1, in);

	// Wave entries
	fseek(in, startPos + 42, 0);
	fread(xwb.waveEntries, 2, 1, in);

	// Looping, Enable Zero Latency Streaming & Remove Loop Tail
	fseek(in, startPos + *xwb.headerSize, 0);
	fread(xwb.loopZeroTail, 2, 1, in);

	// Converted sample
	fseek(in, startPos + *xwb.headerSize + 2, 0);
	fread(xwb.convertSample, 2, 1, in);
}



// File details length
if (xwb.x360) pos = 120;
else if (*xwb.ver == 37) pos = 20;
else if (*xwb.ver == 41) pos = 120; // PC
else if (*xwb.ver == 43) pos = 124; // PC
else pos = 64;

fseek(in, startPos + pos, 0);
fread(&xwb.detailsLength, 4, 1, in);

if (*xwb.ver == 1) *xwb.detailsLength = 20;
else if (xwb.x360) ByteSwap(*xwb.detailsLength);

// xwb.headerSize + 28 = details length

// Num of files
if (*xwb.ver == 1) pos = 12;
else if (xwb.x360) pos = 52;
else if (*xwb.ver == 37) pos = 52;
else if (*xwb.ver == 41) pos = 52; // PC
else if (*xwb.ver == 43) pos = 56; // PC
else pos = 44;

fseek(in, startPos + pos, 0);
fread(&CItem.Files, 4, 1, in);

if (xwb.x360) ByteSwap(CItem.Files);


// First file offset
if (*xwb.ver != 1)
{
	if (xwb.x360) pos = 40; //pos = 130;
	else if (*xwb.ver == 37) pos = 40;
	else if (*xwb.ver == 41) pos = 40; // PC
	else if (*xwb.ver == 43) pos = 44; // PC
	else pos = 32;
	fseek(in, startPos + pos, 0);
	fread(&xwb.offset1, 4, 1, in);

	if (xwb.x360) ByteSwap(*xwb.offset1);
}
else
	*xwb.offset1 = startPos + (CItem.Files * 20) + 80;

pos = startPos + *xwb.headerSize + 4;


if (!gui.SpeedHackIsOn())
{
	ShowXwbSpecs();
}



//-------------------------------------
//	  Offsets / Chunks / Audio Specs
//-------------------------------------

for (int x = 0; x < CItem.Files; ++x, pos += *xwb.detailsLength)
{
	ulong offset[4], chunk[4], magic[4];

	fseek(in, pos, 0);
	fread(magic, 4, 1, in);
	fread(offset, 4, 1, in);
	fread(chunk, 4, 1, in);
	if (xwb.x360)
	{
		ByteSwap(*magic);
		ByteSwap(*offset);
		ByteSwap(*chunk);
	}

	FItem.Offset.push_back(*offset);
	FItem.Chunk.push_back(*chunk);

	memcpy(&xwbMagic, magic, sizeof(ulong));
	FItem.SampleRate.push_back(xwbMagic.sampleRate);

	// Temp ver. 1 Hack
	if (*xwb.ver == 1 && !xwb.x360)
	{
		FItem.Ext.push_back(".wav");
		if (xwbMagic.format == 2) xwbMagic.format = 0;
		else if (xwbMagic.format == 3) xwbMagic.format = 1;

		if (xwbMagic.channels >= 4) FItem.Channels.push_back(1);
		else FItem.Channels.push_back((short)xwbMagic.channels+1);

		// Ugly samplerate fixes
		FItem.SampleRate[x] <<= 1;
		if (FItem.SampleRate[x] >= 22000 && FItem.SampleRate[x] <= 22554)
			FItem.SampleRate[x] = 22050;
		else if (FItem.SampleRate[x] >= 11000 && FItem.SampleRate[x] <= 11110)
			FItem.SampleRate[x] = 11025;


		if (xwbMagic.format == 0)
		{
			xwbMagic.format = 0;
			FItem.Format.push_back("pcm");
			if (xwbMagic.bits == 0)
				FItem.Bits.push_back(8);
			else
				FItem.Bits.push_back(16);
		}
		else //if (magic.formatTag == 3)
		{
			xwbMagic.format = 1;
			FItem.Format.push_back("xadpcm");
			FItem.Bits.push_back(4);
		}

	} // Ver. 1 Hack



	// Ver. 2 / 3 / Misc / Pc
	else if (!xwb.x360)
	{
		FItem.Channels.push_back(xwbMagic.channels);

		if (xwbMagic.format != 2)
		{
			FItem.Ext.push_back(".wav");

			// 16 Bit Pcm
			if (xwbMagic.bits == 1 && xwbMagic.format == 0){
				FItem.Bits.push_back(16);
				FItem.Format.push_back("pcm");
			}
			// 8 Bit Pcm
			else if (xwbMagic.bits == 0 && xwbMagic.format == 0){
				FItem.Bits.push_back(8);
				FItem.Format.push_back("pcm");
			}
			// Xbadpcm
			else if (xwbMagic.format == 1){
				FItem.Bits.push_back(4);
				FItem.Format.push_back("xadpcm");
			}

		}

		// Wma
		else if (xwbMagic.format == 2)
		{
			FItem.Bits.push_back(16);
			FItem.Format.push_back("wma");
			FItem.Ext.push_back(".wma");
		}
	} // End Ver. 2 / 3

	// X360 (Needs work)
	else if (xwb.x360)
	{
		FItem.Channels.push_back((short)xwbMagic.channels +1);
		FItem.Format.push_back("xma");
		FItem.Bits.push_back(16);
		FItem.Ext.push_back(".xma");
	}

// All Offsets (Except offset 1) need to be added to 1st offset
// to get the actual offset
FItem.Offset[x] += *xwb.offset1;

}// End Loop


// First offset


// V1
//else
if (*xwb.ver == 1)
	FItem.Offset[0] = pos;
// V2 / 3 / X360
else
	FItem.Offset[0] = (ulong) *xwb.offset1;



// Lord Of The Rings The Two Towers Hack (Temp)
//if (XWBHACK){
int total = 0;
for (int x = 0; x < CItem.Files; ++x)
	total += FItem.Chunk[x];

//if (startPos !=0)
if (total > CItem.FileSize || CItem.Files <= 0)
{
	FItem.Offset[0] = startPos + *xwb.offset1;
	FItem.Chunk[0] = CItem.FileSize - FItem.Offset[0];
	CItem.Files = 1;
	gui.Status("Xwb Hack Mode", sys.Delay(true), true);
}

//}



//-------------------
//		FileNames
//-------------------

if (*xwb.filenameDir != NULL)
{
CItem.HasNames = 1;
pos = *xwb.filenameDir;
uchar name[64];

for (int x = 0; x < CItem.Files; ++x, pos += 64)
{
	fseek(in, startPos + pos, 0);
	fgets((char*)name, 64, in);

	// Replace '\' & '.' from filename (Counter-Strike)
	// This code needs to be tested
	string n = (char*)name;
	if ((int)n.find("\\") != -1)
		n = str.ReplaceAll((char*)name, "\\", "-");
	if ((int)n.find(".") != -1)
		n = str.ReplaceAll((char*)name, ".", "-");

	FItem.FileName.push_back(n);

	}
}// End filenames loop





//-------------------
//		Xsb
//-------------------

fclose(in);
in =
fopen(str.ChangeFileExt(CItem.OpenFileName, ".xsb").c_str(), "rb");

if (!in) return true;

static struct{
ushort ver[2];
ushort entrySize;
ulong	pos;
}xsb;

fseek(in, 4, 0);
fread(xsb.ver, 4, 1, in);

if (*xsb.ver == 6)
{
	xsb.pos = 56;
	xsb.entrySize = 16;
}
else
{
	xsb.pos = 60;
	xsb.entrySize = 20;

}


int offset[4], lastOffset = 0;

fseek(in, xsb.pos, 0);
fread((char*)offset, 4, 1, in);

if (!*offset)
{
	CItem.HasNames = 0;
	return true;
}
else if (*offset >= CItem.FileSize || *offset < 60)
{
	CItem.HasNames = 0;
	return true;
}


uchar name[48];
CItem.HasNames = 1;
lastOffset = *offset;


for (int x = 0; x < CItem.Files;
							++x, xsb.pos += xsb.entrySize, lastOffset = *offset)
{
	fseek(in, xsb.pos, 0);
	fread((char*)offset, 4, 1, in);
	fseek(in, *offset, 0);
	fgets((char*)name, 48, in);

	if ( (*offset > lastOffset + 48) || *offset < 60 || !strlen((char*)name))
	{
		for (int y = x; y < CItem.Files; ++y)
		{
			FItem.FileName.push_back("");
			str.AutoNumList(0, FItem.FileName[y], y, CItem.Files);
		}
		return true;
	}
	FItem.FileName.push_back((char*)name);
} // End xsb

return true;

}


//==============================================================================


//-----------------------------
//	  Show Xwb specs
//-----------------------------


void ShowXwbSpecs(){

string bankType = 	"Bank Type: ",
compWavEntries = 	"Compact Wave Entries: ",
incWavEntries = 	"Include Wave Entries: ",
looping = 			"Looping: ",
zeroLatency = 		"Zero Latency Streaming: ",
remLoopTail = 		"Remove Loop Tail: ",
convToXadpcm = 		"Convert To XADPCM: ",
convTo8Bit = 		"Convert To 8-bit PCM: ";


if (xwb.x360 || *xwb.ver == 1)
{
	string unknown = "Unknown / NA";
	gui.SetInfoLbl(1, bankType += unknown);
	gui.SetInfoLbl(2, compWavEntries += unknown);
	gui.SetInfoLbl(3, incWavEntries += unknown);
	gui.SetInfoLbl(4, looping += unknown);
	gui.SetInfoLbl(5, zeroLatency += unknown);
	gui.SetInfoLbl(6, remLoopTail += unknown);
	gui.SetInfoLbl(7, convToXadpcm += unknown);
	gui.SetInfoLbl(8, convTo8Bit += unknown);
	return;
}



// Bank Type
if (*xwb.bankType == 0)
	bankType += "In-Memory (Ram)";
else if (*xwb.bankType == 1 && *xwb.padding == 512)
	bankType += "Streaming (Hd)";
else if (*xwb.bankType == 1 && *xwb.padding >= 2048)
	bankType += "Streaming (Dvd)";
else
	bankType += "Unknown";



// Wave Entries
if (*xwb.waveEntries == 0)
{
	compWavEntries += 	"False";
	incWavEntries += 	"False";
}
else if (*xwb.waveEntries == 1)
{
	compWavEntries += 	"False";
	incWavEntries += 	"True";
}
else if (*xwb.waveEntries == 2)
{
	compWavEntries += 	"True";
	incWavEntries += 	"False";
}
else if (*xwb.waveEntries == 3)
{
	compWavEntries += 	"True";
	incWavEntries += 	"True";
}
else
{
	compWavEntries += 	"Unknown";
	incWavEntries += 	"Unknown";
}



// Looping, Enable Zero Latency Streaming & Remove Loop Tail
if (*xwb.loopZeroTail == 0)
{
		looping += 		"False";
		zeroLatency += 	"False";
		remLoopTail += 	"False";
}
else if (*xwb.loopZeroTail == 1)
{
		looping += 		"False";
		zeroLatency += 	"True";
		remLoopTail += 	"False";
}
else if (*xwb.loopZeroTail == 2)
{
		looping += 		"True";
		zeroLatency += 	"False";
		remLoopTail += 	"False";
}
else if (*xwb.loopZeroTail == 3)
{
		looping += 		"True";
		zeroLatency += 	"True";
		remLoopTail += 	"False";
}
else if (*xwb.loopZeroTail == 4)
{
		looping += 		"False";
		zeroLatency += 	"False";
		remLoopTail += 	"True";
}
else if (*xwb.loopZeroTail == 5)
{
		looping += 		"False";
		zeroLatency += 	"True";
		remLoopTail += 	"True";
}
else if (*xwb.loopZeroTail == 6)
{
		looping += 		"True";
		zeroLatency += 	"False";
		remLoopTail += 	"True";
}
else if (*xwb.loopZeroTail == 7)
{
		looping += 		"True";
		zeroLatency += 	"True";
		remLoopTail += 	"True";
}
else
{
		looping += 		"Unknown";
		zeroLatency += 	"Unknown";
		remLoopTail += 	"Unknown";
}



// Converted sample
if (*xwb.convertSample == 0)
{
	convToXadpcm += "False";
	convTo8Bit += "False";
}
else if (*xwb.convertSample == 1)
{
	convToXadpcm += "True";
	convTo8Bit += "False";
}
else if (*xwb.convertSample == 2)
{
	convToXadpcm += "False";
	convTo8Bit += "True";
}
else
{
	convToXadpcm += "Unknown";
	convTo8Bit += "Unknown";
}


gui.SetInfoLbl(1, "Xwb Version: " + str.IntToStr(*xwb.ver));
gui.SetInfoLbl(2, bankType);
gui.SetInfoLbl(3, compWavEntries);
gui.SetInfoLbl(4, incWavEntries);
gui.SetInfoLbl(5, looping);
gui.SetInfoLbl(6, zeroLatency);
gui.SetInfoLbl(7, remLoopTail);
gui.SetInfoLbl(8, convToXadpcm);
gui.SetInfoLbl(9, convTo8Bit);

}

//==============================================================================






