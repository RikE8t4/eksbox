


#include <stdafx.h>
#pragma hdrstop
#include "Main.h"
#include "List.h"
#include "Gui.h"
#include "Strings.h"
#include "SystemX.h"
#include "Engine.h"

#define LIST Form1->ListView1
#define IMGLIST Form1->ImageList1

//------------------------------------------------------------------------------

EkszList::EkszList()
{
mFileList = new TList;
}

EkszList::~EkszList()
{
Clear();
}

//------------------------------------------------------------------------------

void EkszList::Clear(){

mFileList->Clear();
LIST->Items->Count = 0;

}

//------------------------------------------------------------------------------

void EkszList::Load(){

//if (!Files) return;

IMGLIST->ShareImages = true;

Clear();

gui.Status("Loading List...", 0, true);


int x = 0;
while (x < CItem.Files)
{

	TFileItem* fi = new TFileItem;

	// Assign icons
	if (FItem.Format[x] == "pcm")
		fi->IconIndex = 1;
	else if (FItem.Format[x] == "xadpcm")
		fi->IconIndex = 2;
	else if (FItem.Ext[x] == ".xma")
		fi->IconIndex = 3;
	else if (FItem.Ext[x] == ".wma" || FItem.Ext[x] == ".mp3" || FItem.Ext[x] == ".adx"
			|| FItem.Ext[x] == ".wav" || FItem.Ext[x] == ".asf" || FItem.Ext[x] == ".mus")
		fi->IconIndex = 4;
	else if (FItem.Ext[x] == ".ogg")
		fi->IconIndex = 5;
	else if (FItem.Ext[x] == ".ico" || FItem.Ext[x] == ".tbn")//FItem.Ext[x]==".dff"
		fi->IconIndex = 6;
	else if (FItem.Ext[x] == ".bmp")
		fi->IconIndex = 7;
	else if (FItem.Ext[x] == ".jpg")
		fi->IconIndex = 8;
	else if (FItem.Ext[x] == ".png")
		fi->IconIndex = 9;
	else if (FItem.Ext[x] == ".psd")
		fi->IconIndex = 10;
	else if (FItem.Ext[x] == ".tga")
		fi->IconIndex = 11;
	else if (FItem.Ext[x] == ".dds" || FItem.Ext[x]==".x")
		fi->IconIndex = 12;
   else if (FItem.Ext[x]==".txd" || FItem.Ext[x]==".dff" || FItem.Ext[x]==".rwf")
		fi->IconIndex = 23;
   else if (FItem.Ext[x]==".lua")
		fi->IconIndex = 22;
	else if (FItem.Ext[x] == ".txt" || FItem.Ext[x] == ".ini" || FItem.Ext[x] == ".spt")
		fi->IconIndex = 13;
	else if (FItem.Ext[x] == ".mov" || FItem.Ext[x] == ".m1v" || FItem.Ext[x] == ".sfd")
	//|| FItem.Ext[x]==".avi" || FItem.Ext[x]==".mpg" || FItem.Ext[x]==".mpeg"
		fi->IconIndex = 14;
	else if (FItem.Ext[x] == ".rom")
		fi->IconIndex = 15;
	//else if (FItem.Ext[x]==".rar")
	//ListView1->Items->operator [](x)->ImageIndex=16;
	else if (FItem.Ext[x] == ".zip")
		fi->IconIndex = 17;
	else if (FItem.Ext[x] == ".xmv" || FItem.Ext[x] == ".wmv" || FItem.Ext[x] == ".bik")
		fi->IconIndex = 18;
	else if (FItem.Ext[x] == ".xbe")
		fi->IconIndex = 19;
	else if
	(
		FItem.Ext[x] == ".adf" || FItem.Ext[x] == ".adx"
		|| FItem.Ext[x] == ".cab" || FItem.Ext[x] == ".sh4"
		|| FItem.Ext[x] == ".afs" || FItem.Ext[x] == ".arc"
		|| FItem.Ext[x] == ".big" || FItem.Ext[x] == ".dvdxwb"
		|| FItem.Ext[x] == ".fsb"
		|| FItem.Ext[x] == ".hdxwb" || FItem.Ext[x] == ".hot"
		|| FItem.Ext[x] == ".hwx" || FItem.Ext[x] == ".iso"
		|| FItem.Ext[x] == ".lug" || FItem.Ext[x] == ".map"
		|| FItem.Ext[x] == ".mpk"
		|| FItem.Ext[x] == ".msx" || FItem.Ext[x] == ".pak"
		|| FItem.Ext[x] == ".pod" || FItem.Ext[x] == ".piz"
		|| FItem.Ext[x] == ".rcf"
		|| FItem.Ext[x] == ".rez" || FItem.Ext[x] == ".rfa"
		|| FItem.Ext[x] == ".rws"
		|| FItem.Ext[x] == ".samp"|| FItem.Ext[x] == ".stx"
		|| FItem.Ext[x] == ".swwc"
		|| FItem.Ext[x] == ".sxb" || FItem.Ext[x] == ".vxb"
		|| FItem.Ext[x] == ".sr"
		|| FItem.Ext[x] == ".wad" || FItem.Ext[x] == ".wmapack"
		|| FItem.Ext[x] == ".wpx"
		|| FItem.Ext[x] == ".wxv" || FItem.Ext[x] == ".xwb"
		|| FItem.Ext[x] == ".xwc" || FItem.Ext[x] == ".zwb"
		|| FItem.Ext[x] == ".dat"

		|| FItem.Ext[x] == ".met" || FItem.Ext[x] == ".uber"
		|| FItem.Ext[x] == ".xsb"
	)
		fi->IconIndex = 20;
	else
		fi->IconIndex = 0;


	fi->FileName = FItem.FileName[x] + FItem.Ext[x];
	fi->Size = str.SizeToStr(FItem.Chunk[x]);
	#ifdef DEBUG
	//if (GetShowOffsets())
	//{
		fi->Offset = FItem.Offset[x];
	//}
	#endif

	if (CItem.HasAudioInfo && FItem.Format[x] != "")
	{
		fi->SampleRate = FItem.SampleRate[x];
		fi->Channels = FItem.Channels[x];
		fi->Bits = FItem.Bits[x];
		fi->Format = FItem.Format[x];
	}
	else
	{
		fi->SampleRate = 0x0;
		fi->Channels = 0x0;
		fi->Bits = 0x0;
		fi->Format = "Other";
	}

	fi->Checked = fi->StateIndex = true;
	//fi->StateIndex = 0;
	mFileList->Add(fi);

	++x;
}

LIST->Items->Count = mFileList->Count;

}
// End Load List

