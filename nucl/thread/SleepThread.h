#ifndef __NC_SLEEPTHREAD_h__
#define __NC_SLEEPTHREAD_h__

#include "nucl/Time.h"


namespace nc{

	namespace generic{

		namespace thread{

			//é©threadÇñ∞ÇÁÇπÇ‹Ç∑ÅB
			inline void sleepThread( nc::time::Clock clk )
			{

			}

		}//namespace thread

	}//namespace generic

}//namespace nc

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_THREAD

#include "PLT_SleepThread.h"


#else //#ifdef NC_PLATFORM_USE_INTRINSIC_THREAD

namespace nc{

	namespace thread{

		using nc::generic::thread::sleepThread;

	}//namespace thread

}//namespace nc


#endif //#ifdef NC_PLATFORM_USE_INTRINSIC_THREAD

#endif // __NC_SLEEPTHREAD_h__
