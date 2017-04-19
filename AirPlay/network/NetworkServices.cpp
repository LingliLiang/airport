/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "NetworkServices.h"
#include <utility>
#include "AirPlay.h"
#include "network/Network.h"
#include "log/SimpleLog.h"
#include "utils/Variant.h"

#ifdef HAS_AIRPLAY
#include "network/AirPlayServer.h"
#endif // HAS_AIRPLAY

#ifdef HAS_AIRTUNES
#include "network/AirTunesServer.h"
#endif // HAS_AIRTUNES

#ifdef HAS_EVENT_SERVER
#include "network/EventServer.h"
#endif // HAS_EVENT_SERVER

#ifdef HAS_ZEROCONF
#include "network/Zeroconf.h"
#endif // HAS_ZEROCONF

#ifdef HAS_EVENT_SERVER
using namespace EVENTSERVER;
#endif // HAS_EVENT_SERVER

CNetworkServices::CNetworkServices()
{
}

CNetworkServices::~CNetworkServices()
{
}

CNetworkServices& CNetworkServices::GetInstance()
{
  static CNetworkServices sNetworkServices;
  return sNetworkServices;
}

//bool CNetworkServices::OnSettingChanging(const CSetting *setting)
//{
//  if (setting == NULL)
//    return false;
//
//  const std::string &settingId = setting->GetId();
//#ifdef HAS_WEB_SERVER
//  if (settingId == CSettings::SETTING_SERVICES_WEBSERVER ||
//      settingId == CSettings::SETTING_SERVICES_WEBSERVERPORT)
//  {
//    if (IsWebserverRunning() && !StopWebserver())
//      return false;
//
//    if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_WEBSERVER))
//    {
//      if (!StartWebserver())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{33101}, CVariant{33100});
//        return false;
//      }
//    }
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_ESPORT ||
//           settingId == CSettings::SETTING_SERVICES_WEBSERVERPORT)
//    return ValidatePort(((CSettingInt*)setting)->GetValue());
//  else
//#endif // HAS_WEB_SERVER
//
//#ifdef HAS_ZEROCONF
//  if (settingId == CSettings::SETTING_SERVICES_ZEROCONF)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//      return StartZeroconf();
//#ifdef HAS_AIRPLAY
//    else
//    {
//      // cannot disable 
//      if (IsAirPlayServerRunning() || IsAirTunesServerRunning())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{1259}, CVariant{34303});
//        return false;
//      }
//
//      return StopZeroconf();
//    }
//#endif // HAS_AIRPLAY
//  }
//  else
//#endif // HAS_ZEROCONF
//
//#ifdef HAS_AIRPLAY
//  if (settingId == CSettings::SETTING_SERVICES_AIRPLAY)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//    {
//#ifdef HAS_ZEROCONF
//      // AirPlay needs zeroconf
//      if (!CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ZEROCONF))
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{1273}, CVariant{34302});
//        return false;
//      }
//#endif //HAS_ZEROCONF
//
//      // note - airtunesserver has to start before airplay server (ios7 client detection bug)
//#ifdef HAS_AIRTUNES
//      if (!StartAirTunesServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{1274}, CVariant{33100});
//        return false;
//      }
//#endif //HAS_AIRTUNES
//      
//      if (!StartAirPlayServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{1273}, CVariant{33100});
//        return false;
//      }      
//    }
//    else
//    {
//      bool ret = true;
//#ifdef HAS_AIRTUNES
//      if (!StopAirTunesServer(true))
//        ret = false;
//#endif //HAS_AIRTUNES
//      
//      if (!StopAirPlayServer(true))
//        ret = false;
//
//      if (!ret)
//        return false;
//    }
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_AIRPLAYVIDEOSUPPORT)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//    {
//      if (!StartAirPlayServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{1273}, CVariant{33100});
//        return false;
//      }
//    }
//    else
//    {
//      if (!StopAirPlayServer(true))
//        return false;
//    }
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_AIRPLAYPASSWORD ||
//           settingId == CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD)
//  {
//    if (!CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_AIRPLAY))
//      return false;
//
//    if (!CAirPlayServer::SetCredentials(CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD),
//                                        CSettings::GetInstance().GetString(CSettings::SETTING_SERVICES_AIRPLAYPASSWORD)))
//      return false;
//  }
//  else
//#endif //HAS_AIRPLAY
//
//#ifdef HAS_UPNP
//  if (settingId == CSettings::SETTING_SERVICES_UPNPSERVER)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//    {
//      if (!StartUPnPServer())
//        return false;
//
//      // always stop and restart the client and controller if necessary
//      StopUPnPClient();
//      StopUPnPController();
//      StartUPnPClient();
//      StartUPnPController();
//    }
//    else
//      return StopUPnPServer();
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_UPNPRENDERER)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//      return StartUPnPRenderer();
//    else
//      return StopUPnPRenderer();
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_UPNPCONTROLLER)
//  {
//    // always stop and restart
//    StopUPnPController();
//    if (((CSettingBool*)setting)->GetValue())
//      return StartUPnPController();
//  }
//  else
//#endif // HAS_UPNP
//
//  if (settingId == CSettings::SETTING_SERVICES_ESENABLED)
//  {
//    if (((CSettingBool*)setting)->GetValue())
//    {
//      bool result = true;
//#ifdef HAS_EVENT_SERVER
//      if (!StartEventServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{33102}, CVariant{33100});
//        result = false;
//      }
//#endif // HAS_EVENT_SERVER
//
//#ifdef HAS_JSONRPC
//      if (!StartJSONRPCServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{33103}, CVariant{33100});
//        result = false;
//      }
//#endif // HAS_JSONRPC
//      return result;
//    }
//    else
//    {
//      bool result = true;
//#ifdef HAS_EVENT_SERVER
//      result = StopEventServer(true, true);
//#endif // HAS_EVENT_SERVER
//#ifdef HAS_JSONRPC
//      result &= StopJSONRPCServer(false);
//#endif // HAS_JSONRPC
//      return result;
//    }
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_ESPORT)
//  {
//#ifdef HAS_EVENT_SERVER
//    // restart eventserver without asking user
//    if (!StopEventServer(true, false))
//      return false;
//
//    if (!StartEventServer())
//    {
//      CGUIDialogOK::ShowAndGetInput(CVariant{33102}, CVariant{33100});
//      return false;
//    }
//
//#if defined(TARGET_DARWIN_OSX)
//    // reconfigure XBMCHelper for port changes
//    XBMCHelper::GetInstance().Configure();
//#endif // TARGET_DARWIN_OSX
//#endif // HAS_EVENT_SERVER
//  }
//  else if (settingId == CSettings::SETTING_SERVICES_ESALLINTERFACES)
//  {
//#ifdef HAS_EVENT_SERVER
//    if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED))
//    {
//      if (!StopEventServer(true, true))
//        return false;
//
//      if (!StartEventServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{33102}, CVariant{33100});
//        return false;
//      }
//    }
//#endif // HAS_EVENT_SERVER
//
//#ifdef HAS_JSONRPC
//    if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED))
//    {
//      if (!StopJSONRPCServer(true))
//        return false;
//
//      if (!StartJSONRPCServer())
//      {
//        CGUIDialogOK::ShowAndGetInput(CVariant{33103}, CVariant{33100});
//        return false;
//      }
//    }
//#endif // HAS_JSONRPC
//  }
//
//#ifdef HAS_EVENT_SERVER
//  else if (settingId == CSettings::SETTING_SERVICES_ESINITIALDELAY ||
//           settingId == CSettings::SETTING_SERVICES_ESCONTINUOUSDELAY)
//  {
//    if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED))
//      return RefreshEventServer();
//  }
//#endif // HAS_EVENT_SERVER
//
//  return true;
//}
//
//void CNetworkServices::OnSettingChanged(const CSetting *setting)
//{
//  if (setting == NULL)
//    return;
//
//  const std::string &settingId = setting->GetId();
//#ifdef HAS_WEB_SERVER
//  if (settingId == CSettings::SETTING_SERVICES_WEBSERVERUSERNAME ||
//      settingId == CSettings::SETTING_SERVICES_WEBSERVERPASSWORD)
//  {
//    m_webserver.SetCredentials(CSettings::GetInstance().GetString(CSettings::SETTING_SERVICES_WEBSERVERUSERNAME),
//                               CSettings::GetInstance().GetString(CSettings::SETTING_SERVICES_WEBSERVERPASSWORD));
//  }
//  else
//#endif // HAS_WEB_SERVER
//  if (settingId == CSettings::SETTING_SMB_WINSSERVER ||
//      settingId == CSettings::SETTING_SMB_WORKGROUP)
//  {
//    // okey we really don't need to restart, only deinit samba, but that could be damn hard if something is playing
//    //! @todo - General way of handling setting changes that require restart
//    if (HELPERS::ShowYesNoDialogText(CVariant{14038}, CVariant{14039}) == DialogResponse::YES)
//    {
//      CSettings::GetInstance().Save();
//      CApplicationMessenger::GetInstance().PostMsg(TMSG_RESTARTAPP);
//    }
//  }
//}
//
//bool CNetworkServices::OnSettingUpdate(CSetting* &setting, const char *oldSettingId, const TiXmlNode *oldSettingNode)
//{
//  if (setting == NULL)
//    return false;
//
//  const std::string &settingId = setting->GetId();
//  if (settingId == CSettings::SETTING_SERVICES_WEBSERVERUSERNAME)
//  {
//    // if webserverusername is xbmc and pw is not empty we treat it as altered
//    // and don't change the username to kodi - part of rebrand
//    if (CSettings::GetInstance().GetString(CSettings::SETTING_SERVICES_WEBSERVERUSERNAME) == "xbmc" &&
//        !CSettings::GetInstance().GetString(CSettings::SETTING_SERVICES_WEBSERVERPASSWORD).empty())
//      return true;
//  }
//  if (settingId == CSettings::SETTING_SERVICES_WEBSERVERPORT)
//  {
//    // if webserverport is default but webserver is activated then treat it as altered
//    // and don't change the port to new value
//    if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_WEBSERVER))
//      return true;
//  }
//  return false;
//}

