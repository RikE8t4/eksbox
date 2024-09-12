


//============================================================================
//	Scx.cpp - High Speed Brute Force Multi-Format File Scanner / Extractor
//============================================================================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Scx.h"

#include "Strings.h"

#ifdef __BORLANDC__
#include "Main.h"
#include "Extract.h"
#define FORM Form1
#endif

//#define SCANSPEEDTEST

//==============================================================================

int Scx::Init(){

#ifdef __VISUALC__
gui.ClearInfo();
gui.ScxLblSetup();
filex.ClearVariables();
CItem.Files = 0;
return 0;
#else

gui.ProgressFxOff();

if (FORM->ComboSmpRate->Enabled)
{
	gui.ClearInfo();
	gui.ScxLblSetup();
	filex.ClearVariables();
	CItem.Files = 0;

}
else
if (FORM->OpenDialogScx->Execute())
{
	gui.CancelAllSet(false);
	if (gui.LabelArchiveValGet() != "")
		gui.LabelArchiveValSet("0", false);

	gui.LabelFilesValSet("0", true);
	return 0;
}

return -1;
#endif

}

//==============================================================================

int Scx::SilentMode(const string& fileName){

gui.Status("SCX - Hybrid Mode", 0, 0);
gui.CancelAllSet(false);
gui.LabelFilesValSet("0", true);

if (Scan(false) == -1)
{
	return -1;
}

return 0;

}

//==============================================================================

