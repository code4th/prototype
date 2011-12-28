#ifndef __NC_QUATERNION_h__
#define __NC_QUATERNION_h__

#include "Vector.h"
#include "Point.h"

namespace nc{

	namespace generic{

		namespace math{

			//Quaternion�@�S����
			//http://www015.upp.so-net.ne.jp/notgeld/quaternion.html
			//http://hooktail.sub.jp/mathInPhys/quaternion/
			class Quaternion : public nc::math::Coordinate4 {

			public:
				Quaternion() {};
				Quaternion( const Quaternion& rhs )								: nc::math::Coordinate4(rhs) {}
				Quaternion( const nc::math::Coordinate4& rhs )					: nc::math::Coordinate4(rhs) {}
				Quaternion( const nc::math::Coordinate4Data& rhs )				: nc::math::Coordinate4(rhs) {}
				Quaternion( const nc::math::Vector4& rhs )						: nc::math::Coordinate4(rhs) {}
				Quaternion( const nc::math::Vector3& rhs, real_t w )			: nc::math::Coordinate4(rhs, w) {}
				Quaternion( const nc::math::Vector2& rhs, real_t z, real_t w )	: nc::math::Coordinate4(rhs, z, w) {}
				Quaternion( real_t x, real_t y, real_t z, real_t w )			: nc::math::Coordinate4(x, y, z, w) {}
				Quaternion( const real_t arr[] )								: nc::math::Coordinate4(arr) {}

				const Quaternion& operator= ( const Quaternion& rhs ){ return static_cast< const Quaternion & >( assign(rhs) ); }
				const Quaternion& operator= ( const nc::math::Coordinate3& rhs ){ return static_cast< const Quaternion & >( assign(rhs) ); }

				//�N�H�[�^�j�I���̊|���Z
				const Quaternion& operator*=( const Quaternion& rhs );

				//Q1*Q2 = (t1t2-V1�EV2; t1V2 + t2V1 + V1�~V2)
				Quaternion operator*( const Quaternion& rhs ) const;

				//����
				real_t dot( const Quaternion& rhs ) const;

				//Vector�̒���
				real_t length() const;

				//Vector�̒����i�����j
				real_t lengthSquared() const;

				//���K��
				Quaternion normalize();

				//����
				Quaternion conjugate() const;

				//�t�N�H�[�^�j�I��
				Quaternion inverse() const;

				//�ΐ��N�H�[�^�j�I��
				//http://www.tmps.org/index.php?%A5%AF%A5%A9%A1%BC%A5%BF%A5%CB%A5%AA%A5%F3%B5%D5%B1%BF%C6%B0%B3%D8
				Quaternion ln() const;

				//��]��
				nc::math::Vector3 axis() const;

				//��]���ɑ΂���p�x
				real_t	angle() const;

				//yaw/pitch/roll�p
				real_t yaw() const;
				real_t pitch() const;
				real_t roll() const;
			};

		}//namespace math 

	}//namespace generic

}//namespace nc

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_QUATERNION

#include "PLT_Quaternion.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::Quaternion;

	}//namespace math

}//namespace nc

#endif

namespace nc{

	namespace math{
		
		//�Œ�l�x�N�g��
		static const Quaternion	QuaternionZero( 0, 0, 0, 0 );
		static const Quaternion	QuaternionUnit( 0, 0, 0, 1 );

		//�C�ӎ��̉�]��\���N�H�[�^�j�I��
		//��axis�͒P�ʃx�N�g���ł���K�v������B
		Quaternion quaternionRotateAxis( const Vector3& axis, real_t rad );

		//���W�A��
		Quaternion quaternionRotateYawPitchRoll( real_t yaw, real_t pitch, real_t roll );

		//Vector��Quaternion�ŕϊ��i��]�j
		Vector4 transform( const Quaternion& q, const Vector4& vec );
		Vector3 transform( const Quaternion& q, const Vector3& vec );	//W = 0�Ƃ��Čv�Z����܂��B

		//Point
		Point4 transform( const Quaternion& q, const Point4& pt );
		Point3 transform( const Quaternion& q, const Point3& pt );	//W = 0�Ƃ��Čv�Z����܂��B

	}//namespace math

}//namespace nc

#endif // __NC_QUATERNION_h__