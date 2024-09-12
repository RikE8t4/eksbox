#include "stdafx.h"
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include "DX.h"
#include "Engine.h"

#pragma comment(lib, "d3d8.lib")
//#ifdef _DEBUG
//#pragma comment(lib, "d3dx8d.lib")
//#else
#pragma comment(lib, "d3dx8.lib")
//#endif

#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#define FVF_SPRITE (D3DFVF_XYZRHW | D3DFVF_TEX1)

//=======================================================================

DirectX::DirectX(){

mTexture = NULL;
mBGTexture = NULL;
mBarTexture = NULL;
mFontHdr = NULL;
mScreenWidth = 640;
mScreenHeight = 480;
mTriSize = 1.0;
ResetScene();
now = old = FrameCount = 0;       

}

void DirectX::ResetScene(){

mRotSpeed = 0.45f;
mZoom = 28;
mCamX = 1.0;
mCamY = 1.0;
mCamZ = 1.0;

mProgressMax = 100;
mProgressMin = 0;

}

//=======================================================================

void DirectX::InitDX(HWND pHnd){

d3d = Direct3DCreate8(D3D_SDK_VERSION);
D3DPRESENT_PARAMETERS d3dpp;
ZeroMemory(&d3dpp, sizeof(d3dpp));
//d3dpp.Windowed = false;
d3dpp.Windowed = true;
d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
d3dpp.hDeviceWindow = pHnd;
d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
d3dpp.BackBufferWidth = mScreenWidth;
d3dpp.BackBufferHeight = mScreenHeight;
d3dpp.EnableAutoDepthStencil = false;
d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
d3d->CreateDevice(D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
					pHnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp,
					&d3ddev);
InitGraphics();

d3ddev->SetRenderState(D3DRS_LIGHTING, false); 
d3ddev->SetRenderState(D3DRS_AMBIENT, XRGB(50, 50, 50));
//d3ddev->SetRenderState(D3DRS_AMBIENT, XRGB(50, 50, 50));
//d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
//d3ddev->SetRenderState(D3DRS_ZENABLE, true); // turn on Z buffer
//d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
//d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
D3DXCreateSprite(d3ddev, &d3dspt);

}

//=======================================================================

void DirectX::RenderFrame(){

d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, clBlack, 1.0, 0);
d3ddev->BeginScene();

DrawSprite(mBGTexture, 0, 0, 640, 480, false);

d3dspt->Begin(); // D3DXSPRITE_ALPHABLEND
	DrawBar();
d3dspt->End();

DrawFPS();
xc.List->Draw();

static RECT rt; SetRect(&rt, 15, 15, 640, 480); 
string s = "WIN32DX-ABX";
//mFont->DrawTextA(s.c_str(), (int)s.length(), &textbox, 
//DT_LEFT | DT_WORDBREAK, clRed);
DrawFont(s.c_str(), rt, DT_LEFT, clRed, mFontHdr);
//mFont->DrawText(2, 30, clRed, (TCHAR*)StrToWStr(s).c_str(), NULL);

d3ddev->EndScene();
d3ddev->Present(NULL, NULL, NULL, NULL);

}

//=======================================================================

void DirectX::CleanUp(){

XRELEASE(mBGTexture);
XRELEASE(mBarTexture);
XRELEASE(mFontHdr);
XRELEASE(mFontFPS);
XRELEASE(mFontList);
XRELEASE(d3ddev);
XRELEASE(d3d);

}

//=======================================================================

void DirectX::InitGraphics(){

LoadFont(45, 12000, true, "Arial", &mFontHdr);
LoadFont(17, 3000, false, "Arial", &mFontList);
LoadFont(17, 3000, false, "Arial", &mFontFPS);

xc.List = new CList(d3ddev);
xc.List->Init(mFontList, 15, "");
xc.List->Config(35, 90, 800, 22, clWhite, clYellow);
LoadTexture("Media\\BG.png", clBlack, &mBGTexture);
LoadTexture("Media\\Progress.png", clBlack, &mBarTexture);

}

//=======================================================================

//void DirectX::LoadTexture(const LPSTR& pFileName, LPDIRECT3DTEXTURE9* pTexture){
//
//D3DXCreateTextureFromFileEx(d3ddev, 
//							pFileName,
//							D3DX_DEFAULT,
//							D3DX_DEFAULT,
//							D3DX_DEFAULT,
//							NULL,
//							D3DFMT_A8R8G8B8,
//							D3DPOOL_MANAGED,
//							D3DX_DEFAULT,
//							D3DX_DEFAULT,
//							clPink,
//							NULL,
//							NULL,
//							pTexture);
//}

//=======================================================================

