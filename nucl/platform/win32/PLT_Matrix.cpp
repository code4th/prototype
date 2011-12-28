
#include "nucl/math/Matrix.h"
#include "nucl/math/MatrixUtil.h"

#ifdef NC_PLATFORM_USE_INTRINSIC_MATRIX

#include <nmmintrin.h>   // MMX-SSE4.2命令セットを使用する場合インクルード //
#include <smmintrin.h>   // MMX-SSE4.1命令セットを使用する場合インクルード //
#include <intrin.h>      // MMX-SSE3命令セットを使用する場合インクルード   //
#include <emmintrin.h>   // MMX-SSE2命令セットを使用する場合インクルード   //
#include <xmmintrin.h>   // MMX-SSE命令セットを使用する場合インクルード    //
#include <mmintrin.h>    // MMX命令セットを使用する場合インクルード        //


namespace nc{

	namespace platform{

		namespace math{

			nc::platform::math::Matrix33 Matrix33::operator*( const Matrix33& rhs ) const
			{
				//rowはリニア
				//colは離散
				__m128 m0 = _mm_loadu_ps(rowVector(0).idx);
				__m128 m1 = _mm_loadu_ps(rowVector(1).idx);
				__m128 m2 = _mm_loadu_ps(rowVector(2).idx);

				__m128 m3 = _mm_loadu_ps(rhs.colVector(0).idx);
				__m128 m4 = _mm_loadu_ps(rhs.colVector(1).idx);
				__m128 m5 = _mm_loadu_ps(rhs.colVector(2).idx);

				return nc::platform::math::Matrix33(
					_mm_dp_ps( m0, m3, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m0, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m0, m5, 0xF1 ).m128_f32[0],

					_mm_dp_ps( m1, m3, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m1, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m1, m5, 0xF1 ).m128_f32[0],

					_mm_dp_ps( m2, m3, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m2, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m2, m5, 0xF1 ).m128_f32[0]
				);
			}


			nc::platform::math::Matrix33 Matrix33::operator+( const Matrix33& rhs ) const
			{
				return nc::generic::math::Matrix33::operator+( rhs );
			}

			nc::platform::math::Matrix33 Matrix33::operator-( const Matrix33& rhs ) const
			{
				return nc::generic::math::Matrix33::operator-( rhs );
			}


			const nc::platform::math::Matrix33& Matrix33::operator*=( const Matrix33& rhs )
			{
				return static_cast< Matrix33& >( assign( *this * rhs ) );
			}

			const nc::platform::math::Matrix33& Matrix33::operator+=( const Matrix33& rhs )
			{
				return static_cast< Matrix33& >( assign( *this + rhs ) );
			}

			const nc::platform::math::Matrix33& Matrix33::operator-=( const Matrix33& rhs )
			{
				return static_cast< Matrix33& >( assign( *this - rhs ) );
			}



			nc::platform::math::Matrix33 Matrix33::operator*( real_t bias ) const
			{
				return nc::generic::math::Matrix33::operator*( bias );
			}

			nc::platform::math::Matrix33 Matrix33::operator/( real_t bias ) const
			{
				return nc::generic::math::Matrix33::operator/( bias );
			}


			const nc::platform::math::Matrix33& Matrix33::operator*=( real_t bias )
			{
				return static_cast< Matrix33& >( assign( *this * bias ) );
			}

			const nc::platform::math::Matrix33& Matrix33::operator/=( real_t bias )
			{
				return static_cast< Matrix33& >( assign( *this / bias ) );
			}

			//----------------------------------------
			
			nc::platform::math::Matrix44 Matrix44::operator*( const Matrix44& rhs ) const
			{
				//rowはリニア
				//colは離散
				__m128 m0 = _mm_loadu_ps(rowVector(0).idx);
				__m128 m1 = _mm_loadu_ps(rowVector(1).idx);
				__m128 m2 = _mm_loadu_ps(rowVector(2).idx);
				__m128 m3 = _mm_loadu_ps(rowVector(3).idx);

				__m128 m4 = _mm_loadu_ps(rhs.colVector(0).idx);
				__m128 m5 = _mm_loadu_ps(rhs.colVector(1).idx);
				__m128 m6 = _mm_loadu_ps(rhs.colVector(2).idx);
				__m128 m7 = _mm_loadu_ps(rhs.colVector(3).idx);

				return nc::platform::math::Matrix44(
					_mm_dp_ps( m0, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m0, m5, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m0, m6, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m0, m7, 0xF1 ).m128_f32[0],

					_mm_dp_ps( m1, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m1, m5, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m1, m6, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m1, m7, 0xF1 ).m128_f32[0],

					_mm_dp_ps( m2, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m2, m5, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m2, m6, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m2, m7, 0xF1 ).m128_f32[0],

					_mm_dp_ps( m3, m4, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m3, m5, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m3, m6, 0xF1 ).m128_f32[0],
					_mm_dp_ps( m3, m7, 0xF1 ).m128_f32[0]
				);
			}


			nc::platform::math::Matrix44 Matrix44::operator+( const Matrix44& rhs ) const
			{
				return nc::generic::math::Matrix44::operator+( rhs );
			}

			nc::platform::math::Matrix44 Matrix44::operator-( const Matrix44& rhs ) const
			{
				return nc::generic::math::Matrix44::operator-( rhs );
			}

			const nc::platform::math::Matrix44& Matrix44::operator*=( const Matrix44& rhs )
			{
				return static_cast< Matrix44& >( assign( *this * rhs ) );
			}

			const nc::platform::math::Matrix44& Matrix44::operator+=( const Matrix44& rhs )
			{
				return static_cast< Matrix44& >( assign( *this + rhs ) ); 
			}

			const nc::platform::math::Matrix44& Matrix44::operator-=( const Matrix44& rhs )
			{
				return static_cast< Matrix44& >( assign( *this - rhs ) );
			}



			nc::platform::math::Matrix44 Matrix44::operator*( real_t bias ) const
			{
				return nc::generic::math::Matrix44::operator*( bias );
			}

			nc::platform::math::Matrix44 Matrix44::operator/( real_t bias ) const
			{
				return nc::generic::math::Matrix44::operator/( bias );
			}


			const nc::platform::math::Matrix44& Matrix44::operator*=( real_t bias )
			{
				return static_cast< Matrix44& >( assign( *this * bias ) );
			}

			const nc::platform::math::Matrix44& Matrix44::operator/=( real_t bias )
			{
				return static_cast< Matrix44& >( assign( *this / bias ) );
			}

		}//namespace math

	}//namespace platform

}//namespace nc

#endif
