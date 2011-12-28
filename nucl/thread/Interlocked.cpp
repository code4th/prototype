
#include "PlatformInclude.h"
#include "Interlocked.h"

#include "Mutex.h"

namespace nc
{
	namespace generic
	{
		namespace interlocked
		{
			static nc::thread::Mutex mtex;

			int32_t compareAndExchange( int32_t volatile* addr, int32_t oldVal, int32_t newVal )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock(mtex);
				if( *addr == oldVal )
				{
					*addr = newVal;
					return oldVal;
				}
				return *addr;
			}

			int64_t compareAndExchange2( int64_t volatile* addr, int64_t oldVal, int64_t newVal )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock(mtex);
				if( *addr == oldVal )
				{
					*addr = newVal;
					return oldVal;
				}
				return *addr;
			}

			int64_t compareAndExchange2( int64_t volatile* addr, int32_t oldValLow, int32_t oldValHigh, int32_t newValLow, int32_t newValHigh )
			{
				int64_t const oldVal = ((int64_t)oldValLow | ((int64_t)oldValHigh << 32));
				int64_t const newVal = ((int64_t)newValLow | ((int64_t)newValHigh << 32));

				return compareAndExchange2( addr, oldVal, newVal );
			}

			int32_t exchange( int32_t volatile* addr,int32_t value)
			{
				int32_t current;
				do 
				{
					current = *addr;
				} while (compareAndExchange( addr, current, value ) != current);
				return current;
			}

			int32_t exchangeAdd( int32_t volatile* addr,int32_t value )
			{
				int32_t current;
				do 
				{
					current = *addr;
				} while (compareAndExchange( addr, current, current + value ) != current);

				return current + value;
			}

			int32_t increment( int32_t volatile* addr )
			{
				return exchangeAdd( addr, 1 );
			}

			int32_t decrement( int32_t volatile* addr )
			{
				return exchangeAdd( addr, -1 );
			}


			int32_t readAcquire( int32_t volatile* addr )
			{
				return compareAndExchange(addr,0,0);
			}

			void* readAcquire( void* volatile* addr )
			{
				return (void*)compareAndExchange((int32_t volatile*)addr,0,0);
			}


			void writeRelease( int32_t volatile* addr, int32_t value )
			{
				exchange(addr,value);
			}

			void writeRelease( void* volatile* addr, void* value )
			{
				exchange((int32_t volatile*)addr, (int32_t)value);
			}


			bool bitTestAndSet( int32_t volatile* x,int32_t bit )
			{
				int32_t const value=1<<bit;
				int32_t old=*x;
				do
				{
					int32_t const current=compareAndExchange(x,old,old|value);
					if(current==old)
					{
						break;
					}
					old=current;
				}
				while(true);
				return (old&value)!=0;
			}

			bool bitTestAndReset( int32_t volatile* x,int32_t bit )
			{
				int32_t const value=1<<bit;
				int32_t old=*x;
				do
				{
					int32_t const current=compareAndExchange(x,old,old&~value);
					if(current==old)
					{
						break;
					}
					old=current;
				}
				while(true);
				return (old&value)!=0;
			}

		}
	}
}


