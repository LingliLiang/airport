// AirPlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AirPlay.h"
#include"log\SimpleLog.h"

// 这是导出变量的一个示例
AIRPLAY_API int nAirPlay=0;

// 这是导出函数的一个示例。
AIRPLAY_API int fnAirPlay(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 AirPlay.h
CAirPlay::CAirPlay()
{
	return;
}

#include "network\ZeroconfMDNS.h"
#include "network\AsyncSelectWin32.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	SPSTARTLOGD;
	CAsyncSelectWin32 ss;
	CZeroconfMDNS zerocfg;
	zerocfg.Start();
	std::string id = "APlayTest01";
	std::string type = "_airplay._tcp";
	std::string name = "APlayTest01";
	int port = 9921;
	zerocfg.PublishService(id,type,name,port,std::vector<std::pair<std::string, std::string> >());
	Sleep(1000);
	::MessageBoxA(NULL,"Zeroconf Test","Zeroconf",0);
	zerocfg.Stop();
	ss.Close();
	Sleep(1000);
	SPSTOPLOG;
	return 0;
}