

//=======================
//		RcfSwap.cpp
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

#include "RcfSwap.h"
#include "FileSwapHelper.h"


bool Supported::RcfSwap(){

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

ulong hdrSize = FItem.Offset[0] -128;

uchar* buf;
if (! (buf = sys.MemoryAlloc(hdrSize, NULL)) )
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

// Check if extra padding is req. @ beginning for each file
uchar test[8], xPad[8] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
vector<bool>extraPad;
for(int x = 0; x < CItem.Files; ++x)
{
	if (FItem.Format[x] == "pcm")
	{
		fseek(in, FItem.Offset[x] - 256, 0);
		fread(test, 8, 1, in);
		if (*test == *xPad) extraPad.push_back(1);
		else extraPad.push_back(0);
	}
	else extraPad.push_back(0);
}

// Check if padding is req. at eof
bool eofPad = 0;
uchar null[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
fseek(in, CItem.FileSize - 2048, 0);
fread(test, 8, 1, in);
if (*test == *null) eofPad = 1;

fclose(in);
sys.MemoryDel();

TStringList* FileListX = new TStringList;

bool wavExt;
//vector<ushort>channels;
//vector<uint>sampleRate;
//vector<string>audioFmt;
ushort channels[MAX];
uint sampleRate[MAX];
String audioFmt[MAX];

// Collect filenames from OpenDialog
Form1->OpenDialogFileSwap->Filter = ("Wav (Pcm)|*.wav;|All Files (*.*)|*.*");
for (int x; FileListX->Count < CItem.Files; )
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	x = FileListX->Count;
	Form1->OpenDialogFileSwap->Title = "File To Swap With: "
	+ str.StdToAnsi(FItem.FileName[x] + " (" + FItem.Format[x]) + ")";

	if (checked[x] == 0xff) FileListX->Add("");
	else if (Form1->OpenDialogFileSwap->Execute())
	{
		FileListX->AddStrings(Form1->OpenDialogFileSwap->Files);
		for (; x < FileListX->Count; ++x)
		{
			//sampleRate.push_back(00000);
			//channels.push_back(0);
			//audioFmt.push_back(0);

			in = fopen (FileListX->operator [](x).c_str(), "rb");

			GetAudioFormat(FileListX->operator [](x).c_str(), true, false, false,
			audioFmt[x].c_str(), channels[x], sampleRate[x], NULL, in);
			fclose(in);
		}
	}
	else FileListX->Add("");
}

// Write Rcf
//ulong offset[MAX], inChunk[MAX], offsetOut[4], size[4];
//vector<ulong>offset, inChunk;
ushort wavHdrSize, pad = 0;
ulong offsetOut[4], size[4];
ulong offset[MAX], inChunk[MAX];


gui.Caption("FileSwap - File Injection");
gui.ProgressStepSize((float short) 100 / CItem.Files);
gui.Open("Swapping File/s", "", "Working Some Magic...");


*rcf.padding = 2048;
if (rsd4.pcmPadding[0] != 0x2D)
for (int x = 0; x < 1920; ++x) rsd4.pcmPadding[x] = 0x2D;


for (int x = 0; x < CItem.Files && ExtractDlg->Visible; ++x)
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	gui.ProgressCaption(FItem.FileName[x] + FItem.Ext[x]);
	gui.ProgressStep();

	// Write selected files to new container file
	if ((FileListX->operator [](x) != "") && (checked[x] != 0xff)
		&& audioFmt[x] != 0xff)
	{

	in = fopen(FileListX->operator [](x).c_str(), "rb");

	//inChunk.push_back(0);
	inChunk[x] = sys.GetFileSize(in, false);

	// Get wav header size
	if (audioFmt[x] == "pcm")
	{
		wavHdrSize = GetWavHdrSize(in);
		inChunk[x] -= wavHdrSize;
	}

	// Pad each file so that it starts on an even byte boundry
	//offset.push_back(0);
	if (x == 0)
	{
		while ((offset[x] = rcf.fileNameIndexEnd + pad)% *rcf.padding != 0)
		++pad;
	}
	else
	{
		while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)% *rcf.padding != 0)
		++pad;
	}




	// Output offsets & chunks
	*offsetOut = offset[x];
	fseek(out, *rcf.index + 12 * x + 20, 0);

	if (audioFmt[x]=="pcm")
	{
		fwrite(offsetOut, 4, 1, out);

		if (extraPad[x] && x+1 != CItem.Files) inChunk[x] += 128 + 1920;
		else if (x+1 == CItem.Files && !eofPad) inChunk[x] = inChunk[x];

		else inChunk[x] += 128;

		//if (extraPad[x] && x+1 != Files) inChunk[x] += 128 + 1920;
		//else if (x+1 != Files)
		//else if (!extraPad) inChunk[x] += 128;

		fwrite((char*)&inChunk[x], 4, 1, out);
	}
	else
	{
		fwrite(offsetOut, 4, 1, out);
		fwrite((char*)&inChunk[x], 4, 1, out);
	}


	if (inChunk[x] == NULL) continue;
	else if (! (buf = sys.MemoryAlloc(inChunk[x], in)) )
	{
		fclose(out);
		return false;
	}

	// Write files
	if (audioFmt[x] == "pcm") fseek(in, wavHdrSize, 0);
	else fseek(in, 0, 0);
	fread(buf, inChunk[x], 1, in);

	// Write rsd4pcm header
	fseek(out, offset[x], 0);
	if (audioFmt[x]=="pcm")
	{
		fwrite(&rsd4.fullHeader, 128, 1, out);

		if (extraPad[x]) fwrite(&rsd4.pcmPadding, 1920, 1, out);
		fseek(out, offset[x] + rsd4.channelsOffset, 0);
		fwrite(&channels[x], 2, 1, out);
		fseek(out, offset[x] + rsd4.sampleRateOffset, 0);
		fwrite(&sampleRate[x], 2, 1, out);

		if (extraPad[x]) fseek(out, offset[x] + 128 + 1920, 0);
		else fseek(out, offset[x] + 128, 0);
	}


	fwrite(buf, inChunk[x], 1, out);

}

