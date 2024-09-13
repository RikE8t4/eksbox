

//===============================
//		FileSwapHelper.h
//===============================

#include <stdafx.h>
#pragma hdrstop

#define SWAPDIRNAME "(FileSwap)"
#define	MAX 16000

short GetWavHdrSize(FILE* in);


int GetWmaSpecs(ushort& channels, ushort& sampleRate,
ushort& bits, FILE* in);


int GetAudioFormat(const string& fileName,
		bool IsPcmOk, bool IsXadpcmOk, bool IsWmaOk,
		string& audioFmt, ushort& channels, ushort& sampleRate,
		ushort& bits, FILE* in);


