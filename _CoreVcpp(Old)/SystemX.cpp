

//=============================
//		SystemX.cpp
//=============================

#include "stdafx.h" 
#include "FileSpecs.h"
//#include "Strings.h"
#include "SystemX.h"
#include "Helper.h"
//#include "Gui.h"
#include "FormatHdrs.h"

//#include <time.h>
//#include <sys\timeb.h>
//#include <dir.h>

SystemX sys; // Global instance

// Port comments
//------------------
// Untested
// UnCommentMeSoon
// Not needed for xbox
//------------------

//===========================================================================

SystemX::SystemX(){
	IsDirSet = false;
	CItem.DirPath = "";
}

//===========================================================================

SystemX::~SystemX(){
	free(Buf);
	Buf = NULL;
	SupportedFormats.clear();
	ClearVariables();
}

//===========================================================================

void SystemX::SetSupportedFormats(string fmts){

for(int pos; pos = (int)fmts.find("|"); )
{
	//fmts.Delete(pos, 1);
	fmts.resize(pos); // Untested

	int len = (int)fmts.find("|") -1;

	// For when only 1 extension remains
	if (len <= 0)
		len = (int)fmts.size();
	
	SupportedFormats.push_back(fmts.substr(pos, len));
	//fmts.Delete(1, len);
	fmts.resize(len); // Untested
}

}

//===========================================================================

bool SystemX::GetSupportedFormat(string fmtExt){

int files = (int)SupportedFormats.size();

for (int x = 0; x < files; ++x)
{
	if (fmtExt == SupportedFormats[x])
	{
		return 1;
	}
}

return 0;
}

//===========================================================================

uchar* SystemX::MemoryAlloc(const ulong& size, FILE* stream){

// Tackles zero byte file extraction issue
if (!size)
{
	Buf = (uchar*) malloc (1);
}

else if (! (Buf = (uchar*) malloc (size)) ) //buf = new uchar [size]
{
	MemoryDel();
	fclose(stream);
	//gui.Close();
	//gui.Status("Memory Allocation Error", 0, true);
	Sleep(800);
	return 0;
}
return Buf;
}

//===========================================================================

void SystemX::MemoryDel(){
free(Buf); Buf = NULL;
}

//===========================================================================

void SystemX::ExtractError(string fileName, string dirPath){

remove(fileName.c_str());
//_rmdir(dirPath.c_str()); // UnCommentMeSoon

}

//===========================================================================

void SystemX::TimerStart(){
	Time = GetTickCount();
}

double SystemX::TimerStop(){
	return (GetTickCount() - Time) / 1000;
}

//===========================================================================

int SystemX::Delay(const bool& fast){
//if (gui.SpeedHackIsOn()) // UnCommentMeSoon
//	return 0;
//else if (fast)
//	return 440;
//else
	return 1150;
}

//===========================================================================

long SystemX::GetAvailPhyMem(){
GlobalMemoryStatus(&Memory);
return (long)Memory.dwAvailPhys;
}

long SystemX::GetAvailVirMem(){
GlobalMemoryStatus(&Memory);
return (long)Memory.dwAvailVirtual;
}

//===========================================================================

bool SystemX::FileExists(string name){

int rtn = GetFileAttributes(name.c_str());
return ((rtn != -1) && ((FILE_ATTRIBUTE_DIRECTORY & rtn) == 0));

}

//===========================================================================

bool SystemX::FileIsReadOnly(string name){

#ifdef _WINDOWS
ulong attr = GetFileAttributes(name.c_str());

//if (attr == INVALID_FILE_ATTRIBUTES) return false;
if (attr & FILE_ATTRIBUTE_READONLY) 
	return true;

#endif

return false;

}

//===========================================================================


int SystemX::GetWinVer(){

#ifdef _WINDOWS
OSVERSIONINFO osvi;
osvi.dwOSVersionInfoSize = sizeof(osvi);
GetVersionEx(&osvi);
return osvi.dwMajorVersion;
#endif

return 0;

}

