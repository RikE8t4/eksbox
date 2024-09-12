


//==========================================
//	Xiso.cpp - Multi - (Xbox Iso)
//==========================================

// Unfinished!!

#include <stdafx.h>
#pragma hdrstop

#include "Engine.h"
#include "Strings.h"
#include "Gui.h"

//==============================================================================

#define SECTOR 2048
#define PADDING 0xFF
//#define ATTRIB_READ_ONLY 	0x01
//#define ATTRIB_HIDDEN 		0x02
//#define ATTRIB_SYSTEM 		0x04
#define ATTRIB_DIR 			0x10
//#define ATTRIB_ARCHIVE 		0x20
//#define ATTRIB_NORM 		0x80

struct
{
	vector<u_int>offset;
	vector<u_int>tableSize;
}subDirEntry;

//vector<String>dirName;

struct
{
	long	rootDirTable[4];
	int		rootDirTableSize[4];
}xiso;

struct
{
	int 	leftTree[2];
	int 	rightTree[2];
	int		startSector[4];
	int		totSize[4];
	int 	attrib[1];
	int 	fileNameLen[1];
	u_char	fileName[40];
	int		padding[1];
}dirEntry;

//------------------------------------------------------------------------------

bool Supported::XisoRead(FILE* in){

//String bug =
//ExtractFileDir(CItem.OpenFileName) + ExtractFileName(CItem.OpenFileName) + ".txt";
//FILE* outDebug = fopen(bug.c_str(), "wt");

fseek(in, 32 * SECTOR + 20, SEEK_SET);    
fread(xiso.rootDirTable, 4, 1, in);           
fread(xiso.rootDirTableSize, 4, 1, in);        

u_int pos = *xiso.rootDirTable *= SECTOR;          
u_int tableLen = pos + *xiso.rootDirTableSize;

int files = 0;
int dirs = 0;

// Parse root directory entry
bool ParseEntry(FILE* in, u_int pos, u_int tableLen, int& files, int& dirs);

if (!ParseEntry(in, pos, tableLen, files, dirs)) return false;

// Parse all sub directory entries
if(subDirEntry.offset.size())
{
	for (int x = 0; dirs ; ++x, --dirs) // && x <= 0xFF
	{
		pos = subDirEntry.offset[x];
		tableLen = subDirEntry.offset[x] + subDirEntry.tableSize[x];

		if (!ParseEntry(in, pos, tableLen, files, dirs)) return false;


//		// Debug
//		String bug = ExtractFileDir(OpenFileName)+ExtractFileName(OpenFileName)+".txt";
//		FILE* outDebug = fopen(bug.c_str(), "at");
//		fwrite("\nParsed TableEntry", 18, 1, outDebug);
//		String p = "\nPos Now: " + String(ftell(in));
//		int l = p.Length();
//		fwrite(p.c_str(), l, 1, outDebug);
//		fclose(outDebug);
//		// Debug
	}
}
	 								 
subDirEntry.offset.clear();
subDirEntry.tableSize.clear();
CItem.Files = files;


return true;
}


//==============================================================================

