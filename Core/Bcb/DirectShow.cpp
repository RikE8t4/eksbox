


//=============================
//		DirectShow.cpp
//=============================

//#define TESTDISABLE

#include <stdafx.h>
#pragma hdrstop
#include <memory>
#include <math.h> // Crossfade
#include "dir.h"

#include "Gui.h"
#include "Video.h"
#include "DirectShow.h"
#include "MediaPlayer.h"
#include "Strings.h"
#include "SystemX.h"

#ifdef __BORLANDC__
#include "Main.h"
#endif

DirectShow DS;

#define TIMERDEFAULT 310
#define TIMERSEEK 50

// TODO
// Port code so it runs on VC++

//==============================================================================

DirectShow::DirectShow(){

UpdateTimer = new TTimer(NULL);
UpdateTimer->Interval = TIMERDEFAULT;
UpdateTimer->OnTimer = TimerHandler;
CoInitialize(NULL);
GraphMan = NULL;
MediaControl = NULL;
MediaEvent = NULL;
MediaPos = NULL;
VideoWindow = NULL;
BasicAudio = NULL;
IsPlaying = false;
Error = false;
IsPaused = false;
FIsSeeking = false;
FIsFFRewind = false;
FForward = false;
Looping = false;
LoopStart = 0;
LoopEnd = 0;

Volume = 0;
//MediaSeeking = NULL;
//VideoFrameStep = NULL;

}

DirectShow::~DirectShow(){

Stop(false);
UpdateTimer->Enabled = false;
delete UpdateTimer;
//MediaControl->Stop();
//GraphMan->Release();
//MediaControl->Release();
//MediaEvent->Release();
//MediaPos->Release();
//VideoWindow->Release();
CleanUp();
CoUninitialize();

}

//==============================================================================

void DirectShow::HandleEvent(){

  long param1, param2;
  HRESULT hr;

  //while (MediaEvent->GetEvent(&eventCode, &param1, &param2, 0) != E_ABORT)
  while (hr = MediaEvent->GetEvent(&EventCode, &param1, &param2, 0), SUCCEEDED(hr))
  {
	//hr =
	MediaEvent->FreeEventParams(EventCode, param1, param2);

	switch (EventCode)
	{
		case EC_COMPLETE:
			// here when media is completely done playing
			//if (!g_Looping)

			ShowMessage("Stop");
			Stop(true);
			//IsPlaying = false;
			//MediaControl->Stop();
			break;

		default:

		if (IsPlaying)
		if (Looping && TPos >= LoopEnd)
		{
			gui.Caption(str.IntToStr(TPos));
			MediaPos->put_CurrentPosition(LoopStart);
			TPos = LoopStart;
		}
			break;

	}
	MediaEvent->FreeEventParams(EventCode, param1, param2);
  }
}

//==============================================================================

bool DirectShow::Play(){

#ifdef __BORLANDC__
sys.ProcessMessages();
#endif

if (!sys.FileExists(FileName.c_str())) return false;

gui.CaptionMP(FileName.c_str());
gui.Status("Initializing Media Player...", 0, true);

if (Form1->Width != gui.mMidWidth && Form1->Width != gui.mMinWidth)
	gui.MinWidthMode();

if (IsPlaying || Error){
	try
	{
		Stop(false);
	}
	catch(Exception &e)
	{
		gui.Status("DS Error: IsPlaying (" + str.GetFileName(FileName.c_str()) + ")" , 0, true);
	}
}

Error = false;
Looping = false;
LoopStart = 0;
LoopEnd = 0;
//BUpdateCaption = true;


HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
IID_IGraphBuilder, (void **) &GraphMan);

if (FAILED(hr)){
	Error = true;
	IsPlaying = false;
	gui.Status("DS Error: CoCreateInstance (" + str.GetFileName(FileName.c_str()) + ")", 0, true);
	return false;
}

GraphMan->QueryInterface(IID_IMediaControl, (void **) &MediaControl);
GraphMan->QueryInterface(IID_IMediaEvent, (void **) &MediaEvent);
GraphMan->QueryInterface(IID_IMediaPosition, (void **) &MediaPos);
GraphMan->QueryInterface (IID_IVideoWindow, (void**) &VideoWindow);

//MediaEvent->SetNotifyWindow((OAHWND)VideoWnd->Handle, WM_GRAPHNOTIFY, 0);
MediaEvent->SetNotifyWindow((OAHWND)Form1->Handle, WM_GRAPHNOTIFY, 0);
//MediaEvent->SetNotifyFlags(0);	// Turn on notifications


