#pragma once



#include "stdafx.h"
#include "win32utils.h"
#include "../utils/StringConversion.h"

std::string CWin32Util::WUSysMsg(DWORD dwError)
{
#define SS_DEFLANGID MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT)
	CHAR szBuf[512] = {0};

	if ( 0 != ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
		SS_DEFLANGID, szBuf, 511, NULL) )
		return StrConversion::StrFormat("%s (0x%X)", szBuf, dwError);
	else
		return StrConversion::StrFormat("Unknown error (0x%X)", dwError);
}