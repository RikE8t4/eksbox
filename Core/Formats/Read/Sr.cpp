


//=========================================================
//	Sr.cpp - Multi  (Inane multi-format container) 
//=========================================================



#include <stdafx.h>
#include "Engine.h"
#include "Gui.h"
#include "Strings.h"

//==============================================================================

void Supported::SrRead(FILE* in){

// Num of files
uchar files[3];
fseek(in, 0, 0);
fgets((char*)files, 3, in);

string f = str.CharTrimToStr((char*)files);
CItem.Files = str.StrToInt(f);

// Block Size
uchar bs [4];
fseek(in, (int)f.length()+1, 0);
fgets((char*)bs, 4, in);
//int blockSize = str.StrToInt(str.CharTrimToStr((char*)bs));

// Header size
uchar hdr [7];
//gui.Status("ftellIn: " + str.IntToStr(ftell(in)), 0, 0);
//fseek(in, ftell(in), 0); 
fgets((char*)hdr, 7, in);
uint headerSize = str.StrToInt(str.CharTrimToStr((char*)hdr));

// Block start position
uchar dot = '\0';
int x = 0;
fseek(in, 8, 0);
while (dot != 0x0A && x < 32)
{
	fread(&dot, 1, 1, in);
	++x;
}

int blockStart = 8 + x-1;


// Read remaining specs
uchar offset[10];
uchar chunk[10];
uchar chan[5];
uchar samplerate[8];
uchar bit[3];
uchar filename[41];

for (int x = 0, pos = -80; x < CItem.Files; ++x, pos -= blockStart)
{
	pos += blockStart + 80; 

	fseek(in, pos + 1, 0);
	fgets((char*)offset, 10, in);        

	fseek(in, pos + 11, 0);
	fgets((char*)chunk, 10, in);			

	fseek(in, pos + 23, 0);
	fgets((char*)chan, 5, in);	

	fseek(in, pos + 28, 0);
	fgets((char*)samplerate, 8, in); 	

	fseek(in, pos + 36, 0);
	fgets((char*)bit, 3, in);       	

	fseek(in, pos + 39, 0);
	fgets((char*)filename, 28, in);

	FItem.Offset.push_back(str.StrToInt(str.CharTrimToStr((char*)offset)) + headerSize);
	FItem.Chunk.push_back(str.StrToInt(str.CharTrimToStr((char*)chunk)));

	int iChan = str.StrToInt(str.CharTrimToStr((char*)chan));

	if (iChan > 2 || iChan == 0)
		FItem.Channels.push_back(NULL);
	else
		FItem.Channels.push_back(iChan);

	FItem.SampleRate.push_back(str.StrToInt(str.CharTrimToStr((char*)samplerate)));
	FItem.Bits.push_back(str.StrToInt(str.CharTrimToStr((char*)bit)));

	string fn = str.CharTrimToStr((char*)filename);
	FItem.FileName.push_back(fn);
	FItem.Ext.push_back("." + str.ToLower(str.ExtractFileExt(fn)));

	if (FItem.Ext[x] == ".wav")
		FItem.Format.push_back("pcm");
	else
		FItem.Format.push_back("");

} // End for loop

}

//==============================================================================