//===========================================================================

void SystemX::ExecuteAndWait(string path){

// Probably not needed for xbox
#ifdef _WINDOWS
SHELLEXECUTEINFO shInfo;
ZeroMemory(&shInfo, sizeof(shInfo));

shInfo.cbSize = sizeof(shInfo);
shInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_UNICODE;
shInfo.hwnd = (HWND)shInfo.hProcess; // Untested
shInfo.lpFile = path.c_str();
shInfo.nShow = SW_SHOWNORMAL;
shInfo.lpDirectory = ExtractFileDir(path).c_str();
shInfo.lpVerb = "open";

ShellExecuteEx(&shInfo);
WaitForInputIdle(shInfo.hProcess, INFINITE);
CloseHandle(shInfo.hProcess);
#endif

}

//===========================================================================

void SystemX::BuildDirStruct (const char* path){

char c[MAX_PATH];
string str = (char*)path;

for (int x; x = (int)str.find_first_of("\\")+1; )
{
	str[x-1] = '*';

	memset(c, '\0', 120);
	strncpy(c, path, x);

	//if (!DirectoryExists(c))
	CreateDirectory(c, NULL);
	//mkdir(c);
}

}

//===========================================================================

int SystemX::GetOpenFileNameExtLen(){
string ext = ExtractFileExt(CItem.OpenFileName);
return (int)ext.size();
}

//===========================================================================

void SystemX::DirNameSet(){
string dirName = ExtractFileName(CItem.OpenFileName);
dirName.resize((int)dirName.size() - GetOpenFileNameExtLen()-1);
CItem.DirName = dirName;
}


void SystemX::DirPathSet(string path){
CItem.DirPath = ExtractDirPath(path);
IsDirSet = true;
}


void SystemX::DirPathSet(){
CItem.DirPath = ExtractDirPath(CItem.OpenFileName);
IsDirSet = true;
}


void SystemX::DirPathSetSilent(string path){
CItem.DirPath = ExtractDirPath(path);
}


string SystemX::DirPathGet(){
return CItem.DirPath;
}

bool SystemX::DirPathIsSet(){
return IsDirSet;
}

//===========================================================================


void SystemX::DirBrowse(){

// Not needed for xbox
////Form1->EkDirBrowse1->InitialDirectory
//Form1->EkDirBrowse1->Handle = (THandle)Form1->Handle;
//
//if (Form1->EkDirBrowse1->Execute())
//	DirSet(Form1->EkDirBrowse1->Directory + "\\");

}

//===========================================================================

void SystemX::DeleteDir(string path){

#ifdef _WINDOWS
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
remove(cPath);
//_rmdir(cPath);
#endif

}

//===========================================================================

string SystemX::GetTempDir(){

#ifdef _WINDOWS
char cTempDir[MAX_PATH +1];
cTempDir[MAX_PATH] = '\0';
GetTempPath(MAX_PATH, cTempDir);
return (char*)&cTempDir;
#endif

return "";

}

//===========================================================================

void SystemX::GetAllFiles(string path, bool recursive)
{
// UnCommentMeSoon
//TSearchRec sr;
//
//if(!path.IsPathDelimiter(path.Length())) path += "\\";
//
//if(FindFirst(path + "*.*", faAnyFile, sr) == 0)
//{
//	do
//	{
//		if(sr.Name != "." && sr.Name != "..")
//		{
//			if((sr.Attr & faDirectory) && recursive)
//				GetAllFiles(path + sr.Name, true);
//			else
//				Form1->OpenDialog->Files->Add(path + sr.Name);
//				//FilesList->Add(Path + sr.Name);
//		}
//	}
//	while(FindNext(sr) == 0);
//
//	FindClose(sr);
//}

}

//===========================================================================

