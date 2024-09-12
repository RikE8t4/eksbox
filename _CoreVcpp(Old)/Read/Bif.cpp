


//===========================================================
//	Bif.cpp - Star Wars: KOTOR - (Simple container)
//===========================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
//#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

void Supported::BifRead(FILE* in){

Supported f;

fseek(in, 8, 0);
fread(&CItem.Files, 4, 1, in);

f.pos = 24;

for (int x = 0; x < CItem.Files; ++x, f.pos += 16)
{
	fseek(in, f.pos, 0);
	sys.PushOffset(in, 4);
	sys.PushChunk(in, 4);

	// Check format
	uchar fmt[4];
	memset(fmt, '\0', 4);
	fseek(in, FItem.Offset[x], 0);
	fread(fmt, 4, 1, in);

	if (fmt[0] == 'R' && fmt[1] == 'I' && fmt[2] == 'F' && fmt[3] == 'F')
		sys.PushExtension(".wav");
	else
		sys.PushExtension(".dat");
}

sys.RiffSpecs(in, false);

}


