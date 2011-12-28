/*
	Lock-Free アルゴリズムを組む上で使用するcas( compare and swap )命令等
	使用に際してはABA問題には気をつけること。
	sa)
		http://www.radiumsoftware.com/0607.html#060714
		http://www.coos.jp/articles/volatile.aspx
		http://dsas.blog.klab.org/archives/51081649.html


		http://www.xlsoft.com/jp/products/intel/article/guide/index.html

*/
#ifndef __NC_INTERLOCKED_h__
#define __NC_INTERLOCKED_h__


#include "nucl/Types.h"
#include "nucl/StaticAssert.h"

namespace nc
{
	namespace generic
	{
		namespace interlocked
		{
			//CASの動作
			//http://ja.wikipedia.org/wiki/%E3%82%B3%E3%83%B3%E3%83%9A%E3%82%A2%E3%83%BB%E3%82%A2%E3%83%B3%E3%83%89%E3%83%BB%E3%82%B9%E3%83%AF%E3%83%83%E3%83%97
			//See also emulation code.

			/**
			 * 32bit CAS( Compare and Store | Compare and Exchange )です
			 * 
			 * アトミックに値を更新します。
			 * CASに失敗した場合は、もう一度CASをやり直す必要があります。
			 *
			 * @param	int32_t volatile * addr	更新する対象となるアドレス
			 * @param	int32_t oldVal			現状の *addr の値
			 * @param	int32_t newVal			目的 *addr 値
			 *
			 * @retval	== oldval の場合、*addr値は更新されました
			 * @retval	!= oldval の場合、*addr値はCAS前に更新されています
			 */
			int32_t compareAndExchange( int32_t volatile* addr, int32_t oldVal, int32_t newVal );

			/**
			 * 64bit CAS2( Compare and Store | Compare and Exchange )です
			 *
			 * アトミックに値を更新します。
			 * CASに失敗した場合は、もう一度CASをやり直す必要があります。
			 *
			 * @param	int64_t volatile * addr	更新する対象となるアドレス
			 * @param	int64_t oldVal			現状の *addr の値
			 * @param	int64_t newVal			目的 *addr 値
			 *
			 * @retval	== oldval の場合、*addr値は更新されました
			 * @retval	!= oldval の場合、*addr値はCAS前に更新されています
			 */
			int64_t compareAndExchange2( int64_t volatile* addr, int64_t oldVal, int64_t newVal );


			/**
			 * 64bit CAS2( Compare and Store | Compare and Exchange )です
			 *
			 * アトミックに値を更新します。
			 * CASに失敗した場合は、もう一度CASをやり直す必要があります。
			 * 32bit毎に違う値に更新できます。
			 *
			 * @param	int64_t volatile * addr	更新する対象となるアドレス
			 * @param	int32_t oldValLow		現状の *addr の low32bit 値
			 * @param	int32_t oldValHigh		現状の *addr の high32bit 値
			 * @param	int32_t newValLow		目的 *addr の low32bit 値
			 * @param	int32_t newValHigh		目的 *addr の high32bit 値
			 *
			 * @retval	== ( oldValHigh << 32 | oldValLow ) の場合、*addr値は更新されました
			 * @retval	!= ( oldValHigh << 32 | oldValLow ) の場合、*addr値はCAS前に更新されています
			 */
			int64_t compareAndExchange2( int64_t volatile* addr, int32_t oldValLow, int32_t oldValHigh, int32_t newValLow, int32_t newValHigh );

			/**
			 * CASを利用して *addr を value へ更新します。
			 *
			 * CASが成功するまでスピンロックします。
			 *
			 * @param	int32_t volatile * addr	更新する対象となるアドレス
			 * @param	int32_t value			目的 *addr 値
			 *
			 * @retval	更新直前の *addr 値
			 */
			int32_t exchange( int32_t volatile* addr,int32_t value);

			/**
			 * CASを利用して *addr に value を加算します。
			 *
			 * CASが成功するまでスピンロックします。
			 *
			 * @param	int32_t volatile * addr	更新する対象となるアドレス
			 * @param	int32_t value			加算値
			 *
			 * @retval	加算直前の *addr 値
			 */
			int32_t exchangeAdd( int32_t volatile* addr,int32_t value );

			/**
			 * CASを利用して *addr をインクリメントします
			 *
			 * CASが成功するまでスピンロックします。
			 *
			 * @param	int32_t volatile * addr	更新する対象となるアドレス
			 *
			 * @retval	インクリメント直前の *addr 値
			 */
			int32_t increment( int32_t volatile* addr );

			/**
			 * CASを利用して *addr をデクリメントします
			 *
			 * CASが成功するまでスピンロックします。
			 *
			 * @param	int32_t volatile * addr	更新する対象となるアドレス
			 *
			 * @retval	デクリメント直前の *addr 値
			 */
			int32_t decrement( int32_t volatile* addr );

			int32_t readAcquire( int32_t volatile* addr);
			void* readAcquire( void* volatile* addr);

			void writeRelease( int32_t volatile* addr,int32_t value);
			void writeRelease( void* volatile* addr, void* value);

			bool bitTestAndSet( int32_t volatile* x,int32_t bit );
			bool bitTestAndReset( int32_t volatile* x,int32_t bit );

		}//namespace interlocked
	}//namespace generic
}//namespace nc



#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_INTERLOCKED

//各プラットフォーム毎の実装
#include "PLT_Interlocked.h"

/*
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

			int32_t readAcquire( int32_t volatile* addr);
			void* readAcquire( void* volatile* addr);

			void writeRelease( int32_t volatile* addr,int32_t value);
			void writeRelease( void* volatile* addr, void* value);

			bool bitTestAndSet( int32_t volatile* x,int32_t bit );
			bool bitTestAndReset( int32_t volatile* x,int32_t bit );
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
		using nc::platform::interlocked::bitTestAndSet;
		using nc::platform::interlocked::bitTestAndReset;
	}//namespace interlocked

}//namespace nc

*/

#else //#ifdef NC_PLATFORM_USE_INTRINSIC_INTERLOCKED

namespace nc
{
	namespace interlocked
	{
		using nc::generic::interlocked::compareAndExchange;
		using nc::generic::interlocked::compareAndExchange2;
		using nc::generic::interlocked::exchange;
		using nc::generic::interlocked::exchangeAdd;
		using nc::generic::interlocked::increment;
		using nc::generic::interlocked::decrement;
		using nc::generic::interlocked::readAcquire;
		using nc::generic::interlocked::writeRelease;
		using nc::generic::interlocked::bitTestAndSet;
		using nc::generic::interlocked::bitTestAndReset;
	}//namespace interlocked

}//namespace nc

#endif //#ifdef NC_PLATFORM_USE_INTRINSIC_INTERLOCKED


#endif // __NC_INTERLOCKED_h__

