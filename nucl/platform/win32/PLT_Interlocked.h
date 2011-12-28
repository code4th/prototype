
//interlockedä÷êîÇÃëgÇ›çûÇ›î≈
//http://msdn.microsoft.com/ja-jp/library/ttk2z1ws.aspx

//Note: _InterlockedCompareExchange64 is available on x86 systems running on any Pentium architecture;
//it is not available on 386 or 486 architectures.

//InterlockedCompareExchange


//_ReadWriteBarrior
//http://msdn.microsoft.com/ja-jp/library/ms254271%28VS.80%29.aspx

#ifndef PLT_Interlocked_h__
#define PLT_Interlocked_h__

#include "nucl/thread/Interlocked.h"
#include "nucl/Types.h"

namespace nc
{
	namespace platform
	{
		namespace interlocked
		{
			int32_t compareAndExchange( int32_t volatile* addr, int32_t oldVal, int32_t newVal );
			int64_t compareAndExchange2( int64_t volatile* addr, int64_t oldVal, int64_t newVal );
			int64_t compareAndExchange2( int64_t volatile* addr, int32_t oldValLow, int32_t oldValHigh, int32_t newValLow, int32_t newValHigh );

			int32_t exchange( int32_t volatile* addr,int32_t value);
			int32_t exchangeAdd( int32_t volatile* addr,int32_t value );
			int32_t increment( int32_t volatile* addr );
			int32_t decrement( int32_t volatile* addr );

			int32_t readAcquire( int32_t volatile* addr );
			void* readAcquire( void* volatile* addr );
			void writeRelease( int32_t volatile* addr, int32_t value );
			void writeRelease( void* volatile* addr, void* value );
			
		}
	}
}


namespace nc
{
	namespace interlocked
	{
		using nc::platform::interlocked::compareAndExchange;
		using nc::platform::interlocked::compareAndExchange2;
		using nc::platform::interlocked::exchange;
		using nc::platform::interlocked::exchangeAdd;
		using nc::platform::interlocked::increment;
		using nc::platform::interlocked::decrement;
		using nc::platform::interlocked::readAcquire;
		using nc::platform::interlocked::writeRelease;

		using nc::generic::interlocked::bitTestAndSet;
		using nc::generic::interlocked::bitTestAndReset;

	}//namespace interlocked

}//namespace nc


#endif // PLT_Interlocked_h__
