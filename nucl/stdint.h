
/**
 * boostのcstdint.hppを参考に
 */

#ifndef __NC_STDINT_h__
#define __NC_STDINT_h__

//
// Since we always define the INT#_C macros as per C++0x, 
// define __STDC_CONSTANT_MACROS so that <stdint.h> does the right
// thing if possible, and so that the user knows that the macros 
// are actually defined as per C99.
//
#ifndef __STDC_CONSTANT_MACROS
	#define __STDC_CONSTANT_MACROS
#endif


#if ( defined( _MSC_VER ) && ( _MSC_VER >= 1600 )) || \
	( defined( __GNUC__ ) && (__GNUC__ >= 4) ) || \
	( defined( __SNC__ ) )

	#define NC_HAS_STDINT_H
#endif

// stdint.hが利用出来る環境なら、それを利用する。
#if defined(NC_HAS_STDINT_H)
	#include <stdint.h>
	// There is a bug in Cygwin two _C macros
	#if ( !defined(__cplusplus ) || defined(__STDC_CONSTANT_MACROS) ) && defined(__CYGWIN__)
		#undef INTMAX_C
		#undef UINTMAX_C
		#define INTMAX_C(c) c##LL
		#define UINTMAX_C(c) c##ULL
	#endif

	//stdint.hの定義を受け継ぐ
	namespace nc
	{
		using ::int8_t;
		using ::int_least8_t;       
		using ::int_fast8_t;        
		using ::uint8_t;            
		using ::uint_least8_t;      
		using ::uint_fast8_t;       

		using ::int16_t;            
		using ::int_least16_t;      
		using ::int_fast16_t;       
		using ::uint16_t;           
		using ::uint_least16_t;     
		using ::uint_fast16_t;      

		using ::int32_t;            
		using ::int_least32_t;      
		using ::int_fast32_t;       
		using ::uint32_t;           
		using ::uint_least32_t;     
		using ::uint_fast32_t;

		using ::int64_t;            
		using ::int_least64_t;      
		using ::int_fast64_t;       
		using ::uint64_t;           
		using ::uint_least64_t;     
		using ::uint_fast64_t;      

		using ::intmax_t;      
		using ::uintmax_t;     
		
	} // namespace nc


#else  // BOOST_HAS_STDINT_H

	//# include <nc/limits.hpp> // implementation artifact; not part of interface
	# include <limits.h>         // needed for limits macros


	namespace nc
	{

		//  These are fairly safe guesses for some 16-bit, and most 32-bit and 64-bit
		//  platforms.  For other systems, they will have to be hand tailored.
		//
		//  Because the fast types are assumed to be the same as the undecorated types,
		//  it may be possible to hand tailor a more efficient implementation.  Such
		//  an optimization may be illusionary; on the Intel x86-family 386 on, for
		//  example, byte arithmetic and load/stores are as fast as "int" sized ones.

		//  8-bit types  ------------------------------------------------------------//

	# if UCHAR_MAX == 0xff
		typedef signed char     int8_t;
		typedef signed char     int_least8_t;
		typedef signed char     int_fast8_t;
		typedef unsigned char   uint8_t;
		typedef unsigned char   uint_least8_t;
		typedef unsigned char   uint_fast8_t;
	#else
		#error defaults not correct; you must hand modify nc/cstdint.h
	# endif

		//  16-bit types  -----------------------------------------------------------//

	#if USHRT_MAX == 0xffff
		typedef short           int16_t;
		typedef short           int_least16_t;
		typedef short           int_fast16_t;
		typedef unsigned short  uint16_t;
		typedef unsigned short  uint_least16_t;
		typedef unsigned short  uint_fast16_t;
	#else
		#error defaults not correct; you must hand modify nc/cstdint.h
	#endif

		//  32-bit types  -----------------------------------------------------------//

	#if UINT_MAX == 0xffffffff
		typedef int             int32_t;
		typedef int             int_least32_t;
		typedef int             int_fast32_t;
		typedef unsigned int    uint32_t;
		typedef unsigned int    uint_least32_t;
		typedef unsigned int    uint_fast32_t;
	#elif (USHRT_MAX == 0xffffffff)
		typedef short             int32_t;
		typedef short             int_least32_t;
		typedef short             int_fast32_t;
		typedef unsigned short    uint32_t;
		typedef unsigned short    uint_least32_t;
		typedef unsigned short    uint_fast32_t;
	#elif ULONG_MAX == 0xffffffff
		typedef long            int32_t;
		typedef long            int_least32_t;
		typedef long            int_fast32_t;
		typedef unsigned long   uint32_t;
		typedef unsigned long   uint_least32_t;
		typedef unsigned long   uint_fast32_t;
	#else
		#error defaults not correct; you must hand modify nc/cstdint.h
	# endif

		//  64-bit types + intmax_t and uintmax_t  ----------------------------------//

	#if ULONG_MAX != 0xffffffff

		#if ULONG_MAX == 18446744073709551615 // 2**64 - 1
			typedef long                 intmax_t;
			typedef unsigned long        uintmax_t;
			typedef long                 int64_t;
			typedef long                 int_least64_t;
			typedef long                 int_fast64_t;
			typedef unsigned long        uint64_t;
			typedef unsigned long        uint_least64_t;
			typedef unsigned long        uint_fast64_t;
		#else
			#error defaults not correct; you must hand modify nc/cstdint.h
		#endif

	#elif defined(__GNUC__)

		__extension__ typedef long long            intmax_t;
		__extension__ typedef unsigned long long   uintmax_t;
		__extension__ typedef long long            int64_t;
		__extension__ typedef long long            int_least64_t;
		__extension__ typedef long long            int_fast64_t;
		__extension__ typedef unsigned long long   uint64_t;
		__extension__ typedef unsigned long long   uint_least64_t;
		__extension__ typedef unsigned long long   uint_fast64_t;

	#elif defined(_MSC_VER) && (_MSC_VER >= 1400)

		//
		// we have Intel/Microsoft __int64:
		//
		typedef __int64             intmax_t;
		typedef unsigned __int64    uintmax_t;
		typedef __int64             int64_t;
		typedef __int64             int_least64_t;
		typedef __int64             int_fast64_t;
		typedef unsigned __int64    uint64_t;
		typedef unsigned __int64    uint_least64_t;
		typedef unsigned __int64    uint_fast64_t;

	# endif

	} // namespace nc


