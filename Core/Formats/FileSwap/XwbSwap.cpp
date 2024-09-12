

//=======================
//		XwbSwap.cpp
//=======================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Strings.h"
#include "FormatHdrs.h"

#include "Main.h"
#include "Extract.h"

#include "XwbSwap.h"
#include "FileSwapHelper.h"

bool Supported::XwbSwap(){


struct
{
	DWORD       format      : 2;	// 0 = Pcm, 1 = Xbadpcm, 2 = Wma
	DWORD       channels	: 3;	// Channels (1 - 6)
	DWORD       sampleRate  : 26;	// SamplesPerSec
	DWORD       bits  		: 1;	// Bits per sample (8 / 16) [PCM Only]
}xwbMagic;

//struct
//{
//	DWORD       format		: 2;	// 0 = Pcm, 1 = Xma, 2 = Xbadpcm
//	DWORD       channels    : 3;	// ""
//	DWORD       sampleRate  : 18;	// ""
//	DWORD       blockAlign  : 8;    // Block alignment
//	DWORD       bits  		: 1;	// ""
//}xwbMagic360;


//struct
//{
//	DWORD       format      : 1;	// 0 = Pcm, 1 = Xbadpcm
//	DWORD       channels    : 3;	// ""
//	DWORD       sampleRate  : 26;	// ""
//	DWORD       bits  		: 1;	// ""
//}xwbMagicV1;


int filesChecked = 0;
vector<ushort>checked;

for (int x = 0; x < CItem.Files; ++x)
{
	if (Form1->ListView1->Items->operator [](x)->Checked == 1){
		checked.push_back(x);
		++filesChecked;
	}
	else checked.push_back(0xff);
}

if (filesChecked <= 0) return false;

ulong hdrSize = xwb.hdrPos + *xwb.headerSize + CItem.Files * *xwb.detailsLength;

if (*xwb.ver == 1)
{
	hdrSize = FItem.Offset[0];
	*xwb.detailsLength = 20;
}

uchar* buf;
if (! (buf = sys.MemoryAlloc(xwb.hdrPos + hdrSize, NULL)) )
{
	return false;
}

FILE* in = fopen (CItem.OpenFileName.c_str(), "rb");
if (!in){
	gui.Error
	("Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), in);
	return false;
}


string outDir = str.ExtractFileDir(CItem.OpenFileName) + "\\" + SWAPDIRNAME;
sys.CreateDir(outDir);

string outName = outDir + "\\" + str.ExtractFileName(CItem.OpenFileName);

FILE* out = fopen(outName.c_str(), "wb");
if (!out){
	gui.Error
	("Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), in);
	return false;
}

fread(buf, hdrSize, 1, in);
fwrite(buf, hdrSize, 1, out);
fclose(in);
sys.MemoryDel();

TStringList* FileListX = new TStringList;
ushort sampleRate[MAX], channels[MAX], bits[MAX];
String audioFmt[MAX];

// Collect filenames from OpenDialog
Form1->OpenDialogFileSwap->Filter =
(
	"Wav (XAdpcm/Pcm), Wma|*.wav; *.wma;|All Files (*.*)|*.*"
);

for (int x; FileListX->Count < CItem.Files; )
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	x = FileListX->Count;
	Form1->OpenDialogFileSwap->Title = "File To Swap With: "
	+ str.StdToAnsi(FItem.FileName[x] + " (" + FItem.Format[x]) + ")";

	if (checked[x] == 0xff)
		FileListX->Add("");
	else if (Form1->OpenDialogFileSwap->Execute())
	{
		FileListX->AddStrings(Form1->OpenDialogFileSwap->Files);
		for (; x < FileListX->Count; ++x)
		{
			in = fopen (FileListX->operator [](x).c_str(), "rb");

			GetAudioFormat(FileListX->operator [](x).c_str(), true, true, true,
			audioFmt[x].c_str(), channels[x], sampleRate[x], bits[x], in);
			fclose(in);
		}
	}
	else FileListX->Add("");
}

// Write Xact Wave Bank
//vector<ulong>offset, inChunk;
ulong offset[MAX], inChunk[MAX];
ulong offsetOut[4], size[4], magic[4];
ushort wavHdrSize, pad;
uchar wavScan[4];


gui.Caption("FileSwap - File Injection");
gui.ProgressStepSize((float short) 100 / CItem.Files);
gui.Open("Swapping File/s", "", "Working Some Magic...");


for (int x = 0; x < CItem.Files && ExtractDlg->Visible; ++x)
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	gui.ProgressCaption(FItem.FileName[x] + FItem.Ext[x]);
	gui.ProgressStep();
	

	if ((FileListX->operator [](x) != "") && (checked[x] != 0xff)
		&& audioFmt[x] != 0xff)
	{
		in = fopen(FileListX->operator [](x).c_str(), "rb");

		// Audio file info & magic construct
		if (audioFmt[x] == "pcm")
		{
			xwbMagic.format = 0;
			// 16 bit = 1, 8 bit / others = 0
			if (bits[x] == 16)
				xwbMagic.bits = 1;
			else
				xwbMagic.bits = 0;
		}
		else if (audioFmt[x] == "xadpcm")
		{
			xwbMagic.format = 1;
			xwbMagic.bits = 0;
		}
		// Wma
		else
		{
			xwbMagic.format = 2;
			xwbMagic.bits = 1;
		}


		xwbMagic.channels = channels[x];
		if (*xwb.ver == 1)
			xwbMagic.sampleRate = sampleRate[x] / 2;
		else
			xwbMagic.sampleRate = sampleRate[x];


		// Wma->Hard coded magic, cos we're not yet getting info from files
		//if (audioFmt[x] == "wma") *magic = 2148894858;// 44100, stereo
		//else
		memcpy(&magic, &xwbMagic, 4);// Create magic

		//ShowMessage(xwbMagic.format);
		//ShowMessage(xwbMagic.channels);
		//ShowMessage(xwbMagic.sampleRate);
		//ShowMessage(xwbMagic.bits);


		// Ver 1 Xwb
		if (*xwb.ver == 1)// Not yet perfect
		{
			*xwb.headerSize = 76;
			*magic -= 2;
		//Temp code
//        if (audioFmt[x] == "xadpcm")
//			*magic = (sampleRate[x] * 32) + (1 + (channels[x] * 4));
//		else if (audioFmt[x] == "pcm" && channels[x] == 1)
//			*magic = (sampleRate[x] * 32) + 2147483652;
//		else if (audioFmt[x] == "pcm" && channels[x] == 2 && *xwb.ver == 1)
//			*magic = (sampleRate[x] * 32) + 2147483652 - 2;
//
//			*magic-=2;
		}

	//inChunk.push_back(0);
	inChunk[x] = sys.GetFileSize(in, false);

	// Wav header size
	if (audioFmt[x] != "wma")
	{
		fseek(in, 20, 0);
		while (strncmp(wavScan, "data", 4) && !feof(in))
			fread(wavScan, 4, 1, in);

		*wavScan = 0;
		wavHdrSize = ftell(in) +4 ;
		inChunk[x] -= wavHdrSize;
	}

	//offset.push_back(0);

	// Calc Offset
	if (x == 0) offset[x] = FItem.Offset[0];

	// Streaming bank type
	else //if(*xwb.ver != 1 && *xwb.ver != 37
	//		&& *xwb.bankType == 1 && audioFmt[x] != "wma")
	{
		//while ((offset[x] = offset[x-1] + inChunk[x-1]
		//		+ pad - *xwb.headerSize) % (*xwb.padding) != 0)
		while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)
				% (*xwb.padding) != 0)
		++pad;
	}
	// Temp fix, this should not be req
	//else if(*xwb.ver != 1 && *xwb.ver != 37
	 //		&& *xwb.bankType == 1 && audioFmt[x] == "wma")
	//{
	//	while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)
	//			% *xwb.padding != 0)
	//	++pad;
	//}


	// In-memory || ver1
	//else if (*xwb.ver == 1 || *xwb.ver == 37 || *xwb.bankType == 0)
		//while ((offset[x] = offset[x-1] + inChunk[x-1] + pad) % (4) != 0)
	//			while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)
	//			% *xwb.padding != 0)
	//		++pad;


	// Output offset
	if (x == 0)
		*offsetOut = 0x00000000;
	else
		*offsetOut = offset[x] - *xwb.offset1;


	//else *offsetOut = offset[x] - *xwb.offset1;
	//gui.Msg(*magic);
	//gui.Msg(*offsetOut);
	//gui.Msg(inChunk[x]);
	//gui.Msg(xwb.hdrPos + *xwb.headerSize + *xwb.detailsLength * x + 4);


	fseek(out, xwb.hdrPos + *xwb.headerSize + *xwb.detailsLength * x + 4, 0);
	fwrite(&magic, 4, 1, out);
	fwrite(offsetOut, 4, 1, out);
	fwrite(&inChunk[x], 4, 1, out);

	if (FItem.Format[x] == "xadpcm" || FItem.Format[x] == "pcm")
		fseek(in, wavHdrSize, 0);
	else
		fseek(in, 0, 0);


	if (! (buf = sys.MemoryAlloc(inChunk[x], in)) )
	{
		fclose(out);
		return false;
	}

	fread(buf, inChunk[x], 1, in);
	fseek(out, offset[x], 0);
	fwrite(buf, inChunk[x], 1, out);

}
// Write old files from opened xwb to new xwb
else if ((checked[x] == 0xff) || (FileListX->operator [](x) == "")
		|| (audioFmt[x] == 0xff))
{
	inChunk[x] = FItem.Chunk[x];
	//inChunk.push_back(Chunk[x]);

	if (! (buf = sys.MemoryAlloc(inChunk[x], in)) )
	{
		fclose(out);
		return false;
	}

	in = fopen(CItem.OpenFileName.c_str(), "rb");


	// Calc Offset
	//offset.push_back(0);
	if (x == 0) offset[x] = FItem.Offset[0];

	else
	{
		while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)
				% (*xwb.padding) != 0)
			++pad;
	}

	// Streaming bank type
	//else if(*xwb.ver != 1 && *xwb.ver != 37 && *xwb.bankType == 1)
	//{
	//	while ((offset[x] = offset[x-1] + inChunk[x-1]
	//			+ pad - *xwb.headerSize) % (*xwb.padding) != 0)
	//		++pad;
	//}

			//else if(*xwb.ver != 1 && *xwb.bankType == 1 && audioFmt[x] == "wma"){
			 //	while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)
			 //			% *xwb.padding != 0)
			 //		pad+=1;}

	// In-memory || ver1
	//else if ( (*xwb.ver == 1 || *xwb.ver == 37) || *xwb.bankType == 0)
	//	offset[x] = offset[x-1] + inChunk[x-1];


	// Output offset
	if (x == 0)
		*offsetOut = 0x00000000;
	else
		*offsetOut = offset[x] - *xwb.offset1;


	fseek(out, xwb.hdrPos + *xwb.headerSize + *xwb.detailsLength * x + 8, 0);
	//fwrite((char*)&magic, 4, 1, out);//Not required
	fwrite(offsetOut, 4, 1, out);
	fwrite((char*)&inChunk[x], 4, 1, out);

	fseek(in, FItem.Offset[x], 0);
	fread(buf, inChunk[x], 1, in);

	fseek(out, offset[x], 0);
	fwrite(buf, inChunk[x], 1, out);
}