//void DirectX::DrawTexture(const LPDIRECT3DTEXTURE9& pTexture, 
//						  RECT pTexcoords, 
//						  const int& pX, const int& pY, const int& pA)
//{
//    D3DXVECTOR3 center(0.0f, 0.0f, 0.0f), position(pX, pY, 0.0f);
//    d3dspt->Draw(pTexture, &pTexcoords, &center, &position, 
//	D3DCOLOR_ARGB(pA, 255, 255, 255));
//
//}

//=======================================================================

void DirectX::DrawFont(const LPCSTR& pText, RECT pCoords, 
					   const DWORD& pFormat, const D3DCOLOR& Colour, 
					   const LPD3DXFONT& pFont){
					   //CD3DFont& pFont){

pFont->DrawTextA(pText,
                  (int)strlen(pText),
                  &pCoords,
                  pFormat,
                  Colour);

//pFont.DrawText(pX, pY, 
//				Colour, (TCHAR*)pText, NULL);

}

//=======================================================================
void DirectX::LoadFont(const int& pSize, const int& pWeight, const bool& pItalic,
					   const LPSTR& pFontName,
						LPD3DXFONT* pFont){
						//CD3DFont*& pFont){

//pFont = new CD3DFont(pFontName,
//					pSize,   
//					D3DFONT_BOLD); // D3DFONT_ITALIC || D3DFONT_BOLD
//pFont->InitDeviceObjects(d3ddev);
//pFont->RestoreDeviceObjects();


HFONT hFont = 
CreateFont(pSize, 0, 0, 0, pWeight, pItalic, false, false, 
		   0, // SHIFTJIS_CHARSET
		   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		   ANTIALIASED_QUALITY, 
		   FIXED_PITCH | FF_SCRIPT, 
		   pFontName);
D3DXCreateFont(d3ddev, hFont, pFont);

}

//=======================================================================

void DirectX::DrawFPS(){

char fps[32];

if(FrameCount <= 0)
{
    old = now;
    now = timeGetTime();
    FrameCount = 10;
}

--FrameCount;

sprintf(fps, "FPS : %5.2f", 10.0 / (double)(now - old) * 1000.0);
RECT r = {mScreenWidth - 100, 10, 40, 40};
DrawFont(fps, r, DT_LEFT, clWhite, mFontFPS);
//DrawFont(FloatToStr(CItem.PercentExtracted).c_str(), r, DT_LEFT, clWhite, mFontFPS);

}

//=======================================================================

void DirectX::DrawBar(){

//// HEALTHBAR - Juice
//SetRect(&part, 506, 1, 507, 12);
//DrawTexture(mTexture, part, 11, 456, 255);
//for (int i = 0; i < (mHealth * 490 / mMaxHealth); ++i)
//	DrawTexture(mTexture, part, i + 18, 456, 255);

//float FWidth = (float) ClientWidth;
//FXPixel = (__int64)((FWidth * FPercent) / 100.0);
BarSetPosition((int)CItem.PercentExtracted);
//BarSetPosition(100);
//BarSetStepSize(0.5);
//mCurValue = CItem.PercentExtracted;

float width = 299;

// Percent complete
float Range    = (float)mProgressMax - mProgressMin;
float Progress = (float)mCurValue - mProgressMin;
if(Range == 0)
	mPercent = 100;
else
	mPercent = (Progress / Range) * 100.0f;

// Calc Pixel width
mFXPixel = (int)((width * mPercent) / 100.0f);
const int left = 20;
const int top = 450;


// Bar back
for(int i = 0; i <= width * 2; ++i)
	DrawTexture(mBarTexture, left + i, top, 1, 16, 0);

// Bar front
for(int i = 0, p = 1; i <= (mFXPixel * 2); ++i, ++p)
{
	//if (p == 16) p = 1;
	//DrawSprite(mBarTexture, left + i, top, 1 + i, 16, false);
	DrawTexture(mBarTexture, left + i, top, 1, 16, 1);
	//DrawTexture(mBarTexture, left + i, top, 1, 16, 1);
}

}

//=======================================================================

void DirectX::BarSetStepSize(const int& pValue){

mStep = pValue;

}

//=======================================================================

void DirectX::BarStep(){

if((mCurValue + mStep) < mProgressMin)
	mCurValue = mProgressMin;
else if((mCurValue + mStep) <= mProgressMax)
	mCurValue += mStep;
else
	mCurValue = mProgressMax;

}

//=======================================================================

void DirectX::BarSetPosition(const int& pValue){

if(pValue < mProgressMin)
	mCurValue = mProgressMin;
else if(pValue > mProgressMax)
	mCurValue = mProgressMax;

mCurValue = pValue;

}

//=======================================================================

void DirectX::MouseLDownSet()  {
	IsLMouseDown = true;
	CursorPosGet();

}

