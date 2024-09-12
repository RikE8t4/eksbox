//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Video.h"
#include "Main.h"
#include "Open.h"
#include "Gui.h"
#include "SystemX.h"
#include "MediaPlayer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVideoWnd *VideoWnd;
//---------------------------------------------------------------------------
__fastcall TVideoWnd::TVideoWnd(TComponent* Owner)
	: TForm(Owner)
{

bool error = false;
for (int x = 0; x < ParamCount();) // Get files which were dropped on exe
{   // Process whole directory

	if (DirectoryExists(ParamStr(x+1)) && !error)
	{ // Process whole directory
		//sys.GetAllFiles(ParamStr(x+1), true);
		gui.MsgError("Error: Directory drop not currently supported");
		error = true;
	}
	else
	//	sys.GetAllFiles(ParamStr(x+1), true);
	//else
		Form1->OpenDialog->Files->Add(ParamStr(++x)); //_argv[1]
	//if (MediaPlayer.IsValidFormat(ParamStr(x)))
}


if (Form1->OpenDialog->Files->Count && !error){
//ShowMessage("");
Open open; open.DoOpen("");
Form1->OpenDialog->Files->Clear();
}


}
//---------------------------------------------------------------------------

void __fastcall TVideoWnd::FormClick(TObject *Sender)
{
ShowMessage("click");
}
//---------------------------------------------------------------------------

//void  TVideoWnd::HITTEST(TWMNCHitTest &Message)
//{
//  TForm::Dispatch(&Message);
//  if (Message.Result == HTCLIENT)
//     Message.Result = HTCAPTION;
//
//}