ulong SystemX::GetFileSize(FILE* in, const bool& updateSizeLbl){
fseek(in, 0L, SEEK_END);
CItem.FileSize = (ulong)ftell(in);
// UnCommentMeSoon
//if (updateSizeLbl)
//	Form1->LblSizeVal->Caption = CalcSize(CItem.Filesize);
return CItem.FileSize;
}

//===========================================================================

int SystemX::HeaderScanFast (const int& startPos, const int& fileSize,
						const uchar* sig, FILE* in)
{


uchar buf[4];
int x = 0, pos = startPos;

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


//===========================================================================

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

//===========================================================================

// Dump an ini file for use with Xct (Old need a cleanup)
void SystemX::IniDump(string& homeDir){

// UnCommentMeSoon
//string dialogText;
//if (InputQuery("Save file as (Without extension)",
//"Filename (Leave blank to auto-name)", dialogText))
//
//CreateDir (homeDir + "/Names");
//string nameDir = homeDir + "/Names/";
//
//string iniFileName,
//rawName = ExtractFileName (Form1->OpenDialog->FileName);
//ushort extLength = ExtractFileExt (Form1->OpenDialog->FileName).Length();
//rawName = rawName.Delete (rawName.Length() - extLength +1, extLength);
//
//if (dialogText != "")
//{
//	iniFileName = nameDir + dialogText +
//	" (" + ExtractFileName (Form1->OpenDialog->FileName) + ").ini";
//}
//else
//{
//	iniFileName = nameDir + rawName +
//	" (" + ExtractFileName (Form1->OpenDialog->FileName) + ").ini";
//}
//
//FILE* out;
//out = fopen(iniFileName.c_str(), "wt");
//string archiveName = ExtractFileName (Form1->OpenDialog->FileName) + "\n";
//string folderName = "\n" + Form1->EditFolName->Text + "\n";
//fputs (archiveName.c_str(), out);
//fputs (string(CItem.Filesize).c_str(), out);
//fputs (folderName.c_str(), out);
//
//for (int x = 0; x < 52; ++x) fputs ("#", out);
//
//string name;
//
//for (int x = 0; x < CItem.Files; ++x)
//{
//	if (CItem.HasNames && Form1->EditRenFiles->Text == ""
//	&& Form1->ChkXct->Checked)
//	{
//		name = FItem.FileName[x];
//	}
//	else if (CItem.HasNames && Form1->EditRenFiles->Text == ""
//	&& Form1->ChkTidyName->Checked)
//	{
//		name = FItem.FileName[x];
//	}
//	else if (CItem.HasNames && Form1->EditRenFiles->Text == ""
//	&& !Form1->ChkTidyName->Checked)
//	{
//		name = FItem.FileNameBak[x];
//	}
//	else if (Form1->EditRenFiles->Text != "")
//	{
//		name = FItem.FileName[x];
//	}
//	else
//	{     
//		str.AutoNumList(0, name, x, CItem.Files);
//		str.RemoveExt(name);
//	}
//
//	if (Form1->ChkTidyName->Checked)
//		str.StrClean(name);
//
//	name = "\n" + name;
//
//	fputs(name.c_str(), out);
//}
//
//	fputs ("\n", out);
//	for (int x = 0; x < 52; ++x) fputs ("#", out);
//
//	fclose(out);

}// End ini dump


//===========================================================================

void SystemX::WritePcmHeader(const ulong& fileSize,
const short& channels, const short& bits, const uint& sampleRate, FILE* out){

pcm.size = fileSize + 40;
pcm.channels = channels;
pcm.sampleRate = sampleRate;
pcm.bitRate = (sampleRate * channels * bits) >> 3; // >> 3 equivalent to / 8
pcm.blockSize = channels * (bits >> 3);
pcm.bits = bits;
pcm.subChunk2Size = fileSize;
fwrite(&pcm, sizeof (pcm), 1, out);

}

//===========================================================================

#ifdef DEBUG
void SystemX::WriteMsadpcmHeader(const ulong& fileSize,
const short& channels, const uint& sampleRate, FILE* out){

msadpcm.size = fileSize + 40;
msadpcm.channels = channels;
msadpcm.sampleRate = sampleRate;

if (sampleRate == 8000)
{
	msadpcm.bitRate = 4096 * channels;
	msadpcm.blockAlign = 256 * channels;
	msadpcm.unknown2 = 500;
}
else if (sampleRate == 11025)
{
	msadpcm.bitRate = 5644 * channels;
	msadpcm.blockAlign = 256 * channels;
	msadpcm.unknown2 = 500;
}
else if (sampleRate == 22050)
{
	msadpcm.bitRate = 11155 * channels;
	msadpcm.blockAlign = 512 * channels;
	msadpcm.unknown2 = 1012;
}
else if (sampleRate == 44100)
{
	msadpcm.bitRate = 22179 * channels;
	msadpcm.blockAlign = 1024 * channels;
	msadpcm.unknown2 = 2036;
}


msadpcm.subChunk2Size = fileSize;
fwrite(&msadpcm, sizeof(msadpcm), 1, out);

}
#endif

//===========================================================================

void SystemX::WriteXadpcmHeader(const ulong &fileSize,
const short& channels, const uint& sampleRate, FILE* out){

xadpcm.size = fileSize + 40;
xadpcm.channels = channels;
xadpcm.sampleRate = sampleRate;
xadpcm.blockAlign = channels * 36;
xadpcm.bitRate = (sampleRate * xadpcm.blockAlign) >> 6; // >> 6 equivalent to / 64
xadpcm.subChunk2Size = fileSize;
fwrite(&xadpcm, sizeof(xadpcm), 1, out);

}

//===========================================================================

void SystemX::PushFiles(const int &files){
CItem.Files = files;
}

void SystemX::PushFiles(FILE* in, const int& size){
fread(&CItem.Files, size, 1, in);
}

void SystemX::PushOffset(FILE* in, const ushort& size){
fread((char*)&Offset, size, 1, in);
FItem.Offset.push_back(*Offset);
}

void SystemX::PushOffset(const ulong& offset){
FItem.Offset.push_back(offset);
}

void SystemX::PushChunk(FILE* in, const ushort& size){
fread((char*)&Chunk, size, 1, in);
FItem.Chunk.push_back(*Chunk);
}

void SystemX::PushChunk(const ulong& chunk){
FItem.Chunk.push_back(chunk);
}

void SystemX::PushFileName(FILE* in, const ushort& size){
fread(&Name, size, 1, in);
string n = (char*)Name;
n.resize(size);
FItem.FileName.push_back(n);
//FItem.FileName.push_back(string((char*)Name).resize(size));
}

void SystemX::PushFileName(string name){
FItem.FileName.push_back(name);
}

void SystemX::PushExtension(string ext){
FItem.Ext.push_back(ext);
}

void SystemX::PushAudioFormat(const char* audioFmt){
FItem.Format.push_back((char*)audioFmt);
}

void SystemX::PushBits(const short& bits){
FItem.Bits.push_back(bits);
}

void SystemX::PushChannel(FILE* in, const uint& size){
fread(&Channels, size, 1, in);
FItem.Channels.push_back(*Channels);
}

void SystemX::PushChannel(const short& channel){
FItem.Channels.push_back(channel);
}

void SystemX::PushSampleRate(const ushort& sampleRate){
FItem.SampleRate.push_back(sampleRate);
}

void SystemX::PushSampleRate(FILE* in, const uint& size){
fread(&SampleRate, size, 1, in);
FItem.SampleRate.push_back(*SampleRate);
}

//===========================================================================

void SystemX::ClearVariables(){

CItem.Finished = false;
CItem.Loading = false;
CItem.ScxMode = false;
CItem.Extracting = false;
CItem.IniMatch = false;
CItem.HasNames = false;
CItem.HasAudioInfo = false;
CItem.HasFolders = false;
CItem.NeedsWavHeader = false;
CItem.Error = false;
CItem.Files = 0;
//CItem.IncompatileFmts = 0;
CItem.FileSize = 0;
CItem.DirName = "";
//CItem.DirPath = "";
//CItem.OpenFileName = "";
CItem.Format = "";

FItem.Offset.clear();
FItem.OffsetXtra.clear();
FItem.Chunk.clear();
FItem.ChunkXtra.clear();
FItem.Chunk1st.clear();
FItem.Parts.clear();
FItem.SampleRate.clear();
FItem.Channels.clear();
FItem.Bits.clear();
FItem.Format.clear();
FItem.Ext.clear();
FItem.FileName.clear();
FItem.FileNameBak.clear();
}

//===========================================================================

int SystemX::RiffHeaderSize(FILE* in, const uint& offset){

uchar fmt[4];
short hdrSize;
uchar fact[4];

fseek(in, offset + 16, 0);
fread(&hdrSize, 2, 1, in);

fseek(in, offset + 20, 0);
fread(fmt, 2, 1, in);

if (*fmt != 0x01)
{
	fseek(in, offset + hdrSize + 20, 0);
	fread(fact, 4, 1, in);
	if ( !memcmp(fact, "fact", 4) )
		hdrSize += 36;
	else
		hdrSize += 24;
}
else
	hdrSize += 24;

return hdrSize + 4;

}

//===========================================================================

uint SystemX::RiffSize(FILE* in, const uint& offset){

uchar fmt[4];
uint size[4];
short hdrSize;
uchar fact[4];

fseek(in, offset + 16, 0);
fread(&hdrSize, 2, 1, in);

fseek(in, offset + 20, 0);
fread(fmt, 2, 1, in);

if (*fmt != 0x01)
{
	fseek(in, offset + hdrSize + 20, 0);
	fread(fact, 4, 1, in);
	if ( !memcmp(fact, "fact", 4) )
		hdrSize += 36;
	else
		hdrSize += 24;
}
else
	hdrSize += 24;

fseek(in, offset + hdrSize, 0);
fread(size, 4, 1, in);
return *size + hdrSize + 4;

}

//===========================================================================

uint SystemX::WmaSize(FILE* in, const uint& offset){

uchar sig[2048];// Seh
int off = 0;
int size;

fseek(in, offset, 0);
fread(sig, 2048, 1, in);

for(int x = 0; x < 2048; ++x)
	if (sig[x] == 0x53 && sig[x+1] == 0x65 && sig[x+2] == 0x68)
	{
		off = x;
		break;
	}

if (!off)
	return 0;

fseek(in, offset + off + 26, 0);
fread(&size, 4, 1, in);
return size;

}

//===========================================================================

uint SystemX::AsfSize(FILE* in, const uint& offset){

uint hdrSize;
//uint size[4];

fseek(in, offset + 4, 0);
fread(&hdrSize, 4, 1, in);

fseek(in, offset + hdrSize + 12, 0);
fread(&hdrSize, 4, 1, in);

//for (int x = 0; x <
// End = SCEl

// Not finished

return 0;

}

//===========================================================================

// Riff Wav Audio specs
void SystemX::RiffSpecs(FILE* in, const bool& getSizes){

uchar fmt[4], bits[2];
short hdrSize;

for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	fread(fmt, 4, 1, in);

	if (!strncmp((char*)fmt, "RIFF", 4))
	{
		fseek(in, FItem.Offset[x] + 16, 0);
		fread(&hdrSize, 2, 1, in);

		fseek(in, FItem.Offset[x] + 20, 0);
		fread(fmt, 2, 1, in);

		if (*fmt == 0x69)
			PushAudioFormat("xadpcm");
		else if (*fmt == 0x01)
			PushAudioFormat("pcm");
		else if (*fmt == 0x11)
			PushAudioFormat("imaadpcm");
		else
			PushAudioFormat("unknown");

		PushChannel(in, 2);
		PushSampleRate(in, 4);

		fseek(in, ftell(in) +6, 0);
		fread(bits, 2, 1, in);
		PushBits(*bits);

		if (getSizes)
		{
			FItem.Chunk.push_back(RiffSize(in, FItem.Offset[x]));
		}

	}
	else
	{
		FItem.Format.push_back("");
		FItem.Bits.push_back(NULL);
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
	}

}

}// End RiffSpecs