int Scx::Scan(const bool& standAlone){

string maincaption1, maincaption2, progresscaption;

#ifdef __BORLANDC__
maincaption1 = gui.LabelCaption1Get();
maincaption2 = gui.LabelCaption2Get();
progresscaption = gui.ProgressGetCaption();
#endif


int batchFiles = gui.OpenDlgGetTotalFiles();
CItem.ScxMode = true;

if (standAlone)
{
	if (Init() == -1) return 1;
	sys.TimerStart();
	CItem.Loading = true;
}


int extracted = 0, filesTotal = 0, z = 0;
while (z < batchFiles && !gui.CancelAllGet())
{

	if (standAlone)
	{
		#ifdef __VISUALC__
		CItem.OpenFileName = CItem.OpenFileName;
		#else
		CItem.OpenFileName = FORM->OpenDialogScx->Files->operator [](z).c_str();
		#endif
		sys.DirNameSet();
	}


	FILE *in = fopen(CItem.OpenFileName.c_str(), "rb");

	if (!in)
	{
		gui.Error("Couldn't Open: "
		+ str.ExtractFileName(CItem.OpenFileName), 1200, in);
		++z;
		continue;
	}

	if (standAlone)
	{
		if (batchFiles == 1)
		{
			gui.LabelCaption1Set("SCX - ", false);
			gui.LabelCaption2Set(str.ExtractFileName(CItem.OpenFileName), false);
		}
		else
		{
			gui.LabelCaption1Set("SCX - ", false);
			gui.LabelCaption2Set(str.ExtractFileName(CItem.OpenFileName) +
			" (" + str.IntToStr(z+1) + "/" + str.IntToStr(batchFiles) + ")", false);
		}
	}
	else
	{
		gui.LabelCaption1Set("Scx Hybrid Mode - ", false);
		gui.LabelCaption2Set(str.ExtractFileName(CItem.OpenFileName), false);
	}

	sys.GetFileSize(in, true);

	// Check if file is too small to scan
	if (CItem.FileSize < 20)
	{
		fclose(in); ++z; continue;
	}


	//-------------
	// Buffer Calc
	//-------------
	int bufferSize;
	int availMem = sys.GetAvailPhyMem();

	// Small
	if (gui.ComboBufSizeGet() == "Small" && standAlone)
	{
		bufferSize = CItem.FileSize >> 3;
			while (bufferSize > availMem >> 2) bufferSize >>= 3;
	}
	// Medium
	else if (gui.ComboBufSizeGet() == "Medium"  || !standAlone)
	{
		bufferSize = CItem.FileSize >> 2;
			while (bufferSize > availMem >> 1) bufferSize >>= 2;
	}
	// Large
	else
	{                                    
		bufferSize = CItem.FileSize >> 1;
			while (bufferSize > availMem) bufferSize >>= 1;
	}

	if (standAlone)
		gui.LabelAudioValSet(str.SizeToStr((ulong)bufferSize), false);

	uchar* buffer;

	if (! (buffer = sys.MemoryAlloc((ulong)bufferSize, in)) )
	{
		++z;
		continue;
	}


	gui.Open("", "", "Scanning...");
	gui.ProgressReset();
	gui.ProgressStepSize(100 / (CItem.FileSize / bufferSize));
	gui.ProgressCaption("Scanning..");
	gui.ProgressCaptionUpdate();

	fseek(in, 0, SEEK_SET);

	//----------------
	// Header Scanner
	//----------------

	uint posSubtract = (CItem.FileSize / bufferSize) >> 1;

	int x = 0, pos = 0;
	while (!feof(in) && gui.ExtractDlgGetIsShowing())
	{
		#ifdef __BORLANDC__
		sys.ProcessMessages();
		#endif

		fseek(in, pos, 0);
		fread(buffer, 1, bufferSize, in);

		while (!feof(in) && x <= bufferSize)
		{
//			// Mp3
//			if ( (buffer[x] == 0x49 && buffer[x+1] == 0x44  &&
//				buffer[x+2] == 0x33 && buffer[x+3] == 0x03) ||
//				  // Dreamfall
//				(buffer[x] == 0xFF && buffer[x+1] == 0xFB &&
//				buffer[x+7] == 0x00 && buffer[x+8] == 0x00 &&
//				(buffer[x+2] == 0x90 || buffer[x+2] == 0x50 || buffer[x+2] == 0x54) ) )
//				  //&& (buffer[x+4] == 0x00 || buffer[x+6] == 0x00)
//			{
//				FItem.Offset.push_back(pos);
//				FItem.Ext.push_back(".mp3");
//			}
			// Ogg Vorbis
			if (buffer[x] == 0x4F && buffer[x+1] == 0x67  &&
					buffer[x+2] == 0x67 && buffer[x+3] == 0x53 &&
					buffer[x+4] == 0x00 && buffer[x+5] == 0x02 &&
					buffer[x+29] == 0x76 && buffer[x+30] == 0x6F)
					//buffer[x+31] == 0x72 && buffer[x+32] == 0x62)
			{
					FItem.Offset.push_back(pos);
					FItem.Ext.push_back(".ogg");
			}
			// Asf
			if (buffer[x] == 0x53 && buffer[x+1] == 0x43  &&
					buffer[x+2] == 0x48 && buffer[x+3] == 0x6C)
			{
					FItem.Offset.push_back(pos);
					FItem.Ext.push_back(".asf");
			}
//			// Psf
//			else if (buffer[x] == 0x50   && buffer[x+1] == 0x53 &&
//					 buffer[x+2] == 0x46 && buffer[x+3] == 0x01)
//
//			{
//					FItem.Offset.push_back(pos);
//					FItem.Ext.push_back(".psf");
//			}
			// Wav
			else if (buffer[x] == 0x52   && buffer[x+1] == 0x49 &&
					 buffer[x+2] == 0x46 && buffer[x+3] == 0x46 &&
					 buffer[x+8] == 0x57 && buffer[x+9] == 0x41 &&
					 buffer[x+10] == 0x56 && buffer[x+11] == 0x45 &&
					 buffer[x+12] == 0x66 && buffer[x+13] == 0x6D)

			{
					FItem.Offset.push_back(pos);
					FItem.Ext.push_back(".wav");
			}
			// Windows Media Audio
			else if ( buffer[x] == 0x30 && buffer[x+1] == 0x26 &&
					buffer[x+2] == 0xB2 && buffer[x+3] == 0x75 &&
					buffer[x+4] == 0x8E && buffer[x+5] == 0x66 &&
					buffer[x+6] == 0xCF && buffer[x+7] == 0x11 &&
					buffer[x+8] == 0xA6 && buffer[x+9] == 0xD9 &&
					buffer[x+10] == 0x00 && buffer[x+11] == 0xAA &&
					buffer[x+12] == 0x00 && buffer[x+13] == 0x62 &&
					buffer[x+14] == 0xCE && buffer[x+15] == 0x6C)

					// && buffer[x+4] == 0x8E && buffer[x+14] == 0xCE
			{
					FItem.Offset.push_back(pos);
					FItem.Ext.push_back(".wma");
			}

//			// Xwc
//			else if (buffer[x] == 0x4D   && buffer[x+1] == 0x4F &&
//					 buffer[x+2] == 0x53 && buffer[x+3] == 0x20 &&
//					 buffer[x+4] == 0x44 && buffer[x+5] == 0x41
//					 & buffer[x+6] == 0x54)
//			{
//				if (pos){
//					FItem.Offset.push_back(pos);
//					FItem.Ext.push_back(".xwc");
//				}
//			}

			// Xwb
			else if (buffer[x] == 0x57   && buffer[x+1] == 0x42 &&
					buffer[x+2] == 0x4E && buffer[x+3] == 0x44 &&
					buffer[x+5] == 0x00 && buffer[x+6] == 0x00 &&
					buffer[x+7] == 0x00 && buffer[x+11] == 0x00)
			{
				if (pos){
					FItem.Offset.push_back(pos);
					FItem.Ext.push_back(".xwb");
				}
			}
			// Bmp
			else if (buffer[x] == 0x42 && buffer[x+1] == 0x4D  &&
					buffer[x+10] == 0x36 && buffer[x+14] == 0x28
					&& buffer[x+15] == 0x00)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".bmp");
			}
			// Dds
			else if (buffer[x] == 0x44 && buffer[x+1] == 0x44  &&
					buffer[x+2] == 0x53 && buffer[x+3] == 0x20 &&
					buffer[x+4] == 0x7C && buffer[x+5] == 0x00)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".dds");
			}
			// Png
			else if (buffer[x] == 0x89 && buffer[x+1] == 0x50
					&& buffer[x+2] == 0x4E && buffer[x+3] == 0x47
					&& buffer[x+4] == 0x0D && buffer[x+5] == 0x0A
					&& buffer[x+6] == 0x1A && buffer[x+7] == 0x0A
					//buffer[x+12] == 0x49 && buffer[x+13] == 0x48
					)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".png");
			}
			#ifdef SCXFAT
			// Adx
			else if (buffer[x] == 0x80 && buffer[x+1] == 0x00 &&
				buffer[x+4] == 0x03 && buffer[x+5] == 0x12  &&
				buffer[x+6] == 0x04)
					//&& buffer[x+7] == 0x02 &&
					//buffer[x+8] == 0x00 && (buffer[x+10] == 0xBB ||
					//buffer[x+10] == 0xAC) && (buffer[x+11] == 0x80 ||
					//buffer[x+11] == 0x44) )

			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".adx");
			}
