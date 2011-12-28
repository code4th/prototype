#ifndef __NC_POINT_h__
#define __NC_POINT_h__

#include "Coordinate.h"

namespace nc{

	namespace generic{

		namespace math{

			//同次座標系にしたときにz = 1
			struct Point2 : public nc::math::Coordinate2{

				Point2() {};
				Point2( const Point2& rhs )					: Coordinate2(rhs) {}
				Point2( const Coordinate2& rhs )			: Coordinate2(rhs) {}
				Point2( const Coordinate2Data& rhs )		: Coordinate2(rhs) {}
				Point2( real_t x, real_t y )				: Coordinate2(x, y) {}
				Point2( const real_t arr[] )				: Coordinate2(arr) {}

				const Point2& operator= ( const Point2& rhs ){ return static_cast< const Point2 & >( assign(rhs) ); }
				const Point2& operator= ( const Coordinate2Data& rhs ){ return static_cast< const Point2 & >( assign(rhs) ); }

			};

			//同次座標系にしたときにw = 1
			struct Point3 : public nc::math::Coordinate3{

				Point3() {};
				Point3( const Point3& rhs )					: Coordinate3(rhs) {}
				Point3( const Coordinate3& rhs )			: Coordinate3(rhs) {}
				Point3( const Coordinate3Data& rhs )		: Coordinate3(rhs) {}
				Point3( const Point2& rhs, real_t z )		: Coordinate3(rhs, z) {}
				Point3( real_t x, real_t y, real_t z )		: Coordinate3(x, y, z) {}
				Point3( const real_t arr[] )				: Coordinate3(arr) {}

				const Point3& operator= ( const Point3& rhs ){ return static_cast< const Point3 & >( assign(rhs) ); }
				const Point3& operator= ( const Coordinate3Data& rhs ){ return static_cast< const Point3 & >( assign(rhs) ); }

			};

			struct Point4 : public nc::math::Coordinate4{

				Point4() {};
				Point4( const Point4& rhs )							: Coordinate4(rhs) {}
				Point4( const Coordinate4& rhs )					: Coordinate4(rhs) {}
				Point4( const Coordinate4Data& rhs )					: Coordinate4(rhs) {}
				Point4( const Point3& rhs, real_t z )				: Coordinate4(rhs, z) {}
				Point4( const Point2& rhs, real_t z, real_t w )		: Coordinate4(rhs, z, w) {}
				Point4( real_t x, real_t y, real_t z, real_t w )	: Coordinate4(x, y, z, w) {}
				Point4( const real_t arr[] )						: Coordinate4(arr) {}

				const Point4& operator= ( const Point4& rhs ){ return static_cast< const Point4 & >( assign(rhs) ); }
				const Point4& operator= ( const Coordinate4Data& rhs ){ return static_cast< const Point4 & >( assign(rhs) ); }

			};

		}//namespace math
	
	}//namespace generic

}//namespace nc



#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_POINT

#include "PLT_Point.h"

#else

namespace nc{

	//namespace setup
	namespace math{

		using nc::generic::math::Point2;
		using nc::generic::math::Point3;
		using nc::generic::math::Point4;

	}//namespace math

}//namespace nc

#endif

namespace nc{

	namespace math{

		//固定値ポイント
		static const Point2	Point2Zero( 0,0 );
		static const Point3	Point3Zero( 0,0,0 );
		static const Point4	Point4Zero( 0,0,0,0 );

	}//namespace math

}//namespace nc

#endif // __NC_POINT_h__
