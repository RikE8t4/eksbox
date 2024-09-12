


//=============================
//		Settings.cpp
//=============================

#include <stdafx.h>
#pragma hdrstop
#include "Settings.h"

#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Strings.h"

#include "Main.h"
#include "Open.h"

//==============================================================================

struct{
	bool 	list;
	bool 	tidyNames;
	bool 	xctDbase;
	bool 	autoEditClear;
	bool 	autoGuiClear;
	bool	autoScx;
	bool	onTop;
	bool	screenSnap;
	bool	showHints;
	bool	fadeFx;
	bool	scrollTxt;
	bool	animatedProgressBar;
	bool	speed;
	bool	autoXpand;
	bool	loopPlayList;
	bool	continuePlayback;
	int		saveVer;
	int		bufSize;
	int 	format;
	int 	channels;
	int 	sampleRate;
	int 	bits;
	int 	formHeight;
	int 	formWidth;
	int  	xctEntries;
	float	trackBarPos;
	float	progressBarPos;
	uchar 	renFiles[100];
	uchar 	folName[100];
	uchar	openDlgDir[MAX_PATH];
	uchar	openDlgFileSwapDir[MAX_PATH];
	uchar	openDlgScxDir[MAX_PATH];
	uchar	playingFileName[MAX_PATH];
}settings;

//==============================================================================

void TSettings::Load(){

gui.mMinHeight = 112;
gui.mMaxHeight = 375;
gui.mMinWidth = 433;
gui.mMidWidth = 447;
gui.mMaxWidth = 750;

String prevDir = GetCurrentDir();
SetCurrentDir(ExtractFileDir(Application->ExeName));
FILE* load = fopen(ININAME, "rb");

bool set = 0;
if (!load)
// Set default settings if loading fails/dif save ver
{
	defaultSet:
	settings.list 					= true;
	settings.tidyNames 				= true;
	settings.xctDbase 				= true;
	settings.autoEditClear 			= true;
	settings.autoGuiClear 			= false;
	settings.autoScx				= true;
	settings.onTop					= true;
	settings.screenSnap				= true;
	settings.showHints           	= true;
	settings.fadeFx           		= true;
	settings.scrollTxt              = true;
	settings.animatedProgressBar    = true;
	settings.speed					= false;
	settings.autoXpand				= false;
	settings.loopPlayList			= false;
	settings.continuePlayback       = false;
	//settings.saveVer				= SAVEVER;
	settings.bufSize 				= 1;
	settings.format 				= 2;
	settings.channels 				= 2;
	settings.sampleRate				= 8;
	settings.bits					= 3;
	settings.formHeight				= gui.mMinHeight;
	settings.formWidth				= gui.mMinWidth;
	settings.xctEntries    			= NULL;
	settings.trackBarPos           	= FORM->EkszTrackBar->Max;
	settings.progressBarPos        	= FORM->EkImgProgressBar->Max;
	*settings.renFiles 				= NULL;
	*settings.folName 				= NULL;
	*settings.openDlgDir			= NULL;
	*settings.openDlgFileSwapDir	= NULL;
	*settings.openDlgScxDir			= NULL;
	*settings.playingFileName       = NULL;
	set = true;
}
// Read settings
else fread(&settings, sizeof(settings), 1, load);
							

if (!set)
{
	if ( (settings.saveVer !=  SAVEVER) ||
	//if (
		(settings.bufSize >= 3)
	|| (settings.formWidth != gui.mMinWidth && settings.formWidth != gui.mMidWidth
		&& settings.formWidth != gui.mMaxWidth) )
	{
		//gui.MsgError("Save File Is Old/Corrupted");
		goto defaultSet;
	}
}


// Apply settings
FORM->ChkList->Checked 				= settings.list;
FORM->ChkTidyName->Checked 			= settings.tidyNames;
FORM->ChkXct->Checked 				= settings.xctDbase;
FORM->ChkAutoEditClr->Checked 		= settings.autoEditClear;
FORM->ChkAutoGuiClr->Checked 		= settings.autoGuiClear;
FORM->ChkAutoScx->Checked			= settings.autoScx;
FORM->ChkSpeed->Checked				= settings.speed;
FORM->ChkAutoXpand->Checked			= settings.autoXpand;
FORM->BtnLoopPlayList->Checked		= settings.loopPlayList;
FORM->LblXctEntriesVal->Caption  	= settings.xctEntries;
FORM->EkszTrackBar->Position 		= settings.trackBarPos;

gui.AutoXpandSet(settings.autoXpand);
gui.SpeedHackEnable(settings.speed);
FORM->BtnLoopPlayListClick(NULL);

FORM->PopMenuOnTop->Checked			= settings.onTop;
FORM->PopMenuScreenSnap->Checked 	= settings.screenSnap;
FORM->PopMenuShowHints->Checked		= settings.showHints;
FORM->PopMenuFadeFx->Checked		= settings.fadeFx;
FORM->PopMenuScrollingText->Checked	= settings.scrollTxt;
FORM->PopMenuAnimatedProgressBar->Checked = settings.animatedProgressBar;

FORM->ComboBufSize->ItemIndex		= settings.bufSize;
FORM->ComboFormat->ItemIndex		= settings.format;
FORM->ComboChan->ItemIndex			= settings.channels;
if (settings.sampleRate > 8 || settings.sampleRate < 0)
	settings.sampleRate = 8;
FORM->ComboSmpRate->ItemIndex		= settings.sampleRate;
FORM->ComboBits->ItemIndex			= settings.bits;


// Gui height/width // Bad save dealwith
if (!set)
{
	if (settings.formHeight < gui.mMinHeight)
		settings.formHeight = gui.mMinHeight;
	else if (settings.formHeight > gui.mMaxHeight)
		settings.formHeight = gui.mMaxHeight;
}


if (settings.formHeight == gui.mMaxHeight && settings.formWidth == gui.mMidWidth)
	gui.MinWidthMode();
else if (settings.formHeight == gui.mMaxHeight && settings.formWidth == gui.mMaxWidth)
	gui.MaxWidthMode();
else //if (settings.formHeight == gui.MinHeight)
	gui.MiniHeightMode(false);

	
// (OLD): Due to TJvBackgroundImage bug, style needs to be set in Processing
// dlg constructor

if (FORM->PopMenuOnTop->Checked)
	FORM->FormStyle = fsStayOnTop;

Form1->ScreenSnap = Form1->PopMenuScreenSnap->Checked;
Application->ShowHint = FORM->PopMenuShowHints->Checked;
if (settings.fadeFx == false){
	FORM->PopMenuFadeFx->Checked = !FORM->PopMenuFadeFx->Checked; // Hackjob
	gui.FadeFx();
}

gui.ScrollTxtSet(FORM->PopMenuScrollingText->Checked);
gui.AnimatedProgressBarSet(FORM->PopMenuAnimatedProgressBar->Checked);


FORM->EditRenFiles->Text 				= (char*)settings.renFiles;
FORM->EditFolName->Text 				= (char*)settings.folName;
FORM->OpenDialog->FileName 				= "";
String dir 								= (char*)settings.openDlgDir;
FORM->OpenDialog->InitialDir			= dir;
dir 									= (char*)settings.openDlgScxDir;
FORM->OpenDialogScx->InitialDir			= dir;
dir 									= (char*)settings.openDlgFileSwapDir;
FORM->OpenDialogFileSwap->InitialDir	= dir;

fclose(load);
SetCurrentDir(prevDir);

} // End load settings


