
#ifndef __NC_THREAD_h__
#define __NC_THREAD_h__

#include "nucl/Uncopyable.h"
//#include "Interlocked.h"
#include "Mutex.h"
#include "ThreadFunction.h"
#include "nucl/string/string.h"
#include "Assert.h"
#include "nucl/smartptr/SmartPtr.h"
#include "nucl/TLS.h"

#include <list>



#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_THREAD

//各プラットフォーム毎の実装
#include "PLT_ThreadID_t.h"

/*
namespace nc{
	namespace platform{
		namespace thread{
			typedef nc::uint32_t ThreadID_t;
		}//namespace thread
	}//namespace paltform
}//namespace nc
*/


namespace nc{
	namespace thread{
		using nc::platform::thread::ThreadID_t;
	}//namespace thread
}//namespace nc


#else


namespace nc{

	namespace generic{

		namespace thread{

			typedef nc::uint32_t ThreadID_t;

		}//namespace thread

	}//namespace generic

}//namespace nc


namespace nc{

	namespace thread{

		using nc::generic::thread::ThreadID_t;

	}//namespace thread

}//namespace nc

#endif


namespace nc{
	namespace thread{

		class ThreadManager;

	}//namespace thread

	namespace generic{

		namespace thread{
		
			//現在のスレッドIDを取得する
			//uint32_t getCurrentThreadID();
		
			//自threadを眠らせます。
			//inline void sleepThread( nc::time::Clock clk )
			//{
			//
			//}


			//ThreadContextList

			class Thread : private Uncopyable {

				friend class ThreadManager;
	
			public:
				enum E_THREAD_STATUS{
					STATUS_FINISH = 0,	//threadProcが実行を終了した
					STATUS_ACTIVE,		//アクティブ
				};

				typedef nc::SharedPtr< nc::thread::ThreadProc > SpThreadProc_t;

			private:
				static volatile nc::uint32_t	masterThreadCount_;	//全スレッド共通のカウント
				nc::uint32_t					threadCount_;		//何カウント目にこのスレッドが作られたか


			protected:
				nc::thread::ThreadID_t			threadID_;			//スレッドID
				nc::CString_t					description_;		//スレッドの説明

				SpThreadProc_t					threadProc_;		//スレッドにして呼び出す関数
				nc::thread::Mutex				mtex_;

				bool							isActive_;

				//ThreadManagerが、メインスレッドを登録する用の特殊なコンストラクタ。
				//現在実行中のスレッドを元に、Threadを作る。
				//スレッド優先度、理想担当プロセッサ等は、Thread側が決めてよい
				//descriptionは"MainThread"固定
				//threadProc_は任意の無効値で良い
				Thread();

			public:
				Thread(
					const nc::string& description,			//スレッドの説明	
					SpThreadProc_t	threadProc );			//スレッドで実行する関数
					

				~Thread() {};

				//ThreadIDを返します。
				//nc::thread::ThreadID_t threadID() const;

				nc::thread::ThreadID_t Thread::threadID() const
				{
					return threadID_;
				}

				inline const char* description() const
				{
					return description_.c_str();
				}

				inline SpThreadProc_t threadProc()
				{
					return threadProc_;
				}

				//現在のThreadのステータスを返します。
				inline E_THREAD_STATUS getStatus() const
				{
					return isActive_ ? STATUS_ACTIVE : STATUS_FINISH;
					///*not implement ever*/ NC_HALT(); return STATUS_FINISH;
				}

				//優先度を変更します。				
//				inline void changePriority( int priority )
//				{ 
//					priority_ = priority;
					/*not implement ever*/ //NC_HALT();
//				}

				//threadを停止
				inline void abort()
				{
					/*not implement ever*/ NC_HALT();
				}
			};

		}//namespace thread

	}//namespace generic

}//namespace nc



#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_THREAD

//各プラットフォーム毎の実装
#include "PLT_Thread.h"

#else

#include "pthread.h"

namespace nc{

	namespace platform{

		namespace thread{

			namespace detail{

				//現在のスレッドIDを取得する
				nc::thread::ThreadID_t getCurrentThreadID();

				/**
				 *	pthreadをベースとしたThreadクラス。
				 *	
				 *	pthreadとは
				 *	http://codezine.jp/article/detail/1892
				 *	
				 *	全てのpthreadの関数リファレンス
				 *	http://slashdot.jp/journal.pl?op=display&uid=7255&id=117590
				 */
				class Thread : public nc::generic::thread::Thread {

					friend class nc::thread::ThreadManager;

					pthread_t				hThread_;
					int32_t					suspendCount_;

					bool					isActive_;

					//! pthread用の
					static void * functionAdapter(void* vdParam);

				protected:
					Thread();

				public:

					Thread(
						const nc::string& description,
						SpThreadProc_t threadProc,
						nc::uint32_t priority = 0 ) ;

					~Thread();

					void activate()
					{
						/* not implement for pthread. */
					}

					E_THREAD_STATUS getStatus() const;

					void changePriority( nc::uint32_t priority );

					//threadを停止
					void abort();

				};

			}//namespace detail 

		}//namespace thread

	}//namespace platform

}//namespace nc

namespace nc{
	namespace thread{
		using nc::platform::thread::detail::Thread;
		using nc::platform::thread::detail::getCurrentThreadID;
	}
}

#endif //NC_PLATFORM_USE_INTRINSIC_THREAD

#endif // __NC_THREAD_h__
