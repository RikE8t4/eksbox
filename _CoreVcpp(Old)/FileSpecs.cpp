


//=============================
//		FileSpecs.cpp
//=============================

#include "stdafx.h"
#include <vector>
//#include "FileSpecs.h"

struct ContainerItem{

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


struct FileItem{

std::vector<short>	Channels,
				Bits,
				Parts;

std::vector<ushort>	SampleRate;

// OffsetXtra, ChunkXtra & Chunk1st only used for map format
//vector<ulong>
std::vector<int>Offset,
				Chunk,
				OffsetXtra,
				ChunkXtra,
				Chunk1st;

std::vector<string>	Format,
				FileName,
				FileNameBak,
				Ext;
}FItem;