//==============================================================================

//----------
//	Save
//----------

void TSettings::Save(){

SetCurrentDir(ExtractFileDir(Application->ExeName)); // Set dir to same as exe

// Load the settings to get valid data into the struct (Needed for openDlgDir)
FILE* load = fopen(ININAME, "rb");

if (load) fread(&settings, sizeof(settings), 1, load);
fclose(load);

FILE* save = fopen(ININAME, "wb");

if (!save)
{
	gui.MsgError("Error creating " + string(ININAME));
	return;
}

settings.list 			= FORM->ChkList->Checked;
settings.tidyNames 		= FORM->ChkTidyName->Checked;
settings.xctDbase 		= FORM->ChkXct->Checked;
settings.autoEditClear 	= FORM->ChkAutoEditClr->Checked;
settings.autoGuiClear 	= FORM->ChkAutoGuiClr->Checked;
settings.autoScx		= FORM->ChkAutoScx->Checked;
settings.speed			= FORM->ChkSpeed->Checked;
settings.autoXpand   	= FORM->ChkAutoXpand->Checked;
settings.loopPlayList	= FORM->BtnLoopPlayList->Checked;
settings.onTop			= FORM->PopMenuOnTop->Checked;
settings.screenSnap 	= FORM->PopMenuScreenSnap->Checked;
settings.showHints 		= FORM->PopMenuShowHints->Checked;
settings.fadeFx         = FORM->PopMenuFadeFx->Checked;
settings.scrollTxt		= FORM->PopMenuScrollingText->Checked;
settings.animatedProgressBar = FORM->PopMenuAnimatedProgressBar->Checked;
settings.saveVer		= SAVEVER;
//strcpy(settings.saveVer, String(SAVEVER).c_str());
settings.bufSize 		= FORM->ComboBufSize->ItemIndex;
settings.format 		= FORM->ComboFormat->ItemIndex;
settings.channels 		= FORM->ComboChan->ItemIndex;
settings.sampleRate 	= FORM->ComboSmpRate->ItemIndex;
settings.bits			= FORM->ComboBits->ItemIndex;
settings.formHeight		= FORM->Height;
settings.formWidth		= FORM->Width;
settings.xctEntries = FORM->LblXctEntriesVal->Caption.ToInt();
settings.trackBarPos = FORM->EkszTrackBar->Position;
settings.progressBarPos = FORM->EkImgProgressBar->Position;


strcpy(settings.renFiles, FORM->EditRenFiles->Text.c_str());
strcpy(settings.folName, FORM->EditFolName->Text.c_str());


if (CItem.OpenFileName != "")
	strcpy(settings.openDlgDir, str.ExtractFileDir(CItem.OpenFileName).c_str());

if (FORM->OpenDialogScx->Files->Count)
strcpy(settings.openDlgScxDir,
		ExtractFileDir(FORM->OpenDialogScx->Files->operator [](0)).c_str());

if (FORM->OpenDialogFileSwap->Files->Count)
strcpy(settings.openDlgFileSwapDir,
		ExtractFileDir(FORM->OpenDialogFileSwap->Files->operator [](0)).c_str());


fwrite(&settings, sizeof(settings), 1, save);
fclose(save);

} // End save settings

//==============================================================================
