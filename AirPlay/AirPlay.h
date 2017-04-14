#pragma once


#include "network\Network.h"

#include <map>
//template class AIRPLAY_API std::map< unsigned int , std::string>; 

class CSettings
{
public:
	typedef std::map<	unsigned int , std::string> tSettings;
	enum typeSettings
	{
		SETTING_START,
		SETTING_SERVICES_ZEROCONF,
		SETTING_SERVICES_AIRPLAY,
		SETTING_SERVICES_AIRPLAY_DEVICENAME,
		SETTING_SERVICES_AIRPLAY_VIDEOSUPPORT,
		SETTING_SERVICES_AIRPLAY_PORT,
		SETTING_SERVICES_USEAIRPLAYPASSWORD,
		SETTING_SERVICES_AIRPLAYPASSWORD,
		SETTING_SERVICES_AIRPLAYVOLUMECONTROL,
		SETTING_END
	};
public:
	CSettings();
	~CSettings();
	bool GetBool(typeSettings type);
	int GetInt(typeSettings type);
	unsigned long GetULong(typeSettings type);
	std::string GetString(typeSettings type);
	//std::wstring GetWSting(typeSettings type);
private:
	inline bool TypeOut(typeSettings type);
	tSettings m_settings;
};


class AIRPLAY_API CAirPlay {
public:
	CAirPlay(void);
	~CAirPlay();

	bool StartServices();
	bool StopServices();

	CNetwork& getNetwork();
	CSettings& getSettings();
	
private:
	class CAsyncSelectWin32* m_msgwin;
	CSettings* m_settings;
	CNetwork* m_network;

	struct tAirplaySetting
	{
		bool volumeControl;
	};
	typedef std::map<std::string, tAirplaySetting> tSettingMap;
	tSettingMap* m_Airsettings;
};

