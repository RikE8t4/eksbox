


//======================================================================
//	CabForza.cpp - Forza - (Container format containing zlib'd files)
//======================================================================



#include <stdafx.h>
#include "Engine.h"
#include "Strings.h"

//==============================================================================

void Supported::CabForzaRead(FILE* in){

static struct{
	char sig[8];
	int files;
	int version;
	int compSize;
	int decompSize;
	int filenameIndexSize;
}cab;

static struct{
	int offset;
	int compSize;
	int decompSize;
	int filenameOffset;
	int null;
	int unknown;
}cabEntry;

fseek(in, 0, 0);
fread(&cab, sizeof(cab), 1, in);
CItem.Files = cab.files;

int pos = 306;
int fileNameIndex = (cab.files * 24) + (306); // Filename index offset

for (int i = 0; i < CItem.Files; ++i, pos += 24)
{
	fseek(in, pos, 0);
	fread(&cabEntry, sizeof(cabEntry), 1, in);

	FItem.Offset.push_back(cabEntry.offset);
	FItem.Chunk.push_back(cabEntry.compSize);

	char eachname[120];
	fseek(in, fileNameIndex + cabEntry.filenameOffset, 0);
	fread(eachname, sizeof(eachname), 1, in);

	filex.PushFileName((char*)eachname);
	filex.PushExtension("." + str.ExtractFileExt((char*)eachname));

	FItem.Format.push_back(FItem.Ext[i]);
	FItem.Channels.push_back(0);
	FItem.SampleRate.push_back(0);
	FItem.Bits.push_back(0);
}

}

//==============================================================================


