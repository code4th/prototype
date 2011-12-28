#ifndef __NC_TIME_h__
#define __NC_TIME_h__

#include "Types.h"

//#include "string.h"

namespace nc{

	//generic,platform���ʐ錾
	namespace time{

		/**
		 * �v���\�Ȏ����Ԃ̍ŏ��P��
		 *
		 * uint64_t���̏\���ȑ傫�����������^�ł��邱�Ƃ��d�v
		 */
		typedef nc::uint64_t tick_t;
		
		/**
		 *	���Ԃ�\���N���X
		 *
		 *	�J�����_�[�̗l�ȋ@�\�͂���܂���B���Ԃƌ����T�O��\�������ł��B
		 */
		class Clock{
			tick_t	tick_;
		public:
			Clock( );
			Clock( const Clock& rhs );
			Clock( const uint64_t& tick );

			tick_t day() const;
			tick_t hour() const;
			tick_t minute() const;
			tick_t sec() const;
			tick_t msec() const;
			tick_t usec() const;
			tick_t nsec() const;
			tick_t tick() const;

			Clock operator-( const Clock& rhs ) const;
			Clock operator+( const Clock& rhs ) const;

			Clock& operator-=( const Clock& rhs );
			Clock& operator+=( const Clock& rhs );

			Clock operator*( float bias ) const;
			Clock operator/( float bias ) const;

			Clock& operator*=( float bias );
			Clock& operator/=( float bias );

			bool operator>( const Clock& rhs ) const;
			bool operator>=( const Clock& rhs ) const;

			bool operator<( const Clock& rhs ) const;
			bool operator<=( const Clock& rhs ) const;

			bool operator==( const Clock& rhs ) const;

			const Clock& operator=( const Clock& rhs );
		};

		/**
		 *	POSIX time (UNIX time)��\���N���X�B
		 *
		 *	UNIX����	UTC	JST
		 *	0	1970-01-01T00:00:00	1970-01-01T09:00:00
		 *	100000000	1973-03-03T09:46:40	1973-03-03T18:46:40
		 *	1000000000	2001-09-09T01:46:40	2001-09-09T10:46:40
		 *	1234567890	2009-02-13T23:31:30	2009-02-14T08:31:30
		 *	2147483647	2038-01-19T03:14:07	2038-01-19T12:14:07
		 *
		 *	�ƌ����Ă��A����̓^�_��tag���
		 */
		class POSIXClock : public Clock{
		public:
			POSIXClock();
			POSIXClock( const Clock& rhs );
		};

		struct NSec : public Clock{
		public:
			NSec();
			NSec( nc::uint64_t nsec );
			NSec( const Clock& rhs );

			void normalize();
		};

		struct USec : public Clock{
		public:
			USec();
			USec( nc::uint64_t usec );
			USec( const Clock& rhs );

			void normalize();
		};

		struct MSec : public Clock{
		public:
			MSec();
			MSec( nc::uint64_t msec );
			MSec( const Clock& rhs );

			void normalize();
		};

		struct Sec : public Clock{
		public:
			Sec();
			Sec( nc::uint64_t sec );
			Sec( const Clock& rhs );

			void normalize();
		};

		struct Minute : public Clock{
		public:
			Minute();
			Minute( nc::uint64_t minute );
			Minute( const Clock& rhs );

			void normalize();
		};

		struct Hour : public Clock{
		public:
			Hour();
			Hour( nc::uint64_t hour );
			Hour( const Clock& rhs );

			void normalize();
		};

		struct Day : public Clock{
		public:
			Day();
			Day( nc::uint64_t day );
			Day( const Clock& rhs );

			void normalize();
		};

	}//namespace time
	
	namespace generic{

		namespace time{

			//! �����Ԃɉ����Đi�s���鉽������̃J�E���^�[Tick
			nc::time::tick_t getTimeTick();

			//! �����Ԃɂ����Ă̂P�b�Ԃɉ�Tick�i�ނ�
			nc::time::tick_t getTimeTickPerSec();
			
			//! �Ƃ��鎞�Ԃ���̌o�ߎ���(��������̎��ԂȂ̂��́A���ɂ���Ĉ������ς��)
			nc::time::POSIXClock getNowPOSIXClock();

		}//namespace time

	}//namespace generic

}//namespace nc

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_TIME

#include "PLT_Time.h"

#else //#ifdef NC_PLATFORM_USE_TIME

namespace nc{

	//namespace setup
	namespace time{

		using nc::generic::time::getTimeTick;
		using nc::generic::time::getTimeTickPerSec;
		using nc::generic::time::getNowPOSIXClock;

	}//namespace math

}//namespace nc

#endif //#ifdef NC_PLATFORM_USE_TIME


#endif // __NC_TIME_h__