#endif // BOOST_HAS_STDINT_H


/****************************************************

Macro definition section:

Added 23rd September 2000 (John Maddock).
Modified 11th September 2001 to be excluded when
BOOST_HAS_STDINT_H is defined (John Maddock).
Modified 11th Dec 2009 to always define the
INT#_C macros if they're not already defined (John Maddock).

******************************************************/

#if	(!defined(INT8_C) || !defined(INT16_C) || !defined(INT32_C) || !defined(INT64_C))

	//
	// For the following code we get several warnings along the lines of: 
	// 
	// nc/cstdint.h:428:35: error: use of C99 long long integer constant 
	// 
	// So we declare this a system header to suppress these warnings. 
	//
	#if defined(__GNUC__) && (__GNUC__ >= 4) 
		#pragma GCC system_header 
	#endif 

	#include <limits.h>

	//VisualStudio _MSC_VER
	//GCC __GNUC__

	//
	// #define NC_CONF_STDC_CONSTANT_MACROS_HAS_SUFFIX
	//
	// Intel/Microsoft compilers have width specific suffixes:
	//
	#if ( defined( _MSC_VER) && (_MSC_VER >= 1400) ) || \
		( defined(__GNUC__) && (__GNUC__ >= 4) )

		#define NC_CONF_STDC_CONSTANT_MACROS_HAS_SUFFIX

	#endif


	# if defined( NC_CONF_STDC_CONSTANT_MACROS_HAS_SUFFIX )
		// サフィックスが利用出来るなら
		//
		#ifndef INT8_C
		#  define INT8_C(value)     value##i8
		#endif
		#ifndef INT16_C
		#  define INT16_C(value)    value##i16
		#endif
		#ifndef INT32_C
		#  define INT32_C(value)    value##i32
		#endif
		#ifndef INT64_C
		#  define INT64_C(value)    value##i64
		#endif
		#ifndef UINT8_C
		#	define UINT8_C(value)	value##ui8
		#endif
		#ifndef UINT16_C
		#  define UINT16_C(value)   value##ui16
		#endif
		#ifndef UINT32_C
		#  define UINT32_C(value)   value##ui32
		#endif
		#ifndef UINT64_C
		#  define UINT64_C(value)   value##ui64
		#endif
		#ifndef INTMAX_C
		#  define INTMAX_C(value)   value##i64
		#  define UINTMAX_C(value)  value##ui64
		#endif

	# else
		//  do it the old fashioned way:
		// サフィックスが利用できないので、キャストで対応

		//  8-bit types  ------------------------------------------------------------//

		#if (UCHAR_MAX == 0xff) && !defined(INT8_C)
			#define INT8_C(value) static_cast<nc::int8_t>(value)
			#define UINT8_C(value) static_cast<nc::uint8_t>(value##u)
		#endif

		//  16-bit types  -----------------------------------------------------------//

		#if (USHRT_MAX == 0xffff) && !defined(INT16_C)
			#define INT16_C(value) static_cast<nc::int16_t>(value)
			#define UINT16_C(value) static_cast<nc::uint16_t>(value##u)
		#endif

		//  32-bit types  -----------------------------------------------------------//
		#ifndef INT32_C
			#if (UINT_MAX == 0xffffffff)
				#define INT32_C(value) value
				#define UINT32_C(value) value##u
			#elif ULONG_MAX == 0xffffffff
				#define INT32_C(value) value##L
				#define UINT32_C(value) value##uL
			#endif
		#endif

		//  64-bit types + intmax_t and uintmax_t  ----------------------------------//
		#ifndef INT64_C
			#if (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX) || defined(_LLONG_MAX))

				#if	(defined(ULLONG_MAX)		&& ULLONG_MAX		== 18446744073709551615ULL) ||  \
					(defined(ULONG_LONG_MAX)	&& ULONG_LONG_MAX	== 18446744073709551615ULL) ||  \
					(defined(ULONGLONG_MAX)		&& ULONGLONG_MAX	== 18446744073709551615ULL) || \
					(defined(_LLONG_MAX)		&& _LLONG_MAX		== 18446744073709551615ULL)

					#define INT64_C(value) value##LL
					#define UINT64_C(value) value##uLL
				#else
					#error defaults not correct; you must hand modify nc/cstdint.h
				#endif

			#elif ULONG_MAX != 0xffffffff

				#if ULONG_MAX == 18446744073709551615U // 2**64 - 1
					#define INT64_C(value) value##L
					#define UINT64_C(value) value##uL
				#else
					#error defaults not correct; you must hand modify nc/cstdint.h
				#endif

			#else
		
				#error defaults not correct; you must hand modify nc/cstdint.h

			#endif

			#define INTMAX_C(value) INT64_C(value)
			#define UINTMAX_C(value) UINT64_C(value)
	
		#endif

	#endif // Borland/Microsoft specific width suffixes

#endif // INT#_C macros.


#endif // __NC_STDINT_h__
