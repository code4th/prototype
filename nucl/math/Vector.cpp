
#include "Vector.h"

namespace nc{
	namespace generic{

		namespace math{

			/*
			using namespace nc::math;

			real_t Vector2::ccw( const Vector2& rhs ) const
			{
				return (x * rhs.y) - (y * rhs.x);
			}

			real_t Vector2::length() const
			{
				return sqrt( lengthSquared() );
			}

			real_t Vector2::lengthSquared() const
			{
				return x * x + y * y;
			}

			nc::math::Vector2 Vector2::normalize()
			{
				*this /= length();
				return *this;
			}

			nc::math::Vector2 Vector2::direction() const
			{
				return Vector2( *this ).normalize();
			}


			//-----------------------------------------------

			real_t Vector3::length() const
			{
				return sqrt( lengthSquared() );
			}

			real_t Vector3::lengthSquared() const
			{
				return x * x + y * y + z * z;
			}

			real_t Vector3::approximateLength() const
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

			Vector3 Vector3::normalize()
			{
				*this /= length();
				return *this;
			}

			Vector3 Vector3::direction() const
			{
				return Vector3( *this ).normalize();
			}

			//-----------------------------------------------

			real_t Vector4::length() const
			{
				return sqrt( lengthSquared() );
			}

			real_t Vector4::lengthSquared() const
			{
				return x * x + y * y + z * z + w * w;
			}

			Vector4 Vector4::normalize()
			{
				*this /= length();
				return *this;
			}
			*/
		}//namespace math
	}//namespace generic
}//namespace nc
