#pragma once


#include "system.h"
#include "threads/Job.h"


class CAsyncSelectWin32 {
public:
	CAsyncSelectWin32();

	void Close();
	 class CShadowWin : public CJob
	 {
	 public:
		 bool DoWork();
		 static LRESULT WINAPI WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	 };

	 unsigned long m_sWinId;
};


