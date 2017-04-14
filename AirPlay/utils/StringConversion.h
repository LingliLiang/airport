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

	static void Unknown_To_UTF8(std::string &strDes);

	static std::string FormatV(const char *fmt, va_list args);
	static std::wstring FormatV(const wchar_t *fmt, va_list args);	
	static std::string StrFormat(const char *fmt, ...);
	static std::wstring StrFormat(const wchar_t *fmt, ...);

	static void ToUpper(std::string &str);
	static void ToUpper(std::wstring &str);
	static void ToLower(std::string &str);
	static void ToLower(std::wstring &str);

	static std::vector<std::string> Split(const std::string& input, const std::string& delimiter, unsigned int iMaxStrings = 0);
	static std::vector<std::string> Split(const std::string& input, const char delimiter, size_t iMaxStrings = 0);
	static std::vector<std::string> Split(const std::string& input, const std::vector<std::string> &delimiters);


	static int Replace(std::string &str, char oldChar, char newChar);
	static int Replace(std::wstring &str, wchar_t oldChar, wchar_t newChar);
	static int Replace(std::string &str, const std::string &oldStr, const std::string &newStr);
	static int Replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr);

	static bool EqualsNoCase(const std::string &str1, const std::string &str2);
	static bool EqualsNoCase(const std::string &str1, const char *s2);
	static bool EqualsNoCase(const char *s1, const char *s2);
	static int  CompareNoCase(const std::string &str1, const std::string &str2);
	static int  CompareNoCase(const char *s1, const char *s2);

	static bool StartsWith(const std::string &str1, const std::string &str2);
	static bool StartsWith(const std::string &str1, const char *s2);
	static bool StartsWith(const char *s1, const char *s2);
	static bool StartsWithNoCase(const std::string &str1, const std::string &str2);
	static bool StartsWithNoCase(const std::string &str1, const char *s2);
	static bool StartsWithNoCase(const char *s1, const char *s2);
	static bool EndsWith(const std::string &str1, const std::string &str2);
	static bool EndsWith(const std::string &str1, const char *s2);
	static bool EndsWithNoCase(const std::string &str1, const std::string &str2);
	static bool EndsWithNoCase(const std::string &str1, const char *s2);

	static std::string Left(const std::string &str, size_t count);
	static std::string Mid(const std::string &str, size_t first, size_t count = std::string::npos);
	static std::string Right(const std::string &str, size_t count);
	static std::string& Trim(std::string &str);
	static std::string& Trim(std::string &str, const char* const chars);
	static std::string& TrimLeft(std::string &str);
	static std::string& TrimLeft(std::string &str, const char* const chars);
	static std::string& TrimRight(std::string &str);
	static std::string& TrimRight(std::string &str, const char* const chars);

	/* The next several isasciiXX and asciiXXvalue functions are locale independent (US-ASCII only),
	* as opposed to standard ::isXX (::isalpha, ::isdigit...) which are locale dependent.
	* Next functions get parameter as char and don't need double cast ((int)(unsigned char) is required for standard functions). */
	inline static bool isasciidigit(char chr) // locale independent 
	{
		return chr >= '0' && chr <= '9'; 
	}
	inline static bool isasciixdigit(char chr) // locale independent 
	{
		return (chr >= '0' && chr <= '9') || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F'); 
	}
	static int asciidigitvalue(char chr); // locale independent 
	static int asciixdigitvalue(char chr); // locale independent 
	inline static bool isasciiuppercaseletter(char chr) // locale independent
	{
		return (chr >= 'A' && chr <= 'Z'); 
	}
	inline static bool isasciilowercaseletter(char chr) // locale independent
	{
		return (chr >= 'a' && chr <= 'z'); 
	}
	inline static bool isasciialphanum(char chr) // locale independent
	{
		return isasciiuppercaseletter(chr) || isasciilowercaseletter(chr) || isasciidigit(chr); 
	}
};