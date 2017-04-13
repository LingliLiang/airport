#pragma once 

#include "system.h"
class CJob
{
public:
  /*!
   \brief Priority levels for jobs, specified by clients when adding jobs to the CJobManager.
   \sa CJobManager
   */
  enum PRIORITY {
    PRIORITY_LOW_PAUSABLE = 0,
    PRIORITY_LOW,
    PRIORITY_NORMAL,
    PRIORITY_HIGH,
    PRIORITY_DEDICATED, // will create a new worker if no worker is available at queue time
  };
  CJob() { m_hJob = NULL;m_callback = NULL; m_tId = 0;};

  /*!
   \brief Destructor for job objects.
   
   Jobs are destroyed by the CJobManager after the OnJobComplete() callback is complete.
   CJob subclasses  should therefore supply a virtual destructor to cleanup any memory allocated by
   complete or cancelled jobs.
   
   \sa CJobManager
   */
  virtual ~CJob() 
  {
	  ::CloseHandle(m_hJob);
  };

  /*!
   \brief Main workhorse function of CJob instances
   
   All CJob subclasses must implement this function, performing all processing.  Once this function
   is complete, the OnJobComplete() callback is called, and the job is then destroyed.
   
   \sa CJobManager, IJobCallback::OnJobComplete()
   */
  virtual bool DoWork() = 0;  // function to do the work

  /*!
   \brief Function that returns the type of job.
   
   CJob subclasses may optionally implement this function to specify the type of job.
   This is useful for the CJobManager::AddLIFOJob() routine, which preempts similar jobs
   with the new job.
   
   \return a unique character string describing the job.
   \sa CJobManager
   */
  virtual const char *GetType() const { return ""; };

  virtual bool operator==(const CJob* job) const
  {
    return false;
  }

  /*!
   \brief Function for longer jobs to report progress and check whether they have been cancelled.
   
   Jobs that contain loops that may take time should check this routine each iteration of the loop,
   both to (optionally) report progress, and to check for cancellation.
   
   \param progress the amount of the job performed, out of total. 
   \param total the total amount of processing to be performed
   \return if true, the job has been asked to cancel.
   
   \sa IJobCallback::OnJobProgress()
   */
  virtual bool ShouldCancel(unsigned int progress, unsigned int total) const{return false;}


  static unsigned long WINAPI JobWork(LPVOID params)
  {
	  CJob* p  = static_cast<CJob*>(params);
	  if(p)
	  {
		  p->DoWork();
		  delete p;
	  }
	  return 0;
  }

  static void PostJob(CJob* p)
  {
	 p->m_hJob = ::CreateThread(0, 0, JobWork, p, 0, &p->m_tId);
  }
  HANDLE GetHandle(){return m_hJob;}
  unsigned long GetTId(){return m_tId;}
private:
	HANDLE m_hJob;
	 unsigned long m_tId;
  //friend class CJobManager;
  CJob *m_callback;
};

