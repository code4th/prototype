#ifndef __NC_MATH_h__
#define __NC_MATH_h__

#include "nucl/Types.h"


namespace nc
{
	namespace generic
	{
		namespace math{

			//������
			real_t sqrt( real_t n );

			real_t sin( real_t rad );
			real_t cos( real_t rad );
			real_t tan( real_t rad );

			real_t atan2( real_t y, real_t x );

			real_t asin( real_t n );
			real_t acos( real_t n );
			real_t atan( real_t n );

			real_t pow2( real_t n );

		}//namespace math
	}//namespace paltform
}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MATH

//�e�v���b�g�t�H�[�����̎���
#include "PLT_Math.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::sqrt;
		using nc::generic::math::sin;
		using nc::generic::math::cos;
		using nc::generic::math::tan;

		using nc::generic::math::atan2;

		using nc::generic::math::asin;
		using nc::generic::math::acos;
		using nc::generic::math::atan;

		using nc::generic::math::pow2;

	}//namespace math

}//namespace nc

#endif //NC_PLATFORM_USE_INTRINSIC_MATH


namespace nc{

	namespace math{

		//��
		static const real_t PI					= 3.141592653589793238462643383279f;

		//2��
		static const real_t PI2					= PI * 2;
		
		//�P�������x�ł���킷���B
		static const real_t FULL_CIRCLE_DEGREE	= 360.0f;

		//����
		static const real_t HALF_CIRCLE_DEGREE	= FULL_CIRCLE_DEGREE / 2.0f;

		//�P�x�ɑ΂��郉�W�A��
		static const real_t RAD_OF_DEGREE		= PI2 / FULL_CIRCLE_DEGREE;

		//���W�A���Ɠx(Degree)�̕ϊ�
		real_t degToRad( real_t deg );
		real_t radToDeg( real_t rad );

	}//namespace math

}//namespace nc

#endif // __NC_MATH_h__