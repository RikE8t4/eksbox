// XBMCABX.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include <XBFont.h>
#include <XBHelp.h>
#include <XGraphics.h>
#include "XBProfiling.h"
#include <D3D8Perf.h>

#include "Open.h"
#include "Supported.h"
#include "Helper.h"
#include "Strings.h"
#include "SystemX.h"
#include "FileSpecs.h"
#include "XBOXABX.h"
#include "DxControls.h"
//#include "Gui.h"


CXboxAbx xbApp;

struct {
	float time[4];
}Beats = {0.0f, 0.0f, 0.0f, 0.0f};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_SPRITE (D3DFVF_XYZRHW | D3DFVF_TEX1)

const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );
const char* WELCOME_TEXT = "Press Start/A To Extract/Convert Format...";

//------------------------------------------------------------------------------

void __cdecl main()
{
    //CXboxAbx xbApp;
    if(FAILED(xbApp.Create()))
        return;
	xbApp.Run();
}

//-----------------------------------------------------------------------------

INT CXboxAbx::Run()
{
    OUTPUT_DEBUG_STRING( "XBApp: Running the application...\n" );

    // Get the frequency of the timer
    LARGE_INTEGER qwTicksPerSec;
    QueryPerformanceFrequency( &qwTicksPerSec );
    mSecsPerTick = 1.0f / (FLOAT)qwTicksPerSec.QuadPart;

    // Save the start time
    QueryPerformanceCounter( &qwTime );
    qwLastTime.QuadPart = qwTime.QuadPart;
    qwAppTime.QuadPart        = 0;
    qwElapsedTime.QuadPart    = 0;
    qwElapsedAppTime.QuadPart = 0;

    // Run the game loop, animating and rendering frames
    while(true)
    {
        // This code exists purely to trigger a FastCAP profile.
        // It disappears in debug and release builds.
        CXBSimpleFastCapProfile profile;
        // A FastCAP profile could also be triggered via controller input.
        if ( XBProfiling_IsFastCAPRequested() )
            profile.BeginProfiling();

        //-----------------------------------------
        // Handle input

        // Read the input for all connected gamepads
        XBInput_GetInput( mGamepad );

        // Lump inputs of all connected gamepads into one common structure.
        // This is done so apps that need only one gamepad can function with
        // any gamepad.
        ZeroMemory( &mDefaultGamepad, sizeof(mDefaultGamepad) );
        INT nThumbLX = 0;
        INT nThumbLY = 0;
        INT nThumbRX = 0;
        INT nThumbRY = 0;

        for( DWORD i=0; i<4; i++ )
        {
            if( mGamepad[i].hDevice )
            {
                // Only account for thumbstick info beyond the deadzone
                if( mGamepad[i].sThumbLX > XINPUT_DEADZONE ||
                    mGamepad[i].sThumbLX < -XINPUT_DEADZONE )
                    nThumbLX += mGamepad[i].sThumbLX;
                if( mGamepad[i].sThumbLY > XINPUT_DEADZONE ||
                    mGamepad[i].sThumbLY < -XINPUT_DEADZONE )
                    nThumbLY += mGamepad[i].sThumbLY;
                if( mGamepad[i].sThumbRX > XINPUT_DEADZONE ||
                    mGamepad[i].sThumbRX < -XINPUT_DEADZONE )
                    nThumbRX += mGamepad[i].sThumbRX;
                if( mGamepad[i].sThumbRY > XINPUT_DEADZONE ||
                    mGamepad[i].sThumbRY < -XINPUT_DEADZONE )
                    nThumbRY += mGamepad[i].sThumbRY;

                mDefaultGamepad.fX1 += mGamepad[i].fX1;
                mDefaultGamepad.fY1 += mGamepad[i].fY1;
                mDefaultGamepad.fX2 += mGamepad[i].fX2;
                mDefaultGamepad.fY2 += mGamepad[i].fY2;
                mDefaultGamepad.wButtons        |= mGamepad[i].wButtons;
                mDefaultGamepad.wPressedButtons |= mGamepad[i].wPressedButtons;
                mDefaultGamepad.wLastButtons    |= mGamepad[i].wLastButtons;

                if( mGamepad[i].Event )
                    mDefaultGamepad.Event = mGamepad[i].Event;

                for( DWORD b=0; b<8; b++ )
                {
                    mDefaultGamepad.bAnalogButtons[b]        |= mGamepad[i].bAnalogButtons[b];
                    mDefaultGamepad.bPressedAnalogButtons[b] |= mGamepad[i].bPressedAnalogButtons[b];
                    mDefaultGamepad.bLastAnalogButtons[b]    |= mGamepad[i].bLastAnalogButtons[b];
                }
            }
        }

        // Clamp summed thumbstick values to proper range
        mDefaultGamepad.sThumbLX = (SHORT)nThumbLX;
        mDefaultGamepad.sThumbLY = (SHORT)nThumbLY;
        mDefaultGamepad.sThumbRX = (SHORT)nThumbRX;
        mDefaultGamepad.sThumbRY = (SHORT)nThumbRY;

        // Handle special input combo to trigger a reboot to the Xbox Dashboard
        if( mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 0 )
        {
            if( mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 0 )
            {
                if( mDefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK] )
                {
                    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
                    XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
                }
            }
        }

		UpdateFrameRate();
        FrameMove();
        Render();

        // Render the frame again if the external PIX performance tool is running
        // and has requested us to repeat the frame. Note that we don't move, but
        // rather re-render the scene exactly as before.
        if( TRUE == D3DPERF_QueryRepeatFrame() )
            Render();
    }
}

