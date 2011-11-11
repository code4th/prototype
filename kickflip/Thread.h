#pragma once

#include <Windows.h>
#include <process.h>
#include "SmartPointer.h"
#include <string>

/*
Threadはスレッドのハンドルとして機能する
実際のスレッドの中身はThreadFunctionを継承して作成するタイプとしよう

pThread = Thread::Create(new ThreadFunction(false));
pThread = Thread::Create(new ThreadFunction(true));
*/


namespace kickflip
{
	class Semaphore
	{
	public:
		Semaphore()
			: m_iCount(0)
			, m_iMaxCount(1)
		{
			m_hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
		}
		//---------------------------------------------------------------------------
		Semaphore(int iCount)
			: m_iCount(iCount)
			, m_iMaxCount(iCount + 1)
		{
			m_hSemaphore = CreateSemaphore(NULL, m_iCount, m_iMaxCount, NULL);
		}
		//---------------------------------------------------------------------------
		Semaphore(int iCount, int iMaxCount)
			: m_iCount(iCount)
			, m_iMaxCount(iMaxCount)
		{
			m_hSemaphore = CreateSemaphore(NULL, m_iCount, m_iMaxCount, NULL);
		}
		//---------------------------------------------------------------------------
		~Semaphore()
		{
			CloseHandle(m_hSemaphore);
		}
		//---------------------------------------------------------------------------
		int GetCount()
		{
			return m_iCount;
		}
		//---------------------------------------------------------------------------
		int GetMaxCount()
		{
			return m_iMaxCount;
		}
		//---------------------------------------------------------------------------
		int Signal()
		{
			BOOL bIsRelease = ReleaseSemaphore(m_hSemaphore, 1, NULL);
			if(TRUE == bIsRelease)
			{
				InterlockedIncrement((LONG*)&m_iCount);
			}
			return m_iCount;
		}
		//---------------------------------------------------------------------------
		int Wait()
		{
			WaitForSingleObject(m_hSemaphore, INFINITE);
			return InterlockedDecrement((LONG*)&m_iCount);
		}

	protected:
		volatile int m_iCount;
		int m_iMaxCount;

		HANDLE m_hSemaphore;

	};

	class Lock
	{
	private:
		CRITICAL_SECTION	m_cs;
	public:
		Lock()	{ InitializeCriticalSection(&m_cs); }
		~Lock() { DeleteCriticalSection(&m_cs);		}
		void Enter(){ EnterCriticalSection(&m_cs);	}
		void Exit()	{ LeaveCriticalSection(&m_cs);	}
		bool Check(){ return FALSE!=TryEnterCriticalSection(&m_cs); }

	};

	SmartPtr(Thread);

	// スレッドからコールされるオブジェクト
	SmartPtr(ThreadFunction);
	class ThreadFunction :public ReferenceObject
	{
	private:
		friend class Thread;
	public:
		ThreadFunction()
			: m_bIsContinue(false)
		{
		}
		virtual ~ThreadFunction()
		{
		}
		virtual unsigned int Execute(Thread* pThread) = 0;
		void Lock() { m_kLock.Enter(); }
		void Unlock() { m_kLock.Exit(); }

	private:
		kickflip::Lock m_kLock;
		volatile bool m_bIsContinue;
		bool IsContinue(){ return m_bIsContinue; }
		void Continue(){ m_bIsContinue = true; }

	protected:
		void StopContinue(){ m_bIsContinue = false; }

	};

	class Thread :public ReferenceObject
	{
	public:
		enum Status
		{
			UNKNOWN     = -1,
			RUNNING     = 0,
			SUSPENDED,
			COMPLETE
		};
		enum Priority
		{
			IDLE = 0,               // Base priority level
			LOWEST,                 // 2 points below normal
			BELOW_NORMAL,           // 1 point below normal
			NORMAL,
			ABOVE_NORMAL,           // 1 point above normal
			HIGHEST,                // 2 points above normal
			TIME_CRITICAL,          // Absolute highest OS priority available
			NUM_PRIORITIES
		};

	public:
		static ThreadRPtr Create( ThreadFunctionRPtr rpThreadFunction, bool bIsLoop = false, bool uiSleepTime = 0, Priority ePriority = NORMAL, unsigned int uiStackSize = 0)
		{
			Thread* pThread = new Thread(rpThreadFunction, bIsLoop, uiSleepTime, uiStackSize);

			if (NULL != pThread)
			{
				if (!pThread->CreateThread_())
				{
					delete pThread;
					pThread = 0;
				}else{
					pThread->SetPriority(ePriority);
				}
			}
			return pThread;
		}
	protected:
		Thread( ThreadFunctionRPtr rpThreadFunction, bool bIsLoop, bool uiSleepTime, unsigned int uiStackSize )
			: m_hThread(NULL)
			, m_rpThreadFunction(rpThreadFunction)
			, m_ePriority(NORMAL)
			, m_uiStackSize(uiStackSize)
			, m_uiReturn(0)
			, m_eStatus(UNKNOWN)
			, m_bIsLoop(bIsLoop)
			, m_uiSleepTime(uiSleepTime)
			, m_kComplete(0)
			, m_kStart(0)

