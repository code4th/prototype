

#ifndef __NC_TYPES_h__
#define __NC_TYPES_h__


//http://ja.wikipedia.org/wiki/%E6%A8%99%E6%BA%96C%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA#.E6.B5.AE.E5.8B.95.E5.B0.8F.E6.95.B0.E7.82.B9.E5.9E.8B.E3.81.AE.E7.89.B9.E6.80.A7_.3Cfloat.h.3E
/*

# FLT_RADIX - 浮動小数点型の内部表現に使用される基数
# FLT_ROUNDS - 浮動小数点型の丸め方向
# FLT_DIG - float 型の FLT_RADIX を基数とした指数部の桁数
# FLT_MANT_DIG - float 型の FLT_RADIX を基数とした仮数部の桁数
# FLT_MAX_EXP - float 型における FLT_RADIX を基数とした指数部の最大値
# FLT_MIN_EXP - float 型における FLT_RADIX を基数とした指数部の最小値
# FLT_MAX_10_EXP - float 型における10を基数とした指数部の最大値
# FLT_MIN_10_EXP - float 型における10を基数とした指数部の最小値
# FLT_MAX - float 型の最大値
# FLT_MIN - float 型の正の値の最小値（整数型とは違いfloat 型の実数の最小値は-FLT_MAXである）
# FLT_EPSILON - float 型で表現可能な1より大きい最小値と1との差

# DBL_RADIX - 浮動小数点型の内部表現に使用される基数
# DBL_ROUNDS - 浮動小数点型の丸め方向
# DBL_DIG - double 型の FLT_RADIX を基数とした指数部の桁数
# DBL_MANT_DIG - double 型の FLT_RADIX を基数とした仮数部の桁数
# DBL_MAX_EXP - double 型における FLT_RADIX を基数とした指数部の最大値
# DBL_MIN_EXP - double 型における FLT_RADIX を基数とした指数部の最小値
# DBL_MAX_10_EXP - double 型における10を基数とした指数部の最大値
# DBL_MIN_10_EXP - double 型における10を基数とした指数部の最小値
# DBL_MAX - double 型の最大値
# DBL_MIN - double 型の正の値の最小値（整数型とは違いdouble 型の実数の最小値は-DBL_MAXである）
# DBL_EPSILON - double 型で表現可能な1より大きい最小値と1との差

#define FLT_RADIX       2                       // exponent radix 
#define FLT_ROUNDS      1                       // addition rounding: near 
#define FLT_DIG         6                       // # of decimal digits of precision 
#define FLT_MANT_DIG    24                      // # of bits in mantissa 
#define FLT_MAX_EXP     128                     // max binary exponent 
#define FLT_MIN_EXP     (-125)                  // min binary exponent 
#define FLT_MAX_10_EXP  38                      // max decimal exponent 
#define FLT_MIN_10_EXP  (-37)                   // min decimal exponent 
#define FLT_MAX         3.402823466e+38F        // max value 
#define FLT_MIN         1.175494351e-38F        // min positive value 
#define FLT_EPSILON     1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0 

#define DBL_RADIX       2                       // exponent radix 
#define DBL_ROUNDS      1                       // addition rounding: near 
#define DBL_DIG         15                      // # of decimal digits of precision 
#define DBL_MANT_DIG    53                      // # of bits in mantissa 
#define DBL_MAX_EXP     1024                    // max binary exponent 
#define DBL_MIN_EXP     (-1021)                 // min binary exponent 
#define DBL_MAX_10_EXP  308                     // max decimal exponent 
#define DBL_MIN_10_EXP  (-307)                  // min decimal exponent 
#define DBL_MAX         1.7976931348623158e+308 // max value 
#define DBL_MIN         2.2250738585072014e-308 // min positive value 
#define DBL_EPSILON     2.2204460492503131e-016 // smallest such that 1.0+DBL_EPSILON != 1.0 

*/

#include <float.h>

#define REAL_RADIX			FLT_RADIX
#define REAL_ROUNDS			FLT_ROUNDS
#define REAL_DIG			FLT_DIG 
#define REAL_MANT_DIG		FLT_MANT_DIG
#define REAL_MAX_EXP		FLT_MAX_EXP
#define REAL_MIN_EXP		FLT_MIN_EXP
#define REAL_MAX_10_EXP		FLT_MAX_10_EXP
#define REAL_MIN_10_EXP		FLT_MIN_10_EXP
#define REAL_MAX			FLT_MAX
#define REAL_MIN			FLT_MIN
#define REAL_EPSILON		FLT_EPSILON


