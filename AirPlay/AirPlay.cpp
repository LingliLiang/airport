// AirPlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AirPlay.h"
#include"log\SimpleLog.h"
#include "network\ZeroconfMDNS.h"
#include "network\AsyncSelectWin32.h"
#include "utils\StringConversion.h"

struct SettingPair
{
	unsigned int type;
	char* value;
};

SettingPair allDefaultSettings[]= {
	{CSettings::SETTING_START,""},
	{CSettings::SETTING_SERVICES_ZEROCONF,"1"},
	{CSettings::SETTING_SERVICES_AIRPLAY,"1"},
	{CSettings::SETTING_SERVICES_AIRPLAY_DEVICENAME,"AirFake"},
	{CSettings::SETTING_SERVICES_AIRPLAY_VIDEOSUPPORT,"1"},
	{CSettings::SETTING_SERVICES_AIRPLAY_PORT,"9648"},
	{CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD,""},
	{CSettings::SETTING_SERVICES_AIRPLAYPASSWORD,""},
	{CSettings::SETTING_SERVICES_AIRPLAYVOLUMECONTROL,"0"},
	{CSettings::SETTING_END,""}
};

static CSettings::tSettings create_settings_map()
{
	size_t count = _countof(allDefaultSettings);
	CSettings::tSettings settings;
	for (size_t i = 0; i < count; ++i)
	{
		settings.insert(std::make_pair( allDefaultSettings[i].type, allDefaultSettings[i].value));
	}
	return settings;
}

CSettings::CSettings()
	:m_settings(create_settings_map())
{}

CSettings::~CSettings()
{
}

bool CSettings::TypeOut(typeSettings type)
{
	if(type==SETTING_START||type >=SETTING_END)
		return true;
	return false;
}

bool CSettings::GetBool(typeSettings type)
{
	if(TypeOut(type)){
		assert(false);
		return false;
	}
	return StrConversion::CompareNoCase(m_settings[type].c_str(),"1") == 0;
}

int CSettings::GetInt(typeSettings type)
{
		if(TypeOut(type)){
		assert(false);
		return 0;
	}
	return atoi(m_settings[type].c_str());
}

unsigned long CSettings::GetULong(typeSettings type)
{
		if(TypeOut(type)){
		assert(false);
		return 0;
	}
	return atol(m_settings[type].c_str());
}

std::string CSettings::GetString(typeSettings type)
{
	if(TypeOut(type)){
		assert(false);
		return "";
	}
	return m_settings[type].c_str();
}




/////////////////////////////////////////////////////////////////////////////////////////////////

CAirPlay::CAirPlay()
	:m_msgwin(new CAsyncSelectWin32()),m_network(new CNetworkWin32),m_settings(new CSettings)
{
	SPSTARTLOGD;
	if(!m_network || !m_settings)
	{
		SPLOGA(LOG_ERROR, "CAirPlay: Network Instance failed!");
	}
}

CAirPlay::~CAirPlay()
{
	if(m_msgwin)m_msgwin->Close();
	if(m_network) delete m_network;
	m_network = NULL;
	if(m_settings) delete m_settings;
	m_settings = NULL;
	if(m_msgwin) delete m_msgwin;
	m_msgwin = NULL;
	SPSTOPLOG;
}

CNetwork& CAirPlay::getNetwork()
{
	return *m_network;
}

CSettings& CAirPlay::getSettings()
{
	return *m_settings;
}


bool CAirPlay::StartServices()
{
	m_network->NetworkMessage(CNetwork::SERVICES_UP, 0);
	SPLOGA(LOG_ERROR, "AIRPLAY Server: Network start failed!");
	return true;
}

bool CAirPlay::StopServices()
{
	m_network->NetworkMessage(CNetwork::SERVICES_DOWN, 0);
	return true;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	Sleep(2000);
	g_application.StartServices();
	::MessageBoxA(NULL,"Zeroconf Test","Zeroconf",0);
	//HANDLE env;
	//env = ::CreateEventA(0,FALSE,FALSE,NULL);
	//::WaitForSingleObject(env,60000);
	//::CloseHandle(env);
	g_application.StopServices();
	Sleep(2000);
	return 0;
}