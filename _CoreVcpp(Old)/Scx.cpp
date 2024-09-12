


//=========================================================================
//	Scx.cpp - High Speed Brute Force Multi-Format File Scanner / Extractor
//=========================================================================

#include "stdafx.h" 
#include "Scx.h"
#include "Strings.h"

#include "FileSpecs.h"
//#include "Strings.h"
#include "SystemX.h"
#include "Helper.h"
#include "Gui.h"
#include "FormatHdrs.h"

//#define SCANSPEEDTEST

//------------------------------------------------------------------------------

inline void Scx::GetXwbSize(FILE* in, const int&x){

// Get version
ushort ver[2];
fseek(in, FItem.Offset[x] + 4, 0);
fread(ver, 2, 1, in);
if ( *ver == 0x02 || *ver == 0x03 )
{
	uint size[4];
	fseek(in, FItem.Offset[x] + 36, 0);
	fread(size, 4, 1, in);
	FItem.Chunk.push_back(*size + 2048);
}
else
	FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);

}

//------------------------------------------------------------------------------

inline void Scx::GetDdsSize(FILE* in, const int& x){

uchar dxtType[4];
int blockSize = 0;
fseek(in, FItem.Offset[x] + 84, 0);
fread(&dxtType, 4, 1, in);

if (dxtType[0] == 'D' &&  dxtType[1] == 'X' &&
	dxtType[2] == 'T')
{
	if (dxtType[3] == '1')
		blockSize = 8;
	else if (dxtType[3] == '2' || dxtType[3] == '3' || dxtType[3] == '4' || dxtType[3] == '5')
		blockSize = 16;

	int height, width;
	fseek(in, FItem.Offset[x] + 12, 0);
	fread(&height, 4, 1, in);
	fread(&width, 4, 1, in);
	//ShowMessage(height);
	FItem.Chunk.push_back((height >> 2) * (width >> 2) * (blockSize) + 128);
	//ShowMessage((height >> 2) * (width >> 2) * blockSize);
}

else
	if (x < CItem.Files -1)
		FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
	else
		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);

}

//------------------------------------------------------------------------------

// Based on delphi code by Alexande "Elbereth" Devilliers (Dragon UnPACKer)
inline void Scx::GetOggSize(FILE* in, const int& x){

int size = 0;
bool isStart = true;

fseek(in, FItem.Offset[x], 0);

while(1)
{
	fread(&OggHdr, sizeof(OggHdr), 1, in);

	if (memcmp(OggHdr.id, "OggS", 4) || OggHdr.revision != 0)
		break;
	else if (isStart && !(OggHdr.bitFlags & 2)
			|| !isStart && (OggHdr.bitFlags & 2))
		break;

	int oggPageLen = 0;

	for(int j = 0; j < OggHdr.numPageSegments; ++j)
	{
		uchar tbyte;
		fread(&tbyte, 1, 1, in);
		oggPageLen += tbyte;
	}

	size += oggPageLen + sizeof(OggHdr) + OggHdr.numPageSegments;

	if((OggHdr.bitFlags & 4) == 4)
		break;

	fseek(in, FItem.Offset[x] + size, 0);
	isStart = false;
}

FItem.Chunk.push_back(size);


}

//------------------------------------------------------------------------------

inline void Scx::GetPngSize(FILE* in, const int& x){

// 4945 4E44 AE42 6082 / IEND.B`.
int off = sys.HeaderScan(FItem.Offset[x] + 80, CItem.FileSize, "IEND", 4, in);

if (off != -1)
	FItem.Chunk.push_back(off - FItem.Offset[x] + 8);
else
{
	if (x < CItem.Files -1)
		FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
	else
		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);
}

//int off = sys.HeaderScanFast(FItem.Offset[x] + 80,
//1048576, "IEND", in);

//uchar buf[4];
//int pos = FItem.Offset[x] + 80;
//bool found = false;

