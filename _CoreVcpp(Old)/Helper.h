#ifndef HELPER_H
#define HELPER_H

#include "stdafx.h" 
//#include <string>

#define XORBUFC(i, buf, xorVal) while (i) buf[--i] ^= xorVal
#define BSWAPC(i) i = (i>>24) | ((i<<8) & 0x00FF0000) | ((i>>8) & 0x0000FF00) | (i<<24)

/*
#define BSWAPASM(i) __asm	\
{							\
	_asm mov edi, i			\
	_asm mov eax, [edi]		\
	_asm bswap eax			\
	_asm mov [edi], eax		\
}
*/

/*
#define XORBUFASM(bufSiz, buf, xorVal) __asm \
{										\
	_asm mov esi, dword ptr [xorVal]	\
	_asm mov ebx, dword ptr [bufSiz]	\
	_asm begin:							\
		_asm mov eax, [ebx]				\
		_asm dec eax					\
		_asm mov dword ptr [ebx], eax	\
		_asm mov ecx, dword ptr [buf]	\
		_asm add ecx, dword ptr [ebx]	\
		_asm movzx edx, byte ptr [ecx]	\
		_asm xor edx, [esi]				\
		_asm mov eax, dword ptr [buf]	\
		_asm add eax, dword ptr [ebx]	\
		_asm mov byte ptr [eax], dl		\
		_asm cmp dword ptr [ebx], 0		\
		_asm jg	begin					\
}
*/
	
void BSwapAsm(int& i);
void BSwapAsm(uint& i);
void BSwapAsm(ulong& i);
void BSwapAsm(ushort& i);
void Msg(std::string msg);
void Msg(const int& msg);
void XorBufAsm(const int& bufSiz, uchar* buf, const short& xorVal);
string IntToStr(const int& i);
wstring IntToWStr(const int& i);
string FloatToStr(const float& i);
int StrToInt(const string& str);
void Trim(string& str);
void Trim(char* chr);
string CharTrimToStr(char* chr);
string ToLower(const string& str);
wstring ToLower(const wstring& str);
string ToUpper(const string& str);
wstring ToUpper(const wstring& str);
string ReplaceAll(const string& str, const string& orig, const string& repl);
wchar_t* CharToWChar(const char* chr);
wstring CharToWStr(const char* chr);
string WStrToStr(const wstring& str);
wstring StrToWStr(const string& str);
bool DirectoryExists(const string& dirName);
bool FileExists(const string& fileName);
string ChangeFileExt(const string& pName, const std::string& pExt);
string RenameFile(const string& pName, const std::string& pNewName);
string RemoveFileExt(const char* pValue);
string RemoveFileExt(const string& pValue);
string RemoveDrive(const string& pValue);
string ExtractFileExt(const string& pValue);
wstring ExtractFileExt(const wstring& pValue);
string ExtractFileName(const string& pValue);
string ExtractFileDir(const string& pValue);
string ExtractDirPath(const string& pValue);
void ProcessMessages();

#endif