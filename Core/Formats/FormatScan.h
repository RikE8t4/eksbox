#ifndef FORMAT_SCAN_H
#define FORMAT_SCAN_H

//=============================
//		FormatScan.h
//=============================

#include <stdafx.h>
#pragma hdrstop

#define HAS_NO_FILENAMES	0
#define HAS_FILENAMES		1	

#define HAS_NO_AUDIO_INFO	0
#define HAS_AUDIO_INFO		1

#define HAS_NO_FOLDERS		0
#define HAS_FOLDERS			1

#define WAV_HEADER_NOT_REQ	0
#define WAV_HEADER_REQ		1

class FormatScan{

private:

	int mOffset;
	bool mXwb360;

public:

	bool Detect(FILE* in);
	bool Parse(FILE* in);


};
#endif
