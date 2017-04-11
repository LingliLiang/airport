#pragma once

#include <string>
#include <atldef.h>
#include <Windows.h>
#include <atlbase.h>
#include <atlstr.h>
/**
一些常用的字符转换函数
**/
namespace StrConversion {

	bool UTF8_To_ANSI_String(const char* pszSrc, std::string &strDes);

	bool UTF8_To_UNICODE_String(const char* pszSrc, std::wstring &strDes);

	bool ANSI_To_UNICODE_String(const char* pszSrc, std::wstring &strDes);

	bool ANSI_To_UTF8_String(const char* pszSrc, std::string &strDes);

	bool UNICODE_To_ANSI_String(const wchar_t* pszSrc, std::string &strDes);

	bool UNICODE_To_UTF8_String(const wchar_t* pszSrc, std::string &strDes);

	void UTF8_To_CString(const char* input, CString& output);

	void CString_To_UTF8(CString input, std::string &strDes);

	void ANSI_To_CString(const char* input, CString& output);

	void CString_To_ANSI(CString input, std::string &strDes);

}