// Title caption
WCHAR caption[MAXPATH];
SName = ExtractFileName(FileName);
SName.WideChar(caption, MAXPATH);


// Remove extension as we will use this filename for display purposes
int extLen = ExtractFileExt(SName).Length();
if (extLen)
	SName.SetLength(SName.Length() - extLen);
//CLIPSTRING(name);


// Render                                   
FileName.WideChar(WFileName, MAX_PATH);
hr = GraphMan->RenderFile(WFileName, NULL);
if (FAILED(hr)){
	Error = true;
	IsPlaying = false;
	if (PlayingFormat == "xadpcm")
	{
		if (sys.GetWinVer() <= 4) // Win 9x
			gui.Status("Xbox Adpcm playback only supported on XP/Vista", 0, true);
		else
			gui.Status("Install The Xbox Adpcm Codec", 0, true);
	}
	else
	gui.Status("DS Error: Render File (" + str.GetFileName(FileName.c_str()) + ")", 0, true);
	//gui.Status("Render File Error", 0, true);
	return false;
}


long vidWidth = NULL;
VideoWindow->get_Width(&vidWidth);

if (vidWidth)
{
	SetVideoSpecs();
	Form1->ListView1->Visible = false;
	//Form1->ListView1->Visible=false;
	//Form1->ListView1->ViewStyle = vsList;//Test
	IsVideo = true;
}
else{
	IsVideo = false;
	//Form1->ListView1->Visible=true;
	//Form1->ListView1->ViewStyle = vsReport;//Test
}


gui.OpenMedia();
gui.MPProgressBarSetup();
Application->BringToFront(); // ?
MediaPos->put_CurrentPosition(0);
MediaPos->get_Duration(&TDuration);
MediaControl->Run();
IsPlaying = true;

GraphMan->QueryInterface (IID_IBasicAudio, (void**) &BasicAudio);
Volume = Form1->EkszTrackBar->Position -5000;
	BasicAudio->put_Volume(Volume);

//CLEANUP(BasicAudio);

gui.ProgressSetMax(TDuration);
SDuration = str.FormatTime(TDuration).c_str();

#ifndef TESTDISABLE
while (MediaEvent->WaitForCompletion (0, &EventCode) && IsPlaying)
{
	#ifdef __BORLANDC__
	sys.ProcessMessages();
	#endif

	if (!Form1->Focused() && !Form1->Minimized) // Req. to enable key controls
		Form1->SetFocus();

	if (Looping && TPos >= LoopEnd)
	{
		MediaPos->put_CurrentPosition(LoopStart);
		TPos = LoopStart;
	}

//		tagPOINT t;
//		GetCursorPos(&t);
//
//		if (!gui.CursorIsHidden && gui.CursorHasTimedOut()
//			&&
//			t.x >= VideoWnd->ClientOrigin.x &&
//			t.x <= VideoWnd->ClientOrigin.x + VideoWnd->Width &&
//			t.y >= VideoWnd->ClientOrigin.y &&
//			t.y <= VideoWnd->ClientOrigin.y + VideoWnd->Height
//		){
//			//VideoWindow->HideCursor(OATRUE);
//			gui.CursorIsHidden = true;
//			ShowCursor(false);
//			ShowMessage("Hide");
//		}

}

Stop(false);
//gui.Close();

if (Form1->PopMenuOnTop->Checked)
	Form1->FormStyle = fsStayOnTop;

#endif

return true;

}

//==============================================================================

//void DirectShow::WaitForCompletion()
//{
//LONG lEvCode = 0;
//OAFilterState state = State_Running;
//if (!GraphMan)
//	return;
//do
//{
//	MSG Message;
//	MediaControl->GetState(10, &state);
//	if(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
//	{
//		TranslateMessage(&Message);
//		DispatchMessage(&Message);
//	}
//} while (state != State_Stopped);
//}

//==============================================================================

void DirectShow::CleanUp(){

if (IsPlaying)
{
	Stop(false);
}

try
{
	CLEANUP(GraphMan);
	CLEANUP(MediaControl);
	CLEANUP(MediaEvent);
	CLEANUP(MediaPos);
	CLEANUP(VideoWindow);
	CLEANUP(BasicAudio);
}
catch(Exception &e)
{
	ShowMessage("DirectShow Clean Error");
}

}

//==============================================================================

