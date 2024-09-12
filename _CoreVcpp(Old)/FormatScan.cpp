


//=============================
//		FormatScan.cpp
//=============================

//#include <vcl.h>
#include "stdafx.h" 

#include "FormatScan.h"
#include "Supported.h"
#include "FileSpecs.h"
#include "Strings.h"
#include "SystemX.h"
#include "Gui.h"
#include "FormatHdrs.h"
#include "Helper.h"
#include "Scx.h"
//#pragma hdrstop

//#include "Main.h"
//#include "MediaPlayer.h"

#define HAS_NO_FILENAMES	0
#define HAS_FILENAMES		1	

#define HAS_NO_AUDIO_INFO	0
#define HAS_AUDIO_INFO		1

#define HAS_NO_FOLDERS		0
#define HAS_FOLDERS			1

#define WAV_HEADER_NOT_REQ	0
#define WAV_HEADER_REQ		1

//===========================================================================

inline void SetFormatSpecs(const string& pFormat, const string& pLblFormat, const bool& pHasNames, 
						   const bool& pHasAudioInfo, const bool& pHasFolders,
						   const bool& pReqWavHeader){

CItem.Format = pFormat;	
gui.SetLblFileType(pLblFormat);
CItem.HasNames = pHasNames; 
CItem.HasAudioInfo = pHasAudioInfo; 
CItem.HasFolders = pHasFolders;
CItem.NeedsWavHeader = pReqWavHeader;

}

//===========================================================================

