#pragma once


#include "network\Network.h"

#include <map>
//template class AIRPLAY_API std::map< unsigned int , std::string>; 

namespace FeaturesKey
{
	// AirPlay
	extern const char* FAP_Model;
	extern const char* FAP_Features;
	extern const char* FAP_Srcvers;
	extern const char* FAP_Flags;
	//AirTunes
	extern const char* FAT_am;
	extern const char* FAT_cn;
	extern const char* FAT_da;
	extern const char* FAT_et;
	extern const char* FAT_ft;
	extern const char* FAT_md;
	extern const char* FAT_pk;
	extern const char* FAT_sf;
	extern const char* FAT_tp;
	extern const char* FAT_vn;
	extern const char* FAT_vs;
	extern const char* FAT_vv;
}
using namespace FeaturesKey;
class CAirFeatures;

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
		SETTING_SERVICES_AIRTUNES_PORT,
		SETTING_SERVICES_USEAIRPLAYPASSWORD,
		SETTING_SERVICES_AIRPLAYPASSWORD,
		SETTING_SERVICES_AIRPLAYVOLUMECONTROL,
		SETTING_LOGAIRTUNES,
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
	const char* getFeatures(const char* name);
	HWND getAirPlayWnd();
	HWND getAirTunesWnd();
private:
	class CAsyncSelectWin32* m_msgwin;
	CSettings* m_settings;
	CNetwork* m_network;
	CAirFeatures* m_features;
	struct tAirplaySetting
	{
		bool volumeControl;
	};
	typedef std::map<std::string, tAirplaySetting> tSettingMap;
	tSettingMap* m_Airsettings;
};