#define REAL32_RADIX		FLT_RADIX
#define REAL32_ROUNDS		FLT_ROUNDS
#define REAL32_MANT_DIG		FLT_MANT_DIG
#define REAL32_MAX_EXP		FLT_MAX_EXP
#define REAL32_MIN_EXP		FLT_MIN_EXP
#define REAL32_MAX_10_EXP	FLT_MAX_10_EXP
#define REAL32_MIN_10_EXP	FLT_MIN_10_EXP
#define REAL32_MAX			FLT_MAX
#define REAL32_MIN			FLT_MIN
#define REAL32_EPSILON		FLT_EPSILON

#define REAL64_RADIX		DBL_RADIX
#define REAL64_ROUNDS		DBL_ROUNDS
#define REAL64_MANT_DIG		DBL_MANT_DIG
#define REAL64_MAX_EXP		DBL_MAX_EXP
#define REAL64_MIN_EXP		DBL_MIN_EXP
#define REAL64_MAX_10_EXP	DBL_MAX_10_EXP
#define REAL64_MIN_10_EXP	DBL_MIN_10_EXP
#define REAL64_MAX			DBL_MAX
#define REAL64_MIN			DBL_MIN
#define REAL64_EPSILON		DBL_EPSILON

#define __STDC_LIMIT_MACROS

#include <cstddef>
#include <stddef.h>

#include "stdint.h"

namespace nc
{
	/*
	以下の型を使う際には、明確な理由を持つ事

	char
		文字型
		文字定数、文字列定数等の時に使用する。
		255までのカウンタ等の用途は不可とする。(u8使え)

	wchar_t
		ワイド文字型

	bool
		true / false
	*/

	//POD


	//pointer type omitted
	//typedef nc::platform::intptr_t	intptr_t;
	//typedef nc::platform::uintptr_t	uintptr_t;
	
	namespace detail{
		template< bool IsPtrSize32T >
		struct CheckPointerSize{
			typedef nc::uint32_t ptrsize_t;
		};

		template<>
		struct CheckPointerSize< false >{
			typedef nc::uint64_t ptrsize_t;
		};

	}

	typedef detail::CheckPointerSize< sizeof(int*) == 4 >::ptrsize_t ptrsize_t;

	//real type
	typedef float					real32_t;
	typedef double					real64_t;
	typedef real32_t				real_t;


	//UTF-8, UTF-16, UTF-32 の違い
	//http://d.hatena.ne.jp/tanamon/20090216/1234764425

	//UCS と UTF
	//http://homepage1.nifty.com/nomenclator/unicode/ucs_utf.htm

	typedef	uint8_t					char8_t;		//ASCII, UTF-8, etc
	typedef	uint16_t				char16_t;		//UTF-16( in C++0x )
	typedef	uint32_t				char32_t;		//UTF-32( ≒ UCS-4 | in C++0x )


	//UTF-8 is including the ASCII code.

	//UTF-16ではUCS-4にあってUCS-2にない文字のうち
	//[ U+10000 ]から[ U+10FFFF ]の範囲を
	//（Unicode consortiumのいうところの）サロゲートペアとして表します。
	//すなわち[ U+D800 ]から[ U+DBFF ]までの文字を上位サロゲート（high surrogate）とし
	//[ U+DC00 ]から[ U+DFFF ]までの文字を下位サロゲート（low surrogate）として
	//２つの組み合わせて表現します。
	//そのため，UTF-16では，UCS-4のU+110000以降の文字を表現することができません。
	//とは言え、それ以降の文字は今の所定義されていません。

	typedef char8_t					utf8_t;
	typedef char16_t				utf16_t;
	typedef char32_t				utf32_t;
	
	#undef max
	#undef min

	template< class T >
	inline const T& max( const T& lhs, const T& rhs )
	{
		return lhs < rhs ? lhs : rhs; 
	}

	template< typename T >
	inline const T& min( const T& lhs, const T& rhs )
	{
		return rhs < lhs ? lhs : rhs; 
	}


	//32bit
	inline static real32_t abs( real32_t val )
	{
		return val < 0 ? -val : val;
	}

	inline static bool isZero( real32_t val, real_t epsilon = REAL32_EPSILON )
	{
		return abs(val) <= epsilon * abs(val);
	}

	inline static bool isEqual( real32_t lhs, real32_t rhs, real_t epsilon = REAL32_EPSILON )
	{
		return abs(lhs - rhs) <= epsilon * max( abs(lhs), abs(rhs) );
	}

	//64bit
	inline static real64_t abs( real64_t val )
	{
		return val < 0 ? -val : val;
	}

	inline static bool isZero( real64_t val, real_t epsilon = REAL32_EPSILON )
	{
		return abs(val) <= epsilon * abs(val);
	}
	
	inline static bool isEqual( real64_t lhs, real64_t rhs, real_t epsilon = REAL32_EPSILON )
	{
		return abs(lhs - rhs) <= epsilon * max( abs(lhs), abs(rhs) );
	}

}


#endif // __NC_TYPES_h__
