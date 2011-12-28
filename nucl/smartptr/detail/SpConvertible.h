#if !defined(NC_SP_CONVERTIBLE_H)
#define NC_SP_CONVERTIBLE_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"

namespace nc
{
	namespace detail
	{
		template< class Y, class T > struct SpConvertible
		{
			typedef char (&yes) [1];
			typedef char (&no)  [2];

			static yes f( T* );
			static no  f( ... );

			enum _vt { value = sizeof( f( (Y*)0 ) ) == sizeof(yes) };
		};

		struct SpEmpty
		{
		};

		template< bool > struct SpEnableIfConvertibleImpl;

		template<> struct SpEnableIfConvertibleImpl<true>
		{
			typedef SpEmpty type;
		};

		template<> struct SpEnableIfConvertibleImpl<false>
		{
		};

		template< class Y, class T > struct SpEnableIfConvertible: public SpEnableIfConvertibleImpl< SpConvertible< Y, T >::value >
		{
		};

	}
}

#endif