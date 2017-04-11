#include "stdafx.h"
#include "StringConversion.h"

namespace StrConversion {

	bool UTF8_To_ANSI_String(const char* pszSrc, std::string &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, (int)strlen(pszSrc), NULL, 0);
		if (!nLen) {
			return false;
		}

		WCHAR *lpszBuf = new WCHAR[nLen + 1];
		nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, (int)strlen(pszSrc), lpszBuf, nLen);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = L'\0';
		strDes = (const char*)CW2A(lpszBuf,CP_ACP/*CP_UTF8*/);
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	bool UTF8_To_UNICODE_String(const char* pszSrc, std::wstring &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, (int)strlen(pszSrc), NULL, 0);
		if (!nLen) {
			return false;
		}

		WCHAR *lpszBuf = new WCHAR[nLen + 1];
		nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, (int)strlen(pszSrc), lpszBuf, nLen);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = L'\0';
		strDes = lpszBuf;
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	bool ANSI_To_UNICODE_String(const char* pszSrc, std::wstring &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, (int)strlen(pszSrc), NULL, 0);
		if (!nLen) {
			return false;
		}

		WCHAR *lpszBuf = new WCHAR[nLen + 1];
		nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, (int)strlen(pszSrc), lpszBuf, nLen);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = L'\0';
		strDes = lpszBuf;
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	bool ANSI_To_UTF8_String(const char* pszSrc, std::string &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = MultiByteToWideChar(CP_ACP/*CP_UTF8*/, 0, pszSrc, (int)strlen(pszSrc), NULL, 0);
		if (!nLen) {
			return false;
		}

		WCHAR *lpszBuf = new WCHAR[nLen + 1];
		nLen = MultiByteToWideChar(CP_ACP/*CP_UTF8*/, 0, pszSrc, (int)strlen(pszSrc), lpszBuf, nLen);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = L'\0';
		strDes = (const char*)CW2A(lpszBuf, CP_UTF8);
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	bool UNICODE_To_ANSI_String(const wchar_t* pszSrc, std::string &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = WideCharToMultiByte(CP_ACP, 0, pszSrc, (int)wcslen(pszSrc), NULL, 0,NULL,NULL);
		if (!nLen) {
			return false;
		}

		CHAR *lpszBuf = new CHAR[nLen + 1];
		nLen = WideCharToMultiByte(CP_ACP, 0, pszSrc, (int)wcslen(pszSrc), lpszBuf, nLen,NULL,NULL);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = '\0';
		strDes = lpszBuf;
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	bool UNICODE_To_UTF8_String(const wchar_t* pszSrc, std::string &strDes)
	{
		strDes.clear();
		_ATLTRY
		{
			int nLen = WideCharToMultiByte(CP_UTF8, 0, pszSrc, (int)wcslen(pszSrc), NULL, 0,NULL,NULL);
		if (!nLen) {
			return false;
		}

		CHAR *lpszBuf = new CHAR[nLen + 1];
		nLen = WideCharToMultiByte(CP_UTF8, 0, pszSrc, (int)wcslen(pszSrc), lpszBuf, nLen,NULL,NULL);
		if (!nLen) {
			delete[] lpszBuf;
			return false;
		}
		lpszBuf[nLen] = '\0';
		strDes = lpszBuf;
		delete[] lpszBuf;
		}
			_ATLCATCHALL()
		{
			return false;
		}
		return true;
	}

	void UTF8_To_CString(const char* input, CString& output)
	{
		std::string str;
		std::wstring wstr;
		if (sizeof(TCHAR) == 1) {
			UTF8_To_ANSI_String(input, str);
			output = str.c_str();
		}

		else {
			UTF8_To_UNICODE_String(input, wstr);
			output = wstr.c_str();
		}
		//output.Format(_T("%s"),CA2W(str.c_str())); //error
	}

	void CString_To_UTF8(CString input, std::string &strDes)
	{
		if (sizeof(TCHAR) == 1) {
			ANSI_To_UTF8_String((char*)input.GetBuffer(), strDes);
		}
		else {
			UNICODE_To_UTF8_String(input, strDes);
		}
	}


	void ANSI_To_CString(const char* input, CString& output)
	{
		std::string str;
		std::wstring wstr;
		if (sizeof(TCHAR) == 1) {
			output = str.c_str();
		}
		else {
			ANSI_To_UNICODE_String(input, wstr);
			output = wstr.c_str();
		}
	}

	void CString_To_ANSI(CString input, std::string &strDes)
	{
		if (sizeof(TCHAR) == 1) {
			strDes = (char*)input.GetBuffer();
		}
		else {
			UNICODE_To_ANSI_String(input, strDes);
		}
	}
}