//-----------------------------------------------------------------------------

CXboxAbx::CXboxAbx() 
{
    // Initialize member variables
    //g_pXBApp          = this;
	mScreenWidth = 640, 
	mScreenHeight = 480;

    // Direct3D variables
    mD3DEnum		= NULL;
    mD3dDevice		= NULL;
    mDepthBuffer	= NULL;
    mBackBuffer		= NULL;

    // Variables to perform app timing
    mTime           = 0.0f;
    mElapsedTime    = 0.0f;
    mAppTime        = 0.0f;
    mElapsedAppTime = 0.0f;
    mFPS            = 0.0f;
    mStrFrameRate[0] = L'\0';

    // Set up the presentation parameters for a double-buffered, 640x480,
    // 32-bit display using depth-stencil. Override these parameters in
    // your derived class as your app requires.
    ZeroMemory( &mD3dPParam, sizeof(mD3dPParam) );
    mD3dPParam.BackBufferWidth			= mScreenWidth;
    mD3dPParam.BackBufferHeight			= mScreenHeight;
    mD3dPParam.BackBufferFormat			= D3DFMT_A8R8G8B8;
    mD3dPParam.BackBufferCount			= 1;
    mD3dPParam.EnableAutoDepthStencil	= false;//true;
    mD3dPParam.AutoDepthStencilFormat	= D3DFMT_D16;//D3DFMT_D24S8;
    mD3dPParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	mD3dPParam.MultiSampleType			= D3DMULTISAMPLE_NONE;
	mD3dPParam.FullScreen_RefreshRateInHz = 60;

    // Specify number and type of input devices this app will be using. By
    // default, you can use 0 and NULL, which triggers XInputDevices() to
    // pre-alloc the default number and types of devices. To use chat or
    // other devices, override these variables in your derived class.
    mNumInputDeviceTypes = 0;
    mInputDeviceTypes      = NULL;

    m_pdmWalkCounters = NULL;
	mStatusHdrText = CharToWStr(WELCOME_TEXT);

}

//-----------------------------------------------------------------------------