sys.MemoryDel();
fclose(in);
pad = 0;
}// End of xwb write loop

// Finish up
if (*xwb.ver != 1 && *xwb.ver != 37){// Write padding to end of non ver.1 xwb

	//If Bank Type is not In-Memory (Ram)
	//if (*xwb.bankType != 0)
	//{
		*size = sys.GetFileSize(out, false);
		fseek(out, *size, 0);

		uchar zero[2048];
		pad = 0;
		//while ( (pad + *size) % *xwb.offset1  != 0 )
		while ( (pad + *size) % *xwb.padding != 0 )
			++pad, zero[pad] = 0x00;

		fwrite(&zero, pad, 1, out);
	//}
//	else if (*xwb.bankType == 0)
//	{
//		*size = sys.GetFileSize(out);
//		fseek(out, *size, 0);
//
//		uchar zero[2048];
//		pad = 0;
//		//while ((pad + *size) % (4) != 0)
//		while ( (pad + *size) % *xwb.padding != 0 )
//			++pad, zero[pad] = 0x00;
//
//		fwrite(&zero, pad, 1, out);
//	}

	*size = sys.GetFileSize(out, false);

	*size -= *xwb.offset1;
	fseek(out, xwb.hdrPos + 36, 0);
	fwrite(size, 4, 1, out);

}

// Zero out fileNameDirOffset
if (*xwb.ver != 1)
{
	uchar* null = { 0x00000000 } ;
	fseek(out, xwb.hdrPos + 24, 0);
	fwrite(&null, 4, 1, out);
}

delete(FileListX);
//offset.clear();
//inChunk.clear();
checked.clear();
sys.GetFileSize(out, false);
fclose(out);

string progressCaption = "File Saved in: " + string(SWAPDIRNAME) + " Folder";
double time = sys.TimerStop();gui.FinishDisplay(filesChecked, filesChecked, time, "Swapped", progressCaption);
gui.Caption("Finished FileSwap");

return true;
}// End file swap



