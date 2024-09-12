

//Zz=============================zZ
//			EKSZBOX-ABX
//			-----------
//Zz=============================zZ


// Copyright (C) 2006-2008 Kernel Master
// Author: Kernel Master
// kmeksz[At]yahoo.com


//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"

#include "Main.h"
#include "Video.h"
#include "Extract.h"
//---------------------------------------------------------------------------
#include "MediaPlayer.h"
#include "List.h"
#include "Open.h"
#include "Scx.h"
#include "Settings.h"
#include "Strings.h"
#include "Xct.h"
#include "FormatHdrs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "EkImgBtn"
#pragma link "EkImgCheckBox"
#pragma link "EkImgProgressBar"
#pragma link "EkLabel"
#pragma link "EkDirBrowse"
#pragma link "EkImgTrackBar"
#pragma link "EkFadeImgBase"
#pragma resource "*.dfm"
#pragma resource "WindRegions.res"
//#pragma resource "Font.res"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
   
//=============================
//			Init
//=============================

#ifdef DEBUG
TimrDebug->Enabled = true;
//ComboSmpRate->Style = csDropDown; // Causes compiler error
#endif

// Cover entire form with image so we can do easy form drags
Color = clNone;
ImgDrag->Align = alClient;
//ImgMain->AutoSize = true;
ImgMain->Align = alClient;
//ImgMainBak = new Graphics::TBitmap;

Application->HintHidePause = 12000;
//Application->OnShowHint = ShowHint;
Application->OnMessage = AppMessage;
        
Application->OnMinimize = MinimizeForm;
Application->OnRestore = RestoreForm;
Application->OnDeactivate = DeactivateForm;
Application->OnActivate = ActivateForm;

DragAcceptFiles(Handle, true); // Allow Drag & Drop

gui.SetNumItems(); // Need to set it for checkbox loadgraphics
TSettings set; set.Load();
gui.LoadGraphics();


//gui.FontResourceAdd("SEGIO", "segoeui.ttf");
//gui.FontApply("Segoe");
gui.FontSetupMain();
gui.ClearInfo();
gui.ProgressMaxOut();
gui.ProgressFxOn();
//gui.ProgressReset();

gui.Caption("");
gui.Status("Welcome to " + string(APPNAME) +  string(VERSION), 0, false);


// Disable offset column & set width accordingly
#ifndef DEBUG
ListView1->Columns->Delete(2);
ListView1->Columns->operator [](0)->Width = 150;
#endif


// OpenDialog extensions filter (Xbox)
string filter = "\
|adx|afs|arc|bif|big|cab|dat|dvdxwb|fsb|gta5|hdxwb|hot|hwx\
|lug|map|mapU|mpk|msx|pak|piz|pod|rcf|rez|rfa|rws|samp\
|sh4|sr|stx|swwc|sxb|vxb|wad\
|wmapack|wxv|wpx|xbp|xwb|xwc|zsm|zss|zwb";


// Straight audioformats
//filter += "|wav|wma";

// Pc
filter += "|adf";

// Tentative
//filter += "|xbp|xau";

// Debug openDialog filter
#ifdef DEBUG
filter += "|bor|mus|saf|iso";
#endif


sys.SetSupportedFormats(filter + "|asf|mp3|ogg|wav|wma|xma|xbp|xau|met|uber|xsb");
gui.OpenDlgFilter(filter.c_str(), *OpenDialog);

//=============================
//			End Init
//=============================

}

//---------------------------------------------------------------------------

//void __fastcall TForm1::ShowHint(System::AnsiString &HintStr, bool&CanShow, THintInfo &HintInfo)
//{
//  //HintStr = StringReplace(HintStr, "~", "\n", TReplaceFlags() << rfReplaceAll);
//  //int x = HintInfo.HintPos.x - this->Left;
//  //int y = HintInfo.HintPos.y - this->Top;
//  //ImgIcon->Left = x;
//  //ImgIcon->Top = y;
//  //Application->HintHidePause = HintStr.Length()*500;
//  //tagPOINT t;
//  //GetCursorPos(&t);
//  //EkszTrackBar->ScreenToClient(t);
//  //HintInfo.HintPos.x = t.x;
//
//}
//---------------------------------------------------------------------------

