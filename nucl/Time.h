#ifndef __NC_TIME_h__
#define __NC_TIME_h__

#include "Types.h"

//#include "string.h"

namespace nc{

	//generic,platform共通宣言
	namespace time{

		/**
		 * 計測可能な実時間の最小単位
		 *
		 * uint64_t等の十分な大きさを持った型であることが重要
		 */
		typedef nc::uint64_t tick_t;
		
		/**
		 *	時間を表すクラス
		 *
		 *	カレンダーの様な機能はありません。時間と言う概念を表すだけです。
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
		 *	POSIX time (UNIX time)を表すクラス。
		 *
		 *	UNIX時間	UTC	JST
		 *	0	1970-01-01T00:00:00	1970-01-01T09:00:00
		 *	100000000	1973-03-03T09:46:40	1973-03-03T18:46:40
		 *	1000000000	2001-09-09T01:46:40	2001-09-09T10:46:40
		 *	1234567890	2009-02-13T23:31:30	2009-02-14T08:31:30
		 *	2147483647	2038-01-19T03:14:07	2038-01-19T12:14:07
		 *
		 *	と言っても、現状はタダのtag状態
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

			//! 実時間に応じて進行する何かしらのカウンターTick
			nc::time::tick_t getTimeTick();

			//! 実時間においての１秒間に何Tick進むか
			nc::time::tick_t getTimeTickPerSec();
			
			//! とある時間からの経過時間(何処からの時間なのかは、環境によって扱いが変わる)
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