//			// Flac
//			else if (buffer[x] == 0x66 && buffer[x+1] == 0x4C  &&
//					buffer[x+2] == 0x61 && buffer[x+3] == 0x43
//					&& buffer[x+7] == 0x22)
//			{
//				FItem.Offset.push_back(pos);
//				FItem.Ext.push_back(".flac");
//			}
			// Mpc
			else if (buffer[x] == 0x4D && buffer[x+1] == 0x50  &&
					buffer[x+2] == 0x2B && buffer[x+3] == 0x07 &&
					buffer[x+7] == 0x00 )
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".mpc");
			}

				//Non Audio Formats//

			// 3ds
			else if (buffer[x] == 0x4D && buffer[x+1] == 0x4D  &&
					buffer[x+6] == 0x02 && buffer[x+7] == 0x00 &&
					buffer[x+16] == 0x3D && buffer[x+17] == 0x3D)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".3ds");
			}
			// Lwo
			else if (buffer[x] == 0x46 && buffer[x+1] == 0x4F  &&
					buffer[x+8] == 0x4C && buffer[x+9] == 0x57 &&
					buffer[x+10] == 0x4F) //&& buffer[x+11] == 0x32
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".lwo");
			}
			// Pcx
			else if (buffer[x] == 0x0A && buffer[x+1] == 0x05  &&
					buffer[x+2] == 0x01 && buffer[x+3] == 0x08 &&
					buffer[x+4] == 0x00)//&& buffer[x+13] == 0x48
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".pcx");
			}
			// Gamecube - Thp (Video Format)
			else if (buffer[x] == 0x54 && buffer[x+1] == 0x48  &&
					buffer[x+2] == 0x50 && buffer[x+3] == 0x00 &&
					buffer[x+4] == 0x00)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".thp");
			}

