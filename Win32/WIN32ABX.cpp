// ABX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Open.h"
#include "Engine.h"
#include "Gui.h"

extern "C"
{
	extern char ** __argv;
	extern int __argc;
}

//=============================================================================

void Welcome(){

cout<<"\n--------------------------------------------";
cout<<"\n EkszBox-ABX - Win32 - Command Line Version";
cout<<"\n--------------------------------------------\n\n";

}

//=============================================================================

void ParseArgs(){

//cout<<"ParseParams";
//system("pause");

for (int i = 1; i < __argc; ++i){

	if (memcmp("-file", __argv[i], 5)){
		cout<<__argv[i];
		system("pause");
	}


}

//	if (file == "" || path == ""){
//	gui.Error("File or path name empty", 0, 0);
//	return 0;
//}
//
//rtn = op.DoOpen(path + file);

}

//=============================================================================

//int main(int argc, char *argv[])
int _tmain(int argc, _TCHAR* argv[])
{

	Welcome();

	if(0)
	if (argc < 2){
		cout<<"-Error: No arguments specified\n\n";
		system("pause");
		return 1;
	}

	Open op;
	bool rtn = false;
	string s, file;

	if (argc > 1 && argv[1] != "")
		s = argv[1];

	gui.Status("Opening", 0, 0);

	if (s != "")
		rtn = op.DoOpen(s);
	
	//else
	//{
		//ParseArgs();
		//const string path = "c:\\XboxFormats\\";
		//const string path = "c:\\XboxFormats\\PCXWB\\";

		//file = "EMOTION.ADF";
		//file = "adx0.afs";
		//file = "soundeve.arc";
		//file = "sounds.bif";
		//file = "FEMusic2.big";
		//file = "music.big";
		//file = "car_tvr_cerbera.cab"; // ForzaCab - Fix zlib memory leak
		//file = "SchemaMetafile_HardDrive.csc"; // SCX format
		//file = "Cars.rar"; // SCX Test - Png Newly Working
		//file = "DdsBulk.rar"; // - DDs Working
		//file = "css1.dat";
		//file = "streams_music_common_audio.fsb";
		//file = "co.gta5"; // SCX format - Ogg Newly Working
		//file = "sounds.hot";
		//file = "mus_ui_mainmenu_lp.hwx";
		//file = "River.lug";
		//file = "ui.map";
		//file = "ui.mapU";
		//file = "music.mpk";
		//file = "shell_music.msx";
		//file = "AudXbox0.pak";
		//file = "Font36.piz"; // PIZ - Newly Working
		//file = "XBXMUSND.POD";
		//file = "COMMON.POD";
		//file = "music.rcf";
		//file = "Resource.rez";
		//file = "Audio.rfa"; // RFA - Newly Working
		//file = "music (menu).rws";
		//file = "Namco50.samp";
		//file = "gt.sh4";
		//file = "bionicc.sr"; // SR - Newly Working
		//file = "BS_DAIMA.SR";
		//file = "BS_MAKAI.SR";
		//file = "mat2.sr";
		//file = "mat2.sr";
		//file = "mk1.sr";
		//file = "mk2.sr";
		//file = "mk3.sr";
		//file = "namco50.sr";
		//file = "topsecrt.sr";
		//file = "UI.SR";
		//file = "xbox_cap1.sr";

		//file = "sectionz.sr";
		//file = "Reiji_Intro.stx";
		//file = "SND_END1.wpx";
		//file = "Sounds1.xwb"; // PC-XWB
		//file = "StreamingWaveBank.xwb"; // PC-XWB
		//file = "Wave Bank.xwb"; // PC-XWB
		//file = "Streaming Wave Bank.xwb"; // PC-XWB
		//file = "droid.xwb"; // PC-XWB
		//file = "xWaveBank.xwb"; // PC-XWB
		//file = "_EATrax0.xwb";
		//file = "D_ThornsPass.xwb";
		//file = "WB_14FANG2.xwb";
		//file = "crowd (mediumclub).xwb";
		//file = "ENGLISHC_HAM_1A.PAK";
		//file = "959.zwb";
		//file = "speech.vxb";
		//file = "ENGINE9.SXB";
		//file = "GUI.XWC";
		//file = "col_m.zsm";
		//file = "wma.wad";
		//file = "UIMusic.wmapack";
		//if (file == "" || path == ""){
		//	gui.Error("File or path name empty", 0, 0);
		//	return 0;
		//}
		
		//rtn = op.DoOpen(path + file);
	//}
	if (rtn == true){
		gui.Status("Opened", 0, 0);
		Supported sup; 
		rtn = sup.StdExtract(-1);
		if (!rtn)
			gui.Status("Extraction error", 0, 0);
		//Supported sup; sup.StdExtract(1);
	}

	return 0;
}

//=============================================================================

