#ifndef DX_CONTROLS_H
#define DX_CONTROLS_H

//=============================
//		DxControls.h
//=============================

#include "stdafx.h"

#ifdef _WINDOWS
	#include <d3d8.h>
	#include <d3dx8tex.h>
#endif
#ifdef _XBOX
	#include "../XBOX/XBOXABX.h"
#endif
		
//====================================================================================

class CControl {

private:
	LPDIRECT3DDEVICE8* mD3dDevice;	
public:
	//CControl(LPDIRECT3DDEVICE8& pD3dDevice){SetD3DDevice(pD3dDevice);}
	void Init(LPDIRECT3DDEVICE8& pD3dDevice){SetD3DDevice(pD3dDevice);}
	LPDIRECT3DDEVICE8 GetD3DDevice() const {return *mD3dDevice;}
	void SetD3DDevice(LPDIRECT3DDEVICE8& pD3dDevice) {mD3dDevice = &pD3dDevice;}

};

//====================================================================================

class CD3DHelper : public CControl{

	private:
		LPD3DXSPRITE mD3dspt;
		HRESULT Initialize();

	public:
		void LoadTexture(const LPSTR& pFileName, LPDIRECT3DTEXTURE8* pTexture);
		void DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, 
			RECT pTexcoords, 
			const int& pX, const int& pY, const int& pA);

};

//====================================================================================

class CProgressBar : public CD3DHelper{

	private:
		LPDIRECT3DTEXTURE8 mTexture;
		IDirect3DTexture8* TexScreen;
		int mProgressPos;
		float mStep, mMax, mMin, mCurValue, mFXPixel, mPercent;
	
	public:
		CProgressBar(LPDIRECT3DDEVICE8& pD3dDevice){SetD3DDevice(pD3dDevice);}
		void Init();
		void Draw();
		void SetStepSize(const float& pValue);
		void Step();
		void SetPosition(const float& pValue);
};

//====================================================================================

class CList{

	public:
		//CList(LPDIRECT3DDEVICE8 pD3dDevice){SetD3DDevice(pD3dDevice);}
		CList(LPDIRECT3DDEVICE8& pD3dDevice);
		~CList() { mList.clear(); }

		// -1 for pMaxFilesShow param will cause default value to be used
		#ifdef _WINDOWS
		void		Init(const LPD3DXFONT& pFont, const int& pMaxFilesShow, const string& pDirPath);
		#endif
		#ifdef _XBOX
		void		Init(CXBFont& pFont, const int& pMaxFilesShow, const string& pDirPath);
		#endif

		void		Config(const int& pX, const int& pY, 
			const int& pWidth, const int& pSpacing, const DWORD& pColour, const DWORD& pColourSel);
		void		SetDirectory(const string& pPath);
		string		GetDirectory() { return mDirPath; }
		void		SetBlink(const bool& pValue) { mBlink = pValue; }
		void		SelectedInc();
		void		SelectedDec();
		string		GetSelectedPathAndName();
		wstring		GetSelectedName();
		int			GetSelectedIndex(){return mIndex;}
		void		Draw();
		wstring		GetItem(const int& pIndex);

		void	SortListByName() { SortList(false); }
		void	SortListByType() { SortList(true); }
	

	private:
		#ifdef _WINDOWS
		LPD3DXFONT			mFont;
		#endif
		#ifdef _XBOX
		CXBFont*			mFont;
		#endif
		int					mMaxFilesShow;
		int					mFiles;
		int					mIndex;
		int					mFirstItem;
		int					mWidth, mSpacing;
		int					mX, mY;
		bool				mBlink;
		DWORD				mColour, mColourSel;
		vector<wstring>		mList;
		string				mDirPath;

		void	FileCount();
		void	ScrollDown();
		void	ScrollUp();
		void	SortList(const bool& sortByType);
		void	Load();


};

//====================================================================================

class DxControls{

	public:
		CProgressBar* ProgressBar;
		CList* List;

};

//====================================================================================

#ifdef _WINDOWS
	extern DxControls xc;
#endif

#endif