//			// Gamecube - Adp (Audio Format)
//			else if (buffer[x] == 0x3B && buffer[x+1] == 0x3B  &&
//					buffer[x+2] == 0x3B && buffer[x+3] == 0x3B &&
//					buffer[x+4] == 0x00)
//			{
//				FItem.Offset.push_back(pos);
//				FItem.Ext.push_back(".adp");
//			}
//
////			// Gamecube - Ymf (Audio Format)
////			else if (buffer[x] == 0x00 && buffer[x+1] == 0x00  &&
////					buffer[x+2] == 0x01 && buffer[x+3] == 0x80 &&
////					buffer[x+4] == 0x00)
////			{
////				FItem.Offset.push_back(pos);
////				FItem.Ext.push_back(".ymf");
////			}

			// X (Direct X Model)
			else if (buffer[x] == 0x78 && buffer[x+1] == 0x6F  &&
					buffer[x+2] == 0x66 && buffer[x+3] == 0x20 &&
					buffer[x+16] == 0x0D && buffer[x+17] == 0x0A)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".x");
			}
			// Xbg (Xbox Geometry)
			else if (buffer[x] == 0x58 && buffer[x+1] == 0x42  &&
					buffer[x+2] == 0x47 && buffer[x+3] == 0x02 &&
					buffer[x+18] == 0x80 && buffer[x+19] == 0x3F)
			{
				FItem.Offset.push_back(pos);
				FItem.Ext.push_back(".xbg");
			}
			#endif

			if (x == bufferSize)
			{
				x = 0;
				pos -= posSubtract;// Buffer back-tracking
				gui.LabelFilesValSet(str.IntToStr((int)FItem.Offset.size()), false);
				break;
			}

		++pos, ++x;
		}// End inner while loop

	gui.ProgressStep();
	}// End outer while loop

	sys.MemoryDel();

	CItem.Files = (int)FItem.Offset.size();

	#ifdef __BORLANDC__
	if (!standAlone){
		gui.LabelCaption1Set(maincaption1, false);
		gui.LabelCaption2Set(maincaption2, false);
		gui.ProgressSetCaption(progresscaption);
	}
	#endif

	if (CItem.Files > 0){


	// Calc filechunks
	for (int x = 0; x < CItem.Files; ++x)
	{
		// Xwb
		if (FItem.Ext[x] == ".xwb")
		{
			fmtUtil.XwbSize(in, x);
		}
		// Wav
		else if (FItem.Ext[x] == ".wav")
		{
			FItem.Chunk.push_back(fmtUtil.RiffSize(in, FItem.Offset[x]));
		}
		// Wma
		else if (FItem.Ext[x] == ".wma")
		{
			fmtUtil.WmaSize(in, FItem.Offset[x]);
		}
		// Ogg
		else if (FItem.Ext[x] == ".ogg")
		{
			fmtUtil.OggSize(in, x);
		}
		// Bmp
		else if (FItem.Ext[x] == ".bmp")
		{
			int size;
			fseek(in, FItem.Offset[x] + 2, 0);
			fread(&size, 4, 1, in);
			FItem.Chunk.push_back(size);
		}
		// Dds
		else if (FItem.Ext[x] == ".dds")
		{
			fmtUtil.DdsSize(in, x);
		}
		// Png
		else if (FItem.Ext[x] == ".png")
		{
			fmtUtil.PngSize(in, x);
		}
		// Others
		else
		{
			if (x < CItem.Files -1)
				FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
			else
				FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);
		}
	}


	if (FItem.Ext[CItem.Files-1] != ".ogg" && FItem.Ext[CItem.Files-1] != ".wav"
		&& FItem.Ext[CItem.Files-1] != ".bmp" && FItem.Ext[CItem.Files-1] != ".dds"
		&& FItem.Ext[CItem.Files-1] != ".png"
		&& FItem.Ext[CItem.Files-1] != ".wma" && FItem.Ext[CItem.Files-1] != ".xwb")
	{
		FItem.Chunk.pop_back();
		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);
	}