//===========================================================================

// (Very much built on hacks: needs fixing)
void SystemX::WmaSpecs(FILE* in, const bool& getSizes){

//uchar bitrate[2];
uchar hdr[16];
//ushort serOffset[2];
//ushort tagSize[2];


for (int x = 0; x < CItem.Files; ++x)
{
	fseek(in, FItem.Offset[x], 0);
	fread(hdr, 16, 1, in);

	if (!memcmp(hdr, wma.header, 16))
	{
		if (getSizes)
			FItem.Chunk.push_back(sys.WmaSize(in, FItem.Offset[x]));

		//fseek(in, Offset[x] + 150, 0);
		//fread(serOffset, 2, 1, in);

		uchar sig[8192];

		fseek(in, FItem.Offset[x], 0);
		fread(sig, 8192, 1, in);

		int pos = 0;

		for(int y = 0; y < 8192; ++y)
			if ( (sig[y] == 0x53 && sig[y+1] == 0x65)
				&& (sig[y+2] == 0x72 || sig[y+2] == 0x73) && sig[y+3] == 0x00 )
			{
				pos = y;
				break;
			}

		//if (pos!=0)
		//{
			fseek(in, FItem.Offset[x] + pos + 66, 0);

			// Channels
			PushChannel(in, 2);

			// SampleRate
			PushSampleRate(in, 4);

			// Bits
			uint bits[2];
			fseek(in, ftell(in) + 6, 0);
			fread(bits, 2, 1, in); PushBits(*bits);
		//}
		//else
		//{
			// Channels
		//	sys.PushChannel(0);

			// SampleRate
		//	sys.PushSampleRate(0);
		//	sys.PushBits(0);
		//}

		PushAudioFormat("wma");

		 //gui.Msg(*serOffset);

//		if ( *serOffset == 114 )
//		{
//			// Channels position
//			pos = *serOffset - 102;
//			//gui.Msg(pos);
//		}
//		else if ( *serOffset != 46 )  //*serOffset == 1196 && *serOffset == 1519
//		{
//			fseek(in, *serOffset + 150, 0);
//			fread(tagSize, 2, 1, in);
//
//			// We will now go start of this:
//			// W.i.n.d.o.w.s. .M.e.d.i.a. .A.u.d.i.o. .9...$.1.6.0.
//			pos = *serOffset + *tagSize + 182;
//			//fseek(in, pos, 0);
//
//			// Bitrate
//			//fseek(in, pos + 46, 0);
//			//gui.Msg(pos);
//		}
//		else if ( *serOffset == 46 )
//		{
//			fseek(in, Offset[x] + 196, 0);
//			fread(serOffset, 2, 1, in);
//
//			pos = Offset[x] + *serOffset + 192 - 2 + 70 - 202;
//			//gui.Msg(pos);
//		}
//
//
//		else
//		{
//			AudioFormat.push_back(NULL);
//			Bits.push_back(NULL);
//			Channels.push_back(NULL);
//			SampleRate.push_back(NULL);
//			continue;
//		}
//
//
//		// Channels
//		fseek(in, pos + 202, 0);
//		PushChannel(in, 2);
//
//		// SampleRate
//		PushSampleRate(in, 4);
//
//		// Bits
//		fseek(in, pos + 214, 0);
//		fread(bits, 2, 1, in); PushBits(*bits);
//
//		PushAudioFormat("wma");
	}
	else
	{
		FItem.Format.push_back(NULL);
		FItem.Bits.push_back(NULL);
		FItem.Channels.push_back(NULL);
		FItem.SampleRate.push_back(NULL);
	}

}

}// End WmaSpecs

//===========================================================================
