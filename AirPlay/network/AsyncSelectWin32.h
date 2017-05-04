#pragma once


#include "system.h"
#include "threads/Job.h"


class CAsyncSelectWin32 {
public:
	CAsyncSelectWin32();

	void Start();
	void Stop();
	 class CShadowWin : public CJob
	 {
	 public:
		 bool DoWork();
		 static LRESULT WINAPI WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		 static int wnd_count;
	 };

	 static HWND m_wnd[2];
	 unsigned long m_sWinId;
};