//==============================================================================

	if (!standAlone)
	{
		if (CItem.Files == 1){
			FItem.FileName.push_back(CItem.DirName);
		}
		else
		{
			bool tidy = gui.ChkTidyName();
			for (int x = 0; x < CItem.Files; ++x)
			{
				string name;
				str.AutoNumScx(tidy, name, x, CItem.Files);
				FItem.FileName.push_back(name);
			}
		}
		fclose(in);
		//gui.Close();
		CItem.ScxMode = false;
		if (CItem.Files > 0)
			return 0;
		return -1;
	}

//==============================================================================

	// DirName
	if (!sys.DirPathIsSet())
	{
		sys.DirPathSetSilent();
	}

	if (gui.EditFolderNameTextGet() != "")
	{
		string txt = gui.EditFolderNameTextGet();
		if (gui.ChkTidyName())
			str.StrClean(txt);
		CItem.DirName = txt;
	}

	if (gui.ChkTidyName())
			str.StrClean(CItem.DirName);

	if (gui.ExtractDlgGetIsShowing())
		sys.CreateDir(sys.DirPathGet() + "\\" + CItem.DirName);

	if (gui.EditRenFilesTxtGet() != "")
		CItem.OpenFileName = gui.EditRenFilesTxtGet();

	bool tidy = gui.ChkTidyName();

	for (int x = 0; x < CItem.Files; ++x)
	{
		string name;
		str.AutoNumScx(tidy, name, x, CItem.Files);
		FItem.FileName.push_back
		(
			sys.DirPathGet() + "\\" + CItem.DirName + "\\" + name
		);
	}

	if (gui.EditRenFilesTxtGet() != "")
	{
		string text = gui.EditRenFilesTxtGet();
		for (int x = 0; x < CItem.Files; ++x)
		{
			FItem.FileNameBak.push_back(CItem.DirName);
			if (tidy)
			{
				str.StrClean(text);
				str.StrClean(FItem.FileNameBak[x]);
			}
			FItem.FileName[x] =
			sys.DirPathGet() + "\\" + CItem.DirName + "\\" +
			str.StringReplacer(text, x, CItem.Files) + FItem.Ext[x];
		}
		sys.DirNameSet();
	}

	gui.ProgressReset();
	gui.ProgressStepSize((float) 100 / CItem.Files);
	gui.Open("", "", "Writing...");


	#ifdef SCANSPEEDTEST
	double time = sys.TimerStop();
	gui.FinishDisplay(CItem.Files, CItem.Files, time, "Scanned", "Finished");
	CItem.Files = 0;
	return 0;
	#endif


	// Ouput All Files
	for (int i = 0; i < CItem.Files && gui.ExtractDlgGetIsShowing(); ++i, ++extracted, ++filesTotal)
	{
		#ifdef __BORLANDC__
		sys.ProcessMessages();
		#endif

		gui.ProgressCaptionBasic(str.ExtractFileName(FItem.FileName[i]), false);
		gui.ProgressStep();

		FILE* out = fopen (FItem.FileName[i].c_str(), "wb");

		if (! (buffer = sys.MemoryAlloc(FItem.Chunk[i], out)) )
		{
			continue;
		}

		fseek(in, FItem.Offset[i], 0);
		fread(buffer, 1, FItem.Chunk[i], in);

		try
		{
			fwrite(buffer, 1, FItem.Chunk[i], out);
		}
		catch(exception &e)
		{
			--extracted;
		}

		fclose(out);

		sys.MemoryDel();

	} // End output loop
	
	gui.LabelArchiveValSet(str.IntToStr(extracted), false);

	} // End if (CItem.Files > 0)
	else
	{
		if (!standAlone)
		{
			CItem.Files = 0;
			gui.Error("No recognised formats found", sys.Delay(true), in);
			return -1;
		}
		gui.LabelArchiveValSet(str.IntToStr(extracted), false);
		gui.ProgressCaption(str.IntToStr(extracted) + " File/s Extracted");
		gui.FormUpdate();
	}

	filex.ClearVariables();
	gui.ProgressCaption("");
	fclose(in);

	++z;
} // End while loop


double time = sys.TimerStop();
gui.FinishDisplay(extracted, filesTotal, time, "Extracted", "Finished");

gui.ClearInfo();
gui.ScxLblSetup();
CItem.ScxMode = false;
CItem.Loading = false;
CItem.Files = 0;

return 0;

} // End Scan

//==============================================================================