//=======================================================================

void DirectX::MouseLUpSet()  {
	IsLMouseDown = false;
}

//=======================================================================

void DirectX::MouseRDownSet()  {
	IsRMouseDown = true;
	CursorPosGet();
}

//=======================================================================

void DirectX::MouseRUpSet()  {
	IsRMouseDown = false;
}

//=======================================================================

void DirectX::CursorPosGet(){
	
	POINT p; GetCursorPos(&p);
	mCursorX = (float)p.x;
	mCursorY = (float)p.y;

}

//-----------------------------------------------------------------------------

// Based on code from - Billboard Xbox Sample
void DirectX::DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, 
						   const int& pX,
						   const int& pY,					  
						   const int& pWidth,
						   const int& pHeight,
						   const int& pXImg){

D3DSURFACE_DESC d3dsd;
pTexture->GetLevelDesc(0, &d3dsd);
float x = (float)pX;
float y = (float)pY;

struct SpriteVertex
{
    float sx, sy, sz, rhw;
    float tu, tv;
};

SpriteVertex vSprite[4] =
{
    { x-0.5f, y-0.5f, 0.99f, 1.0f, (float)pXImg, 0.0f },
    { x-0.5f + pWidth, y-0.5f, 0.99f, 1.0f, 0.0, 0.0f },
    { x-0.5f + pWidth, y-0.5f + pHeight, 0.99f, 1.0f,  0.0, 1.0 },
    { x-0.5f, y-0.5f + pHeight, 0.99f, 1.0f, 0.0f, 1.0 },
};

// Set state
d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1,  D3DTA_TEXTURE);
d3ddev->SetTextureStageState(0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1);
d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP,    D3DTOP_DISABLE);
d3ddev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
d3ddev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

// Display the sprite
d3ddev->SetTexture(0, pTexture);
d3ddev->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1);
d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vSprite, sizeof(SpriteVertex));
}

//-----------------------------------------------------------------------------

void DirectX::DrawSprite(IDirect3DBaseTexture8* pTexture, 
		const int& pPosX, const int& pPosY, 
		const int& pSizeX, const int& pSizeY, 
		const bool& pTransparency)
{
// Based on code by BenJeremy
// - http://forums.xbox-scene.com/index.php?showtopic=4016

struct SpriteVertex
{
	float x, y, z, rhw;  // Z always 0.0, rhw always 1.0
	float u, v;          // Texture coords
};


SpriteVertex sprite[4] = // Single square sprite
{ 
{ (float)pPosX, (float)pPosY, 0, 1, 0.0, 0.0 },
{ (float)(pPosX + pSizeX), (float)pPosY, 0, 1, 1.0, 0.0 },
{ (float)(pPosX + pSizeX), (float)(pPosY + pSizeY), 0, 1, 1.0, 1.0 },
{ (float)pPosX, (float)(pPosY + pSizeY), 0, 1, 0.0, 1.0 }
/*	| 			| 						|	|	|	|
	x			y						z	w	tw	tv
*/
};

static IDirect3DVertexBuffer8* vb;

d3ddev->CreateVertexBuffer(4 * sizeof(SpriteVertex), 
							   D3DUSAGE_WRITEONLY, 
							   FVF_SPRITE, D3DPOOL_DEFAULT, &vb);

// Lock vertex buffer
SpriteVertex* pVerts;
vb->Lock(0, 0, (BYTE**)&pVerts, 0);

// Fill vertex buffer
memcpy(pVerts, sprite, sizeof(sprite));

// Unlock vertex buffer
vb->Unlock();

d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

if (pTransparency)
{
	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}
else
	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
d3ddev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

// Draw sprite
d3ddev->SetTexture(0, pTexture);
d3ddev->SetVertexShader(FVF_SPRITE);
d3ddev->SetStreamSource(0, vb, sizeof(SpriteVertex));
d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

// Clean up
d3ddev->SetTexture(0, NULL);
vb->Release();

}

//-----------------------------------------------------------------------------

HRESULT DirectX::LoadTexture(const LPSTR& pFileName, const D3DCOLOR& pColour, 
					LPDIRECT3DTEXTURE8* pTexture){

return D3DXCreateTextureFromFileEx(d3ddev, 
							pFileName,
							D3DX_DEFAULT, // Width
							D3DX_DEFAULT, // Height
							D3DX_DEFAULT, // Mip
							NULL, // Usage
							D3DFMT_A8R8G8B8, // Format
							D3DPOOL_MANAGED,
							D3DX_DEFAULT, // Filter
							D3DX_DEFAULT, // Mipfilter
							pColour,
							NULL, // SrcInfo
							NULL, // Palette
							pTexture);
}
