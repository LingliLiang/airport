#pragma once



#include "system.h"
#include <string>


class CWin32Util
{
public:
	static std::string WUSysMsg(DWORD dwError);
};