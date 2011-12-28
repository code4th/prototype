#if !defined(NC_THROW_EXCEPTION_H)
#define NC_THROW_EXCEPTION_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "NuclConfig.h"

namespace nc
{

#if defined( NC_NO_EXCEPTIONS )

	template<class E> void throwException(E const & e)
	{
	}

	//void throwException(std::exception const & e); // user defined

#else

	template<class E> void throwException(E const & e)
	{
		throw e;
	}

#endif


	//this is tr1 exception by std::weak_ptr
	class bad_weak_ptr: public std::exception
	{
	public:

		virtual char const * what() const throw()
		{
			return "tr1::bad_weak_ptr";
		}
	};

}

#endif