// Write old files from opened container file to new file
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


	//Calc offset
	//if (x == 0) offset[x] = Offset[0] -128;
	//else offset[x] = offset[x-1] + inChunk[x-1];


	// Pad each file so that it starts on an even byte boundry
	//offset.push_back(0);
	if (x == 0)
	{
		while ((offset[x] = rcf.fileNameIndexEnd + pad)% *rcf.padding != 0)
		++pad;
	}
	else
	{
		while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)% *rcf.padding != 0)
		++pad;
	}


	// Output offsets & chunks
	*offsetOut = offset[x];
	//*offsetOut = offset[x] - Offset[0];
	//*offsetOut = offset[x] - offset[0];
	fseek(out, *rcf.index + 12 * x + 20, 0);


	if (FItem.Format[x]=="pcm")
	{
		fwrite(offsetOut, 4, 1, out);
		inChunk[x] += 128;
		fwrite((char*)&inChunk[x], 4, 1, out);
		//inChunk[x] -= 128;
	}
	else
	{
		//*offsetOut += 128;
		fwrite(offsetOut, 4, 1, out);
		fwrite((char*)&inChunk[x], 4, 1, out);
	}


	if (inChunk[x]==NULL) continue;

	// Write files
	fseek(in, FItem.Offset[x], 0);
	fread(buf, inChunk[x], 1, in);

	// Write rsd4pcm header
	if (FItem.Format[x] == "pcm")
	{
		fseek(out, offset[x], 0);
		fwrite(&rsd4.fullHeader, 128, 1, out);
	}
	else fseek(out, offset[x], 0);

	fwrite(buf, inChunk[x], 1, out);

}

sys.MemoryDel();
fclose(in);
pad = 0;
}// End of main write loop


// Write padding to eof
//if (eofPad == 1)        //not sure about this
//{
	*size = sys.GetFileSize(out, false);
	fseek(out, *size, 0);

	uchar zero[2048];
	pad=0;
	//gui.Msg(*rcf.padding);
	//gui.Msg(*size);

	while ( (pad + *size) % *rcf.padding != 0 )
		++pad, zero[pad] = 0x00;

	fwrite(zero, pad, 1, out);
	sys.GetFileSize(out, false);
//}


delete(FileListX);
extraPad.clear();
checked.clear();
//channels.clear();
//sampleRate.clear();
//audioFmt.clear();
//offset.clear();
//inChunk.clear();
sys.GetFileSize(out, false);
fclose(out);

string progressCaption = "File Saved in: " + string(SWAPDIRNAME) + " Folder";double time = sys.TimerStop();gui.FinishDisplay(filesChecked, filesChecked, time, "Swapped", progressCaption);gui.Caption("Finished FileSwap");
return true;

}// End file swap





