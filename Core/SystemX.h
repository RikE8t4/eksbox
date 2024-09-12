#ifndef SYSTEMX_H
#define SYSTEMX_H

//=============================
//		SystemX.h
//=============================

#include <stdafx.h>
#pragma hdrstop

class SystemX{

private:

	unsigned char* 	mBuf;
	double 			mTime;

	bool  			mIsDirSet;
	vector<string>	mSupportedFormats;

public:

	void SetSupportedFormats(const string& fmts);
	bool GetSupportedFormat(const string& fmtExt);
	uchar* MemoryAlloc(const ulong& size, FILE* stream);
	void MemoryDel();

	string GetAppDir();
	void ExtractError(const string& fileName, const string& dirPath);
	void TimerStart();
	double TimerStop();
	int Delay(const bool& fast);
	long GetAvailPhyMem();
	long GetAvailVirMem();
	void ProcessMessages();
	bool FileExists(const string& name);
	bool FileIsReadOnly(const string& name);
	bool DirectoryExists(const std::string& dirName);
	bool CreateDir(const std::string& dirName);
	bool RemoveDir(const std::string& dirName);
	void BuildDirStruct(const char* path);
	int GetWinVer();
	void ExecuteAndWait(const std::string& path);
	int GetOpenFileNameExtLen();
	void DirNameSet();
	void DirPathSet(const string& dir);
	void DirPathSet();
	void DirPathSetSilent();
	string DirPathGet();
	bool DirPathIsSet();
	void DirBrowse();
	void DeleteDir(const string& path);
	string GetTempDir();
	void GetAllFiles(const string& path, bool recursive);
	ulong GetFileSize(FILE* in, const bool& updateSizeLbl);
	int HeaderScanFast(const ulong& startPos, const ulong& fileSize,
						const uchar* sig, FILE* in);
	int HeaderScan (const ulong& startPos, const ulong& fileSize,
						const char* sig, const ushort& sigSize, FILE* in);

	SystemX();
	~SystemX();


};// End System

extern SystemX sys; // Global instance

#endif