HRESULT CXboxAbx::Create()
{
    HRESULT hr;

    // Create the Direct3D object
    OUTPUT_DEBUG_STRING( "XBApp: Creating Direct3D...\n" );
    if( NULL == ( mD3DEnum = Direct3DCreate8(D3D_SDK_VERSION) ) )
    {
        OUTPUT_DEBUG_STRING( "XBApp: Unable to create Direct3D!\n" );
        return E_FAIL;
    }

    // Create the device
    OUTPUT_DEBUG_STRING( "XBApp: Creating the D3D device...\n" );
    if( FAILED( hr = mD3DEnum->CreateDevice( 0, D3DDEVTYPE_HAL, NULL, 
                                           D3DCREATE_HARDWARE_VERTEXPROCESSING, 
                                           &mD3dPParam, &mD3dDevice ) ) )
    {
        OUTPUT_DEBUG_STRING( "XBApp: Could not create D3D device!\n" );
        return hr;
    }

    // Allow global access to the device
    //g_pd3dDevice = mD3dDevice;

    // Store pointers to the depth and back buffers
    mD3dDevice->GetDepthStencilSurface( &mDepthBuffer );
    mD3dDevice->GetBackBuffer( 0, 0, &mBackBuffer );

    // Initialize core peripheral port support. Note: If these parameters
    // are 0 and NULL, respectively, then the default number and types of 
    // controllers will be initialized.
    XInitDevices( mNumInputDeviceTypes, mInputDeviceTypes );

    // Create the gamepad devices
    OUTPUT_DEBUG_STRING( "XBApp: Creating gamepad devices...\n" );
    if( FAILED( hr = XBInput_CreateGamepads( &mGamepad ) ) )
    {
        OUTPUT_DEBUG_STRING( "XBApp: Call to CreateGamepads() failed!\n" );
        return hr;
    }

    // Initialize the app's device-dependent objects
    OUTPUT_DEBUG_STRING( "XBApp: Initializing the app...\n" );
    if( FAILED( hr = Initialize() ) )
    {
        OUTPUT_DEBUG_STRING( "XBApp: Call to Initialize() failed!\n" );
        return hr;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
VOID CXboxAbx::Destroy()
{
    SAFE_RELEASE(mD3dDevice);
    SAFE_RELEASE(mD3DEnum);
	SAFE_RELEASE(mSprite);
	SAFE_RELEASE(mBGTexture);
	SAFE_RELEASE(mIconTexture);
}

//-----------------------------------------------------------------------------

void CXboxAbx::UpdateFrameRate(){

// Get the current time (keep in LARGE_INTEGER format for precision)
QueryPerformanceCounter( &qwTime );
qwElapsedTime.QuadPart = qwTime.QuadPart - qwLastTime.QuadPart;
qwLastTime.QuadPart    = qwTime.QuadPart;
qwElapsedAppTime.QuadPart = qwElapsedTime.QuadPart;
qwAppTime.QuadPart    += qwElapsedAppTime.QuadPart;

// Store the current time values as floating point
mTime           = mSecsPerTick * ((FLOAT)(qwTime.QuadPart));
mElapsedTime    = mSecsPerTick * ((FLOAT)(qwElapsedTime.QuadPart));
mAppTime        = mSecsPerTick * ((FLOAT)(qwAppTime.QuadPart));
mElapsedAppTime = mSecsPerTick * ((FLOAT)(qwElapsedAppTime.QuadPart));

// Compute the FPS (frames per second) once per second
static FLOAT fLastTime = 0.0f;
static DWORD dwFrames  = 0L;
dwFrames++;

if( mTime - fLastTime > 1.0f )
{
    mFPS = dwFrames / (mTime - fLastTime);
    fLastTime = mTime;
    dwFrames = 0L;
    swprintf(mStrFrameRate, L"%0.02f fps", mFPS);
}

}

//-----------------------------------------------------------------------------

HRESULT CXboxAbx::Initialize()
{
	HRESULT hr = E_FAIL;

    if(FAILED(m_Font.Create("Font.xpr"))) // Create a font
        hr =  XBAPPERR_MEDIANOTFOUND;
	
	mDXC.List = new CList(mD3dDevice);
	mDXC.List->Init(m_Font, -1, "d:\\XboxFormats");
	mDXC.List->SetBlink(true);
	mDXC.ProgressBar = new CProgressBar(mD3dDevice);
	mDXC.ProgressBar->Init();

	
	//mD3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	//mD3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//mD3dDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	//mD3dDevice->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//mD3dDevice->SetRenderState(D3DRS_DITHERENABLE, true);
	//mD3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	//mD3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	//mD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

   //// Set up world matrix
   //D3DXMATRIX matWorld;
   //D3DXMatrixIdentity( &matWorld );
   //mD3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

   //// Set up view matrix
   //D3DXMATRIX  matView;
   //D3DXVECTOR3 vEyePt    = D3DXVECTOR3( 0.0f, 0.0f,-250.0f );
   //D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f,   0.0f );
   //D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f,   0.0f );
   //D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
   //mD3dDevice->SetTransform( D3DTS_VIEW, &matView );

   //// Set up proj matrix
   //D3DXMATRIX matProj;
   //D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 4.0f/3.0f, 1.0f, 1000.0f );
   //mD3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

   //// Setup a base material
   //D3DMATERIAL8 mtrl;
   //XBUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f, 1.0f );
   //mD3dDevice->SetMaterial( &mtrl );

   hr = D3DXCreateSprite(mD3dDevice, &mSprite);
   LoadTexture("d:\\Media\\BG.png", clBlack, &mBGTexture);
   LoadTexture("d:\\Media\\Icon.png", clBlack, &mIconTexture);
   LoadTexture("d:\\Media\\Progress.png", clBlack, &mBarTexture);

   //DmEnableGPUCounter(true); // Enable GPU Performance counters so we can read them.

   return hr;
}

//-----------------------------------------------------------------------------

// Based on code from - Billboard Xbox Sample
void CXboxAbx::DrawTexture(const LPDIRECT3DTEXTURE8& pTexture, 
						   const int& pX,
						   const int& pY,					  
						   const int& pWidth,
						   const int& pHeight,
						   const int& pXImg){

D3DSURFACE_DESC d3dsd;
pTexture->GetLevelDesc(0, &d3dsd);
FLOAT x = (float)pX;
FLOAT y = (float)pY;

struct SpriteVertex
{
    FLOAT sx, sy, sz, rhw;
    FLOAT tu, tv;
};

SpriteVertex vSprite[4] =
{
    { x-0.5f, y-0.5f, 0.99f, 1.0f, (float)pXImg, 0.0f },
    { x-0.5f + pWidth, y-0.5f, 0.99f, 1.0f, 0.0, 0.0f },
    { x-0.5f + pWidth, y-0.5f + pHeight, 0.99f, 1.0f,  0.0, 1.0 },
    { x-0.5f, y-0.5f + pHeight, 0.99f, 1.0f, 0.0f, 1.0 },
};

// Set state
mD3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
mD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1,  D3DTA_TEXTURE);
mD3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1);
mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,    D3DTOP_DISABLE);
mD3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
mD3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
mD3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
mD3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
mD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
mD3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
mD3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

