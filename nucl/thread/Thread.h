
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

//�e�v���b�g�t�H�[�����̎���
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
		
			//���݂̃X���b�hID���擾����
			//uint32_t getCurrentThreadID();
		
			//��thread�𖰂点�܂��B
			//inline void sleepThread( nc::time::Clock clk )
			//{
			//
			//}


			//ThreadContextList

			class Thread : private Uncopyable {

				friend class ThreadManager;
	
			public:
				enum E_THREAD_STATUS{
					STATUS_FINISH = 0,	//threadProc�����s���I������
					STATUS_ACTIVE,		//�A�N�e�B�u
				};

				typedef nc::SharedPtr< nc::thread::ThreadProc > SpThreadProc_t;

			private:
				static volatile nc::uint32_t	masterThreadCount_;	//�S�X���b�h���ʂ̃J�E���g
				nc::uint32_t					threadCount_;		//���J�E���g�ڂɂ��̃X���b�h�����ꂽ��


			protected:
				nc::thread::ThreadID_t			threadID_;			//�X���b�hID
				nc::CString_t					description_;		//�X���b�h�̐���

				SpThreadProc_t					threadProc_;		//�X���b�h�ɂ��ČĂяo���֐�
				nc::thread::Mutex				mtex_;

				bool							isActive_;

				//ThreadManager���A���C���X���b�h��o�^����p�̓���ȃR���X�g���N�^�B
				//���ݎ��s���̃X���b�h�����ɁAThread�����B
				//�X���b�h�D��x�A���z�S���v���Z�b�T���́AThread�������߂Ă悢
				//description��"MainThread"�Œ�
				//threadProc_�͔C�ӂ̖����l�ŗǂ�
				Thread();

			public:
				Thread(
					const nc::string& description,			//�X���b�h�̐���	
					SpThreadProc_t	threadProc );			//�X���b�h�Ŏ��s����֐�
					

				~Thread() {};

				//ThreadID��Ԃ��܂��B
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

				//���݂�Thread�̃X�e�[�^�X��Ԃ��܂��B
				inline E_THREAD_STATUS getStatus() const
				{
					return isActive_ ? STATUS_ACTIVE : STATUS_FINISH;
					///*not implement ever*/ NC_HALT(); return STATUS_FINISH;
				}

				//�D��x��ύX���܂��B				
//				inline void changePriority( int priority )
//				{ 
//					priority_ = priority;
					/*not implement ever*/ //NC_HALT();
//				}

				//thread���~
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

//�e�v���b�g�t�H�[�����̎���
#include "PLT_Thread.h"

#else

#include "pthread.h"

namespace nc{

	namespace platform{

		namespace thread{

			namespace detail{

				//���݂̃X���b�hID���擾����
				nc::thread::ThreadID_t getCurrentThreadID();

				/**
				 *	pthread���x�[�X�Ƃ���Thread�N���X�B
				 *	
				 *	pthread�Ƃ�
				 *	http://codezine.jp/article/detail/1892
				 *	
				 *	�S�Ă�pthread�̊֐����t�@�����X
				 *	http://slashdot.jp/journal.pl?op=display&uid=7255&id=117590
				 */
				class Thread : public nc::generic::thread::Thread {

					friend class nc::thread::ThreadManager;

					pthread_t				hThread_;
					int32_t					suspendCount_;

					bool					isActive_;

					//! pthread�p��
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

					//thread���~
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
