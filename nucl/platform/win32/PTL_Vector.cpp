

#include "nucl/math/Vector.h"


#include <nmmintrin.h>   // MMX-SSE4.2���߃Z�b�g���g�p����ꍇ�C���N���[�h //
#include <smmintrin.h>   // MMX-SSE4.1���߃Z�b�g���g�p����ꍇ�C���N���[�h //
#include <intrin.h>      // MMX-SSE3���߃Z�b�g���g�p����ꍇ�C���N���[�h   //
#include <emmintrin.h>   // MMX-SSE2���߃Z�b�g���g�p����ꍇ�C���N���[�h   //
#include <xmmintrin.h>   // MMX-SSE���߃Z�b�g���g�p����ꍇ�C���N���[�h    //
#include <mmintrin.h>    // MMX���߃Z�b�g���g�p����ꍇ�C���N���[�h        //


namespace nc{

	namespace platform{

		namespace math{

			
			real_t Vector3::dot( const Vector3& rhs ) const
			{
				__m128 m0 = _mm_loadu_ps(this->idx);
				__m128 m1 = _mm_loadu_ps(rhs.idx);
				return _mm_dp_ps( m0, m1, 0xE1 ).m128_f32[0];

				//���������I�[�o�[����XMM���W�X�^�Ƀ��[�h���Ă邯�ǁA0xE1�Ōv�Z���Ă���̂Ŗ��Ȃ��B
				//return _mm_dp_ps( *((__m128*)(this->idx)), *((__m128*)(rhs.idx)), 0xE1 ).m128_f32[0];

				//return ( x * rhs.x ) + ( y * rhs.y ) + ( z * rhs.z );
			}

			//------------------------------------

			real_t Vector4::dot( const Vector4& rhs ) const
			{
				__m128 m0 = _mm_loadu_ps(this->idx);
				__m128 m1 = _mm_loadu_ps(rhs.idx);
				return _mm_dp_ps( m0, m1, 0xF1 ).m128_f32[0];

				//return _mm_dp_ps( *((__m128*)(this->idx)), *((__m128*)(rhs.idx)), 0xF1 ).m128_f32[0];

				//return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
			}


		}//namespace math

	}//namespace platform

}//namespace nc