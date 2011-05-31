#pragma once

#include <Windows.h>
#include <process.h>
#include "kickflipSmartPointer.h"
#include <string>

/*
Threadはスレッドのハンドルとして機能する
実際のスレッドの中身はThreadFunctionを継承して作成するタイプとしよう

pThread = Thread::Create(new ThreadFunction(false));
pThread = Thread::Create(new ThreadFunction(true));
*/


namespace kickflip
{
	SmartPtr(Thread);

	// スレッドからコールされるオブジェクト
	SmartPtr(ThreadFunction);
	class ThreadFunction :public ReferenceObject
	{
	private:
		friend class Thread;
	public:
		ThreadFunction(bool bIsLoop = false, unsigned int uiSleepTime = 0 )
			: m_bIsLoop(bIsLoop)
			, m_uiSleepTime(uiSleepTime)
			, m_bComplete(false)
		{
		}
		virtual ~ThreadFunction()
		{
		}
		virtual unsigned int Execute(Thread* pThread) = 0;
	private:
		void StopLoop() { m_bIsLoop = false; }
		bool IsComplate() { return m_bComplete; }
		virtual bool IsLoop() {return m_bIsLoop;}
		virtual unsigned int GetSleepTime() { return m_uiSleepTime; }
	private:
		bool m_bIsLoop;
		unsigned int m_uiSleepTime;
		bool m_bComplete;

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
		static ThreadRPtr Create( ThreadFunctionRPtr rpThreadFunction, Priority ePriority = NORMAL, unsigned int uiStackSize = 0 )
		{
			Thread* pThread = new Thread(rpThreadFunction, uiStackSize);

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
		Thread( ThreadFunctionRPtr rpThreadFunction, unsigned int uiStackSize )
			: m_hThread(NULL)
			, m_rpThreadFunction(rpThreadFunction)
			, m_ePriority(NORMAL)
			, m_uiStackSize(uiStackSize)
			, m_uiReturn(0)
			, m_eStatus(UNKNOWN)
		{}
	public:
		~Thread()
		{
			WaitForComplete();

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
			int iRet = SuspendThread(m_hThread);
			if ( -1 != iRet ) m_eStatus = SUSPENDED;
			return iRet;
		}
		int Resume()
		{
			int iSuspendCountLast = ResumeThread(m_hThread);
			switch (iSuspendCountLast)
			{
			case -1: 
				break;
			case 0: 
			case 1:
				m_eStatus = RUNNING;
				break;
			default:
				m_eStatus = SUSPENDED;
				break;
			}
			return iSuspendCountLast;
		}

		bool WaitForComplete()
		{
			switch(m_eStatus)
			{
			case RUNNING:
				while(SUSPENDED != m_eStatus) Suspend();
				m_rpThreadFunction->StopLoop();
				Resume();
				WaitForSingleObject(m_hThread, INFINITE);
				m_eStatus = COMPLETE;
				return true;
			}

			return false;
		}
		ThreadFunctionRPtr GetFunction() { return m_rpThreadFunction; }

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

			ThreadFunction* pObject = pThis->m_rpThreadFunction;
			while(true)
			{
				pThis->m_uiReturn = pObject->Execute(pThis);
				if(false == pObject->IsLoop()) break;
				Sleep(pObject->GetSleepTime());
			}

			pThis->m_rpThreadFunction->m_bComplete = true;
			pThis->m_rpThreadFunction = NULL;
			pThis->m_eStatus = COMPLETE;

			return pThis->m_uiReturn;
		}

	protected:
		HANDLE m_hThread;
		ThreadFunctionRPtr m_rpThreadFunction;
		Priority m_ePriority;
		unsigned int m_uiStackSize;

		volatile unsigned int m_uiReturn;
		volatile Status m_eStatus;
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

		class Lock
		{
			CRITICAL_SECTION	m_cs;
		public:
			Lock()
			{
				InitializeCriticalSection(&m_cs);
			}
			~Lock()
			{
				DeleteCriticalSection(&m_cs);
			}

			void Enter()
			{
				EnterCriticalSection(&m_cs);
			}
			bool Check()
			{
				return TryEnterCriticalSection(&m_cs)!=FALSE;
			}
			void Exit()
			{
				LeaveCriticalSection(&m_cs);
			}

		};

}

