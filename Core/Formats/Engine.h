#ifndef ENGINE_H
#define ENGINE_H

//=============================
//		Engine.h
//=============================

#include <stdafx.h>
#pragma hdrstop

//==============================================================================

struct ContainerItem{

bool 	Finished, 			// Has the file finished being read
		Loading,  			// Is it still loading
		ScxMode, 			// Are we in SCX mode
		Extracting, 		// Is the current file having its contents extracted/converted right now
		IniMatch,       	// Filename database match
		HasNames,       	// Does the file contain names
		HasAudioInfo,   	// Does the file contain audio info
		HasFolders,     	// Does the file contain folders
		NeedsWavHeader, 	// Does the file need a wavheader to be made
		Error;          	// Has there been a read/extract error

int 	Files,          	// Total files contained
		IncompatibleFmts,	// Gets incremented if this file is incompatible
		PlayableFmts,		// Total playable files contained
		FileSize; 			// <

float	PercentExtracted;   // <

string	DirName,			// Directory name
		DirPath,            // Path to directory
		OpenFileName,       // Name of open file
		Format;             // Format of open file

};

//==============================================================================

struct FileItem{

// Parts only used for map format
vector<short>	Channels,
				Bits,
				Parts;

vector<USHORT>	SampleRate;

// OffsetXtra, ChunkXtra & Chunk1st only used for map format
vector<int>		Offset,
				Chunk,
				OffsetXtra,
				ChunkXtra,
				Chunk1st;

vector<string>	Format,
				FileName,
				FileNameBak,
				Ext;
};

//==============================================================================

extern ContainerItem CItem;
extern FileItem FItem;

//==============================================================================

class FormatUtil{

public:

	bool 	DetectEaAsf(const string& fileName, const int& offset);

	uint 	AsfSize(FILE* in, const uint& offset);
	void 	DdsSize(FILE* in, const int& x);
	void 	OggSize(FILE* in, const int& x);
	void 	PngSize(FILE* in, const int& x);
	int 	RiffHeaderSize(FILE* in, const uint& offset);
	uint 	RiffSize(FILE* in, const uint& offset);
	uint 	WmaSize(FILE* in, const uint& offset);
	void 	XwbSize(FILE* in, const int&x);

	void 	RiffSpecs(FILE* in, const bool& getSizes);
	void 	WmaSpecs(FILE* in, const bool& getSizes);

	void 	WritePcmHeader(
			const ulong& fileSize, const short& channels, const short& bits,
			const uint& sampleRate, FILE* out);

	#ifdef DEBUG
	void 	WriteMsadpcmHeader(
			const ulong& fileSize, const short& channels, const uint& sampleRate,
			FILE* out);
	#endif

	void 	WriteXadpcmHeader(
			const ulong& fileSize, const short& channels, const uint& sampleRate,
			FILE* out);


};

extern FormatUtil fmtUtil;

//==============================================================================

class FileX{

public:

	void 	PushFiles(const int& files);
	void 	PushFiles(FILE* in, const int& size);
	void 	PushOffset(FILE* in, const ushort& size);
	void 	PushOffset(const ulong& offset);
	void 	PushChunk(FILE* in, const ushort& size);
	void 	PushChunk(const ulong& chunk);
	void 	PushChunkDefault();
	void 	PushFileName(FILE* in, const ushort& size);
	void 	PushFileName(const string& name);
	void 	PushFileNameDefault();
	void 	PushExtension(const string& ext);
	void 	PushAudioFormat(const char* audioFmt);
	void 	PushBits(const short& bits);
	void 	PushChannel(FILE* in, const uint& size);
	void 	PushChannel(const short& channel);
	void 	PushSampleRate(FILE* in, const uint& size);
	void 	PushSampleRate(const ushort& sampleRate);
	void 	ClearVariables();

public:

	long pos, offset[4], chunk[4];
	ushort sr[4];
	uchar chan[2], bits[2];

	ushort firstOffset[2];
	uchar fmt[4];
	long magic[4];

	ushort sounds, chunksTot[2], files[4];  // Used for halo maps

};

extern FileX filex;

//==============================================================================

class Supported{

public:

	// Format Read
	void 	AdfRead(FILE* in);
	void 	AdxRead(FILE* in);
	bool 	AfsRead(FILE* in);
	void 	ArcRead(FILE* in);
	bool 	AsfMusRead(FILE* in);
	void 	BifRead(FILE* in);
	void 	BigCodeRead(FILE* in);
	bool 	BigEaRead(FILE* in);

	// Debug ver
	void 	BorRead(FILE* in);

	void	CabForzaRead(FILE* in);
	void 	DatRollerRead(FILE* in);
	void 	FsbRead(FILE* in);

	bool 	Gta5Read();
	void 	HotRead(FILE* in);
	void 	HwxRead();
	bool 	LugRead();
	void 	M3uRead();
	bool 	MapHaloRead(FILE* in);
	void 	MpkRead(FILE* in);
	void 	MsxRead(FILE* in);
	bool 	PakGoblinRead(FILE* in);

	// Debug ver
	void 	PakDreamRead(FILE* in);

	void 	PizRead(FILE* in);
	void 	PodRead(FILE* in);
	void 	RcfRead(FILE* in);
	void 	RezRead(FILE* in);
	void 	RfaRead(FILE* in);
	bool 	RwsRead(FILE* in);
	bool 	SampRead();
	void 	Sh4Read(FILE* in);
	void 	SrRead(FILE* in);
	void 	StxRead(FILE* in);
	bool 	WadRead();
	bool 	WavRead(FILE* in);

	// Debug ver
//	void 	WavRawRead(FILE* in);

	void 	WmapackRead(FILE* in);
	bool 	WmaRead(FILE* in);
	void 	WpxRead(FILE* in);
	bool 	XbpRead();
	bool 	XisoRead(FILE* in);
	bool 	XwbRead(FILE* in, unsigned long startPos, const bool& xwb360);
	bool 	XwcRead(FILE* in);
	void 	ZsmRead(FILE* in);

	bool 	MiscRead();

	// Extract / Convert
	bool 	EaExtract();
	bool 	MapExtract(int i, unsigned char* buffer, FILE* in, FILE* out);
	bool 	StdExtract(const int& pFileNum);

	// FileSwap
	bool 	StxSwap();
	bool 	RcfSwap();
	bool 	WpxSwap();
	bool 	XwbSwap();


};


//==============================================================================
#endif
