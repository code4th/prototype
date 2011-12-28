
#ifndef __NC_RANDOM_h__
#define __NC_RANDOM_h__

#include "Types.h"
#include "Assert.h"

namespace nc{
	
	/*!
	@class
		Random
	@brief
		make random value by XorShift algorithm.
		not thread safe.

		�����́A340��2823��6692�a0938��4634�\6337��4607��4317��6821��1455
		��������̂ŁA�����p�^�[�����J��Ԃ��S�z�͂܂��Ȃ��B

		128 bit Xorshift �͘A������S�̏o�͂𓾂�΁A
		���������łȂ��ߋ��̐�����������Ă��܂��̂ŁA�Í��ɂ͌����Ȃ��B

		back�͑O�̃o�[�W�����ł́A�ŏ��ɌĂяo�����Ƃ��ɁA
		��O�̗�����Ԃ��Ă������A���̃o�[�W�����ł͍Ō�̗�����Ԃ��B
		���̂ق��������₷���B
	*/
	class Random{

		uint32_t xor_seed[4];	//seeds

	public:

		//!< �����^�����_���̍ő�l
		static const uint32_t RANDOM_MAX = _UI32_MAX;

		Random();

		//!< seed��ݒ�
		void setSeed( uint32_t seed );

		//!< 0 <= N <= RANDOM_MAX �����Ȃ������̃����_���l���擾
		uint32_t rand();

		//!< 0 <= N <= max �𖞂��������Ȃ������̃����_���l���擾
		uint32_t rand( uint32_t max );

		//!	min <= N <= max �𖞂��������Ȃ������̃����_���l���擾
		//	min > max ���̓���͕ۏ؂��Ȃ�
		uint32_t rand( uint32_t min, uint32_t max );


		//! 0 <= N <= RANDOM_MAX �����Ȃ������̃����_���l���擾
		//	rand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���randBack�̌Ăяo���́A���O��rand�l��Ԃ��܂��B
		uint32_t randBack();

		//! 0 <= N <= max �𖞂��������Ȃ������̃����_���l���擾
		//	rand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���randBack�̌Ăяo���́A���O��rand�l��Ԃ��܂��B
		uint32_t randBack( uint32_t max );


		//! min <= N <= max �𖞂��������Ȃ������̃����_���l���擾
		//  min > max ���̓���͕ۏ؂��Ȃ�
		//	rand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���randBack�̌Ăяo���́A���O��rand�l��Ԃ��܂��B
		uint32_t randBack( uint32_t min, uint32_t max );


		//!< 0.0f <= N <= 1.0f ���������_��l����
		real_t frand();

		//! 0.0f <= N <= max <= 1.0f ���������_��l����
		//	��������� 0 <= max <= 1.0f
		real_t frand( real_t max );

		//! min <= N  <= max  ���������_��l����
		//	min > max ���̓���͕ۏ؂��Ȃ�
		//	��������� 0 <= min < max <= 1.0f
		real_t frand( real_t min, real_t max );

		//!< 0.0f <= N <= 1.0f ���������_��l�����̋t��
		//	frand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���frandBack�̌Ăяo���́A���O��frand�l��Ԃ��܂��B
		real_t frandBack();

		//!< 0.0f <= N <= max ���������_��l�����̋t��
		//	��������� 0 <= max <= 1.0f
		//	frand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���frandBack�̌Ăяo���́A���O��frand�l��Ԃ��܂��B
		real_t frandBack( real_t max );


		//! min <= N  <= max  ���������_��l����
		//	min > max ���̓���͕ۏ؂��Ȃ�
		//	��������� 0 <= min < max <= 1.0f
		//	frand�̏o�͏������t�ȃo�[�W����
		//	�ŏ���frandBack�̌Ăяo���́A���O��frand�l��Ԃ��܂��B
		real_t frandBack( real_t min, real_t max );

	};



}//namespace nc

#endif // __NC_RANDOM_h__
