

// Win32 VisualStudio platform

#include "PLT_Atomic.h"

namespace nc
{
	namespace platform
	{
		namespace atomic
		{
			void add(volatile int32_t &var, int32_t i)
			{
				InterlockedExchangeAdd( (volatile LONG *)&var, i);
			}

			void sub(volatile int32_t &var, int32_t i)
			{
				InterlockedExchangeAdd( (volatile LONG *)&var, -i);
			}

			void inc( volatile int32_t& var )
			{
				add( var, 1 );
			}

			void dec( volatile int32_t& var )
			{
				sub( var, 1 );
			}

			void set(volatile int32_t &var, int32_t i)
			{
				InterlockedExchange( (volatile LONG *)&var, i);
			}

		}
	}
}


