


//==========================================
//	Wma.cpp - Multi - ()
//==========================================



#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "Scx.h"

//==============================================================================

bool Supported::WmaRead(FILE* in){

int size = fmtUtil.WmaSize(in, 0);

if (size != CItem.FileSize) // Scanfile
{
	fclose(in);

	Scx scx;
	if (scx.SilentMode(CItem.OpenFileName) != 0)
		return false;

	in = fopen(CItem.OpenFileName.c_str(), "rb");

	for (int x = 0; x < CItem.Files; ++x)
		FItem.Format.push_back("wma");
}
else// Single wma- Don't scan
{
	CItem.Files = 1;
	FItem.Offset.push_back(0);
	FItem.Chunk.push_back(size);
	FItem.FileName.push_back(CItem.DirName);
	FItem.Ext.push_back(".wma");
}

fmtUtil.WmaSpecs(in, false);
return true;

}

//==============================================================================