//for (int xx = 0; xx < 1048576; ++xx, ++pos)
//{
//	fseek(in, pos, 0);
//	fread(&buf, 4, 1, in);
//	//1048576
//	if (buf[0] == 0x49)
//	if (buf[1] == 0x45)
//	if (buf[2] == 0x4E)
//	if (buf[3] == 0x44)
//	{
//		found = true;
//		break;
//	}
//}
//
//int off = 1024;
//
//if (off != -1)
//if (found)
//	FItem.Chunk.push_back(pos - FItem.Offset[x] + 8);
//	FItem.Chunk.push_back(off - FItem.Offset[x] + 8);
//else
//{
//	if (x < CItem.Files -1)
//		FItem.Chunk.push_back(FItem.Offset[x + 1] - FItem.Offset[x]);
//	else
//		FItem.Chunk.push_back(CItem.FileSize - FItem.Offset[CItem.Files - 1]);
//}

}

//------------------------------------------------------------------------------

int Scx::Init(){

//gui.ProgressFxOff();
sys.ClearVariables();
CItem.Files = 0;
return 0;

//if (Form1->ComboSmpRate->Enabled)
//{
//	gui.ClearInfo();
//	gui.ScxLblSetup();
//	sys.ClearVariables();
//	CItem.Files = 0;
//
//}
//else
//if (Form1->OpenDialogScx->Execute())
//{
//	gui.CancelAllSet(false);
//	//if (Form1->LblArchiveVal->Caption != "")
//	if (Form1->LblArchiveVal->Caption != "")
//		Form1->LblArchiveVal->Caption = 0;
//	Form1->LblFilesVal->Caption = 0;
//	Form1->LblFilesVal->Update();
//	return 0;
//}
//return -1;
}


int Scx::SilentMode(string fileName){

gui.Status("Scx Mode", 0);

//Form1->Caption = "SCX - Hybrid Mode";
//gui.CancelAllSet(false);
//gui.ProgressCaption("");
//Form1->LblFilesVal->Caption = 0;
//Form1->LblFilesVal->Update();
//gui.LblsSetup();

if (Scan(false, "") == -1)
{
	//gui.Caption(APPNAME);
	//Form1->Caption = APPNAME;
	//gui.ProgressCaptionBasic("", true);
	return -1;
}

//gui.Caption(APPNAME);
//Form1->Caption = APPNAME;
//gui.ProgressCaptionBasic("", true);

return 0;

}


