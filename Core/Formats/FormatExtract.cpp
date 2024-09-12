


//=============================================================
//		StdExtract.cpp - Standard/Generic file extraction
//=============================================================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Strings.h"

#ifdef __BORLANDC__
#include "Main.h"
#include "Extract.h"
#include "MediaPlayer.h"
#endif

#include "Convert/Adx2wav.h"
#include "Convert/ConvertStx.h"
#include "LowLevel.h"
#include "Zlib/CZlib.h"


//==============================================================================

// Audio files are written in scattered block form within map files,
// hence the need for an addon function
bool Supported::MapExtract(int i, uchar* buffer, FILE* in, FILE* out){

fseek(in, FItem.Offset[i], 0);

if (FItem.Parts[i] > 1)
{
	int xPos = 0, a = 0;
	while (a < i)
	{
		xPos += (FItem.Parts[a]);
		++a;
	}
	fread(buffer, FItem.Chunk1st[i], 1, in);
	fwrite(buffer, FItem.Chunk1st[i], 1, out);
	free(buffer); buffer = NULL;

	for (int z = 0; z < FItem.Parts[i]; ++z, ++xPos)
	{
		if (! (buffer = sys.MemoryAlloc(FItem.ChunkXtra[xPos], out)) )
		{
			free(buffer); buffer = NULL;
			fclose(in);
			sys.ExtractError(
			FItem.FileName[i] + FItem.Ext[i],sys.DirPathGet() + CItem.DirName);
			return false;
		}

		fseek(in, FItem.OffsetXtra[xPos], 0);
		fread(buffer, FItem.ChunkXtra[xPos], 1, in);
		fwrite(buffer, FItem.ChunkXtra[xPos], 1, out);
		free(buffer); buffer = NULL;

		// Tmp fix/hack - Certain files will be too long without this
		if (FItem.ChunkXtra[xPos] < 65520) 
			break;
	}

}// End if parts < y
else
{
	fread(buffer, FItem.Chunk[i], 1, in);
	fwrite(buffer, FItem.Chunk[i], 1, out);
	free(buffer); buffer = NULL;
}

return true;

}

//==============================================================================