void DirectShow::Stop(const bool& stopMsg){

if (IsPlaying || Error)
{
	IsPlaying = false;

	EventCode = State_Stopped;
	if (MediaPos != NULL){
		MediaControl->StopWhenReady();
		//MediaPos->put_CurrentPosition(0);
		//VideoWindow->put_Owner(NULL);
	}
	//MediaPos->put_CurrentPosition(0);   // Reset position
	//if (VideoWnd->Visible && !MediaPlayer.PlayListGetSize()){
	if (stopMsg)
	if (VideoWnd->Visible){
		VideoWnd->Close();
		VideoWindow->put_Visible(OAFALSE);
		Form1->ListView1->Visible = true;
	}

	if (stopMsg)
		gui.Status("Playback Stopped", 0, true);

	IsScrolling = false;
	gui.MPProgressBarReset();
	if (Form1->PopMenuOnTop->Checked)
		Form1->FormStyle = fsStayOnTop;
}

}

//==============================================================================

float DirectShow::GetPos(){

return TPos;

}

//==============================================================================

float DirectShow::GetDuration(){

return TDuration;

}

//==============================================================================

void DirectShow::SetVolume(long level){

if (IsPlaying){
	//Full volume is 0, and –10,000 is silence
	if (level == -5000)	level = -10000;
	Volume = level;
	GraphMan->QueryInterface (IID_IBasicAudio, (void**) &BasicAudio);
	BasicAudio->put_Volume(Volume);
}

}

//==============================================================================

void DirectShow::Crossfade(int fadeLength){

if (!IsPlaying) return;

//float x = msg.pt.x - Form1->Left - EkImgProgressBar->Left -3;
//float step = play.GetDuration() / EkImgProgressBar->Width;
//float seekTo = x * step;

//do
//{
//  d+=1/frequency;
//
//  fade = cos(PI/_length*(d-_startTime)/2)
//			*(_startVolume - _finishVolume )+_finishVolume;
//
// signal*=fade;
//}
//
//while(d<(_length+_startTime));

//dB = 20 x log (V1/V2)

if (!fadeLength) fadeLength = 1;

BasicAudio->get_Volume((long*)&Volume);
										  //10000
//int step = (vol + 10000) * (fadeLength / 1000);

//ShowMessage("Vol: " + String(Volume));

float l = .5;

for (; Volume >= -10000; l += l, Volume -= l){
	BasicAudio->put_Volume(Volume);
	Sleep(fadeLength*45);
}

}

//==============================================================================

void DirectShow::Seek(const double& pos){

if (IsPlaying)
{
	MediaPos->put_CurrentPosition(pos);
	TPos = pos;
}
}

//==============================================================================

void DirectShow::Pause(){

if (!IsPlaying) return;

if (!IsPaused)
{
	MediaControl->Pause();
	IsPaused = true;
}
else
{
	IsScrolling = false;
	MediaControl->Run();
	IsPaused = false;
}
}

//==============================================================================

void DirectShow::SetVideoSpecs(){

//long w, h;
//VideoWindow->get_Width(&w);
//VideoWindow->get_Height(&h);
//
//int left = (Screen->Width / 2) - w / 2;
//int top = Form1->Top - h;
//MoveWindow(VideoWnd->Handle, left, top, w, h, true);
//gui.CursorStartTimer();

VideoWnd->Width = Form1->ListView1->Width+6;
VideoWnd->Height = Form1->ListView1->Height+9;

VideoWnd->Left = 27;
VideoWnd->Top = 21;

//if (Form1->Height != gui.MinHeight)
if (Form1->Height == gui.mMinHeight)
	gui.ToggleMiniMode();

//VideoWnd->Focused();
long vidWidth = VideoWnd->Width;
long vidHeight = VideoWnd->Height;

if (!VideoWnd->Visible)
	VideoWnd->Show();

VideoWnd->Parent = Form1;
VideoWindow->put_Owner((OAHWND)VideoWnd->Handle);
VideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);


if (IsFullscreen){ // Re-enable fullscreen
	IsFullscreen = !IsFullscreen;
	ToggleFullScreen();
}
else
	VideoWindow->SetWindowPosition(1, 1, vidWidth-2, vidHeight-6);
	//VideoWindow->SetWindowPosition(0, 0, vidWidth, vidHeight);

//VideoWindow->put_Caption(caption);

}

//==============================================================================

