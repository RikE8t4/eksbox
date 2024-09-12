


//=============================
//		MediaPlayer.cpp
//=============================

#include <stdafx.h>
#pragma hdrstop
#include <dir.h>

#include "Engine.h"
#include "SystemX.h"
#include "Gui.h"
#include "Strings.h"
#include "DirectShow.h"
#include "MediaPlayer.h"
#include "LowLevel.h"
#include "Video.h"

#include "Convert/Adx2wav.h"
#include "Convert/ConvertStx.h"

CMediaPlayer MediaPlayer;

// TODO
// Port code so it runs on VC++

//==============================================================================

CMediaPlayer::CMediaPlayer(){

if (waveOutGetNumDevs() == 0)
	NoSoundcard = !true;
else
	NoSoundcard = false;

SeekGrow = 0;
Current = 0;
PLLooping = false;
TempDir = sys.GetTempDir();
ForwRwndTimer = new TTimer(NULL);
ForwRwndTimer->Interval = 40;
ForwRwndTimer->OnTimer = TimerHandler;
ForwRwndTimer->Enabled = false;
//ForwRwndTimer->Enabled = true;

}

//==============================================================================

CMediaPlayer::~CMediaPlayer(){

PlayList.clear();
Format.clear();
FileSize.clear();
delete ForwRwndTimer;

}

//==============================================================================

void CMediaPlayer::MsgNoSoundcard(){

gui.Status("Error: No soundcard/soundcard drivers", 0, true);

}

//==============================================================================

void CMediaPlayer::Reset(){

Current = 0;

}

//==============================================================================

void CMediaPlayer::SetStopped(bool state){

Stopped = state;

}

//==============================================================================

bool CMediaPlayer::SupportedVideoFormat(const string& path){

string ext = str.ToLower(str.GetFileExt(path));

if (ext == "asf" || ext == "avi" || ext == "m1v" || ext == "m2v"
 || ext == "mpg" || ext == "mpeg" || ext == "wmv")
return true;

return false;
}

//==============================================================================

bool CMediaPlayer::SupportedAudioFormat(const string& path){

string ext = str.ToLower(str.GetFileExt(path));
AudioFormat = false;

if (ext == "adx" || ext == "stx")
{
	AudioFormat = true;
	return true;
}

return false;

}

//==============================================================================

bool CMediaPlayer::SupportedImageFormat(const string& path, const bool& directShow){

string ext = str.ToLower(str.GetFileExt(path));
ImageFormat = false;

if (ext == "bmp" || ext == "jpg" || (!directShow && ext == "png")
	|| (!directShow && ext == "tga") || (!directShow && ext == "tif")
	|| (!directShow && ext == "ico")
)
{
	ImageFormat = true;
	return true;
}

return false;
}

//==============================================================================

bool CMediaPlayer::SupportedMiscFormat(const string& path){

string ext = str.ToLower(str.GetFileExt(path));
MiscFormat = "";

if (ext == "txt")
{
	MiscFormat = ext;
	return true;
}

return false;

}

//==============================================================================

bool CMediaPlayer::IsValidFormat(const string& path){

string ext = str.ToLower(str.GetFileExt(path));
VideoFormat = false;


if (ext == "mp3"){
	Format.push_back("mp3");
	//FItem.Ext.push_back(".mp3");
}
else if (ext == "ogg"){
	Format.push_back("ogg");
	//Ext.push_back(".ogg");
}
else if (ext == "m3u"){
	//Format.push_back("m3u");
	//Ext.push_back(".ogg");
}
else if (ext == "wav"){ // Test
	Format.push_back("wav");
}
else if (ext == "wma"){ // Test
	Format.push_back("wma");
}
else if (ext == "flac"){
	Format.push_back("flac");
}
else if (ext == "asf"){
	if (fmtUtil.DetectEaAsf(path.c_str(), 0)) return false;
	Format.push_back("asf");
	VideoFormat = true;
	//Ext.push_back(".asf");
}
else if (ext == "avi"){
	Format.push_back("avi");
	VideoFormat = true;
	//Ext.push_back(".avi");
}
else if (ext == "m1v"){
	Format.push_back("m1v");
	VideoFormat = true;
	//Ext.push_back(".m1v");
}
else if (ext == "m2v"){
	Format.push_back("m2v");
	VideoFormat = true;
	//Ext.push_back(".m2v");
}
else if (ext == "mpg"){
	Format.push_back("mpg");
	VideoFormat = true;
	//Ext.push_back(".mpg");
}
else if (ext == "mpeg"){
	Format.push_back("mpeg");
	VideoFormat = true;
	//Ext.push_back(".mpeg");
}
else if (ext == "wmv"){
	Format.push_back("wmv");
	VideoFormat = true;
	//Ext.push_back(".wmv");
}
else
return false;


DS.SetPlayingFormat(ext.c_str());
return true;

//if (ext == "jpg"){
//	Format.push_back("jpg");
//	Ext.push_back(".jpg");
//  return true;
//}
//if (ext == "bmp"){
//	Format.push_back("bmp");
//	Ext.push_back(".bmp");
//  return true;
//}

}

