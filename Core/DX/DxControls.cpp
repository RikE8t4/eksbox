

//=============================
//		DxControls.cpp
//=============================

#include <stdafx.h>
#ifdef _XBOX
#include "../XBOX/XBOXABX.h"
#endif
//#include <d3d8.h>
//#include <d3dx8.h>
//#include <d3dx8tex.h>
//#include <algorithm> // sort
//#include "Gui.h"
#include "Strings.h"
#include "DxControls.h"

#ifdef _WINDOWS
	DxControls xc;
#endif


//====================================================================================

//CControl::CControl(LPDIRECT3DDEVICE8 pD3dDevice){
//
//SetD3DDevice(pD3dDevice);
//
//}

//====================================================================================

void CD3DHelper::LoadTexture(const LPSTR& pFileName, LPDIRECT3DTEXTURE8* pTexture){

D3DXCreateTextureFromFileEx(this->GetD3DDevice(), 
							pFileName,
							D3DX_DEFAULT,
							D3DX_DEFAULT,
							D3DX_DEFAULT,
							NULL,
							D3DFMT_A8R8G8B8,
							D3DPOOL_MANAGED,
							D3DX_DEFAULT,
							D3DX_DEFAULT,
							clPink,
							NULL,
							NULL,
							pTexture);

}

//====================================================================================

HRESULT CD3DHelper::Initialize(){

if (SUCCEEDED(D3DXCreateSprite(this->GetD3DDevice(), &mD3dspt)))
	return S_OK;

return S_FALSE;

}


 //====================================================================================

void CD3DHelper::DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, 
			RECT pTexcoords, 
			const int& pX, const int& pY, const int& pA){
	
//D3DXVECTOR2 center(0.0f, 0.0f), position((float)pX, (float)pY),
//scaling(0.0f, 0.0f), rotation(0.0f, 0.0f);
D3DXVECTOR2 scaling(1, 1), rcenter(0, 0), trans((float)pX, (float)pY);
//mD3dspt->Begin();

//mD3dspt->Draw(pTexture, &pTexcoords, &scaling, &rcenter, 
//			  0, &trans, XARGB(pA, 255, 255, 255));

//mD3dspt->End();

}


//====================================================================================

void CProgressBar::Init(){

this->LoadTexture("d:\\Media\\Progress.png", &mTexture);

}

//====================================================================================

void CProgressBar::Draw(){

SetStepSize(0.5);
mMax = 100;
mMin = 0;

float width = 299;

// Percent complete
float Range    = mMax - mMin;
float Progress = mCurValue - mMin;
if(Range == 0)
	mPercent = 100;
else
	mPercent = float((Progress / Range) * 100.0);

//mFXPixel = __int64(((width * mPercent) / 100.0));
mFXPixel = float(((width * mPercent) / 100.0));


// Bar back
RECT part = {0, 0, 1, 16}; // left, top, right, bottom,

for(int i = 0; i <= width; ++i)
this->DrawTexture(mTexture, part, i, 0, 255);

// Bar front
for(int i = 0, p = 1; i <= (mFXPixel); ++i, ++p)
{
	if (p == 16) p = 1;
	SetRect(&part, 1, 0, 2, 16);
	this->DrawTexture(mTexture, part, i, 0, 255);
}


}

//====================================================================================

void CProgressBar::SetStepSize(const float& pValue){

mStep = pValue;

}

//====================================================================================

void CProgressBar::Step(){

if((mCurValue + mStep) < mMin)
	mCurValue = mMin;
else if((mCurValue + mStep) <= mMax)
	mCurValue += mStep;
else
	mCurValue = mMax;

}

//====================================================================================

void CProgressBar::SetPosition(const float& pValue){

if(pValue < mMin)
	mCurValue = mMin;
else if(pValue > mMax)
	mCurValue = mMax;

mCurValue = pValue;

}


//====================================================================================

CList::CList(LPDIRECT3DDEVICE8& pD3dDevice){
	
//SetD3DDevice(pD3dDevice);
#ifdef _XBOX
mFont		= NULL;
#endif
mWidth		= 85;
mSpacing	= 20;
mX			= 80;
mY			= 210;
mColour		= clWhite;
mColourSel	= clYellow;
mMaxFilesShow = 8;
mBlink = false;

}