bool FormatScan::Detect(FILE* in){

gui.Status("Detecting Format", 0);
Xwb360 = false;
Offset = 0;
char Name[128];
strcpy(Name, ToLower(ExtractFileName(CItem.OpenFileName)).c_str());

Ext = ExtractFileExt(CItem.OpenFileName);
Ext = str.ToLower(Ext);
fseek(in, 0, 0);
fread(Header, 22, 1, in);

//===========================================================================

// Adf - (GTA: Vice City)
if ( Ext == "adf" && Header[8] == 0x22 && Header[9] == 0x22 )
{
	SetFormatSpecs("adf", "Adf", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Adx - (Multi)
else if ( Ext == "adx" ) //Header[0] != 0x80 && Header[1] != 0x00
{
	SetFormatSpecs("adx", "Adx", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Afs - (Multi)
else if (  !memcmp(Header, "AFS", 3) && Ext == "afs" )
{
	SetFormatSpecs("afs", "Afs", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Arc - (Sims2)
else if ( Ext == "arc" )
{
	SetFormatSpecs("arc", "Arc", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Asf/Mus - (Multi)
else if ( (Ext == "asf" || Ext == "mus") && Header[0] == 0x53 && Header[1] == 0x43
		&& Header[2] == 0x48 && Header[3] == 0x6C)
{
	SetFormatSpecs("mus", "Mus", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Bif (Star Wars: KOTOR)
else if ( !memcmp(Header, "BIFFV1", 6) )
{
	SetFormatSpecs("bif", "Bif", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Big (CodeMasters) - (Colin McRae Rally 2005)
else if ( Header[9] == 0x8 && Header[13] == 0x8 && !memcmp(Header, "BIGF", 4) )
{
	SetFormatSpecs("bigcode", "Big (CM)", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Big (EA) - (Fight Night Round 2)
else if ( Header[9] == 0x0 && Header[13] == 0x0 && !memcmp(Header, "BIGF", 4) )
{
	SetFormatSpecs("bigea", "Big (EA)", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

#ifdef DEBUG
// Bor (Senile) - (Beats Of Rage)
else if ( !memcmp(Header, "BOR ", 4) )
{
	SetFormatSpecs("bor", "Bor", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}
#endif

//===========================================================================

// Cab - (Forza)
else if ( Ext == "cab"
	&& Header[0] == 0xAA && Header[1] == 0xAA && Header[2] == 0xC0)
{   
	SetFormatSpecs("cabforza", "CabForza", 
		HAS_FILENAMES, HAS_NO_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// DatRoller (RollerCoaster Tycoon)
else if (Ext == "dat" && Header[0] == 0x30)
{
	SetFormatSpecs("datroller", "DatRoller", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Fsb3 - Flatout 2,  BioShock (PC)
else if ( !memcmp(Header, "FSB3", 4) )
{
	SetFormatSpecs("fsb", "Fsb3", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Hot- (Voodoo Vince)
else if ( !memcmp(Header, "HOT", 3) )
{
	fseek(in, 32, 0);
	fread(Header, 1, 1, in);
	if (Header[0] != 0x48)
	{
		//gui.Error("Error: Only audio hot containers are supported",
		sys.Delay(false);
		fclose(in);
		return false;
	}
	
	SetFormatSpecs("hot", "Hot", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Hwx- (Star Wars Episode III: Revenge Of The Sith)
else if ( Ext == "hwx" )
{
	SetFormatSpecs("hwx", "Hwx", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Lug - (Fable)
else if (Ext == "lug" )
{
	SetFormatSpecs("lug", "Lug", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Map - (Halo / Stubbs)
else if ( !memcmp(Header, "daeh", 4) )
{
	fseek(in, 4, 0);
	fread(Header, 1, 1, in);
	if (Header[0] == 0x7)
	{
		//gui.Error("Error: PC Halo maps arn't supported", sys.Delay(false), in);
		return false;
	}
	SetFormatSpecs("maphalo", "Map (Halo)", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Mpk - (Mission Impossible)
else if ( !memcmp(Header, "MPAK", 4) )
{
	SetFormatSpecs("mpk", "Mpk", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Msx - (MK Deception)
else if (Ext == "msx" ) //header[0] == 0x02
{
	SetFormatSpecs("msx", "Msx", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// PakGoblin - (Goblin Commander)
else if (!memcmp(Header, "RIFF", 4) && !memcmp(Name, "audxbox", 4))
{
	SetFormatSpecs("pakgoblin", "PakGob", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
	//gui.Status("detected", 0);
}

//===========================================================================

#ifdef DEBUG
// PakDream - (Dreamfall: The Longest Journey)
else if ( !memcmp(Header, "tlj_", 4) )
{
	SetFormatSpecs("pakdream", "PakDrm", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}
#endif

//===========================================================================

// Piz - (Mashed)
else if ( !memcmp(Header, "PIZ", 3) )
{
	SetFormatSpecs("piz", "Piz", 
		HAS_FILENAMES, HAS_NO_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Pod - (BloodRayne 2)
else if ( !memcmp(Header, "POD3", 4) )
{
	SetFormatSpecs("pod", "Pod", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Rcf (Radcore) - (Multi)
else if ( !memcmp(Header, "RADCORE", 4) )
{
	SetFormatSpecs("rcf", "Rcf", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Rez - (Mojo!)
else if ( Header[0] == 0x80 && Header[4] == 0x8C && Ext == "rez" )
{
	SetFormatSpecs("rez", "Rez", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Rfa - (RalliSport Challenge)
else if ( !memcmp(Header, "Refractor", 6) ) //!memcmp(ext, ".rfa", 4)
{
	SetFormatSpecs("rfa", "Rfa", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Rws - (Multi)
else if ( Ext == "rws" )//else if (!memcmp(header, rws.header, 5))
{
	// Compatiblity Check
	if (Header[20] == 0x0E && Header[21] == 0x00)
	{
		//if ( !(header[0] == 0x09 && header[1] == 0x08) )
		//{
			//gui.Error("Error: Unsupported Rws Type", sys.Delay(false), in);
			return false;
		//}
	}
	//Contains names but not are yet being parsed
	SetFormatSpecs("rws", "Rws", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);

	//if (header[20] == 0x0E)
	//	iRws.Read(in, 2);
	//else
}

//===========================================================================

// Samp - (Namco Museum 50th Anniversary)
else if ( Header[0] == 0x69 && Header[1] == 0x00 && Ext == "samp" )
{
	SetFormatSpecs("samp", "Samp", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Sh4 - (Silent Hill 4)
else if ( Header[0] == 0x53 && Header[1] == 0x48 && Header[2] == 0x34 )
{
	SetFormatSpecs("sh4", "Sh4", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Sr - (Multi)
else if ( Header[15] == 0x20 && Header[16] == 0x20
		&& Ext == "sr" ) // && header[17] == 0x20
{
	SetFormatSpecs("sr", "Sr", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Stx - (Kakuto Chojin)
else if ( !memcmp(Header, "STHD", 4) )
{
	SetFormatSpecs("stx", "Stx", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Sxb/Vxb - (Ford Racing 3)
else if ( Ext == "sxb" || Ext == "vxb" )
{
	fseek(in, 4, 0);
	fread(&Offset, 4, 1, in);

	fseek(in, Offset, 0);
	fread(Header, 4, 1, in);

	if (memcmp(Header, "WBND", 4)){
		//gui.Error("Sxb/Vxb fault", sys.Delay(false), in);
		return false;
	}
	SetFormatSpecs("xwb", "Xwb", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Wad - (Disney's Extreme Skate Adventure)
else if ( Ext == "wad" && !memcmp(Header, wma.header, 16) )
{
	SetFormatSpecs("wad", "Wad", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// WavRaw - (Testing only -> Used to write headers on raw wav's)
#ifdef DEBUG
else if ( Ext == "eksz" )
{
	SetFormatSpecs("wavraw", "Wav", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);

	//a_str(" (EkszBox).wav")

	//OpenFileName = OpenFileName.SetLength(OpenFileName.Length() - 5) +
	//" (EkszBox).wav";

	//Form1->OpenDialog->FileName = OpenFileName;
	//in = fopen(OpenFileName.c_str(), "rb");

	//EkszBox::Formats::Wav iWav; iWav.Read(in);

	//FileSize = sys.GetFileSize(in);

	//gui.Status("Finished Processing", 0, true);
	//return false;
}
#endif

//===========================================================================

// Wmapack - (Forza Motorsport)
else if ( Header[0] == 0x01 && Header[5] == 0x08 && Ext == "wmapack" )
{
	SetFormatSpecs("wmapack", "Wmapack", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

// Wpx - (Capcom Vs Snk 2 EO)
else if ( Ext == "wpx" && Header[2] == 0x00 )
{
	SetFormatSpecs("wpx", "Wpx", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Xbp - (Multi)
else if ( Ext == "xbp" )
{
	SetFormatSpecs("xbp", "Xbp", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
}

//===========================================================================

//#ifdef DEBUG
//// Xpr - (Multi)
//else if  ( !memcmp(header, "XPR1", 4) ) //!memcmp(ext, ".xwc", 4)
//{
//	SetFormatSpecs("xpr", "Xpr", 
//		HAS_FILENAMES, HAS_NO_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
//}
//#endif

//===========================================================================

#ifdef DEBUG
// Xiso - (Multi)
else if ( Ext == "iso" )
{
	fseek(in, 65536, 0);
	fread(Header, 20, 1, in);

	// This works backwards for some reason??
	if ( memcmp(Header, "MICROSOFT*XBOX*MEDIA", 20) )
	{
		//gui.Error("Unsupported Iso Type", sys.Delay(false), in);
		return false;
	}


	//gui.Error("Xiso support disabled until it's more stable",
	//sys.Delay(false), NULL);
	//return false;

	SetFormatSpecs("xiso", "Xbox Iso", 
		HAS_FILENAMES, HAS_NO_AUDIO_INFO, HAS_FOLDERS, WAV_HEADER_NOT_REQ);

	//Supported fmt;
	//if (fmt.XisoRead(in) == -1) return false;
}
#endif

//===========================================================================

// Xwb - (Multi)
else if ( Ext == "xwb" || !memcmp(Header, "WBND", 4) )
{

if (memcmp(Header, "WBND", 4) && memcmp(Header, "DNBW", 4))
{
	//gui.Status("Scanning For Xwb Header", 0, true);

	// Scan for xbox xwb signature
	Offset = sys.HeaderScan(0, sys.GetFileSize(in, false), "WBND", 4, in);

	// Xwb signature found
	if (Offset == -1)
	{
		//gui.Status("Scanning For Xwb360 Header", 0, true);
		Offset = sys.HeaderScan(0, sys.GetFileSize(in, false), "DNBW", 4, in);
		if (Offset != -1)
		{
			Xwb360 = true;
		}
		else
		{
			//gui.Error("Xwb header not found", sys.Delay(false), in);
			return false;
		}
	}
}
else if (!memcmp(Header, "DNBW", 4)) Xwb360 = true;

SetFormatSpecs("xwb", "Xwb", 
	HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);

}

//===========================================================================

// Xwc - (Chronicles Of Riddick: Escape From Butcher Bay)
else if  ( !memcmp(Header, "MOS DATA", 5) ) //!memcmp(ext, ".xwc", 4)
{
	if ( Ext == "xtc")
	{
		//gui.Error("Incompatible Format", sys.Delay(false), in);
		return false;
	}
	SetFormatSpecs("xwc", "Xwc", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

#ifdef DEBUG
// Sz - (OutRun 2006 - Coast 2 Coast)
else if ( Ext == "sz" )
{
	SetFormatSpecs("sz", "Sz", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}
#endif

//===========================================================================

// Zsm - (Multi)
else if  ( !memcmp(Header, "ZSNDXBOX", 8) )
{
	SetFormatSpecs("zsm", "Zsm", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Zwb- ((Unknown)
else if ( Ext == "zwb" )
{
	SetFormatSpecs("zwb", "Zwb", 
		HAS_NO_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_REQ);
}

//===========================================================================

// Misc formats
else
{

#ifdef DEBUG
// Misc - (Multi)
if ( Ext == "csc" )
{
	SetFormatSpecs("csc", "Csc", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
	return true;
}
#endif

//===========================================================================

// Wav - (Multi)
#ifndef DEBUG
	if (!memcmp(Header, "RIFF", 4) && Header[8] == 0x57 && Ext != "wav")
#endif
#ifdef DEBUG
	if (!memcmp(Header, "RIFF", 4) && Header[8] == 0x57)
#endif
		//&& memcmp(Ext, ".wav", 4) && memcmp(Ext, ".csc", 4)
{
	SetFormatSpecs("wav", "Wav", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
	return true;
}

//===========================================================================

// Wma - (Multi)  (Used for WmaSpecs testing)
#ifndef DEBUG
	else if (!memcmp(Header, wma.header, 16) && Ext != "wma")
#endif
#ifdef DEBUG
	else if (!memcmp(Header, wma.header, 16))
#endif
{
	SetFormatSpecs("wma", "Wma", 
		HAS_FILENAMES, HAS_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
	return true;
}

//===========================================================================

// Gta5 - (Gta: San Andreas)
uchar chk;
fseek(in, 7984, 0);
fread(&chk, 1, 1, in);

fseek(in, 8016, 0);
fread(Header, 4, 1, in);
if (Ext == "gta5" || (!memcmp(Header, "OggS", 4) && chk == 0xFF) )
{
	SetFormatSpecs("gta5", "Gta5", 
		HAS_NO_FILENAMES, HAS_NO_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);
	return true;
}

//===========================================================================

else  // Incompatible
{
	// UnCommentMeSoon
	//if (Form1->ChkAutoScx->Checked)
	//{
		SetFormatSpecs("unknown", "Unknown", 
			HAS_NO_FILENAMES, HAS_NO_AUDIO_INFO, HAS_NO_FOLDERS, WAV_HEADER_NOT_REQ);

		return true;
	//   }

	//++CItem.IncompatileFmts;
	// UnCommentMeSoon
	//if (!MediaPlayer.PlayListGetSize()){ //test
	//	gui.Error("Incompatible Format, try using SCX", sys.Delay(true), in);
	//	gui.ProgressReset();
	//}
	//return false;
}

//===========================================================================

}

gui.Status("Recognized Format Found: " + CItem.Format, 0);
return true;

}
//End FormatScan::Detect
//===========================================================================


// FormatScan - Parse
//-------------------

bool FormatScan::Parse(FILE* in){

Supported fmt;
gui.Status("Reading Format.." , 0);

if (CItem.Format == "adf")
	fmt.AdfRead(in);
//else if (CItem.Format == "adx")
//	fmt.AdxRead(in);
else if (CItem.Format == "afs" && !fmt.AfsRead(in))
	return false;
else if (CItem.Format == "arc")
	fmt.ArcRead(in);
//else if (CItem.Format == "mus" && !fmt.AsfMusRead(in))
//	return false;
else if (CItem.Format == "bif")
	fmt.BifRead(in);
else if (CItem.Format == "bigcode")
	fmt.BigCodeRead(in);
else if (CItem.Format == "bigea" && !fmt.BigEaRead(in))
	return false;
//#ifdef DEBUG
//else if (CItem.Format == "bor")
//	fmt.BorRead(in);
//#endif
else if (CItem.Format == "cabforza")
	fmt.CabForzaRead(in);
else if (CItem.Format == "datroller")
	fmt.DatRollerRead(in);
else if (CItem.Format == "fsb")
	fmt.FsbRead(in);
else if (CItem.Format == "hot")
	fmt.HotRead(in);
else if (CItem.Format == "hwx")
	fmt.HwxRead(in);
else if (CItem.Format == "lug")
	fmt.LugRead();
else if (CItem.Format == "maphalo" && !fmt.MapHaloRead(in))
	return false;
else if (CItem.Format == "mpk")
	fmt.MpkRead(in);
else if (CItem.Format == "msx")
	fmt.MsxRead(in);
else if (CItem.Format == "pakgoblin" && !fmt.PakGoblinRead(in))
	return false;
//#ifdef DEBUG
//else if (CItem.Format == "pakdream")
//	fmt.PakDreamRead(in);
//#endif
else if (CItem.Format == "piz")
	fmt.PizRead(in);
else if (CItem.Format == "pod")
	fmt.PodRead(in);
else if (CItem.Format == "rcf")
	fmt.RcfRead(in);
else if (CItem.Format == "rez")
	fmt.RezRead(in);
else if (CItem.Format == "rfa")
	fmt.RfaRead(in);
else if (CItem.Format == "rws" && !fmt.RwsRead(in))
	return false;
else if (CItem.Format == "samp" && !fmt.SampRead())
	return false;
else if (CItem.Format == "sh4")
	fmt.Sh4Read(in);
else if (CItem.Format == "sr")
	fmt.SrRead(in);
else if (CItem.Format == "stx")
	fmt.StxRead(in);
//#ifdef DEBUG
//else if (CItem.Format == "sz" && !fmt.XwbRead(in, Offset, Xwb360))
//	return false;
//#endif
else if (CItem.Format == "wad" && !fmt.WadRead())
	return false;
//#ifdef DEBUG
//else if (CItem.Format == "wavraw")
//	fmt.WavRawRead(in);
//#endif
//else if (CItem.Format == "wav" && !fmt.WavRead(in))
//	return false;
//else if (CItem.Format == "wmapack")
if (CItem.Format == "wmapack")
	fmt.WmapackRead(in);
else if (CItem.Format == "wpx")
	fmt.WpxRead(in);
//else if (CItem.Format == "xbp" && !fmt.XbpRead())
//	return false;
//#ifdef DEBUG
//else if (CItem.Format == "xiso" && !fmt.XisoRead(in))
//	return false;
//#endif
else if (CItem.Format == "xwb" && !fmt.XwbRead(in, Offset, Xwb360))
	return false;
else if (CItem.Format == "xwc")
	fmt.XwcRead(in);
else if (CItem.Format == "zsm")
	fmt.ZsmRead(in);
else if (CItem.Format == "zwb" && !fmt.XwbRead(in, 4, 0))
	return false;


//// Miscellaneous
//#ifdef DEBUG
//else if (CItem.Format == "csc" && !fmt.MiscRead())
//	return false;
//#endif
//else if (CItem.Format == "wma" && !fmt.WmaRead(in)) {
//	return false;
//}

else if (CItem.Format == "gta5" && !fmt.Gta5Read())
	return false;
// Scx
else if (CItem.Format == "unknown" && !fmt.MiscRead())
	return false;

return true;

}

