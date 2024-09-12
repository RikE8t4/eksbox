#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

//=============================
//		MediaPlayer.h
//=============================

#include <stdafx.h>
#pragma hdrstop
#include "DirectShow.h"

//==============================================================================

class CMediaPlayer {

private:
	bool NoSoundcard;
	bool VideoFormat;
	bool ImageFormat;
	bool AudioFormat;
	string MiscFormat;
	bool Stopped;
	bool Busy;
	bool PLLooping;
	bool FForward, FRewind;
	int Current;
	double SeekGrow;
	FILE* In;
	FILE* Out;
	string TempDir;
	bool ExtractToTemp();
	vector<String>PlayList;
	vector<String>Format;
	vector<int>FileSize;
	string PlayingFileName;
	TTimer* ForwRwndTimer;
	void __fastcall TimerHandler(TObject* Sender);
	void MsgNoSoundcard();
	bool SupportedVideoFormat(const string& path);
	bool SupportedAudioFormat(const string& path);
	bool SupportedImageFormat(const string& path, const bool& directShow);
	bool SupportedMiscFormat(const string& path);

public:
	bool Pause();
	bool Stop();
	float GetPosition();
	float GetDuration() {return DS.GetDuration();}
	void SetVolume(long level);
	void Crossfade(int fadeLength);
	void Seek(float pos);
	void CleanUp();
	bool Play(int num);
	bool Play(String path);
	bool PlayPrev();
	bool PlayNext();
	bool PlayNext(String path);
	void Rewind();
	void FastForward();

	int GetCurrentNum(){return Current;}
	string GetPlayingFileName() {return PlayingFileName;}
	void StopUpdateCaption() {DS.StopUpdateCaption();}
	void StartUpdateCaption() {DS.StartUpdateCaption();}
	void SetIsScrolling(bool status) {DS.SetIsScrolling(status);}
	bool GetIsPlaying() {return DS.GetIsPlaying();}
	bool GetIsPaused() {return DS.GetIsPaused();}
	bool GetIsLoopingPlaylist() {return PLLooping;}
	bool GetIsVideo() {return DS.GetIsVideo();}
	void SetVideoSpecs() {DS.SetVideoSpecs();}
	bool GetIsFullScreen() {return DS.GetIsFullScreen();}
	void SetIsLoopingPlaylist(bool state) {PLLooping = state;}
	void ToggleFullScreen() {DS.ToggleFullScreen();}
	void SetIsSeeking(bool state) {DS.FIsSeeking = state;}
	void SetIsFFRewind(bool state);

	void SetLoopRegions(double start, double end) {DS.SetLoopRegions(start, end);}
	void LoopRegionToggle() {DS.LoopRegionToggle();}

	void SetStopped(bool state);
	void SetBusy(bool state);
	void Reset();
	void ListSelectCurrent();

	bool IsValidFormat(const string& path);
	void PlayListAddEntry(String path);
	void PlayListDelLastEntry();
	void PlayListClear();
	void PlayListLoad();
	void PlayListPlay();
	void PlayListPlayNext();
	void PlayListPlayPrev();
	int PlayListGetSize() {return PlayList.size();}
	CMediaPlayer();
	~CMediaPlayer();

};

extern CMediaPlayer MediaPlayer;

//==============================================================================

#endif