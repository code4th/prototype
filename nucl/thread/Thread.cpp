
#include "Thread.h"
//#include "PLT_Thread.h"


namespace nc{

	namespace generic{

		namespace thread{

			using nc::interlocked::readAcquire;
			using nc::interlocked::increment;
			using nc::interlocked::decrement;

			volatile nc::uint32_t Thread::masterThreadCount_ = 0;

			//ThreadManagerが、メインスレッドを登録する用の特殊なコンストラクタ。
			Thread::Thread()
				: threadID_( -1 )
				, description_( "MainThread" )
				//, priority_( 0 )
				//, idealProcessor_( -1 )
				//, threadProc_( 0 )
			{
				//スレッドが増えるわけではないので、マスターカウントは増えない。

				threadID_ = GetCurrentThreadId();

			}


			Thread::Thread(	const nc::string& description, SpThreadProc_t threadProc )
				: threadID_( -1 )
				, description_( description.c_str() )
				//, priority_( priority )
				//, idealProcessor_( idealProcessor )
				, threadProc_( threadProc )
			{
				//マスターカウント
				threadCount_ = interlocked::increment( reinterpret_cast<volatile int32_t*>( &masterThreadCount_ ) );
			}






		}// namespace thread

	}//namespace generic

}// namespace nc

#ifndef NC_PLATFORM_USE_INTRINSIC_THREAD

namespace nc{

	namespace platform{

		namespace thread{

			namespace detail{


				//現在のスレッドIDを取得する
				nc::thread::ThreadID_t getCurrentThreadID()
				{
					return (nc::uint32_t)pthread_self().p;
				}

				//--------------------------------------------------

				//ThreadManagerのinitializeから呼ばれる特殊なコンストラクタ
				Thread::Thread()
					: suspendCount_( 0 )
				{
					//自分自身のThreadHandleを取得
					hThread_ = pthread_self();


					/*
					switch( GetThreadPriority( hThread_ ) )
					{
					case THREAD_PRIORITY_TIME_CRITICAL:		priority_ = PRIORITY_TIME_CRITICAL; break;
					case THREAD_PRIORITY_HIGHEST:			priority_ = PRIORITY_HIGHEST; break;
					case THREAD_PRIORITY_ABOVE_NORMAL:		priority_ = PRIORITY_HIGH; break;
					case THREAD_PRIORITY_NORMAL:			priority_ = PRIORITY_NORMAL; break;
					case THREAD_PRIORITY_BELOW_NORMAL:		priority_ = PRIORITY_LOW; break;
					case THREAD_PRIORITY_LOWEST:			priority_ = PRIORITY_LOWEST; break;
					}
					*/

				}

				Thread::Thread(	const nc::string& description, SpThreadProc_t threadProc, nc::uint32_t priority )
					: nc::generic::thread::Thread( description, threadProc, priority )
					, suspendCount_( 0 )
				{
					//threadを構築する。
					pthread_create( &hThread_, NULL, &Thread::functionAdapter, this );

					{
						nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );
						isActive_ = true;
					}

					changePriority( priority_ );
				}

				Thread::~Thread()
				{

				}

				void * Thread::functionAdapter( void* vdParam )
				{
					nc::platform::thread::detail::Thread* thread = (nc::platform::thread::detail::Thread*)vdParam;

					/*
						pthread_detach 関数を呼ぶことで親スレッド（つまり親プロセス）から分離し、勝手に終る事を宣言しています。
						こうする事で親プロセスはスレッドの生死を管理しなくてもよくなります。

						とか言う記述を見たから入れてみたら、なんかデッドロックすんだけどｗ
					*/
					//pthread_detach( pthread_self( ));
					
					NC_TRACE( "ThreadMain enter: ID=%d, Desc=%s", thread->threadID(), thread->description_.c_str() );

					thread->threadProc_->threadMain();

					NC_TRACE( "ThreadMain exit: ID=%d, Desc=%s", thread->threadID(), thread->description_.c_str() );

					{
						nc::thread::ScopedLock< nc::thread::Mutex > lock( thread->mtex_ );
						thread->isActive_ = false;
					}

					pthread_exit(NULL);

					return 0;
				}

				Thread::E_THREAD_STATUS Thread::getStatus() const
				{
					return isActive_ ? STATUS_ACTIVE : STATUS_FINISH;
				}

				void Thread::abort()
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					if( getStatus() == STATUS_FINISH )return;

