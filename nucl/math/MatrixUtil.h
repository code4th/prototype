#ifndef __NC_MATRIXUTIL_h__
#define __NC_MATRIXUTIL_h__

#include "Matrix.h"

namespace nc{

	//-------------------------------------------------

	namespace generic{

		namespace math{

			//Matrix�̌`��OpenGL�`���ƂȂ�
			//������s����|���Ă���
		
			//���s�ړ��s��
			//	1,	0,	0,	tx,
			//	0,	1,	0,	ty,
			//	0,	0,	1,	tz,
			//	0,	0,	0,	1

			//DirectX -> World * Camera * Screnn
			//OpenGL -> Screen * Camera * World

			//DirectX�`����OpenGL�`���́A�]�u�̊֌W�ɂ���̂�
			//���ݕϊ��͓]�u�s������߂鎖�ŕϊ��\�ł���B
			//�������A�|���Z�̕������ς��̂Œ��ӂ��鎖�B

			//���W�n�͊�{�I�ɉE��n���̗p���Ă��܂��B
		

			nc::math::Matrix33 matrix33RotateAxisX( real_t rad );
			nc::math::Matrix33 matrix33RotateAxisY( real_t rad );
			nc::math::Matrix33 matrix33RotateAxisZ( real_t rad );

			//axis�͒P�ʃx�N�g���ł���K�v������B
			nc::math::Matrix33 matrix33RotateAxis( const nc::math::Vector4& axis, real_t rad );

			/*
			Yaw
			y ���̃��[ (���W�A���P��)
			Pitch
			x ���̃s�b�` (���W�A���P��)
			Roll
			z ���̃��[�� (���W�A���P��)
			*/
			/*
			�g�����X�t�H�[���́A���[���A�s�b�`�A���[�̏��ɍs���܂��B
			�I�u�W�F�N�g�̃��[�J�����W������ɂ����ꍇ�A���̃g�����X�t�H�[��������
			z ���Ax ���Ay �������̏��ɉ�]���Ƃ��Ďg�p������]�ɓ������Ȃ�܂��B
			*/
			nc::math::Matrix33 matrix33RotateYawPitchRoll( real_t yaw, real_t pitch, real_t roll );

			nc::math::Matrix33 matrix33RotateQuaternion( const Quaternion& q );

			nc::math::Quaternion quaternionRotateMatrix( const nc::math::Matrix33& mtx );

			nc::math::Matrix33 matrix33Scale( real_t sx, real_t sy, real_t sz );

			nc::math::Matrix44 matrix44RotateAxisX( real_t rad );
			nc::math::Matrix44 matrix44RotateAxisY( real_t rad );
			nc::math::Matrix44 matrix44RotateAxisZ( real_t rad );
			nc::math::Matrix44 matrix44RotateAxis( const nc::math::Vector4& axis, real_t rad );

			nc::math::Matrix44 matrix44Scale( const nc::math::Vector3& sv );
			nc::math::Matrix44 matrix44Scale( const nc::math::Vector4& sv );
			nc::math::Matrix44 matrix44Scale( real_t sx, real_t sy, real_t sz, real_t sw = 1 );

			nc::math::Matrix44 matrix44Translate( const nc::math::Vector3& tv );
			nc::math::Matrix44 matrix44Translate( const nc::math::Vector4& tv );
			nc::math::Matrix44 matrix44Translate( real_t tx, real_t ty, real_t tz, real_t tw = 1 );

			/*
			eye
			���_
			at
			�J�����̒����Ώ�
			up
			���݂̃��[���h���W�ɂ����������B���̍\���̂̒l�͒ʏ� [0, 1, 0] �ł��B
			*/
			nc::math::Matrix44 matrix44LookAt( const Vector3& eye, const Vector3& at, const Vector3& up );
	
			/*
			w
			[in] �r���[ �{�����[���̕��B 
			h
			[in] �r���[ �{�����[���̍����B 
			zn
			[in] �r���[ �{�����[���̍ŏ� z �l�B 
			zf
			[in] �r���[ �{�����[���̍ő� z �l�B
			*/
			nc::math::Matrix44 matrix44Ortho( real_t w, real_t h, real_t zn, real_t zf );

