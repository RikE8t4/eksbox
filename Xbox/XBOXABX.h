#ifndef XBOX_ABX_H
#define XBOX_ABX_H

#include "stdafx.h"
#include <XBFont.h>
//#include <XGraphics.h>
//#include <XBUtil.h>
////#include <XBHelp.h>
#include <xbdm.h>
#include <XBInput.h>

#include "DxControls.h"

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
#define XBAPPERR_MEDIANOTFOUND       0x82000003

//-----------------------------------------------------------------------------

class CXboxAbx
{
	protected:
    // Main objects used for creating and rendering the 3D scene
	D3DPRESENT_PARAMETERS	mD3dPParam;		// The D3D rendering device
	LPDIRECT3DDEVICE8		mD3dDevice;		// The D3D rendering device
    LPDIRECT3D8				mD3DEnum;		// The D3D enumerator object
    LPDIRECT3DSURFACE8		mBackBuffer;	// The back buffer
	LPDIRECT3DSURFACE8		mFrontBuffer;
    LPDIRECT3DSURFACE8		mDepthBuffer;	// The depth buffer

    // Variables for timing
    FLOAT      mTime;				// Current absolute time in seconds
    FLOAT      mElapsedTime;		// Elapsed absolute time since last frame
    FLOAT      mAppTime;			// Current app time in seconds
    FLOAT      mElapsedAppTime;		// Elapsed app time since last frame
    FLOAT      mFPS;				// instantaneous frame rate
    WCHAR      mStrFrameRate[20];	// Frame rate written to a string
    HANDLE     mFrameCounter;		// Handle to frame rate perf counter

    // Members to init the XINPUT devices.
    XDEVICE_PREALLOC_TYPE* mInputDeviceTypes;
    DWORD                  mNumInputDeviceTypes;
    XBGAMEPAD*             mGamepad;
    XBGAMEPAD              mDefaultGamepad;

	LARGE_INTEGER qwTime, qwLastTime, qwElapsedTime;
	LARGE_INTEGER qwAppTime, qwElapsedAppTime;
	FLOAT mSecsPerTick;

    // Helper functions
    HRESULT RenderGradientBackground( DWORD dwTopColor, DWORD dwBottomColor );

    HRESULT Initialize();
    HRESULT FrameMove();
    HRESULT Render();

    CXBFont           m_Font;

    DM_COUNTDATA      m_dmCounterData;   // Counter data
    DM_COUNTINFO      m_dmCounterInfo;   // Info on current counter
    PDM_WALK_COUNTERS m_pdmWalkCounters; // For walking counters
    HANDLE            m_hCounter;        // Handle to current counter
    HRESULT           m_hrQuery;         // HRESULT of query

	wstring	mStatusMemory;
	//char	m_file_select[MAX_PATH];
	wstring	mStatusHdrText;
	wstring	mStatusText;
	CXBPackedResource  mXprResource;      // General app packed resource

private:
		int mScreenWidth, mScreenHeight;
		DxControls mDXC;
public:

	HRESULT LoadTexture(const LPSTR& pFileName, const D3DCOLOR& pColour, 
		LPDIRECT3DTEXTURE8* pTexture);
	void DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, const int& pX,
		const int& pY, const int& pWidth, 
		const int& pHeight, const int& pXImg);
	void DrawSprite(IDirect3DBaseTexture8* pTexture, 
		const int& pPosX, const int& pPosY, const int& pSizeX, const int& pSizeY, 
		const bool& pTransparency);
	LPD3DXSPRITE mSprite;
	LPDIRECT3DTEXTURE8 mBGTexture;
	LPDIRECT3DTEXTURE8 mIconTexture;
	LPDIRECT3DTEXTURE8 mBarTexture;

    // Functions to create, run, and clean up the application
	void UpdateFrameRate();
    HRESULT Create();
    INT     Run();
    VOID    Destroy();
    CXboxAbx(); // Constructor

	void DrawMemoryTxt();
	//void DrawFileTxt(string txt);
	void DrawHdrTxt(string txt);
	void DrawStatusTxt(string txt);

};

extern CXboxAbx xbApp;

#endif