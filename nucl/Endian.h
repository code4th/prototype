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

	//! ����Little�G���f�B�A���̏ꍇ��true��Ԃ��܂��B
	bool isLittleEndian() {
		//�N���R���p�C�����ɉ������āI
		uint32_t i = 1;
		return (*((uint8_t*)&i)) ? true : false;
	};

	//! ����Big�G���f�B�A���̏ꍇ��true��Ԃ��܂��B
	bool isBigEndian() {
		return !isLittleEndian();
	};

	//! �����̒l���G���f�B�A���ϊ����܂��B
	template< typename T >
	T flipEndian( const T& val )
	{
		return detail::EndianFlip_Impl< T, sizeof(T) >::flip( val );
	}

	//! ���̃G���f�B�A����Little�̏ꍇ
	//	�����̒l��Big�G���f�B�A���ɕϊ����܂��B
	//	�����łȂ��ꍇ�͕ϊ����܂���
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

	//! ���̃G���f�B�A����Big�̏ꍇ
	//	�����̒l��Little�G���f�B�A���ɕϊ����܂��B
	//	�����łȂ��ꍇ�͕ϊ����܂���
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