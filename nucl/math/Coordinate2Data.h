#ifndef __NC_COORDINATE2DATA_h__
#define __NC_COORDINATE2DATA_h__

#include "CoordinateData.h"


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_COORDINATE_DATA

#include "PLT_Coordinate2Data.h"

#else

namespace nc{

	//namespace math setting
	namespace math{

		struct Coordinate2DataImpl{
			union{
				real_t idx[ 2 ];
				struct{
					real_t x, y;
				};
			};
		};

	}//namespace math

}//namespace nc

#endif


namespace nc{

//	namespace generic{

		namespace math{

			struct Coordinate2Data : public nc::math::Coordinate2DataImpl{

				typedef	nc::math::CoordinateHasData_XY_tag CoordinateHasData_Spec_tag;

// 				union{
// 					real_t idx[ 2 ];
// 					struct{
// 						real_t x, y;
// 					};
// 				};


				//íPçÄ +/-ââéZéq
				Coordinate2Data operator+ () const;
				Coordinate2Data operator- () const;

				//â¡å∏éZ
				Coordinate2Data operator+ ( const Coordinate2Data& rhs ) const;
				Coordinate2Data operator- ( const Coordinate2Data& rhs ) const;
				const Coordinate2Data& operator+=( const Coordinate2Data& rhs );
				const Coordinate2Data& operator-=( const Coordinate2Data& rhs );

				//ÉXÉJÉâÅ[î{
				Coordinate2Data operator* ( real_t bias ) const;
				Coordinate2Data operator/ ( real_t bias ) const;
				const Coordinate2Data& operator*=( real_t rhs );
				const Coordinate2Data& operator/=( real_t rhs );

				//î‰ärââéZ
				bool operator== ( const Coordinate2Data& rhs ) const;
				bool operator!= ( const Coordinate2Data& rhs ) const;

				//ë„ì¸
				Coordinate2Data& assign( const Coordinate2Data& rhs );
				Coordinate2Data& assign( const Swizzle2<real_t>& rhs );
				Coordinate2Data& assign( real_t x, real_t y  );
				Coordinate2Data& assign( const real_t arr[]  );

				real_t& operator[]( int n )
				{
					return idx[n];
				}

				const real_t& operator[]( int n ) const
				{
					return idx[n];
				}

			};

			Coordinate2Data operator * ( real_t lhs, const Coordinate2Data& rhs );




			inline Coordinate2Data Coordinate2Data::operator- () const
			{
				return Coordinate2Data().assign( -x, -y );
			}

			inline Coordinate2Data Coordinate2Data::operator+ () const
			{
				return *this;
			}

			inline Coordinate2Data Coordinate2Data::operator+ ( const Coordinate2Data& rhs ) const
			{
				return Coordinate2Data().assign( x + rhs.x, y + rhs.y );
			}

			inline Coordinate2Data Coordinate2Data::operator- ( const Coordinate2Data& rhs ) const
			{
				return Coordinate2Data().assign( x - rhs.x, y - rhs.y );
			}

			inline const Coordinate2Data& Coordinate2Data::operator+=( const Coordinate2Data& rhs )
			{
				*this = *this + rhs;
				return *this;
			}

			inline const Coordinate2Data& Coordinate2Data::operator-=( const Coordinate2Data& rhs )
			{
				*this = *this - rhs;
				return *this;
			}

			inline Coordinate2Data Coordinate2Data::operator* ( real_t bias ) const
			{
				return Coordinate2Data().assign( x * bias, y * bias );
			}

			inline Coordinate2Data Coordinate2Data::operator/ ( real_t bias ) const
			{
				return Coordinate2Data().assign( x / bias, y / bias );
			}

			inline const Coordinate2Data& Coordinate2Data::operator*=( real_t rhs )
			{
				*this = *this * rhs;
				return *this;
			}

			inline const Coordinate2Data& Coordinate2Data::operator/=( real_t rhs )
			{
				*this = *this / rhs;
				return *this;
			}


			inline bool Coordinate2Data::operator== ( const Coordinate2Data& rhs ) const
			{
				return ( isEqual( x, rhs.x ) && isEqual( y, rhs.y ) ) ? true : false;
			}

			inline bool Coordinate2Data::operator!= ( const Coordinate2Data& rhs ) const
			{
				return !operator==( rhs );
			}

			inline Coordinate2Data& Coordinate2Data::assign( const Coordinate2Data& rhs )
			{
				x = rhs.x;
				y = rhs.y;
				return *this;
			}

			inline Coordinate2Data& Coordinate2Data::assign( const Swizzle2<real_t>& rhs )
			{
				x = rhs.d0_;
				y = rhs.d1_;
				return *this;
			}

			inline Coordinate2Data& Coordinate2Data::assign( real_t x, real_t y )
			{
				this->x = x; this->y = y;
				return *this;
			}

			inline Coordinate2Data& Coordinate2Data::assign( const real_t arr[]  )
			{
				this->x = arr[0]; this->y = arr[1];
				return *this;
			}

			inline Coordinate2Data operator * ( real_t lhs, const Coordinate2Data& rhs )
			{
				return rhs * lhs;
			}


		}//namespace math

//	}//namespace generic


}//namespace nc


#endif // __NC_COORDINATE2DATA_h__
