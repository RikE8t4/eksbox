// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)

#ifdef _XBOX
#include <Xtl.h>
#include <winbase.h> 
#endif

#ifdef _WINDOWS
#include <windows.h> 
#endif

#ifdef WIN32DX
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#endif

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <tchar.h>

using std::string;
using std::vector;
using namespace std;

#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ulong unsigned long

#define int8 INT8			// Char
#define uint8 UINT8			// UChar
#define int16 INT16			// Short
#define uint16 UINT16		// UShort
#define int32 INT32			// Int / Long
#define uint32 UINT32		// UInt / ULong
#define int64 INT64			// Long
#define uint64 UINT64		// ULong
#define int128 long long	// Long Long
#define uint128 unsigned long long	// ULong Long

#define float32 float
#define float64 double

#ifndef _CONSOLE
#define XRGB D3DCOLOR_XRGB
#define XARGB D3DCOLOR_ARGB
#define XRADIUS D3DXToRadian
#define CUSTOMFVF2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) // 2D
//#define CUSTOMFVF3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 3D
#define CUSTOMFVF3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) // 3D
#define clBlack XRGB(0, 0, 0)
#define clWhite XRGB(255, 255, 255)
#define clRed XRGB(255, 0, 0)
#define clGreen XRGB(0, 255, 0)
#define clBlue XRGB(0, 0, 255)
#define clPink XRGB(255, 0, 255)
#define clYellow XRGB(255, 255, 0)
#define clTransparent XARGB(255, 255, 255, 255)
#define PIE 3.14159f

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define XRELEASE(x)		if (x != NULL) { x->Release(); x = NULL; }
#define XDELETE(x) 		if (x != NULL) { delete x; x = NULL; }

#endif

// TODO: reference additional headers your program requires here
