
#include "Math.h"
#include <math.h>

namespace nc
{
	namespace generic
	{
		namespace math
		{
			real_t sqrt( real_t n )
			{
				return ::sqrt( n );
			}

			real_t sin( real_t rad )
			{
				return ::sin( rad );
			}

			real_t cos( real_t rad )
			{
				return ::cos( rad );
			}

			real_t tan( real_t rad )
			{
				return ::tan( rad );
			}


			real_t asin( real_t n )
			{
				return ::asin( n );
			}

			real_t acos( real_t n )
			{
				return ::acos( n );
			}

			real_t atan( real_t n )
			{
				return ::atan( n );
			}

			real_t atan2( real_t y, real_t x )
			{
				return ::atan2( y, x );
			}

			real_t pow2( real_t n )
			{
				return n * n;
			}

		}//namespace math

	}//namespace generic


	//‹¤’Ê‚ÌŽÀ‘•
	namespace math
	{
		real_t degToRad( real_t deg )
		{
			return deg * RAD_OF_DEGREE;
		}

		real_t radToDeg( real_t rad )
		{
			return rad * (HALF_CIRCLE_DEGREE / PI);
		}

	}//namespace math

}//namespace nc

