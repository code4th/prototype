#ifndef __NC_VECTOR_h__
#define __NC_VECTOR_h__

#include "nucl/Types.h"
#include "nucl/math/Math.h"
#include "nucl/math/Coordinate.h"
#include "nucl/TypeTraits.h"
#include "nucl/Assert.h"

namespace nc{

	namespace generic{

		namespace math{

			//同次座標系にしたときにz = 0
			struct Vector2 : public nc::math::Coordinate2{

				Vector2() {};
				Vector2( const Vector2& rhs )					: Coordinate2(rhs) {}
				Vector2( const Coordinate2& rhs )		: Coordinate2(rhs) {}
				Vector2( const nc::math::Coordinate2Data& rhs )	: Coordinate2(rhs) {}
				Vector2( real_t x, real_t y )			: Coordinate2(x, y) {}
				Vector2( const real_t arr[] )			: Coordinate2(arr) {}

				const Vector2& operator= ( const Vector2& rhs ){ return static_cast< const Vector2 & >( assign(rhs) ); }
				const Vector2& operator= ( const Coordinate2& rhs ){ return static_cast< const Vector2 & >( assign(rhs) ); }
				const Vector2& operator= ( const Coordinate2Data& rhs ){ return static_cast< const Vector2 & >( assign(rhs) ); }

				//の関数は、次の公式に基づく外積から z 要素を決定する。((this->x, this->y, 0) cross (rhs.x, rhs.y, 0))
				//z 要素の値が正の場合、ベクトル rhs はベクトル this から反時計回りである。この情報は、背面のカリングに役立つ。
				real_t ccw( const Vector2& rhs ) const;

				//Vectorの長さ
				real_t length() const;

				//Vectorの長さ（平方）fastest.
				real_t lengthSquared() const;

				//正規化します。
				Vector2 normalize();

				//正規化された方向を返します。
				//内部でnormalizeしてます。
				Vector2 direction() const;
			};

			//同次座標系にしたときにw = 0
			struct Vector3 : public nc::math::Coordinate3{

				Vector3() {};
				Vector3( const Vector3& rhs )				: Coordinate3(rhs) {}
				Vector3( const Coordinate3& rhs )			: Coordinate3(rhs) {}
				Vector3( const nc::math::Coordinate3Data& rhs )			: Coordinate3(rhs) {}
				Vector3( const Vector2& rhs, real_t z )		: Coordinate3(rhs, z) {}
				Vector3( real_t x, real_t y, real_t z )		: Coordinate3(x, y, z) {}
				Vector3( const real_t arr[] )				: Coordinate3(arr) {}

				const Vector3& operator= ( const Vector3& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }
				const Vector3& operator= ( const Coordinate3& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }
				const Vector3& operator= ( const Coordinate3Data& rhs ){ return static_cast< const Vector3 & >( assign(rhs) ); }

				//Vectorの長さ slow.
				real_t length() const;

				//Vectorの長さ（平方）fastest.
				real_t lengthSquared() const;

				//Vectorのおおよその長さ（not strictly. but faster than length?）
				real_t approximateLength() const;

				//正規化します
				Vector3 normalize();

				//正規化された方向を返します。
				//内部でnormalizeしてます。
				Vector3 direction() const;
			};

			//同次座標系にしたときにz = 0
			struct Vector4 : public nc::math::Coordinate4{

				Vector4() {};
				Vector4( const Vector4& rhs )								: Coordinate4(rhs) {}
				Vector4( const Coordinate4& rhs )							: Coordinate4(rhs) {}
				Vector4( const nc::math::Coordinate4Data& rhs )							: Coordinate4(rhs) {}
				Vector4( const nc::math::Coordinate3Data& rhs, real_t w )						: Coordinate4(rhs, w) {}
				Vector4( const Vector2& rhs, real_t z, real_t w )			: Coordinate4(rhs, z, w) {}
				Vector4( real_t x, real_t y, real_t z, real_t w )			: Coordinate4(x, y, z, w) {}
				Vector4( const real_t arr[] )								: Coordinate4(arr) {}

				const Vector4& operator= ( const Vector4& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }
				const Vector4& operator= ( const Coordinate4& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }
				const Vector4& operator= ( const Coordinate4Data& rhs ){ return static_cast< const Vector4 & >( assign(rhs) ); }

				//Vectorの長さ
				real_t length() const;

				//Vectorの長さ（平方）
				real_t lengthSquared() const;

				//正規化します
				Vector4 normalize();
			};


			//--------------------inline impliment--------------------------


			inline real_t Vector2::ccw( const Vector2& rhs ) const
			{
				return (x * rhs.y) - (y * rhs.x);
			}

			inline real_t Vector2::length() const
			{
				return nc::math::sqrt( lengthSquared() );
			}

			inline real_t Vector2::lengthSquared() const
			{
				return x * x + y * y;
			}

			inline Vector2 Vector2::normalize()
			{
				*this /= length();
				return *this;
			}

			inline Vector2 Vector2::direction() const
			{
				return Vector2( *this ).normalize();
			}

			//-------------------------------

			inline real_t Vector3::length() const
			{
				return nc::math::sqrt( lengthSquared() );
			}

			inline real_t Vector3::lengthSquared() const
			{
				return x * x + y * y + z * z;
			}

			inline real_t Vector3::approximateLength() const
			{
				real_t a, b, c;
				if (x < 0.0f) a = -x;
				else a = x;

				if (y < 0.0f) b = -y;
				else b = y;

				if (z < 0.0f) c = -z;
				else c = z;

				if (a < b)
				{
					real_t t = a;
					a = b;
					b = t;
				}

				if (a < c)
				{
					real_t t = a;
					a = c;
					c = t;
				}
				return a * 0.9375f + (b + c) * 0.375f;
			}

			inline Vector3 Vector3::normalize()
			{
				*this /= length();
				return *this;
			}

			inline Vector3 Vector3::direction() const
			{
				return Vector3( *this ).normalize();
			}

			//-------------------------------

			inline real_t Vector4::length() const
			{
				return nc::math::sqrt( lengthSquared() );
			}

			inline real_t Vector4::lengthSquared() const
			{
				return x * x + y * y + z * z + w * w;
			}

			inline Vector4 Vector4::normalize()
			{
				*this /= length();
				return *this;
			}

		}//namespace math

	}//namespace generic

}//namespace nc




#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_VECTOR

#include "PLT_Vector.h"

#else

namespace nc{

	//namespace setup
	namespace math{

		using nc::generic::math::Vector2;
		using nc::generic::math::Vector3;
		using nc::generic::math::Vector4;

	}//namespace math

}//namespace nc

#endif

namespace nc{

	namespace math{

		//固定値ベクトル
		static const Vector2	Vector2Zero( 0,0 );
		static const Vector3	Vector3Zero( 0,0,0 );
		static const Vector4	Vector4Zero( 0,0,0,0 );

		static const Vector2	Vector2AxisX( 1, 0 );
		static const Vector2	Vector2AxisY( 0, 1 );

		static const Vector3	Vector3AxisX( 1, 0, 0 );
		static const Vector3	Vector3AxisY( 0, 1, 0 );
		static const Vector3	Vector3AxisZ( 0, 0, 1 );

		static const Vector4	Vector4AxisX( 1, 0, 0, 0 );
		static const Vector4	Vector4AxisY( 0, 1, 0, 0 );
		static const Vector4	Vector4AxisZ( 0, 0, 1, 0 );
		static const Vector4	Vector4AxisW( 0, 0, 0, 1 );

	}//nsmespace math

}//namespace nc

//#include "VectorUtil.h"

#endif // __NC_VECTOR_h__

