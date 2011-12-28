#ifndef __NC_THREADMANAGER_h__
#define __NC_THREADMANAGER_h__

#include "nucl/Singleton.h"
#include "nucl/thread/Thread.h"
#include "nucl/smartptr/SmartPtr.h"

namespace nc{

	namespace thread{

		class ThreadManager : public Singleton< ThreadManager >{

		public:
			typedef std::list< SharedPtr< Thread >, nc::memory::CommonHeapAllocator_t > ThreadList_t;

			ThreadList_t	threadList_;

			//ThreadManager�̏�����
			void initialize()
			{
				//initialize���Ă�thread���A���C���X���b�h�Ƃ��ēo�^����܂��B
				threadList_.push_back( SharedPtr< Thread >( new Thread ) );
			}

			void cleanup()
			{
				threadList_.clear();
			}

			void registerThread( Thread& thread )
			{

			}
			
		};

	}//namespace thread

}//namespace nc


#endif // __NC_THREADMANAGER_h__