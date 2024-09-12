
#include <fstream>

//=============================
//		ConvertStx.cpp
//=============================

// All code/format cracking/reversing by Kernel Master
//-----------------------------------------------------

#define ushort unsigned short
#define uchar unsigned char

//------------------------------------------------------------------------------

struct{
	uchar 	id[4];
	ushort	hdrSize;
	ushort	channels;
	ushort	unknown1;
	ushort	unknown2;
	ushort	unknown3;
	ushort	null1;
	ushort	pages;
	ushort	curPage;
	ushort	null2;
	ushort	interleaving;	// Also related to loop/file start & end pos
	ushort	nextPage;
	ushort	loopStart;
	ushort	loopEnd;
	ushort	null3;
} stx = {
	'S', 'T', 'H', 'D',		// ID
	32,						// HdrSize
	2,   					// Channels
	0,						// Unknown1
	0,						// Unknown2
	0,						// Unknown3
	0,						// Null1
	1024,					// Pages
	0,						// CurPage
	0,						// Null2
	1008,					// Interleaving
	1,          			// NextPage
	0,						// LoopStart
	0,						// LoopEnd
	0,						// Null3
};

//------------------------------------------------------------------------------

inline float ProgressUpdate(const int& bytesWritten, const int&bytesTotal){

	return 100 / (float)(bytesTotal / bytesWritten); 

}
	
//------------------------------------------------------------------------------

inline void StxLoopFix(FILE* in, FILE* out, const int& offset, const int& bufSize,
							uchar* bufL, uchar* bufR)
{

short dataInPage;
fseek(in, offset + 22, 0);
fread(&dataInPage, sizeof(short), 1, in);

fseek(in, offset + 32, 0);
fread(bufL, dataInPage, 1, in);
fseek(in, offset + 32 + bufSize, 0);
fread(bufR, dataInPage, 1, in);

uchar* bufLR = (uchar*) malloc(dataInPage << 1);

int i = 0, j = 0;
while(i < dataInPage)
{
	// Left Channel
	bufLR[j] = bufL[i];
	bufLR[j+1] = bufL[i+1];
	bufLR[j+2] = bufL[i+2];
	bufLR[j+3] = bufL[i+3];
	// Right Channel
	bufLR[j+4] = bufR[i];
	bufLR[j+5] = bufR[i+1];
	bufLR[j+6] = bufR[i+2];
	bufLR[j+7] = bufR[i+3];
	i += 4, j += 8;
}

fwrite(bufLR, dataInPage << 1, 1, out);
free(bufLR);

}// End StxLoopFix


//------------------------------------------------------------------------------

