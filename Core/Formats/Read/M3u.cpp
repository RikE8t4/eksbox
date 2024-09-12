


//===========================================================
//	M3u.cpp - Multi - (Playlist format)
//===========================================================



#include <stdafx.h>
#pragma hdrstop
#include "Engine.h"
#include "Gui.h"
#include "Strings.h"
#include "SystemX.h"
#ifdef _MEDIAPLAYER
#include "MediaPlayer.h"
#endif

#define BUFSIZE 512

//==============================================================================

void Supported::M3uRead(){

FILE* in = fopen (CItem.OpenFileName.c_str(), "rt");

uchar buf[BUFSIZE];
string dirPath = str.ExtractFileDir(CItem.OpenFileName) + "\\";
fseek(in, 0, 0);

while (!feof(in))
{
	memset(buf, 0x00, BUFSIZE);
	fgets((char*)buf, BUFSIZE, in);

	if (!memcmp(buf, "http:", 5))
		gui.Error("Web Streams Are Not Supported", sys.Delay(true), 0);

	else if (buf[0] != '#')
	{
		string path;

		if (sys.FileExists((char*)&buf))
			path = (char*)&buf;
		else{
			path = dirPath + string((char*)&buf);
			path.resize(path.length()-1);
		}

		if (sys.FileExists(path))
		{
			FILE* fil = fopen(path.c_str(), "rb");
			if ( (CItem.FileSize = sys.GetFileSize(fil, false)) <= 0)
				gui.Error("Error: File <= 0KB", sys.Delay(true), in);
			#ifdef _MEDIAPLAYER
			else if (MediaPlayer.IsValidFormat(path))
			{
				MediaPlayer.PlayListAddEntry(path.c_str());
				++CItem.Files;
			}
			#endif
			fclose(fil);
		}
		else{
			//++CItem.IncompatileFmts;
			//gui.Error("M3U Entry Error", sys.Delay(true), 0);
		}
		//ShowMessage(path);
	}
}

}

//==============================================================================

/*  Reference

 M3U
 Classical\Other Band - New Song.mp3
 crap.mp3
 F:\more music\foo - bar.mp3

 Comedy\Weird Al - Everything You Know Is Wrong.mp3
 Weird Al - This Is The Life.mp3
 http://www.site.com/~user/gump.mp3

 PLS
[playlist]
File1=Alternative\everclear - SMFTA.mp3
Title1=Everclear - So Much For The Afterglow
Length1=233
File2=Comedy\Weird Al - Everything You Know Is Wrong.mp3
Title2=Weird Al - Everything You Know Is Wrong
Length2=227
File3=Weird Al - This Is The Life.mp3
Title3=Weird Al Yankovic - This is the Life
Length3=187
File4=http://www.site.com/~user/gump.mp3
Title4=Weird Al: Bad Hair Day - Gump
Length4=129
File5=http://www.site.com:8000/listen.pls
Title5=My Cool Stream
Length5=-1
NumberOfEntries=5
Version=2

*/
