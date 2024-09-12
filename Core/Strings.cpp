

//=============================
//		Strings.cpp
//=============================


#include <stdafx.h>
#pragma hdrstop

#include "Engine.h"
#include "Strings.h"
#include "SystemX.h"
#include <sstream>

Strings str; // Global instance

//====================================================================================


string Strings::IntToStr(const int& i){

std::ostringstream o;
if (!(o << i))
     return "";
   return o.str();

}

//====================================================================================

wstring Strings::IntToWStr(const int& i){

std::wstringstream o;
if (!(o << i))
     return L"";
   return o.str();

}

//====================================================================================

string Strings::FloatToStr(const float& i){

std::ostringstream o;
if (!(o << i))
     return "";
   return o.str();

}

//====================================================================================

int Strings::StrToInt(const string& str){

return atoi(str.c_str());

}

//====================================================================================

string Strings::CharTrimToStr(char* chr){

string str = chr; 

str.erase(str.find_last_not_of(0x20)+1);
str.erase(0, str.find_first_not_of(0x20));
return str;

}

//==============================================================================

void Strings::Trim(string& strng){

size_t start = strng.find_first_not_of(0x0A);
size_t end = strng.find_last_not_of(0x0A);

if (strng.npos != start && strng.npos != end && end != strng.length()-1)
	strng = strng.substr(start, end-start);

start = strng.find_first_not_of(" \t");
end = strng.find_last_not_of(" \t");

if (strng.npos != start && strng.npos != end && end != strng.length()-1)
	strng = strng.substr(start, end-start);

}

//====================================================================================

