
#include "nucl/thread/Thread.h"
#include "PLT_Thread.h"

namespace nc{

	namespace platform{

		namespace thread{

			namespace detail{

				//現在のスレッドIDを取得する
				nc::thread::ThreadID_t getCurrentThreadID()
				{
					return (nc::thread::ThreadID_t)GetCurrentThreadId();
				}

				//--------------------------------------------------

				//ThreadManagerのinitializeから呼ばれる特殊なコンストラクタ
				Thread::Thread()
					: suspendCount_( 0 )
				{
					//自分自身のThreadHandleを取得
					DuplicateHandle(
						GetCurrentProcess(),
						GetCurrentThread(),
						GetCurrentProcess(),
						&hThread_,
						0,			//DUPLICATE_SAME_ACCESSにより無視される
						TRUE,		//継承可
						DUPLICATE_SAME_ACCESS);

					switch( GetThreadPriority( hThread_ ) )
					{
					case THREAD_PRIORITY_TIME_CRITICAL:		priority_ = PRIORITY_TIME_CRITICAL; break;
					case THREAD_PRIORITY_HIGHEST:			priority_ = PRIORITY_HIGHEST; break;
					case THREAD_PRIORITY_ABOVE_NORMAL:		priority_ = PRIORITY_HIGH; break;
					case THREAD_PRIORITY_NORMAL:			priority_ = PRIORITY_NORMAL; break;
					case THREAD_PRIORITY_BELOW_NORMAL:		priority_ = PRIORITY_LOW; break;
					case THREAD_PRIORITY_LOWEST:			priority_ = PRIORITY_LOWEST; break;
					}						
				}

				Thread::Thread(
					const nc::string& description,
					SpThreadProc_t threadProc,
					E_THREAD_PRIORITY priority,
					uint8_t idealProcessor,
					E_THREAD_STATUS createStatus )

				: nc::generic::thread::Thread( description, threadProc )
				, suspendCount_( 0 )
				{
					//念の為…
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					idealProcessor_ = idealProcessor;
				
					//Suspend起動
					if( createStatus == STATUS_SUSPEND )
					{
						++suspendCount_;
					}

					//threadを構築する。
					hThread_ = CreateThread(
						NULL,
						0,
						&Thread::functionAdapter,
						(LPVOID)this,
						createStatus == STATUS_SUSPEND ? CREATE_SUSPENDED : 0,
						(LPDWORD)&threadID_);

					changePriority( priority );
					setIdealProcessor( idealProcessor_ );
				}

				Thread::~Thread()
				{

				}

				DWORD WINAPI Thread::functionAdapter( LPVOID vdParam )
				{
					nc::thread::Thread* thread = (nc::thread::Thread*)vdParam;

					NC_TRACE( "ThreadMain enter: ID=%d, Desc=%s", thread->threadID(), thread->description_.c_str() );

					thread->threadProc_->threadMain();

					NC_TRACE( "ThreadMain exit: ID=%d, Desc=%s", thread->threadID(), thread->description_.c_str() );

					return 0;
				}

				Thread::E_THREAD_STATUS Thread::getStatus() const
				{
					//isFinish
					{
						DWORD exitCode;
						GetExitCodeThread( hThread_, &exitCode );

						if( exitCode != STILL_ACTIVE ) return STATUS_FINISH;
					}

					//isActive
					{
						interlocked::readAcquire( (int32_t volatile*)( &suspendCount_ ) );
						if( suspendCount_ <= 0 ) return STATUS_ACTIVE;
					}

					return STATUS_SUSPEND;
				}

				void Thread::activate()
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					E_THREAD_STATUS status = getStatus();
					if( status == STATUS_FINISH )return;
					if( status == STATUS_ACTIVE )return;

					NC_TRACE( "ActivateThread : ID=%d, Desc=%s, SuspendCount=%d", threadID(), description_.c_str(), suspendCount_ );

					if( --suspendCount_ == 0 )
					{
						NC_TRACE( "ActivateThread : ID=%d, Desc=%s, [Activate NOW] ", threadID(), description_.c_str() );
						ResumeThread( hThread_ );
					}
				}

				void Thread::abort()
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					if( getStatus() == STATUS_FINISH )return;

					TerminateThread( hThread_, 0 );
				}

				void Thread::suspend()
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					if( getStatus() == STATUS_FINISH )return;

					NC_TRACE( "SuspendThread : ID=%d, Desc=%s, SuspendCount=%d", threadID(), description_.c_str(), suspendCount_ );

					//初めてのsuspend
					if( suspendCount_++ == 0 )
					{
						NC_TRACE( "SuspendThread : ID=%d, Desc=%s, [Suspend NOW]", threadID(), description_.c_str() );
						SuspendThread( hThread_ );
					}
				}

				int Thread::convertRealPriority( int threadPriority ) const
				{
					switch( threadPriority )
					{
					case PRIORITY_TIME_CRITICAL:	return THREAD_PRIORITY_TIME_CRITICAL;
					case PRIORITY_HIGHEST:			return THREAD_PRIORITY_HIGHEST;
					case PRIORITY_HIGH:				return THREAD_PRIORITY_ABOVE_NORMAL;
					case PRIORITY_NORMAL:			return THREAD_PRIORITY_NORMAL;
					case PRIORITY_LOW:				return THREAD_PRIORITY_BELOW_NORMAL;
					case PRIORITY_LOWEST:			return THREAD_PRIORITY_LOWEST;
					}
					return THREAD_PRIORITY_NORMAL;
				}

				void Thread::changePriority( E_THREAD_PRIORITY priority )
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					priority_ = priority;
					SetThreadPriority( hThread_, convertRealPriority( priority_ ) );
				}

				void Thread::setIdealProcessor( uint8_t idealProcessor )
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					idealProcessor_ = idealProcessor;
					SetThreadIdealProcessor( hThread_, idealProcessor_ );
				}

			}//namespace detail
		}// namespace thread
	}// namespace platform
}// namespace nc

