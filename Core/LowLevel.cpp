


//================================================================
//		LowLevel.cpp
//================================================================


#include <stdafx.h>
#pragma hdrstop

//==============================================================================

// Doesn't work perfectly if size is not rounded
inline void ByteSwapBufC(const long& size, char* buf)
{

int i = 0;
while (i < size)
{
	int b1 = buf[i+3];
	int b2 = buf[i+2];
	int b3 = buf[i+1];
	int b4 = buf[i];

	buf[i] = b1;
	buf[i+1] = b2;
	buf[i+2] = b3;
	buf[i+3] = b4;

	i += 4;
}

}

//==============================================================================

inline void ByteSwapC(ushort& i)
{
	i = (i>>8) | (i<<8);
}


//inline void ByteSwapC(short& i)
//{
//i = (i>>8) | (i<<8);
//}

//==============================================================================

inline void ByteSwapC(uint& i)
{
i = (i>>24) | ((i<<8) & 0x00FF0000) | ((i>>8) & 0x0000FF00) | (i<<24);
}

//==============================================================================

inline void XorBufC(const int& xorVal, uchar* buffer, const uint& bufSize)
{
uint i = bufSize;
while (i)
{
	buffer[--i] ^= xorVal;
}
}

//==============================================================================

void __fastcall XorBufAsm(const int& xorVal, uchar* buf, const uint& bufSize){

// Changed - Needs to be tested
_asm{
mov eax, [buf];
mov ecx, bufSize;
dec ecx; 
mov edi, xorVal;

Begin:
//xor eax[ecx], edi; // Issue in VC++
dec ecx;
jnz Begin

//xor eax[ecx], edi; // Issue in VC++
}

}

//==============================================================================

void __fastcall ByteSwap(ulong& i){

_asm{
mov edi, i
mov eax, [edi]
bswap eax
mov [edi], eax
}

}


//==============================================================================

void __fastcall ByteSwap(uint& i){

_asm{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//==============================================================================

void __fastcall ByteSwap(int& i){

_asm
{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//==============================================================================

void __fastcall ByteSwap(ushort& i){

_asm
{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//==============================================================================

//void __fastcall ByteSwap(short& i){
//
//_asm
//{
//	mov edi, i
//	mov ax, [edi]
//	xchg ah, al
//	mov [edi], ax
//}
//
//}

//==============================================================================