void CNetworkServices::Start()
{
  StartZeroconf();

  //if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED) && !StartEventServer())
  //  CGUIDialogKaiToast::QueueNotification(CGUIDialogKaiToast::Warning, g_localizeStrings.Get(33102), g_localizeStrings.Get(33100));
  //if (CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED) && !StartJSONRPCServer())
  //  CGUIDialogKaiToast::QueueNotification(CGUIDialogKaiToast::Warning, g_localizeStrings.Get(33103), g_localizeStrings.Get(33100));
  
  // note - airtunesserver has to start before airplay server (ios7 client detection bug)
  StartAirTunesServer();
  StartAirPlayServer();
}

void CNetworkServices::Stop(bool bWait)
{
  if (bWait)
  {
    StopZeroconf();
  }

  StopEventServer(bWait, false);
  StopAirPlayServer(bWait);
  StopAirTunesServer(bWait);
}

bool CNetworkServices::StartAirPlayServer()
{
	if (!g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_AIRPLAY_VIDEOSUPPORT))
    return true;

#ifdef HAS_AIRPLAY
  if (!g_application.getNetwork().IsAvailable() || !g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_AIRPLAY))
    return false;

  if (IsAirPlayServerRunning())
    return true;
  
  if (!CAirPlayServer::StartServer(g_application.getSettings().GetInt(CSettings::SETTING_SERVICES_AIRPLAY_PORT), true))
    return false;
  
  if (!CAirPlayServer::SetCredentials(g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD),
                                      g_application.getSettings().GetString(CSettings::SETTING_SERVICES_AIRPLAYPASSWORD)))
    return false;
  
