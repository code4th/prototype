#if !defined(NC_LEXICAL_CAST_H)
#define NC_LEXICAL_CAST_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.

LexicalCast
*/

#include <string>
#include <sstream>

namespace nc
{

	/*
		nc::u8_t a = LexicalCast< int >( "123456789" );
	*/

	template< class T >
	class LexicalCast{

		std::istringstream iss_;

	public:
		LexicalCast( const std::string& str )
		: iss_( str )
		{}

		operator T()
		{
			T target;
			iss_ >> target;
			return target;
		}
	};

}//namespace nc;
#endif