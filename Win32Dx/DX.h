#include "stdafx.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8tex.h>
//#include "Common Files\d3dfont.h"
#include "DX/DxControls.h"

class DirectX{

private:
	LPDIRECT3D8 d3d;
	LPDIRECT3DDEVICE8 d3ddev;
	LPD3DXSPRITE d3dspt;
	LPDIRECT3DTEXTURE8 mTexture;
	LPDIRECT3DTEXTURE8* mMeshTexture;
	LPD3DXMESH mMesh;   
	D3DMATERIAL8* mMaterial;   
	DWORD mNumMaterials;
	LPDIRECT3DTEXTURE8 mBarTexture, mBGTexture;   
	LPD3DXFONT mFontHdr, mFontFPS, mFontList;   
	//CD3DFont *mFont;

	bool IsLMouseDown, IsRMouseDown, IsMouseMoving;
	float mCamX, mCamY, mCamZ;
	float mCursorX, mCursorY;
	float mPercent;
	int mProgressStep, mProgressMax, mProgressMin, 
		mCurValue, mFXPixel, mStep;
	HWND mHnd;

	DWORD now, old;     
	int FrameCount;       

	void CursorPosGet();

public:
	int mScreenWidth;
	int mScreenHeight;
	float mTriSize;
	float mRotSpeed;
	int mZoom;

	DirectX();
	void InitDX(HWND pHnd);
	void RenderFrame();
	void CleanUp();
	void InitGraphics();
	//void LoadMesh(const LPSTR& pFileName, DWORD& pNumMaterials, LPD3DXMESH& pMesh, 
	//	D3DMATERIAL9*&,  LPDIRECT3DTEXTURE9*& pTexture);
	//void LoadTexture(const LPSTR& pFileName, LPDIRECT3DTEXTURE9* pTexture);
	//void DrawTexture(const LPDIRECT3DTEXTURE9& pTexture, 
	//	RECT pTexcoords, 
	//	const int& pX, const int& pY, const int& pA);

	void DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, 
						   const int& pX,
						   const int& pY,					  
						   const int& pWidth,
						   const int& pHeight,
						   const int& pXImg);
	void DrawSprite(IDirect3DBaseTexture8* pTexture, 
		const int& pPosX, const int& pPosY, 
		const int& pSizeX, const int& pSizeY, 
		const bool& pTransparency);
	HRESULT LoadTexture(const LPSTR& pFileName, const D3DCOLOR& pColour, 
					LPDIRECT3DTEXTURE8* pTexture);

	void DrawFont(const LPCSTR& pText, RECT pCoords, 
		const DWORD& pFormat, const D3DCOLOR& Colour, const LPD3DXFONT& pFont);//CD3DFont& pFont);
	void LoadFont(const int& pSize, const int& pWeight, const bool& pItalic,
		const LPSTR& pFontName, LPD3DXFONT* pFont); // CD3DFont*& pFont);
	void DrawFPS();
	void DrawBar();
	void BarSetStepSize(const int& pValue);
	void BarStep();
	void BarSetPosition(const int& pValue);
	void IncRotSpeed() {mRotSpeed+=0.005f;}
	void DecRotSpeed() {mRotSpeed-=0.005f;}
	void IncZoom() {BarStep();}
	void DecZoom() {BarSetPosition(25);}
	void Up() { xc.List->SelectedDec(); }
	void Down() { xc.List->SelectedInc(); }
	void MouseLDownSet();
	void MouseLUpSet();
	void MouseRDownSet();
	void MouseRUpSet();
	void MouseMoveSet(const bool& pState) {IsMouseMoving = pState;}
	void ResetScene();
	void SetHwnd(HWND pHnd){mHnd = pHnd;}

};


