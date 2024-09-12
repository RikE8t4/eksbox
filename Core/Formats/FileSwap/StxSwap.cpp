

//=======================
//		StxSwap.cpp
//=======================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"

#include "Main.h"

#include "StxSwap.h"
#include "ConvertStx.h"
#include "FileSwapHelper.h"

#include "Extract.h"

#include "Gui.h"
#include "Strings.h"
#include "SystemX.h"

bool Supported::StxSwap(){

if (CItem.Files > 1)
{
	gui.Error("Surround Sound Stx's are not supported", sys.Delay(false), NULL);
	return false;
}


if (Form1->ListView1->Items->operator [](0)->Checked != true)
	return false;


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

ulong hdrSize = 2048;
uchar hdrBuf[2048];

fread(hdrBuf, hdrSize, 1, in);
fwrite(hdrBuf, hdrSize, 1, out);
fclose(in);


Form1->OpenDialogFileSwap->Filter = ("Wav (XAdpcm)|*.wav;|All Files (*.*)|*.*");

Form1->OpenDialogFileSwap->Title = "File To Swap With: " + str.StdToAnsi(FItem.FileName[0]
+ " (" + FItem.Format[0]) + ")";

ushort sampleRate, channels;

if (Form1->OpenDialogFileSwap->Execute())
{
	in = fopen (Form1->OpenDialogFileSwap->FileName.c_str(), "rb");

	int rtnVal =
	GetAudioFormat(Form1->OpenDialogFileSwap->FileName.c_str(), false, true, false,
	NULL, channels, sampleRate, NULL, in);


	if (rtnVal == -1)
	{
		fclose(in);
		fclose(out);
		return false;
	}
	//fclose(in);
}
else
{
	fclose(in);
	fclose(out);
	return false;
}



// Gui Stuff
CItem.FileSize = sys.GetFileSize(in, false);
float short chk = (CItem.FileSize / 2048 + (40) );
gui.ProgressStepSize((float short) 100 / chk);

gui.Caption("FileSwap - File Injection");
gui.Open("Converting/Swapping File/s", "", "Working Some Magic...");
gui.ProgressCaption(FItem.FileName[0] + FItem.Ext[0]);

short wavHdrSize = GetWavHdrSize(in);

// Get stx looping info
fclose(in);
in = fopen (CItem.OpenFileName.c_str(), "rb");
uchar loopBuf;
fseek(in, 26, 0);
fread(&loopBuf, 1, 1, in);

bool looping = true;

if (loopBuf == 0xFF)
	looping = false;

fclose(in);
in = fopen (Form1->OpenDialogFileSwap->FileName.c_str(), "rb");

XbadpcmToStx(in, out, channels, sampleRate, wavHdrSize, looping);

fclose(in);


// Finish up
sys.GetFileSize(out, false);
fclose(out);

gui.ProgressMaxOut();
string progressCaption = "File Saved in: " + string(SWAPDIRNAME) + " Folder";
double time = sys.TimerStop();int extracted = 1;gui.FinishDisplay(extracted, extracted, time, "Swapped", progressCaption);
gui.Caption("Finished FileSwap");

return true;
}// End file swap