// Display the sprite
mD3dDevice->SetTexture(0, pTexture);
mD3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1);
mD3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vSprite, sizeof(SpriteVertex));
}

//-----------------------------------------------------------------------------

void CXboxAbx::DrawSprite(IDirect3DBaseTexture8* pTexture, 
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

static bool forward = true;
static bool forward2 = true;
static float alpha = 0.0f;
static float alpha2 = 0.0f;

if (forward2)
	alpha2-=0.5f;
else
	alpha2+=0.5f;

if (alpha2 > 5.0f || alpha2 < -5.0f)
	forward2 = !forward2;


if (forward)
	alpha+=2.5f;
else
	alpha-=2.5f;

if (alpha > 1000.0f || alpha < -1000.0f)
	forward = !forward;


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

mD3dDevice->CreateVertexBuffer(4 * sizeof(SpriteVertex), 
							   D3DUSAGE_WRITEONLY, 
							   FVF_SPRITE, D3DPOOL_DEFAULT, &vb);

// Lock vertex buffer
SpriteVertex* pVerts;
vb->Lock(0, 0, (BYTE**)&pVerts, 0);

// Fill vertex buffer
memcpy(pVerts, sprite, sizeof(sprite));

// Unlock vertex buffer
vb->Unlock();

mD3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
mD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

//mD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
//mD3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, alpha);
//mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
//mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


if (pTransparency)
{
	// Get alpha info from texture
	mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}
else
	mD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

//mD3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
//mD3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

// Draw sprite
mD3dDevice->SetTexture(0, pTexture);
mD3dDevice->SetVertexShader(FVF_SPRITE);
mD3dDevice->SetStreamSource(0, vb, sizeof(SpriteVertex));
mD3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

// Clean up
mD3dDevice->SetTexture(0, NULL);
vb->Release();

}

