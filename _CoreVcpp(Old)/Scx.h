#ifndef SCX_H
#define SCX_H

//=========================================================================
//	Scx.h - High Speed Brute Force Multi-Format File Scanner / Extractor
//=========================================================================

#include "stdafx.h" 

class Scx{

private:

//#define ID_WMA "\x30\x26\xB2\x75\x8E\x66\xCF\x11\xA6\xD9\x00\xAA\x00\x62\xCE\x6C"
//#define ID_XWB "\x57\x42\x4E\x44"

#pragma pack(1)
struct{
	char		id [4];
	uchar		revision; 	// Should be 0
	uchar		bitFlags; 	// 0x01: unset = fresh packet
							// 		   set = continued packet
							// 0x02: unset = not first page of logical bitstream
							//         set = first page of logical bitstream (bos)
							// 0x04: unset = not last page of logical bitstream
							//         set = last page of logical bitstream (eos)
	__int64  	absoluteGranulePos;
	int			streamSerialNumber;
	int			pageSeqNum;
	int			pageChecksum;
	uchar		numPageSegments;
}OggHdr;
#pragma pack(8)


public:

	void GetXwbSize(FILE* in, const int&x);
	void GetDdsSize(FILE* in, const int& x);
	void GetOggSize(FILE* in, const int& x);
	void GetPngSize(FILE* in, const int& x);
	int Init();
	int Scan(const bool& standAlone, const string& fileName);
	int SilentMode(string fileName);
	//vector<ulong>&offset, vector<ulong>&chunk

};


#endif
