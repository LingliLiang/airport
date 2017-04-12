
// Locks.h: interface for the All Lock class.

#pragma once

#include "threads/CriticalSection.h"

/**
* This template can be used to define the base implementation for any UniqueLock
* (such as CSingleLock) that uses a Lockable as its mutex/critical section.
*/
template<typename L> class UniqueLock
{
protected:
	L& mutex;
	bool owns;
	inline UniqueLock(L& lockable) : mutex(lockable), owns(true) { mutex.lock(); }
	inline UniqueLock(L& lockable, bool try_to_lock_discrim ) : mutex(lockable) { owns = mutex.try_lock(); }
	inline ~UniqueLock() { if (owns) mutex.unlock(); }

public:

	inline bool owns_lock() const { return owns; }

	//This also implements lockable
	inline void lock() { mutex.lock(); owns=true; }
	inline bool try_lock() { return (owns = mutex.try_lock()); }
	inline void unlock() { if (owns) { mutex.unlock(); owns=false; } }

	/**
	* See the note on the same method on CountingLockable
	*/
	inline L& get_underlying() { return mutex; }

private:
	DISALLOW_COPY_AND_ASSIGN(UniqueLock);
};


/**
* This implements a "guard" pattern for a CCriticalSection that
*  borrows most of it's functionality from boost's unique_lock.
*/
class CSingleLock : public UniqueLock<CCriticalSection>
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
	CAtomic();
	~CAtomic();
	T operator()()
	{
		return m_atomic;
	}
	void operator=(const T&)
	{
		int type = typeid(T);
		switch(type)
		{
		case typeid(bool):

			break;
		default:
			
			break;
		}
	}
private:
	T m_atomic;
	volatile long m_dft;
};