#ifdef HAS_ZEROCONF
  std::vector<std::pair<std::string, std::string> > txt;
  CNetworkInterface* iface = g_application.getNetwork().GetFirstConnectedInterface();
  txt.push_back(std::make_pair("deviceid", iface != NULL ? iface->GetMacAddress() : "FF:FF:FF:FF:FF:F2"));
  txt.push_back(std::make_pair("model", "AppleTV3,2C"));
  txt.push_back(std::make_pair("srcvers", AIRPLAY_SERVER_VERSION_STR));

  // for ios8 clients we need to announce mirroring support
  // else we won't get video urls anymore.
  // We also announce photo caching support (as it seems faster and
  // we have implemented it anyways).
  txt.push_back(std::make_pair("features", "0x23F7"));

  CZeroconf::GetInstance()->PublishService("servers.airplay", "_airplay._tcp", 
	  g_application.getSettings().GetString(CSettings::SETTING_SERVICES_AIRPLAY_DEVICENAME).c_str(),
	  g_application.getSettings().GetInt(CSettings::SETTING_SERVICES_AIRPLAY_PORT), txt);
#endif // HAS_ZEROCONF

  return true;
#endif // HAS_AIRPLAY
  return false;
}

bool CNetworkServices::IsAirPlayServerRunning()
{
#ifdef HAS_AIRPLAY
  return CAirPlayServer::IsRunning();
#endif // HAS_AIRPLAY
  return false;
}

bool CNetworkServices::StopAirPlayServer(bool bWait)
{
#ifdef HAS_AIRPLAY
  if (!IsAirPlayServerRunning())
    return true;

  CAirPlayServer::StopServer(bWait);

#ifdef HAS_ZEROCONF
  CZeroconf::GetInstance()->RemoveService("servers.airplay");
#endif // HAS_ZEROCONF

  return true;
#endif // HAS_AIRPLAY
  return false;
}

