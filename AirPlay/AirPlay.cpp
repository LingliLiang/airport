// AirPlay.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "AirPlay.h"
#include"log\SimpleLog.h"

// ���ǵ���������һ��ʾ��
AIRPLAY_API int nAirPlay=0;

// ���ǵ���������һ��ʾ����
AIRPLAY_API int fnAirPlay(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� AirPlay.h
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