bool Supported::StdExtract(const int& pFileNum){ // Needs cleaning up


#ifdef _MEDIAPLAYER
if (MediaPlayer.GetIsPlaying())  // No Extract support if file is playing
	return false;
#endif


int filesChecked = 0;
static int extracted = 0, filesTotal = 0;
int* checked = new int [CItem.Files];


#ifdef __BORLANDC__
if (Form1->ChkList->Checked)
{
int x = 0;
	while (x < CItem.Files)
	{
		if (Form1->ListView1->Items->operator [](x)->Checked)
		{
			checked[filesChecked++] = x;
		}
	++x;
	}
	if (filesChecked <= 0) return false;
}
else
{
int x = 0;
	while (x < CItem.Files)
	{
		checked[x++] = x;
	}
filesChecked = CItem.Files;
}
#else
for (int x = 0; x < CItem.Files; ++x)
{
	if (x == pFileNum -1 || pFileNum == -1)
	{
		checked[filesChecked++] = x;
	}

}
#endif


string dirName = sys.DirPathGet() + "\\" + CItem.DirName + "\\";

if (!sys.DirectoryExists(dirName))
	if (!sys.CreateDir(dirName))
	{
		gui.Error("Couldn't Make Directory: " + dirName, sys.Delay(true), 0);
		return false;
	}


FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");
if (!in)
{
	sys.ExtractError(CItem.OpenFileName, sys.DirPathGet() + CItem.DirName);
	gui.Error
	(
	"Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), in
	);
	return false;
}


#ifdef __BORLANDC__
bool
autoOn = 0, pcmOn = 0, xadpcmOn = 0,
chanAuto = 0, smpRateAuto = 0, bitsAuto = 0;
if (CItem.NeedsWavHeader)
{
	if (gui.ComboChanGet() == "Auto")
		chanAuto = 1;
	if (Form1->ComboSmpRate->Text == "Auto")
		smpRateAuto = 1;
	if (gui.ComboBitsGet() == "Auto")
		bitsAuto = 1;
}

if (Form1->ComboFormat->Text == "PCM")
	pcmOn = 1;
else if (Form1->ComboFormat->Text == "XADPCM")
	xadpcmOn  = 1;
else
	autoOn = 1;
#else
bool
autoOn = 1, pcmOn = 0, xadpcmOn = 0,
chanAuto = 1, smpRateAuto = 1, bitsAuto = 1;
#endif

// Setup Progress
CItem.PercentExtracted = 0.0f;

__int64 xTotalToExtract = 0, xTotalExtracted = 0;
for (int x = 0; x < CItem.Files; ++x)
	xTotalToExtract += FItem.Chunk[x];

gui.Open("Extracting/Converting Files", "", "Writing...");
gui.ProgressSetMax(xTotalToExtract);


// Extract files
int i;

for (int x = 0; x < filesChecked && gui.ExtractDlgGetIsShowing(); ++x, ++extracted, ++filesTotal)
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	i = checked[x];

	string name = sys.DirPathGet() + "\\" +  CItem.DirName + "\\"
	+ FItem.FileName[i] + FItem.Ext[i];

	if ((FItem.Format[i] == "adx" && pcmOn) 
		|| (FItem.Format[i] == "adx" && CItem.Format == "adx"))
		name = str.ChangeFileExt(name, ".wav");

	if (name.length() >= MAX_PATH)
	{
		sys.RemoveDir(name);
		gui.Error("Error: Max path (259) limit reached", sys.Delay(false), NULL);
		continue;
	}

	gui.ProgressCaptionBasic(FItem.FileName[i] + FItem.Ext[i], false);

	if (CItem.HasFolders || (int)FItem.FileName[i].find("\\") != -1)
		sys.BuildDirStruct(name.c_str());

	FILE* out = fopen(name.c_str(), "wb");

	if (!out)
	{
		gui.Error("Error: Output file could not be created", sys.Delay(false), out);
		continue;
	}

	try{
		if (CItem.NeedsWavHeader)
		{
			if (!chanAuto)
			{
				if (gui.ComboChanGet() == "Mono") 
					FItem.Channels[i] = 1;
				else 
					FItem.Channels[i] = 2;
			}
			if (!smpRateAuto)
				FItem.SampleRate[i] = gui.ComboSmpRateGet();

//		#ifdef DEBUG
//		// Adpcm header
//		if (FItem.Format[i] == "msadpcm")
//		{
//			sys.WriteMsadpcmHeader
//			(
//				FItem.Chunk[i], FItem.Channels[i], FItem.SampleRate[i], out
//			);
//		}
//		else
//		#endif

		// Pcm header
		if ( pcmOn || (autoOn && FItem.Format[i] == "pcm") )
		{
			if (!bitsAuto) 
				FItem.Bits[i] = str.StrToInt(gui.ComboBitsGet());

			fmtUtil.WritePcmHeader
			(
				FItem.Chunk[i], FItem.Channels[i],
				FItem.Bits[i], FItem.SampleRate[i], out
			);
		}
		// Xadpcm header
		else if ( FItem.Format[i] == "xadpcm" && (xadpcmOn || autoOn) )
		{
			fmtUtil.WriteXadpcmHeader
			(
				FItem.Chunk[i], FItem.Channels[i], FItem.SampleRate[i], out
			);
		}


		}// End write header

		if (CItem.Format == "stx")
		{
			bool surround = CItem.Files > 1;
			StxToXbadpcm(in, out, FItem.Channels[x], FItem.Offset[x], 
				surround, true, CItem.PercentExtracted);
			fclose(out);
			continue;
		}
		else if ((FItem.Format[i] == "adx" && pcmOn) // Adx to pcm - container format
		|| (FItem.Format[i] == "adx" && CItem.Format == "adx")) // Adx to pcm
		{
			adx2wav(FItem.Channels[i], FItem.SampleRate[i], FItem.Offset[i], in, out);
			fclose(out);
			continue;
		}
		else if (CItem.Format == "cabforza")
		{
			fseek(in, FItem.Offset[i], 0);
			CZlib z;
			z.DecompStream(in, out);  // Leaking memory!!!!
			fclose(out);
			continue;
		}
		else if (CItem.Format == "maphalo")
		{
			uchar* bufmap;

			if (! (bufmap = (uchar*) malloc(FItem.Chunk[i])) )
			{
				sys.ExtractError
				(
					FItem.FileName[i] + FItem.Ext[i], sys.DirPathGet() + CItem.DirName
				);
				free(bufmap);
				bufmap = NULL;
				fclose(out);
				continue;
			}
			if (!MapExtract(i, bufmap, in, out))
			{
				fclose(out);
				return false;
			}
			free(bufmap);
			bufmap = NULL;
			fclose(out);
			continue;
		}

//		uchar* buffer;
//
//		if (! (buffer = (uchar*) malloc(FItem.Chunk[i])) )
//		{
//			sys.ExtractError
//			(
//				FItem.FileName[i] + FItem.Ext[i], sys.DirPathGet() + CItem.DirName
//			);
//			fclose(out);
//			continue;
//		}

//		if (CItem.Format == "maphalo")
//		{
//			if (!MapExtract(i, buffer, in, out)) return false;
//			fclose(out);
//			continue;
//		}

		fseek(in, FItem.Offset[i], 0);
		const int BUF_SIZE = BUFSIZ << 7;

		if (FItem.Chunk[i] < BUF_SIZE) // Req for filesizes less than BUF_SIZE
		{ 
			int size = FItem.Chunk[i];
			uchar* buf = (uchar*) malloc(size);
			fread(buf, 1, size, in);
			fwrite(buf, 1, size, out);
			free(buf); buf = NULL;
			xTotalExtracted += size;
		}
		else
		{
			static char buffer[BUF_SIZE];
			int written, read, totWritten = 0;

			while ((read = (int)fread(buffer, 1, sizeof(buffer), in)) > 0 )
			{

				// Vice city decryption
				if (CItem.Format == "adf")
				{
					int bufSize = BUF_SIZE -1; // Use a copy as value is changed
					XORBUFC(bufSize, buffer, 0x22);
				}

		
				if ((written = (int)fwrite(buffer, 1, read, out)) != read)
					gui.Error("Write short", 1000, out);

				totWritten += written;
			
				float progress = float(xTotalExtracted += written);
				CItem.PercentExtracted = 100 / (xTotalToExtract / progress);
				//(progress / xTotalToExtract) * 100.0f; //

				// Write remaining data to file (Req for files with un-aligned sizes)
				if (totWritten + BUF_SIZE > FItem.Chunk[i])
				{
					int size = FItem.Chunk[i] - totWritten;
					uchar* buf = (uchar*) malloc(size);
					fread(buf, 1, size, in);
					fwrite(buf, 1, size, out);
					free(buf); buf = NULL;
					break;
				}

			} // End while
		} // End else

		gui.ProgressSetPosition(xTotalExtracted);

//		fread(buffer, FItem.Chunk[i], 1, in);
//
//		// Vice city decryption
//		if (CItem.Format == "adf")
//		{
//			int bufSize = FItem.Chunk[i] -1 ; // Use a copy as value is changed
//			XORBUFC(bufSize, buffer, 0x22); // LowLevel.cpp/h
//		}
//
//		fwrite(buffer, FItem.Chunk[i], 1, out);
//		free(buffer);
//		buffer = NULL;

	}
	// End try

	catch(exception &e)
	{
		sys.ExtractError(
		FItem.FileName[i] + FItem.Ext[i], sys.DirPathGet() + CItem.DirName);
		gui.Error("Extraction Error", 1000, NULL);
		--extracted;
	}

	fclose(out);

}
// End main while loop

CItem.PercentExtracted = 100.0f;

delete [] checked;
fclose(in);

              
if (CItem.Finished)
{
	double time = sys.TimerStop();
	gui.FormDoubleBufferedSet(true);

	if (gui.ChkAutoGuiClr()){
		gui.ClearInfo();
		filex.ClearVariables();
	}

	gui.FinishDisplay(extracted, filesTotal, time, "Saved", "Finished");
	gui.NameCaption("Finished Processing");
	gui.FormDoubleBufferedSet(false);
}

return true;

}

//==============================================================================

