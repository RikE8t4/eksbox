


//==================================
//  Xct.cpp - Xct Database Handler
//==================================

#include <stdafx.h>
#pragma hdrstop

#ifdef __BORLANDC__
#include "Main.h"
#endif

#include "Xct.h"
#include "Engine.h"
#include "Gui.h"
#include "Strings.h"
#include "SystemX.h"

//==============================================================================

void Xct::FindMatch(string& pDirName, bool& pIniMatch, int& pEntries){

#ifdef __BORLANDC__
if (Form1->ChkAutoEditClr->Checked)
	Form1->EditFolName->Text = "";

if (Form1->ChkXct->Checked)
{
	//Form1->EditFolName->Enabled = 0;
	Form1->EditRenFiles->Enabled = 0;
}
else
{
	//Form1->EditFolName->Enabled = 1;
	Form1->EditRenFiles->Enabled = 1;
	return;
}
#endif

string appDir = sys.GetAppDir();
string name = appDir + XCT;

FILE* in = fopen(name.c_str(), "rb");
if (!in)
	return;

string fileName = str.ToLower(str.ExtractFileName(CItem.OpenFileName));
fileName = str.RemoveFileExt(fileName);

int files;
vector<ulong>offset, size;
ulong off[4], sz[4];

uchar buf[BUFSIZE];
string line;

fread(buf, 4, 1, in);
files = *buf;
fseek(in, 8, 0);

int x = 0;
while (x < files)
{
	fread(off, 4, 1, in);
	fread(sz, 4, 1, in);
	offset.push_back(*off);
	size.push_back(*sz);
	++x;
}
x = 0;

while (x < files)
{
	fseek(in, offset[x], 0);
	fgets((char*)buf, BUFSIZE, in);

	line = (char*)buf;
	str.Trim(line);

	int nameLen = (int)line.find_last_of(".");

	if (nameLen > 0)
		line.resize(nameLen);


	if ( str.ToLower(line) == fileName )
	{
		fgets((char*)buf, BUFSIZE, in);
		line = (char*)buf;
		str.Trim(line);

		if (str.StrToInt(line) == CItem.FileSize)
		{ // Full match
			pIniMatch = true;
			break;
		}
	}
++x;
}

offset.clear(); size.clear();

// Xct entry reader
if (pIniMatch)
{
	fgets((char*)buf, BUFSIZE, in);

	// Create custom dir name
	pDirName = (char*)buf;
	str.Trim(pDirName);

	string caption = " - " + pDirName;
	#ifdef __BORLANDC__
	gui.CaptionSave(APPNAME, caption); // For mediaplayer stop press revertion
	#endif
	fgets((char*)buf, BUFSIZE, in);
	fgets((char*)buf, BUFSIZE, in);

	x = 0;						
	while (buf[0] != '#' && x < CItem.Files)
	{
		FItem.FileName[x] = (char*)buf;
		FItem.FileName[x].resize(FItem.FileName[x].length() -2);
		fgets((char*)buf, BUFSIZE, in);
		++x;
	}                     
}

#ifdef __BORLANDC__
if (pIniMatch && Form1->ChkXct->Checked)
	Form1->EditFolName->Text = pDirName.c_str();
if (CItem.Loading) // Only show on fileLoad
	Form1->LblXctEntriesVal->Caption = files;
#endif

pEntries = files;

fclose(in);

}

//==============================================================================

// Dump an ini file for use with Xct
void Xct::Dump(){

string dialogText = "";
#ifdef __BORLANDC__
if (!InputQuery("Save file as (Without extension)",
"Filename (Leave blank to auto-name)", dialogText.c_str()))
	dialogText = "";
#endif

string dirName = sys.GetAppDir() + "\\Names\\";

string rawName = str.ExtractFileName(CItem.OpenFileName);
int extLength = (int)str.ExtractFileExt(CItem.OpenFileName).length();
rawName.erase(rawName.length() - extLength-1, extLength+1);

string iniFileName;

if (dialogText != "")
{
	iniFileName = dirName + dialogText +
	" (" + str.ExtractFileName(CItem.OpenFileName) + ").ini";
}
else
{
	iniFileName = dirName + rawName +
	" (" + str.ExtractFileName (CItem.OpenFileName) + ").ini";
}

if (!sys.DirectoryExists(dirName))
	sys.CreateDir(dirName);

FILE* out = fopen(iniFileName.c_str(), "wt");
if (!out)
	return;

string archiveName = str.ExtractFileName(CItem.OpenFileName) + "\n";
string folderName = "\n" + gui.EditFolderNameTextGet() + "\n";
fputs (archiveName.c_str(), out);
fputs (str.IntToStr(CItem.FileSize).c_str(), out);
fputs (folderName.c_str(), out);

for (int x = 0; x < 52; ++x)
	fputs ("#", out);

string name;

for (int x = 0; x < CItem.Files; ++x)
{
	if (CItem.HasNames && gui.EditRenFilesTxtGet() == ""
	&& gui.ChkXct())
	{
		name = FItem.FileName[x].c_str();
	}
	else if (CItem.HasNames && gui.EditRenFilesTxtGet() == ""
	&& gui.ChkXct())
	{
		name = FItem.FileName[x].c_str();
	}
	else if (CItem.HasNames && gui.EditRenFilesTxtGet() == ""
	&& !gui.ChkTidyName())
	{
		name = FItem.FileNameBak[x].c_str();
	}
	else if (gui.EditRenFilesTxtGet() != "")
	{
		name = FItem.FileName[x].c_str();
	}
	else
	{
		str.AutoNumList(0, name, x, CItem.Files);
		name = str.RemoveFileExt(name);
	}

	if (gui.ChkTidyName())
		str.StrClean(name);

	name = "\n" + name;

	fputs(name.c_str(), out);
}

	fputs ("\n", out);
	for (int x = 0; x < 52; ++x) fputs ("#", out);

	fclose(out);

}// End xct dump

//==============================================================================

