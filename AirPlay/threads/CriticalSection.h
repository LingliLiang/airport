#pragma once
#include "system.h"

#if (defined TARGET_WINDOWS)


class CCriticalSection
{
public:
	CRITICAL_SECTION mutex;
	inline CCriticalSection()
	{
		InitializeCriticalSection(&mutex);
	}
	inline ~CCriticalSection()
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
private:
	DISALLOW_COPY_AND_ASSIGN(CCriticalSection);
};


#endif