//====================================================================================
#ifdef _WINDOWS
void CList::Init(const LPD3DXFONT& pFont, const int& pMaxFilesShow, const string& pDirPath){
#endif
#ifdef _XBOX
void CList::Init(CXBFont& pFont, const int& pMaxFilesShow, const string& pDirPath){
#endif

if (pMaxFilesShow == -1)
	mMaxFilesShow = 10;
else
	mMaxFilesShow = pMaxFilesShow;

#ifdef _WINDOWS
mFont = pFont;
#endif
#ifdef _XBOX
mFont = &pFont;
#endif
mIndex = 0;
mFirstItem = 0;
SetDirectory(pDirPath);

FileCount();
Load();

}

//====================================================================================

void CList::Config(const int& pX, const int& pY, const int& pWidth, 
				   const int& pSpacing, const DWORD& pColour, const DWORD& pColourSel){

//if (pMaxFilesShow != mMaxFilesShow && pMaxFilesShow > 0)
//{
//	mMaxFilesShow = pMaxFilesShow;
//	//mList = new wstring[mMaxFilesShow];
//}

if (pX != -1)
	mX	= pX;
if (pY != -1)
	mY = pY;
if (pWidth != -1)
	mWidth = pWidth;
if (pSpacing != -1)
	mSpacing = pSpacing;

mColour	= pColour;
mColourSel = pColourSel;

}

//====================================================================================

void CList::SetDirectory(const string& pPath){

if (pPath.size() <= 0) {
	#ifdef _WINDOWS
	mDirPath = "c:\\XboxFormats\\*"; 
	#endif
	#ifdef _XBOX
	mDirPath = "d:\\XboxFormats\\*"; 
	#endif
}
else
	mDirPath = pPath;

Load();

}

//====================================================================================

void CList::SortList(const bool& sortByType){

//mIndex = mFirstItem = 0;
int siz = (int)mList.size();

if (sortByType)
	for (int x = 0, y = 1; x < siz; ++x, y = 0)
	{
		while (y < siz)
		{
			if (str.ToUpper(str.ExtractFileExt(mList[x])) < str.ToUpper(str.ExtractFileExt((mList[y]))))
				mList[y].swap(mList[x]);
			++y;
		}
	}

else
	for (int x = 0, y = 1; x < siz; ++x, y = 0)
	{
		while (y < siz)
		{
			if (toupper(mList[x][0]) < toupper(mList[y][0]))
				mList[y].swap(mList[x]);
			++y;
		}
	}

}

//====================================================================================

void CList::Load(){

WIN32_FIND_DATA	fd;
HANDLE find = FindFirstFile(string(mDirPath + "\\*").c_str(), &fd);
mList.clear();

if(INVALID_HANDLE_VALUE == find)
	return;
else
{
	do{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			mList.push_back(str.CharToWStr(fd.cFileName));
		}
	}
	while (FindNextFile(find, &fd));
}

FindClose(find); 
SortList(false);

}

//====================================================================================

void CList::FileCount(){

WIN32_FIND_DATA	fd;
HANDLE find = FindFirstFile(string(mDirPath + "\\*").c_str(), &fd);
mFiles = 0;

if(INVALID_HANDLE_VALUE == find)
	return;
else
{
	do{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			++mFiles;
	} while (FindNextFile(find, &fd));
}

FindClose(find); 

}

//====================================================================================

void CList::SelectedInc(){

if (mIndex < mFiles - 1){
	++mIndex;
	ScrollDown();
}

}

//====================================================================================

void CList::SelectedDec(){

if (mIndex > 0){
	--mIndex;
	ScrollUp();
}

}

//====================================================================================

string CList::GetSelectedPathAndName() { 
	
return mDirPath + "\\" + str.WStrToStr(GetSelectedName()); 

}

//====================================================================================

wstring CList::GetSelectedName(){

if (mIndex <= mFiles)
	return mList[mIndex];

return L'';

}

//====================================================================================

void CList::ScrollDown(){

if (mIndex >= mMaxFilesShow && mFirstItem < mFiles - mMaxFilesShow)
	++mFirstItem;

}

//====================================================================================

void CList::ScrollUp(){

if (mIndex < mFirstItem)
	--mFirstItem;

}

//====================================================================================

wstring CList::GetItem(const int& pIndex){

if (pIndex <= mFiles)
	return mList[pIndex];

return L'';

}

//====================================================================================

void CList::Draw(){

#ifdef _WINDOWS
//-------------

RECT r = {mX, mY, 100, 100};

for (int i = 0; i < mMaxFilesShow; ++i){
	SetRect(&r, mX, mY + (i * mSpacing), mWidth, 800);
	if (GetSelectedIndex() == i)
		mFont->DrawTextA(str.WStrToStr(GetItem(i)).c_str(), (int)GetItem(i).length(),
		&r, DT_LEFT, mColourSel);
	else
		mFont->DrawTextA(str.WStrToStr(GetItem(i)).c_str(), (int)GetItem(i).length(), 
		&r, DT_LEFT, mColour);
}

#endif
//----

#ifdef _XBOX
//-------------


float x = (float)mX, y = (float)mY;
//static float xx = 0;
//static bool forward = true;
//if (forward)
//xx += .9;
//else
//xx -= .9;
//if (xx > 8 || xx < -8)
//forward = !forward;
static D3DCOLOR colour = XARGB(0, 100, 100, 100);

if (mBlink)
{
	if (colour < 0x44444444)
		colour = XRGB(50, 50, 50);

	colour += XRGB(20, 20, 20);
	mColourSel = mColourSel | colour * 50;
}

for (int i = mFirstItem; i < mMaxFilesShow + mFirstItem; ++i)
	if (GetSelectedIndex() != i){
		mFont->SetScaleFactors(1, 1); 
		mFont->DrawText(x, y + ((i - mFirstItem) * mSpacing), mColour, GetItem(i).c_str());
	}
	else{
		mFont->SetScaleFactors(1.1f, 1.1f);
		mFont->DrawText(x, y + ((i - mFirstItem) * mSpacing), mColourSel, GetItem(i).c_str());
	}

#endif
//----

}

//====================================================================================