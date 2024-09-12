#ifndef FILE_SPECS_H
#define FILE_SPECS_H

//=============================
//		FileSpecs.h
//=============================

#include "stdafx.h"
//#include "FileSpecs.h"

extern struct ContainerItem{

bool 		Finished,
			Loading,
			ScxMode,
			Extracting,
			IniMatch,
			HasNames,
			HasAudioInfo,
			HasFolders,
			NeedsWavHeader,
			Error;

int 		Files, IncompatileFmts, PlayableFmts;

//ulong		FileSize;
int			FileSize;

float		PercentExtracted;

string 		DirName,
			DirPath,
			OpenFileName,
			Format;
}CItem;


extern struct FileItem{

// Parts only used for map format
vector<short>	Channels,
				Bits,
				Parts;

vector<ushort>	SampleRate;

// OffsetXtra, ChunkXtra & Chunk1st only used for map format
//vector<ulong>
vector<int>		Offset,
				Chunk,
				OffsetXtra,
				ChunkXtra,
				Chunk1st;

vector<string>	Format,
				FileName,
				FileNameBak,
				Ext;
}FItem;


#endif