//==============================================================================

bool CMediaPlayer::Pause(){

DS.FIsSeeking = false; // No seek display when paused
DS.Pause();
return true;

}

//==============================================================================

bool CMediaPlayer::Stop(){

if (!CItem.Files && !PlayList.size() && !DS.GetIsPlaying())
	return false;

Stopped = true;

if (Form1->Width != gui.mMinWidth && !Form1->ListView1->Visible)
	Form1->ListView1->Visible = true;

DS.Stop(true);
return true;

}

//==============================================================================

float CMediaPlayer::GetPosition(){

if (!CItem.Files && !PlayList.size() && !DS.GetIsPlaying())
	return false;

return DS.GetPos();

}

//==============================================================================

void CMediaPlayer::SetVolume(long level){

if (Form1->Height != gui.mMinHeight)
	DS.SetVolume(level);
else
	DS.SetVolume(level);

}

//==============================================================================

void CMediaPlayer::Crossfade(int fadeLength){

DS.Crossfade(fadeLength);

}

//==============================================================================

void CMediaPlayer::Seek(float pos){

if (!CItem.Files && !PlayList.size() && !DS.GetIsPlaying())
	return;

DS.Seek(pos);
}

//==============================================================================

void CMediaPlayer::CleanUp(){

string temp = sys.GetTempDir() + "EkszTemp";
sys.DeleteDir(temp.c_str());

}

//==============================================================================

void CMediaPlayer::ListSelectCurrent(){

if (!Form1->ListView1->Visible) return;

Form1->ListView1->ClearSelection();
Form1->ListView1->Items->Item[Current]->Selected = true;
Form1->ListView1->Items->Item[Current]->Focused = true;

if (CItem.Files <= 1)
	return; // Skip single file containers/formats

int index = Current;

int itemHeight = Form1->ListView1->Items->Item[1]->Position.y -
Form1->ListView1->Items->Item[0]->Position.y;


if (index == Form1->ListView1->Items->Count-1) // 2nd last to last item fudge
	--index;

if (index == 0) { // Last item to first
	Form1->ListView1->Scroll(Form1->ListView1->ViewOrigin.x, Form1->ListView1->Items->Item[0]->Position.y-itemHeight);
	Form1->ListView1->SetFocus();
	return;
}

if (Form1->ListView1->Items->Item[index+1]->Position.y >= Form1->ListView1->Height
|| Form1->ListView1->TopItem->Position.y == Form1->ListView1->Items->Item[index+1]->Position.y
|| Form1->ListView1->TopItem->Position.y < 0)
{
	Form1->ListView1->Scroll(Form1->ListView1->ViewOrigin.x,
	Form1->ListView1->Items->Item[index-1]->Position.y);
}

Form1->ListView1->SetFocus();

}

//==============================================================================

void CMediaPlayer::PlayListAddEntry(String path){

if (PlayList.size())
for (unsigned int i = 0; i < PlayList.size(); ++i)
{
	if (path == PlayList[i]){
		//gui.ProgressCaption("Playlist entry already exists");
		return;
	}
}if (path != "" && CItem.FileSize)
{
	PlayList.push_back(path);
	FileSize.push_back(CItem.FileSize);
	if (GetIsPlaying())
		gui.CaptionMP(path.c_str());
}

}