void StxToXbadpcm (FILE* in, FILE* out, const int& channels,
								int offset, const bool& surround,
								const bool& progressEnable, float& progress)
{

if (!channels) return;

if (offset < 2048)
	offset = 2048;

// Setup progress
if (progressEnable)
	progress = 0;

int bytesWritten = 0;
			  
int bufSize;

if (channels == 1)						// Mono
	bufSize = 2016;
else if (channels == 2 && !surround)	// Stereo
	bufSize = 1008;
else if (surround)						// Surround
	bufSize = 504;


uchar* bufL = (uchar*) malloc(bufSize);
uchar* bufR;

if (channels != 1)
	bufR = (uchar*) malloc(bufSize);


// Fill struct
fseek(in, 0, 0);
fread(&stx, sizeof(stx), 1, in);

int fileSize = stx.pages * 2048;
bool looped = stx.loopStart != 0xFFFF;
int loopStart = stx.loopStart * 2048 - 2048;
int loopEnd = stx.loopEnd * 2048 - 2048;

// Mono
if (channels == 1)
	while (offset <= fileSize)
	{
		fseek(in, offset + 32, 0);
		fread(bufL, bufSize, 1, in);
		fwrite(bufL, bufSize, 1, out);
		offset += 2048;
		if (progressEnable)
			progress = ProgressUpdate(bytesWritten += bufSize, fileSize);
	}
// Stereo
else if (channels == 2 && !surround)
	while (offset <= fileSize)
	{
		// Looping
		if (looped)
		if (offset == loopStart || offset == loopEnd)
		{
			StxLoopFix(in, out, offset, bufSize, bufL, bufR);
			offset += 2048;
			continue;
		}

		fseek(in, offset + 32, 0);
		fread(bufL, bufSize, 1, in);
		fread(bufR, bufSize, 1, in);

		uchar bufLR[2016];

		int i = 0, j = 0;
		while(i < 1008)
		{
			// Left Channel
			bufLR[j] = bufL[i];
			bufLR[j+1] = bufL[i+1];
			bufLR[j+2] = bufL[i+2];
			bufLR[j+3] = bufL[i+3];
			//Right Channel
			bufLR[j+4] = bufR[i];
			bufLR[j+5] = bufR[i+1];
			bufLR[j+6] = bufR[i+2];
			bufLR[j+7] = bufR[i+3];
			i += 4, j += 8;
		}
		fwrite(bufLR, 2016, 1, out);
		offset += 2048;
		if (progressEnable)
			progress = ProgressUpdate(bytesWritten += bufSize, fileSize);
	}
// Surround
else if (surround)
	while (offset <= fileSize)
	{
		// Looping
		if (looped)
		if (offset == loopStart || offset == loopEnd)
		{
			StxLoopFix(in, out, offset, bufSize, bufL, bufR);
			offset += 2048;
			continue;
		}

		fseek(in, offset + 32, 0);
		fread(bufL, bufSize, 1, in);
		fread(bufR, bufSize, 1, in);

		uchar bufLR[1008];

		int i = 0, j = 0;
		while(i < 504)
		{
			// Left Channel
			bufLR[j] = bufL[i];
			bufLR[j+1] = bufL[i+1];
			bufLR[j+2] = bufL[i+2];
			bufLR[j+3] = bufL[i+3];
			//Right Channel
			bufLR[j+4] = bufR[i];
			bufLR[j+5] = bufR[i+1];
			bufLR[j+6] = bufR[i+2];
			bufLR[j+7] = bufR[i+3];
			i += 4, j += 8;
		}
		fwrite(bufLR, 1008, 1, out);
		offset += 2048;
		if (progressEnable)
			progress = ProgressUpdate(bytesWritten += bufSize, fileSize);
	}

free(bufL);

if (channels != 1)
	free(bufR);

}// End StxToXbadpcm


//------------------------------------------------------------------------------


