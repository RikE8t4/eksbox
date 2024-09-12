//---------------------------------------------------------------------
#include <vcl.h>
#include "Main.h"
#pragma hdrstop
#include "Extract.h"
#include "SystemX.h"
#include "Gui.h"
#include "Open.h"

//void LoadOKDlgGraphics();
//
//#define SKINDIR 	"\\Graphics\\"
//#define BTNOUT 	SKINDIR 	"btnOut.bmp"
//#define BTNOVER 	SKINDIR 	"btnOver.bmp"
//#define BTNDOWN 	SKINDIR 	"btnDown.bmp"

//---------------------------------------------------------------------
#pragma link "EkImgBtn"
#pragma resource "ExtractRegions.res"
#pragma link "EkFadeImgBase"
#pragma resource "*.dfm"
TExtractDlg *ExtractDlg;

//---------------------------------------------------------------------

__fastcall TExtractDlg::TExtractDlg(TComponent* AOwner)
	: TForm(AOwner)
{

//LoadOKDlgGraphics();

//if (Form1->OnTop1->Checked)
//// fsStayOnTop workaround, (Req. due to TJvBackgroundImage bug)
//{
//	//Form1->FormStyle = fsNormal;
//	Form1->FormStyle = fsStayOnTop;
//}
//else
//{
//	Form1->FormStyle = fsNormal;
//}//gui.FontSetupExtract();
				  
try
{
	BGImg->Picture->LoadFromFile
	(ExtractFileDir(Application->ExeName) + "\\Graphics\\extract.bmp");
}
	catch(Exception &e)
{
	gui.MsgError("Error Loading Graphics - App Will Now Terminate");
	Application->Terminate();
}

gui.WindRegion(104, Handle);

if (Form1->OpenDialog->Files->Count)
{
	Open open; open.DoOpen("");
	Form1->OpenDialog->Files->Clear();
}



}
//---------------------------------------------------------------------

void __fastcall TExtractDlg::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if (Key == VK_ESCAPE)
{
	gui.CancelAllSet(true);
	gui.Status("Canceled", 0, true);
	Show();
	Close();
}
}
//---------------------------------------------------------------------------

void __fastcall TExtractDlg::FormShow(TObject *Sender)
{
Form1->Enabled = 0;
//CancelBtn->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TExtractDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
Form1->Enabled = 1;
}
//---------------------------------------------------------------------------

void __fastcall TExtractDlg::BGImgMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
// Form Drag
if (Button == mbLeft){
	ReleaseCapture();
	SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}
}
//---------------------------------------------------------------------------

//void LoadOKDlgGraphics(){
//
//Graphics::TBitmap* btnOut = new Graphics::TBitmap;
//Graphics::TBitmap* btnOver = new Graphics::TBitmap;
//Graphics::TBitmap* btnDown = new Graphics::TBitmap;
//
//try {
//	String appPath = ExtractFileDir(Application->ExeName);
//
//	// Load large button images
//	btnOut->Transparent = true;
//	btnOver->Transparent = true;
//	btnDown->Transparent = true;
//
//	btnOut->LoadFromFile(appPath + String(BTNOUT));
//	btnOver->LoadFromFile(appPath + String(BTNOVER));
//	btnDown->LoadFromFile(appPath + String(BTNDOWN));
//
//	// Extract Dlg
//	ExtractDlg->CancelAllBtn->Width = btnOut->Width;
//	ExtractDlg->CancelAllBtn->Height = btnOut->Height;
//	ExtractDlg->CancelAllBtn->ImageOut->Assign(btnOut);
//	ExtractDlg->CancelAllBtn->ImageOver->Assign(btnOver);
//	ExtractDlg->CancelAllBtn->ImageDown->Assign(btnDown);
//
//	ExtractDlg->CancelCurrentBtn->Width = btnOut->Width;
//	ExtractDlg->CancelCurrentBtn->Height = btnOut->Height;
//	ExtractDlg->CancelCurrentBtn->ImageOut->Assign(btnOut);
//	ExtractDlg->CancelCurrentBtn->ImageOver->Assign(btnOver);
//	ExtractDlg->CancelCurrentBtn->ImageDown->Assign(btnDown);
//}
//catch (Exception &e){
//	gui.MsgError("Error loading graphics - App will now terminate");
//	Application->Terminate();
//}
//
//delete btnOut;
//delete btnOver;
//delete btnDown;
//
//}

void __fastcall TExtractDlg::BtnStopClick(TObject *Sender)
{
// Needs to first be reshown to fix a bug when app has been
// minimized whilst processing
//Show();
Close();
}
//---------------------------------------------------------------------------
void __fastcall TExtractDlg::BtnStopAllClick(TObject *Sender)
{
gui.CancelAllSet(true);
gui.Status("Canceled", 0, true);
//Show();
Close();
}
//---------------------------------------------------------------------------

