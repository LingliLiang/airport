// AirPlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "network/AsyncSelectWin32.h"
#include"log\SimpleLog.h"
#include "network/Zeroconf.h"
#include "ZeroconfBrowser.h"
#include <map>
using namespace std;


extern HWND g_hWnd;
#define WM_SOCKET (WM_USER + 101) 

CAsyncSelectWin32::CAsyncSelectWin32()
{
}

void CAsyncSelectWin32::Start()
{
	CShadowWin* p = new CShadowWin;
	CShadowWin::PostJob(p);
	m_sWinId = p->GetTId();
}


void CAsyncSelectWin32::Stop()
{
	::PostMessage(m_wnd[0],WM_CLOSE,0,0);
	::PostMessage(m_wnd[1],WM_CLOSE,0,0);
}

HWND CAsyncSelectWin32::m_wnd[2] = {0,0};

int CAsyncSelectWin32::CShadowWin::wnd_count = 0;

LRESULT WINAPI CAsyncSelectWin32::CShadowWin::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(hwnd == CAsyncSelectWin32::m_wnd[0])
	{
		SPLOGA(LOG_INFO,"wnd  of BONJOUR_EVENT");
	}
	else if(hwnd == CAsyncSelectWin32::m_wnd[1])
	{
		SPLOGA(LOG_INFO,"wnd  of   BONJOUR_BROWSER_EVENT");
	}

	switch(uMsg)
	{
	case BONJOUR_EVENT:
		SPLOGA(LOG_INFO,"BONJOUR_EVENT");
		CZeroconf::GetInstance()->ProcessResults();
		break;
	case BONJOUR_BROWSER_EVENT:
		SPLOGA(LOG_INFO,"BONJOUR_BROWSER_EVENT");
		CZeroconfBrowser::GetInstance()->ProcessResults();
		break;
	case WM_DESTROY:
		{
			SPLOGA(LOG_INFO,"window close !\n");
			if((--wnd_count)<=0)
				::PostThreadMessage(::GetCurrentThreadId(),WM_QUIT,0,0);
		}
	case WM_CREATE:
		{
		}
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

bool CAsyncSelectWin32::CShadowWin::DoWork()
{
	TCHAR szClassName[] = _T("WSAAsyncSelect Shadow");
	static WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szClassName;
	wndClass.hIconSm = NULL;


	ATOM atom = RegisterClassEx(&wndClass);
	if(0 == atom)
	{
		SPLOGAN(LOG_INFO,"WSAAsyncSelect Shadow RegisterClassEx failed - %d",GetLastError());
		return false;
	}
	HWND hwnd = CreateWindowEx(0,(TCHAR *)atom,_T(""),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,HWND_MESSAGE,NULL,NULL,NULL);
	if(hwnd == NULL)
	{
		SPLOGAN(LOG_INFO,"WSAAsyncSelect AirPlay CreateWindowEx failed - %d",GetLastError());
		return false;
	}
	wnd_count++;
	CAsyncSelectWin32::m_wnd[0] = hwnd;

	 hwnd = CreateWindowEx(0,(TCHAR *)atom,_T(""),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,HWND_MESSAGE,NULL,NULL,NULL);

	if(hwnd == NULL)
	{
		SPLOGAN(LOG_INFO,"WSAAsyncSelect AirTunes CreateWindowEx failed - %d",GetLastError());
		return false;
	}
	wnd_count++;
	CAsyncSelectWin32::m_wnd[1] = hwnd;
		
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	SPLOGA(LOG_INFO,"CAsyncSelectWin32 Shutdown!");
	return true;
}
