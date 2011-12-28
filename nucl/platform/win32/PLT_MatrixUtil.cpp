
#include "nucl/math/MatrixUtil.h"
#include "nucl/math/VectorUtil.h"

#include <nmmintrin.h>   // MMX-SSE4.2命令セットを使用する場合インクルード //
#include <smmintrin.h>   // MMX-SSE4.1命令セットを使用する場合インクルード //
#include <intrin.h>      // MMX-SSE3命令セットを使用する場合インクルード   //
#include <emmintrin.h>   // MMX-SSE2命令セットを使用する場合インクルード   //
#include <xmmintrin.h>   // MMX-SSE命令セットを使用する場合インクルード    //
#include <mmintrin.h>    // MMX命令セットを使用する場合インクルード        //


namespace nc{

	namespace platform{

		namespace math{

			//using nc::math::transform;
			
			nc::math::Vector2 transform( const nc::math::Matrix33& mtx, const nc::math::Vector2& vec )
			{
				nc::math::Vector3 vec3( vec, 0 );

				return nc::math::Vector3(
					nc::math::dot( mtx.x, vec3 ),
					nc::math::dot( mtx.y, vec3 ),
					nc::math::dot( mtx.z, vec3 )
				).orthoProjection();
			}

			nc::math::Vector3 transform( const nc::math::Matrix33& mtx, const nc::math::Vector3& vec )
			{
				return nc::math::Vector3(
					nc::math::dot( mtx.x, vec ),
					nc::math::dot( mtx.y, vec ),
					nc::math::dot( mtx.z, vec )
				);
			}


			nc::math::Vector3 transform( const nc::math::Matrix44& mtx, const nc::math::Vector3& vec )
			{
				//平行移動が行われない
				nc::math::Matrix33 mtx33( mtx );
				return nc::math::transform( mtx33, vec );
			}

			nc::math::Vector4 transform( const nc::math::Matrix44& mtx, const nc::math::Vector4& vec )
			{
				/* Left mul

				__m128 s0 = _mm_load_ps(&vec.idx[0]);
				__m128 m1 = _mm_mul_ps(_mm_shuffle_ps(s0, s0, _MM_SHUFFLE(0,0,0,0)), _mm_load_ps(&mtx.x.idx[0]));
				__m128 m2 = _mm_mul_ps(_mm_shuffle_ps(s0, s0, _MM_SHUFFLE(1,1,1,1)), _mm_load_ps(&mtx.y.idx[0]));
				__m128 m3 = _mm_mul_ps(_mm_shuffle_ps(s0, s0, _MM_SHUFFLE(2,2,2,2)), _mm_load_ps(&mtx.z.idx[0]));
				__m128 m4 = _mm_mul_ps(_mm_shuffle_ps(s0, s0, _MM_SHUFFLE(3,3,3,3)), _mm_load_ps(&mtx.w.idx[0]));
				__m128 m5 = _mm_add_ps(_mm_add_ps(m1, m2), _mm_add_ps(m3, m4));
			
				__declspec(align(16)) real_t res[4];
				_mm_store_ps(&res[0], m5);

				return nc::math::Vector4( res );

				*/

				/*
				Vector4 res;

				__m128 s0 = _mm_loadu_ps(&vec.x);
				__m128 m1 = _mm_dp_ps(s0, _mm_loadu_ps(&mtx.x.x), 0xf8);
				__m128 m2 = _mm_dp_ps(s0, _mm_loadu_ps(&mtx.y.x), 0xf4);
				__m128 m3 = _mm_dp_ps(s0, _mm_loadu_ps(&mtx.z.x), 0xf2);
				__m128 m4 = _mm_dp_ps(s0, _mm_loadu_ps(&mtx.w.x), 0xf1);
				__m128 m5 = _mm_or_ps(_mm_or_ps(m1, m2), _mm_or_ps(m3, m4));

				_mm_storeu_ps(&res.x, m5);
				//_mm_storeu_ps(&a.x, m5);

				return res;
				*/

				/*
				// Right mul
				return nc::math::Vector4( 
					_mm_dp_ps( *((__m128*)(mtx.x.idx)), *((__m128*)(vec.idx)), 0xF1 ).m128_f32[0],
					_mm_dp_ps( *((__m128*)(mtx.y.idx)), *((__m128*)(vec.idx)), 0xF1 ).m128_f32[0],
					_mm_dp_ps( *((__m128*)(mtx.z.idx)), *((__m128*)(vec.idx)), 0xF1 ).m128_f32[0],
					_mm_dp_ps( *((__m128*)(mtx.w.idx)), *((__m128*)(vec.idx)), 0xF1 ).m128_f32[0]
				);
				*/

				// Original Code
				return nc::math::Vector4(
					nc::math::dot( mtx.x, vec ),
					nc::math::dot( mtx.y, vec ),
					nc::math::dot( mtx.z, vec ),
					nc::math::dot( mtx.w, vec )
				);
			}

			nc::math::Point2 transform( const nc::math::Matrix33& mtx, const nc::math::Point2& pt )
			{
				return nc::math::transform( mtx, nc::math::Vector3(pt, 1) ).orthoProjection();
			}

			nc::math::Point3 transform( const nc::math::Matrix33& mtx, const nc::math::Point3& pt )
			{
				return nc::math::transform( mtx, nc::math::Vector3(pt) );
			}


			nc::math::Point3 transform( const nc::math::Matrix44& mtx, const nc::math::Point3& pt )
			{
				//wを1として計算する
				return nc::math::transform( mtx, nc::math::Point4( pt, 1 ) ).orthoProjection();
			}

			nc::math::Point4 transform( const nc::math::Matrix44& mtx, const nc::math::Point4& pt )
			{
				return nc::math::transform( mtx, static_cast<nc::math::Vector4>(pt) );
			}

		}//namespace math

	}//namespace platform

}//namespace nc


