#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <queue>
#include <cassert>
#include "utils/StringConversion.h"
#include "XZip.h"

/******
Simple log , only for windows system.
Author : liang 
Date: 2016.4.13
******/

/**
indentify log type
**/
typedef enum enum_log_type 
{
	LOG_NAN = 1,
	LOG_INFO = 1<<2,
	LOG_ERROR = 1<<3,
	LOG_DEBUG = 1<<4
}TypeLog;

namespace SimpleLOG
{
	/**
	Log Format : [<date>] [<type> ERROR,INFO,DEBUG,NAN] [<Strings>]
	**/

#define UNREF(a) a;
#define TRY_LOG(proc) 	try{  	EnterCriticalSection(&proc)
#define TRY_END(proc) 	}catch (std::exception& e){ UNREF(e);LeaveCriticalSection(&proc); } catch(...){ LeaveCriticalSection(&proc); m_sError = "Write failed."; } LeaveCriticalSection(&proc)

#define CLIENT_INFINITE            0x0000FFFF   //aboat 1min
#define MAX_QUEUE_BUFFER 26
#define MAX_STRING 1024


	/**
	struct stone one peice's log
	**/
	typedef struct struct_log_format 
	{
		std::string date;
		TypeLog type;
		std::string logstring;
	}FmtLog;

	//SimpleLog
	extern class SimpleLog g_sLog;

#define	SPLOGW(type,wchar_text) SimpleLOG::g_sLog.PushLogW((TypeLog)type,wchar_text)
#define	SPLOGA(type,char_text) SimpleLOG::g_sLog.PushLog((TypeLog)type,char_text)


#define	SPLOGWN(type,wchar_format,...) SimpleLOG::g_sLog.PushLogWN((TypeLog)type,wchar_format, ##__VA_ARGS__)
#define	SPLOGAN(type,char_format,...) SimpleLOG::g_sLog.PushLogN((TypeLog)type,char_format, ##__VA_ARGS__)

#ifndef _UNICODE
	/**Ascii version**/
#define SPLOGT   SPLOGA
#define SPLOGTN   SPLOGAN
#else
	/**Unicode version**/
#define SPLOGT   SPLOGW
#define SPLOGTN   SPLOGWN
#endif

#define SPSTARTLOG               SimpleLOG::g_sLog.Start()
#define SPSTOPLOG                   SimpleLOG::g_sLog.Stop()
#define SPWAITTHREAD(handle) SimpleLOG::g_sLog.AddClientThreadHandle((HANDLE)handle)
#define GLOG                                SimpleLOG::g_sLog

	class SimpleLog
	{
	public:
		SimpleLog::SimpleLog(void)
			:m_exitTread(true),
			m_bStarted(false),
			m_sLogName("default.log")
		{
			::InitializeCriticalSection(&m_csMutex);
		}
		SimpleLog::~SimpleLog(void)
		{
			if(m_exitTread == false)
			{
				Stop();
			}
			DeleteCriticalSection(&m_csMutex);
		}
		/**
		Get Loacl system time. format : "year.month.day hour:minute:second.mseconds"
		**/
		std::string GetCurrentLocalTime()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			std::string date;
			char* buffer = new char[24]; //length = 23+1('\0')
			sprintf_s(buffer,24, "%04d.%02d.%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,
				sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds); 
			date = buffer;
			delete [] buffer;
			return date;
		}
		/**
		Get type string by enum define
		**/
		const char* GetType(TypeLog type)
		{
			switch(type)
			{
			case LOG_INFO: return "[INFO]";
			case LOG_ERROR: return "[ERROR]";
			case LOG_DEBUG: return "[DEBUG]";
			default: break;
			}
			return "";
		}
		/**
		push a piece log
		**/
		bool PushLogW(TypeLog type, std::wstring wcontext)
		{
			if(m_exitTread)
				return false;
			std::string context;
			StrConversion::UNICODE_To_ANSI_String(wcontext.data(),context);
			FmtLog fLog = {GetCurrentLocalTime(), type, context};
			TRY_LOG(m_csMutex);
			m_qLogBlock.push(fLog);
			TRY_END(m_csMutex);
			ResumeThread(m_hThread);
			m_nPushCount++;
			return true;
		}

