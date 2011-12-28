#ifndef __NC_PLT_VECTOR_h__
#define __NC_PLT_VECTOR_h__

namespace nc{

	namespace platform{

		namespace math{

			using nc::generic::math::Vector2;
			//using nc::generic::math::Vector4;

			//�v���b�g�t�H�[���g���� Vector3
			struct Vector3 : public nc::generic::math::Vector3{
				Vector3() {};
				Vector3( const Vector3& rhs )								: nc::generic::math::Vector3(rhs) {}
				Vector3( const Coordinate3& rhs )							: nc::generic::math::Vector3(rhs) {}
				Vector3( const Coordinate3Data& rhs )						: nc::generic::math::Vector3(rhs) {}
				Vector3( const Vector2& rhs, real_t z )						: nc::generic::math::Vector3(rhs, z) {}
				Vector3( real_t x, real_t y, real_t z )						: nc::generic::math::Vector3(x, y, z) {}
				Vector3( const real_t arr[] )								: nc::generic::math::Vector3(arr) {}

				const Vector3& operator= ( const Vector3& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }
				const Vector3& operator= ( const Coordinate3& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }
				const Vector3& operator= ( const Coordinate3Data& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }

				//����
				real_t dot( const Vector3& rhs ) const;

				/*
				//�O��
				Vector3 cross( const Vector3& rhs ) const;

				//Vector�̒��� slow.
				real_t length() const;

				//Vector�̒����i�����jfastest.
				real_t lengthSquared() const;

				//Vector�̂����悻�̒����inot strictly. but faster than length?�j
				real_t approximateLength() const;

				//���K�����܂�
				Vector3 normalize();

				//���K�����ꂽ������Ԃ��܂��B
				//������normalize���Ă܂��B
				Vector3 direction() const;
				*/
			};

			//�v���b�g�t�H�[���g���� Vector4
			struct Vector4 : public nc::generic::math::Vector4{

				Vector4() {};
				Vector4( const Vector4& rhs )								: nc::generic::math::Vector4(rhs) {}
				Vector4( const Coordinate4& rhs )							: nc::generic::math::Vector4(rhs) {}
				Vector4( const Vector3& rhs, real_t w )						: nc::generic::math::Vector4(rhs, w) {}
				Vector4( const Vector2& rhs, real_t z, real_t w )			: nc::generic::math::Vector4(rhs, z, w) {}
				Vector4( real_t x, real_t y, real_t z, real_t w )			: nc::generic::math::Vector4(x, y, z, w) {}
				Vector4( const real_t arr[] )								: nc::generic::math::Vector4(arr) {}

				const Vector4& operator= ( const Vector4& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }
				const Vector4& operator= ( const Coordinate4& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }
				const Vector4& operator= ( const Coordinate4Data& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }

				//����
				real_t dot( const Vector4& rhs ) const;

				/*
				//�O��
				//���������� help me!
				Vector4 cross( const Vector4& vec2, const Vector4& rhs3 );

				//Vector�̒���
				real_t length() const;

				//Vector�̒����i�����j
				real_t lengthSquared() const;

				//���K�����܂�
				Vector4 normalize();
				*/
			};

		}//namespace math

	}//namespace platform

	//namespace setup
	namespace math{

		using nc::generic::math::Vector2;
		using nc::platform::math::Vector3;
		using nc::platform::math::Vector4;

	}//namespace math

}//namespace nc

#endif // __NC_PLT_VECTOR_h__