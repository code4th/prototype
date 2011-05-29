#pragma once

#include <Windows.h>
#include <process.h>
#include "kickflipSmartPointer.h"
#include <string>

/*
Thread�̓X���b�h�̃n���h���Ƃ��ċ@�\����
���ۂ̃X���b�h�̒��g��ThreadFunction���p�����č쐬����^�C�v�Ƃ��悤

pThread = Thread::Create(new ThreadFunction(false));
pThread = Thread::Create(new ThreadFunction(true));
*/


namespace kickflip
{
	SmartPtr(Thread);

	// �X���b�h����R�[�������I�u�W�F�N�g
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
			RUNNING     = 0,
			SUSPENDED,
			COMPLETE
		};
	public:
		static ThreadRPtr Create( ThreadFunctionRPtr rpThreadObject, unsigned int uiStackSize = 0 )
		{
			Thread* pThread = new Thread(rpThreadObject, uiStackSize);

			if (NULL != pThread)
			{
				if (!pThread->CreateThread_())
				{
					delete pThread;
					pThread = 0;
				}
			}
			return pThread;
		}
		~Thread()
		{
			WaitForComplete();

			if (m_hThread) CloseHandle(m_hThread);
			m_hThread = 0;
		}
		/*
		bool SetPriority(Thread::Priority ePriority)
		{
		return SystemSetPriority(ePriority);
		}
		*/
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
				m_rpThreadObject->StopLoop();
				Resume();
				WaitForSingleObject(m_hThread, INFINITE);
				m_eStatus = COMPLETE;
				return true;
			}

			return false;
		}

	protected:
		Thread( ThreadFunctionRPtr rpThreadObject, unsigned int uiStackSize )
			: m_hThread(NULL)
			, m_rpThreadObject(rpThreadObject)
			, m_uiStackSize(uiStackSize)
		{
		}
	private:
		bool CreateThread_()
		{
			if(NULL == m_rpThreadObject) return false;
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

			ThreadFunction* pObject = pThis->m_rpThreadObject;
			while(true)
			{
				pThis->m_uiReturn = pObject->Execute(pThis);
				if(false == pObject->IsLoop()) break;
				Sleep(pObject->GetSleepTime());
			}

			pThis->m_rpThreadObject->m_bComplete = true;
			pThis->m_rpThreadObject = NULL;
			pThis->m_eStatus = COMPLETE;

			return pThis->m_uiReturn;
		}

	protected:
		ThreadFunctionRPtr m_rpThreadObject;
		HANDLE m_hThread;
		unsigned int m_uiStackSize;
		volatile unsigned int m_uiReturn;
		volatile Status m_eStatus;
	};


	/*
	class MyThreadObject : public ThreadObject
	{
		// �X���b�h����
		virtual unsigned int ThreadFunction()
		{
			return 0;
		}

		// �X���b�h������M
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