void XbadpcmToStx (FILE* in, FILE* out, const int& channels,
							const int& sampleRate,
							const int& wavHdrSize, const bool& looping)
{

// Note: No surround sound support

int bufSize;

if (channels == 1)		// Mono
	bufSize = 2016;
else if (channels == 2)	// Stereo
	bufSize = 1008;

uchar* bufL = (uchar*) malloc(bufSize);
uchar* bufR;

if (channels != 1)
	bufR = (uchar*) malloc(bufSize);


stx.id[0] = 'S'; stx.id[1] = 'T'; stx.id[2] = 'H'; stx.id[3] = 'D';
stx.hdrSize = 32;
stx.channels = channels;
stx.unknown1 = 0;
stx.unknown2 = 0;
stx.unknown3 = 0;
stx.null1 = 0;
stx.pages = 1024;
stx.curPage = 0;
stx.null2 = 0;
stx.interleaving = bufSize;
stx.nextPage = 1;
stx.loopStart = 0;
stx.loopEnd = 0;


if (!looping){
	stx.loopStart 	= 0xFFFF;
	stx.loopEnd 	= 0xFFFF;
}

fseek(in, 0L, SEEK_END);
int inSizeRound = ftell(in) - wavHdrSize; // Raw input xbadpcm audio size


// Round input wav size up
while ( (inSizeRound % 2016) != 0)
	++inSizeRound;


int pageCalc = inSizeRound / 2016;
int dataEnd = inSizeRound % 2016; // Wav data size in last page
int offset = wavHdrSize; // Sets us up to seek to end of wav header

int x = 0;

// Mono
if (channels == 1)
	while (offset < inSizeRound - 2016)
	{
		stx.curPage = x + 1;
		stx.nextPage = x + 2;
		++x;

		fwrite(&stx, sizeof(stx), 1, out);

		if (x >= pageCalc - 1)
		{
			memset(bufL, 0x00, bufSize);
			offset += 2016;
		}

		fseek(in, offset, 0);
		fread(bufL, bufSize, 1, in);
		fwrite(bufL, bufSize, 1, out);
		offset += 2016;
	}
// Stereo
else if (channels == 2)
	while (offset < inSizeRound - 2016)
	{
		fseek(in, offset, 0);

		uchar bufLR[2016];

		if (x >= pageCalc - 1)
		{
			memset(bufL, 0x00, bufSize);
			memset(bufR, 0x00, bufSize);
			memset(bufLR, 0x00, bufSize);
			offset += 2016;
		}

		fread(bufLR, 2016, 1, in);

		int i = 0, j = 0;
		while(i < 1008)
		{
			// Left Channel
			bufL[i]		= bufLR[j];     
			bufL[i+1]	= bufLR[j+1];
			bufL[i+2]	= bufLR[j+2];
			bufL[i+3]	= bufLR[j+3];
			//Right Channel
			bufR[i]		= bufLR[j+4];
			bufR[i+1]	= bufLR[j+5];
			bufR[i+2]	= bufLR[j+6];
			bufR[i+3]	= bufLR[j+7];
			i += 4, j += 8;
		}

		stx.curPage = x + 1;
		stx.nextPage = x + 2;
		++x;

		fwrite(&stx, sizeof(stx), 1, out);
		fwrite(bufL, 1008, 1, out);
		fwrite(bufR, 1008, 1, out);
		offset += 2016;
	}


if (channels != 1)
	free(bufR);

free(bufL);

// Loop Info
fseek(out, 0L, SEEK_END);
int outSize = ftell(out);

stx.pages = (ushort)outSize / 2048;
if (looping)
{
	stx.loopStart = 1;
	stx.loopEnd = stx.pages -1;
}

//fseek(out, 26, 0);
//fwrite(stx.loopStart, 2, 1, out);
//fwrite(stx.loopEnd, 2, 1, out);

for (int x = 0; x < outSize; x += 2048)
{
	//fseek(out, x, 0);
	//stx.curPage = y + 1;
	//stx.nextPage = y + 2;
	//fwrite(&stx, sizeof(stx), 1, out);
	fseek(out, x+16, 0);
	fwrite(&stx.pages, 2, 1, out);
	fseek(out, x+26, 0);
	fwrite(&stx.loopStart, 2, 1, out);
	fwrite(&stx.loopEnd, 2, 1, out);

	if (x >= outSize - 2048)
	{
		if (dataEnd)
			stx.interleaving = dataEnd;
		fseek(out, x+22, 0);
		fwrite(&stx.interleaving, 2, 1, out);
	}

}


// Write sampleRates
for (int x = 32; x <= 480; x += 64)
{
	fseek(out, x, 0);
	fwrite(&sampleRate, 2, 1, out);
}

// Write channels
fseek(out, 6, 0);
fwrite(&stx.channels, 2, 1, out);


// Write panning value for mono files
if (channels == 1 && looping)
{
	uchar monoPanVal[10] = {
	0x64, 0x6E, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x00,
	};

	fseek(out, 36, 0);
	fwrite(monoPanVal, 10, 1, out);
}


}// End XbadpcmToStx






//------------------------------------------

// Old Stereo Algo
//		int i = 0;
//		while(i < 504)
//		{
//			fwrite(&bufL[i], 4, 1, out);
//			fwrite(&bufR[i], 4, 1, out);
//			i += 4;
//		}
//
//		offset += 2048;

//------------------------------------------

// Old Surround Algo
//		int i = 0;
//		while(i < 1008)
//		{
//			fwrite(&bufL[i], 4, 1, out);
//			fwrite(&bufR[i], 4, 1, out);
//			i += 4;
//		}
//
//		offset += 2048;