bool CNetworkServices::StartAirTunesServer()
{
#ifdef HAS_AIRTUNES
	if (!g_application.getNetwork().IsAvailable() || !g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_AIRPLAY))
    return false;

  if (IsAirTunesServerRunning())
    return true;

  if (!CAirTunesServer::StartServer(g_application.getSettings().GetInt(CSettings::SETTING_SERVICES_AIRTUNES_PORT), true,
                                    g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_USEAIRPLAYPASSWORD),
                                    g_application.getSettings().GetString(CSettings::SETTING_SERVICES_AIRPLAYPASSWORD)))
  {
    SPLOGA(LOG_ERROR, "Failed to start AirTunes Server");
    return false;
  }

  return true;
#endif // HAS_AIRTUNES
  return false;
}

bool CNetworkServices::IsAirTunesServerRunning()
{
#ifdef HAS_AIRTUNES
  return CAirTunesServer::IsRunning();
#endif // HAS_AIRTUNES
  return false;
}

bool CNetworkServices::StopAirTunesServer(bool bWait)
{
#ifdef HAS_AIRTUNES
  if (!IsAirTunesServerRunning())
    return true;

  CAirTunesServer::StopServer(bWait);
  return true;
#endif // HAS_AIRTUNES
  return false;
}

bool CNetworkServices::StartEventServer()
{
#ifdef HAS_EVENT_SERVER
  if (!CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED))
    return false;

  if (IsEventServerRunning())
    return true;

  CEventServer* server = CEventServer::GetInstance();
  if (!server)
  {
    SPLOGAN(LOGERROR, "ES: Out of memory");
    return false;
  }

  server->StartServer();

  return true;
#endif // HAS_EVENT_SERVER
  return false;
}

bool CNetworkServices::IsEventServerRunning()
{
#ifdef HAS_EVENT_SERVER
  return CEventServer::GetInstance()->Running();
#endif // HAS_EVENT_SERVER
  return false;
}

bool CNetworkServices::StopEventServer(bool bWait, bool promptuser)
{
#ifdef HAS_EVENT_SERVER
  if (!IsEventServerRunning())
    return true;

  CEventServer* server = CEventServer::GetInstance();
  if (!server)
  {
    SPLOGAN(LOGERROR, "ES: Out of memory");
    return false;
  }

  if (promptuser)
  {
    if (server->GetNumberOfClients() > 0)
    {
      if (HELPERS::ShowYesNoDialogText(CVariant{13140}, CVariant{13141}, CVariant{""}, CVariant{""}, 10000) != 
        DialogResponse::YES)
      {
        SPLOGAN(LOGNOTICE, "ES: Not stopping event server");
        return false;
      }
    }
    SPLOGAN(LOGNOTICE, "ES: Stopping event server with confirmation");

    CEventServer::GetInstance()->StopServer(true);
  }
  else
  {
    if (!bWait)
      SPLOGAN(LOGNOTICE, "ES: Stopping event server");

    CEventServer::GetInstance()->StopServer(bWait);
  }

  return true;
#endif // HAS_EVENT_SERVER
  return false;
}

bool CNetworkServices::RefreshEventServer()
{
#ifdef HAS_EVENT_SERVER
  if (!CSettings::GetInstance().GetBool(CSettings::SETTING_SERVICES_ESENABLED))
    return false;

  if (!IsEventServerRunning())
    return false;

  CEventServer::GetInstance()->RefreshSettings();
  return true;
#endif // HAS_EVENT_SERVER
  return false;
}

bool CNetworkServices::StartZeroconf()
{
#ifdef HAS_ZEROCONF
  if (!g_application.getSettings().GetBool(CSettings::SETTING_SERVICES_ZEROCONF))
    return false;

  if (IsZeroconfRunning())
    return true;

  SPLOGA(LOG_INFO, "starting zeroconf publishing");
  return CZeroconf::GetInstance()->Start();
#endif // HAS_ZEROCONF
  return false;
}

bool CNetworkServices::IsZeroconfRunning()
{
#ifdef HAS_ZEROCONF
  return CZeroconf::GetInstance()->IsStarted();
#endif // HAS_ZEROCONF
  return false;
}

bool CNetworkServices::StopZeroconf()
{
#ifdef HAS_ZEROCONF
  if (!IsZeroconfRunning())
    return true;

  SPLOGA(LOG_INFO, "stopping zeroconf publishing");
  CZeroconf::GetInstance()->Stop();

  return true;
#endif // HAS_ZEROCONF
  return false;
}

bool CNetworkServices::ValidatePort(int port)
{
  if (port <= 0 || port > 65535)
    return false;
  return true;
}