void Strings::Trim(char* chr){ // Not working perfectly?

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

string Strings::ToLower(const string& str){

string s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = tolower(s[i]); 

return s;

}

//====================================================================================

wstring Strings::ToLower(const wstring& str){

wstring s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = tolower(s[i]); 

return s;

}

//====================================================================================

string Strings::ToUpper(const string& str){

string s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = toupper(s[i]); 

return s;

}

//====================================================================================

wstring Strings::ToUpper(const wstring& str) {

wstring s = str;
for (int i = 0; i < (int)s.length(); ++i) 
	s[i] = toupper(s[i]); 

return s;

}

//====================================================================================

string Strings::ReplaceAll(const string& str, const string& orig, const string& repl){

if ((int)str.find(orig) == -1 || str.empty() || orig.empty() || repl.empty()) 
	return str;

string s = str;

while (1)
{
	size_t p = s.find(orig);
	if (p == s.npos)
		break;
	s.replace(p, repl.size(), repl);
	p += repl.size();
}

return s;

}

//====================================================================================

wchar_t* Strings::CharToWChar(const char* chr){

// Memory leak!!!
int	length = MultiByteToWideChar(CP_ACP, 0, chr, -1, 0, 0);		
WCHAR* wfilename = new wchar_t[length];	

MultiByteToWideChar(CP_ACP, 0, chr, -1, wfilename, length);
return wfilename;

}

//====================================================================================

wstring Strings::CharToWStr(const char* chr){

int length = (int)strlen(chr);
wstring res(length, ' ');
//wstring res(length, L'');
for (int i = 0; i < length; ++i)
	res[i] = cin.widen(chr[i]);

return res;

}

//====================================================================================

wstring Strings::StrToWStr(const string& str){

wstring res(str.length(), ' ');
//wstring res(str.length(), L'');
std::copy(str.begin(), str.end(), res.begin());
return res;

}

//====================================================================================

string Strings::WStrToStr(const wstring& str){

string res(str.length(), ' '); 
std::copy(str.begin(), str.end(), res.begin());
return res;

}

//====================================================================================

string Strings::ChangeFileExt(const std::string& pName, const std::string& pExt) {

if (!pName.length() || !pExt.length()) return pName;

string n = pName;
string e = pExt;

int pos = (int)n.find_last_of('.');
if (pos == -1) 
	return pName;
else
{
	n.replace(pos, pos + pExt.length(), pExt);
	return n;
}

}

//====================================================================================

string Strings::RenameFile(const std::string& pName, const std::string& pNewName){

// To be coded
return "";

}

//====================================================================================

string Strings::RemoveFileExt(const char* pValue){

if (pValue == "") return "";

string s = pValue;

int pos = (int)s.find('.');
if (pos == -1)
	return s;
else
{
	int extLen = (int)s.length() - pos;
	s.resize(s.length() - extLen);
	return s;
}

}

//====================================================================================

string Strings::RemoveFileExt(const std::string& pValue){

if (!pValue.length()) return "";

string s = pValue;

int pos = (int)s.find('.');
if (pos == -1)
	return pValue;
else
{ 
	int extLen = (int)s.length() - pos;
	s.resize(s.length() - extLen);
	return s;
}

}

//====================================================================================

//String Strings::RemoveFileExt(const String& pValue){
//
//if (!pValue.Length()) return "";
//
//String str = pValue;
//
//int pos = (int)str.AnsiPos('.');
//if (!pos)
//	return pValue;
//else
//{
//	int extLen = (int)str.Length() - pos+1;
//	str.SetLength(str.Length() - extLen);
//	return str;
//}
//
//}

//====================================================================================

string Strings::RemoveDrive(const std::string& pValue){

string s = pValue;
int pos = (int)s.find("\\");
if (pos != -1)
	return s.substr(pos+1);
else
	return pValue;

}

//====================================================================================

wstring Strings::ExtractFileExt(const wstring& pValue){

if (!pValue.length() || (int)pValue.find('.') == -1) return L"";

wstring s = pValue;

int pos = (int)s.find('.');
if (pos == -1)
	return L"";
else 
	return s.substr(pos+1);

}

//====================================================================================

string Strings::ExtractFileExt(const std::string& pValue){

if (!pValue.length() || (int)pValue.find('.') == -1) return "";

string s = pValue;

int pos = (int)s.find('.');
if (pos == -1)
	return "";
else 
	return s.substr(pos+1);

}

//====================================================================================

string Strings::ExtractFileExt(const char* pValue){

if (!pValue) return "";

string s = pValue;

int pos = (int)s.find('.');
if (pos == -1)
	return "";
else 
	return s.substr(pos+1);

}

//====================================================================================

string Strings::ExtractFileName(const std::string& pValue){

if (pValue == "") return "";

std::string s = pValue;

int pos = (int)s.find_last_of("\\");

if (pos == -1)
	return s;
else 
	return s.substr(pos+1);

}

//====================================================================================

std::string Strings::ExtractFileDir(const std::string& pValue){

if (pValue == "") return "";

std::string s = pValue;

int pos = (int)s.find_last_of("\\");

if (pos == -1)
	return s;
else 
	s.resize(pos);

pos = (int)s.find_last_of("\\");

if (pos == -1)
	return s;
else 
	return s.substr(pos+1);


}

//====================================================================================

std::string Strings::ExtractDirPath(const std::string& pValue){

if (pValue == "") return "";

std::string s = pValue;

int pos = (int)s.find_last_of("\\");

if (pos == -1)
	return s;
else
	s.resize(pos);

return s;

}

//==============================================================================

string Strings::GetFileExt(const string& name){

string s = name;
int dotPos = (int)s.find_last_of(".");
if (dotPos != -1)
	return s.erase(0, dotPos+1);
else
	return s;

}

//==============================================================================

string Strings::GetFileName(const string& name){

int slash = (int)name.find_last_of("\\");
if (slash == -1)
	slash = (int)name.find_last_of("/");

if (slash){
	string fn = name;
	fn.erase(1, slash);
	fn = RemoveFileExt(fn);
	return fn;
}

return "";

}

//==============================================================================

bool Strings::MaxPathClip(string& path){

if (path.length() <= 265)
	return false;

path.resize(365);
return true;

}

//==============================================================================

string Strings::FormatTime(const float& time){

if (time <= 0 || time == 0xfffffff) return "";

int mins = (int)time / 60;
int secs = (int)time % 60;
char zero1[] = "0";
char zero2[] = "0";

if (mins > 9)
	*zero1 = NULL;
if (secs > 9)
	*zero2 = NULL;

return zero1 + str.IntToStr(mins) + ":" + zero2 + str.IntToStr(secs);

}

//==============================================================================

string Strings::SizeToStr(const int& size){

int dot;
string s;

if (size >= 1073741824) // GB - Untested
{
	s = str.FloatToStr(float (size) / 1073741824);
	dot = (int)s.find(".");
	if (dot != -1)
		s.resize(dot + 2);
	return s + "GB";
}
else if (size >= 1048576) // MB
{
	s = str.FloatToStr(float (size) / 1048576);
	dot = (int)s.find(".");
	if (dot != -1)
		s.resize(dot + 2);
	return s + "MB";
}
else if (size >= 1024) // KB
{
	s = str.FloatToStr(float (size) / 1024);
	dot = (int)s.find(".");
	if (dot != -1)
		s.resize(dot);
	return s + "KB";
}
else if (size > 0) // bytes
{
	s = str.FloatToStr(float (size));
	dot = (int)s.find(".");
	if (dot != -1)
		s.resize(dot - 1);
		
	if (size == 1)
		return s + "byte";
	else
		return s + "bytes";
}
else
	return "0bytes";

}

//==============================================================================

bool Strings::IllegalCharRemover(string& value){

int x, ogLen = (int)value.length();

// Remove all illegal characters /\*<>|":
while ( (x = (int)value.find("\\")) || (x = (int)value.find("*")) || (x = (int)value.find("<"))
		|| (x = (int)value.find(">")) || (x = (int)value.find("|")) || (x = (int)value.find("\""))
		|| (x = (int)value.find(":")) )
{
	value[x-1] = '?';
}
while (x = (int)value.find("?"))
	value.erase(x, 1);

if (ogLen == (int)value.length()) // No char were removed
	return 0;

return 1;

}

//==============================================================================

void Strings::StrClean(string& value){

str.Trim(value);
value = str.ToLower(value);


// Remove '-' & ' ' & '/' from start
if (value.find("_") == 0) value.erase (value.find("_"), 1);
if (value.find("/") == 0) value.erase (value.find("/"), 1);
if (value.find(" ") == 0) value.erase (value.find(" "), 1);

int x;


// Remove '_' after every '('
while ((x = (int)value.find("(_")) != -1)
{
	value.erase(x+1, 1);
}


// Remove '_' before every ')'
while ((x = (int)value.find("_)")) != -1)
{
	value.erase(x, 1);
}


// Change all '_' to ' '
while ((x = (int)value.find("_")) != -1 )
	value[x] = ' ';

// Change all '/' to ' \\ '
while ((x = (int)value.find("/")) != -1 )
{
	//value.erase (x, 1);
	value[x] = '\\';
}


// Remove all illegal characters /\*<>|":
while ( ((x = (int)value.find("*")) != -1 ) || ((x = (int)value.find("<")) != -1 ) || ((x = (int)value.find(">")) != -1 )
	|| ((x = (int)value.find("|")) != -1 )  || ((x = (int)value.find(":")) != -1 ) )
//while ( ((x = (int)value.find("\\")) != -1 ) || ((x = (int)value.find("*")) != -1 ) || ((x = (int)value.find("<")) != -1 )
//		|| ((x = (int)value.find(">")) != -1 ) || ((x = (int)value.find("|")) != -1 ) || ((x = (int)value.find("\"")) != -1 )
//		|| ((x = (int)value.find(":")) != -1 ) )
{
	value.erase (x, 1);
}


// Remove all spaces > 1
while ((x = (int)value.find("  ")) != -1 )
	value.erase (x, 1);


// Capitalize letters after every ' '
while ((x = (int)value.find(" ")) != -1 )
{
	value[x] = '?';
	value[x+1] = toupper(value[x+1]);
}
while ((x = (int)value.find("?")) != -1)
	value[x] = ' ';


// Capitalize letters after every ' \\ '
while ((x = (int)value.find("\\")) != -1 )
{
	value[x] = '?';
	value[x+1] = toupper(value[x+1]);
}
while ((x = (int)value.find("?")) != -1 )
	value[x] = '\\';

// Capitalize letters after every '('
while ((x = (int)value.find("(")) != -1 )
{
	value[x] = '?';
	value[x+1] = toupper(value[x+1]);
}
while ((x = (int)value.find("?")) != -1 )
	value[x] = '(';

// Capitalize letters after every ' ' '
while ((x = (int)value.find("'")) != -1 )
{
	value[x] = '?';
	value[x+1] = toupper(value[x+1]);
}
while ((x = (int)value.find("?")) != -1 )
	value[x] = '\'';


// Check again to make sure that no space exists at the start
if (value.find(" ") == 0) value.erase (value.find(" "), 1);
      
// InitCaps
value[0] = toupper(value[0]);

}// End CleanName

//==============================================================================

string Strings::StringReplacer(const string& text, const int& x,
											const int& maxNum){

string fileName = FItem.FileNameBak[x];

string NewText = ReplaceAll(text, "%f", fileName);

NewText = ReplaceAll(NewText, "%a", CItem.DirName);

return ReplaceAll(NewText, "%n", AutoNumBasic(x, maxNum));

}

//==============================================================================

string Strings::AutoNumBasic (const int& num, const int& maxNum){

int prefixLen = 0;

if (maxNum > 9999)
	prefixLen = 4;
else if (maxNum > 999)
	prefixLen = 3;
else if (maxNum > 99)
	prefixLen = 2;
else if (maxNum > 9)
	prefixLen = 1;
else if (maxNum >= 1)
	prefixLen = 0;

string numS = "0000";

if (num+1 < 10){
	numS.resize(prefixLen);
	numS += str.IntToStr(num+1);
}
else if (num+1 < 100){
	numS.resize(prefixLen-1);
	numS += str.IntToStr(num+1);
}
else if (num+1 < 1000){
	numS.resize(prefixLen-2);
	numS += str.IntToStr(num+1);
}
else if (num+1 < 10000){
	numS.resize(prefixLen-3);
	numS += str.IntToStr(num+1);
}
else if (num+1 < 100000){
	numS.resize(prefixLen-4);
	numS += str.IntToStr(num+1);
}

return numS;

}

//==============================================================================

void Strings::AutoNum (const string& prefix, string& name, const int& num,
									const string& extension){

if (num < 10)
	name = prefix + " - 0000" + str.IntToStr(num) + " " + extension;
else if (num < 100)
	name = prefix + " - 000" + str.IntToStr(num) + " " + extension;
else if (num < 1000)
	name = prefix + " - 00" + str.IntToStr(num) + " " + extension;
else if (num < 10000)
	name = prefix + " - 0" + str.IntToStr(num) + " " + extension;
else
	name = prefix + " - " + str.IntToStr(num) + " " + extension;

}

//==============================================================================

void Strings::AutoNumList (const bool& tidy, string& name,
									const int& num, const int& maxNum){

string prefix = str.ExtractFileName(CItem.OpenFileName);
prefix.resize(prefix.length() - sys.GetOpenFileNameExtLen()-1);

if (tidy) StrClean(prefix);

if (num+1 < 10)
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 100)
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 1000)
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 10000)
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];

}

//==============================================================================

void Strings::AutoNumScx (const bool& tidy, string& name,
									const int& num, const int& maxNum){

string prefix = ExtractFileName(CItem.OpenFileName);
prefix.resize(prefix.length() - sys.GetOpenFileNameExtLen()-1);

if (tidy)
	StrClean(prefix);

if (num+1 < 10)
	name = prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 100)
	name =   prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 1000)
	name =  prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else if (num+1 < 10000)
	name =   prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];
else
	name =  prefix + " - " + AutoNumBasic(num, maxNum) + FItem.Ext[num];

}

//==============================================================================