		bool PushLogWN(TypeLog type, std::wstring wcontext,...)
		{
			if(m_exitTread)
				return false;
			std::string context;
			va_list args;
			va_start(args, context);
			std::wstring str = StrConversion::FormatV(wcontext.c_str(), args);
			va_end(args);
			StrConversion::UNICODE_To_ANSI_String(str.data(),context);
			FmtLog fLog = {GetCurrentLocalTime(), type, context};
			TRY_LOG(m_csMutex);
			m_qLogBlock.push(fLog);
			TRY_END(m_csMutex);
			ResumeThread(m_hThread);
			m_nPushCount++;
			return true;
		}

		/**
		push a piece log
		**/
		bool PushLog(TypeLog type, std::string context)
		{
			if(m_exitTread)
				return false;
			FmtLog fLog = {GetCurrentLocalTime(), type, context};
			TRY_LOG(m_csMutex);
			m_qLogBlock.push(fLog);
			TRY_END(m_csMutex);
			ResumeThread(m_hThread);
			m_nPushCount++;
			return true;
		}

		bool PushLogN(TypeLog type, std::string context,...)
		{
			if(m_exitTread)
				return false;
			va_list args;
			va_start(args, context);
			std::string str = StrConversion::FormatV(context.c_str(), args);
			va_end(args);
			FmtLog fLog = {GetCurrentLocalTime(), type, str};
			TRY_LOG(m_csMutex);
			m_qLogBlock.push(fLog);
			TRY_END(m_csMutex);
			ResumeThread(m_hThread);
			m_nPushCount++;
			return true;
		}

