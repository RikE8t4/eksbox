
#include "stdafx.h" 
#include "Gui.h" 
#include "Helper.h" 
#include <sstream>

//====================================================================================

void BSwapAsm(ulong& i)
{

_asm{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//====================================================================================


void BSwapAsm(int& i)
{

_asm{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//====================================================================================


void BSwapAsm(uint& i)
{

_asm{
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}
//====================================================================================


void BSwapAsm(ushort& i)
{

_asm {
	mov edi, i
	mov eax, [edi]
	bswap eax
	mov [edi], eax
}

}

//====================================================================================


void XorBufAsm(const int& bufSiz, uchar* buf, const short& xorVal){
// You don't need to preserve the EAX, EBX, ECX, EDX, ESI, or EDI registers
_asm{

//mov eax, dword ptr[bufSiz]
//mov edi, dword ptr[buf]
////mov bh, byte ptr[xorVal]	
//
//begin:
//	test        eax, eax 
//	je          end
//	mov         cl, byte ptr [eax+edi-1] 
//	dec         eax  
//	xor         cl, 22h
//	mov         byte ptr [eax+edi],cl 
//	jmp         begin
//end:


// Unoptimized
mov esi, dword ptr [xorVal]			// Mov xorval reference into esi
mov ebx, dword ptr [bufSiz]			// Mov bufsize reference into ebx

begin:
	mov         eax, [ebx]			// Mov bufsize into eax
	dec         eax					// Dec bufsize
	mov         dword ptr [ebx], eax// Mov bufsize into ebx
	mov         ecx, dword ptr [buf]// Mov buf into edx 
	add         ecx, dword ptr [ebx]// ^
	movzx       edx, byte ptr [ecx] // ^
	xor         edx, [esi]
	mov         eax, dword ptr [buf]// Mov result into buf 
	add         eax, dword ptr [ebx]// ^
	mov         byte ptr [eax], dl  // ^
	cmp         dword ptr [ebx], 0  // Loop while bufsize is greater than 0
	jg			begin

}

}

//====================================================================================

void Msg(std::string msg){

#ifdef WIN32
MessageBox(NULL, msg.c_str(), NULL, MB_OK); 
#endif

}

//====================================================================================

void Msg(const int& msg){

#ifdef WIN32
MessageBox(NULL, IntToStr(msg).c_str(), NULL, MB_OK); 
#endif

}

//====================================================================================

string IntToStr(const int& i){

std::ostringstream o;
if (!(o << i))
     return "";
   return o.str();
}

//====================================================================================

wstring IntToWStr(const int& i){

std::wstringstream o;
if (!(o << i))
     return L"";
   return o.str();
}

//====================================================================================

string FloatToStr(const float& i){

std::ostringstream o;
if (!(o << i))
     return "";
   return o.str();

}

//====================================================================================

int StrToInt(const string& str){

return atoi(str.c_str());

}

//====================================================================================

void Trim(string& str){

str.erase(str.find_last_not_of(0x20)+1);
str.erase(0, str.find_first_not_of(0x20));

//unsigned int begin = str.find_first_not_of(' ');
//unsigned int end = str.find_last_not_of(' ');
//str.substr(begin, end-begin+1);

}

//====================================================================================

void Trim(char* chr){ // Not working perfectly

int len = (int)strlen(chr);
char* newchr = (char*) malloc (len);
memset(newchr, '\0', len);
int i = 0;
for(; i < len; ++i){
	if (chr[i] != 0x20)
		newchr[i] = chr[i];
	else
		break;
}

memset(chr, '\0', len);
strncpy(chr, newchr, i);
free(newchr);
newchr = NULL;

}

//====================================================================================

string CharTrimToStr(char* chr){

string str = (char*)chr; 

str.erase(str.find_last_not_of(0x20)+1);
str.erase(0, str.find_first_not_of(0x20));
return str;

}

//====================================================================================

string ToLower(const string& str){

string s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = tolower(s[i]); 

return s;

}

//====================================================================================

wstring ToLower(const wstring& str){

wstring s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = tolower(s[i]); 

return s;

}

//====================================================================================

string ToUpper(const string& str) {

string s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = toupper(s[i]); 

return s;

}

//====================================================================================

wstring ToUpper(const wstring& str) {

wstring s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = toupper(s[i]); 

return s;

}

//====================================================================================

string ReplaceAll(const string& str, const string& orig, const string& repl){

if ((int)str.find(orig) == -1 || str.empty() || orig.empty() || repl.empty()) 
	return str;

string s = str;

while (1)
{
	size_t p = s.find(orig);
	if (p == s.npos) break;
	s.replace(p, repl.size(), repl);
	p += repl.size();
}

return s;

}

//====================================================================================

wchar_t* CharToWChar(const char* chr) {

// Memory leak!!!
int	length = MultiByteToWideChar(CP_ACP, 0, chr, -1, 0, 0);		
WCHAR* wfilename = new wchar_t[length];	

MultiByteToWideChar(CP_ACP, 0, chr, -1, wfilename, length);
return wfilename;

}

//====================================================================================

wstring CharToWStr(const char* chr){

int length = (int)strlen(chr);
//int	length = MultiByteToWideChar(CP_ACP, 0, chr, -1, 0, 0);	
wstring res(length, L''); 
for (int i = 0; i < length; ++i)
	res[i] = cin.widen(chr[i]);

return res;

}

//====================================================================================

wstring StrToWStr(const string& str)
{

wstring res(str.length(), L''); 
std::copy(str.begin(), str.end(), res.begin());
return res;

}

//====================================================================================

string WStrToStr(const wstring& str)
{

string res(str.length(), ' '); 
std::copy(str.begin(), str.end(), res.begin());
return res;

}


//====================================================================================

bool DirectoryExists(const std::string& dirName){

int attr = GetFileAttributes(dirName.c_str()); 

//if (attr == 0xFFFFFFFF) 
//{ 
//    ulong error = GetLastError(); 
//    if (error == ERROR_FILE_NOT_FOUND) 
//		return false; // file not found     
//    else if (error == ERROR_PATH_NOT_FOUND) 
//		return false; // path not found 
//    else if (error == ERROR_ACCESS_DENIED) 
//		return false; // file or directory exists, but access is denied 
//    else 
//		return false; // some other error has occured 
//} 

if (attr & FILE_ATTRIBUTE_DIRECTORY) 
    return true;	
else 
    return false;	

}

//====================================================================================

bool FileExists(const std::string& fileName){

int rtn = GetFileAttributes(fileName.c_str());
return ((rtn != -1) && ((FILE_ATTRIBUTE_DIRECTORY & rtn) == 0));

}

//====================================================================================

void CreateDirectory(const std::string& dirName){

CreateDirectory(dirName.c_str(), NULL);

}

//====================================================================================

string ChangeFileExt(const std::string& pName, const std::string& pExt) {

if (!pName.length() || !pExt.length()) return pName;

string n = pName;
string e = pExt;

int pos = (int)n.find_last_of('.');
if (!pos) 
	return pName;
else
{ 
	int extLen = (int)n.length() - pos;
	n.replace(pos, pos + pExt.length(), pExt);
	return n;
}

};

//====================================================================================

string RenameFile(const std::string& pName, const std::string& pNewName) {

// To be coded
return "";

};

//====================================================================================

string RemoveFileExt(const std::string& pValue) {

if (!pValue.length()) return "";

string str = pValue;

int pos = (int)str.find('.');
if (!pos) 
	return "";
else
{ 
	int extLen = (int)str.length() - pos;
	str.resize(str.length() - extLen);
	return str;
}

}

//====================================================================================

string RemoveDrive(const std::string& pValue){

string s = pValue;
int pos = (int)s.find("\\");
if (pos != -1)
	return s.substr(pos+1);
else
	return pValue;

}

//====================================================================================

string RemoveFileExt(const char* pValue) {

if (pValue == "") return "";

string str = pValue;

int pos = (int)str.find('.');
if (pos==-1) 
	return str;
else
{ 
	int extLen = (int)str.length() - pos;
	str.resize(str.length() - extLen);
	return str;
}

}

//====================================================================================

wstring ExtractFileExt(const wstring& pValue){

if (!pValue.length() || pValue.find('.') == -1) return L"";

wstring str = pValue;

int pos = (int)str.find('.');
if (!pos) 
	return L"";
else 
	return str.substr(pos+1);

}

//====================================================================================

string ExtractFileExt(const std::string& pValue) {

if (!pValue.length() || pValue.find('.') == -1) return "";

string str = pValue;

int pos = (int)str.find('.');
if (!pos) 
	return "";
else 
	return str.substr(pos+1);

}

//====================================================================================

string ExtractFileExt(const char* pValue) {

if (!pValue) return "";

string str = pValue;

int pos = (int)str.find('.');
if (!pos) 
	return "";
else 
	return str.substr(pos+1);

}

//====================================================================================

void ProcessMessages()
{
#ifdef _WINDOWS
  MSG msg;
  while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#endif
} 

//====================================================================================

string ExtractFileName(const std::string& pValue) {

if (pValue == "") return "";

std::string str = pValue;

int pos = (int)str.find_last_of("\\");
//Msg(pos);
if (!pos) 
	return str;
else 
	return str.substr(pos+1);

}

//====================================================================================

std::string ExtractFileDir(const std::string& pValue) {

if (pValue == "") return "";

std::string str = pValue;

int pos = (int)str.find_last_of("\\");

if (!pos) 
	return str;
else 
	str.resize(pos);

pos = (int)str.find_last_of("\\");

if (!pos) 
	return str;
else 
	return str.substr(pos+1);


}

//====================================================================================

std::string ExtractDirPath(const std::string& pValue) {

if (pValue == "") return "";

std::string str = pValue;

int pos = (int)str.find_last_of("\\");

if (!pos) 
	return str;
else 
	str.resize(pos);


return str;

}

//====================================================================================




