#ifndef __NC_COORDINATE4DATA_h__
#define __NC_COORDINATE4DATA_h__

#include "CoordinateData.h"


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_COORDINATE_DATA

#include "PLT_Coordinate4Data.h"

#else

namespace nc{

	//namespace math setting
	namespace math{

		struct Coordinate4DataImpl{

			union{
				real_t idx[ 4 ];
				struct{
					real_t x, y, z, w;
				};
			};		

		};

		//•K—v‚É‰‚¶‚Ä–¼‘O‚ğÚ‚¹‘Ö‚¦‚Ä‚­‚¾‚³‚¢
		//using nc::generic::math::Coordinate4Data;

	}//namespace math

}//namespace nc

#endif

namespace nc{

//	namespace generic{

		namespace math{
			
			struct Coordinate4Data : public nc::math::Coordinate4DataImpl{

				typedef	nc::math::CoordinateHasData_XYZW_tag CoordinateHasData_Spec_tag;
// 
// 				union{
// 					real_t idx[ 4 ];
// 					struct{
// 						real_t x, y, z, w;
// 					};
// 				};		

				//‘ã“ü

				//’P€ +/-‰‰Zq
				Coordinate4Data operator+ () const;
				Coordinate4Data operator- () const;

				//ƒxƒNƒgƒ‹‚Ì‰ÁŒ¸Z
				Coordinate4Data operator+ ( const Coordinate4Data& rhs ) const;
				Coordinate4Data operator- ( const Coordinate4Data& rhs ) const;
				const Coordinate4Data& operator+=( const Coordinate4Data& rhs );
				const Coordinate4Data& operator-=( const Coordinate4Data& rhs );

				//ƒXƒJƒ‰[”{
				Coordinate4Data operator* ( real_t bias ) const;
				Coordinate4Data operator/ ( real_t bias ) const;
				const Coordinate4Data& operator*=( real_t rhs );
				const Coordinate4Data& operator/=( real_t rhs );

				//”äŠr‰‰Z
				bool operator== ( const Coordinate4Data& rhs ) const;
				bool operator!= ( const Coordinate4Data& rhs ) const;

				//‘ã“ü
				Coordinate4Data& assign( const nc::math::Coordinate2Data& rhs, real_t z = 0, real_t w = 0 );
				Coordinate4Data& assign( const nc::math::Coordinate3Data& rhs, real_t w = 0 );
				Coordinate4Data& assign( const Coordinate4Data& rhs );
				Coordinate4Data& assign( const Swizzle4<real_t>& rhs );
				Coordinate4Data& assign( real_t x, real_t y, real_t z, real_t w );
				Coordinate4Data& assign( const real_t arr[] );

				real_t& operator[]( int n )
				{
					return idx[n];
				}

				const real_t& operator[]( int n ) const
				{
					return idx[n];
				}

				//Coordinate3Data‚Ö‚Ì³Ë‰e
				//x' = x, y' = y, z' = z  as is.
				nc::math::Coordinate3Data orthoProjection() const;

				//Coordinate3Data‚Ö‚Ì“§‹Ë‰e
				//e = Å“_
				//x' = x * (e / w), y' = y * (e / w), z' = z * (e / w)
				nc::math::Coordinate3Data persProjection( real_t e = 1.0f ) const;
			};

			Coordinate4Data operator * ( real_t lhs, const Coordinate4Data& rhs );




			inline Coordinate4Data Coordinate4Data::operator+() const
			{
				return *this;
			}

			inline Coordinate4Data Coordinate4Data::operator-() const
			{
				return Coordinate4Data().assign( -x, -y, -z, -w );
			}


			inline Coordinate4Data Coordinate4Data::operator+( const Coordinate4Data& rhs ) const
			{
				return Coordinate4Data().assign( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
			}

			inline Coordinate4Data Coordinate4Data::operator-( const Coordinate4Data& rhs ) const
			{
				return Coordinate4Data().assign( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
			}

			inline const Coordinate4Data& Coordinate4Data::operator+=( const Coordinate4Data& rhs )
			{
				*this = *this + rhs;
				return *this;
			}

			inline const Coordinate4Data& Coordinate4Data::operator-=( const Coordinate4Data& rhs )
			{
				*this = *this - rhs;
				return *this;
			}


			inline Coordinate4Data Coordinate4Data::operator*( real_t bias ) const
			{
				return Coordinate4Data().assign( x * bias, y * bias, z * bias, w * bias );
			}

			inline Coordinate4Data Coordinate4Data::operator/( real_t bias ) const
			{
				return Coordinate4Data().assign( x / bias, y / bias, z / bias, w / bias );
			}

			inline const Coordinate4Data& Coordinate4Data::operator*=( real_t rhs )
			{
				*this = *this * rhs;
				return *this;
			}

			inline const Coordinate4Data& Coordinate4Data::operator/=( real_t rhs )
			{
				*this = *this / rhs;
				return *this;
			}


			inline bool Coordinate4Data::operator==( const Coordinate4Data& rhs ) const
			{
				return ( isEqual( x, rhs.x ) && isEqual( y, rhs.y ) && isEqual( z, rhs.z ) && isEqual( w, rhs.w ) ) ? true : false;
			}

			inline bool Coordinate4Data::operator!=( const Coordinate4Data& rhs ) const
			{
				return !operator==( rhs );
			}


			inline Coordinate4Data& Coordinate4Data::assign( const nc::math::Coordinate2Data& rhs, real_t z /*= 0*/, real_t w /*= 0 */ )
			{
				this->x = rhs.x;
				this->y = rhs.y;
				this->z = z;
				this->w = w;
				return *this;
			}

			inline Coordinate4Data& Coordinate4Data::assign( const nc::math::Coordinate3Data& rhs, real_t w /*= 0 */ )
			{
				this->x = rhs.x;
				this->y = rhs.y;
				this->z = rhs.z;
				this->w = w;
				return *this;
			}

			inline Coordinate4Data& Coordinate4Data::assign( const Coordinate4Data& rhs )
			{
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				w = rhs.w;
				return *this;
			}

			inline Coordinate4Data& Coordinate4Data::assign( const Swizzle4<real_t>& rhs )
			{
				x = rhs.d0_;
				y = rhs.d1_;
				z = rhs.d2_;
				w = rhs.d3_;
				return *this;
			}

			inline Coordinate4Data& Coordinate4Data::assign( real_t x, real_t y, real_t z, real_t w )
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
				return *this;
			}

			inline Coordinate4Data& Coordinate4Data::assign( const real_t arr[] )
			{
				this->x = arr[0];
				this->y = arr[1];
				this->z = arr[2];
				this->w = arr[3];

				return *this;
			}

			inline nc::math::Coordinate3Data Coordinate4Data::orthoProjection() const
			{
				return nc::math::Coordinate3Data().assign( x, y, z );
			}

			inline nc::math::Coordinate3Data Coordinate4Data::persProjection( real_t e /*= 1.0f */ ) const
			{
				real_t n = (e / w);
				return nc::math::Coordinate3Data().assign( x * n, y * n, z * n );
			}


			inline Coordinate4Data operator * ( real_t lhs, const Coordinate4Data& rhs )
			{
				return rhs * lhs;
			}


		}//namespace math

//	}//namespace generic


}//namespace nc


#endif // __NC_COORDINATE4DATA_h__
