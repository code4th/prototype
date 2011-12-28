
#ifndef __NC_THREADFUNCTION_h__
#define __NC_THREADFUNCTION_h__

namespace nc{
	
	namespace thread{


		class ThreadProc{
		public:
			ThreadProc() {};
			virtual ~ThreadProc() {};

			virtual void threadMain() = 0;

		};

	}//namespace thread

}//namespace nc

#endif // __NC_THREADFUNCTION_h__

