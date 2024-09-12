


//==========================================================================
//	MapHalo.cpp - Halo / Stubbs - (Very complicated multi format container)
//==========================================================================

#include <stdafx.h>
#include "../Supported.h"
#include "../FileSpecs.h"
#include "../Gui.h"
#include "../SystemX.h"
#include "../Helper.h"
#include "../Zlib/CZlib.h"

//------------------------------------------------------------------------------

bool Supported::MapHaloRead(FILE* in){


Supported file;

static struct
{
	ushort	headerSize;
	int		decompSize[4];
	int		index[4];
	int		indexMagic[4];
	int		magic;
	uchar  tagClass[12];
	uchar 	path[100];
	uchar 	name[38];
	uchar 	nextname[38];
	//uchar 	name[100];
	//uchar 	nextname[100];
	ulong  nameOffset[4], metaOffset[4];
}map;


map.headerSize = 2048;

fseek(in, 8, 0);
fread(map.decompSize, 4, 1, in);
fseek(in, 16, 0);
fread(map.index, 4, 1, in);

// Decompress
if (ExtractFileExt(CItem.OpenFileName) == "map")
{
	FILE* out = 
	fopen(ChangeFileExt(CItem.OpenFileName.c_str(),".mapU").c_str(), "wb");

	if (!out){
		gui.Error("Error writing output file", sys.Delay(false), out);
		return false;
	}

	uchar headerBuf[2048];
	fseek(in, 0, 0);
	fread(headerBuf, 2048, 1, in);
	fwrite (headerBuf, 2048, 1, out);

	gui.Status("Decompressing, Wait..", 0);
	fseek(in, 2048, 0);
	CZlib zl; 
	zl.DecompStream(in, out);
	fclose(in); 
	fclose(out);
	CItem.OpenFileName = ChangeFileExt(CItem.OpenFileName, ".mapU");
	gui.Status(CItem.OpenFileName, 300);

	//Form1->OpenDialog->FileName = CItem.OpenFileName;
	in = fopen(CItem.OpenFileName.c_str(), "rb");
}

CItem.FileSize = sys.GetFileSize(in, true);

if (*map.decompSize != CItem.FileSize)
{
	gui.Error("Map file was not correctly read and/or decompressed", 1500, in);
	return false;
}

// Calc magic
fseek(in, *map.index, 0);
fread(map.indexMagic, 4, 1, in);
map.magic = *map.indexMagic - (*map.index + 40 - 4);

// Total no. of files
fseek(in, *map.index  + 12, 0);
fread(&CItem.Files, 4, 1, in);

file.sounds = 0;
file.pos = 100;

for (ushort x = 0; x < CItem.Files; ++x, file.pos += 32)
{
	fseek(in, *map.index + file.pos, 0);
	fread(map.tagClass, 12, 1, in);

	// Only parse sound files
	if (!strncmp((char*)map.tagClass, "!dns", 4))
	{
		fseek(in, *map.index + file.pos + 16, 0);
		fread(map.nameOffset, 4, 1, in);
		fread(map.metaOffset, 4, 1, in);
		*map.nameOffset -= map.magic;
		*map.metaOffset -= map.magic;

		// File path
		fseek(in, *map.nameOffset, 0);
		fgets((char*)map.path, 100, in);

//		String name = (char*)map.path;
//		name = name.Delete (1, name.Pos ("\\"));
//
//		for (int a; name.Pos("\\"); )
//		{
//			a=name.Pos ("\\");
//			name.Delete(a, 1);
//			name.Insert (" - ", a);
//		}
//		String prefix = name + " - ";
		//String prefix = name;
		//ShowMessage((char*)map.path);

		// SampleRate
		fseek (in, *map.metaOffset + 6, 0);
		fread(file.sr, 2, 1, in);
		if (!*file.sr) // 0 = 22050, 1 = 44100
			*file.sr = 22050;
		else
			*file.sr = 44100;

		// Channels
		fseek (in, *map.metaOffset + 108, 0);
		fread(file.chan, 2, 1, in);
		*file.chan += 1;

		// No. of files per meta
		fseek(in, *map.metaOffset + 208, 0);
		fread(file.files, 4, 1, in);

		// No. of chunks per meta (total)
		fseek(in, *map.metaOffset + 224, 0);
		fread(file.chunksTot, 2, 1, in);
		

		// Offsets, chunks, individual names & set sound info for each file
		uchar jumpSize[4];
		int accChunks = 0;

		fseek(in, *map.metaOffset + 236, 0);
		for (int i = 0; i < *file.files; ++i, ++file.sounds)
		{
			memset(map.name, 0x00, 38);
			fread(map.name, 38, 1, in);
			//FItem.FileName.push_back((char*)map.name);

			//gui.Status(string((char*)map.path)
			//+ "\\" + string((char*) map.name), 0);

			// JumpSize
			fseek(in, ftell(in) + 4, 0);
			fread(jumpSize, 4, 1, in);

			// First chunk size per file
			fseek(in, ftell(in) + 18, 0);
			fread(file.chunk, 4, 1, in);

			// Fix for incorrect chunks (Skips offending file)
			if (*file.chunk > 100000000 || *file.chunk <= 56)
			{
				//FItem.FileName.pop_back();
				--file.sounds;
				continue;
			}
			FItem.Chunk.push_back(*file.chunk);
			FItem.Chunk1st.push_back(*file.chunk);

			// Offsets
			fseek(in, ftell(in) + 4, 0);
			fread(file.offset, 4, 1, in);
			FItem.Offset.push_back(*file.offset);

			// Set audio specs
			FItem.Format.push_back("xadpcm");
			FItem.SampleRate.push_back(*file.sr);
			FItem.Channels.push_back(*file.chan);
			FItem.Bits.push_back(4);
			FItem.Ext.push_back(".wav");

			// Jump to correct sub pos
			if (*jumpSize != 255 && *file.files != *file.chunksTot)
			{
				fseek(in, (*map.metaOffset + 236) + (*jumpSize * 124), 0);

				// Loop for each file
				int parts = 0;
				memset(map.nextname, '\0', 38);
				memcpy(map.nextname, map.name, 38);
				for (;!strncmp((char*)map.nextname, (char*)map.name, 38) &&
					accChunks < *file.chunksTot - 1; ++parts, ++accChunks)
				{
					//gui.Status(IntToStr(i), 0);
					//gui.Status(IntToStr(parts), 0);
					//gui.Status((char*)map.nextname, 0);
					//gui.Status((char*)map.name, 0);

					// Read filename to see if it matches previous (Hack)
					fread(map.nextname, 38, 1, in);

					// Subchunks / offsets
					fseek(in, ftell(in) + 26, 0);
					fread(file.chunk, 4, 1, in);
					FItem.ChunkXtra.push_back(*file.chunk);
					FItem.Chunk[file.sounds] += *file.chunk;

					fseek(in, ftell(in) + 4, 0);
					fread(file.offset, 4, 1, in);
					FItem.OffsetXtra.push_back(*file.offset);

					/*
					// DEBUG LOG
					string outNam = sys.DirPathGet() + "\\mapdebug.log";
					FILE* log = fopen(outNam.c_str(), "ab");
					string offtxt = " " + IntToStr(*file.offset) + " ";
					string chunktxt = " " + IntToStr(*file.chunk) + " ";
					if (parts==0) fputs((char*)map.name, log);
					fputs(" Pos:",log);
					string postxt = IntToStr(ftell(in));
					fputs(postxt.c_str(), log);
					fputs(" OffsetXtra:", log);
					fputs(offtxt.c_str(), log);
					fputs(" ChunkXtra:", log);
					fputs(chunktxt.c_str(), log);
					fclose(log);
					// DEBUG LOG
					*/

					fseek(in, ftell(in) + 48, 0);
				}
			FItem.Parts.push_back(parts);
			}
			// If not jump to correct sub pos
			// Set no. of parts (This is for single chunk files)
			else
			{
				FItem.Parts.push_back(0);
			}

			// Jump backwards to first sub entry
			fseek(in, (*map.metaOffset + 236) + (i + 1) * (124), 0);

			FItem.FileName.push_back(
			string((char*)map.path) + "\\" + string((char*)map.name));

		}// End main inner loop


	}

}// End main loop


// Show info
//gui.SetInfoLbl(1, "Map Index: " + String(*map.index));
//gui.SetInfoLbl(2, "Map Index Magic: " + String(*map.indexMagic));
//gui.SetInfoLbl(3, "Map Magic: " + String(map.magic));
//gui.SetInfoLbl(4, "Total Files: " + String(CItem.Files));

CItem.Files = file.sounds;

return true;

}