//-----------------------------------------------------------------------------

HRESULT CXboxAbx::LoadTexture(const LPSTR& pFileName, const D3DCOLOR& pColour, 
					LPDIRECT3DTEXTURE8* pTexture){

return D3DXCreateTextureFromFileEx(mD3dDevice, 
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

//-----------------------------------------------------------------------------

void CXboxAbx::DrawMemoryTxt(){

string s = str.SizeToStr(sys.GetAvailPhyMem());
mStatusMemory = StrToWStr(s);

}

//-----------------------------------------------------------------------------

void CXboxAbx::DrawHdrTxt(string txt){

if (!txt.length())
	mStatusHdrText = CharToWStr(WELCOME_TEXT);
else
	mStatusHdrText = CharToWStr(txt.c_str());

this->UpdateFrameRate();
Render();

}

//-----------------------------------------------------------------------------

void CXboxAbx::DrawStatusTxt(string txt){

//float width = m_Font.GetTextWidth(CharToWChar(txt.c_str()));
//if (width + 85 > SCREEN_WIDTH)
//{
//	while(m_Font.GetTextWidth(CharToWChar(txt.c_str())) + 85 > SCREEN_WIDTH)
//	{
//		txt.resize(txt.length()-1);
//	}
//	//txt.resize(SCREEN_WIDTH - 80);
//	strncpy(m_status_text, txt.c_str(), txt.length());
//	strcat(m_status_text, "...");
//}
//else
	mStatusText = CharToWStr(txt.c_str());
	//strcpy(m_status_text, txt.c_str());

this->UpdateFrameRate(); 
Render();

}

//-----------------------------------------------------------------------------

HRESULT CXboxAbx::FrameMove()
{


static bool fastScroll = false;
int scroll = 0;

static float lastTime = 0.0f;
static float delay = 0.15f;
float speedup = 0.0;

if(mTime - lastTime > 0.2f)
{
	lastTime = mTime;
	fastScroll = true;
	delay = 0.15f;
}

if ((mDefaultGamepad.wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
	(mDefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) == false) )
	scroll = 1;
else if ( (mDefaultGamepad.wLastButtons & XINPUT_GAMEPAD_DPAD_UP &&
		 (mDefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP) == false) )
	scroll = -1;

if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] ||
	mDefaultGamepad.fY1 < -0.25f)
{
	scroll = 1;
	if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER])
		speedup = mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] / 255.0f;
	else
		speedup = abs(mDefaultGamepad.fY1); // Max = -1.0000000 / Min 0
	
}
else if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] ||
	mDefaultGamepad.fY1 > 0.25f)
{
	scroll = -1;
	if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER])
		speedup = mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] / 255.0f;
	else
		speedup = abs(mDefaultGamepad.fY1); 
}


if(fastScroll && scroll == 1 && mTime - lastTime > delay){
	mDXC.List->SelectedInc();
	lastTime = mTime;
	if (speedup){								
			delay = (0.84 / speedup) * (0.16f - (delay / 2)); 
			if (speedup >= 0.9500000f)
				delay -= 0.065f;
	}
	else if (delay > 0.06)						
		delay -= 0.02f - speedup;
}
else if(fastScroll && scroll == -1 && mTime - lastTime > delay){
	mDXC.List->SelectedDec();
	lastTime = mTime;
	if (speedup){								
			delay = (0.84 / speedup) * (0.16f - (delay / 2)); 
			if (speedup >= 0.9500000f)
				delay -= 0.065f;
	}
	else if (delay > 0.06)						
		delay -= 0.02f - speedup;
}



if (!scroll && mDefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN){
	mDXC.List->SelectedInc();
	lastTime = mTime;
	fastScroll = false;
}

if (!scroll && mDefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP){
	mDXC.List->SelectedDec();
	lastTime = mTime;
	fastScroll = false;
}


if (!scroll && mDefaultGamepad.Event == XBGAMEPAD_RIGHT_TRIGGER){
	mDXC.List->SelectedInc();
	lastTime = mTime;
	fastScroll = false;
}

