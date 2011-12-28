
#include "Quaternion.h"

namespace nc{

	namespace generic{

		namespace math{

			using namespace nc::math;

			const Quaternion& Quaternion::operator*=( const Quaternion& rhs )
			{
				*this = *this * rhs;
				return *this;
			}

			nc::math::Quaternion Quaternion::operator*( const Quaternion& rhs ) const
			{
				//Q1*Q2 = (t1t2-V1・V2; t1V2 + t2V1 + V1×V2)
				return Quaternion(
					w * rhs.x +
					rhs.w * x +
					y * rhs.z +
					-z * rhs.y,

					w * rhs.y +
					rhs.w * y +
					z * rhs.x +
					-x * rhs.z,

					w * rhs.z +
					rhs.w * z +
					x * rhs.y +
					-y * rhs.x,

					w * rhs.w +      
					-x * rhs.x +      
					-y * rhs.y +      
					-z * rhs.z
					);
			}

			real_t Quaternion::dot( const Quaternion& rhs ) const
			{
				return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
			}

			real_t Quaternion::length() const
			{
				return sqrt( lengthSquared() );
			}

			real_t Quaternion::lengthSquared() const
			{
				return x * x + y * y + z * z + w * w;
			}

			nc::math::Quaternion Quaternion::normalize()
			{
				*this /= length();
				return *this;
			}

			nc::math::Quaternion Quaternion::conjugate() const
			{
				return Quaternion( -x, -y, -z, w );
			}

			nc::math::Quaternion Quaternion::inverse() const
			{
				return conjugate() / lengthSquared();
			}

			nc::math::Quaternion Quaternion::ln() const
			{
				const real_t theta = acos( w );
				const real_t isintheta = theta / sin( theta );
				return Quaternion( x * isintheta, y * isintheta, z * isintheta, 0 );
			}

			nc::math::Vector3 Quaternion::axis() const
			{
				real_t s = sqrt(x * x + y * y + z * z);
				return Vector3( x/s, y/s, z/s );
			}

			real_t Quaternion::angle() const
			{
				return acos(w) * 2.0f;
			}

			real_t Quaternion::yaw() const
			{
				return asin(-2 * (x * z - w * y));
			}

			real_t Quaternion::pitch() const
			{
				return atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z);
			}

			real_t Quaternion::roll() const
			{
				return atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z);
			}

			//---------------------------------------

		}//namespace math

	}//namespace generic

	namespace math{

		//任意軸の回転を表すクォータニオン
		//軸は単位ベクトルである必要がある。
		nc::math::Quaternion quaternionRotateAxis( const Vector3& axis, real_t rad )
		{
			return Quaternion( axis * sin( rad / 2 ), cos( rad / 2 ) );
		}

		Quaternion quaternionRotateYawPitchRoll( real_t yaw, real_t pitch, real_t roll )
		{
			real_t p = pitch / 2.0f;
			real_t y = yaw / 2.0f;
			real_t r = roll / 2.0f;

			real_t sinp = sin(p);
			real_t siny = sin(y);
			real_t sinr = sin(r);
			real_t cosp = cos(p);
			real_t cosy = cos(y);
			real_t cosr = cos(r);

			return Quaternion(
				sinr * cosp * cosy - cosr * sinp * siny,
				cosr * sinp * cosy + sinr * cosp * siny,
				cosr * cosp * siny - sinr * sinp * cosy,
				cosr * cosp * cosy + sinr * sinp * siny
				).normalize();
		}

		Vector3 transform( const Quaternion& q, const Vector3& vec )
		{
			return transform( q, Vector4( vec, 0 ) ).orthoProjection();
		}

		Vector4 transform( const Quaternion& q, const Vector4& vec )
		{
			return q * vec * q.conjugate();
		}

		Point4 transform( const Quaternion& q, const Point4& pt )
		{
			return transform( q, Vector4(pt) );
		}

		Point3 transform( const Quaternion& q, const Point3& pt )
		{
			return transform( q, Vector3(pt) );
		}

	}//namespace math

}//namespace nc