// ParseEntry
bool ParseEntry(FILE* in, u_int pos, u_int tableLen, int& files, int& dirs){

while (pos < tableLen) //&& pos <= tableLen + 0xFF
{
memset(&dirEntry, 0x00, sizeof(dirEntry));
fseek(in, pos, 0);

fread(dirEntry.leftTree, 2, 1, in);
fread(dirEntry.rightTree, 2, 1, in);
fread(dirEntry.startSector, 4, 1, in);
fread(dirEntry.totSize, 4, 1, in);
fread(dirEntry.attrib, 1, 1, in);
fread(dirEntry.fileNameLen, 1, 1, in);
fread(dirEntry.fileName, *dirEntry.fileNameLen, 1, in);



//	// Debugging
//	String bug = ExtractFileDir(OpenFileName)+ExtractFileName(OpenFileName)+".txt";
//	FILE* outDebug = fopen(bug.c_str(), "at");
//	String x1 = "\n\nFile Name:\t"  + String((char*)dirEntry.fileName);
//	String x2 = "\nStart Sector:\t"  + String(*dirEntry.startSector*SECTOR);
//	String x3 = "\nTable Length:\t"  + String(tableLen);
//	String x4 = "\nFiles:\t"  + String(files);
//	String x5 = "\nDirectories:\t"  + String(dirs);
//
//	String x6 = "\nLeftTree:\t"  + String(*dirEntry.leftTree);
//	String x7 = "\nRightTree:\t"  + String(*dirEntry.rightTree);
//	String x8 = "\nTotal Size:\t"  + String(*dirEntry.totSize);
//	String x9 = "\nAttrib:\t"  + String(*dirEntry.attrib);
//	String x10 = "\nFile Name Len:\t"  + String(*dirEntry.fileNameLen);
//	String x11 = "\nPosition:\t"  + String(pos);
//
//	fwrite(x1.c_str(), x1.Length(), 1, outDebug);
//	fwrite(x2.c_str(), x2.Length(), 1, outDebug);
//	fwrite(x3.c_str(), x3.Length(), 1, outDebug);
//	fwrite(x4.c_str(), x4.Length(), 1, outDebug);
//	fwrite(x5.c_str(), x5.Length(), 1, outDebug);
//	fwrite(x6.c_str(), x6.Length(), 1, outDebug);
//	fwrite(x7.c_str(), x7.Length(), 1, outDebug);
//	fwrite(x8.c_str(), x8.Length(), 1, outDebug);
//	fwrite(x9.c_str(), x9.Length(), 1, outDebug);
//	fwrite(x10.c_str(), x10.Length(), 1, outDebug);
//	fwrite(x11.c_str(), x11.Length(), 1, outDebug);
//	fclose(outDebug);
//	// Debugging


//if (*dirEntry.attrib == ATTRIB_DIR && *dirEntry.startSector)
//{
//
//dirName.push_back((char*)dirEntry.fileName);
//
//}


// Valid file
if (*dirEntry.attrib != ATTRIB_DIR && *dirEntry.totSize && *dirEntry.startSector)
{
	FItem.Offset.push_back(*dirEntry.startSector * SECTOR);
	FItem.Chunk.push_back(*dirEntry.totSize);

	FItem.FileName.push_back((char*)dirEntry.fileName);
	FItem.Format.push_back("");
	FItem.Ext.push_back(str.ExtractFileExt(FItem.FileName[files]));

	++files;

//	outDebug = fopen(bug.c_str(), "at");
//	fwrite("\nValidFile", 10, 1, outDebug);
//	fclose(outDebug);
}

// Valid directory
else if (*dirEntry.attrib == ATTRIB_DIR && *dirEntry.totSize
		&& *dirEntry.startSector)
{
	subDirEntry.offset.push_back(*dirEntry.startSector * SECTOR);
	subDirEntry.tableSize.push_back(*dirEntry.totSize);
	++dirs;

	//String n = (char*)dirEntry.fileName;

	//++dirs;
	//if (*dirEntry.leftTree || *dirEntry.rightTree)
	//	++dirs;


//	outDebug = fopen(bug.c_str(), "at");
//	fwrite("\nValidDir", 9, 1, outDebug);
//	fclose(outDebug);
}
//else if (!*dirEntry.startSector)
//{

//}

else if (!*dirEntry.startSector && (!*dirEntry.totSize && files)){
//	//outDebug = fopen(bug.c_str(), "at");
//	//fwrite("\nERROR", 6, 1, outDebug);
//	//fclose(outDebug);
	//subDirEntry.offset.push_back(*dirEntry.startSector * SECTOR);
	//subDirEntry.tableSize.push_back(*dirEntry.totSize);
	//++dirs;
	gui.Msg("Error");
	//gui.Msg(pos);
	return false;
}



*dirEntry.padding = PADDING;
//int x = 0;

while (*dirEntry.padding == PADDING) // && x < 20
{
	fread(dirEntry.padding, 1, 1, in);
	//++x;
}


pos = ftell(in)-1;


//if (dirEntry.rightTree != 0)
//{
//	u_char buf[32];
//	fread(buf, 32, 1, in);
//	//if (!memcmp(buf, "\ff\ff\ff\ff\ff\ff", 6))
//	//{
//		//gui.Msg("Tree");
//		//gui.MsgA("help", "cap");
//		//pos = ftell(in)-1;
//	//	return;
//	//}
//}

	

}

return true;

}// End parseEntry

//==============================================================================

















