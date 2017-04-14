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

#include "Stopwatch.h"
#include "system.h"

static int64_t CurrentHostCounter(void)
{
#if defined(TARGET_WINDOWS)
	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);
	return( (int64_t)PerformanceCount.QuadPart );
#else
	struct timespec now;
#ifdef CLOCK_MONOTONIC_RAW
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
#else
	clock_gettime(CLOCK_MONOTONIC, &now);
#endif // CLOCK_MONOTONIC_RAW
	return( ((int64_t)now.tv_sec * 1000000000L) + now.tv_nsec );
#endif
}

static int64_t CurrentHostFrequency(void)
{
#if defined(TARGET_WINDOWS)
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	return( (int64_t)Frequency.QuadPart );
#else
	return( (int64_t)1000000000L );
#endif
}


CStopWatch::CStopWatch(bool useFrameTime /*=false*/)
{
	m_timerPeriod      = 0.0f;
	m_startTick        = 0;
	m_stopTick         = 0;
	m_isRunning        = false;
	m_useFrameTime     = useFrameTime;

	m_timerPeriod = 1.0f / (float)CurrentHostFrequency();
}

CStopWatch::~CStopWatch()
{
}

int64_t CStopWatch::GetTicks() const
{
	return CurrentHostCounter();
}
