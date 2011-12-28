#ifndef __NC_COORDINATE3DATA_h__
#define __NC_COORDINATE3DATA_h__

#include "CoordinateData.h"

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_COORDINATE_DATA

#include "PLT_Coordinate3Data.h"

#else

namespace nc{

	//namespace math setting
	namespace math{

		struct Coordinate3DataImpl{
			union{
				real_t idx[ 3 ];
				struct{
					real_t x, y, z;
				};
			};
		};

		//•K—v‚É‰‚¶‚Ä–¼‘O‚ğÚ‚¹‘Ö‚¦‚Ä‚­‚¾‚³‚¢
		//using nc::generic::math::Coordinate3Data;

	}//namespace math

}//namespace nc

#endif

namespace nc{

//	namespace generic{

		namespace math{

			struct Coordinate3Data : public Coordinate3DataImpl{

				typedef	nc::math::CoordinateHasData_XYZ_tag CoordinateHasData_Spec_tag;
// 
// 				union{
// 					real_t idx[ 3 ];
// 					struct{
// 						real_t x, y, z;
// 					};
// 				};


				//’P€ +/-‰‰Zq
				Coordinate3Data operator+ () const;
				Coordinate3Data operator- () const;

				//ƒxƒNƒgƒ‹‚Ì‰ÁŒ¸Z
				Coordinate3Data operator+ ( const Coordinate3Data& rhs ) const;
				Coordinate3Data operator- ( const Coordinate3Data& rhs ) const;
				const Coordinate3Data& operator+=( const Coordinate3Data& rhs );
				const Coordinate3Data& operator-=( const Coordinate3Data& rhs );

				//ƒXƒJƒ‰[”{
				Coordinate3Data operator* ( real_t bias ) const;
				Coordinate3Data operator/ ( real_t bias ) const;
				const Coordinate3Data& operator*=( real_t rhs );
				const Coordinate3Data& operator/=( real_t rhs );

				//”äŠr‰‰Z
				bool operator== ( const Coordinate3Data& rhs ) const;
				bool operator!= ( const Coordinate3Data& rhs ) const;

				//‘ã“ü
				Coordinate3Data& assign( const nc::math::Coordinate2Data& rhs, real_t z = 0 );
				Coordinate3Data& assign( const Coordinate3Data& rhs );
				Coordinate3Data& assign( const Swizzle3<real_t>& rhs );
				Coordinate3Data& assign( real_t x, real_t y, real_t z );
				Coordinate3Data& assign( const real_t arr[] );

				//Coordinate2Data‚Ö‚Ì³Ë‰e
				//x' = x, y' = y.  as is.
				nc::math::Coordinate2Data orthoProjection() const;

				//Coordinate2Data‚Ö‚Ì“§‹Ë‰e
				//e = Å“_
				//x' = x * (e / z), y' = y * (e / z)
				nc::math::Coordinate2Data persProjection( real_t e = 1.0f ) const;

				real_t& operator[]( int n )
				{
					return idx[n];
				}

				const real_t& operator[]( int n ) const
				{
					return idx[n];
				}
			};

			Coordinate3Data operator * ( real_t lhs, const Coordinate3Data& rhs );



			inline Coordinate3Data Coordinate3Data::operator+ () const
			{
				return *this;
			}


			inline Coordinate3Data Coordinate3Data::operator- () const
			{
				return Coordinate3Data().assign( -x, -y, -z );
			}

			inline Coordinate3Data Coordinate3Data::operator+ ( const Coordinate3Data& rhs ) const
			{
				return Coordinate3Data().assign( x + rhs.x, y + rhs.y, z + rhs.z );
			}

			inline Coordinate3Data Coordinate3Data::operator- ( const Coordinate3Data& rhs ) const
			{
				return Coordinate3Data().assign( x - rhs.x, y - rhs.y, z - rhs.z );
			}

			inline const Coordinate3Data& Coordinate3Data::operator+=( const Coordinate3Data& rhs )
			{
				*this = *this + rhs;
				return *this;
			}

			inline const Coordinate3Data& Coordinate3Data::operator-=( const Coordinate3Data& rhs )
			{
				*this = *this - rhs;
				return *this;
			}

			inline Coordinate3Data Coordinate3Data::operator* ( real_t bias ) const
			{
				return Coordinate3Data().assign( x * bias, y * bias, z * bias );
			}

			inline Coordinate3Data Coordinate3Data::operator/ ( real_t bias ) const
			{
				return Coordinate3Data().assign( x / bias, y / bias, z / bias );
			}

			inline const Coordinate3Data& Coordinate3Data::operator*=( real_t rhs )
			{
				*this = *this * rhs;
				return *this;
			}

			inline const Coordinate3Data& Coordinate3Data::operator/=( real_t rhs )
			{
				*this = *this / rhs;
				return *this;
			}


			inline bool Coordinate3Data::operator== ( const Coordinate3Data& rhs ) const
			{
				return ( isEqual( x, rhs.x ) && isEqual( y, rhs.y ) && isEqual( z, rhs.z ) ) ? true : false;
			}

			inline bool Coordinate3Data::operator!= ( const Coordinate3Data& rhs ) const
			{
				return !operator==( rhs );
			}


			inline Coordinate3Data& Coordinate3Data::assign( const nc::math::Coordinate2Data& rhs, real_t z /*= 0 */ )
			{
				x = rhs.x;
				y = rhs.y;
				z = z;
				return *this;
			}

			inline Coordinate3Data& Coordinate3Data::assign( const Coordinate3Data& rhs )
			{
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				return *this;
			}

			inline Coordinate3Data& Coordinate3Data::assign( const Swizzle3<real_t>& rhs )
			{
				x = rhs.d0_;
				y = rhs.d1_;
				z = rhs.d2_;
				return *this;
			}

			inline Coordinate3Data& Coordinate3Data::assign( real_t x, real_t y, real_t z )
			{
				this->x = x; this->y = y; this->z = z;
				return *this;
			}

			inline Coordinate3Data& Coordinate3Data::assign( const real_t arr[] )
			{
				this->x = arr[0]; this->y = arr[1];
				return *this;
			}

			inline nc::math::Coordinate2Data Coordinate3Data::orthoProjection() const
			{
				return nc::math::Coordinate2Data().assign( x, y );
			}

			inline nc::math::Coordinate2Data Coordinate3Data::persProjection( real_t e /*= 1.0f */ ) const
			{
				real_t n = (e / z);
				return nc::math::Coordinate2Data().assign( x * n, y * n );
			}


			inline Coordinate3Data operator * ( real_t lhs, const Coordinate3Data& rhs )
			{
				return rhs * lhs;
			}

		}//namespace math

//	}//namespace generic


}//namespace nc




#endif // __NC_COORDINATE3DATA_h__