//==============================================================================

void CMediaPlayer::PlayListDelLastEntry(){

PlayList.pop_back();

}

//==============================================================================

void CMediaPlayer::PlayListClear(){

PlayList.clear();
Format.clear();
FileSize.clear();

}

//==============================================================================

void CMediaPlayer::PlayListLoad(){

if (!PlayList.size()) return;

Current = -1;
Stopped = false;
PlayListPlayNext();

}

//==============================================================================

void CMediaPlayer::PlayListPlay(){
--Current;
PlayListPlayNext();
}

//==============================================================================

void CMediaPlayer::PlayListPlayNext(){

if (!PlayList.size() || Stopped || (!PLLooping && Current+1 == (int)PlayList.size()))
	return;

if (Current+1 != (int)PlayList.size()) // If we're not on the first track inc
	++Current;
else if (PLLooping) // If looping always go back to first if we're on last
	Current = 0;

if ((int)PlayList.size() < Current) return; // Range check
if (!sys.FileExists(PlayList[Current].c_str())) return; // No play file which doesnt exist

if (NoSoundcard){
	MsgNoSoundcard();
	if (!VideoFormat)
	return;
}

Format[Current].c_str()[0] = toupper(Format[Current].c_str()[0]);
Form1->LblArchiveVal->Caption = Format[Current];
Form1->LblSizeVal->Caption = str.SizeToStr(FileSize[Current]).c_str();

DS.FileName = PlayList[Current];
DS.Play();
if (DS.Error) return; // Dont keep looping on error

if ((Current+1 != (int)PlayList.size()) || PLLooping) // If we're not on the last track play next
	PlayListPlayNext();

}

//==============================================================================

void CMediaPlayer::PlayListPlayPrev(){

if (!PlayList.size() || Stopped || (!PLLooping && Current == 0))
	return;

if (Current != 0)
	--Current;
else if (PLLooping)
	Current = PlayList.size()-1;

if ((int)PlayList.size() < Current) return; // Range check
if (!sys.FileExists(PlayList[Current].c_str())) return; // No play file which doesnt exist

if (NoSoundcard){
	MsgNoSoundcard();
	if (!VideoFormat)
	return;
}

Format[Current].c_str()[0] = toupper(Format[Current].c_str()[0]);
Form1->LblArchiveVal->Caption = Format[Current];
Form1->LblSizeVal->Caption = str.SizeToStr(FileSize[Current]).c_str();


DS.FileName = PlayList[Current];
DS.Play();
if (DS.Error) return; // Dont keep looping on error

if (Current >= 0) // If we're not on the first track play prev
	PlayListPlayPrev();

}

//==============================================================================

bool CMediaPlayer::Play(String path){

Current = CItem.Files-1;
DS.FileName = path;
DS.Play();
if (DS.Error) return false; // Dont keep looping on error
return true;

}

//==============================================================================

