// DirectX1.cpp : Defines the entry point for the application.
//

#include <stdafx.h>
#include "WIN32DXABX.h"
#include "DX.h"
#include "Open.h"
#include "Engine.h"
#include "Strings.h"
#include "ThreadABX.h"
#define MAX_LOADSTRING 100

// Globals
DirectX dx;

void				RegisterWindow(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int nCmdShow, int Width, int Height);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int x, y;

//=======================================================================

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	RegisterWindow(hInstance); // Initialize global strings

	// Perform application initialization:
	HWND hnd = InitInstance (hInstance, nCmdShow, dx.mScreenWidth, dx.mScreenHeight);
	dx.SetHwnd(hnd);
	dx.InitDX(hnd);
	MSG msg;



	// Main message loop:
	while (1) 
	{
		//DWORD timeStart = GetTickCount();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else


		dx.RenderFrame();

		//if (KEY_UP(VK_UP)){
		//	dx.Up();
		//	ReleaseCapture();
		//}
		//else if (KEY_UP(VK_DOWN)){
		//	dx.Down();
		//	ReleaseCapture();
		//}

		//while ((GetTickCount() - timeStart) < 25);
	}
	dx.CleanUp();
	return (int) msg.wParam;
}

//=======================================================================

//  PURPOSE: Registers the window class.

void RegisterWindow(HINSTANCE hInstance){

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName	= "WindowClass";

    RegisterClassEx(&wc);

}

//=======================================================================

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, int Width, int Height)
{

  SIZE siz; 
  siz.cx = GetSystemMetrics(SM_CXSCREEN);
  siz.cy = GetSystemMetrics(SM_CYSCREEN);
  int x = (siz.cx / 2) - (Width / 2);
  int y = (siz.cy / 2) - (Height / 2);

   HWND hWnd = CreateWindowEx(NULL, "WindowClass", "WIN32DX-ABX", 
	   //WS_EX_TOPMOST | WS_POPUP|
	   //WS_OVERLAPPEDWINDOW | 
	   WS_OVERLAPPEDWINDOW - WS_SIZEBOX - WS_MAXIMIZEBOX,
      x, y, Width, Height, NULL, NULL, hInstance, NULL);

   //SetWindowLong(hWnd, 0, WS_MAXIMIZEBOX | SWP_NOSIZE | SWP_NOMOVE);
	//SetWindowPos(hWnd, hWnd, 0, 0, 0, 0, SWP_FRAMECHANGED |
	//		 SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE); 

   if (!hWnd)
      return NULL;

   ShowWindow(hWnd, nCmdShow);

   return hWnd;
}

//=======================================================================

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static bool isDown = false;

	switch(wParam)
	{
	case VK_DOWN:
		if (!isDown)
			dx.Down();
			break;
	case VK_UP:
		if (!isDown)
			dx.Up();
			break;
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	case VK_LEFT:
		dx.IncZoom();
		break;
	case VK_RIGHT:
		dx.DecZoom();
		break;
	case VK_INSERT:
		break;
	case VK_ADD:
		//dx.IncRotSpeed();
		break;
	case VK_SUBTRACT:
		//dx.DecRotSpeed();
		break;
	case VK_RETURN: // Extract
		if (isDown)
		{
			//Open o;
			////Msg("Opening");
			//const string path = "c:\\XboxFormats\\";
			//string file = WStrToStr(xc.List->GetSelected());
			//
			////auto unsigned long id = 0;
			////HANDLE ht = 
			////CreateThread(0, 0, 
			//	//o.DoOpen(path + file), this, 0, &id);
			//	//NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			//int rtn = o.DoOpen(path + file);
			//if (rtn == 1){
			//	//Msg("Opened");
			//	Supported sup; 
			//	rtn = sup.StdExtract(-1);
			//}
			//if (!rtn)
			//	Msg("Extraction error");
			//else
			//	Msg("Extraction Complete");
			StartAbxThread();
		}
		break;
	}


	switch (message) 
	{
	case WM_KEYDOWN:
		isDown = true;
		break;
	case WM_KEYUP:
		isDown = false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		dx.MouseLDownSet();
		break;
	case WM_LBUTTONUP:
		dx.MouseLUpSet();
		break;
	case WM_RBUTTONDOWN:
		dx.MouseRDownSet();
		break;
	case WM_RBUTTONUP:
		dx.MouseRUpSet();
		break;
	case WM_MOUSEMOVE:
		RECT r;
		GetWindowRect(hWnd, &r);
		POINT p; GetCursorPos(&p);
		ScreenToClient(hWnd, &p);
		{
			x = p.x, y = p.y;
			dx.MouseMoveSet(true);
		}
		//ReleaseCapture();
		break;
	case WM_MOUSELEAVE:
		dx.MouseMoveSet(false);
	case WM_MBUTTONDOWN:
		//dx.ResetScene();
		break;
	default:
		dx.MouseMoveSet(false);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//=======================================================================








