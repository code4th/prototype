#ifndef __NC_PLT_VECTOR_h__
#define __NC_PLT_VECTOR_h__

namespace nc{

	namespace platform{

		namespace math{

			using nc::generic::math::Vector2;
			//using nc::generic::math::Vector4;

			//プラットフォーム拡張版 Vector3
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

				//内積
				real_t dot( const Vector3& rhs ) const;

				/*
				//外積
				Vector3 cross( const Vector3& rhs ) const;

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
				*/
			};

			//プラットフォーム拡張版 Vector4
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

				//内積
				real_t dot( const Vector4& rhs ) const;

				/*
				//外積
				//★未実装★ help me!
				Vector4 cross( const Vector4& vec2, const Vector4& rhs3 );

				//Vectorの長さ
				real_t length() const;

				//Vectorの長さ（平方）
				real_t lengthSquared() const;

				//正規化します
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