		/**
		Use default.log by default name.you must call this on first pushlog to apply the name. "xxx.log"
		**/
		bool SetLogFileName(const char* filename)
		{
			if(m_exitTread) //only set value when thread not started
			{
				m_sLogName = filename;
			}
			return true;
		}
		/**
		Get simple error infomation 
		**/
		std::string GetErrorInfo() const
		{
			return m_sError;
		}
		/**
		Start log must call before first push log
		**/
		void Start()
		{
			//run once until m_bStarted false agin
			if(m_bStarted == false)
				m_bStarted = !m_bStarted;
			else
				return;
			m_exitTread = false;
			DWORD dwAttr = ::GetFileAttributesA(m_sLogName.c_str());
			//file exist backup log file to previous_logs foleder
			if(INVALID_FILE_ATTRIBUTES  != dwAttr)
			{
				BY_HANDLE_FILE_INFORMATION file_info = {0};
				HANDLE hFile = ::CreateFileA(m_sLogName.c_str(), GENERIC_READ, 0, 
					NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				std::string filedate;
				if( GetFileInformationByHandle((HANDLE)hFile,&file_info) )
				{
					SYSTEMTIME unisys;
					SYSTEMTIME sys;
					::FileTimeToSystemTime(&file_info.ftLastWriteTime, &unisys);
					::SystemTimeToTzSpecificLocalTime(NULL,&unisys,&sys);
					char* buffer = new char[21]; //length = 23+1('\0')
					sprintf_s(buffer,21, "_%04d_%02d_%02d_%02d_%02d_%02d",sys.wYear,sys.wMonth,sys.wDay,
						sys.wHour,sys.wMinute,sys.wSecond); 
					filedate = buffer;
					delete [] buffer;
				}
				::CloseHandle(hFile);
				ZipPreviousLog(filedate); // backup log file
				::DeleteFileA(m_sLogName.c_str()); //remove log file
			}
			WatchThread();
		}
		/**
		End log must call ,when exitting.
		**/
		void Stop()
		{
			//waitting client's thread
			if(m_hClientThread.size() > 0)
			{
				::WaitForMultipleObjects((DWORD)m_hClientThread.size(), &(m_hClientThread[0]), true, CLIENT_INFINITE);
			}
			m_exitTread = true; // set thread exit flage

			DWORD sign = ResumeThread(m_hThread); //Resume thread to exit.
			while(1)
			{
				if( sign <= 1 || sign == 0xFFFFFFFF )
					break;
				sign = ResumeThread(m_hThread);
			}
			WaitForSingleObject( m_hThread, INFINITE ); //wait thread to exit
			CloseHandle(m_hThread);
			OnFinalLog();
			if(m_bStarted == true)
				m_bStarted = !m_bStarted;
			ClearClinetThread();
		}

		/**
		add item will cause to wait all thread 1min, thread handle must vailed
		**/
		void AddClientThreadHandle(HANDLE hThread)
		{
			std::vector<HANDLE>::iterator it = m_hClientThread.begin();
			for( ; it !=  m_hClientThread.end() ; )
			{
				if( *it == hThread)
				{
					return ;
				}
				it++;
			}
			m_hClientThread.push_back(hThread);
		}

		/**
		Clear up,close all thread handle,and delete from waitting threads array,if want do yourself by hand, don't call this function
		**/
		void ClearClinetThread()
		{
			while(m_hClientThread.size())
			{
				CloseHandle(m_hClientThread[m_hClientThread.size() - 1]);
				m_hClientThread.pop_back();
			}
		}

		/**
		Remove a existed thread from waitting array.
		**/
		void RemoveThread(HANDLE hToRe)
		{
			std::vector<HANDLE>::iterator it = m_hClientThread.begin();
			for( ; it !=  m_hClientThread.end() ; )
			{
				if( *it == hToRe)
				{
					m_hClientThread.erase(it);
					break;
				}
				it++;
			}
		}


	private:
		/**
		Write log
		**/
		bool WriteLog(const char* peer_log)
		{
			try
			{
				m_fs.write(peer_log,strlen(peer_log));
				m_fs.write("\n",1);
			}
			catch(...)
			{
				m_fs.close();
				throw ;
			}
			return true;
		}
		/**
		Write log
		**/
		bool WriteLog(std::string date, std::string type, std::string context)
		{
			//int len = strnlen_s(date.c_str(),MAX_PATH) + strnlen_s(type.c_str(),MAX_PATH) + strnlen_s(context.c_str(),MAX_PATH) +2;
			int len = (int)strnlen_s(date.c_str(),24);
			len +=(int)strnlen_s(type.c_str(), 8);
			len +=(int)strnlen_s(context.c_str(),MAX_STRING+1);
			len+=3; // 2*space + 1*'\0'
			assert(len>0);
			bool result = false;
			char* buffer = new char[len];
			sprintf_s(buffer,len, "%s %s %s", date.c_str(), type.c_str(), context.c_str()); 
			result = WriteLog(buffer);
			delete [] buffer;
			return result;
		}
		/**
		Write log block
		**/
		void WriteLogBlock()
		{
			// enter critical section
			TRY_LOG(m_csMutex);
			//write logs
			int nBlock = (int)m_qLogBlock.size();

			if(nBlock >= MAX_QUEUE_BUFFER)
			{
				m_fs.open(m_sLogName.c_str(), std::ios::app | std::ios::out );
				for(int peice = 0; peice < nBlock; peice++)
				{
					FmtLog fLog = m_qLogBlock.front();
					WriteLog(fLog.date,GetType(fLog.type),fLog.logstring);
					m_qLogBlock.pop();
				}
				m_fs.close();
			}
			// leave critical section
			TRY_END(m_csMutex);
		}
		/**
		write down data on program exitting, and clear up other things
		**/
		void OnFinalLog()
		{
			int nBlock = (int)m_qLogBlock.size();
			if(nBlock > 0)
			{
				TRY_LOG(m_csMutex);
				m_fs.open(m_sLogName.c_str(), std::ios::app | std::ios::out );
				for(int peice = 0; peice < nBlock; peice++)
				{
					FmtLog fLog = m_qLogBlock.front();
					WriteLog(fLog.date,GetType(fLog.type),fLog.logstring);
					m_qLogBlock.pop();
				}
				m_fs.close();
				TRY_END(m_csMutex);
			}
		}
		/**
		Watcher thread, when queue size big than MAX_QUEUE_BUFFER, write down logs
		**/
		static DWORD WINAPI watcher(LPVOID lpParam)
		{
			//DWORD param[1] = {0};
			//memcpy_s(param,sizeof(param),lpParam,sizeof(param));
			//delete [] lpParam;
			//SimpleLog* pLog = (SimpleLog*)(param[0]);
			SimpleLog* pLog =  (SimpleLog*)(lpParam);
			DWORD sign ;
			while(!(pLog->m_exitTread))
			{
				pLog->WriteLogBlock();
				//::OutputDebugStringA("Thread\n");
				if(!(pLog->m_exitTread))
					sign = SuspendThread(pLog->m_hThread);
			}
			//::OutputDebugStringA("Exit Thread\n");
			return 0;
		}
		/**
		Begain Watcher thread
		**/
		void WatchThread()
		{
			DWORD threadId = 0;
			//DWORD* param = new DWORD[1]();
			//param[0] = (DWORD)this;
			//param[1] =(DWORD)&m_hThread;
			//param[2] =(DWORD)&m_exitTread ;
			//param[3] =(DWORD)&m_qLogBlock ;
			m_hThread = ::CreateThread(NULL, 0, watcher, (LPVOID)this, CREATE_SUSPENDED, &threadId );
			ResumeThread(m_hThread);
			//SuspendThread(m_hThread);
		}
		/**
		Create zip file for previous log
		**/
		void ZipPreviousLog(std::string& file_date)
		{
			HZIP hZip = NULL;
			ZRESULT ret  = ZR_OK;
			CreateDirectory(_T("previous_log"),NULL);
#ifndef _UNICODE
			std::string zipname = "previous_log";
			zipname.append(file_date);
			zipname.append(".zip");
			hZip = CreateZip((void*)zipname.c_str(),0,ZIP_FILENAME);
			if(!hZip)
				return;
			ret = ZipAdd(hZip,m_sLogName.c_str(),(void*)m_sLogName.c_str(),0,ZIP_FILENAME);
			CloseZip(hZip);
			std::string zippath = "previous_log\\";
			zippath.append(zipname);
			MoveFileEx(zipname.c_str(),zippath.c_str(),MOVEFILE_REPLACE_EXISTING);
#else
			std::wstring logname;
			std::wstring filedate;
			std::wstring zipname = _T("previous_log");
			StrConversion::ANSI_To_UNICODE_String(file_date.c_str(),filedate);
			zipname.append(filedate);
			zipname.append(_T(".zip"));

			hZip = CreateZip((void*)zipname.c_str(),0,ZIP_FILENAME);
			if(!hZip)
				return;
			StrConversion::ANSI_To_UNICODE_String(m_sLogName.c_str(),logname);
			ret = ZipAdd(hZip,logname.c_str(),(void*)logname.c_str(),0,ZIP_FILENAME);
			CloseZip(hZip);
			std::wstring zippath = _T("previous_log\\");
			zippath.append(zipname);
			MoveFileEx(zipname.c_str(),zippath.c_str(),MOVEFILE_REPLACE_EXISTING);
#endif
			if(ret != ZR_OK)
				m_sError = "Zip log file failed.";
		}

	private:
		/*static */std::queue<FmtLog> m_qLogBlock;  //log queue

	private:
		HANDLE m_hThread;
		std::vector<HANDLE> m_hClientThread; //waitting threads
		bool m_exitTread;
		bool m_bStarted;
		size_t m_nPushCount;
		CRITICAL_SECTION m_csMutex; //critical section
		std::fstream m_fs;
		std::string m_sLogName; //log file name
		std::string m_sError;
	};


} //end namespace