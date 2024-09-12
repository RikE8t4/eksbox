


//=======================================================================
//	Lug.cpp - Fable - (Audio container which req. pertaining .met file
//						for offsets, sizes & filenames)
//=======================================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

bool Supported::LugRead(){


Supported f;

static struct
{
	uchar	fileName[100];
	uint	fileNameLen[4];

}lug;



FILE* in = 
fopen(ChangeFileExt(CItem.OpenFileName.c_str(), ".met").c_str(), "rb");

if (!in)
{
	gui.Error("Couldn't Find " + CItem.DirName + ".met", 1500, NULL);
	return false;
}

// Num of files
fseek(in, 4, 0);
fread(&CItem.Files, 4, 1, in);

// Offsets / sizes / filenames
f.pos = 12;
for (int x = 0; x < CItem.Files; ++x)
{

	fseek(in, f.pos, 0);
	fread(lug.fileNameLen, 4, 1, in);

	memset(lug.fileName, '\0', sizeof(lug.fileName));
	fread(lug.fileName, *lug.fileNameLen, 1, in);

	string tmpName = RemoveDrive((char*)lug.fileName);

	FItem.FileName.push_back(tmpName);
	FItem.Ext.push_back("." + ToLower(ExtractFileExt(tmpName)));

	f.pos += *lug.fileNameLen + 4;
	fseek(in, f.pos, 0);

	sys.PushChunk(in, 4);
	sys.PushOffset(in, 4);
	f.pos = ftell(in) + 20;
}

// Reset
fclose(in);
in = fopen(CItem.OpenFileName.c_str(), "rb");
if (!in)
{
	gui.Error("Couldn't Find " + CItem.DirName + ".lug", 1500, NULL);
	return false;
}

sys.RiffSpecs(in, false);

return true;
}


