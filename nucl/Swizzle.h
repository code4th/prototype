#ifndef __NC_SWIZZLE_h__
#define __NC_SWIZZLE_h__

#include "Assert.h"

namespace nc{

	template< typename T >
	class Swizzle2{
	public:
		//メモリ的に連続していないので注意
		T& d0_;
		T& d1_;

		Swizzle2( const Swizzle2& rhs )
			: d0_(rhs.d0_)
			, d1_(rhs.d1_)
		{}

		Swizzle2( T& d0, T& d1 )
			: d0_(d0)
			, d1_(d1)
		{}

		const T& operator[]( uint32_t idx ) const
		{
			//NC_ASSERT( idx < 2 );
			if( idx == 0 ) return d0_;
			else return d1_;
		}

		T& operator[]( uint32_t idx )
		{
			//NC_ASSERT( idx < 2 );
			if( idx == 0 ) return d0_;
			else return d1_;
		}
	};

	template< typename T >
	class Swizzle3{
	public:
		//メモリ的に連続していないので注意
		T& d0_;
		T& d1_;
		T& d2_;

		Swizzle3( const Swizzle3& rhs )
			: d0_(rhs.d0_)
			, d1_(rhs.d1_)
			, d2(rhs.d2_)		
		{}

		Swizzle3( T& d0, T& d1, T& d2 )
			: d0_(d0)
			, d1_(d1)
			, d2_(d2)
		{}

		const T& operator[]( uint32_t idx ) const
		{
			//NC_ASSERT( idx < 3 );
			switch( idx )
			{
			case 0:	return d0_;
			case 1:	return d1_;
			case 2:	return d2_;
			}
			return d2_;
		}

		T& operator[]( uint32_t idx )
		{
			//NC_ASSERT( idx < 3 );
			switch( idx )
			{
			case 0:	return d0_;
			case 1:	return d1_;
			case 2:	return d2_;
			}
			return d2_;
		}
	};

	template< typename T >
	class Swizzle4{
	public:
		//メモリ的に連続していないので注意
		T& d0_;
		T& d1_;
		T& d2_;
		T& d3_;

		Swizzle4( const Swizzle4& rhs )
			: d0_(rhs.d0_)
			, d1_(rhs.d1_)
			, d2(rhs.d2_)		
			, d3(rhs.d3_)		
		{}

		Swizzle4( T& d0, T& d1, T& d2, T& d3 )
			: d0_(d0)
			, d1_(d1)
			, d2_(d2)
			, d3_(d3)
		{}

		const T& operator[]( uint32_t idx ) const
		{
			//NC_ASSERT( idx < 3 );
			switch( idx )
			{
			case 0:	return d0_;
			case 1:	return d1_;
			case 2:	return d2_;
			case 3:	return d3_;
			}
			return d3_;
		}

		T& operator[]( uint32_t idx )
		{
			//NC_ASSERT( idx < 3 );
			switch( idx )
			{
			case 0:	return d0_;
			case 1:	return d1_;
			case 2:	return d2_;
			case 3:	return d3_;
			}
			return d3_;
		}
	};


}//namespace nc

#endif // __NC_SWIZZLE_h__