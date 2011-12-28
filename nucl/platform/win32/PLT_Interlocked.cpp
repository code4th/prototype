

#include "PLT_Interlocked.h"
#include "nucl/thread/Mutex.h"

//#include <intrin.h>

extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
extern "C" __int64 __cdecl _InterlockedCompareExchange64( __int64 volatile *, __int64, __int64 );
extern "C" void _ReadWriteBarrier(void);

#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)
#pragma intrinsic(_ReadWriteBarrier)


namespace nc
{
	namespace platform
	{
		namespace interlocked
		{
			namespace
			{
				static nc::thread::Mutex mtex;
			}

			int32_t compareAndExchange( int32_t volatile* addr, int32_t oldVal, int32_t newVal )
			{
				return _InterlockedCompareExchange( (long volatile*)addr, (long)newVal, (long)oldVal );

				//register int32_t retval;

				//_asm
				//{
				//	mov ecx,addr
				//	mov eax,oldVal
				//	mov ebx,newVal
				//	lock cmpxchg [ecx],ebx
				//	mov retval,eax
				//}

				//return retval;
			}

			int64_t compareAndExchange2( int64_t volatile* addr, int64_t oldVal, int64_t newVal )
			{
				return _InterlockedCompareExchange64( (__int64 volatile*)addr, (__int64)oldVal, (__int64)newVal );
			}

			int64_t compareAndExchange2( int64_t volatile* addr, int32_t oldValLow, int32_t oldValHigh, int32_t newValLow, int32_t newValHigh )
			{
				int64_t const oldVal = ((int64_t)oldValLow | ((int64_t)oldValHigh << 32));
				int64_t const newVal = ((int64_t)newValLow | ((int64_t)newValHigh << 32));

				return _InterlockedCompareExchange64( (__int64 volatile*)addr, (__int64)oldVal, (__int64)newVal );

				//register int32_t retval1, retval2;

				//_asm
				//{
				//	mov esi,_ptr
				//	mov eax,old1
				//	mov edx,old2
				//	mov ebx,new1
				//	mov ecx,new2
				//	lock cmpxchg8b [esi]
				//	mov retval,eax
				//	mov retva2,edx
				//}

				//return int64_t( (int64_t)retval1 | (int64_t)retval2 << 32 );
			}

			int32_t exchange( int32_t volatile* addr,int32_t value)
			{
				//なんか納得行かないけど、int32_tがintで定義されてるので、LONGにキャストする…。本末転倒。
				return InterlockedExchange( (volatile LONG *)addr, value );
			}

			int32_t exchangeAdd( int32_t volatile* addr,int32_t value )
			{
				return InterlockedExchangeAdd( (volatile LONG *)addr, value );
			}

			int32_t increment( int32_t volatile* addr )
			{
				return InterlockedIncrement( (volatile LONG *)addr );
			}

			int32_t decrement( int32_t volatile* addr )
			{
				return InterlockedDecrement( (volatile LONG *)addr );
			}

			//boostのinterlocked_read.hpp参考

			int32_t readAcquire( int32_t volatile* addr )
			{
				long const res=*addr;
				_ReadWriteBarrier();
				return res;
			}

			void* readAcquire( void* volatile* addr )
			{
				void* const res=*addr;
				_ReadWriteBarrier();
				return res;
			}

			void writeRelease( int32_t volatile* addr, int32_t value )
			{
				_ReadWriteBarrier();
				*addr=value;
			}

			void writeRelease( void* volatile* addr, void* value )
			{
				_ReadWriteBarrier();
				*addr=value;
			}
		}
	}
}
