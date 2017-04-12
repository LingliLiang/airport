#pragma once

#include <string>
#include <atldef.h>
#include <atlbase.h>
#include <atlstr.h>
#include <vector>
/**
一些常用的字符转换函数
**/
class StrConversion {
public:
	static bool UTF8_To_ANSI_String(const char* pszSrc, std::string &strDes);

	static bool UTF8_To_UNICODE_String(const char* pszSrc, std::wstring &strDes);

	static bool ANSI_To_UNICODE_String(const char* pszSrc, std::wstring &strDes);

	static bool ANSI_To_UTF8_String(const char* pszSrc, std::string &strDes);

	static bool UNICODE_To_ANSI_String(const wchar_t* pszSrc, std::string &strDes);

	static bool UNICODE_To_UTF8_String(const wchar_t* pszSrc, std::string &strDes);

	static void UTF8_To_CString(const char* input, CString& output);

	static void CString_To_UTF8(CString input, std::string &strDes);

	static void ANSI_To_CString(const char* input, CString& output);

	static void CString_To_ANSI(CString input, std::string &strDes);

	static std::string FormatV(const char *fmt, va_list args);
	static std::wstring FormatV(const wchar_t *fmt, va_list args);	
	static std::string StrFormat(const char *fmt, ...);
	static std::wstring StrFormat(const wchar_t *fmt, ...);

	static void ToUpper(std::string &str);
	static void ToUpper(std::wstring &str);
	static void ToLower(std::string &str);
	static void ToLower(std::wstring &str);

	static std::vector<std::string> Split(const std::string &str, const char* token);
	static std::vector<std::wstring> Split(const std::wstring &str, const wchar_t* token);

	static int Replace(std::string &str, char oldChar, char newChar);
	static int Replace(std::wstring &str, wchar_t oldChar, wchar_t newChar);
	static int Replace(std::string &str, const std::string &oldStr, const std::string &newStr);
	static int Replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr);

	static bool EqualsNoCase(const std::string &str1, const std::string &str2);
	static bool EqualsNoCase(const std::string &str1, const char *s2);
	static bool EqualsNoCase(const char *s1, const char *s2);
	static int  CompareNoCase(const std::string &str1, const std::string &str2);
	static int  CompareNoCase(const char *s1, const char *s2);
};