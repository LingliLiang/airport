#pragma once 

#define TARGET_WINDOWS
#if _MSC_VER < 1700
#define USE_SYS_INTTYPES
#endif
//#define HAS_AIRTUNES
#define HAS_AIRPLAY

#define HAS_MDNS
#define HAS_ZEROCONF

#define HAS_LOG


// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include "sys\socket.h"

#define BONJOUR_EVENT             ( WM_USER + 0x100 )	// Message sent to the Window when a Bonjour event occurs.
#define BONJOUR_BROWSER_EVENT     ( WM_USER + 0x110 )