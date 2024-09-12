


//=============================
//		Gui.cpp
//=============================

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

#ifdef _XBOX
#include "../XBOX/XBOXABX.h"
#endif

//==============================================================================

Gui::Gui(){

//mAutoXpand = false;
mShowOptions = true;
mAppDir = sys.GetAppDir();

}

//==============================================================================

Gui::~Gui(){

#ifdef __BORLANDC__
mCheckBoxVtr.clear();
mLabelVtr.clear();
#endif

}

//==============================================================================

void Gui::Msg (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, 0);
#endif

}

//==============================================================================

void Gui::MsgA (const string& msg, const char* caption){

#ifdef _WINDOWS
MessageBoxA(NULL, msg.c_str(), caption, 0);
#endif

}

//==============================================================================

void Gui::MsgInfo (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, MB_ICONINFORMATION);
#endif

}

//==============================================================================

bool Gui::MsgWarning (const string& msg){

#ifdef _WINDOWS
if (MB_OK == MessageBox(NULL, msg.c_str(), NULL, MB_OKCANCEL | MB_ICONWARNING))
	return true;
#endif

return false;

}

//==============================================================================

void Gui::MsgError (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, MB_OK|MB_ICONERROR);
#endif

}

//==============================================================================

#ifdef __BORLANDC__
void Gui::SetNumItems(){

mCheckBoxes = 0, mLabels = 0;

int componentCount = FORM->ControlCount;

for(int i = 0; i < componentCount; ++i)
{
	TComponent * Component = FORM->Components[i];

	if(Component->ClassNameIs("TEkImgCheckBox") &&
		Component->Name != "BtnLoopPlayList"){
		++mCheckBoxes;
		mCheckBoxVtr.push_back(i);
	}
	else if(Component->ClassNameIs("TLabel")){
		++mLabels;
		mLabelVtr.push_back(i);
	}
}

}
#endif

//==============================================================================

#ifdef __BORLANDC__
TEkImgCheckBox* Gui::GetChkBox(const int& num){

TComponent* Component = FORM->Components[mCheckBoxVtr[num]];
return dynamic_cast<TEkImgCheckBox*>(Component);

}
#endif

//==============================================================================

