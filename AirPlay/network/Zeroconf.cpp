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
#include "Zeroconf.h"

#include <cassert>

#include "system.h" //HAS_ZEROCONF define
#include "threads/CriticalSection.h"
#include "threads/Locks.h"

#include "ZeroconfMDNS.h"


long CZeroconf::sm_singleton_guard = 0;
CZeroconf* CZeroconf::smp_instance = 0;

CZeroconf::CZeroconf():mp_crit_sec(new CCriticalSection),m_started(false)
{
}

CZeroconf::~CZeroconf()
{
  delete mp_crit_sec;
}

bool CZeroconf::PublishService(const std::string& fcr_identifier,
                               const std::string& fcr_type,
                               const std::string& fcr_name,
                               unsigned int f_port,
                               std::vector<std::pair<std::string, std::string> > txt /* = std::vector<std::pair<std::string, std::string> >() */)
{
  CSingleLock lock(*mp_crit_sec);
  CZeroconf::PublishInfo info = {fcr_type, fcr_name, f_port, txt};
  std::pair<tServiceMap::const_iterator, bool> ret = m_service_map.insert(std::make_pair(fcr_identifier, info));
  if(!ret.second) //identifier exists
    return false;
  if(m_started)
  {
	   CPublish::PostJob(new CPublish(fcr_identifier,info));
    //CJobManager::GetInstance().AddJob(new CPublish(fcr_identifier, info), NULL);
  }

  //not yet started, so its just queued
  return true;
}

bool CZeroconf::RemoveService(const std::string& fcr_identifier)
{
  CSingleLock lock(*mp_crit_sec);
  tServiceMap::iterator it = m_service_map.find(fcr_identifier);
  if(it == m_service_map.end())
    return false;
  m_service_map.erase(it);
  if(m_started)
    return doRemoveService(fcr_identifier);
  else
    return true;
}

bool CZeroconf::ForceReAnnounceService(const std::string& fcr_identifier)
{
  if (HasService(fcr_identifier) && m_started)
  {
    return doForceReAnnounceService(fcr_identifier);
  }
  return false;
}

bool CZeroconf::HasService(const std::string& fcr_identifier) const
{
  return (m_service_map.find(fcr_identifier) != m_service_map.end());
}

bool CZeroconf::Start()
{
  CSingleLock lock(*mp_crit_sec);
  if(!IsZCdaemonRunning())
  {
    //disable something
    return false;
  }
  if(m_started)
    return true;
  m_started = true;

  //CJobManager::GetInstance().AddJob(new CPublish(m_service_map), NULL);
  CPublish::PostJob(new CPublish(m_service_map));
  return true;
}

void CZeroconf::Stop()
{
  CSingleLock lock(*mp_crit_sec);
  if(!m_started)
    return;
  doStop();
  m_started = false;
}

CZeroconf*  CZeroconf::GetInstance()
{
  CSpinLock lock(sm_singleton_guard);
  if(!smp_instance)
  {
    smp_instance  = new CZeroconfMDNS;
  }
  assert(smp_instance);
  return smp_instance;
}

void CZeroconf::ReleaseInstance()
{
  CSpinLock lock(sm_singleton_guard);
  delete smp_instance;
  smp_instance = 0;
}

CZeroconf::CPublish::CPublish(const std::string& fcr_identifier, const PublishInfo& pubinfo)
{
  m_servmap.insert(std::make_pair(fcr_identifier, pubinfo));
}

CZeroconf::CPublish::CPublish(const tServiceMap& servmap) 
  : m_servmap(servmap)
{
}

bool CZeroconf::CPublish::DoWork()
{
  for(tServiceMap::const_iterator it = m_servmap.begin(); it != m_servmap.end(); ++it)
    CZeroconf::GetInstance()->doPublishService(it->first, it->second.type, it->second.name, it->second.port, it->second.txt);

  return true;
}
