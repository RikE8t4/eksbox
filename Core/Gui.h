#ifndef GUI_H
#define GUI_H

//=============================
//		Gui.h
//=============================

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"

#ifdef __BORLANDC__
#include "Main.h"

#define SKINDIR "\\Graphics\\"

#define CHKOUT 		SKINDIR 	"cbcOut.bmp"
#define CHKOVER 	SKINDIR 	"cbcOver.bmp"
#define CHKDOWN 	SKINDIR 	"cbcDown.bmp"
#define UCHKOUT 	SKINDIR 	"cbuOut.bmp"
#define UCHKOVER 	SKINDIR 	"cbuOver.bmp"

#define BTNOPTOUT 	SKINDIR 	"btnoptOut.bmp"
#define BTNOPTOVER 	SKINDIR 	"btnoptOver.bmp"
#define BTNOPTDOWN 	SKINDIR 	"btnoptDown.bmp"

#define FORM Form1
#endif


class Gui{

private:

	//string 	XctCaption;
	string 	mCaption1Bak;
	string	mCaption2Bak;
	string 	mProgessCaption;
	string 	mFontPath;
	string 	mAppDir;
	bool 	mCancelAll;
	bool 	mDragAndDrop;
	bool 	mListOn;
	bool 	mAutoXpand;
	bool 	mSpeedOn;
	bool 	mShowOptions;
	bool	mScrollTxt;
	bool	mAnimatedProgressBar;
	float 	mInc;
	float 	mIncP;
	double 	mCursorTime;


public:

	Gui();
	~Gui();
	string GetAppDir() { return mAppDir; }
	void Msg (const string& msg);
	void MsgA (const string& msg, const char* caption);
	void MsgInfo (const string& msg);
	bool MsgWarning (const string& msg);
	void MsgError (const string& msg);

	#ifdef __BORLANDC__
	void SetNumItems();
	TEkImgCheckBox* GetChkBox(const int& num);
	TLabel* GetLbl(const int& num);
	void FontResourceAdd(const string& resName, const string& fileName);
	void FontResourceDel();
	void FontSetupMain();
	void FontSetupExtract();
	void FontApplyMain(const String& fontName);
	void FontApplyExtract(const String& fontName);
	#endif

	void LoadList();

	bool ChkAutoGuiClr();
   	bool ChkList();
	bool ChkTidyName();
	bool ChkXct();

	string ComboBitsGet();
	string ComboChanGet();
	int ComboSmpRateGet();
	string ComboBufSizeGet();
	string EditRenFilesTxtGet();
	string EditFolderNameTextGet();
	void EditFolderNameTextSet(const string& txt);
	void ClearEditFolderName();

	void LabelFilesValSet(const string& txt, const bool& update);
	void LabelArchiveValSet(const string& txt, const bool& update);
	void LabelAudioValSet(const string& txt, const bool& update);
	void LabelCaption1Set(const string& txt, const bool& update);
	void LabelCaption2Set(const string& txt, const bool& update);
	
	string LabelArchiveValGet();
	string LabelCaption1Get();
	string LabelCaption2Get();

	string ProgressGetCaption();
	void ProgressSetCaption(const string& txt);
	int ProgressGetPosition();
	void ProgressSetPosition(const float& pos);
	void ProgressSetMax(const float& max);
	void ProgressSetScroll(const bool& value);
	void ProgressCaption(const string& name);
	void ProgressCaptionBasic(const string& name, const bool& refresh);
	void ProgressCaptionUpdate();
	void ProgressStepSize (const float& inc);
	void ProgressStep();
	void ProgressStepBy(const int step);
	void ProgressReset();
	void ProgressMaxOut();
	void ProgressUpdate();
	void ProgressFxOn();
	void ProgressFxOff();
	void MPProgressBarSetup();
	void MPProgressBarReset();

	void FinishDisplay(int& extracted, int& filesTotal,
		const double& time, const string& statusCaption, const string& progressCaption);
	bool DidDragAndDrop() {return mDragAndDrop;}
	void DragAndDropSet(const bool& value) {mDragAndDrop = value;}
	void CancelAllSet(const bool& value) {mCancelAll = value;}
	bool CancelAllGet() {return mCancelAll;}
	void ListSet(const bool& value) {mListOn = value;}
	bool ListGet() {return mListOn;}
	void AutoXpandSet(const bool& value) {mAutoXpand = value;}
	bool AutoXpandGet() {return mAutoXpand;}
	bool SpeedHackIsOn() {return mSpeedOn;}
	void SpeedHackEnable(const bool& value);

	bool ExtractDlgGetIsShowing();
	void OpenDlgResetFileName();
	int OpenDlgGetTotalFiles();
	#ifdef __BORLANDC__
	void OpenDlgFilter(String ext, TOpenDialog& openDialog);
	void OpenDlgFilterAdd(String ext, TOpenDialog& openDialog);
	#endif

	void NameCaption(const string& name);
	void Caption(const string& txt);
	void CaptionMP(const string& txt);
	void CaptionSave();
	void CaptionSave(const string& txt1, const string& txt2);
	void CaptionRevert();
	void Status (const string& caption, const int& sleep, const bool& refresh);
	void Error (const string& caption, const int& sleep, FILE* stream);
	void Finish();
	bool IsExtracting() {return CItem.Extracting;}
	void WindRegion(const int& resNum, const HRGN& handle);

	void FormDoubleBufferedSet(const bool& value);
	void FormUpdate();
	int FormWidthGet();
	void FormVisibleSet(const bool& value);
	void MaxWidthMode();
	void MinWidthMode();
	void XpandItemsVisibile(const bool& state);
	void MiniHeightMode(const bool& onlyReposition);
	void NormHeightMode(const bool& onlyReposition);
	void ToggleMiniMode();
	void ToggleXpandMode();
	void CenterForm();
	void OptionsToggleViewMode();
	void Open(const char* caption1, const char* caption2, const char* captionDlg);
	void OpenMedia();
	void Close();
	void ScxLblSetup();
	void ClearInfo();
	void ExtInfo(const int& files, const vector<string>& ext);
	void SetInfoLbl(int labelNum, const string& txt);
	void ScreenSnap();
	void ShowHints();
	void FadeFx();
	void ScrollingText();
	bool ScrollTxtIsOn(){return mScrollTxt;}
	void ScrollTxtSet(bool state){mScrollTxt = state;}
	void AnimatedProgressBar();
	bool AnimatedProgressBarIsOn(){return mAnimatedProgressBar;}
	void AnimatedProgressBarSet(bool state){mAnimatedProgressBar=state;}
	void OnTop();

	#ifdef __BORLANDC__
	void __fastcall LoadImg(const String& name, Graphics::TBitmap* dest, const bool& transparent);
	void __fastcall LoadGraphics();
	#endif

	bool CursorHasTimedOut();
	void CursorStartTimer();

	// Gui specs
	int mMinHeight;
	int mMaxHeight;
	int mMinWidth;
	int mMidWidth;
	int mMaxWidth;
	int	mCheckBoxes;
	int mLabels;

	bool 	mCursorIsHidden;
	#ifdef __BORLANDC__
	vector<int>mCheckBoxVtr;
	vector<int>mLabelVtr;
	#endif

};// End Gui

extern Gui gui; // Global instance

#endif
