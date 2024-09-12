


//=============================
//		Open.cpp
//=============================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"

#ifdef __BORLANDC__
#include "Main.h"
#include "MediaPlayer.h"
#include "Extract.h"
#include "List.h"
#endif

#include "Open.h"
#include "FormatScan.h"
#include "Strings.h"
#include "Xct.h"

//==============================================================================

bool Open::DoOpen(const string& fileName){

	CItem.ScxMode = false;
	CItem.Finished = false;
	gui.ProgressReset();
	gui.ProgressFxOff();
	gui.ScxLblSetup();
	gui.ClearEditFolderName();
	gui.CancelAllSet(false);
	sys.TimerStart();

	int files = gui.OpenDlgGetTotalFiles();

	for (int z = 0; z < files && !gui.CancelAllGet(); ++z)
	{

	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	// Reset
	filex.ClearVariables();

	CItem.IniMatch = false;
	CItem.Error = false;
	CItem.Files = 0;

	#ifdef __BORLANDC__
	CItem.OpenFileName = Form1->OpenDialog->Files->operator [](z).c_str();
	#else
	CItem.OpenFileName = fileName;
	#endif

	sys.DirNameSet();
	FILE *in = fopen(CItem.OpenFileName.c_str(), "rb");

	if (!in){
		++CItem.IncompatibleFmts;
		gui.Error("Couldn't Open: "
		+ str.ExtractFileName(CItem.OpenFileName), sys.Delay(false), in);
		continue;
	}


	#ifdef _MEDIAPLAYER
	if (MediaPlayer.GetIsPlaying())
		CItem.FileSize = sys.GetFileSize(in, false);
	else
		CItem.FileSize = sys.GetFileSize(in, true);
	#else
	CItem.FileSize = sys.GetFileSize(in, true);
	#endif


	if (CItem.FileSize == 0) 
	{
		++CItem.IncompatibleFmts;
		gui.Error("Error: Zero Byte File", sys.Delay(false), in);
		continue;
	}

	// If container is playing and next file is a non container: crash!!
	// Solution: clear & start again
	  
	// Playable format?
	#ifdef _MEDIAPLAYER
	if (MediaPlayer.IsValidFormat(CItem.OpenFileName))
	{
		fclose(in);

		if (!MediaPlayer.PlayListGetSize()){
			MediaPlayer.Stop();
			gui.ClearInfo();
		}

		if (str.ExtractFileExt(CItem.OpenFileName) == ".m3u"){
			Supported fmt;
			fmt.M3uRead();
		}
		else
			MediaPlayer.PlayListAddEntry(CItem.OpenFileName.c_str());

		if (!MediaPlayer.PlayListGetSize()){
			gui.ClearInfo();
		}

		continue;
	}
	#endif


	#ifdef __BORLANDC__
	String 	archiveVal 	= Form1->LblArchiveVal->Caption,
			audioVal	= Form1->LblAudioVal->Caption,
			sizeVal		= Form1->LblSizeVal->Caption;
	#endif


	#ifdef _MEDIAPLAYER
	if (MediaPlayer.GetIsPlaying() && !MediaPlayer.GetIsPaused()) // Pause if playing
		MediaPlayer.Pause();
	#endif


	FormatScan fmtScan;

	// Format detect
	if (!fmtScan.Detect(in)){
		fclose(in);

		#ifdef _MEDIAPLAYER
		if (MediaPlayer.GetIsPlaying() && !MediaPlayer.GetIsPaused()) // Pause if playing
			MediaPlayer.Pause();
		#endif

		continue;
	}


	CItem.Loading = true;

	if (!sys.DirPathIsSet())
		sys.DirPathSetSilent();


	// App caption
	#ifdef _MEDIAPLAYER
	if (!MediaPlayer.GetIsPlaying())
	{
		gui.Status("Loading File..", 0, true);

		if (files > 1 && !MediaPlayer.PlayListGetSize())
			gui.Caption(str.ExtractFileName(CItem.OpenFileName) +
			  " (" + str.IntToStr(z +1)
			+ "/" + str.IntToStr(files) + ")" );
		else
			gui.NameCaption(CItem.OpenFileName.c_str());
	}
	#else
		gui.Status("Loading File..", 0, true);

		if (files > 1)
			gui.Caption(str.ExtractFileName(CItem.OpenFileName) +
			  " (" + str.IntToStr(z +1)
			+ "/" + str.IntToStr(files) + ")" );
		else
			gui.NameCaption(CItem.OpenFileName.c_str());
	#endif

	// Parse
	if (!fmtScan.Parse(in) || CItem.Error || CItem.Files <= 0){

		#ifdef _MEDIAPLAYER
		if (MediaPlayer.GetIsPlaying())
		{
			gui.ProgressSetMax(MediaPlayer.GetDuration()); // Revert progressbar
			if (MediaPlayer.GetIsPlaying() && MediaPlayer.GetIsPaused()) // UnPause if playing
				MediaPlayer.Pause();

			Form1->LblArchiveVal->Caption	= archiveVal;
			Form1->LblAudioVal->Caption		= audioVal;
			Form1->LblSizeVal->Caption		= sizeVal;
		}
		#endif

		fclose(in);
		continue;
	}

	#ifdef _MEDIAPLAYER
	if (MediaPlayer.GetIsPlaying())
	{
		gui.ProgressSetMax(MediaPlayer.GetDuration()); // Revert progressbar
			if (MediaPlayer.GetIsPlaying()) // UnPause if playing
				MediaPlayer.Pause();
	}
	MediaPlayer.PlayListClear();
	#endif

	// Close input stream as we are not using a global instance
	fclose(in);



	#ifndef DEBUG
	// If all read file chunks added together are greater than filesize
	// file wasn't read correctly
	__int64 total = 0;
	for (int x = 0; x < CItem.Files; ++x)
		total += FItem.Chunk[x];

	if (total > CItem.FileSize)
	{
		++CItem.IncompatibleFmts;
		gui.Error(" File wasn't read correctly", sys.Delay(false), in);
		continue;
	}
	#endif


	// Give archives without extensions theresuch
	if (!sys.GetOpenFileNameExtLen())
	{
		str.RenameFile(CItem.OpenFileName, CItem.OpenFileName + "." + CItem.Format);
		CItem.OpenFileName = CItem.OpenFileName + "." + CItem.Format;
		#ifdef __BORLANDC__
		Form1->OpenDialog->Files->operator [](z) = CItem.OpenFileName.c_str();
		#endif
		#ifdef _MEDIAPLAYER
		if (files > 1 && !MediaPlayer.PlayListGetSize())
		#else
		if (files > 1)
		#endif
			gui.Caption(str.ExtractFileName(CItem.OpenFileName) +
			" (File " + str.IntToStr(z +1)
			+ " of " + str.IntToStr(files) + ")" );
		else
			gui.NameCaption(CItem.OpenFileName.c_str());
	}

	// Show num of files contained therein
	gui.LabelFilesValSet(str.IntToStr(CItem.Files), false);

	// Make filenames if none exist
	if (!CItem.HasNames)
	{
		int x = 0;
		while(x < CItem.Files)
		{
			FItem.FileName.push_back("");
			str.AutoNumList(0, FItem.FileName[x], x, CItem.Files);
			++x;
		}
	}

                
	// Remove file ext's & lowercasify theresuch
	for (int x = 0; x < CItem.Files; ++x)
	{
		FItem.FileName[x] = str.RemoveFileExt(FItem.FileName[x]);
		if (!FItem.Ext.empty())
			FItem.Ext[x] = str.ToLower(FItem.Ext[x]);
	}


	// Display audio type on AudioLabel (based on extension)
	gui.ExtInfo(CItem.Files, FItem.Ext);

	// Make a copy of fileName vector array for reverting from renamed files
	FItem.FileNameBak = FItem.FileName;

	// Xct - (Custom filename database format)
	if (gui.ChkXct())
	{
		Xct x; 
		int entries;
		x.FindMatch(CItem.DirName, CItem.IniMatch, entries);
	}
	else if (gui.EditRenFilesTxtGet() != "")// Rename files according to editbox
	{
		string text = gui.EditRenFilesTxtGet();
		for (int x = 0; x < CItem.Files; ++x)
			FItem.FileName[x] =
			str.StringReplacer(text, x, CItem.Files);
	}


	// CleanNames
	if (gui.ChkTidyName() && !CItem.IniMatch)
	{
		int x = 0;
		while (x < CItem.Files)
		{
			str.StrClean(FItem.FileName[x]);
			++x;
		}
	}


	if (CItem.IniMatch)
		gui.Caption(CItem.DirName.c_str());

	// Form specs
	if (gui.FormWidthGet() != gui.mMaxWidth && gui.AutoXpandGet() == true)
	{
		gui.FormVisibleSet(false);
		gui.NormHeightMode(false);
		gui.MaxWidthMode();
		gui.CenterForm();
		gui.FormVisibleSet(true);
	}

	gui.LoadList();

	if (files == z + 1)
		CItem.Finished = true;

	CItem.Loading = false;


	// No batch mode
	if (files == 1)
	{
		gui.Status("File Loaded In " + str.IntToStr(sys.TimerStop()) + " Sec/s", 0, false);
	}
	// Batch standard
	else
	{
		Supported fmt; fmt.StdExtract(-1);
	}



	}// End main while batch loop


	CItem.Loading = false;
	gui.Close();
	#ifdef _MEDIAPLAYER
	if (CItem.Error && !MediaPlayer.PlayListGetSize() && !MediaPlayer.GetIsPlaying())
	#else
	if (CItem.Error)
	#endif
	{
		gui.ClearInfo();
		int compat = files - CItem.IncompatibleFmts;
		gui.Caption("Finished");
		gui.Error(str.IntToStr(compat) + " Compatible Format/s Found",
		sys.Delay(true), NULL);
	}

	CItem.IncompatibleFmts = 0;

	if (!gui.CancelAllGet() && files != 1)
	{
		gui.ProgressUpdate();
	}

	#ifdef _MEDIAPLAYER
	if (MediaPlayer.PlayListGetSize())
	{
		Form1->OpenDialog->Files->Clear();
		if (!MediaPlayer.GetIsPlaying()) // Needs a fix
			MediaPlayer.PlayListLoad();
	}
	#endif

	return true;

}

//==============================================================================
