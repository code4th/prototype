
#ifndef __NC_STATICASSERT_h__
#define __NC_STATICASSERT_h__

namespace nc
{
	template< bool >
	class StaticAssert;

	template<>
	class StaticAssert<true>{};
	
	#define STATIC_ASSERT( exp )	StaticAssert< exp >()
}

#endif // __NC_STATICASSERT_h__