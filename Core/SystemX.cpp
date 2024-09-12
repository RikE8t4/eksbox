

//=============================
//		SystemX.cpp
//=============================

#include <stdafx.h>
#pragma hdrstop

#include "Engine.h"
#include "Strings.h"
#include "SystemX.h"
#include "Gui.h"
#include "FormatHdrs.h"

#ifdef __BORLANDC__
#include "Main.h"
#include <dir.h>
#endif

#ifdef __VISUALC__
#include <direct.h>
#endif

SystemX sys; // Global instance

//==============================================================================

SystemX::SystemX(){

mIsDirSet = false;
CItem.DirPath = "";

}

//==============================================================================

SystemX::~SystemX(){

free(mBuf);
mBuf = NULL;
mSupportedFormats.clear();
filex.ClearVariables();

}

//==============================================================================

void SystemX::SetSupportedFormats(const string& fmts){

string s = fmts;

for(int pos; pos = (int)s.find("|"); )
{
	s.erase(pos, 1);

	int len = (int)s.find("|") -1;

	// For when only 1 extension remains
	if (len <= 0)
		len = (int)s.length();

	mSupportedFormats.push_back(s.substr(pos, len));
	s.erase(1, len);
}

}

//==============================================================================

bool SystemX::GetSupportedFormat(const string& fmtExt){

int files = (int)mSupportedFormats.size();

for (int x = 0; x < files; ++x)
{
	if (fmtExt == mSupportedFormats[x])
	{
		return 1;
	}
}

return 0;

}

//==============================================================================

uchar* SystemX::MemoryAlloc(const ulong& size, FILE* stream){

// Tackles zero byte file extraction issue
if (!size)
{
	mBuf = (uchar*) malloc (1);
}

else if (! (mBuf = (uchar*) malloc (size)) ) //buf = new uchar [size]
{
	MemoryDel();
	fclose(stream);
	gui.Close();
	gui.Status("Memory Allocation Error", 0, true);
	Sleep(800);
	return 0;
}

return mBuf;

}

//==============================================================================

void SystemX::MemoryDel(){

free(mBuf); mBuf = NULL;

}

//==============================================================================

string SystemX::GetAppDir(){

#ifndef _XBOX
return str.ExtractDirPath(__argv[0]);
#endif

#ifdef _XBOX
return "d:\\";
#endif

}

//==============================================================================

void SystemX::ExtractError(const string& fileName, const string& dirPath){

remove(fileName.c_str());
RemoveDir(dirPath);
//_rmdir(dirPath.c_str());

}

//==============================================================================

void SystemX::TimerStart(){
	mTime = GetTickCount();
}

double SystemX::TimerStop(){
	return (GetTickCount() - mTime) / 1000;
}

//==============================================================================

int SystemX::Delay(const bool& fast){

if (gui.SpeedHackIsOn())
	return 0;
else if (fast)
	return 440;
else
	return 1150;

}

//==============================================================================

long SystemX::GetAvailPhyMem(){

MEMORYSTATUS mem;
GlobalMemoryStatus(&mem);
return mem.dwAvailPhys;

}

//==============================================================================

long SystemX::GetAvailVirMem(){

MEMORYSTATUS mem;
GlobalMemoryStatus(&mem);
return mem.dwAvailVirtual;

}

//====================================================================================

void SystemX::ProcessMessages()
{

#ifdef __BORLANDC__
Application->ProcessMessages();
#else

	#ifdef _WINDOWS
	MSG msg;
	while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	#endif

#endif
}

//====================================================================================

bool SystemX::FileExists(const string& name){

DWORD rtn = GetFileAttributes(name.c_str());
return ((FILE_ATTRIBUTE_DIRECTORY & rtn) == 0);

}

//==============================================================================

bool SystemX::FileIsReadOnly(const string& name){

DWORD attr = GetFileAttributes(name.c_str());

if (attr == INVALID_FILE_ATTRIBUTES)
	return false;

if (attr & FILE_ATTRIBUTE_READONLY) 
	return true;

return false;

}

//==============================================================================

