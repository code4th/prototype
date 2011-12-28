#ifndef __NC_VECTORUTIL_h__
#define __NC_VECTORUTIL_h__

#include "Vector.h"

namespace nc{

	namespace generic{
		namespace math{

			template< typename T, typename U >
			inline real_t dot( const T& lhs, const U& rhs )
			{
				return dot( lhs, rhs, T::CoordinateHasData_Spec_tag(), U::CoordinateHasData_Spec_tag() );
			}

			template< typename T, typename U >
			inline real_t dot( const T& lhs, const U& rhs, nc::math::CoordinateHasData_XY_tag, nc::math::CoordinateHasData_XY_tag )
			{
				return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y );
			}

			template< typename T, typename U >
			inline real_t dot( const T& lhs, const U& rhs, nc::math::CoordinateHasData_XYZ_tag, nc::math::CoordinateHasData_XYZ_tag )
			{
				return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z );
			}

			template< typename T, typename U >
			inline real_t dot( const T& lhs, const U& rhs, nc::math::CoordinateHasData_XYZW_tag, nc::math::CoordinateHasData_XYZW_tag )
			{
				return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z ) + ( lhs.w * rhs.w );
			}


			template< typename T, typename U >
			inline T cross( const T& lhs, const U& rhs )
			{
				return cross( lhs, rhs, T::CoordinateHasData_Spec_tag(), U::CoordinateHasData_Spec_tag() );
			}

			template< typename T, typename U >
			inline T cross( const T& lhs, const U& rhs, nc::math::CoordinateHasData_XYZ_tag, nc::math::CoordinateHasData_XYZ_tag )
			{
				return T().assign( ( lhs.y * rhs.z ) - ( lhs.z * rhs.y ), ( lhs.z * rhs.x ) - ( lhs.x * rhs.z ), ( lhs.x * rhs.y ) - ( lhs.y * rhs.x ) );
			}

			//簡易表記
			template< typename T, typename U >
			inline T crossXYZ( const T& lhs, const U& rhs )
			{
				return cross( lhs, rhs, nc::math::CoordinateHasData_XYZ_tag(), nc::math::CoordinateHasData_XYZ_tag() );
			}

			template< typename T, typename U >
			inline T crossXY( const T& lhs, const U& rhs )
			{
				return cross( lhs, rhs, nc::math::CoordinateHasData_XY_tag(), nc::math::CoordinateHasData_XY_tag() );
			}

			template< typename T, typename U >
			inline real_t angleBetween2Vectors( const T& lhs, const U& rhs )
			{
				return dot( lhs, rhs ) / (lhs.length() * rhs.length());
			}

			//線形補間
			//戻り値は単位長ではないので注意（特にQuaternion）
			template< typename T >
			inline T lerp( const T& lhs, const T& rhs, real_t ratio )
			{
				NC_ASSERT( ratio >= 0.0f && ratio <= 1.0f );
				//return lhs * (1.0f - ratio) + rhs * ratio; 
				return static_cast<T>( lhs + (( rhs - lhs ) * ratio) ); 
			}

			//球面線形補間
			template< typename T >
			inline T slerp( const T& lhs, const T& rhs, real_t ratio )
			{
				NC_ASSERT( ratio >= 0.0f && ratio <= 1.0f );

				real_t ndot = dot(lhs,rhs);
				real_t sintheta = sqrt( 1 - (ndot * ndot) );

				return (lhs * ((sintheta * (1 - ratio)) / sintheta)) + (rhs * ((sintheta * ratio) / sintheta));
			}

			//エルミート補間
			template< typename T >
			inline T hermite( const T& v1, const T& t1, const T& v2, const T& t2, real_t s )
			{
				NC_ASSERT( s >= 0.0f && s <= 1.0f );

				//Q(s) = (2s3 - 3s2 + 1)v1 + (-2s3 + 3s2)v2 + (s3 - 2s2 + s)t1 + (s3 - s2)t2. 
				real_t s3 = s * s * s;
				real_t s2 = s * s;

				return static_cast<T>( (v1 * ((2 * s3) - (3 * s2) + 1)) + (v2 * ((-2 * s3) + (3 * s2))) + (t1 * (s3 - (2 * s2) + s)) + (t2 * (s3 - s2)) );
			}

			//Catmull-Rom スプラインは、次のように設定することで、エルミート スプラインから導出できます。
			//	v1 = p2
			//	v2 = p3
			//	t1 = (p3 - p1) / 2
			//	t2 = (p4 - p2) / 2
			template< typename T >
			inline T cutmullRom( const T& p1, const T& p2, const T& p3, const T& p4, real_t s )
			{
				NC_ASSERT( s >= 0.0f && s <= 1.0f );
				return hermite< T >( p2, (p3 - p1) / 2.0f, p3, (p4 - p2) / 2.0f, s );
			}

			//Barycentric Coordinates
			//http://msdn.microsoft.com/ja-jp/library/bb205505%28VS.85%29.aspx
			//http://mathworld.wolfram.com/BarycentricCoordinates.html
			//V1 + f(V2-V1) + g(V3-V1) 
			template< typename T >
			inline T barycentric( const T& v1, const T& v2, const T& v3, real_t f, real_t g )
			{
				return static_cast<T>( v1 + ((v2 - v1) * f) + ((v3 - v1) * f) );
			}

			//球状平方補間
			//Slerp(Slerp(pQ1, pC, t), Slerp(pA, pB, t), 2t(1 - t))
			template< typename T >
			inline T squad( const T& v1, const T& vA, const T& vB, const T& vC, real_t ratio )
			{
				NC_ASSERT( ratio >= 0.0f && ratio <= 1.0f );
				return slerp< T >( slerp< T >(v1, vC, ratio), slerp< T >(vA, vB, ratio), 2 * ratio * (1 - ratio));
			}

			//球状平方補間準備
			template< typename T >
			inline T squadSetup( T& vAout, T& vBout, T& vCout, const T& v0, const T& v1, const T& v2, const T& v3 )
			{
				//誰か実装してください。
			}

		}//nsmespace math
	}//namespace generic
}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_VECTOR_UTIL

#include "PLT_VectorUtil.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::dot;
		using nc::generic::math::cross;
		using nc::generic::math::lerp;
		using nc::generic::math::slerp;
		using nc::generic::math::hermite;
		using nc::generic::math::cutmullRom;
		using nc::generic::math::barycentric;
		using nc::generic::math::squad;
		using nc::generic::math::squadSetup;

	}//namespace math

}//namespace nc

#endif

#endif // __NC_VECTORUTIL_h__