#ifndef __NC_ENDIAN_h__
#define __NC_ENDIAN_h__

#include "Types.h"

namespace nc{

	namespace detail{

		#define __BYTE_MASK( byte )	(0x000000FF << ((byte - 1) << 3))

		template< typename T, size_t SizeT >
		class EndianFlip_Impl{
		public://method
			static T flip( const T& val )
			{
				T tmp = 0;
				for( int n = 1; n <= SizeT; ++n )
				{
					tmp |= (val & __BYTE_MASK(n)) << (SizeT - n - 1);
				}
				return tmp;
			}
		};

		template< typename T >
		class EndianFlip_Impl< T, 0 >{
		public://method
			static T flip( const T& val )
			{
				return val;
			}
		};


		template< typename T >
		class EndianFlip_Impl< T, 1 >{
		public://method
			static T flip( const T& val )
			{
				return val;
			}
		};

		template< typename T >
		class EndianFlip_Impl< T, 2 >{
		public://method
			static T flip( const T& val )
			{
				return ((val & __BYTE_MASK(1)) << 8) | ((val & __BYTE_MASK(2)) >> 8);
			}
		};

		template< typename T >
		class EndianFlip_Impl< T, 4 >{
		public://method
			static T flip( const T& val )
			{
				return	((val & __BYTE_MASK(1)) << 24)	|
					((val & __BYTE_MASK(2)) << 8)	|
					((val & __BYTE_MASK(3)) >> 8)	|
					((val & __BYTE_MASK(4)) >> 24) ;
			}
		};

		template< typename T >
		class EndianFlip_Impl< T, 8 >{
		public://method
			static T flip( const T& val )
			{
				return	((val & __BYTE_MASK(1)) << 56)	|
					((val & __BYTE_MASK(2)) << 40)	|
					((val & __BYTE_MASK(3)) << 24)	|
					((val & __BYTE_MASK(4)) << 8)	|
					((val & __BYTE_MASK(5)) >> 8)	|
					((val & __BYTE_MASK(6)) >> 24) |
					((val & __BYTE_MASK(7)) >> 40) |
					((val & __BYTE_MASK(8)) >> 56) ;
			}
		};

		#undef __BYTE_MASK
	}

	//! 環境がLittleエンディアンの場合はtrueを返します。
	bool isLittleEndian() {
		//誰かコンパイル時に解決して！
		uint32_t i = 1;
		return (*((uint8_t*)&i)) ? true : false;
	};

	//! 環境がBigエンディアンの場合はtrueを返します。
	bool isBigEndian() {
		return !isLittleEndian();
	};

	//! 引数の値をエンディアン変換します。
	template< typename T >
	T flipEndian( const T& val )
	{
		return detail::EndianFlip_Impl< T, sizeof(T) >::flip( val );
	}

	//! 環境のエンディアンがLittleの場合
	//	引数の値をBigエンディアンに変換します。
	//	そうでない場合は変換しません
	template< typename T >
	T toBigEndian( const T& val )
	{
		if( IsLittleEndian() )
		{
			return flipEndian( val );
		}
		else
		{
			return val;
		}
	}

	//! 環境のエンディアンがBigの場合
	//	引数の値をLittleエンディアンに変換します。
	//	そうでない場合は変換しません
	template< typename T >
	T toLittleEndian( const T& val )
	{
		if( !IsLittleEndian() )
		{
			return flipEndian( val );
		}
		else
		{
			return val;
		}
	}


}//namespace nc

#endif // __NC_ENDIAN_h__