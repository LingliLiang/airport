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

#include "threads/SystemClock.h"
#include "system.h"
#include "threads/Locks.h"
#include "log/SimpleLog.h"
#include "utils/StringConversion.h"

#include "SectionLoader.h"
#include "LibraryLoader.h"
#include "Win32DllLoader.h"

static std::shared_ptr<CSectionLoader> g_sectionLoaderRef(std::make_shared<CSectionLoader>());

//  delay for unloading dll's
#define UNLOAD_DELAY 30*1000 // 30 sec.

//Define this to get loggin on all calls to load/unload sections/dlls
//#define LOGALL

LibraryLoader* CSectionLoader::LoadModule(const char* sName, const char* sCurrentDir/*=NULL*/, bool bLoadSymbols/*=false*/)
{
#ifdef LOGALL
	SPLOGAN(LOG_DEBUG, "Loading dll %s", sName);
#endif

	LibraryLoader* pLoader = NULL;

#if defined(TARGET_WINDOWS)
	pLoader = new Win32DllLoader(sName, false);
#endif

	if (!pLoader)
	{
		SPLOGAN(LOG_ERROR, "Unable to create dll %s", sName);
		return NULL;
	}

	if (!pLoader->Load())
	{
		delete pLoader;
		return NULL;
	}
	pLoader->IncrRef();
	return pLoader;
}

void CSectionLoader::ReleaseModule(LibraryLoader*& pDll)
{
	if (!pDll)
		return;
	if (pDll->IsSystemDll())
	{
		SPLOGAN(LOG_ERROR, "%s is a system dll and should never be released", pDll->GetName());
		return;
	}

	int iRefCount=pDll->DecrRef();
	if (iRefCount==0)
	{

#ifdef LOGALL
		SPLOGAN(LOG_DEBUG, "Releasing Dll %s", pDll->GetFileName());
#endif

		if (!pDll->HasSymbols())
		{
			pDll->Unload();
			delete pDll;
			pDll=NULL;
		}
		else
			SPLOGAN(LOG_INFO, "%s has symbols loaded and can never be unloaded", pDll->GetName());
	}
#ifdef LOGALL
	else
	{
		SPLOGAN(LOG_DEBUG, "Dll %s is still referenced with a count of %d", pDll->GetFileName(), iRefCount);
	}
#endif
}

CSectionLoader::CSectionLoader(void)
{}

CSectionLoader::~CSectionLoader(void)
{
	UnloadAll();
}

LibraryLoader *CSectionLoader::LoadDLL(const std::string &dllname, bool bDelayUnload /*=true*/, bool bLoadSymbols /*=false*/)
{
	CSingleLock lock(g_sectionLoader.m_critSection);

	if (dllname.empty()) return NULL;
	// check if it's already loaded, and increase the reference count if so
	for (int i = 0; i < (int)g_sectionLoader.m_vecLoadedDLLs.size(); ++i)
	{
		CDll& dll = g_sectionLoader.m_vecLoadedDLLs[i];
		if (StrConversion::EqualsNoCase(dll.m_strDllName, dllname))
		{
			dll.m_lReferenceCount++;
#ifdef LOGALL
			SPLOGAN(LOG_DEBUG, "Already loaded Dll %s at 0x%x", dll.m_pDll->GetFileName(), dll.m_pDll);
#endif
			return dll.m_pDll;
		}
	}

	// ok, now load the dll
	SPLOGAN(LOG_DEBUG, "SECTION:LoadDLL(%s)\n", dllname.c_str());
	LibraryLoader* pDll = LoadModule(dllname.c_str(), NULL, bLoadSymbols);
	if (!pDll)
		return NULL;

	CDll newDLL;
	newDLL.m_strDllName = dllname;
	newDLL.m_lReferenceCount = 1;
	newDLL.m_bDelayUnload=bDelayUnload;
	newDLL.m_pDll=pDll;
	g_sectionLoader.m_vecLoadedDLLs.push_back(newDLL);

	return newDLL.m_pDll;
}

void CSectionLoader::UnloadDLL(const std::string &dllname)
{
	CSingleLock lock(g_sectionLoader.m_critSection);

	if (dllname.empty()) return;
	// check if it's already loaded, and decrease the reference count if so
	for (int i = 0; i < (int)g_sectionLoader.m_vecLoadedDLLs.size(); ++i)
	{
		CDll& dll = g_sectionLoader.m_vecLoadedDLLs[i];
		if (StrConversion::EqualsNoCase(dll.m_strDllName, dllname))
		{
			dll.m_lReferenceCount--;
			if (0 == dll.m_lReferenceCount)
			{
				if (dll.m_bDelayUnload)
					dll.m_unloadDelayStartTick = XbmcThreads::SystemClockMillis();
				else
				{
					SPLOGAN(LOG_DEBUG,"SECTION:UnloadDll(%s)", dllname.c_str());
					if (dll.m_pDll)
						ReleaseModule(dll.m_pDll);
					g_sectionLoader.m_vecLoadedDLLs.erase(g_sectionLoader.m_vecLoadedDLLs.begin() + i);
				}

				return;
			}
		}
	}
}

void CSectionLoader::UnloadDelayed()
{
	CSingleLock lock(g_sectionLoader.m_critSection);

	// check if we can unload any unreferenced dlls
	for (int i = 0; i < (int)g_sectionLoader.m_vecLoadedDLLs.size(); ++i)
	{
		CDll& dll = g_sectionLoader.m_vecLoadedDLLs[i];
		if (dll.m_lReferenceCount == 0 && XbmcThreads::SystemClockMillis() - dll.m_unloadDelayStartTick > UNLOAD_DELAY)
		{
			SPLOGAN(LOG_DEBUG,"SECTION:UnloadDelayed(DLL: %s)", dll.m_strDllName.c_str());

			if (dll.m_pDll)
				ReleaseModule(dll.m_pDll);
			g_sectionLoader.m_vecLoadedDLLs.erase(g_sectionLoader.m_vecLoadedDLLs.begin() + i);
			return;
		}
	}
}

void CSectionLoader::UnloadAll()
{
	// delete the dll's
	CSingleLock lock(g_sectionLoader.m_critSection);
	std::vector<CDll>::iterator it = g_sectionLoader.m_vecLoadedDLLs.begin();
	while (it != g_sectionLoader.m_vecLoadedDLLs.end())
	{
		CDll& dll = *it;
		if (dll.m_pDll)
			ReleaseModule(dll.m_pDll);
		it = g_sectionLoader.m_vecLoadedDLLs.erase(it);
	}
}
