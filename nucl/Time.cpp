
#include "Time.h"

#include <time.h>

namespace nc{

	namespace generic{

		namespace time{

			//! 実時間に応じて進行する何かしらのカウンターTick
			nc::time::tick_t getTimeTick()
			{
				return (uint64_t)clock();
			}

			//! 実時間においての１秒間に何Tick進むか
			nc::time::tick_t getTimeTickPerSec()
			{
				return CLOCKS_PER_SEC;
			}

			nc::time::POSIXClock getNowPOSIXClock()
			{
				//timeの返却値が、POSIXタイムの経過ミリ秒を期待しています。
				return nc::time::POSIXClock( nc::time::MSec( ::time(NULL) ) );
			}

		}//namespace time

	}//namespace generic

}//namespace nc

namespace nc{

	namespace time{

		Clock::Clock() : tick_(0)
		{

		}

		Clock::Clock( const Clock& rhs ) : tick_( rhs.tick_ )
		{

		}

		Clock::Clock( const uint64_t& tick ) : tick_( tick )
		{

		}


		tick_t Clock::day() const
		{
			return tick_ / ( getTimeTickPerSec() * 86400 );
		}

		tick_t Clock::hour() const
		{
			return tick_ / ( getTimeTickPerSec() * 3600 );
		}

		tick_t Clock::minute() const
		{
			return tick_ / ( getTimeTickPerSec() * 60 );
		}

		tick_t Clock::sec() const
		{
			return tick_ / getTimeTickPerSec();
		}

		tick_t Clock::msec() const
		{
			return ( tick_ * 1000 ) / getTimeTickPerSec();
		}

		tick_t Clock::usec() const
		{
			return ( tick_ * 1000000 ) / getTimeTickPerSec();
		}

		tick_t Clock::nsec() const
		{
			return ( tick_ * 1000000000 ) / getTimeTickPerSec();
		}

		tick_t Clock::tick() const
		{
			return tick_;
		}

		Clock Clock::operator-( const Clock& rhs ) const
		{
			return Clock( tick_ - rhs.tick_ );
		}

		Clock Clock::operator+( const Clock& rhs ) const
		{
			return Clock( tick_ + rhs.tick_ );
		}

		Clock& Clock::operator-=( const Clock& rhs )
		{
			tick_ -= rhs.tick_;
			return *this;
		}

		Clock& Clock::operator+=( const Clock& rhs )
		{
			tick_ += rhs.tick_;
			return *this;
		}

		Clock Clock::operator*( float bias ) const
		{
			return Clock( (tick_t)(tick_ * bias) );
		}

		Clock Clock::operator/( float bias ) const
		{
			return Clock( (tick_t)(tick_ / bias) );
		}

		Clock& Clock::operator*=( float bias )
		{
			tick_ = (tick_t)(tick_ * bias);
			return *this;
		}

		Clock& Clock::operator/=( float bias )
		{
			tick_ = (tick_t)(tick_ / bias);
			return *this;
		}


		bool Clock::operator>( const Clock& rhs ) const
		{
			return tick_ > rhs.tick();
		}

		bool Clock::operator>=( const Clock& rhs ) const
		{
			return tick_ >= rhs.tick();
		}

		bool Clock::operator<( const Clock& rhs ) const
		{
			return tick_ < rhs.tick();
		}

		bool Clock::operator<=( const Clock& rhs ) const
		{
			return tick_ <= rhs.tick();
		}

		const Clock& Clock::operator=( const Clock& rhs )
		{
			tick_ = rhs.tick_;
			return *this;
		}

		bool Clock::operator==( const Clock& rhs ) const
		{
			return tick_ == rhs.tick();
		}

		//-----------------------------------------------


		time::POSIXClock::POSIXClock()
		{

		}

		time::POSIXClock::POSIXClock( const Clock& rhs )
			: Clock( rhs )
		{

		}


		//-----------------------------------------------
		

		NSec::NSec() : Clock( 0 )
		{

		}

		NSec::NSec( nc::uint64_t nsec ) 
			: Clock( (nsec * getTimeTickPerSec()) / 1000000000 )
		{

		}

		NSec::NSec( const Clock& rhs )
			: Clock( rhs )
		{

		}


		void NSec::normalize()
		{
			*this = NSec( nsec() );
		}

		//-----------------------------------------------


		USec::USec() : Clock( 0 )
		{

		}

		USec::USec( nc::uint64_t usec )
			: Clock( (usec * getTimeTickPerSec()) / 1000000 )
		{

		}

		USec::USec( const Clock& rhs )
			: Clock( rhs )
		{

		}

		void USec::normalize()
		{
			*this = USec( usec() );
		}


		//-----------------------------------------------

		MSec::MSec() : Clock( 0 )
		{

		}

		MSec::MSec( nc::uint64_t msec )
			: Clock( (msec * getTimeTickPerSec()) / 1000 )
		{

		}

		MSec::MSec( const Clock& rhs )
			: Clock( rhs )
		{

		}

		void MSec::normalize()
		{
			*this = MSec( msec() );
		}

		//-----------------------------------------------

		Sec::Sec() : Clock( 0 )
		{

		}

		Sec::Sec( nc::uint64_t sec )
			: Clock( sec * getTimeTickPerSec() )
		{

		}

		Sec::Sec( const Clock& rhs )
			: Clock( rhs )
		{

		}
		
		void Sec::normalize()
		{
			*this = Sec( sec() );
		}

		//-----------------------------------------------
		
		Minute::Minute() : Clock( 0 )
		{

		}

		Minute::Minute( nc::uint64_t minute )
			: Clock( minute * ( 60 * getTimeTickPerSec() ) )
		{

		}

		Minute::Minute( const Clock& rhs )
			: Clock( rhs )
		{

		}

		void Minute::normalize()
		{
			*this = Minute( minute() );
		}

		//-----------------------------------------------


		Hour::Hour() : Clock( 0 )
		{

		}

		Hour::Hour( nc::uint64_t hour )
			: Clock( hour * ( 3600 * getTimeTickPerSec() ) )
		{

		}

		Hour::Hour( const Clock& rhs )
			: Clock( rhs )
		{

		}

		void Hour::normalize()
		{
			*this = Hour( hour() );
		}

		//-----------------------------------------------


		Day::Day() : Clock( 0 )
		{

		}

		Day::Day( nc::uint64_t day )
			: Clock( day * ( 86400 * getTimeTickPerSec() ) )
		{

		}
			
		Day::Day( const Clock& rhs )
			: Clock( rhs )
		{

		}

		void Day::normalize()
		{
			*this = Day( day() );
		}

	}//namespace time

}//namespace nc