bool CMediaPlayer::Play(int num){ // Container format play

Current = num;

// Bounds
if (Current >= CItem.Files) // Play first track
	Current = 0;
else if (Current < 0) // Play last track
	Current = CItem.Files-1;

// Init
VideoFormat = false;
ImageFormat = false;
AudioFormat = false;
MiscFormat = "";

if (!IsValidFormat(FItem.Ext[Current])
	&& !SupportedAudioFormat(FItem.Ext[Current])
	&& !SupportedImageFormat(FItem.Ext[Current], false)
	&& !SupportedMiscFormat(FItem.Ext[Current])
	){
	Format.clear();
	return false;
}
			 
Format.clear();


if (SupportedVideoFormat(str.GetFileExt(FItem.Ext[Current])))
	VideoFormat = true;
else
	VideoFormat = false;


if (NoSoundcard && !VideoFormat)
{
	MsgNoSoundcard();
	return false;
}

DS.SetPlayingFormat(FItem.Format[Current].c_str());


if (Form1->Height != gui.mMinHeight) // Listview
	ListSelectCurrent();


if (CItem.Files == 1 && !FItem.Offset[0] && !CItem.NeedsWavHeader &&
	// Supported non container audio's
	( CItem.Format == "wma" && FItem.Format[0] == "wma"
		|| (CItem.Format == "wav" && FItem.Format[0] == "pcm")
		|| (CItem.Format == "wav" && FItem.Format[0] == "xadpcm") ) )
{
	Play(CItem.OpenFileName.c_str());
	return true;
}
else if (CItem.Files == 1 &&
	(FItem.Format[Current] == "bmp" || FItem.Format[Current] == "jpg" ||
	FItem.Format[Current] == "wmv"  || FItem.Format[Current] == "m1v" ||
	FItem.Format[Current] == "mpeg" ||
	FItem.Format[Current] == "mpg"  || FItem.Format[Current] == "avi" ||
	FItem.Format[Current] == "flac" || FItem.Format[Current] == "ogg" ||
	FItem.Format[Current] == "mp3" && CItem.Format != "adf") )
{
	Play(CItem.OpenFileName.c_str());
	return true;
}


if (!ExtractToTemp())
	return false;


// Open images in external programs
if (ImageFormat || MiscFormat == "txt")
{
	sys.ExecuteAndWait(PlayingFileName.c_str());
	gui.Status("Opened/Attempted to open " + FItem.FileName[Current] + FItem.Ext[Current], 0, true);
	return true;
}

DS.FileName = PlayingFileName.c_str();
DS.Play();
if (DS.Error) return false;// Dont keep looping upon error

if (!Stopped) return PlayNext();

return true;

}

//==============================================================================

bool CMediaPlayer::PlayPrev(){

if ((!CItem.Files && !PlayList.size()) || CItem.Files == 1)
	return false;

--Current;

return Play(Current);
}

//==============================================================================

bool CMediaPlayer::PlayNext(){

if ((!CItem.Files && !PlayList.size()) || CItem.Files == 1)
	return false;

//Crossfade(10);
++Current;

return Play(Current);
}

//==============================================================================

void CMediaPlayer::SetIsFFRewind(bool state){

DS.FIsFFRewind = state;
DS.FForward = FForward;

}

//==============================================================================

void CMediaPlayer::FastForward(){

if (Busy && FForward) return;
FForward = true;
FRewind = false;
Busy = true;
ForwRwndTimer->Enabled = true;
//DS.ResetTimer();     //
//StartUpdateCaption();    //

}

//==============================================================================

void CMediaPlayer::Rewind(){

if (Busy && FRewind) return;
FForward = false;
FRewind = true;
Busy = true;
ForwRwndTimer->Enabled = true;

}

//==============================================================================

void __fastcall CMediaPlayer::TimerHandler(TObject* Sender){

float pos = GetPosition();
SeekGrow += 0.04;

if (Busy && FForward){
	pos += 0.1 + SeekGrow;
	MediaPlayer.Seek(pos);
	if (!DS.FIsFFRewind)
		SetIsFFRewind(true);
}
else if (Busy && FRewind){
	pos -= 0.1 + SeekGrow;
	MediaPlayer.Seek(pos);
	if (!DS.FIsFFRewind)
		SetIsFFRewind(true);
}
else // Stop FastForward / Rewind
{
	SetIsFFRewind(false);
	MediaPlayer.Seek(pos);
	ForwRwndTimer->Enabled = false;
}

}

//==============================================================================

void CMediaPlayer::SetBusy(bool state){
			   
Busy = state;

if (!Busy){
	FForward = false;
	FRewind = false;
	SeekGrow = 0;
}

}

//==============================================================================

