


//===========================================================
//	Xbp.cpp - Multi - (Seemingly obfuscated container)
//===========================================================

// Unfinished!!

#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "Scx.h"

//==============================================================================

bool Supported::XbpRead(){

Scx scx;
if (scx.SilentMode(CItem.OpenFileName) == -1)
{
	return false;
}

FILE* in = fopen(CItem.OpenFileName.c_str(), "rb");

for (int x = 0; x < CItem.Files; ++x)
{
	if (FItem.Ext[x] == ".wma")
		FItem.Format.push_back("wma");
	else
		FItem.Format.push_back("");
}

fmtUtil.WmaSpecs(in, false);

fclose(in);
return true;

}

//==============================================================================
