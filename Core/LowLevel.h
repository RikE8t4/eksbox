#ifndef LOWLEVEL_H
#define LOWLEVEL_H

//================================================================
//		LowLevel.h
//================================================================

#include <stdafx.h>

#define XORBUFC(i, buf, xorVal) while (i) buf[--i] ^= xorVal
#define BSWAPC(i) i = (x>>24) | ((i<<8) & 0x00FF0000) | ((i>>8) & 0x0000FF00) | (i<<24)


#define SHL(i, sh)	\
_asm {            	\
push eax;			\
mov eax, [i]; 		\
shl eax, sh;        \
mov [i], eax;		\
pop eax;			\
}


#define SHR(i, sh)	\
_asm {            	\
push eax;			\
mov eax, [i]; 		\
shr eax, sh;        \
mov [i], eax;		\
pop eax;			\
}


#define BSWAP(i)  	\
_asm {          	 \
push eax;     		\
mov eax, [i];     	\
bswap eax;  		\
mov [i], eax;	 	\
pop eax;   	  		\
}

//==============================================================================

extern inline void ByteSwapBufC(const long& size, char* buf);
extern inline void ByteSwapC(ushort& i);
//extern inline void ByteSwapC(short& i);
extern inline void ByteSwapC(uint& i);
extern inline void XorBufC(const int& xorVal, uchar* buffer, const uint& bufSize);
extern void __fastcall XorBufAsm(const int& xorVal, uchar* buffer,
								const uint& bufSize);

// These are not working correctly
extern void __fastcall ByteSwap(ulong& i);
extern void __fastcall ByteSwap(uint& i);
extern void __fastcall ByteSwap(int& i);
extern void __fastcall ByteSwap(ushort& i);
//extern void __fastcall ByteSwap(short& i);

//==============================================================================

#endif
