/*
	Lock-Free �A���S���Y����g�ޏ�Ŏg�p����cas( compare and swap )���ߓ�
	�g�p�ɍۂ��Ă�ABA���ɂ͋C�����邱�ƁB
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
			//CAS�̓���
			//http://ja.wikipedia.org/wiki/%E3%82%B3%E3%83%B3%E3%83%9A%E3%82%A2%E3%83%BB%E3%82%A2%E3%83%B3%E3%83%89%E3%83%BB%E3%82%B9%E3%83%AF%E3%83%83%E3%83%97
			//See also emulation code.

			/**
			 * 32bit CAS( Compare and Store | Compare and Exchange )�ł�
			 * 
			 * �A�g�~�b�N�ɒl���X�V���܂��B
			 * CAS�Ɏ��s�����ꍇ�́A������xCAS����蒼���K�v������܂��B
			 *
			 * @param	int32_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 * @param	int32_t oldVal			����� *addr �̒l
			 * @param	int32_t newVal			�ړI *addr �l
			 *
			 * @retval	== oldval �̏ꍇ�A*addr�l�͍X�V����܂���
			 * @retval	!= oldval �̏ꍇ�A*addr�l��CAS�O�ɍX�V����Ă��܂�
			 */
			int32_t compareAndExchange( int32_t volatile* addr, int32_t oldVal, int32_t newVal );

			/**
			 * 64bit CAS2( Compare and Store | Compare and Exchange )�ł�
			 *
			 * �A�g�~�b�N�ɒl���X�V���܂��B
			 * CAS�Ɏ��s�����ꍇ�́A������xCAS����蒼���K�v������܂��B
			 *
			 * @param	int64_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 * @param	int64_t oldVal			����� *addr �̒l
			 * @param	int64_t newVal			�ړI *addr �l
			 *
			 * @retval	== oldval �̏ꍇ�A*addr�l�͍X�V����܂���
			 * @retval	!= oldval �̏ꍇ�A*addr�l��CAS�O�ɍX�V����Ă��܂�
			 */
			int64_t compareAndExchange2( int64_t volatile* addr, int64_t oldVal, int64_t newVal );


			/**
			 * 64bit CAS2( Compare and Store | Compare and Exchange )�ł�
			 *
			 * �A�g�~�b�N�ɒl���X�V���܂��B
			 * CAS�Ɏ��s�����ꍇ�́A������xCAS����蒼���K�v������܂��B
			 * 32bit���ɈႤ�l�ɍX�V�ł��܂��B
			 *
			 * @param	int64_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 * @param	int32_t oldValLow		����� *addr �� low32bit �l
			 * @param	int32_t oldValHigh		����� *addr �� high32bit �l
			 * @param	int32_t newValLow		�ړI *addr �� low32bit �l
			 * @param	int32_t newValHigh		�ړI *addr �� high32bit �l
			 *
			 * @retval	== ( oldValHigh << 32 | oldValLow ) �̏ꍇ�A*addr�l�͍X�V����܂���
			 * @retval	!= ( oldValHigh << 32 | oldValLow ) �̏ꍇ�A*addr�l��CAS�O�ɍX�V����Ă��܂�
			 */
			int64_t compareAndExchange2( int64_t volatile* addr, int32_t oldValLow, int32_t oldValHigh, int32_t newValLow, int32_t newValHigh );

			/**
			 * CAS�𗘗p���� *addr �� value �֍X�V���܂��B
			 *
			 * CAS����������܂ŃX�s�����b�N���܂��B
			 *
			 * @param	int32_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 * @param	int32_t value			�ړI *addr �l
			 *
			 * @retval	�X�V���O�� *addr �l
			 */
			int32_t exchange( int32_t volatile* addr,int32_t value);

			/**
			 * CAS�𗘗p���� *addr �� value �����Z���܂��B
			 *
			 * CAS����������܂ŃX�s�����b�N���܂��B
			 *
			 * @param	int32_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 * @param	int32_t value			���Z�l
			 *
			 * @retval	���Z���O�� *addr �l
			 */
			int32_t exchangeAdd( int32_t volatile* addr,int32_t value );

			/**
			 * CAS�𗘗p���� *addr ���C���N�������g���܂�
			 *
			 * CAS����������܂ŃX�s�����b�N���܂��B
			 *
			 * @param	int32_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 *
			 * @retval	�C���N�������g���O�� *addr �l
			 */
			int32_t increment( int32_t volatile* addr );

			/**
			 * CAS�𗘗p���� *addr ���f�N�������g���܂�
			 *
			 * CAS����������܂ŃX�s�����b�N���܂��B
			 *
			 * @param	int32_t volatile * addr	�X�V����ΏۂƂȂ�A�h���X
			 *
			 * @retval	�f�N�������g���O�� *addr �l
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

//�e�v���b�g�t�H�[�����̎���
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