if (!scroll && mDefaultGamepad.Event == XBGAMEPAD_LEFT_TRIGGER){
	mDXC.List->SelectedDec();
	lastTime = mTime;
	fastScroll = false;
}

if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_X]){
	mDXC.List->SortListByName();
}

if (mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_Y]){
	mDXC.List->SortListByType();
}


if (mDefaultGamepad.Event == XBGAMEPAD_BLACK){

}

if (mDefaultGamepad.Event == XBGAMEPAD_WHITE){

}

if ( mDefaultGamepad.fY2 > 0.25f ){

}

else if ( mDefaultGamepad.fY2 < -0.20f ){

}
           



if (mDefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START ||
	mDefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_A])
{
	Open op;
	bool rtn = false;
	std::string s, file;
	if (s != "")
		rtn = op.DoOpen(s);
	else
	{
		file = WStrToStr(mDXC.List->GetSelectedName());

		if (file == ""){
			DrawHdrTxt("Filename is blank");
			return 0;
		}

		DrawHdrTxt("Opening: " + file);
		//Sleep(500);
		rtn = op.DoOpen(mDXC.List->GetSelectedPathAndName());
		DrawHdrTxt(mDXC.List->GetSelectedPathAndName());
		Sleep(5000);
	}

	if (rtn == true){
		DrawHdrTxt("Extracting/Converting: " + file);
		Supported sup; 
		//Supported sup; sup.StdExtract(1);
		if (sup.StdExtract(-1))
			DrawHdrTxt(""); // Reset
			//DrawHdrTxt("Successful extraction");
		else
			DrawHdrTxt("Extraction error");
}
else
{
	DrawHdrTxt("Unsupported/Missing file?");
}


}

if (DmWalkPerformanceCounters( &m_pdmWalkCounters, &m_dmCounterInfo ) == XBDM_ENDOFLIST)
{
    // If we hit the end, restart from the beginning
    m_pdmWalkCounters = NULL;
    DmWalkPerformanceCounters( &m_pdmWalkCounters, &m_dmCounterInfo );
}


ZeroMemory( &m_dmCounterData, sizeof( DM_COUNTDATA ) );

// Set up our query type
DWORD dwQuery = m_dmCounterInfo.Type & DMCOUNT_COUNTTYPE;
if (dwQuery == DMCOUNT_EVENT)
{
    dwQuery |= ( m_dmCounterInfo.Type & DMCOUNT_COUNTSUBTYPE ) | DMCOUNT_PERSEC;
}
else if (dwQuery == DMCOUNT_VALUE || dwQuery == DMCOUNT_PRATIO)
{
    dwQuery |= 0;
}

m_hrQuery = DmQueryPerformanceCounterHandle( m_hCounter, dwQuery, &m_dmCounterData );

return S_OK;
}


//-----------------------------------------------------------------------------