bool SystemX::DirectoryExists(const std::string& dirName){

DWORD attr = GetFileAttributes(dirName.c_str());

//if (attr == 0xFFFFFFFF)
//{
//	ulong error = GetLastError();
//	if (error == ERROR_FILE_NOT_FOUND)
//		return false; // file not found
//	else if (error == ERROR_PATH_NOT_FOUND)
//		return false; // path not found
//	else if (error == ERROR_ACCESS_DENIED)
//		return false; // file or directory exists, but access is denied
//	else
//		return false; // some other error has occured
//}
if (attr == 0xFFFFFFFF)
	return false;

if (attr & FILE_ATTRIBUTE_DIRECTORY) 
	return true;

return false;

}

//==============================================================================

bool SystemX::CreateDir(const std::string& dirName){

return CreateDirectory(dirName.c_str(), NULL) != 0;
//return mkdir(dirName.c_str()) != 0;

}

//==============================================================================

bool SystemX::RemoveDir(const std::string& dirName){

return RemoveDirectory(dirName.c_str()) != 0;

}

//==============================================================================

void SystemX::BuildDirStruct (const char* path){

char c[120];
string str = (char*)path;

for (int x; (x = (int)str.find("\\")) != -1; )
{
	str[x] = '*';

	memset(c, 0x00, 120);
	strncpy(c, path, x);

	//if (!DirectoryExists(c))
	//CreateDir(c);
	mkdir(c);
}

}

//==============================================================================

int SystemX::GetWinVer(){

#ifdef _WINDOWS
OSVERSIONINFO osvi;
osvi.dwOSVersionInfoSize = sizeof(osvi);
GetVersionEx(&osvi);
return osvi.dwMajorVersion;
#else
return -1;
#endif

//// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
//// Onfail, try using the OSVERSIONINFO structure.
//OSVERSIONINFOEX osvi;
//
//ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
//osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
//
//if( ! (GetVersionEx ((OSVERSIONINFO *) &osvi)) )
//{
//	osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
//	if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )
//	return 0;
//}
//
//return osvi.dwMajorVersion;

}

//==============================================================================

void SystemX::ExecuteAndWait(const std::string& path){

//SHELLEXECUTEINFOA shInfo;
SHELLEXECUTEINFO shInfo;
ZeroMemory(&shInfo, sizeof(shInfo));shInfo.cbSize = sizeof(shInfo);shInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_UNICODE;
shInfo.hwnd = (HWND)shInfo.hProcess;
shInfo.lpFile = path.c_str();
shInfo.nShow = SW_SHOWNORMAL;
shInfo.lpDirectory = str.ExtractFileDir(path).c_str();
shInfo.lpVerb = "open";ShellExecuteEx(&shInfo);WaitForInputIdle(shInfo.hProcess, INFINITE);// error, use GetLastError()//WaitForSingleObject(shInfo.hProcess, INFINITE);//DWORD ret;//GetExitCodeProcess(shInfo.hProcess, &ret);CloseHandle(shInfo.hProcess);
}

//==============================================================================

int SystemX::GetOpenFileNameExtLen(void){

return (int)str.ExtractFileExt(CItem.OpenFileName).length();

}

//==============================================================================

void SystemX::DirNameSet(void){

CItem.DirName = str.ExtractFileName(CItem.OpenFileName);
CItem.DirName.resize((int)CItem.DirName.length() - GetOpenFileNameExtLen()-1);

}

//==============================================================================

void SystemX::DirPathSet(const string& dir){

CItem.DirPath = str.ExtractDirPath(dir);
mIsDirSet = true;

}

//==============================================================================

void SystemX::DirPathSet(){

CItem.DirPath = str.ExtractDirPath(CItem.OpenFileName);
mIsDirSet = true;

}

//==============================================================================

void SystemX::DirPathSetSilent(){
					
CItem.DirPath = str.ExtractDirPath(CItem.OpenFileName);

}

//==============================================================================

string SystemX::DirPathGet(){

return CItem.DirPath;

}

//==============================================================================

bool SystemX::DirPathIsSet(){

return mIsDirSet;

}

//==============================================================================

void SystemX::DirBrowse(){

#ifdef __BORLANDC__
Form1->EkDirBrowse1->Handle = (THandle)Form1->Handle;

if (Form1->EkDirBrowse1->Execute())
	DirPathSet(str.AnsiToStd(Form1->EkDirBrowse1->Directory) + "\\");
#endif

}