#ifdef __BORLANDC__
TLabel* Gui::GetLbl(const int& num){

TComponent* Component = FORM->Components[mLabelVtr[num]];
return dynamic_cast<TLabel*>(Component);

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontResourceAdd(const string& resName, const string& fileName){

TResourceStream& res = *new TResourceStream((int)HInstance, resName.c_str(), "RES");
string tempDir = sys.GetTempDir() + "EkszTemp";
sys.CreateDir(tempDir);
mFontPath = tempDir + "\\" + fileName;
res.SaveToFile(str.StdToAnsi(mFontPath));
res.Free();
AddFontResource(mFontPath.c_str());
SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontResourceDel(){

RemoveFontResource(mFontPath.c_str());

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontSetupMain(){

if (sys.GetWinVer() <= 5) // Win Xp or less
{
	FontApplyMain("Tahoma");
}
else //if (sys.GetWinVer() > 5) // Win Vista
{
}

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontSetupExtract(){

if (sys.GetWinVer() <= 5) // Win Xp or less
	FontApplyExtract("Tahoma");

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontApplyMain(const String& fontName){

int std = 8;

FORM->LblCaption->Font->Name = fontName;
FORM->LblCaption->Font->Size = std;
//FORM->LblCaption->Font->Style<<fsBold;

for (int i = 0; i < mLabels ; ++i){
	GetLbl(i)->Font->Name = fontName;
	GetLbl(i)->Font->Size = std;
}

for (int i = 0; i < mCheckBoxes ; ++i){
	GetChkBox(i)->Font->Name = fontName;
	GetChkBox(i)->Font->Size = std;
}

FORM->ListView1->Font->Name = "Verdana";
FORM->ListView1->Font->Size = 7;
//FORM->ListView1->Font->Name = fontName;
//FORM->ListView1->Font->Size = std;

FORM->EkImgProgressBar->Font->Name = fontName;
FORM->EkImgProgressBar->Font->Size = std;

FORM->EditRenFiles->Font->Name = fontName;
FORM->EditRenFiles->Font->Size = std;
FORM->EditFolName->Font->Name = fontName;
FORM->EditFolName->Font->Size = std;
FORM->ComboFormat->Font->Name = fontName;
FORM->ComboFormat->Font->Size = std;
FORM->ComboBufSize->Font->Name = fontName;
FORM->ComboBufSize->Font->Size = std;
FORM->ComboSmpRate->Font->Name = fontName;
FORM->ComboSmpRate->Font->Size = std;
FORM->ComboChan->Font->Name = fontName;
FORM->ComboChan->Font->Size = std;
FORM->ComboBits->Font->Name = fontName;
FORM->ComboBits->Font->Size = std;

}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::FontApplyExtract(const String& fontName){

int std = 8;

ExtractDlg->BtnStop->Font->Name = fontName;
ExtractDlg->BtnStop->Font->Size = std;
ExtractDlg->BtnStopAll->Font->Name = fontName;
ExtractDlg->BtnStopAll->Font->Size = std;

}
#endif

//==============================================================================

void Gui::LoadList(){

#ifdef __BORLANDC__
if (ChkList())
	FORM->List.Load();
#endif

}

//==============================================================================

bool Gui::ChkAutoGuiClr(){

#ifdef __BORLANDC__
return FORM->ChkAutoGuiClr->Checked;
#else
return true;
#endif

}

//==============================================================================

bool Gui::ChkList(){

#ifdef __BORLANDC__
return FORM->ChkList->Checked;
#else
return false;
#endif

}

//==============================================================================

bool Gui::ChkTidyName(){

#ifdef __BORLANDC__
return FORM->ChkTidyName->Checked;
#else
return true;
#endif

}

//==============================================================================

bool Gui::ChkXct(){

#ifdef __BORLANDC__
return FORM->ChkXct->Checked;
#else
return true;
#endif

}

//==============================================================================

string Gui::ComboBitsGet(){

#ifdef __BORLANDC__
return FORM->ComboBits->Text.c_str();
#else
return str.IntToStr(16);
#endif

}

//==============================================================================

string Gui::ComboChanGet(){

#ifdef __BORLANDC__
return FORM->ComboChan->Text.c_str();
#else
return "Stereo";
#endif

}

//==============================================================================

int Gui::ComboSmpRateGet(){

#ifdef __BORLANDC__
return FORM->ComboSmpRate->Text.ToInt();
#else
return 44100;
#endif

}

//==============================================================================

string Gui::ComboBufSizeGet(){

#ifdef __BORLANDC__
if (FORM->ComboBufSize->ItemIndex == 0)
	return "Small";
if (FORM->ComboBufSize->ItemIndex == 1)
	return "Medium";
if (FORM->ComboBufSize->ItemIndex == 2)
	return "Large";
return "Medium";
#else
return "Medium";
#endif

}

//==============================================================================

string Gui::EditRenFilesTxtGet(){

#ifdef __BORLANDC__
return FORM->EditRenFiles->Text.c_str();
#else
return "";
#endif

}

//==============================================================================

void Gui::LabelFilesValSet(const string& txt, const bool& update){

#ifdef __BORLANDC__
FORM->LblFilesVal->Caption = txt.c_str();
if (update)
	FORM->LblFilesVal->Update();
#endif

}

//==============================================================================

void Gui::LabelArchiveValSet(const string& txt, const bool& update){

#ifdef __BORLANDC__
FORM->LblArchiveVal->Caption = txt.c_str();
if (update)
	FORM->LblArchiveVal->Update();
#endif

}

//==============================================================================

void Gui::LabelAudioValSet(const string& txt, const bool& update){

#ifdef __BORLANDC__
FORM->LblAudioVal->Caption = txt.c_str();
if (update)
	FORM->LblAudioVal->Update();
#endif

}

//==============================================================================

void Gui::LabelCaption1Set(const string& txt, const bool& update){

#ifdef __BORLANDC__
FORM->LblCaption->Text1 = txt.c_str();
if (update)
	FORM->LblCaption->Update();
#endif

}

//==============================================================================

void Gui::LabelCaption2Set(const string& txt, const bool& update){

#ifdef __BORLANDC__
FORM->LblCaption->Text2 = txt.c_str();
if (update)
	FORM->LblCaption->Update();
#endif

}

//==============================================================================

string Gui::LabelArchiveValGet(){

#ifdef __BORLANDC__
return FORM->LblArchiveVal->Caption.c_str();
#else
return "";
#endif

}

//==============================================================================

string Gui::LabelCaption1Get(){

#ifdef __BORLANDC__
return FORM->LblCaption->Text1.c_str();
#else
return "";
#endif

}

//==============================================================================

string Gui::LabelCaption2Get(){

#ifdef __BORLANDC__
return FORM->LblCaption->Text2.c_str();
#else
return "";
#endif

}

//==============================================================================

string Gui::EditFolderNameTextGet(){

#ifdef __BORLANDC__
return FORM->EditFolName->Text.c_str();
#else
return "";
#endif

}

//==============================================================================

void Gui::EditFolderNameTextSet(const string& txt){

#ifdef __BORLANDC__
FORM->EditFolName->Text = txt.c_str();
#endif

}

//==============================================================================

void Gui::ClearEditFolderName(){

#ifdef __BORLANDC__
if (FORM->ChkAutoEditClr->Checked)
	FORM->EditFolName->Clear();
#endif

}

//==============================================================================

string Gui::ProgressGetCaption(){

#ifdef __BORLANDC__
return FORM->EkImgProgressBar->Text.c_str();
#else
return "";
#endif

}

//==============================================================================

void Gui::ProgressSetCaption(const string& txt){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Text = txt.c_str();
#endif

}

//==============================================================================

int Gui::ProgressGetPosition(){

#ifdef __BORLANDC__
return FORM->EkImgProgressBar->Position;
#else
return -1;
#endif

}

//==============================================================================

void Gui::ProgressSetPosition(const float& pos){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Position = pos;
#endif

}

//==============================================================================

void Gui::ProgressSetMax(const float& max) {

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Max = max;
#endif

}

//==============================================================================

void ProgressSetScroll(const bool& value) {

#ifdef __BORLANDC__
FORM->EkImgProgressBar->ScrollText = value;
#endif

}

//==============================================================================

void Gui::ProgressCaption(const string& name){

string ext = str.ExtractFileExt(name);

mProgessCaption = str.ExtractFileName(name);

if (!mSpeedOn)
	ProgressCaptionUpdate();

}

//==============================================================================

void Gui::ProgressCaptionBasic(const string& name, const bool& refresh){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Text = name.c_str();
if (refresh)
	FORM->EkImgProgressBar->Refresh();
#else
	#ifdef _XBOX
		xbApp.DrawStatusTxt(name);
	#else
		cout<<"FileName: "<<name<<endl;
	#endif
#endif

}

//==============================================================================

void Gui::ProgressCaptionUpdate(){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Text = mProgessCaption.c_str();
#endif

}

//==============================================================================

void Gui::ProgressStepSize (const float& inc){

mInc = inc;
mIncP = inc;

}

//==============================================================================

void Gui::ProgressStep(){

if (!mSpeedOn)
	ProgressUpdate();

mInc += mIncP;

}

//==============================================================================

void Gui::ProgressStepBy(const int step){

if (!mSpeedOn)
	ProgressUpdate();

mInc += step;

}

//==============================================================================

void Gui::ProgressReset(){

mInc = 0;
mIncP = 0;
#ifdef __BORLANDC__
FORM->EkImgProgressBar->Position = mInc;
FORM->EkImgProgressBar->Text = "";
ExtractDlg->Caption = String (int (mInc)) + "%";
#endif

}

//==============================================================================

void Gui::ProgressSetScroll(const bool& value) {

#ifdef __BORLANDC__
FORM->EkImgProgressBar->ScrollText = value;
#endif

}

//==============================================================================

void Gui::ProgressMaxOut(){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Position = 100;
ExtractDlg->Caption = String (100) + "%";
#endif

}

//==============================================================================

void Gui::ProgressUpdate(){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Position = mInc;
ExtractDlg->Caption = String (int (mInc)) + "%";
#endif

}

//==============================================================================

void Gui::ProgressFxOn(){

#ifdef __BORLANDC__
if (FORM->EkImgProgressBar->AnimateFx == efxNone) {
	FORM->EkImgProgressBar->AnimateFx = efx2;
	FORM->EkImgProgressBar->AnimationInterval = 31;
	FORM->EkImgProgressBar->Animation = true;
}
#endif

}

//==============================================================================

void Gui::ProgressFxOff(){

#ifdef __BORLANDC__
if (FORM->EkImgProgressBar->AnimateFx == efx2) {
	FORM->EkImgProgressBar->AnimateFx = efxNone;
	FORM->EkImgProgressBar->AnimateDirection = edForward;
	FORM->EkImgProgressBar->AnimationInterval = 11;
	FORM->EkImgProgressBar->Animation = false;
}
#endif

}

//==============================================================================

void Gui::MPProgressBarSetup(){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->TrackBarMode = true;
FORM->EkImgProgressBar->AutoUpdate = false;
if (AnimatedProgressBarIsOn())
	FORM->EkImgProgressBar->AutoUpdate = true;
if (ScrollTxtIsOn()){
	ProgressSetScroll(true);
	FORM->EkImgProgressBar->TextEllipse = etlNone;
}
	else
		FORM->EkImgProgressBar->TextEllipse = etlEndEllipsis;
#endif

}

//==============================================================================

void Gui::MPProgressBarReset(){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->TrackBarMode = false;
if (FORM->EkImgProgressBar->AutoUpdate)
	FORM->EkImgProgressBar->AutoUpdate = false;
if (ScrollTxtIsOn())
	ProgressSetScroll(false);
#endif

}

//==============================================================================

void Gui::FinishDisplay(int& extracted, int& filesTotal,
			const double& time, const string& statusCaption, const string& progressCaption){

#ifdef __BORLANDC__
FORM->EkImgProgressBar->Text = progressCaption.c_str();
#endif

if (time > 60)
{
	int min = time / 60;
	int sec = (int)time % 60;
	Status(statusCaption + " " + str.IntToStr(extracted) + " File/s in "
	+ str.IntToStr(min) + " Min/s " + str.IntToStr(sec) + " Sec/s", 0, false);
}
else
{
	Status(statusCaption + " " + str.IntToStr (extracted)
	+ " File/s in " + str.IntToStr (time) + " Sec/s", 0, false);
}

if (filesTotal != extracted)
{
	MsgError(str.IntToStr(filesTotal - extracted)
	+ " file/s not extracted due to error/s");
}

Finish();
Close();
extracted = 0,
filesTotal = 0;

}

//==============================================================================

void Gui::SpeedHackEnable(const bool& value){

if (mSpeedOn != value){
	mSpeedOn = value;
	#ifdef __BORLANDC__
	FORM->TimrRefreshRate->Enabled = value;
	#endif
}

}

//==============================================================================

bool Gui::ExtractDlgGetIsShowing(){

#ifdef __BORLANDC__
return ExtractDlg->Showing;
#else
return true;
#endif

}

//==============================================================================

void Gui::OpenDlgResetFileName(){

#ifdef __BORLANDC__
FORM->OpenDialog->FileName = CItem.OpenFileName.c_str();
#endif

}

//==============================================================================

int Gui::OpenDlgGetTotalFiles(){

#ifdef __BORLANDC__
return FORM->OpenDialog->Files->Count;
#else
return 1;
#endif

}

//==============================================================================

#ifdef __BORLANDC__
// String format must be like this: |wav|wma|wmv
void Gui::OpenDlgFilter(String ext, TOpenDialog& openDialog)
{
	openDialog.Filter = ("All Files|*.*");
	openDialog.Filter += ("|Supported Audio Files / Archives|");

	OpenDlgFilterAdd(ext, openDialog);

	//openDialog.Filter += ("|PC Audio Files / Archives|");

	//OpenDlgFilterAdd(pcExt, openDialog);

	//openDialog.Filter += ("|All Files|*.*");
}
#endif

//==============================================================================

#ifdef __BORLANDC__
void Gui::OpenDlgFilterAdd(String ext, TOpenDialog& openDialog)
{
	vector<String>extVect;

	for(int pos; pos = ext.Pos("|");)
	{
		ext.Delete(pos, 1);

		int len = ext.Pos("|") -1;
		// For when only 1 extension remains
		if (len <= 0)
			len = ext.Length();

		extVect.push_back(ext.SubString(pos, len));
		ext.Delete(1, len);
	}

	for (uint x = 0; x < extVect.size(); ++x)
		openDialog.Filter += "*." + extVect[x] + ";";

	extVect.clear();
}
#endif

//==============================================================================

void Gui::NameCaption(const string& name){

#ifdef __BORLANDC__
FORM->LblCaption->Text1 = APPNAME;
FORM->LblCaption->Text2 = " - " + ExtractFileName(name.c_str());
#endif

}

//==============================================================================

void Gui::Caption(const string& txt){

#ifdef __BORLANDC__
FORM->LblCaption->Text1 = APPNAME;

if (txt.length())
	FORM->LblCaption->Text2 = " - " +  String(txt.c_str());
else
	FORM->LblCaption->Text2 = "";
#endif

}

//==============================================================================

void Gui::CaptionMP(const string& txt){

#ifdef __BORLANDC__
FORM->LblCaption->Text1 = APPNAME;

if (MediaPlayer.PlayListGetSize())
	FORM->LblCaption->Text2 =
	" - MPlayer ("+
	String(MediaPlayer.GetCurrentNum() +1)
	+ "/" + MediaPlayer.PlayListGetSize() + ")";
else
	FORM->LblCaption->Text2 =
	" - MPlayer ("+
	String(MediaPlayer.GetCurrentNum() +1)
	+ "/" + CItem.Files + ")";
#endif

}

//==============================================================================

void Gui::CaptionSave(){

#ifdef __BORLANDC__
mCaption1Bak = FORM->LblCaption->Text1.c_str();
mCaption2Bak = FORM->LblCaption->Text2.c_str();
#endif

}

//==============================================================================

void Gui::CaptionSave(const string& txt1, const string& txt2){

mCaption1Bak = txt1;
mCaption2Bak = txt2;

}

//==============================================================================

void Gui::CaptionRevert(){

#ifdef __BORLANDC__
if (!FORM->ChkXct->Checked)
		NameCaption(CItem.OpenFileName.c_str());
else{
	FORM->LblCaption->Text1 = mCaption1Bak.c_str();
	FORM->LblCaption->Text2 = mCaption2Bak.c_str();
}
#endif

}

//==============================================================================

void Gui::Status (const string& caption, const int& sleep, const bool& refresh){

#ifdef __BORLANDC__
if (MediaPlayer.GetIsPlaying())
	return;

FORM->EkImgProgressBar->Text = caption.c_str();

if (refresh)
	FORM->EkImgProgressBar->Update();
#else
	#ifdef _XBOX
		xbApp.DrawStatusTxt(caption);
	#else
		cout<<"Status: "<<caption<<endl;
	#endif
#endif

Sleep((ulong)sleep);

}

//==============================================================================

void Gui::Error (const string& caption, const int& sleep, FILE* stream){

fclose(stream);
Status(caption, 0, true);
Sleep((ulong)sleep);
CItem.Error = true;

}

//==============================================================================

void Gui::Finish(){

CItem.Extracting = false;
#ifdef __BORLANDC__
FORM->EkImgProgressBar->Position = FORM->EkImgProgressBar->Max;
ExtractDlg->Caption = "100%";
#endif

}

//==============================================================================

// Loads region files contained in res file and applies them to the gui (Fast)
void Gui::WindRegion(const int& resNum, const HRGN& handle){

#ifdef __BORLANDC__
HINSTANCE hInstance = GetModuleHandle(NULL);
HRSRC region = FindResource(hInstance, MAKEINTRESOURCE(resNum), "BINARY");
LPRGNDATA data = (LPRGNDATA)LoadResource(NULL, region);
HRGN rgn = ExtCreateRegion(NULL, SizeofResource(NULL, region), data);
SetWindowRgn(handle, rgn, true);
#endif

//if (sys.GetWinVer() >= 5) {
//	SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED);
//	SetLayeredWindowAttributes(handle, 0x00FF00FF, 0, LWA_COLORKEY);
//}
}

//==============================================================================

void Gui::FormDoubleBufferedSet(const bool& value){

#ifdef __BORLANDC__
FORM->DoubleBuffered = false;
#endif

}

//==============================================================================

void Gui::FormUpdate(){

#ifdef __BORLANDC__
FORM->Update();
#endif

}

//==============================================================================

int Gui::FormWidthGet(){

#ifdef __BORLANDC__
return FORM->Width;
#else
return mMaxWidth;
#endif

}

//==============================================================================

void Gui::FormVisibleSet(const bool& value){

#ifdef __BORLANDC__
FORM->Visible = value;
#endif

}

//==============================================================================

void Gui::MaxWidthMode(){

#ifdef __BORLANDC__
FORM->DoubleBuffered = true;
if (mShowOptions)
	XpandItemsVisibile(true);
if (FORM->Height == gui.mMinHeight)
	gui.ToggleMiniMode();

// Dirty fix
Graphics::TBitmap* out = new Graphics::TBitmap;
out->LoadFromFile(ExtractFileDir(Application->ExeName) + SKINDIR + "BtnGuiWidthLOut.bmp");
FORM->BtnGuiWidth->SetPicOut(out);
out->Free();
// Dirty fix end
//LoadImg("BtnGuiWidthLOut", FORM->BtnGuiWidth->ImageOut, true);
LoadImg("BtnGuiWidthLOver", FORM->BtnGuiWidth->ImageOver, true);
LoadImg("BtnGuiWidthLDown", FORM->BtnGuiWidth->ImageDown, true);
LoadImg("WindMainXpand", FORM->ImgMain->Picture->Bitmap, false);
FORM->BtnGuiWidth->Left = 733;
FORM->Width = gui.mMaxWidth;
WindRegion(102, FORM->Handle);
FORM->TimrDblBuff->Enabled = true;

#endif

}

//==============================================================================

void Gui::MinWidthMode(){

#ifdef __BORLANDC__
FORM->DoubleBuffered = true;
XpandItemsVisibile(false);

// Dirty fix
Graphics::TBitmap* out = new Graphics::TBitmap;
out->LoadFromFile(ExtractFileDir(Application->ExeName) + SKINDIR + "BtnGuiWidthROut.bmp");
FORM->BtnGuiWidth->SetPicOut(out);
out->Free();
// Dirty fix end

LoadImg("BtnGuiWidthROver", FORM->BtnGuiWidth->ImageOver, true);
LoadImg("BtnGuiWidthRDown", FORM->BtnGuiWidth->ImageDown, true);
LoadImg("WindMain", FORM->ImgMain->Picture->Bitmap, false);
WindRegion(101, FORM->Handle);
FORM->BtnGuiWidth->Left = 429;
FORM->Width = gui.mMidWidth;
FORM->TimrDblBuff->Enabled = true;
#endif

}

//==============================================================================

void Gui::XpandItemsVisibile(const bool& state){

#ifdef __BORLANDC__
for (int i = 0; i < mCheckBoxes ; ++i)
	GetChkBox(i)->Visible = state;

FORM->BtnSelDir->Visible = state;
FORM->BtnIni->Visible = state;
FORM->LblRenameFiles->Visible = state;
FORM->LblFolderName->Visible = state;
FORM->EditRenFiles->Visible = state;
FORM->EditFolName->Visible = state;
#endif

}

//==============================================================================

void Gui::MiniHeightMode(const bool& onlyReposition){

#ifdef __BORLANDC__
FORM->DoubleBuffered = true;
int left = 76;
int top = 28;

FORM->LblCaption->Width = 308;
FORM->LblArchiveTxt->Top = top;
FORM->LblArchiveVal->Top = top;
FORM->LblAudioTxt->Top = top;
FORM->LblAudioVal->Top = top;
FORM->LblFilesTxt->Top = top;
FORM->LblFilesVal->Top = top;
FORM->LblSizeTxt->Top = top;
FORM->LblSizeVal->Top = top;

if (onlyReposition)
	return;

LoadImg("BtnGuiHeightDownD", FORM->BtnGuiHeight->ImageDown, false);
LoadImg("BtnGuiHeightOutD", FORM->BtnGuiHeight->ImageOut, false);
LoadImg("BtnGuiHeightOverD", FORM->BtnGuiHeight->ImageOver, false);
LoadImg("WindMini", FORM->ImgMain->Picture->Bitmap, false);
WindRegion(103, FORM->Handle);

FORM->LblCaption->Left = 26;
FORM->ImgIcon->Left = FORM->LblCaption->Left - 20;

FORM->BtnOpen->Top = 27;
FORM->BtnOpen->Left = FORM->ListView1->Left - 28;
FORM->BtnExt->Left = FORM->BtnOpen->Left;
FORM->BtnExt->Top = FORM->BtnOpen->Top + FORM->BtnOpen->Height - 3;
FORM->BtnScan->Left = FORM->BtnOpen->Left;
FORM->BtnScan->Top = FORM->BtnExt->Top + FORM->BtnExt->Height - 3;

FORM->EkImgProgressBar->Top = 55;
FORM->EkImgProgressBar->Width = 354;
FORM->EkImgProgressBar->Left = 126 - left;

FORM->BtnPrev->Top = FORM->EkImgProgressBar->Top + 20;
FORM->BtnPrev->Left = FORM->EkImgProgressBar->Left-1;
FORM->BtnPlay->Top = FORM->BtnPrev->Top;
FORM->BtnPlay->Left = FORM->BtnPrev->Left + 16;
FORM->BtnPause->Top = FORM->BtnPlay->Top;
FORM->BtnPause->Left = FORM->BtnPlay->Left + 16;
FORM->BtnStop->Top = FORM->BtnPause->Top;
FORM->BtnStop->Left = FORM->BtnPause->Left + 16;
FORM->BtnNext->Top = FORM->BtnStop->Top;
FORM->BtnNext->Left = FORM->BtnStop->Left + 16;
FORM->BtnLoopPlayList->Top = FORM->BtnNext->Top;
FORM->BtnLoopPlayList->Left = FORM->BtnNext->Left + 18;
FORM->BtnFade->Top = FORM->BtnLoopPlayList->Top;
FORM->BtnFade->Left = FORM->BtnLoopPlayList->Left + 16;
FORM->BtnClrPlayList->Top = FORM->BtnFade->Top;
FORM->BtnClrPlayList->Left = FORM->BtnFade->Left + 16;

FORM->EkszTrackBar->Top = FORM->BtnFade->Top + 5;
FORM->EkszTrackBar->Left = FORM->BtnClrPlayList->Left + 26;
FORM->EkszTrackBar->Width = FORM->EkImgProgressBar->Width - 140;

FORM->BtnGuiWidth->Visible = false;
FORM->ListView1->Visible = false;
FORM->Width = gui.mMinWidth;
FORM->Height = gui.mMinHeight;
FORM->TimrDblBuff->Enabled = true;
#endif

}

//==============================================================================

void Gui::NormHeightMode(const bool& onlyReposition){

#ifdef __BORLANDC__
FORM->DoubleBuffered = true;

if (!onlyReposition){
	LoadImg("WindMain", FORM->ImgMain->Picture->Bitmap, false);
	LoadImg("BtnGuiHeightOutU", FORM->BtnGuiHeight->ImageOut, false);
	LoadImg("BtnGuiHeightOverU", FORM->BtnGuiHeight->ImageOver, false);
	LoadImg("BtnGuiHeightDownU", FORM->BtnGuiHeight->ImageDown, false);
	FORM->LblCaption->Width = 287;
	FORM->BtnSelDir->Top = 291;
	FORM->BtnSelDir->Left = 482;
	FORM->BtnIni->Top = 291;
	FORM->BtnIni->Left = 511;

	WindRegion(101, FORM->Handle);
	FORM->LblCaption->Left = 47;
	FORM->ImgIcon->Left = FORM->LblCaption->Left - 20;
}
					   
int top = 349;
FORM->LblArchiveTxt->Top = top;
FORM->LblArchiveTxt->Left = 41;
FORM->LblArchiveVal->Top = top;
FORM->LblArchiveVal->Left = FORM->LblArchiveTxt->Left + FORM->LblArchiveTxt->Width + 5;
FORM->LblAudioTxt->Top = top;
FORM->LblAudioTxt->Left = 153;
FORM->LblAudioVal->Top = top;
FORM->LblAudioVal->Left = FORM->LblAudioTxt->Left + FORM->LblAudioTxt->Width + 5;
FORM->LblFilesTxt->Top = top;
FORM->LblFilesTxt->Left = 249;
FORM->LblFilesVal->Top = top;
FORM->LblFilesVal->Left = FORM->LblFilesTxt->Left + FORM->LblFilesTxt->Width + 5;
FORM->LblSizeTxt->Top = top;
FORM->LblSizeTxt->Left = 323;
FORM->LblSizeVal->Top = top;
FORM->LblSizeVal->Left = FORM->LblSizeTxt->Left + FORM->LblSizeTxt->Width + 5;
if (onlyReposition)
	return;

FORM->EkImgProgressBar->Width = 394;
FORM->EkImgProgressBar->Top = 308;
FORM->EkImgProgressBar->Left = 29;
FORM->BtnOpen->Top = FORM->ListView1->Top -5;

int left = 1;
FORM->BtnOpen->Left = left;
FORM->BtnExt->Top = FORM->BtnOpen->Top + FORM->BtnOpen->Height-3;
FORM->BtnExt->Left = left;
FORM->BtnScan->Top = FORM->BtnExt->Top + FORM->BtnExt->Height-3;
FORM->BtnScan->Left = left;

FORM->BtnPrev->Top = FORM->BtnScan->Top + 42;
FORM->BtnPrev->Left = FORM->BtnScan->Left+2;   //+3
FORM->BtnPlay->Top = FORM->BtnPrev->Top + 17;
FORM->BtnPlay->Left = FORM->BtnPrev->Left;
FORM->BtnPause->Top = FORM->BtnPlay->Top + 17;
FORM->BtnPause->Left = FORM->BtnPlay->Left;
FORM->BtnStop->Top = FORM->BtnPause->Top + 17;
FORM->BtnStop->Left = FORM->BtnPause->Left;
FORM->BtnNext->Top = FORM->BtnStop->Top + 17;
FORM->BtnNext->Left = FORM->BtnStop->Left;

FORM->BtnLoopPlayList->Top = FORM->BtnNext->Top + 19;
FORM->BtnLoopPlayList->Left = FORM->BtnNext->Left;
FORM->BtnFade->Top = FORM->BtnLoopPlayList->Top + 17;
FORM->BtnFade->Left = FORM->BtnLoopPlayList->Left;
FORM->BtnClrPlayList->Top = FORM->BtnFade->Top + 17;
FORM->BtnClrPlayList->Left = FORM->BtnFade->Left;

FORM->EkszTrackBar->Top = 331;
FORM->EkszTrackBar->Left = FORM->EkImgProgressBar->Left;
FORM->EkszTrackBar->Width = FORM->EkImgProgressBar->Width;

FORM->BtnGuiWidth->Visible = true;
FORM->Width = gui.mMidWidth;
FORM->Height = gui.mMaxHeight;
if (!FORM->ListView1->Visible)
	FORM->ListView1->Visible = true;
FORM->TimrDblBuff->Enabled = true;
#endif

}

//==============================================================================

void Gui::ToggleMiniMode(){

#ifdef __BORLANDC__
if (FORM->Height >= gui.mMaxHeight - 5)
	MiniHeightMode(false);
else
	NormHeightMode(false);
#endif

}

//==============================================================================

void Gui::ToggleXpandMode(){

#ifdef __BORLANDC__
if (FORM->Width == gui.mMinWidth || FORM->Width == gui.mMidWidth)
	MaxWidthMode();
else
	MinWidthMode();
#endif

}

//==============================================================================

void Gui::CenterForm(){ // Manually center form

#ifdef __BORLANDC__
SIZE size; // Get screen resolution
size.cx = GetSystemMetrics(SM_CXSCREEN); // e.g. 1024
size.cy = GetSystemMetrics(SM_CYSCREEN); // e.g. 768
FORM->Top = (size.cy >> 1) - (FORM->Height >> 1);
FORM->Left = (size.cx >> 1) - (FORM->Width >> 1);
#endif

// FORM->Position = poDesktopCenter;  <- Caused 2 bugs
// (1) Drag & drop would stop working if autolist was on & form
// was in compact mode last time app was run.
// (2) Listview checkboxes were changed back to standard issue ones.

}

//==============================================================================

void Gui::OptionsToggleViewMode(){

#ifdef __BORLANDC__
bool state = !FORM->BtnSelDir->Visible;
mShowOptions = state;

FORM->DoubleBuffered = true;

if (state)
	FORM->LblOptions->Caption = "Options";
else
	FORM->LblOptions->Caption = "Information";

// Options Hide
FORM->BtnSelDir->Visible=state;
FORM->LblBuf->Visible=state;
FORM->LblRenameFiles->Visible=state;
FORM->LblFolderName->Visible=state;
FORM->LblFmt->Visible=state;
FORM->LblChan->Visible=state;
FORM->LblSmpRate->Visible=state;
FORM->LblBits->Visible=state;
FORM->ComboBufSize->Visible=state;

for (int i = 0; i < mCheckBoxes ; ++i)
	GetChkBox(i)->Visible = state;

FORM->BtnIni->Visible=state;

FORM->ComboFormat->Visible=state;
FORM->ComboChan->Visible=state;
FORM->ComboSmpRate->Visible=state;
FORM->ComboBits->Visible=state;
FORM->EditRenFiles->Visible=state;
FORM->EditFolName->Visible=state;

// Info Show
int left = 517;
FORM->LblInfo02->Left=left;
FORM->LblInfo03->Left=left;
FORM->LblInfo04->Left=left;
FORM->LblInfo05->Left=left;
FORM->LblInfo06->Left=left;
FORM->LblInfo07->Left=left;
FORM->LblInfo08->Left=left;
FORM->LblInfo09->Left=left;
FORM->LblInfo01->Left=left;
FORM->LblXctEntriesTxt->Left=left;
FORM->LblXctEntriesVal->Left = left + FORM->LblXctEntriesTxt->Width + 5;

FORM->LblInfo02->Visible=!state;
FORM->LblInfo03->Visible=!state;
FORM->LblInfo04->Visible=!state;
FORM->LblInfo05->Visible=!state;
FORM->LblInfo06->Visible=!state;
FORM->LblInfo07->Visible=!state;
FORM->LblInfo08->Visible=!state;
FORM->LblInfo09->Visible=!state;
FORM->LblInfo01->Visible=!state;
FORM->LblXctEntriesTxt->Visible=!state;
FORM->LblXctEntriesVal->Visible=!state;
									
FORM->DoubleBuffered = false;
#endif

}

//==============================================================================

void Gui::Open(const char* caption1, const char* caption2, const char* captionDlg){

this->ProgressReset();
this->ProgressCaptionBasic(caption2, false);
#ifdef __BORLANDC__
ExtractDlg->LblCaption->Caption = captionDlg;
#endif

CItem.Extracting = true;
Status(caption1, 0, false);

#ifdef __BORLANDC__
if (!ExtractDlg->Showing){
	FORM->EkImgProgressBar->TextEllipse = etlEndEllipsis;

	int tweak = 43;
	if (FORM->Height == mMaxHeight) // Tweak top pos in compact mode, else tweak = 0
		tweak = 0;
	int topPos = (FORM->Height >> 1) - (ExtractDlg->Height >> 1) + FORM->Top + tweak;
	int leftPos = (FORM->Width >> 1) - (ExtractDlg->Width >> 1) + FORM->Left;

	ExtractDlg->Top =  topPos;
	ExtractDlg->Left = leftPos;

	FORM->Update();
	ExtractDlg->Show();

	sys.TimerStart();
}
#else
sys.TimerStart();
#endif

}

//==============================================================================

void Gui::OpenMedia(){

CItem.Extracting = true;
ProgressFxOff();
#ifdef __BORLANDC__
FORM->EkImgProgressBar->Animation = true;
#endif

}

//==============================================================================

void Gui::Close(){

#ifdef __BORLANDC__
ExtractDlg->Close();
#endif

}

//==============================================================================

void Gui::ScxLblSetup(){

#ifdef __BORLANDC__
if (!CItem.ScxMode) // Norm
{
	FORM->ComboBufSize->Enabled = false;
	FORM->ComboSmpRate->Enabled = true;
	FORM->ComboFormat->Enabled = true;
	FORM->ComboChan->Enabled = true;
	FORM->ComboBits->Enabled = true;
	FORM->LblFilesTxt->Caption = "Files:";
	FORM->LblFilesTxt->Hint = "Amount of files contained within file/archive";
	FORM->LblArchiveTxt->Caption = "Archive:";
	FORM->LblArchiveTxt->Hint = "Format of loaded file";
	FORM->LblAudioTxt->Caption = "Audio:";

}
else// Scx
{
	FORM->ComboBufSize->Enabled = true;
	FORM->ComboSmpRate->Enabled = false;
	FORM->ComboFormat->Enabled = false;
	FORM->ComboChan->Enabled = false;
	FORM->ComboBits->Enabled = false;
	FORM->LblFilesTxt->Caption = "Found:";
	FORM->LblArchiveTxt->Caption = "Extracted:";
	FORM->LblAudioTxt->Caption = "Buffer:";
	FORM->LblFilesTxt->Hint = "No of files found";
	FORM->LblArchiveTxt->Hint = "No of files extracted";

	FORM->LblCaption->Text1 = "SCX Mode - ";
	FORM->LblCaption->Text2 = "High Speed Brute Force File Scanner";

	if (FORM->ChkAutoEditClr->Checked) FORM->EditFolName->Clear();
	gui.Status("Buffer re-enabled, adjust as required", 0, false);
}

FORM->LblArchiveVal->Left = FORM->LblArchiveTxt->Left
+ FORM->LblArchiveTxt->Width + 5;

FORM->LblAudioVal->Left = FORM->LblAudioTxt->Left
+ FORM->LblAudioTxt->Width + 5;

FORM->LblFilesVal->Left = FORM->LblFilesTxt->Left
+ FORM->LblFilesTxt->Width + 5;

if (FORM->Height == mMinHeight)
	MiniHeightMode(true);
else
	NormHeightMode(true);
#endif

}

//==============================================================================

void Gui::ClearInfo(){

Caption("");

#ifdef __BORLANDC__
FORM->LblInfo02->Caption = "";
FORM->LblInfo03->Caption = "";
FORM->LblInfo04->Caption = "";
FORM->LblInfo05->Caption = "";
FORM->LblInfo06->Caption = "";
FORM->LblInfo07->Caption = "";
FORM->LblInfo08->Caption = "";
FORM->LblInfo09->Caption = "";
FORM->LblSizeVal->Caption = "";
FORM->LblFilesVal->Caption = "";
FORM->LblArchiveVal->Caption = "";
FORM->LblAudioVal->Caption = "";
FORM->LblInfo01->Caption = "";
FORM->ListView1->Clear();
#endif

}

//==============================================================================

void Gui::ExtInfo(const int& files, const vector<string>& ext){

struct{
	bool adx;
	bool asf;
	bool mp3;
	bool ogg;
	bool wav;
	bool wma;
	bool xma;
}format = {0, 0, 0, 0, 0, 0};

         
for (int x = 0; x < files; ++x)
{
	if (ext[x] == ".adx")
		format.adx = true;
	if (ext[x] == ".asf")
		format.asf = true;
	else if (ext[x] == ".mp3")
		format.mp3 = true;
	else if (ext[x] == ".ogg")
		format.ogg = true;
	else if (ext[x] == ".wav")
		format.wav = true;
	else if (ext[x] == ".wma")
		format.wma = true;
	else if (ext[x] == ".xma")
		format.xma = true;
}

#ifdef __BORLANDC__

if ( (format.adx && format.mp3) || (format.adx && format.ogg) ||
	(format.adx && format.wav) || (format.adx && format.wma)
	|| (format.adx && format.xma)
	|| (format.mp3 && format.ogg) ||
	(format.mp3 && format.wav) || (format.mp3 && format.wma)
	|| (format.mp3 && format.xma)
	|| (format.ogg && format.wav) || (format.ogg && format.wma)
	|| (format.ogg && format.xma)
	|| (format.wav && format.wma) || (format.wav && format.xma) )
	FORM->LblAudioVal->Caption = "Multi";
else if (format.adx)
	FORM->LblAudioVal->Caption = "Adx";
else if (format.asf)
	FORM->LblAudioVal->Caption = "Asf";
else if (format.mp3)
	FORM->LblAudioVal->Caption = "Mp3";
else if (format.ogg)
	FORM->LblAudioVal->Caption = "Ogg";
else if (format.wav)
	FORM->LblAudioVal->Caption = "Wav";
else if (format.wma)
	FORM->LblAudioVal->Caption = "Wma";
else if (format.xma)
	FORM->LblAudioVal->Caption = "Xma";
else
	FORM->LblAudioVal->Caption = "None";

#endif

}

//==============================================================================

void Gui::SetInfoLbl(int labelNum, const string& txt){

#ifdef __BORLANDC__
if (labelNum == 1)
	FORM->LblInfo01->Caption = txt.c_str();

else if (labelNum == 2)
	FORM->LblInfo02->Caption = txt.c_str();

else if (labelNum == 3)
	FORM->LblInfo03->Caption = txt.c_str();

else if (labelNum == 4)
	FORM->LblInfo04->Caption = txt.c_str();

else if (labelNum == 5)
	FORM->LblInfo05->Caption = txt.c_str();

else if (labelNum == 6)
	FORM->LblInfo06->Caption = txt.c_str();

else if (labelNum == 7)
	FORM->LblInfo07->Caption = txt.c_str();

else if (labelNum == 8)
	FORM->LblInfo08->Caption = txt.c_str();

else if (labelNum == 9)
	FORM->LblInfo09->Caption = txt.c_str();
#endif

}

//==============================================================================

void Gui::OnTop(){

#ifdef __BORLANDC__
FORM->PopMenuOnTop->Checked = ! FORM->PopMenuOnTop->Checked;
FORM->FormStyle = FORM->PopMenuOnTop->Checked ? fsStayOnTop : fsNormal;
#endif

}

//==============================================================================

void Gui::ScreenSnap(){

#ifdef __BORLANDC__
FORM->PopMenuScreenSnap->Checked = ! FORM->PopMenuScreenSnap->Checked;
FORM->ScreenSnap = FORM->PopMenuScreenSnap->Checked;
#endif

}

//==============================================================================

void Gui::ShowHints(){

#ifdef __BORLANDC__
FORM->PopMenuShowHints->Checked = ! FORM->PopMenuShowHints->Checked;
Application->ShowHint = FORM->PopMenuShowHints->Checked;
#endif

}

//==============================================================================

void Gui::FadeFx(){

#ifdef __BORLANDC__
FORM->PopMenuFadeFx->Checked = ! FORM->PopMenuFadeFx->Checked;

bool fxEnable = FORM->PopMenuFadeFx->Checked;

for (int i = 0; i < gui.mCheckBoxes ; ++i){
	gui.GetChkBox(i)->FadeEnable = fxEnable;
}

FORM->BtnMin->FadeEnable = fxEnable;
FORM->BtnGuiHeight->FadeEnable = fxEnable;
FORM->BtnClose->FadeEnable = fxEnable;
FORM->BtnGuiWidth->FadeEnable = fxEnable;

FORM->BtnPrev->FadeEnable = fxEnable;
FORM->BtnPlay->FadeEnable = fxEnable;
FORM->BtnPause->FadeEnable = fxEnable;
FORM->BtnStop->FadeEnable = fxEnable;
FORM->BtnNext->FadeEnable = fxEnable;
FORM->BtnFade->FadeEnable = fxEnable;
FORM->BtnClrPlayList->FadeEnable = fxEnable;

// Player toggle btn
FORM->BtnLoopPlayList->FadeEnable = fxEnable;

// Options
FORM->BtnIni->FadeEnable = fxEnable;
FORM->BtnSelDir->FadeEnable = fxEnable;
FORM->BtnTgle->FadeEnable = fxEnable;

FORM->BtnOpen->FadeEnable = fxEnable;
FORM->BtnExt->FadeEnable = fxEnable;
FORM->BtnScan->FadeEnable = fxEnable;
#endif

}

//==============================================================================

void Gui::ScrollingText(){

#ifdef __BORLANDC__
FORM->PopMenuScrollingText->Checked = ! FORM->PopMenuScrollingText->Checked;
mScrollTxt = FORM->PopMenuScrollingText->Checked;
#endif

}

//==============================================================================

void Gui::AnimatedProgressBar(){

#ifdef __BORLANDC__
FORM->PopMenuAnimatedProgressBar->Checked = ! FORM->PopMenuAnimatedProgressBar->Checked;
mAnimatedProgressBar = FORM->PopMenuAnimatedProgressBar->Checked;
#endif

}

//==============================================================================

void Gui::CursorStartTimer(){

mCursorTime = GetTickCount();
mCursorIsHidden = false;

}

//==============================================================================

bool Gui::CursorHasTimedOut(){

if ((GetTickCount() - mCursorTime) >= 4096){
	//mCursorIsHidden = true;
	return true;
}

return false;

}

//==============================================================================

#ifdef __BORLANDC__

void __fastcall Gui::LoadImg(const String& name, Graphics::TBitmap* dest, const bool& transparent){

Graphics::TBitmap* bufImg = new Graphics::TBitmap;

try
{
	bufImg->LoadFromFile(ExtractFileDir(Application->ExeName) + String(SKINDIR) + name + ".bmp");
}
catch(Exception &e){
	gui.MsgError("Error Loading: " + str.AnsiToStd(name));
}

dest->Assign(bufImg);
if (transparent)
	dest->Transparent = true;
bufImg->Free();

}

//==============================================================================

void __fastcall Gui::LoadGraphics(){

Graphics::TBitmap* btnSmOut = new Graphics::TBitmap;
Graphics::TBitmap* btnSmOver = new Graphics::TBitmap;
Graphics::TBitmap* btnSmDown = new Graphics::TBitmap;

Graphics::TBitmap* btnOut = new Graphics::TBitmap;
Graphics::TBitmap* btnOver = new Graphics::TBitmap;
Graphics::TBitmap* btnDown = new Graphics::TBitmap;

Graphics::TBitmap* chkOut = new Graphics::TBitmap;
Graphics::TBitmap* chkOver = new Graphics::TBitmap;
Graphics::TBitmap* chkDown = new Graphics::TBitmap;
Graphics::TBitmap* unchkOut = new Graphics::TBitmap;
Graphics::TBitmap* unchkOver = new Graphics::TBitmap;


try
{
	String appPath = ExtractFileDir(Application->ExeName);
	String path = appPath + String(SKINDIR);

	// Load small button images
	btnSmOut->Transparent = true;
	btnSmOver->Transparent = true;
	btnSmDown->Transparent = true;

	FORM->EkImgProgressBar->ImageBack->LoadFromFile(path + "ProgressBack.bmp");
	FORM->EkImgProgressBar->ImageFront->LoadFromFile(path + "ProgressFront.bmp");

	// Player
	btnSmOut->LoadFromFile(path + "btnplyOut.bmp");
	btnSmOver->LoadFromFile(path + "btnplyOver.bmp");
	btnSmDown->LoadFromFile(path + "btnplyDown.bmp");

	FORM->BtnPrev->Width = btnSmOut->Width;
	FORM->BtnPrev->Height = btnSmOut->Height;
	FORM->BtnPrev->ImageOut->Assign(btnSmOut);
	FORM->BtnPrev->ImageOver->Assign(btnSmOver);
	FORM->BtnPrev->ImageDown->Assign(btnSmDown);

	FORM->BtnPlay->ImageOut->Assign(btnSmOut);
	FORM->BtnPlay->ImageOver->Assign(btnSmOver);
	FORM->BtnPlay->ImageDown->Assign(btnSmDown);

	FORM->BtnPause->ImageOut->Assign(btnSmOut);
	FORM->BtnPause->ImageOver->Assign(btnSmOver);
	FORM->BtnPause->ImageDown->Assign(btnSmDown);

	FORM->BtnStop->ImageOut->Assign(btnSmOut);
	FORM->BtnStop->ImageOver->Assign(btnSmOver);
	FORM->BtnStop->ImageDown->Assign(btnSmDown);

	FORM->BtnNext->ImageOut->Assign(btnSmOut);
	FORM->BtnNext->ImageOver->Assign(btnSmOver);
	FORM->BtnNext->ImageDown->Assign(btnSmDown);

	FORM->BtnFade->ImageOut->Assign(btnSmOut);
	FORM->BtnFade->ImageOver->Assign(btnSmOver);
	FORM->BtnFade->ImageDown->Assign(btnSmDown);

	FORM->BtnClrPlayList->ImageOut->Assign(btnSmOut);
	FORM->BtnClrPlayList->ImageOver->Assign(btnSmOver);
	FORM->BtnClrPlayList->ImageDown->Assign(btnSmDown);

	// Player toggle btn
	FORM->BtnLoopPlayList->ImgCheck->LoadFromFile(path + "BtnPlyTglSelOut.bmp");
	FORM->BtnLoopPlayList->ImgCheckOver->LoadFromFile(path + "BtnPlyTglSelOver.bmp");
	FORM->BtnLoopPlayList->ImgDown->LoadFromFile(path + "BtnPlyTglDown.bmp");
	FORM->BtnLoopPlayList->ImgUnCheck->LoadFromFile(path + "BtnPlyTglUnSelOut.bmp");
	FORM->BtnLoopPlayList->ImgUnCheckOver->LoadFromFile(path + "BtnPlyTglUnSelOver.bmp");

	// Options
	btnSmOut->LoadFromFile(appPath + String(BTNOPTOUT));
	btnSmOver->LoadFromFile(appPath + String(BTNOPTOVER));
	btnSmDown->LoadFromFile(appPath + String(BTNOPTDOWN));
	FORM->BtnIni->Width = btnSmOut->Width;
	FORM->BtnIni->Height = btnSmOut->Height;
	FORM->BtnIni->ImageOut->Assign(btnSmOut);
	FORM->BtnIni->ImageOver->Assign(btnSmOver);
	FORM->BtnIni->ImageDown->Assign(btnSmDown);

	FORM->BtnSelDir->Width = btnSmOut->Width;
	FORM->BtnSelDir->Height = btnSmOut->Height;
	FORM->BtnSelDir->ImageOut->Assign(btnSmOut);
	FORM->BtnSelDir->ImageOver->Assign(btnSmOver);
	FORM->BtnSelDir->ImageDown->Assign(btnSmDown);

	FORM->BtnTgle->Width = btnSmOut->Width;
	FORM->BtnTgle->Height = btnSmOut->Height;
	FORM->BtnTgle->ImageOut->Assign(btnSmOut);
	FORM->BtnTgle->ImageOver->Assign(btnSmOver);
	FORM->BtnTgle->ImageDown->Assign(btnSmDown);
	// End load small button images


	// Load large button images
	btnOut->Transparent = true;
	btnOver->Transparent = true;
	btnDown->Transparent = true;

	btnOut->LoadFromFile(path + "BtnATopOut.bmp");
	btnOver->LoadFromFile(path + "BtnATopOver.bmp");
	btnDown->LoadFromFile(path + "BtnATopDown.bmp");

	FORM->BtnOpen->Width = btnOut->Width;
	FORM->BtnOpen->Height = btnOut->Height;
	FORM->BtnOpen->ImageOut->Assign(btnOut);
	FORM->BtnOpen->ImageOver->Assign(btnOver);
	FORM->BtnOpen->ImageDown->Assign(btnDown);

	btnOut->LoadFromFile(path + "BtnAMidOut.bmp");
	btnOver->LoadFromFile(path + "BtnAMidOver.bmp");
	btnDown->LoadFromFile(path + "BtnAMidDown.bmp");

	FORM->BtnExt->Width = btnOut->Width;
	FORM->BtnExt->Height = btnOut->Height;
	FORM->BtnExt->ImageOut->Assign(btnOut);
	FORM->BtnExt->ImageOver->Assign(btnOver);
	FORM->BtnExt->ImageDown->Assign(btnDown);

	btnOut->LoadFromFile(path + "BtnABtmOut.bmp");
	btnOver->LoadFromFile(path + "BtnABtmOver.bmp");
	btnDown->LoadFromFile(path + "BtnABtmDown.bmp");

	FORM->BtnScan->Width = btnOut->Width;
	FORM->BtnScan->Height = btnOut->Height;
	FORM->BtnScan->ImageOut->Assign(btnOut);
	FORM->BtnScan->ImageOver->Assign(btnOver);
	FORM->BtnScan->ImageDown->Assign(btnDown);
	// End load button images
           

	// Load Checkbox images
	chkOut->Transparent = true;
	chkOver->Transparent = true;
	chkDown->Transparent = true;
	unchkOut->Transparent = true;
	unchkOver->Transparent = true;

	chkOut->PixelFormat = pf24bit;
	chkOver->PixelFormat = pf24bit;
	chkDown->PixelFormat = pf24bit;
	unchkOut->PixelFormat = pf24bit;
	unchkOver->PixelFormat = pf24bit;

	chkOut->LoadFromFile(appPath + String(CHKOUT));
	chkOver->LoadFromFile(appPath + String(CHKOVER));
	chkDown->LoadFromFile(appPath + String(CHKDOWN));
	unchkOut->LoadFromFile(appPath + String(UCHKOUT));
	unchkOver->LoadFromFile(appPath + String(UCHKOVER));

	for (int i = 0; i < gui.mCheckBoxes ; ++i){
		gui.GetChkBox(i)->Width = chkOut->Width;
		gui.GetChkBox(i)->Height = chkOut->Height;
		gui.GetChkBox(i)->ImgCheck->Assign(chkOut);
		gui.GetChkBox(i)->ImgCheckOver->Assign(chkOver);
		gui.GetChkBox(i)->ImgDown->Assign(chkDown);
		gui.GetChkBox(i)->ImgUnCheck->Assign(unchkOut);
		gui.GetChkBox(i)->ImgUnCheckOver->Assign(unchkOver);
	}

	// End load Checkbox images

	//Load list images
	FORM->ListView1->StateImages->Width = unchkOut->Width;
	FORM->ListView1->StateImages->Height = unchkOut->Height;
	FORM->ImgListGui->Width = unchkOut->Width;
	FORM->ImgListGui->Height = unchkOut->Height;

	// Get bottom left pixel
	TColor color = (TColor)GetPixel(unchkOut->Canvas->Handle, 0, unchkOut->Height - 1);

	FORM->ImgListGui->AddMasked(unchkOut, color);
	FORM->ImgListGui->AddMasked(chkOut, color);

	delete btnSmOut;
	delete btnSmOver;
	delete btnSmDown;
	delete btnOut;
	delete btnOver;
	delete btnDown;

	delete chkOut;
	delete chkOver;
	delete chkDown;
	delete unchkOut;
	delete unchkOver;

}
catch (Exception &e)
{
	delete btnSmOut;
	delete btnSmOver;
	delete btnSmDown;
	delete btnOut;
	delete btnOver;
	delete btnDown;

	delete chkOut;
	delete chkOver;
	delete chkDown;
	delete unchkOut;
	delete unchkOver;
	gui.MsgError("Error Loading Graphics - App Will Now Terminate");
	Application->Terminate();
}

}// End load graphics

#endif

//==============================================================================

Gui gui; // Global instance