int Scx::Scan(const bool& standAlone, const string& fileName){

//string 	maincaption1 = Form1->LblCaption->Text1,
//		maincaption2 = Form1->LblCaption->Text2,
//		progresscaption = Form1->EkImgProgressBar->Text;
string 	maincaption1,
		maincaption2,
		progresscaption;


int batchFiles = 1;
CItem.ScxMode = true;

if (standAlone)
{
	if (Init() == -1) return 1;
	batchFiles = 1;
	//batchFiles = Form1->OpenDialogScx->Files->Count;
	sys.TimerStart();
	CItem.Loading = true;
	//CItem.ScxMode = true;
}


int extracted = 0, filesTotal = 0, z = 0;
//while (z < batchFiles)//&& !gui.CancelAllGet()
//{

	if (standAlone && fileName != "")
	{
		CItem.OpenFileName = fileName;
		//CItem.OpenFileName = Form1->OpenDialogScx->Files->operator [](z);
		sys.DirNameSet();
	}


	FILE *in = fopen(CItem.OpenFileName.c_str(), "rb");

	if (!in)
	{
		gui.Error("Couldn't Open: " +
		ExtractFileName(CItem.OpenFileName), 1200, in);
		//++z;
		//continue;
	}

	if (standAlone){
		if (batchFiles == 1){
			//Form1->LblCaption->Text1 = "SCX - ";
			//Form1->LblCaption->Text2 = ExtractFileName(CItem.OpenFileName);
		}
		else{
			//gui.Caption( "SCX - (File " + String(z+1) + " of " + batchFiles + ")" );
			//Form1->LblCaption->Text1 = "SCX - ";
			//Form1->LblCaption->Text2 = ExtractFileName(CItem.OpenFileName) +
			//" (" + String(z+1) + "/" + batchFiles + ")";
		}
		//Form1->Caption = "SCX - (File " + String(z+1) + " of " + batchFiles + ")";
	}

	else{
		//Form1->LblCaption->Text1 = "Scx Hybrid Mode - ";
		//Form1->LblCaption->Text2 = ExtractFileName(CItem.OpenFileName);
		//gui.NameCaption(CItem.OpenFileName);
	}

	sys.GetFileSize(in, true);

	// Check if file is too small to scan
	if (CItem.FileSize < 20)
	{
		fclose(in); //++z; continue;
	}


	//-------------
	// Buffer Calc
	//-------------
	int bufferSize = CItem.FileSize;
	int availMem = sys.GetAvailPhyMem();
	while (bufferSize > 2097152 || bufferSize > (int)availMem >> 2) bufferSize >>= 1;

	gui.Status("SCX - Available Mem: " + str.SizeToStr(availMem), 1000);
	gui.Status("SCX - Buf Size: " + str.SizeToStr(bufferSize), 1000);
	gui.Status("SCX - Allocating Memory..", 0);

	//// Small
	//if (!Form1->ComboBufSize->ItemIndex && standAlone)
	//{
	//	bufferSize = CItem.FileSize >> 3;
	//		while (bufferSize > availMem >> 2) bufferSize >>= 3;
	//}
	//// Medium
	//else if (Form1->ComboBufSize->ItemIndex == 1 || !standAlone)
	//{
	//	bufferSize = CItem.FileSize >> 2;
	//		while (bufferSize > availMem >> 1) bufferSize >>= 2;
	//}
	//// Large
	//else
	//{                                    
	//	bufferSize = CItem.FileSize >> 1;
	//		while (bufferSize > availMem) bufferSize >>= 1;
	//}

	//if (standAlone)
	//	Form1->LblAudioVal->Caption = sys.CalcSize((ulong)bufferSize);

	//_heapmin; //Release unused heap areas (Any point?)
	uchar* buffer;

	if (! (buffer = sys.MemoryAlloc((ulong)bufferSize, in)) )
	{
		//++z;
		//continue;
		gui.Error("Error Allocating Memory", 1000, 0);
	}

	//if (FileSize >= 10240000)
	//{
		//gui.Open("", "", "Scanning...");
		//gui.ProgressReset();
		//gui.ProgressStepSize(100 / (CItem.FileSize / bufferSize));
	//}
		//gui.ProgressCaption("Scanning..");
		//gui.ProgressCaptionUpdate();



	fseek(in, 0, SEEK_SET);

	//----------------
	// Header Scanner
	//----------------
	//int loops = FileSize / bufferSize;
	//int posSubtract = (bufferSize / loops) >> 2;
	uint posSubtract = (CItem.FileSize / bufferSize) >> 1;

	//msg.info(posSubtract);
	gui.Status("SCX - Scanning For Known Formats..", 0);

	int x = 0, pos = 0;
	while (!feof(in)) // && ExtractDlg->Showing
	{
		//Application->ProcessMessages();

		fseek(in, pos, 0);
		fread(buffer, 1, bufferSize, in);

		while (!feof(in) && x <= bufferSize)
		{
							//msg.info(buffer[x]);//msg.info(buffer[x+1]);
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
				//Form1->LblFilesVal->Caption = FItem.Offset.size();
				break;
			}

		++pos, ++x;
		}// End inner while loop

	//gui.ProgressStep();
	}// End outer while loop

	sys.MemoryDel();

	CItem.Files = (int)FItem.Offset.size();

	//if (!standAlone){
	//	Form1->LblCaption->Text1 = maincaption1;
	//	Form1->LblCaption->Text2 = maincaption2;
	//	Form1->EkImgProgressBar->Text = progresscaption;
	//}

	if (CItem.Files > 0){


	// Calc filechunks
	for (int x = 0; x < CItem.Files; ++x)
	{
		// Xwb
		if (FItem.Ext[x] == ".xwb")
		{
			GetXwbSize(in, x);
		}
		// Wav
		else if (FItem.Ext[x] == ".wav")
		{
			FItem.Chunk.push_back(sys.RiffSize(in, FItem.Offset[x]));
		}
		else if (FItem.Ext[x] == ".wma")
		{
			uint size = sys.WmaSize(in, FItem.Offset[x]);

			//if (size < 52428800)// 50Mb
				FItem.Chunk.push_back(size);
			//else
			//	FItem.Chunk.push_back(0);
		}
		// Ogg
		else if (FItem.Ext[x] == ".ogg")
		{
			GetOggSize(in, x);
		}
		// End ogg filesize calc

		// Bmp filesize calc
		else if (FItem.Ext[x] == ".bmp")
		{
			int size;
			fseek(in, FItem.Offset[x] + 2, 0);
			fread(&size, 4, 1, in);
			FItem.Chunk.push_back(size);
		}
		// End Bmp filesize calc

		// Dds filesize calc
		else if (FItem.Ext[x] == ".dds")
		{
			GetDdsSize(in, x);
		}
		// End Dds filesize calc

		// Png filesize calc
		else if (FItem.Ext[x] == ".png")
		{
			GetPngSize(in, x);
		}
		// End Png filesize calc

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
	 //ShowMessage("end");
//------------------------------------------------------------------------------

	if (!standAlone)
	{
		if (CItem.Files == 1){
			FItem.FileName.push_back(CItem.DirName);
		}
		else
		{
			bool tidy = false;
			//bool tidy = Form1->ChkTidyName->Checked;
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
		//revert
//		if (!CItem.Files)
//		{
			//ShowMessage(maincaption1+maincaption2);
//		}
//		else
//		{
//			gui.Finish();
//			gui.ProgressCaption("");
//			gui.ProgressReset();
//		}
		if (CItem.Files > 0)
			return 0;
		return -1;
	}

//------------------------------------------------------------------------------

	// DirName
	if (!sys.DirPathIsSet())
	{
		sys.DirPathSetSilent(ExtractFileDir(CItem.OpenFileName) + "\\" );
	}


	//if (Form1->EditFolName->Text != "")
	//{
	//	if (Form1->ChkTidyName->Checked)
	//		str.StrClean(Form1->EditFolName->Text);
	//	CItem.DirName = Form1->EditFolName->Text;
	//}
	//else
	//{
	//	DirName = ExtractFileName (OpenFileName);
	//	ushort extLength = ExtractFileExt (DirName).Length();
	//	DirName.SetLength(DirName.Length() -extLength +1);
	//}


	//if (Form1->ChkTidyName->Checked)
	//		str.StrClean(CItem.DirName);


	//if (ExtractDlg->Showing)
	CreateDirectory(string(sys.DirPathGet() + "\\" + CItem.DirName).c_str(), NULL);


	//if (Form1->EditRenFiles->Text == "")
	//	OpenFileName = ExtractFileName (OpenFileName);
	//else

	//if (Form1->EditRenFiles->Text != "")
	//	CItem.OpenFileName = Form1->EditRenFiles->Text;

	// Name output files
	//if (CItem.Files == 1){
	//	FileName.push_back(sys.DirGet() + "\\" + Ext[0]);
	//}
	//else
	//{

	//bool tidy = Form1->ChkTidyName->Checked;
	bool tidy = false;

	for (int x = 0; x < CItem.Files; ++x)
	{
		string name;
		str.AutoNumScx(tidy, name, x, CItem.Files);
		FItem.FileName.push_back
		(
			sys.DirPathGet() + "\\" + CItem.DirName + "\\" + name
		);
	}
	//}

	//if (Form1->EditRenFiles->Text != "")
	//{
	//	String text = Form1->EditRenFiles->Text;
	//	for (int x = 0; x < CItem.Files; ++x)
	//	{
	//		FItem.FileNameBak.push_back(CItem.DirName);
	//		if (tidy)
	//		{
	//			str.StrClean(text);
	//			str.StrClean(FItem.FileNameBak[x]);
	//		}
	//		FItem.FileName[x] =
	//		sys.DirGet() + "\\" + CItem.DirName + "\\" +
	//		str.StringReplacer(text, x, CItem.Files) + FItem.Ext[x];
	//		//ShowMessage(FItem.FileName[x]);
	//	}
	//	sys.SetDirName();
	//}

	//{
	//	text = text.Delete(text.Pos("%"), 1);
	//	++percent;
	//}
	//if (percent!=0) FileName[x].Delete(text.Length(), percent);

	//gui.ProgressReset();
	//gui.ProgressStepSize((float short) 100 / CItem.Files);
	//gui.Open("", "", "Writing...");


	#ifdef SCANSPEEDTEST
	double time = sys.TimerStop();
	gui.FinishDisplay(CItem.Files, CItem.Files, time, "Scanned", "Finished");
	CItem.Files = 0;
	return 0;
	#endif


	// Ouput All Files
	for (int i = 0; i < CItem.Files; ++i, ++extracted, ++filesTotal)//&& ExtractDlg->Visible
	{
		//Application->ProcessMessages();

		//Form1->LblArchiveVal->Caption = extracted;
		//Form1->JvgProgress1->Caption = ExtractFileName(FileName[x]);
		//gui.ProgressCaptionBasic(ExtractFileName(FItem.FileName[x]), false);
		//gui.ProgressCaption("Writing: " + ExtractFileName(FItem.FileName[x]));
		//gui.ProgressStep();
		//Form1->JvgProgress1->Update();

		FILE* out = fopen (FItem.FileName[i].c_str(), "wb");

		if (! (buffer = sys.MemoryAlloc(FItem.Chunk[i], out)) )
			continue;

		fseek(in, FItem.Offset[i], 0);
		fread(buffer, 1, FItem.Chunk[i], in);

		try
		{
			fwrite(buffer, 1, FItem.Chunk[i], out);
		}
		catch(exception &e)
		{
			exception f = e;
			--extracted;
		}

		fclose(out);

		sys.MemoryDel();

	}// End output loop
	//Form1->LblArchiveVal->Caption = extracted;
	}// End if (CItem.Files > 0)
	else
	{
		if (!standAlone)
		{
			CItem.Files = 0;
			//Form1->LblCaption->Text1 = maincaption1;
			//Form1->LblCaption->Text2 = maincaption2;
			//Form1->EkImgProgressBar->Text = progresscaption;
			//ShowMessage(maincaption1 + maincaption2);
			//sys.ClearVariables();
			gui.Error("No recognised formats found", sys.Delay(true), in);
			//gui.ProgressReset();
			//gui.ProgressCaption("");
			return -1;
		}
		//Form1->LblArchiveVal->Caption = extracted;
		//gui.ProgressCaption(String (extracted) + " File/s Extracted");
		//Form1->Update();
	}
	sys.ClearVariables();
	//gui.ProgressCaption("");
	fclose(in);


	++z;
//}// End main while loop


double time = sys.TimerStop();
gui.FinishDisplay(extracted, filesTotal, time, "Extracted", "Finished");

//gui.ClearInfo();
//gui.ScxLblSetup();
CItem.ScxMode = false;
CItem.Loading = false;
CItem.Files = 0;
//Form1->Refresh();
return 0;

}// End Scan