//------------------------------------------------------------------------------

void EkszList::CheckAllNone(const bool& check){

int x = 0;
while(x < LIST->Items->Count)
{
	TFileItem* fi = (TFileItem*) mFileList->Items[x];
	fi->Checked = check;

	LIST->UpdateItems(x, x);
	++x;
}

}

//---------------------------------------------------------------------------

void EkszList::CheckAllAudio(){

int x = 0;
while(x < LIST->Items->Count)
{
	TFileItem* fi = (TFileItem*) mFileList->Items[x];

	string ext = str.GetFileExt(fi->FileName);

	if (sys.GetSupportedFormat(ext))
		fi->Checked = true;
	else
		fi->Checked = false;

	LIST->UpdateItems(x, x);

	++x;
}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1Data(TObject *Sender, TListItem *Item)
{
         
TFileItem* fi = (TFileItem*) List.mFileList->Items[Item->Index];

//Item->StateIndex = fi->StateIndex;
Item->Checked = fi->Checked;
Item->Caption = fi->FileName.c_str();
Item->SubItems->Add(fi->Size.c_str());

#ifdef DEBUG
//if (List.GetShowOffsets())
Item->SubItems->Add(fi->Offset);
#endif

if (CItem.HasAudioInfo && fi->SampleRate != 0)
{
	Item->SubItems->Add(fi->SampleRate);
	Item->SubItems->Add(fi->Channels);
	Item->SubItems->Add(fi->Bits);
	Item->SubItems->Add(fi->Format.c_str());
}
else
{
	Item->SubItems->Add("");
	Item->SubItems->Add("");
	Item->SubItems->Add("");
	Item->SubItems->Add("");
	//Item->SubItems->Add("alien");
}


}

//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1GetImageIndex(TObject *Sender, TListItem *Item)
{

TFileItem* fi = (TFileItem*) List.mFileList->Items[Item->Index];
Item->ImageIndex = fi->IconIndex;
Item->StateIndex = fi->Checked;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1SelectItem(TObject *Sender, TListItem *Item,
      bool Selected)
{
			 
//if (!Selected) return;
//
//TFileItem* fi = (TFileItem*) List.FileList->Items[Item->Index];
//fi->Checked = Selected;
//ListView1->UpdateItems(Item->Index, Item->Index);

}
//---------------------------------------------------------------------------

// Based on code posted by Remy Lebeau/Gambit (TeamB)

void __fastcall TForm1::ListView1MouseDown(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{

if (Button != mbLeft) return;

if (ListView1->GetHitTestInfoAt(X, Y).Contains(htOnStateIcon))
{
	TListItem* Item = ListView1->GetItemAt(X, Y);
	if (Item)
	{
		TFileItem* fi = (TFileItem*) List.mFileList->Items[Item->Index];
		fi->Checked = !fi->Checked;

		// 0 cbcOut, 1 cbcOver, 2 cbcDown
		// 3 cbuOut, 4 cbuOver
		//fi->StateIndex = 1;

		ListView1->UpdateItems(Item->Index, Item->Index);
	}
}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1Edited(TObject *Sender, TListItem *Item,
	  AnsiString &S)
{

if (Item->Index < CItem.Files) // Bounds check
{
	TFileItem* fi = (TFileItem*) List.mFileList->Items[Item->Index];
	fi->FileName = S.c_str();
	S = str.RemoveFileExt(S.c_str()).c_str();
	FItem.FileName[Item->Index] = S.c_str();
}

}

//---------------------------------------------------------------------------

