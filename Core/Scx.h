#ifndef SCX_H
#define SCX_H

//=========================================================================
//	Scx.h - High Speed Brute Force Multi-Format File Scanner / Extractor
//=========================================================================

#include <stdafx.h>
#pragma hdrstop

//==============================================================================

class Scx{

private:

public:

	int Init();
	int Scan(const bool& standAlone);
	int SilentMode(const string& fileName);

};

//==============================================================================

#endif
