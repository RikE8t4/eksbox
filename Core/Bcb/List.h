#ifndef LIST_H
#define LIST_H

//=============================
//		List.h
//=============================

#include <stdafx.h>
#pragma hdrstop
#include "Main.h"


struct TFileItem
{
	string FileName;
	ulong Offset;
	string Size;
	ushort SampleRate;
	short Channels;
	short Bits;
	string Format;
	int IconIndex;
	int StateIndex;
	bool Checked;
};


class EkszList{

	private:

		bool mOffsetsShow;

	public:

		void Clear();

		void Load();

		void CheckAllNone(const bool& check);

		void CheckAllAudio();

		void SetShowOffsets(bool show) { mOffsetsShow = show; }

		bool GetShowOffsets() { return mOffsetsShow; }

		TList* mFileList;

		EkszList();

		~EkszList();

};

#endif
