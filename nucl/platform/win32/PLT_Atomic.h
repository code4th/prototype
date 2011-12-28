
#ifndef PLT_Atomic_h__
#define PLT_Atomic_h__

#include "PlatformInclude.h"

#include "nucl/Types.h"

namespace nc
{
	namespace platform
	{
		namespace atomic
		{
			void add( volatile int32_t& var, int32_t i);
			void sub( volatile int32_t& var, int32_t i);
			void inc( volatile int32_t& var );
			void dec( volatile int32_t& var );
			void set( volatile int32_t& var, int32_t i);

			template< typename T >
			void swap( volatile T& lhs, volatile T& rhs )
			{
				STATIC_ASSERT( sizeof(T) == 4 );
				//no implement

				//need lock!!
			}

		}
	}
}

#endif // PLT_Atomic_h__
