#ifndef DIRECTSHOW_H
#define DIRECTSHOW_H

//=============================
//		DirectShow.h
//=============================


#include <stdafx.h>
#pragma hdrstop
#include "Dshow.h"

#define WM_GRAPHEVENT WM_APP + 1

#define CLEANUP(x) 	  if (x != NULL) {\
						x->Release();\
						x = NULL;\
						}

//==============================================================================

class DirectShow{

public:
	bool   Error;
	String FileName;

	void HandleEvent();
	bool Play();
	//bool Play(AnsiString fileName);
	void Stop(const bool& stopMsg);
	float GetPos();
	float GetDuration();
	void SetVolume(long level);
	void Crossfade(int fadeLength);
	void Seek(const double& pos);
	void Pause();

	void SetPlayingFormat(String format) {PlayingFormat = format;}
	bool GetIsPlaying() {return IsPlaying;}
	bool GetIsPaused() {return IsPaused;}
	bool GetIsVideo() {return IsVideo;}
	bool GetIsFullScreen() {return IsFullscreen;}
	void SetIsScrolling(bool status) {IsScrolling=status;}
	void ToggleFullScreen();
	void SetVideoSpecs();
	void StopUpdateCaption() {BUpdateCaption = false;}
	void StartUpdateCaption() {BUpdateCaption = true;}
	void SetLoopRegions(double start, double end);
	void LoopRegionToggle();

	void ResetTimer(){UpdateTimer->Enabled = false; UpdateTimer->Enabled = true;}
	bool FIsSeeking;
	bool FIsFFRewind;
	bool FForward;

	DirectShow();
	~DirectShow();

private:
	//std::auto_ptr<IGraphBuilder>GraphMan;
	//CComPtr
	IGraphBuilder* GraphMan;
	IMediaControl* MediaControl;
	IMediaEventEx* MediaEvent;
	IMediaPosition* MediaPos;
	IVideoWindow* VideoWindow;
	IBasicAudio* BasicAudio;
	//IMediaSeeking 	*MediaSeeking;
	//IVideoFrameStep	*VideoFrameStep;
	WCHAR 			WFileName[MAX_PATH];
	bool 			IsPaused;
	bool			IsPlaying;
	bool 			IsScrolling;
	bool 			IsFullscreen;
	long			EventCode;
	long 			Volume;
	bool			IsVideo;
	bool			BUpdateCaption;
	bool 			Looping;
	double 			LoopStart;
	double			LoopEnd;
	String			PlayingFormat;
	String			SName;
	String 			SDuration;
	//bool			Error;
	TTimer* 		UpdateTimer;
	REFTIME 		TPos;
	REFTIME			TDuration;

	void __fastcall TimerHandler(TObject* Sender);
	void UpdateCaption();
	void UpdateCaptionSeek();
	void UpdateCaptionFFRew();
	void CleanUp();
	bool GetIsOverVideo();
	//void SetVideoSpecs();
	//void WaitForCompletion();
};

extern DirectShow DS;

//==============================================================================

#endif
