#pragma once
#include "stdio.h"
#include "Open.h"
#include "Gui.h"
#include "Engine.h"
#include "Strings.h"

class ThreadABX

{

public:
	static DWORD WINAPI S_Run(LPVOID p){ return ((ThreadABX*)p)->Run(); }
	void SetText(const char* txt){strcpy(mTxt, txt);}

private:
	char mTxt[20];
	HWND mHnd;
	DWORD Run();
	void StartThread();

};

DWORD ThreadABX::Run(){
	if (CItem.Files) return -1;
	Open o;
	const string path = "c:\\XboxFormats\\";
	string file = str.WStrToStr(xc.List->GetSelectedName());
	int rtn = o.DoOpen(path + file);
			if (rtn == 1){
				//Msg("Opened");
				Supported sup; 
				rtn = sup.StdExtract(-1);
			}
			if (!rtn)
				gui.Msg("Extraction error");
			else
				gui.Msg("Extraction Complete");

	return rtn;
}

void ThreadABX::StartThread(){

DWORD id = 0;

HANDLE h = CreateThread(NULL, 0, S_Run, this, 0, &id);

if(h != NULL)
	CloseHandle(h);

}


void StartAbxThread(){

	ThreadABX t;
	auto DWORD id;
	HANDLE h = CreateThread(NULL, 0, ThreadABX::S_Run, (LPVOID)&t, 0, &id);
	if(h != NULL){
		//MessageBox(NULL, "Thread Finished", NULL, 0);
		CloseHandle(h);
	}

}