HRESULT CXboxAbx::Render()
{
	mD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, clBlack, 1.0, 0);
	mD3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, clBlack, 1.0, 0);
	mD3dDevice->BeginScene();

	/*
	int x = 200;
	RECT pos = {x, 0, x+640, 480}; // left, top, right, bottom,
	mSprite->Draw(mTexture, &pos, NULL, NULL, 0, NULL, clWhite);
	*/

	// Kenny Ghost Roam
	const COLOURS = 6;
	static D3DCOLOR memColour[COLOURS] = {clWhite, clRed, clGreen, clBlue, clPink, clYellow};
	static int colorIndex = 0;
	static float x = 0.00;
	static bool forward = true;
	static float delay = 0.0;
	const minSpeed = 1;
	static float speed = minSpeed;

	if (delay > 70 && x <= 1)
	{
		speed = minSpeed;
		delay = 1.0;
	}

	delay += 0.08f;
	if (delay > 20 && x <= 1)
		speed += 1.07f;

	if (x > 35 && x < 145){
		++colorIndex;
		if (colorIndex > COLOURS)
			colorIndex = 0;
	}

	if (speed > 30 && speed > minSpeed){
		speed -= minSpeed;
		delay = 0.0;
	}

	if (delay > 10){

		if (x > 167 || x < 0)
			forward = !forward;
	
		if (forward)
			x += speed;
		else
			x -= speed;
	}

	
	if (CItem.Files)
		x = 0;

	// Draw BG Image
	mSprite->Begin();
		DrawSprite(mBGTexture, 0, 0, 640, 480, false);
		const barWidth = 500;
		const barLeft = 64, barTop = 78;
		//const float barSpeed = 5;
		int barPos = int((barWidth *  CItem.PercentExtracted) / 100.0f);
		//static bool barForward = true;

		//if (barPos >= barWidth || barPos <= 0)
		//	barForward = !barForward;

		//if (barForward)
		//	barPos += barSpeed;
		//else
		//	barPos -= barSpeed;

		// Bar Back
		for (int i = 0; i < barWidth; ++i)
			DrawTexture(mBarTexture, i + barLeft, barTop, 1, 16, 0);
		// Bar front
		for (int i = 0, p = 0; i < barPos; ++i){
			//p += 1;
			//if (p > 8)
			//	p = 0;
			DrawTexture(mBarTexture, (int)i + barLeft, barTop, 1, 16, 1 + p);
		}

		//RECT part = {0, 0, 1, 16}; // left, top, right, bottom
		//SetRect(&part, 1, 0, 2, 16);
		//D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
			//RECT srcRect = {i, 0, i +1, 16}; // left, top, right, bottom,
			//D3DXVECTOR2 scaling(1,1), rcenter(0,0);
			//mSprite->Draw(mBarTexture, &srcRect, &scaling, &rcenter, 
			//0, NULL, clWhite);
		//}
	
	mSprite->End();

	//mXmvPlayer.RenderFrame();

    //m_Font.Begin();
	//int totalFrames = m_VideoDesc.FramesPerSecond * m_pXMVDecoder->GetTimeFromStart();
	//m_Font.DrawText(450,  200, clYellow, IntToWStr(mXmvPlayer.GetCurrentFrame()).c_str());
	//m_Font.DrawText(450,  230, clYellow, IntToWStr(mXmvPlayer.GetFramesPerSecond()).c_str());
	//m_Font.DrawText(450,  260, clYellow, IntToWStr(mXmvPlayer.GetTotalFrames()).c_str());
	
	//wstring time = str.FormatTimeW((float)mXmvPlayer.GetTimeFromStart() / 1000);
	//m_Font.DrawText(450,  290, clYellow, time.c_str());

	//time = str.FormatTimeW((float)mXmvPlayer.GetTotalTime() / 1000);
	//m_Font.DrawText(450,  320, clYellow, time.c_str());
	//m_Font.End();

    m_Font.Begin();
	m_Font.m_fFontHeight = 21;
	m_Font.DrawText(308,  40, memColour[colorIndex], mStatusMemory.c_str());
    m_Font.DrawText(450,  40, clYellow, mStrFrameRate);

	m_Font.m_fFontHeight = 30;
	m_Font.SetScaleFactors(1.5, 1.5);
	m_Font.DrawText(64,  34, XARGB(200, 255, 0, 0), L"XBOX-ABX");
	
	m_Font.m_fFontHeight = 21;
	m_Font.SetScaleFactors(1, 1);

	static D3DCOLOR colour = XARGB(0, 100, 100, 100);
	static int listY = 500;

	if (CItem.Files)
		colour = clWhite;
	else if (listY <= 0)
	{
		if (colour < 0x44444444)
			colour = XRGB(50, 50, 50);

		colour += XRGB(10, 10, 10);
	}

	//int peak = mWmaPlayer.GetTotalPeak();
	int peak = 0;
	if (peak < 0)
		peak = 0;

	//peak = 100 / peak;

	// 4294967295
	//int max = 200 / 4294967295;
	//int xdance = int((200 * (max)) / 100.0f);


	int xdance = 60;
	if (peak > 0x10000000)
		xdance += tan((float)peak) / cos((float)peak*2);

	if (xdance < 60)
		xdance = 60;
	else if (xdance > 100)
		xdance = 100;

	//wstring percent = StrToWStr(IntToStr(mWmaPlayer.GetPercentCompleted()) + "%");
	//m_Font.DrawText(xdance, 440, clRed, (mWmaPlayer.GetWmaAuthorL() + L" - " + mWmaPlayer.GetWmaTitleL() + L" [" + percent + L"]").c_str());

	m_Font.DrawText(64, 101, colour, mStatusHdrText.c_str());
	m_Font.DrawText(64, 135, clYellow, mStatusText.c_str());

	//// Calc BPM
	//static int index = 0;
	//static float lastTime = 0.0f;
	//if (mTime - lastTime > 0.3f)
	//if (peak > 0x79000000){
	//	lastTime = mTime;
	//	Beats.time[index] = mTime;
	//	++index;
	//	if (index > 4){
	//		index = 0;
	//		if( Beats.time[3] - Beats.time[2] > 0.3f && Beats.time[3] - Beats.time[2] <= 2.0f ) // 200 Beats per min max
	//			mStatusHdrText = StrToWStr(IntToStr( (int)(Beats.time[3] - Beats.time[2])));
	//		else if( Beats.time[2] - Beats.time[1] > 0.3f && Beats.time[2] - Beats.time[1] <= 2.0f ) // 200 Beats per min max
	//			mStatusHdrText = StrToWStr(IntToStr((int) Beats.time[2] - Beats.time[1]));
	//		else if( Beats.time[1] - Beats.time[2] > 0.3f && Beats.time[1] - Beats.time[0] <= 2.0f ) // 200 Beats per min max
	//			mStatusHdrText = StrToWStr(IntToStr((int) Beats.time[1] - Beats.time[0]));
	//	}
	//}


	if (delay > 0.15){
		if (listY > 0)
			listY -= 18;
	}

	DWORD selcol = clRed;
	//if (mWmaPlayer.GetTotalPeak() > 0x50000000 && mWmaPlayer.GetTotalPeak() < 0x55000000)
	//	selcol = XRGB(220, mWmaPlayer.GetTotalPeak(), 220);
	//else if (mWmaPlayer.GetTotalPeak() > 0x55000000)
	//	selcol = XRGB(mWmaPlayer.GetTotalPeak(), 255, 250);
	
			
	DrawMemoryTxt();
	mDXC.List->Config(90, 180 + listY, -1, 26, clWhite, selcol );
	mDXC.List->Draw();
    m_Font.End();

	// Draw Kenny Ghost
	mSprite->Begin();
		DrawSprite(mIconTexture, (int)x + 240, 37 + +xdance/4-12, 32, 32, true);
	mSprite->End();

	//mDXC.ProgressBar->Draw();

	// Render a simple progress bar to show the percent completed
    //struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
    //BACKGROUNDVERTEX v[8];
    //FLOAT x1 =  40, x2 = x1 + (256*50)/100, x3 = 64+256;
    //FLOAT y1 = 200, y2 = y1 + 20;
    //v[0].p = D3DXVECTOR4( x1-0.5f, y1-0.5f, 1.0f, 1.0f );  v[0].color = 0xffffff00;
    //v[1].p = D3DXVECTOR4( x2-0.5f, y1-0.5f, 1.0f, 1.0f );  v[1].color = 0xffffff00;
    //v[2].p = D3DXVECTOR4( x2-0.5f, y2-0.5f, 1.0f, 1.0f );  v[2].color = 0xffffff00;
    //v[3].p = D3DXVECTOR4( x1-0.5f, y2-0.5f, 1.0f, 1.0f );  v[3].color = 0xffffff00;
    //v[4].p = D3DXVECTOR4( x2-0.5f, y1-0.5f, 1.0f, 1.0f );  v[4].color = 0xff8080ff;
    //v[5].p = D3DXVECTOR4( x3-0.5f, y1-0.5f, 1.0f, 1.0f );  v[5].color = 0xff8080ff;
    //v[6].p = D3DXVECTOR4( x3-0.5f, y2-0.5f, 1.0f, 1.0f );  v[6].color = 0xff8080ff;
    //v[7].p = D3DXVECTOR4( x2-0.5f, y2-0.5f, 1.0f, 1.0f );  v[7].color = 0xff8080ff;

	mD3dDevice->EndScene();
    mD3dDevice->Present(NULL, NULL, NULL, NULL);
    return S_OK;
}


//-----------------------------------------------------------------------------



