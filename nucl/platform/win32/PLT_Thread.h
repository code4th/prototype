
#ifndef __NC_PLT_THREAD_h__
#define __NC_PLT_THREAD_h__

//#include "PlatformInclude.h"
#include "nucl/Types.h"

namespace nc{
	namespace platform{
		namespace thread{

			namespace detail{


				//���݂̃X���b�hID���擾����
				nc::thread::ThreadID_t getCurrentThreadID();

				//��thread�𖰂点�܂��B
				inline void sleepThread( nc::time::Clock clk )
				{

				}

				class Thread : public nc::generic::thread::Thread {
							
					friend class nc::thread::ThreadManager;

				public:
					enum E_THREAD_STATUS{
						STATUS_FINISH = 0,	//threadProc�����s���I������
						STATUS_ACTIVE,		//�A�N�e�B�u
						STATUS_SUSPEND,		//�ꎞ��~��
					};

					enum E_THREAD_PRIORITY{
						PRIORITY_TIME_CRITICAL = 0,			//�ŗD��Ŏ��s�����ׂ��X���b�h

						PRIORITY_HIGHEST,					//�D��x +2
						PRIORITY_HIGH,						//�D��x +1
						PRIORITY_NORMAL,					//�D��x �}0
						PRIORITY_LOW,						//�D��x -1
						PRIORITY_LOWEST,					//�D��x -2

						DEFAULT_PRIORITY = PRIORITY_NORMAL, //�f�t�H���g�̗D��x
					};

				private:

					E_THREAD_PRIORITY		priority_;			//�D��x
					HANDLE					hThread_;
					int32_t					suspendCount_;
					nc::uint8_t				idealProcessor_;	//���z�I�ȃv���Z�b�T�[�ԍ�

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

					//thread���~
					void abort();

					//Win32�g��---------------

					//thread���A�N�e�B�u��
					void activate();

					//thread���ꎞ��~
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