//==============================================================================

void SystemX::DeleteDir(const string& path){

char cPath [MAX_PATH];
memset (cPath, 0, sizeof(cPath));
strcpy (cPath, path.c_str());

SHFILEOPSTRUCT shStr;
shStr.hwnd = NULL;
shStr.wFunc = FO_DELETE;
shStr.pFrom = cPath;
shStr.pTo = NULL;
shStr.fFlags = FOF_SILENT|FOF_NOCONFIRMATION|FOF_NOERRORUI;
SHFileOperation(&shStr);
_rmdir(cPath);

}

//==============================================================================

string SystemX::GetTempDir(){

char cTempDir[MAX_PATH +1];
cTempDir[MAX_PATH] = '\0';
GetTempPath(MAX_PATH, cTempDir);
return (char*)&cTempDir;

}

//==============================================================================

void SystemX::GetAllFiles(const string& path, bool recursive)
{

// TODO: Port to vc++
#ifdef __BORLANDC__
TSearchRec sr;
String apath = path.c_str();

if(!apath.IsPathDelimiter(apath.Length())) apath += "\\";

if(FindFirst(apath + "*.*", faAnyFile, sr) == 0)
{
	do
	{
		if(sr.Name != "." && sr.Name != "..")
		{
			if((sr.Attr & faDirectory) && recursive)
				GetAllFiles(str.AnsiToStd(apath + sr.Name), true);
			else
				Form1->OpenDialog->Files->Add(apath + sr.Name);
				//FilesList->Add(Path + sr.Name);
		}
	}
	while(FindNext(sr) == 0);

	FindClose(sr);
}

#endif

}

//==============================================================================

ulong SystemX::GetFileSize(FILE* in, const bool& updateSizeLbl){

fseek(in, 0L, SEEK_END);
CItem.FileSize = ftell(in);
#ifdef __BORLANDC__
if (updateSizeLbl)
	Form1->LblSizeVal->Caption = str.SizeToStr(CItem.FileSize).c_str();
#endif
return CItem.FileSize;

}

//==============================================================================

int SystemX::HeaderScanFast (const ulong& startPos, const ulong& fileSize,
						const uchar* sig, FILE* in)
{

//if (fileSize <= 64 || !sig || !in) return -1;

uchar buf[4];
ulong x = 0, pos = startPos;

while (x < fileSize)
{
	fseek(in, pos, 0);
	fread(&buf, 4, 1, in);

	if (buf[0] == sig[0] &&  buf[1] == sig[1]
		&& buf[2] == sig[2] && buf[3] == sig[3])
	{
		return pos;
	}
	++x, ++pos;
}

return -1;

}// End HeaderScan

//==============================================================================

int SystemX::HeaderScan (const ulong& startPos, const ulong& fileSize,
						const char* sig, const ushort& sigSize, FILE* in)
{

if (fileSize <= 64 || !sig || !in) return -1;

//ulong 	bufSize = fileSize >> 4;
//uint 	availMem = GetAvailPhyMem();

//------------
int bufSize = fileSize >> 2, availMem = GetAvailPhyMem();

// Medium
while (bufSize > availMem >> 1) bufSize >>= 2;


uchar* buf;
if (! (buf = MemoryAlloc(bufSize, NULL)) )
	return -1;


int loops = fileSize / bufSize;
int posSubtractor =	bufSize / loops;
//int posSubtractor = (fileSize / bufSize) >> 1;

int x = 0, pos = startPos;
//fseek(in, 0, SEEK_SET);
						  
if (sigSize == 4)
while (!feof(in))
{
	fseek(in, pos, 0);
	fread(buf, bufSize, 1, in);
	while (1)
	{
		if ( buf[x] == sig[0] &&  buf[x+1] == sig[1]
			&& buf[x+2] == sig[2] && buf[x+3] == sig[3] )
		{
			MemoryDel();
			return pos;
		}
		if (x == bufSize)
		{
			x = 0;
			pos -= posSubtractor;// Buffer back-tracking
			break;
		}
	++x, ++pos;
	}
}

MemoryDel();
return -1;

}// End HeaderScan

//==============================================================================