void DirectShow::ToggleFullScreen(){

if (!IsPlaying || !IsVideo) return;

if (!IsFullscreen)
{
	return;// Disable until it works
	
	// Get screen resolution
	SIZE size;
	size.cx = GetSystemMetrics(SM_CXSCREEN);
	size.cy = GetSystemMetrics(SM_CYSCREEN);

	VideoWnd->ManualFloat(Rect(0,0,size.cx, size.cy));
	VideoWindow->put_Owner((OAHWND)VideoWnd->Handle);
	VideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);

	VideoWnd->Top = 0;
	VideoWnd->Left = 0;
	VideoWnd->Width = size.cx;
	VideoWnd->Height = size.cy;

	//VideoWnd->PopupMode = pmNone;
	VideoWindow->SetWindowPosition(0, 0, VideoWnd->Width, VideoWnd->Height);
}
else
{
	VideoWnd->Left = Form1->Left + Form1->ListView1->Left;
	VideoWnd->Top = Form1->Top + Form1->ListView1->Top;
	VideoWnd->Width = Form1->ListView1->Width;
	VideoWnd->Height = Form1->ListView1->Height;
	VideoWindow->SetWindowPosition(0, 0, VideoWnd->Width, VideoWnd->Height);
	//VideoWnd->PopupMode = pmExplicit;
}                         

IsFullscreen = !IsFullscreen;

}

//==============================================================================

bool DirectShow::GetIsOverVideo(){

POINT pt;
GetCursorPos(&pt);

if (pt.x >= VideoWnd->ClientOrigin.x &&
	pt.x <= VideoWnd->ClientOrigin.x + VideoWnd->Width &&
	pt.y  >= VideoWnd->ClientOrigin.y &&
	pt.y <= VideoWnd->ClientOrigin.y + VideoWnd->Height)
{
	return true;
}

return false;

}

//==============================================================================

void DirectShow::LoopRegionToggle(){

Looping = ! Looping;

}

//==============================================================================

void DirectShow::SetLoopRegions(double start, double end){

if (Looping)
{ 	// Reset loop
	Looping = false;
	return;
}

if (start)
	LoopStart = start;
if(end)
	LoopEnd = end;

if (LoopEnd && LoopEnd > LoopStart)
{
	Looping = true;
}

}

//==============================================================================

void __fastcall DirectShow::TimerHandler(TObject* Sender){

if (IsPlaying && FIsFFRewind){
	UpdateTimer->Interval = TIMERSEEK;
	UpdateCaptionFFRew();
}
else if (IsPlaying && !FIsSeeking){
	UpdateTimer->Interval = TIMERDEFAULT;
	UpdateCaption();
}
else if (IsPlaying && FIsSeeking){
	UpdateTimer->Interval = TIMERSEEK;
	UpdateCaptionSeek();
}

//if (GetIsOverVideo())
//	ShowCursor();
//	//VideoWnd->Cursor = Controls::crNone;
//else
//	ShowCursor();
//	//VideoWnd->Cursor = Controls::crDefault;

}

//==============================================================================

void DirectShow::UpdateCaption(){

if (Error) return; //|| !BUpdateCaption

MediaPos->get_CurrentPosition(&TPos);
gui.ProgressSetPosition(TPos);

String nameAndTime = SName + " - [" + str.StdToAnsi(str.FormatTime(TPos));
if (!IsPaused)
	Form1->EkImgProgressBar->Text = nameAndTime + " - " + SDuration + "]";
	//gui.Status(nameAndTime + " - " + SDuration + "]", 0, true);
	//gui.ProgressCaptionBasic(nameAndTime + " - " + SDuration + "]", true);
else{
	gui.ProgressSetScroll(false);
	Form1->EkImgProgressBar->Text = "Playback Paused";
}
	//gui.Status("Playback Paused", 0, true);
                       //gui.ScrollTxtIsOn()
if (!IsScrolling){
	gui.MPProgressBarSetup();
	//if (ScrollTxtIsOn())
	IsScrolling = true;
}

}

//==============================================================================

void DirectShow::UpdateCaptionSeek(){

if (Error) return;

gui.ProgressSetScroll(false);
IsScrolling = false;
double pos = gui.ProgressGetPosition();

//String nameAndTime = SName + " - [" + str.FormatTime(pos);
String nameAndTime = "Seek To: [" + str.StdToAnsi(str.FormatTime(pos));
//if (!IsPaused)
	Form1->EkImgProgressBar->Text = nameAndTime + " - " + SDuration + "]";

//MediaPos->put_CurrentPosition(pos);
//TPos = pos;

}

//==============================================================================

void DirectShow::UpdateCaptionFFRew(){

if (Error) return;

gui.ProgressSetScroll(false);
gui.ProgressSetPosition(TPos);
IsScrolling = false;
MediaPos->get_CurrentPosition(&TPos);
String nameAndTime;

if (FForward)
	nameAndTime = "Fast Forward: [" + str.StdToAnsi(str.FormatTime(TPos));
else
	nameAndTime = "Rewind: [" + str.StdToAnsi(str.FormatTime(TPos));

Form1->EkImgProgressBar->Text = nameAndTime + " - " + SDuration + "]";

}

//==============================================================================


