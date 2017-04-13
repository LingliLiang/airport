#pragma once
#include "system.h"
#include "threads/Lockables.h"
#if (defined TARGET_WINDOWS)

// forward declare in preparation for the friend declaration
namespace XbmcThreads
{
	class ConditionVariable;


	class RecursiveMutex
	{
		CRITICAL_SECTION mutex;

		// needs acces to 'mutex'
		friend class XbmcThreads::ConditionVariable;
	public:
		inline RecursiveMutex()
		{
			InitializeCriticalSection(&mutex);
		}

		inline ~RecursiveMutex()
		{
			DeleteCriticalSection(&mutex);
		}

		inline void lock()
		{
			EnterCriticalSection(&mutex);
		}

		inline void unlock()
		{
			LeaveCriticalSection(&mutex);
		}

		inline bool try_lock()
		{
			return TryEnterCriticalSection(&mutex) ? true : false;
		}
	};

}

class CCriticalSection : public XbmcThreads::CountingLockable<XbmcThreads::RecursiveMutex>
{
public:
	CCriticalSection(){}
private:
	DISALLOW_COPY_AND_ASSIGN(CCriticalSection);
};

#endif