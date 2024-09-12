


//=============================
//		Gui.cpp
//=============================

#include "stdafx.h" 

#include "FileSpecs.h"
#include "SystemX.h"
#include "Gui.h"
#include "Helper.h"
//#include "Main.h"
#include "Strings.h"
#ifdef _XBOX
#include "../XBOX/XBOXABX.h"
#endif
//#include "LoadGraphics.cpp"
//#include "MediaPlayer.h"


#define FORM Form1

//------------------------------------------------------------------------------

Gui::Gui(){

// BCB Specific
//AppDir = ExtractFileDir(Application->ExeName);
#ifndef _XBOX
AppDir = ExtractFileDir(__argv[0]);
#endif
#ifdef _XBOX
AppDir = ExtractFileDir("d:\\");
#endif

}

//===========================================================================

Gui::~Gui(){


}

//===========================================================================

void Gui::Msg (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, 0);
#endif

}

void Gui::MsgA (const string& msg, const char* caption){

#ifdef _WINDOWS
MessageBoxA(NULL, msg.c_str(), caption, 0);
#endif

}

void Gui::MsgInfo (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, MB_ICONINFORMATION);
#endif

}

bool Gui::MsgWarning (const string& msg){

#ifdef _WINDOWS
if (MB_OK == MessageBox(NULL, msg.c_str(), NULL, MB_OKCANCEL | MB_ICONWARNING)) 
	return true;
#endif

return false;

}

void Gui::MsgError (const string& msg){

#ifdef _WINDOWS
MessageBox(NULL, msg.c_str(), NULL, MB_OK|MB_ICONERROR);
#endif

}


//===========================================================================

void Gui::FinishDisplay(int& extracted, int& filesTotal,
			const double& time, string statusCaption, string progressCaption){

// BCB Specific
//FORM->EkImgProgressBar->Text = progressCaption;

if (time > 60)
{
	int min = (int)time / 60;
	int sec = (int)time % 60;
	Status(statusCaption + " " + IntToStr(extracted) + " File/s in "
	+ IntToStr(min) + " Min/s " + IntToStr(sec) + " Sec/s", 0);
}
else
{
	Status(statusCaption + " " + IntToStr(extracted)
	+ " File/s in " + IntToStr((int)time) + " Sec/s", 0);
}

if (filesTotal != extracted)
{
	Status(IntToStr(filesTotal - extracted) 
	+ " file/s not extracted due to error/s", 0);
	//MsgError(string(filesTotal - extracted)
	//+ " file/s not extracted due to error/s");
}

//ProgressMaxOut();

Finish();
extracted = 0,
filesTotal = 0;


}// End FinishDisplay

//===========================================================================

void Gui::SpeedHackEnable(const bool& value){

if (SpeedOn != value){
	SpeedOn = value;
	// BCB Specific
	//FORM->TimrRefreshRate->Enabled = value;
}

}

//===========================================================================

void Gui::NameCaption(const string& name){
// BCB Specific
//name = ExtractFileName(name);
//FORM->LblCaption->Text1 = APPNAME;
//FORM->LblCaption->Text2 = " - " + name;
//name = ExtractFileName(name);
//FORM->LblCaption->Text1 = name;
//FORM->LblCaption->Text2 = " - " + string(APPNAME);
}

//===========================================================================

void Gui::Caption(const string& txt){

// BCB Specific
//FORM->LblCaption->Text1 = APPNAME;
//if (txt.Length())
//	FORM->LblCaption->Text2 = " - " +  txt;
//else
//	FORM->LblCaption->Text2 = "";
//
//FORM->LblCaption->Text1 = txt;
//if (!txt.Length())
//	FORM->LblCaption->Text2 = APPNAME;
//else
//	FORM->LblCaption->Text2 = " - " + string(APPNAME);

}


//===========================================================================

void Gui::Status (const string& caption, const int& sleep){

// BCB Specific
//if (MediaPlayer.GetIsPlaying()) return;
	//gui.ProgressSetScroll(false);

//FORM->EkImgProgressBar->Text = caption;

//if (refresh)
//FORM->EkImgProgressBar->Update();

//if (MediaPlayer.GetIsPlaying()){
//	Sleep(300);
//	MediaPlayer.SetIsScrolling(false);
//}

#ifdef _XBOX
	xbApp.DrawStatusTxt(caption);
#else
	cout<<"Status: "<<caption<<endl;
#endif

Sleep((ulong)sleep);

}

//===========================================================================

void Gui::Error (const string& caption, const int& sleep, FILE* stream){
if (stream)
	fclose(stream);

#ifdef _XBOX
	xbApp.DrawStatusTxt(caption);
#else
	cout<<"Error: "<<caption<<endl;
#endif

Sleep((ulong)sleep);
CItem.Error = true;

}

//===========================================================================

void Gui::Finish(){

CItem.Extracting = false;
// BCB Specific
//FORM->EkImgProgressBar->Position = 100;
//ExtractDlg->Caption = "100%";

}

//===========================================================================

void Gui::SetLblFileType(const string& txt){

#ifdef _BCB
Form1->LblArchiveVal->Caption = txt;
#endif

}

//===========================================================================

Gui gui; // Global instance


