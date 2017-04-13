
// Locks.h: interface for the All Lock class.

#pragma once

#include "threads/CriticalSection.h"
#include "threads/Helpers.h"
#include "threads/Lockables.h"


/**
* This implements a "guard" pattern for a CCriticalSection that
*  borrows most of it's functionality from boost's unique_lock.
*/
class CSingleLock : public XbmcThreads::UniqueLock<CCriticalSection>
{
public:
	inline CSingleLock(CCriticalSection& cs) : UniqueLock<CCriticalSection>(cs) {}
	inline CSingleLock(const CCriticalSection& cs) : UniqueLock<CCriticalSection> ((CCriticalSection&)cs) {}

	inline void Leave() { unlock(); }
	inline void Enter() { lock(); }
protected:
	inline CSingleLock(CCriticalSection& cs, bool dicrim) : UniqueLock<CCriticalSection>(cs,true) {}
};

class CSpinLock
{
public:
	CSpinLock(long& lock):m_Lock(lock)
	{
		while (InterlockedCompareExchange(&m_Lock, 1, 0) != 0)
		{
			Sleep(0);
		}
	}
	~CSpinLock()
	{
		InterlockedExchange(&m_Lock, 0);
	}
private:
	long& m_Lock;
	DISALLOW_COPY_AND_ASSIGN(CSpinLock);
};


//basic type T
template<typename T>
class CAtomic
{
public:
	CAtomic():m_lock(0){}
	CAtomic( T t):m_atomic(t),m_lock(0){}
	~CAtomic(){}
	T get()
	{
		return m_atomic;
	}
	void operator=(const T& t)
	{
		CSpinLock lock(m_lock);
		m_atomic = t;
	}
private:
	volatile T m_atomic;
	long m_lock;
};
