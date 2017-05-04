// AirPlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "AirPlay.h"
#include"log\SimpleLog.h"
#include "network\ZeroconfMDNS.h"
#include "network\AsyncSelectWin32.h"
#include "utils\StringConversion.h"

// AirPlay
const char* FeaturesKey::FAP_Model = "model";
const char* FeaturesKey::FAP_Features="features";
const char* FeaturesKey::FAP_Srcvers = "srcvers";
const char* FeaturesKey::FAP_Flags = "flags";
//AirTunes
const char* FeaturesKey::FAT_am = "am";
const char* FeaturesKey::FAT_cn = "cn";
const char* FeaturesKey::FAT_da = "da";
const char* FeaturesKey::FAT_et = "et";
const char* FeaturesKey::FAT_ft = "ft";
const char* FeaturesKey::FAT_md = "md";
const char* FeaturesKey::FAT_pk = "pk";
const char* FeaturesKey::FAT_sf = "sf";
const char* FeaturesKey::FAT_tp = "tp";
const char* FeaturesKey::FAT_vn = "vn";
const char* FeaturesKey::FAT_vs = "vs";
const char* FeaturesKey::FAT_vv = "vv";

class CAirFeatures
{
public:
	typedef std::map<	std::string , std::string> tFeatures;
	typedef std::map<	std::string , std::string>::iterator tFeaturesIter;
	tFeatures m_fap;
	tFeatures m_fat;
	const char* GetKey(const char* key)
	{
		tFeaturesIter it = m_fap.find(key);
		if(it!=m_fap.end())
		{
			return it->second.c_str();
		}
		it = m_fat.find(key);
		if(it!=m_fat.end())
		{
			return it->second.c_str();
		}
		return "";
	}
	CAirFeatures()
	{
		char* pk = "0c4c7dbe9d16f05505e974686cb5e3cd6bbf4b0f3ce76abc000f112aa803262b";
		char* vs = "220.68";
		char* am = "AppleTV5,3";
		char* ft = "0x27FB";
		char* sf = "0x4";
		m_fap.insert(std::make_pair(FAP_Model,am));
		m_fap.insert(std::make_pair(FAP_Features,ft));
		m_fap.insert(std::make_pair(FAP_Flags,sf));
		m_fap.insert(std::make_pair(FAP_Srcvers,vs));
		m_fap.insert(std::make_pair(FAT_pk,pk));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_fat.insert(std::make_pair(FAT_am,am)); //device model

		//0 - PCM  1 - Apple Lossless (ALAC)  2 - AAC  3 - AAC ELD (Enhanced Low Delay)
		m_fat.insert(std::make_pair(FAT_cn,"0,1,2,3")); // 	audio codecs

		//0 - no encryption  1 - RSA (AirPort Express)  2 - ?  3 - FairPlay 4 - MFiSAP (3rd-party devices) 5 -  	FairPlay SAPv2.5
		m_fat.insert(std::make_pair(FAT_et,"0")); // 	Encryption Types

		m_fat.insert(std::make_pair(FAT_ft,ft)); // 	features types ??

		//0 - text  1 - artwork  2 - progress 
		m_fat.insert(std::make_pair(FAT_md,"0,1,2")); // supported metadata types

		m_fat.insert(std::make_pair(FAT_tp,"UDP")); // 	supported transport: TCP or UDP

		m_fat.insert(std::make_pair(FAT_cn,"0,1,2,3")); // 	audio codecs

		m_fat.insert(std::make_pair(FAT_vs,vs)); // 	server version 

		m_fat.insert(std::make_pair(FAT_da,"true")); // 	?
		m_fat.insert(std::make_pair(FAT_pk,pk)); // 	p key ?
		m_fat.insert(std::make_pair(FAT_sf,sf)); // ?
		m_fat.insert(std::make_pair(FAT_vn,"65537")); // ?
		m_fat.insert(std::make_pair(FAT_vv,"2")); // ?

	}
};

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
	{CSettings::SETTING_SERVICES_AIRTUNES_PORT,"9649"},
	{CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD,"0"},
	{CSettings::SETTING_SERVICES_AIRPLAYPASSWORD,""},
	{CSettings::SETTING_SERVICES_AIRPLAYVOLUMECONTROL,"0"},
	{CSettings::SETTING_LOGAIRTUNES,"1"},
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
	:m_msgwin(new CAsyncSelectWin32()),
	m_network(new CNetworkWin32),
	m_settings(new CSettings),
	m_features(new CAirFeatures)
{
	SPSTARTLOGD;
	if(!m_network || !m_settings)
	{
		SPLOGA(LOG_ERROR, "CAirPlay: Network Instance failed!");
	}
}

CAirPlay::~CAirPlay()
{
	if(m_network) delete m_network;
	m_network = NULL;
	if(m_settings) delete m_settings;
	m_settings = NULL;
	if(m_features) delete m_features;
	m_features = NULL;
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

const char* CAirPlay::getFeatures(const char* name)
{
	return m_features->GetKey(name);
}

HWND CAirPlay::getAirPlayWnd()
{
	return CAsyncSelectWin32::m_wnd[0];
}

HWND CAirPlay::getAirTunesWnd()
{
	return CAsyncSelectWin32::m_wnd[1];
}

bool CAirPlay::StartServices()
{
	if(m_msgwin)m_msgwin->Start();
	m_network->NetworkMessage(CNetwork::SERVICES_UP, 0);
	//SPLOGA(LOG_ERROR, "AIRPLAY Server: Network start failed!");
	return true;
}

bool CAirPlay::StopServices()
{
	m_network->NetworkMessage(CNetwork::SERVICES_DOWN, 0);
	if(m_msgwin)m_msgwin->Stop();
	return true;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_application.StartServices();
	HANDLE env;
	env = ::CreateEventA(0,FALSE,FALSE,NULL);
	::WaitForSingleObject(env,1000);
	::ResetEvent(env);
	::MessageBoxA(NULL,"Zeroconf Test","Zeroconf",0);
	g_application.StopServices();
	::WaitForSingleObject(env,1000);
	CloseHandle(env);
	return 0;
}