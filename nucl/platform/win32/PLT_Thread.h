
#ifndef __NC_PLT_THREAD_h__
#define __NC_PLT_THREAD_h__

//#include "PlatformInclude.h"
#include "nucl/Types.h"

namespace nc{
	namespace platform{
		namespace thread{

			namespace detail{


				//現在のスレッドIDを取得する
				nc::thread::ThreadID_t getCurrentThreadID();

				//自threadを眠らせます。
				inline void sleepThread( nc::time::Clock clk )
				{

				}

				class Thread : public nc::generic::thread::Thread {
							
					friend class nc::thread::ThreadManager;

				public:
					enum E_THREAD_STATUS{
						STATUS_FINISH = 0,	//threadProcが実行を終了した
						STATUS_ACTIVE,		//アクティブ
						STATUS_SUSPEND,		//一時停止中
					};

					enum E_THREAD_PRIORITY{
						PRIORITY_TIME_CRITICAL = 0,			//最優先で実行されるべきスレッド

						PRIORITY_HIGHEST,					//優先度 +2
						PRIORITY_HIGH,						//優先度 +1
						PRIORITY_NORMAL,					//優先度 ±0
						PRIORITY_LOW,						//優先度 -1
						PRIORITY_LOWEST,					//優先度 -2

						DEFAULT_PRIORITY = PRIORITY_NORMAL, //デフォルトの優先度
					};

				private:

					E_THREAD_PRIORITY		priority_;			//優先度
					HANDLE					hThread_;
					int32_t					suspendCount_;
					nc::uint8_t				idealProcessor_;	//理想的なプロセッサー番号

					static DWORD WINAPI functionAdapter(LPVOID vdParam);

					int convertRealPriority( int threadPriority ) const;
				
				protected:
					Thread();

				public:

					Thread(
						const nc::string& description,
						SpThreadProc_t threadProc,
						E_THREAD_PRIORITY priority = DEFAULT_PRIORITY,
						uint8_t idealProcessor = -1,
						E_THREAD_STATUS createStatus = STATUS_SUSPEND ) ;

					~Thread();

					E_THREAD_STATUS getStatus() const;

					void changePriority( E_THREAD_PRIORITY priority );

					void setIdealProcessor( uint8_t idealProcessor );

					//threadを停止
					void abort();

					//Win32拡張---------------

					//threadをアクティブ化
					void activate();

					//threadを一時停止
					void suspend();
				};

			}//namespace detail
		}// namespace thread
	}// namespace platform
}// namespace nc

namespace nc{
	namespace thread{
		using nc::platform::thread::detail::Thread;
		using nc::platform::thread::detail::getCurrentThreadID;
		using nc::platform::thread::detail::sleepThread;
		
	}//namespace thread
}//namespace nc

#endif // __NC_PLT_THREAD_h__