		{
		    SignalComplete();
			SignalStart();
		}
	public:
		~Thread()
		{
			SignalStart();
		    Shutdown();
			WaitForComplete();
			m_rpThreadFunction = NULL;

			if (m_hThread) CloseHandle(m_hThread);
			m_hThread = 0;
		}
		bool SetPriority(Priority ePriority)
		{
			if (m_ePriority != ePriority)
			{
				int iPriority;

				switch (ePriority)
				{
				case ABOVE_NORMAL:
					iPriority = THREAD_PRIORITY_ABOVE_NORMAL;
					break;
				case BELOW_NORMAL:
					iPriority = THREAD_PRIORITY_BELOW_NORMAL;
					break;
				case HIGHEST:
					iPriority = THREAD_PRIORITY_HIGHEST;
					break;
				case IDLE:
					iPriority = THREAD_PRIORITY_IDLE;
					break;
				case LOWEST:
					iPriority = THREAD_PRIORITY_LOWEST;
					break;
				case NORMAL:
					iPriority = THREAD_PRIORITY_NORMAL;
					break;
				case TIME_CRITICAL:
					iPriority = THREAD_PRIORITY_TIME_CRITICAL;
					break;
				default:
					return false;
				}
				if (!SetThreadPriority(m_hThread, iPriority))
					return false;

				m_ePriority = ePriority;
			}

			return true;
		}		
		int Suspend()
		{
			if (m_hThread == 0)
				return -1;

			int iRet = SuspendThread(m_hThread);
			if (iRet != -1)
				m_eStatus = SUSPENDED;
			return iRet;

		}
		int Resume()
		{
			if (m_hThread == 0)
				return -1;

			int iPreviousSuspendCount = ResumeThread(m_hThread);
			switch (iPreviousSuspendCount)
			{
			case -1: 
				break;
			case 0: // fall through
			case 1:
				m_eStatus = RUNNING;

				break;
			default:
				m_eStatus = SUSPENDED;
			}
			return iPreviousSuspendCount;
		}
		void Lock() { if(NULL!=m_rpThreadFunction) m_rpThreadFunction->Lock();}
		void Unlock() { if(NULL!=m_rpThreadFunction) m_rpThreadFunction->Unlock();}

		bool WaitForComplete()
		{
			if (m_eStatus == RUNNING)
			{
				WaitForSingleObject(m_hThread, INFINITE);

				m_eStatus = COMPLETE;

				return true;
			}

			return false;
		}

		ThreadFunctionRPtr GetThreadFunction() { return m_rpThreadFunction; }

		void SignalStart()
		{
			m_kStart.Signal();
		}

	private:
		bool CreateThread_()
		{
			if(NULL == m_rpThreadFunction) return false;
			m_hThread = (HANDLE)_beginthreadex(NULL, m_uiStackSize, callThreadObject, this, CREATE_SUSPENDED, NULL);

			if (0 == m_hThread) return false;

			m_eStatus = SUSPENDED;

			/*
			m_ePriority = NORMAL;

			// Initialize the thread affinity based on the process affinity
			DWORD_PTR processAffinityMask;
			DWORD_PTR systemAffinityMask;

			if (GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask,
			&systemAffinityMask))
			{
			m_kAffinity.SetAffinityMask(
			static_cast<unsigned int>(processAffinityMask));
			}
			*/
			return true;
		}
		static unsigned int WINAPI callThreadObject(void* p)
		{

			Thread* pThis = reinterpret_cast<Thread*>(p);
			ThreadFunction* pThreadFunction = pThis->m_rpThreadFunction;
			pThis->WaitStart();
			while(pThis->IsLoop())
			{
				pThreadFunction->Continue();
				pThis->m_uiReturn = pThreadFunction->Execute(pThis);				
				if(pThreadFunction->IsContinue())
				{
					Sleep(pThis->GetSleepTime());
					pThis->SignalStart();
				}
				pThis->SignalComplete();
				pThis->WaitStart();
			}

			// If last loop was set, then thread is terminated by some controlling 
			// thread.
			pThis->SignalComplete();
			pThis->m_rpThreadFunction = NULL;
			pThis->m_eStatus = COMPLETE;

			return pThis->m_uiReturn;

			return pThis->m_uiReturn;

		}
		void StopLoop() { m_bIsLoop = false; }
		virtual bool IsLoop() {return m_bIsLoop;}
		virtual unsigned int GetSleepTime() { return m_uiSleepTime; }

		void WaitStart()
		{
			m_kStart.Wait();
		}
		void WaitComplete()
		{
			m_kComplete.Wait();
		}
		void SignalComplete()
		{
			m_kComplete.Signal();
		}
		void Shutdown()
		{
			// SetLastLoop() is not threadsafe to be called in the middle of a loop,
			// because "DoLoop(); Shutdown();" might not iterate if SetLastLoop()
			// gets set before the loop checks it.
			WaitComplete();
			SignalComplete();
			StopLoop();

			// Now that we're at the end, signal start to finish the thread
			SignalStart();
		}

	protected:
		HANDLE m_hThread;
		ThreadFunctionRPtr m_rpThreadFunction;
		Priority m_ePriority;
		unsigned int m_uiStackSize;

		volatile unsigned int m_uiReturn;
		volatile Status m_eStatus;

		volatile bool m_bIsLoop;
		unsigned int m_uiSleepTime;

		kickflip::Semaphore m_kComplete;
		kickflip::Semaphore m_kStart;


	};


	/*
	class MyThreadObject : public ThreadObject
	{
		// スレッド処理
		virtual unsigned int ThreadFunction()
		{
			return 0;
		}

		// スレッド完了受信
		virtual void ThreadComplate()
		{
		}

	}

	ThreadObjectRPtr rpThreadObject = new ThreadObject();
	rpThreadObject->ThreadStart();
	rpThreadObject->IsThreadComplate();

	*/


}