			/*
			l
			�r���[ �{�����[���̍ŏ� x �l
			r
			�r���[ �{�����[���̍ő� x �l
			b
			�r���[ �{�����[���̍ŏ� y �l
			t
			�r���[ �{�����[���̍ő� y �l
			zn
			�r���[ �{�����[���̍ŏ� z �l
			zf
			�r���[ �{�����[���̍ő� z �l
			*/
			nc::math::Matrix44 matrix44OrthoOffCenter( real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf );

			/*
			w
			�߂��̃r���[ �v���[���ł̃r���[ �{�����[���̕�
			h
			�߂��̃r���[ �v���[���ł̃r���[ �{�����[���̍���
			zn
			�߂��̃r���[ �v���[���� z �l
			zf
			�����̃r���[ �v���[���� z �l
			*/
			nc::math::Matrix44 matrix44Perspective( real_t w, real_t h, real_t zn, real_t zf );

			/*
			l
			�r���[ �{�����[���̍ŏ� x �l
			r
			�r���[ �{�����[���̍ő� x �l
			b
			�r���[ �{�����[���̍ŏ� y �l
			t
			�r���[ �{�����[���̍ő� y �l
			zn
			�r���[ �{�����[���̍ŏ� z �l
			zf
			�r���[ �{�����[���̍ő� z �l
			*/
			nc::math::Matrix44 matrix44PerspectiveOffCenter(  real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf );

			/*
			fovy
			y �����̎���p (���W�A���P��)
			Aspect
			�A�X�y�N�g�� (�r���[��Ԃ̕��������ŏ��Z���Ē�`)
			zn
			�߂��̃r���[ �v���[���� z �l
			zf
			�����̃r���[ �v���[���� z �l
			*/
			nc::math::Matrix44 matrix44PerspectiveFov( real_t fovY, real_t aspect, real_t zn, real_t zf );


			//Matrix�̌v�Z�̍ۂɁA�ǂ����ɂ���ꎞ�ϐ��Ɍv�Z�r�����i�[����̂�
			//�����ĎQ�ƂŎ󂯎���āA���̂��̂�ύX����悤�ɂ͂��Ă��܂���B
			
			nc::math::Vector2 transform( const nc::math::Matrix33& mtx, const nc::math::Vector2& vec );	//Z = 0�Ƃ��Čv�Z����܂��B
			nc::math::Vector3 transform( const nc::math::Matrix33& mtx, const nc::math::Vector3& vec );

			nc::math::Vector3 transform( const nc::math::Matrix44& mtx, const nc::math::Vector3& vec );	//W = 0�Ƃ��Čv�Z����܂��B
			nc::math::Vector4 transform( const nc::math::Matrix44& mtx, const nc::math::Vector4& vec );

			nc::math::Point2 transform( const nc::math::Matrix33& mtx, const nc::math::Point2& pt );		//Z = 1�Ƃ��Čv�Z����܂��B
			nc::math::Point3 transform( const nc::math::Matrix33& mtx, const nc::math::Point3& pt );

			nc::math::Point3 transform( const nc::math::Matrix44& mtx, const nc::math::Point3& pt );		//W = 1�Ƃ��Čv�Z����܂��B
			nc::math::Point4 transform( const nc::math::Matrix44& mtx, const nc::math::Point4& pt );
			
		}//namespace math

	}//namespace generic

}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MATRIX_UTIL

#include "PLT_MatrixUtil.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::matrix33RotateAxisX;
		using nc::generic::math::matrix33RotateAxisY;
		using nc::generic::math::matrix33RotateAxisZ;
		using nc::generic::math::matrix33RotateAxis;

		using nc::generic::math::matrix33RotateYawPitchRoll;
		using nc::generic::math::matrix33RotateQuaternion;
		using nc::generic::math::quaternionRotateMatrix;
		using nc::generic::math::matrix33Scale;
		using nc::generic::math::matrix44RotateAxisX;
		using nc::generic::math::matrix44RotateAxisY;
		using nc::generic::math::matrix44RotateAxisZ;
		using nc::generic::math::matrix44RotateAxis;
		using nc::generic::math::matrix44Scale;
		using nc::generic::math::matrix44Translate;
		using nc::generic::math::matrix44LookAt;
		using nc::generic::math::matrix44Ortho;
		using nc::generic::math::matrix44OrthoOffCenter;
		using nc::generic::math::matrix44Perspective;
		using nc::generic::math::matrix44PerspectiveOffCenter;
		using nc::generic::math::matrix44PerspectiveFov;
		using nc::generic::math::transform;
		
	}//namespace math

}//namespace nc

#endif


#endif // __NC_MATRIXUTIL_h__