

//=======================
//		WpxSwap.cpp
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

#include "WpxSwap.h"
#include "FileSwapHelper.h"

#define WPXHEADERSIZE 52

bool Supported::WpxSwap(){

struct{
	unsigned long	padding[4];
}wpx;

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

ulong hdrSize = FItem.Offset[0];

uchar* hdrBuf;
if (! (hdrBuf = sys.MemoryAlloc(hdrSize, NULL)) )
{
	return false;
}

FILE* inWpx = fopen (CItem.OpenFileName.c_str(), "rb");
if (!inWpx){
	gui.Error
	("Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), inWpx);
	return false;
}

string outDir = str.ExtractFileDir(CItem.OpenFileName) + "\\" + SWAPDIRNAME;
sys.CreateDir(outDir);

string outName = outDir + "\\" + str.ExtractFileName(CItem.OpenFileName);

FILE* out = fopen(outName.c_str(), "wb");
if (!out){
	gui.Error
	("Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), inWpx);
	return false;
}

fread(hdrBuf, hdrSize, 1, inWpx);
fwrite(hdrBuf, hdrSize, 1, out);

sys.MemoryDel();

TStringList* FileListX = new TStringList;

bool wavExt;
ushort channels[MAX];
ushort bits[MAX];
ushort sampleRate[MAX];
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
			FILE* in = fopen (FileListX->operator [](x).c_str(), "rb");

			GetAudioFormat(FileListX->operator [](x).c_str(), true, false, false,
			audioFmt[x].c_str(), channels[x], sampleRate[x], bits[x], in);
			fclose(in);
			//ShowMessage(sampleRate[x]);
			//ShowMessage(channels[x]);
		}
	}
	else FileListX->Add("");
}

// Write Wpx
ushort wavHdrSize;
int offset, lastOffset, inChunk, lastInChunk;

gui.ProgressStepSize((float short) 100 / CItem.Files);
gui.CaptionSave();
gui.Caption("FileSwap - File Injection");
gui.Open("Swapping File/s", "", "Working Some Magic...");

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

	FILE* in = fopen(FileListX->operator [](x).c_str(), "rb");
	if (!in){
		gui.Error
		("Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), in);
		return false;
	}

	lastInChunk = inChunk;
	inChunk = sys.GetFileSize(in, false);

	wavHdrSize = GetWavHdrSize(in);
	inChunk -= wavHdrSize;

	lastOffset = offset;
//	// Pad each file so that it starts on an even byte boundry
//	ushort pad = 0;
//	//offset.push_back(0);
//	if (x == 0)
//	{
//		while ((offset[x] = (FItem.Offset[0] - WPXHEADERSIZE) + pad)% (44)!= 0)
//		++pad;
//	}
//	else
//	{
//		while ((offset[x] = offset[x-1] + inChunk[x-1] + pad)% (44)!= 0)
//		++pad;
//	}

	// Output offsets & chunks
	if (x == 0)
		offset = FItem.Offset[0] - WPXHEADERSIZE;
	else
		offset = lastOffset + WPXHEADERSIZE + lastInChunk;

	fseek(out, (x + 1) * 4, 0);
	fwrite(&offset, 4, 1, out);

	uchar* buf;
	if (inChunk == NULL) continue;
	else if (! (buf = sys.MemoryAlloc(inChunk, in)) )
	{
		fclose(out);
		return false;
	}

	// Write header
	// Old
	if (x != 0){
		uchar hdrBuf[WPXHEADERSIZE];
		fseek(inWpx, FItem.Offset[x] - WPXHEADERSIZE, 0);
		fread(hdrBuf, WPXHEADERSIZE, 1, inWpx);
		fseek(out, offset, 0);
		fwrite(hdrBuf, WPXHEADERSIZE, 1, out);
	}

	// New
	fseek(out, offset + 22, 0);
	fwrite(&channels[x], 2, 1, out);

	fseek(out, offset + 24, 0);
	fwrite(&sampleRate[x], 2, 1, out);

	int bitRate = sampleRate[x] << 1;
	fseek(out, offset + 28, 0);
	fwrite(&bitRate, 2, 1, out);

	fseek(out, offset + 34, 0);
	fwrite(&bits[x], 2, 1, out);

	int inChunkOut = inChunk + 44;
	fseek(out, offset + 4, 0);
	fwrite(&inChunkOut, 4, 1, out);

	//if (x == 17)
	//{
		inChunkOut = inChunk;
		fseek(out, offset + 48, 0);
		fwrite(&inChunkOut, 4, 1, out);
	//}

	// Read audio
	fseek(in, wavHdrSize, 0);
	fread(buf, inChunk, 1, in);
	fclose(in);

	// Write audio
	fseek(out, offset + WPXHEADERSIZE, 0);
	fwrite(buf, inChunk, 1, out);
}

// Write old files from opened container file to new file
else if ((checked[x] == 0xff) || (FileListX->operator [](x) == "")
		|| (audioFmt[x] == 0xff))
{
		
	lastInChunk = inChunk;
	inChunk = FItem.Chunk[x];

	FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");

	uchar* buf;
	if (! (buf = sys.MemoryAlloc(inChunk, in)) )
	{
		fclose(out);
		return false;
	}


	lastOffset = offset;

	// Output offsets & chunks
	if (x == 0)
		offset = FItem.Offset[0] - WPXHEADERSIZE;
	else
		offset = lastOffset + WPXHEADERSIZE + lastInChunk;

	fseek(out, (x + 1) * 4, 0);
	fwrite(&offset, 4, 1, out);

	// Write header
	// Old
	if (x != 0){
		uchar hdrBuf[WPXHEADERSIZE];
		fseek(inWpx, FItem.Offset[x] - WPXHEADERSIZE, 0);
		fread(hdrBuf, WPXHEADERSIZE, 1, inWpx);
		fseek(out, offset, 0);
		fwrite(hdrBuf, WPXHEADERSIZE, 1, out);
	}

	// New
	//ShowMessage(offset + 4);
	//fseek(out, offset + 4, 0);
	//fwrite(&inChunkOut, 4, 1, out);
	int inChunkOut = inChunk + 44;
	fseek(out, offset + 4, 0);
	fwrite(&inChunkOut, 4, 1, out);

	if (inChunk==NULL) continue;

	// Read audio
	fseek(in, FItem.Offset[x], 0);
	fread(buf, inChunk, 1, in);
	fclose(in);

	// Write audio
	fseek(out, offset + WPXHEADERSIZE, 0);
	fwrite(buf, inChunk, 1, out);
}

sys.MemoryDel();
//fclose(in);
}// End of main write loop

delete(FileListX);
checked.clear();
sys.GetFileSize(out, false);
fclose(inWpx);
fclose(out);

string progressCaption = "File Saved in: " + string(SWAPDIRNAME) + " Folder";double time = sys.TimerStop();gui.FinishDisplay(filesChecked, filesChecked, time, "Swapped", progressCaption);gui.Caption("Finished FileSwap");
return true;
} // End file swap