void __fastcall TForm1::Erase(TWMEraseBkgnd &Msg)
{
Msg.Result = false;
}

//---------------------------------------------------------------------------

// Drag & Drop
void __fastcall TForm1::WmDropFiles(TWMDropFiles& Msg)
{
	// Cleanup
	if (!gui.DidDragAndDrop())
	{
		//sys.ClearVariables();
		OpenDialog->Files->Clear();
	}

	// Init
	gui.DragAndDropSet(true);

	// Process
	char buff[MAX_PATH];
	HDROP hDrop = (HDROP)Msg.Drop;
	int numFiles = DragQueryFile(hDrop, -1,  NULL, NULL);
	for (int i = 0; i < numFiles; ++i)
	{
		DragQueryFile(hDrop, i, buff, sizeof(buff));

		if (DirectoryExists(buff)) // Process whole directory
			sys.GetAllFiles(buff, true);
		else //if (!MediaPlayer.IsValidFormat(buff))
			OpenDialog->Files->Add(buff);
	}

	Open open; open.DoOpen("");
	this->OpenDialog->Files->Clear();
	DragFinish(hDrop);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BtnOpenClick(TObject *Sender)
{

if (OpenDialog->Execute()){
	Open open;
	open.DoOpen("");
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnScanClick(TObject *Sender)
{
if (MediaPlayer.GetIsPlaying())
	BtnStopClick(this);
Scx scx;
scx.Scan(true);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnExtClick(TObject *Sender)
{

if (CItem.Loading || CItem.Files <= 0)
	return;

if (this->EditFolName->Text != "")
	CItem.DirName = this->EditFolName->Text.c_str();

Supported fmt;  fmt.StdExtract(-1);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditFolNameDblClick(TObject *Sender)
{
EditFolName->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditRenFilesDblClick(TObject *Sender)
{
EditRenFiles->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboSmpRateDblClick(TObject *Sender)
{
ComboSmpRate->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopupMenu1Popup(TObject *Sender)
{

// FileSwap popup menu
if (ChkList->Checked && !CItem.Loading && CItem.Files >= 1)
{
	if ((CItem.Format == "xwb" && !xwb.x360)
		|| CItem.Format == "stx" || CItem.Format == "rcf" || CItem.Format == "wpx")
		SwapChkdMenu->Enabled = 1;
	else
		SwapChkdMenu->Enabled = 0;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SwapChkdMenuClick(TObject *Sender)
{

// FileSwap popup menu click
if (CItem.Format == "xwb" && !xwb.x360)
{
	Supported fmt;  fmt.XwbSwap();
}
else if (CItem.Format == "rcf")
{
	Supported fmt;  fmt.RcfSwap();
}
else if (CItem.Format == "stx")
{
	Supported fmt;  fmt.StxSwap();
}
else if (CItem.Format == "wpx")
{
	Supported fmt;  fmt.WpxSwap();
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAllMenuClick(TObject *Sender)
{
List.CheckAllNone(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAllAudioMenuClick(TObject *Sender)
{
List.CheckAllAudio();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkNoneMenuClick(TObject *Sender)
{
List.CheckAllNone(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Show1Click(TObject *Sender)
{

if (ListView1->Columns->Count > 3)
{
	for (int x = 0; x < ListView1->Columns->Count; ++x)
		ListView1->Columns->Delete(1);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1ColumnClick(TObject *Sender,
	  TListColumn *Column)
{

//ListView1->CustomSort(NULL, Column->Index);
//POINT pt;
//GetCursorPos (&pt);
//PopupMenu2->Popup (pt.x, pt.y);

}

//---------------------------------------------------------------------------


void __fastcall TForm1::ShowAllButOffsets1Click(TObject *Sender)
{

//if (ListView1->Columns->Count == 7)
//{
//	ListView1->Columns->operator [](0)->Width = 150;
//	ListView1->Columns->Delete(2);
//	List.SetShowOffsets(false);
//	ListView1->Invalidate();
//	//List.Load();
//	//ListView1->Columns->operator [](0)->Caption="Name";
//	//ListView1->Columns->operator [](1)->Caption="Size";
//	//ListView1->Columns->operator [](2)->Caption="SR";
//	//ListView1->Columns->operator [](3)->Caption="Ch";
//	//ListView1->Columns->operator [](4)->Caption="Bit";
//	//ListView1->Columns->operator [](5)->Caption="Fmt";
//}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Verbose1Click(TObject *Sender)
{

//ListView1->Columns->BeginUpdate();
//
//if (ListView1->Columns->Count < 7 && ListView1->Columns->Count >= 2)
//{
//	ListView1->Columns->operator [](0)->Width = 116;
//	ListView1->Columns->Insert(2);
//	ListView1->Columns->operator [](2)->Caption="Offset";
//	List.SetShowOffsets(true);
//	ListView1->Invalidate();
//	//List.Load();
//}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditRenFilesKeyUp(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{

if (FItem.FileName.empty() || Key != VK_RETURN || CItem.ScxMode)
	return;

string text = EditRenFiles->Text.c_str();

int x = 0;
while (x < CItem.Files)
{
	FItem.FileName[x] =
	str.StringReplacer(text, x, CItem.Files);

	if (ChkTidyName->Checked)
		str.StrClean(FItem.FileName[x]);
	++x;
}

if (ChkList->Checked)
{
	List.Load();
	gui.Status("List Loaded", 0, true);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditFolNameChange(TObject *Sender)
{

if (ChkXct->Checked)
	return;
sys.DirNameSet();

}

//---------------------------------------------------------------------------

void __fastcall TForm1::CheckAll2Click(TObject *Sender)
{

int x = 0;
while(x < ListView1->Items->Count)
{
	ListView1->Items->operator [](x)->Checked = 1;
	++x;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckNone1Click(TObject *Sender)
{

int x = 0;
while(x < ListView1->Items->Count)
{
	ListView1->Items->operator [](x)->Checked = 0;
	++x;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnIniClick(TObject *Sender)
{

if (CItem.Loading || CItem.Files <= 0) return;

// Dump xct name file
if (!CItem.Loading && this->EditFolName->Text == "")
	gui.MsgError("Sorry, folder name text box must not be blank");
else if (!CItem.Loading)
{
	Xct x;
	x.Dump();
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnTgleClick(TObject *Sender)
{
gui.OptionsToggleViewMode();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnSelDirClick(TObject *Sender)
{
sys.DirBrowse();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::EditRenFilesChange(TObject *Sender)
{

if (CItem.Loading || CItem.ScxMode || !CItem.Files)
	return;

if (EditRenFiles->Text == "")
	FItem.FileName = FItem.FileNameBak;

if (ChkTidyName->Checked)
{
	for (int x = 0; x < CItem.Files; ++x)
		str.StrClean(FItem.FileName[x]);
}

if (EditRenFiles->Text == "" && ChkList->Checked)
{
	List.Load();
	gui.Status("List Loaded", 0, true);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuOnTopClick(TObject *Sender)
{                  
gui.OnTop();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuScreenSnapClick(TObject *Sender)
{
gui.ScreenSnap();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuShowHintsClick(TObject *Sender)
{
gui.ShowHints();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuFadeFxClick(TObject *Sender)
{
gui.FadeFx();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuScrollingTextClick(TObject *Sender)
{
gui.ScrollingText();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuAnimatedProgressBarClick(TObject *Sender)
{
gui.AnimatedProgressBar();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopMenuCloseClick(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimrDebugTimer(TObject *Sender)
{

#ifdef DEBUG

if (!LblMem->Visible)
LblMem->Visible = 1;

if (!CItem.Loading || CItem.Finished)
{
	LblMem->Caption =
	"Free Memory: " + String(sys.GetAvailPhyMem() / 1048576) + "MB";
}
#endif

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimrRefreshRateTimer(TObject *Sender)
{

if (gui.IsExtracting() && gui.SpeedHackIsOn() && !MediaPlayer.GetIsPlaying())
{
	gui.ProgressUpdate();
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimrDblBuffTimer(TObject *Sender)
{

DoubleBuffered = false;
TimrDblBuff->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditFolNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

if (Button == 2 && EditFolName->SelStart == EditFolName->Text.Length())
	 EditFolName->SelStart=0;
else if (Button == 2 && EditFolName->SelStart != EditFolName->Text.Length())
	EditFolName->SelStart = EditFolName->Text.Length();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditRenFilesMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{

if (Button == 2 && EditRenFiles->SelStart == EditRenFiles->Text.Length())
	 EditRenFiles->SelStart=0;
else if (Button == 2 && EditRenFiles->SelStart != EditRenFiles->Text.Length())
	EditRenFiles->SelStart = EditRenFiles->Text.Length();
	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboSmpRateChange(TObject *Sender)
{

if (ComboSmpRate->Text == "")
	ComboSmpRate->Text = "Auto";

try
{
	ComboSmpRate->Text.ToInt();
}
catch(Exception &e)
{
	ComboSmpRate->Text = "Auto";
}

}

//------------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

sys.~SystemX();
//List.~EkszList();// Doesn't work ??
TSettings set;
set.Save();
//gui.FontResourceDel();

}

//------------------------------------------------------------------------------

void __fastcall TForm1::ImgDragMouseDown(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{

// Form Drag
if (Button == mbLeft){
	ReleaseCapture();
	SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}
// Popup menu
else if (Button == mbRight)
{
	TPoint p = ClientToScreen(Point(X, Y));
	PopupMenuMain->Popup(p.x, p.y);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImgDragDblClick(TObject *Sender)
{

if (!MediaPlayer.GetIsPlaying())
	BtnGuiWidthClick(this);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeactivateForm(TObject* Sender){

this->LblCaption->Color1 = clGray;
this->LblCaption->Color2 = clGray;

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ActivateForm(TObject* Sender){

this->LblCaption->Color1 = clRed;
this->LblCaption->Color2 = clWhite;

}

//---------------------------------------------------------------------------

void __fastcall TForm1::MinimizeForm(TObject* Sender){

Minimized = true;

}

//---------------------------------------------------------------------------

void __fastcall TForm1::RestoreForm(TObject* Sender){

Minimized = false;
if (ExtractDlg->Enabled){ // Fix minimize dialog close issue
	ExtractDlg->Show();
	ExtractDlg->Close();
}

}

//---------------------------------------------------------------------------

//void __fastcall TForm1::WMNCHitTest(TWMNCHitTest &Msg){
//
//ShowMessage("hit");
//
//}

//---------------------------------------------------------------------------

//void __fastcall TForm1::WndProc(TMessage& Msg)
//{
//
//switch (Msg.Msg)
//{
////	case WM_LBUTTONDOWN:
////		ShowMessage("Down");
////		break;
////
//	case WM_MOUSEMOVE:
//		
//        ShowCursor(true);
//		tagPOINT t;
//		GetCursorPos(&t);
//		if (t.x >= VideoWnd->ClientOrigin.x &&
//		t.x <= VideoWnd->ClientOrigin.x + VideoWnd->Width &&
//		t.y >= VideoWnd->ClientOrigin.y &&
//		t.y <= VideoWnd->ClientOrigin.y + VideoWnd->Height)
//			gui.CursorStartTimer();
//		//ShowMessage("Move");
//		break;
//}
//
//TForm::WndProc(Msg);
//
//}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)
{

// Disable unsupported chars
if (Key == '\\' || Key == '/'|| Key == ':' || Key == '*' || Key == '?'
	|| Key == '"' || Key == '<' || Key == '>' || Key == '|' )
	Key = NULL;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AppMessage(MSG &msg, bool &handled)
{


//// Handle directshow events
//switch(msg.message)
//{
//	case WM_GRAPHEVENT:
//		DS.HandleEvent();
//		break;
//	default:
//		break;
//}

if(msg.message == WM_LBUTTONUP || msg.message == WM_KEYUP)
	MediaPlayer.SetBusy(false);


//if(msg.message == WM_LBUTTONUP && ProgressMouseDown){
//	//ShowMessage("UP");
//
//	//pos := (FCursorPos.x - ClientOrigin.x)* (FMaxValue - FMinValue) / Width;
//	//FCursorPos.x := FCursorPos.x - ClientOrigin.x;
//
//	float x = msg.pt.x - ClientOrigin.x - EkImgProgressBar->Left+6;
//	float step = MediaPlayer.GetDuration() / EkImgProgressBar->Width;
//	float seekTo = x * step;
//	MediaPlayer.Seek(seekTo);
//	MediaPlayer.StartUpdateCaption();
//}

if(msg.message != WM_LBUTTONDOWN && msg.message != WM_KEYDOWN){	handled = false;
	return;
}TShiftState Shift = KeyDataToShiftState(msg.wParam);if (msg.message == WM_KEYDOWN)// Key controls{	if(Shift.Contains(ssCtrl) && msg.wParam == 'O')		BtnOpenClick(this);
	else if(Shift.Contains(ssCtrl) && msg.wParam == 'E')
		BtnExtClick(this);
	else if(Shift.Contains(ssCtrl) && msg.wParam == 'S')
		BtnScanClick(this);
	else if(Shift.Contains(ssCtrl) && msg.wParam == 'T')
		BtnGuiHeightClick(this);
	else if(Shift.Contains(ssCtrl) && msg.wParam == 'M')
		BtnMinClick(this);
	else if(Shift.Contains(ssCtrl) && msg.wParam == 'Q')
		BtnCloseClick(this);


	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 189) // Vol -
	{
		if (EkszTrackBar->Position > EkszTrackBar->Min)
		EkszTrackBar->Position -= EkszTrackBar->Max>>6;
		MediaPlayer.SetVolume(EkszTrackBar->Position -5000);
	}

	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 187) // Vol +
	{
		if (EkszTrackBar->Position < EkszTrackBar->Max)
		EkszTrackBar->Position += EkszTrackBar->Max>>6;
		MediaPlayer.SetVolume(EkszTrackBar->Position -5000);
	}
		
	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 'F')		MediaPlayer.ToggleFullScreen();	else if (msg.wParam == 'C')		BtnClrPlayListClick(this);	else if (msg.wParam == 'L'){		BtnLoopPlayList->Checked = !BtnLoopPlayList->Checked;		BtnLoopPlayListClick(this);	}	else if (msg.wParam == 'M'){		static float vol = EkszTrackBar->Position;		if (EkszTrackBar->Position == EkszTrackBar->Min) // Revert			EkszTrackBar->Position = vol; // Restore pos		else // Mute		{			vol = EkszTrackBar->Position; // Save pos			EkszTrackBar->Position = EkszTrackBar->Min;		}		MediaPlayer.SetVolume(EkszTrackBar->Position -5000);	}	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 'R')		MediaPlayer.LoopRegionToggle();	else if (MediaPlayer.GetIsPlaying() && msg.wParam == VK_LEFT)		BtnPrevClick(this);	else if (MediaPlayer.GetIsPlaying() && msg.wParam == VK_RIGHT)		BtnNextClick(this);	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 190)		MediaPlayer.FastForward();
	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 188)
		MediaPlayer.Rewind();
	else if (MediaPlayer.GetIsPlaying() && msg.wParam == 'P')
		BtnPauseClick(this);
	else if (MediaPlayer.GetIsPlaying() && (msg.wParam == VK_NUMPAD1 || msg.wParam == 49)) // Loop start
		MediaPlayer.SetLoopRegions(gui.ProgressGetPosition(), 0);
	else if (MediaPlayer.GetIsPlaying() && (msg.wParam == VK_NUMPAD2 || msg.wParam == 50)) // Loop end
		MediaPlayer.SetLoopRegions(0, gui.ProgressGetPosition());
	else if (msg.wParam == VK_SPACE && !EditRenFiles->Focused()
	&& !EditFolName->Focused())
	{
		if (!MediaPlayer.GetIsPlaying())
			BtnPlayClick(this);
		else
			BtnStopClick(this);
	}}if(msg.message == WM_LBUTTONDOWN && ChkXct->Checked && !EditRenFiles->Enabled &&
	(msg.pt.x  >= EditRenFiles->ClientOrigin.x
	&& msg.pt.x <= EditRenFiles->ClientOrigin.x + EditRenFiles->Width) &&
	(msg.pt.y  >= EditRenFiles->ClientOrigin.y
	&& msg.pt.y <= EditRenFiles->ClientOrigin.y + EditRenFiles->Height)
)
{
	gui.Status("Renaming files only supported if Xct is disabled", 0, true);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnFormMove(TWMMove& Msg){

if (MediaPlayer.GetIsPlaying() && MediaPlayer.GetIsVideo()
	&& !MediaPlayer.GetIsFullScreen())
	
if (VideoWnd->Left != this->Left + this->ListView1->Left ||
		VideoWnd->Top != this->Top + this->ListView1->Top)
{
	VideoWnd->Left = 27;
	VideoWnd->Top = 21;
	VideoWnd->Width = this->ListView1->Width +6;
	VideoWnd->Height = this->ListView1->Height +6;
}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ChkListClick(TObject *Sender)
{

if (ChkList->Checked)
	gui.Status("List Enabled", 0, false);
else
	gui.Status("List Disabled", 0, false);

gui.ListSet(ChkList->Checked);

if (CItem.Loading || !List.mFileList || CItem.Files == 0 || CItem.ScxMode
	|| MediaPlayer.GetIsPlaying())
return;

if (ChkList->Checked)
{
	List.Load();
	gui.Status("List Loaded", 0, false);
}
else if (!ChkList->Checked)
	List.Clear();

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ChkXctClick(TObject *Sender)
{

if (CItem.Loading || CItem.ScxMode)
	return;

if (!CItem.Files)
{
	if (ChkXct->Checked)
		gui.Status("Xct Enabled", 0, false);
	else if (!ChkXct->Checked)
		gui.Status("Xct Disabled", 0, false);
	return;
}


if (ChkXct->Checked)
{
	Xct xct; xct.FindMatch(CItem.DirName, CItem.IniMatch, NULL);
	if (ChkList->Checked && CItem.IniMatch)
	{
		List.Load();
	}
	if (!CItem.IniMatch)
		gui.Status("No Matching Xct Database Entry Found", 0, false);
			else
			{
				gui.Status("Matching Xct Database Entry Found", 0, false);
				gui.Caption(CItem.DirName.c_str());
			}
}
else
{
	gui.Caption(str.ExtractFileName(CItem.OpenFileName));
	EditRenFiles->Enabled = true;
	string text = this->EditRenFiles->Text.c_str();
	EditFolName->Text = "";

	int x = 0;
	while(x < CItem.Files)
	{
		if (text != "")
			FItem.FileName[x] = str.StringReplacer(text, x, CItem.Files);
		else
			FItem.FileName[x] = FItem.FileNameBak[x];
		if (ChkTidyName->Checked)
			str.StrClean(FItem.FileName[x]);
		++x;
	}

	if (ChkList->Checked && CItem.IniMatch)
		List.Load();

	gui.Status("Xct Database Disabled", 0, false);
}

}

//------------------------------------------------------------------------------

void __fastcall TForm1::ChkTidyNameClick(TObject *Sender)
{

if (CItem.Loading || ChkXct->Checked)     //|| ChkXct->Checked
	return;

if (CItem.ScxMode || !CItem.Files)
{
	if (ChkTidyName->Checked)
		gui.Status("Tidy Names Enabled", 0, false);
	else
		gui.Status("Tidy Names Disabled", 0, false);
}

string text = EditRenFiles->Text.c_str();

	int x = 0;
	while (x < CItem.Files)
	{
		if (text != "")
			FItem.FileName[x] = str.StringReplacer(text, x, CItem.Files);
		else
			FItem.FileName[x] = FItem.FileNameBak[x];
		if (ChkTidyName->Checked)
			str.StrClean(FItem.FileName[x]);
		++x;
	}

	if (ChkList->Checked){
		List.Load();
	}

	if (ChkTidyName->Checked)
		gui.Status("Tidy Names Enabled", 0, false);
	else
		gui.Status("Tidy Names Disabled", 0, false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAutoEditClrClick(TObject *Sender)
{

if (CItem.Loading) return;

if (ChkAutoEditClr->Checked)
	gui.Status("AutoEdit-Clear Enabled", 0, false);
else
	gui.Status("AutoEdit-Clear Disabled", 0, false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAutoGuiClrClick(TObject *Sender)
{

if (CItem.Loading) return;

if (ChkAutoGuiClr->Checked)
	gui.Status("AutoGui-Clear Enabled", 0, false);
else
	gui.Status("AutoGui-Clear Disabled", 0, false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAutoScxClick(TObject *Sender)
{

if (CItem.Loading) return;

if (ChkAutoScx->Checked)
	gui.Status("AutoScx Enabled", 0, false);
else
	gui.Status("AutoScx Disabled", 0, false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkSpeedClick(TObject *Sender)
{

if (ChkSpeed->Checked)
	gui.Status("SpeedHack Enabled", 0, false);
else
	gui.Status("SpeedHack Disabled", 0, false);

gui.SpeedHackEnable(ChkSpeed->Checked);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChkAutoXpandClick(TObject *Sender)
{

if (ChkAutoXpand->Checked)
	gui.Status("AutoXpand Enabled", 0, false);
else
	gui.Status("AutoXpand Disabled", 0, false);

gui.AutoXpandSet(ChkAutoXpand->Checked);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnPlayClick(TObject *Sender)
{

if ((!CItem.Files && !MediaPlayer.PlayListGetSize()) || MediaPlayer.GetIsPlaying())
	return;

MediaPlayer.SetStopped(false);

if (FItem.Channels.size() == 0 && MediaPlayer.PlayListGetSize())
{
	MediaPlayer.PlayListPlay();
	return;
}

int num = 0;
if (ListView1->SelCount)
	num = ListView1->Selected->Index;

MediaPlayer.Play(num);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnStopClick(TObject *Sender)
{
MediaPlayer.Stop();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnPrevClick(TObject *Sender)
{

MediaPlayer.SetStopped(false);

if (MediaPlayer.PlayListGetSize())
	MediaPlayer.PlayListPlayPrev();
else
	MediaPlayer.PlayPrev();

Sleep(10);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnNextClick(TObject *Sender)
{

MediaPlayer.SetStopped(false);

if (MediaPlayer.PlayListGetSize())
	MediaPlayer.PlayListPlayNext();
else
	MediaPlayer.PlayNext();

Sleep(10);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnPauseClick(TObject *Sender)
{

MediaPlayer.Pause();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{

MediaPlayer.Stop();
//DS.~DirectShow();
string temp = sys.GetTempDir() + "EkszTemp";
sys.DeleteDir(temp);

}

//---------------------------------------------------------------------------

void __fastcall TForm1::BtnFadeClick(TObject *Sender)
{

//MediaPlayer.ToggleFullScreen();
//MediaPlayer.Crossfade(this->EditRenFiles->Text.ToInt());

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnCloseClick(TObject *Sender)
{

this->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnMinClick(TObject *Sender)
{

Application->Minimize();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnGuiWidthClick(TObject *Sender)
{

if (!MediaPlayer.GetIsPlaying())
	gui.ToggleXpandMode();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnGuiHeightClick(TObject *Sender)
{

if (MediaPlayer.GetIsVideo() && MediaPlayer.GetIsPlaying())
	return;

if (this->Width == gui.mMaxWidth)
	gui.MinWidthMode();

gui.ToggleMiniMode();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnClrPlayListClick(TObject *Sender)
{
MediaPlayer.PlayListClear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnLoopPlayListClick(TObject *Sender)
{

if (BtnLoopPlayList->Checked)
	MediaPlayer.SetIsLoopingPlaylist(true);
else
	MediaPlayer.SetIsLoopingPlaylist(false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1DblClick(TObject *Sender)
{

if (!CItem.Files && !MediaPlayer.PlayListGetSize())
	return;

MediaPlayer.SetStopped(false);
int num = 0;
if (ListView1->SelCount)
	num = ListView1->Selected->Index;

MediaPlayer.Play(num);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkszTrackBarMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

EkszTrackBar->Animation = true;
EkszTrackBar->AutoUpdate = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkszTrackBarMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{

EkszTrackBar->Animation = false;
EkszTrackBar->AutoUpdate = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkszTrackBarChange(TObject *Sender)
{
MediaPlayer.SetVolume(EkszTrackBar->Position -5000);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkImgProgressBarMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
MediaPlayer.StopUpdateCaption();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkImgProgressBarMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{

MediaPlayer.Seek(gui.ProgressGetPosition());
MediaPlayer.StartUpdateCaption();
MediaPlayer.SetIsSeeking(false);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EkImgProgressBarChange(TObject *Sender)
{
	MediaPlayer.SetIsSeeking(true);
}

//---------------------------------------------------------------------------



