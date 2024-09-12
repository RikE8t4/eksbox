


//======================================================================
//	PakGoblin.cpp - Goblin Commander - (Simple container which req.
//					pertaining ..\\scripts\\Sonica.XB for offsets)
//======================================================================


#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"

//------------------------------------------------------------------------------

bool Supported::PakGoblinRead(FILE* in){


#define MAXPAKFILES 978
ushort PAK_ID;

static struct{
	ushort pakId[1];
	uint 	files;
	uint 	tableOffset[4];
	uchar 	fileName[40];
	uint 	fileNameLen[4];
	uint 	extLen[1];
}goblin;


goblin.files = 0;

if (ExtractFileName(ToLower(CItem.OpenFileName)) == "audxbox0.pak")
	PAK_ID = 0;
else
	PAK_ID = 1;


// Open 'Sonica.XB'
fclose(in);

string nam = CItem.OpenFileName;
int openNamLen = (int)nam.find_last_of("\\");
string sonica = nam;
sonica.resize(openNamLen);
sonica += "\\sonica.xb";

in = fopen(sonica.c_str(), "rb");


// If 'Sonica.XB' is not in same folder as *.pak file,
// then check ..\\scripts\\ dir for it
if (!in)
{
	openNamLen = (int)sonica.find_last_of("\\");
	sonica.resize(openNamLen);
	openNamLen = (int)sonica.find_last_of("\\");
	sonica.resize(openNamLen);
	sonica += "\\scripts\\sonica.xb";

	in = fopen(sonica.c_str(), "rb");

	if (!in){
		gui.Error("Couldn't Find sonica.xb", 1500, NULL);
		return false;
	}
}

fseek(in, 12, 0);
fread(goblin.tableOffset, 4, 1, in);

uint pos = *goblin.tableOffset + 20;

for (int x = 0; x < MAXPAKFILES; ++x)
{

	fseek(in, pos, 0);
	fread(goblin.fileNameLen, 4, 1, in);

	if (*goblin.fileNameLen > 80)
		break;

	memset(&goblin.fileName, 0x00, sizeof(goblin.fileName));
	fread(goblin.fileName, *goblin.fileNameLen, 1, in);

	*goblin.extLen = 0x00;
	fread(goblin.extLen, 1, 1, in);

	pos += *goblin.fileNameLen + *goblin.extLen + 27;

	*goblin.pakId = 0x00;

	fseek(in, pos, 0);
	fread(goblin.pakId, 1, 1, in);

	uint offset[4];
	fseek(in, ftell(in) + 3, 0);
	fread(offset, 4, 1, in);

	if (*goblin.pakId == PAK_ID)
	{
		FItem.Offset.push_back(*offset);
		FItem.FileName.push_back((char*)goblin.fileName);
		FItem.Ext.push_back(".wav");
		++goblin.files;
	}

	pos = ftell(in) + 2;

}


CItem.Files = goblin.files;


// Reset
fclose(in);
in = fopen(CItem.OpenFileName.c_str(), "rb");
if (!in)
{
	gui.Error("Couldn't Find " + CItem.DirName + ".pak", 1500, NULL);
	return false;
}

sys.RiffSpecs(in, true);

return true;
}


