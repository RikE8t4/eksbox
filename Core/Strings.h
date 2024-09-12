#ifndef STRINGS_H
#define STRINGS_H

//=============================
//		Strings.h
//=============================

#include <stdafx.h>
#pragma hdrstop

/*
#define CLIPSTRING(x) if (x.Length() >= 32){\
						x.SetLength(32);\
						x = x + "...";\
						}
*/

class Strings{

	public:

		#ifdef __BORLANDC__
		std::string AnsiToStd(const String& str) { return std::string(str.c_str()); }
		String StdToAnsi(const std::string& str) { return String(str.c_str()) ; }
		#endif
		string IntToStr(const int& i);
		wstring IntToWStr(const int& i);
		std::string FloatToStr(const float& i);
		int StrToInt(const string& str);
		string CharTrimToStr(char* chr);
		void Trim(string& strng);
		void Trim(char* chr);
		std::string ToLower(const string& str);
		wstring ToLower(const wstring& str);
		string ToUpper(const string& str);
		wstring ToUpper(const wstring& str);
		std::string ReplaceAll(const string& str, const string& orig, const string& repl);
		wchar_t* CharToWChar(const char* chr);
		wstring CharToWStr(const char* chr);
		wstring StrToWStr(const string& str);
		std::string WStrToStr(const wstring& str);
		std::string ChangeFileExt(const std::string& pName, const std::string& pExt);
		std::string RenameFile(const std::string& pName, const std::string& pNewName);
		std::string RemoveDrive(const std::string& pValue);
		std::string RemoveFileExt(const char* pValue);
		std::string RemoveFileExt(const std::string& pValue);
		//String RemoveFileExt(const String& pValue);
		wstring ExtractFileExt(const wstring& pValue);
		std::string ExtractFileExt(const std::string& pValue);
		std::string ExtractFileExt(const char* pValue);
		std::string ExtractFileName(const std::string& pValue);
		std::string ExtractFileDir(const std::string& pValue);
		std::string ExtractDirPath(const std::string& pValue);
		std::string GetFileExt(const string& name);
		std::string GetFileName(const string& name);
		bool MaxPathClip(string& path);
		std::string FormatTime(const float& time);
		std::string SizeToStr(const int& size);
		bool IllegalCharRemover(string& value);
		void StrClean(string& value);
		std::string StringReplacer(const string& text, const int& x,
								const int& maxNum);

		std::string AutoNumBasic (const int& num, const int& maxNum);

		void AutoNum (const string& prefix, string& name, const int& num,
			const string& extension);

		void AutoNumList (const bool& tidy, string& name,
			const int& num, const int& maxNum);

		void AutoNumScx (const bool& tidy, string& name,
			const int& num, const int& maxNum);

};// End Strings

extern Strings str; // Global instance

#endif
