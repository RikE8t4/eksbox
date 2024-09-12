#ifndef SYSTEMX_H
#define SYSTEMX_H

//=============================
//		SystemX.h
//=============================

#include "stdafx.h"


class SystemX{

protected:
	 vector<string>SupportedFormats;

private:

	unsigned char* Buf;
	double Time;
	MEMORYSTATUS Memory;

	unsigned int SampleRate[4], Channels[4];
	unsigned long Offset[4], Chunk[4];
	char Name[100];
	bool  IsDirSet;
	

public:

	//int GetFiles();

	//void SetFiles(int files);

	//Ansistring GetExt(int index);

	//vector<Ansistring> GetExtVector();

	//void SetExt(Ansistring ext);

	//string GetOpenFileName();

	//void SetOpenFileName(string fileName);

	void SetSupportedFormats(string fmts);
	bool GetSupportedFormat(string fmtExt);
	uchar* MemoryAlloc(const ulong& size, FILE* stream);
	void MemoryDel();

	void ExtractError(string fileName, string dirPath);
	void TimerStart();
	double TimerStop();
	int Delay(const bool& fast);
	long GetAvailPhyMem();
	long GetAvailVirMem();
	bool FileExists(string name);
	bool FileIsReadOnly(string name);
	int GetWinVer();
	void ExecuteAndWait(string path);
	void BuildDirStruct(const char* path);
	int GetOpenFileNameExtLen();
	void DirNameSet();
	void DirPathSet(string path);
	void DirPathSet();
	void DirPathSetSilent(string path);
	string DirPathGet();
	bool DirPathIsSet();
	void DirBrowse();
	void DeleteDir(string path);
	string GetTempDir();
	void GetAllFiles(string path, bool recursive);
	ulong GetFileSize(FILE* in, const bool& updateSizeLbl);
	int HeaderScanFast(const int& startPos, const int& fileSize,
						const uchar* sig, FILE* in);
	int HeaderScan (const ulong& startPos, const ulong& fileSize,
						const char* sig, const ushort& sigSize, FILE* in);
	void IniDump(string& homeDir);

	void WritePcmHeader
	(
		const ulong& fileSize, const short& channels, const short& bits,
		const uint& sampleRate, FILE* out
	);
	#ifdef DEBUG
	void WriteMsadpcmHeader
	(
		const ulong& fileSize, const short& channels, const uint& sampleRate,
		FILE* out
	);
	#endif
	void WriteXadpcmHeader
	(
		const ulong& fileSize, const short& channels, const uint& sampleRate,
		FILE* out
	);

	void PushFiles(const int& files);
	void PushFiles(FILE* in, const int& size);
	void PushOffset(FILE* in, const ushort& size);
	void PushOffset(const ulong& offset);
	void PushChunk(FILE* in, const ushort& size);
	void PushChunk(const ulong& chunk);
	void PushFileName(FILE* in, const ushort& size);
	void PushFileName(string name);
	void PushExtension(string ext);
	void PushAudioFormat(const char* audioFmt);
	void PushBits(const short& bits);
	void PushChannel(FILE* in, const uint& size);
	void PushChannel(const short& channel);
	void PushSampleRate(FILE* in, const uint& size);
	void PushSampleRate(const ushort& sampleRate);
	void ClearVariables();

	int RiffHeaderSize(FILE* in, const uint& offset);
	uint RiffSize(FILE* in, const uint& offset);
	uint WmaSize(FILE* in, const uint& offset);
	uint AsfSize(FILE* in, const uint& offset);
	void RiffSpecs(FILE* in, const bool& getSizes);
	void WmaSpecs(FILE* in, const bool& getSizes);

	SystemX();
	~SystemX();


};// End System

extern SystemX sys; // Global instance

#endif