bool CMediaPlayer::ExtractToTemp(){

if (!CItem.Files)
	return false;

// Disable xadpcm extraction/playback on win 9x or less
if (sys.GetWinVer() <= 4 && FItem.Format[Current] == "xadpcm")
{
	gui.Error("Error: Xbox Adpcm playback not supported on pre XP OS's",
	sys.Delay(false), NULL);
	return false;
}


FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");
if (!in)
{
	sys.ExtractError(CItem.OpenFileName.c_str(), 0);
	gui.Error
	(
	"Couldn't Open: " + str.ExtractFileName(CItem.OpenFileName), sys.Delay(true), in
	);
	return false;
}

//if (ImageFormat)
//	gui.Status("Open image with associated program...", 0, true);
//else
if (ImageFormat || MiscFormat == "txt")
	gui.Status("Opening " + FItem.FileName[Current] + FItem.Ext[Current]
	+ " with associated program...", 0, true);
else
	gui.Status("Processing, Please Wait...", 0, true);

// Extract files

#ifdef __BORLANDC__
sys.ProcessMessages();
#endif

int i = Current;

if (CItem.HasFolders){ // Remove dir structure
	for (int x; x = FItem.FileName[i].find("\\");)
	{
		FItem.FileName[i].erase (x, 1);
		FItem.FileName[i].insert(" - ", x);
	}
}


string ekszDir = "EkszTemp\\";
PlayingFileName = TempDir + ekszDir + FItem.FileName[i] + FItem.Ext[i];
if (FItem.Format[i] == "adx")
	PlayingFileName = str.ChangeFileExt(PlayingFileName, ".wav");

if (sys.FileExists(PlayingFileName.c_str())) // Don't extract an already extracted file...
	return true; // ...Doesn't work on vista


// Make Dir
string dirName = TempDir + ekszDir;

if (!sys.DirectoryExists(dirName))
if (!sys.CreateDir(dirName))
{
	gui.Error("Couldn't Make Directory: " + dirName, sys.Delay(true), 0);
	return -1;
}


if (PlayingFileName.length() > 259)
{
	gui.Error("Error: Max path (259) limit reached", sys.Delay(false), NULL);
	return false;
}


FILE* out = fopen(PlayingFileName.c_str(), "wb");
			
try{
	if (CItem.NeedsWavHeader)
	{
		// Pcm header
		if (FItem.Format[i] == "pcm")
			fmtUtil.WritePcmHeader(
				FItem.Chunk[i], FItem.Channels[i],
				FItem.Bits[i], FItem.SampleRate[i], out
			);
		// Xadpcm header
		else if (FItem.Format[i] == "xadpcm")
			fmtUtil.WriteXadpcmHeader(
				FItem.Chunk[i], FItem.Channels[i], FItem.SampleRate[i], out
			);
	}// End write header

	// Convert adx to pcm for container formats containg adx's
	if (FItem.Format[i] == "adx")
	{
		gui.Status("Decompressing Adx...", 0, true);
		int pos = 0;
		if (FItem.Channels.size()) pos = FItem.Offset[i];
		adx2wav(FItem.Channels[i], FItem.SampleRate[i], pos, in, out);
		fclose(in);
		fclose(out);
		return true;
	}
	else if (CItem.Format == "stx"){
		gui.Status("Converting Stx...", 0, true);
		bool surround = CItem.Files > 1;
		StxToXbadpcm(in, out, FItem.Channels[i], FItem.Offset[i], surround, true, CItem.PercentExtracted);
		fclose(in);
		fclose(out);
		return true;
	}

	uchar* buffer;
	if (! (buffer = sys.MemoryAlloc(FItem.Chunk[i], out)) )
	{
		sys.ExtractError(FItem.FileName[i] + FItem.Ext[i], 0);
		return false;
	}

	if (CItem.Format == "maphalo"){

		Supported sup;
		sup.MapExtract(i, buffer, in, out);
		fclose(in);
		fclose(out);
		return true;
	}


	fseek(in, FItem.Offset[i], 0);
	fread(buffer, FItem.Chunk[i], 1, in);

	// Vice city decryption
	if (CItem.Format == "adf")
	{
		int bufSize = FItem.Chunk[i] -1; // Use a copy as value is changed
		XORBUFC(bufSize, buffer, 0x22); // LowLevel.cpp/h
		gui.Status("Decyrpting Adf...", 0, true);
	}

	fwrite(buffer, FItem.Chunk[i], 1, out);

}
// End try

catch(Exception &e)
{
	remove(PlayingFileName.c_str());
	return false;
}


sys.MemoryDel();
fclose(out);
fclose(in);
return true;
}

//==============================================================================


