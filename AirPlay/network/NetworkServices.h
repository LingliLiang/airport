#pragma once
/*
 *      Copyright (C) 2013 Team XBMC
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

#include "system.h"
//#include "settings/lib/ISettingCallback.h"

class CNetworkServices /*: public ISettingCallback*/
{
public:
  static CNetworkServices& GetInstance();
  
  //virtual bool OnSettingChanging(const CSetting *setting) override;
  //virtual void OnSettingChanged(const CSetting *setting) override;
  //virtual bool OnSettingUpdate(CSetting* &setting, const char *oldSettingId, const TiXmlNode *oldSettingNode) override;

  void Start();
  void Stop(bool bWait);

  bool StartAirPlayServer();
  bool IsAirPlayServerRunning();
  bool StopAirPlayServer(bool bWait);
  bool StartAirTunesServer();
  bool IsAirTunesServerRunning();
  bool StopAirTunesServer(bool bWait);

  bool StartEventServer();
  bool IsEventServerRunning();
  bool StopEventServer(bool bWait, bool promptuser);
  bool RefreshEventServer();

  bool StartZeroconf();
  bool IsZeroconfRunning();
  bool StopZeroconf();

private:
  CNetworkServices();
  CNetworkServices(const CNetworkServices&);
  CNetworkServices const& operator=(CNetworkServices const&);
  virtual ~CNetworkServices();

  bool ValidatePort(int port);
};
