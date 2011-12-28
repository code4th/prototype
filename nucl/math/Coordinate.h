#ifndef __NC_COORDINATE_h__
#define __NC_COORDINATE_h__

#include "CoordinateData.h"

namespace nc{

	namespace generic{

		namespace math{

			class Coordinate2 : public nc::math::Coordinate2Data{
			public:

				Coordinate2();
				Coordinate2( const Coordinate2& rhs );
				Coordinate2( const nc::math::Coordinate2Data& rhs );
				Coordinate2( const Swizzle2<real_t>& rhs );
				Coordinate2( real_t x, real_t y );
				Coordinate2( const real_t arr[] );

				const nc::math::Coordinate2Data& operator= ( const nc::math::Coordinate2Data& rhs ){ return assign(rhs); }

			};


			class Coordinate3 : public nc::math::Coordinate3Data{
			public:

				Coordinate3();
				Coordinate3( const Coordinate3& rhs );
				Coordinate3( const nc::math::Coordinate3Data& rhs );
				Coordinate3( const Swizzle3<real_t>& rhs );
				Coordinate3( const Coordinate2& rhs, real_t z = 0 );
				Coordinate3( real_t x, real_t y, real_t z );
				Coordinate3( const real_t arr[] );

				const nc::math::Coordinate3Data& operator= ( const nc::math::Coordinate3Data& rhs ){
					return assign(rhs);
				}

			};




			class Coordinate4 : public nc::math::Coordinate4Data{
			public:

				Coordinate4();
				Coordinate4( const Coordinate4& rhs );
				Coordinate4( const nc::math::Coordinate4Data& rhs );
				Coordinate4( const Coordinate3& rhs, real_t w = 0 );
				Coordinate4( const Swizzle4<real_t>& rhs );
				Coordinate4( const Coordinate2& rhs, real_t z = 0, real_t w = 0 );
				Coordinate4( real_t x, real_t y, real_t z, real_t w );
				Coordinate4( const real_t arr[] );

				const nc::math::Coordinate4Data& operator= ( const nc::math::Coordinate4Data& rhs );

			};


			//--------------------inline impliment--------------------------

			inline Coordinate2::Coordinate2()
			{}

			inline Coordinate2::Coordinate2( const Coordinate2& rhs )
			{
				assign( rhs );
			}

			inline Coordinate2::Coordinate2( const nc::math::Coordinate2Data& rhs )
			{
				assign( rhs );
			}

			inline Coordinate2::Coordinate2( const Swizzle2<real_t>& rhs )
			{
				assign( rhs );
			}

			inline Coordinate2::Coordinate2( real_t x, real_t y )
			{
				assign( x, y );
			}

			inline Coordinate2::Coordinate2( const real_t arr[]  )
			{
				assign( arr );
			}


			//-------------------------------


			inline Coordinate3::Coordinate3()
			{}

			inline Coordinate3::Coordinate3( const Coordinate2& rhs, real_t z /*= 0 */ )
			{
				assign( rhs, z );
			}

			inline Coordinate3::Coordinate3( const Coordinate3& rhs )
			{
				assign( rhs );
			}

			inline Coordinate3::Coordinate3( const nc::math::Coordinate3Data& rhs )
			{
				assign( rhs );
			}


			inline Coordinate3::Coordinate3( const Swizzle3<real_t>& rhs )
			{
				assign( rhs );
			}

			inline Coordinate3::Coordinate3( real_t x, real_t y, real_t z )
			{
				assign( x, y, z );
			}

			inline Coordinate3::Coordinate3( const real_t arr[] )
			{
				assign( arr );
			}


			//-------------------------------


			inline Coordinate4::Coordinate4()
			{

			}

			inline Coordinate4::Coordinate4( const Coordinate2& rhs, real_t z /*= 0*/, real_t w /*= 0 */ )
			{
				assign( rhs, z, w );
			}

			inline Coordinate4::Coordinate4( const Coordinate3& rhs, real_t w /*= 0 */ )
			{
				assign( rhs, w );
			}

			inline Coordinate4::Coordinate4( const Coordinate4& rhs )
			{
				assign( rhs );
			}

			inline Coordinate4::Coordinate4( const nc::math::Coordinate4Data& rhs )
			{
				assign( rhs );
			}

			inline Coordinate4::Coordinate4( const Swizzle4<real_t>& rhs )
			{
				assign( rhs );
			}

			inline Coordinate4::Coordinate4( real_t x, real_t y, real_t z, real_t w )
			{
				assign( x, y, z, w );
			}

			inline Coordinate4::Coordinate4( const real_t arr[] )
			{
				assign( arr );
			}


		}//namespace math

	}//namespace generic

}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_COORDINATE

#include "PLT_Coordinate.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::Coordinate2;
		using nc::generic::math::Coordinate3;
		using nc::generic::math::Coordinate4;

	}//namespace math

}//namespace nc

#endif

//ìØéüç¿ïWån(homogenous coordinate)


#endif // __NC_COORDINATE_h__