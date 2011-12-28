#if !defined(NC_CHECKED_DELETE_H)
#define NC_CHECKED_DELETE_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "NuclConfig.h"

namespace nc
{

	template<class T>
	inline void checkedDelete(T * x)
	{
		typedef char NonZeroTypeChk[ sizeof(T)? 1: -1 ];
		(void) sizeof(NonZeroTypeChk);
		delete x;
	}

	template<class T>
	inline void checkedArrayDelete(T * x)
	{
		typedef char NonZeroTypeChk[ sizeof(T)? 1: -1 ];
		(void) sizeof(NonZeroTypeChk);
		delete [] x;
	}

	template<class T>
	struct CheckedDeleter
	{
		typedef void Result_t;
		typedef T* Argument_t;

		void operator()(T* x) const
		{
			nc::checkedDelete(x);
		}
	};

	template<class T>
	struct CheckedArrayDeleter
	{
		typedef void Result_t;
		typedef T* Argument_t;

		void operator()(T* x) const
		{
			nc::checkedArrayDelete(x);
		}
	};

}//namespace nc

#endif