					pthread_cancel(hThread_);

				}

				void Thread::changePriority( nc::uint32_t priority )
				{
					nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );

					priority_ = priority;
					
					/*
					
						No Implement.

					*/

					//SetThreadPriority( hThread_, convertRealPriority( priority_ ) );
				}

			}// namespace detail

		}// namespace thread

	}//namespace generic

}// namespace nc

#endif

/*

Thread priority

POSIX defines a single contiguous range of numbers that determine a
thread's priority. Win32 defines priority classes and priority
levels relative to these classes. Classes are simply priority base
levels that the defined priority levels are relative to such that,
changing a process's priority class will change the priority of all
of it's threads, while the threads retain the same relativity to each
other.

A Win32 system defines a single contiguous monotonic range of values
that define system priority levels, just like POSIX. However, Win32
restricts individual threads to a subset of this range on a
per-process basis.

The following table shows the base priority levels for combinations
of priority class and priority value in Win32.

Process Priority Class               Thread Priority Level
-----------------------------------------------------------------
1 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_IDLE
1 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_IDLE
1 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_IDLE
1 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_IDLE
1 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_IDLE
2 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_LOWEST
3 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_BELOW_NORMAL
4 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_NORMAL
4 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_LOWEST
5 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_ABOVE_NORMAL
5 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_BELOW_NORMAL
5 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_LOWEST
6 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_HIGHEST
6 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_NORMAL
6 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_BELOW_NORMAL
7 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_ABOVE_NORMAL
7 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_NORMAL
7 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_LOWEST
8 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_HIGHEST
8 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_ABOVE_NORMAL
8 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_BELOW_NORMAL
8 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_LOWEST
9 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_HIGHEST
9 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_NORMAL
9 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_BELOW_NORMAL
10 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_ABOVE_NORMAL
10 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_NORMAL
11 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_HIGHEST
11 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_ABOVE_NORMAL
11 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_LOWEST
12 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_HIGHEST
12 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_BELOW_NORMAL
13 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_NORMAL
14 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_ABOVE_NORMAL
15 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_HIGHEST
15 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_TIME_CRITICAL
15 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_TIME_CRITICAL
15 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_TIME_CRITICAL
15 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_TIME_CRITICAL
15 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_TIME_CRITICAL
16 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_IDLE
17 REALTIME_PRIORITY_CLASS            -7
18 REALTIME_PRIORITY_CLASS            -6
19 REALTIME_PRIORITY_CLASS            -5
20 REALTIME_PRIORITY_CLASS            -4
21 REALTIME_PRIORITY_CLASS            -3
22 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_LOWEST
23 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_BELOW_NORMAL
24 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_NORMAL
25 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_ABOVE_NORMAL
26 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_HIGHEST
27 REALTIME_PRIORITY_CLASS             3
28 REALTIME_PRIORITY_CLASS             4
29 REALTIME_PRIORITY_CLASS             5
30 REALTIME_PRIORITY_CLASS             6
31 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_TIME_CRITICAL

Windows NT:  Values -7, -6, -5, -4, -3, 3, 4, 5, and 6 are not supported.


As you can see, the real priority levels available to any individual
Win32 thread are non-contiguous.

An application using pthreads-win32 should not make assumptions about
the numbers used to represent thread priority levels, except that they
are monotonic between the values returned by sched_get_priority_min()
and sched_get_priority_max(). E.g. Windows 95, 98, NT, 2000, XP make
available a non-contiguous range of numbers between -15 and 15, while
at least one version of WinCE (3.0) defines the minimum priority
(THREAD_PRIORITY_LOWEST) as 5, and the maximum priority
(THREAD_PRIORITY_HIGHEST) as 1.

Internally, pthreads-win32 maps any priority levels between
THREAD_PRIORITY_IDLE and THREAD_PRIORITY_LOWEST to THREAD_PRIORITY_LOWEST,
or between THREAD_PRIORITY_TIME_CRITICAL and THREAD_PRIORITY_HIGHEST to
THREAD_PRIORITY_HIGHEST. Currently, this also applies to
REALTIME_PRIORITY_CLASSi even if levels -7, -6, -5, -4, -3, 3, 4, 5, and 6
are supported.

If it wishes, a Win32 application using pthreads-win32 can use the Win32
defined priority macros THREAD_PRIORITY_IDLE through
THREAD_PRIORITY_TIME_CRITICAL.
*/

