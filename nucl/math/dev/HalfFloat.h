
#ifndef __NC_HALFFLOAT_h__
#define __NC_HALFFLOAT_h__




/*! \file math/half.h
    16-bit floating point numbers */

//#include "../core/platform.h"
#include "nucl/Types.h"



//---------------------------------------------------------------------------
//
// Implementation --
//
// Representation of a float:
//
//	We assume that a float, f, is an IEEE 754 single-precision
//	floating point number, whose bits are arranged as follows:
//
//	    31 (msb)
//	    | 
//	    | 30     23
//	    | |      | 
//	    | |      | 22                    0 (lsb)
//	    | |      | |                     |
//	    X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
//
//	    s e        m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 254, f is a normalized number:
//
//	            s    e-127
//	    f = (-1)  * 2      * 1.m
//
//	If e is 0, and m is not zero, f is a denormalized number:
//
//	            s    -126
//	    f = (-1)  * 2      * 0.m
//
//	If e and m are both zero, f is zero:
//
//	    f = 0.0
//
//	If e is 255, f is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000000 00000000000000000000000 = 0.0
//	    0 01111110 00000000000000000000000 = 0.5
//	    0 01111111 00000000000000000000000 = 1.0
//	    0 10000000 00000000000000000000000 = 2.0
//	    0 10000000 10000000000000000000000 = 3.0
//	    1 10000101 11110000010000000000000 = -124.0625
//	    0 11111111 00000000000000000000000 = +infinity
//	    1 11111111 00000000000000000000000 = -infinity
//	    0 11111111 10000000000000000000000 = NAN
//	    1 11111111 11111111111111111111111 = NAN
//
// Representation of a HalfFloat:
//
//	Here is the bit-layout for a HalfFloat number, h:
//
//	    15 (msb)
//	    | 
//	    | 14  10
//	    | |   |
//	    | |   | 9        0 (lsb)
//	    | |   | |        |
//	    X XXXXX XXXXXXXXXX
//
//	    s e     m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 30, h is a normalized number:
//
//	            s    e-15
//	    h = (-1)  * 2     * 1.m
//
//	If e is 0, and m is not zero, h is a denormalized number:
//
//	            S    -14
//	    h = (-1)  * 2     * 0.m
//
//	If e and m are both zero, h is zero:
//
//	    h = 0.0
//
//	If e is 31, h is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000 0000000000 = 0.0
//	    0 01110 0000000000 = 0.5
//	    0 01111 0000000000 = 1.0
//	    0 10000 0000000000 = 2.0
//	    0 10000 1000000000 = 3.0
//	    1 10101 1111000001 = -124.0625
//	    0 11111 0000000000 = +infinity
//	    1 11111 0000000000 = -infinity
//	    0 11111 1000000000 = NAN
//	    1 11111 1111111111 = NAN
//
// Conversion:
//
//	Converting from a float to a HalfFloat requires some non-trivial bit
//	manipulations.  In some cases, this makes conversion relatively
//	slow, but the most common case is accelerated via table lookups.
//
//	Converting back from a HalfFloat to a float is easier because we don't
//	have to do any rounding.  In addition, there are only 65536
//	different HalfFloat numbers; we can convert each of those numbers once
//	and store the results in a table.  Later, all conversions can be
//	done using only simple table lookups.
//
//---------------------------------------------------------------------------



#define HALF_MIN		5.96046448e-08f	// Smallest positive HalfFloat
#define HALF_NRM_MIN	6.10351562e-05f	// Smallest positive normalized HalfFloat
#define HALF_MAX		65504.0f	// Largest positive HalfFloat
#define HALF_EPSILON	0.00097656f	// Smallest positive e for which
// HalfFloat (1.0 + e) != HalfFloat (1.0)

#define HALF_MANT_DIG	11		// Number of digits in mantissa
// (significand + hidden leading 1)

#define HALF_DIG		2		// Number of base 10 digits that
// can be represented without change

#define HALF_RADIX		2		// Base of the exponent

#define HALF_MIN_EXP	-13		// Minimum negative integer such that
// HALF_RADIX raised to the power of
// one less than that integer is a
// normalized HalfFloat

#define HALF_MAX_EXP	16		// Maximum positive integer such that
// HALF_RADIX raised to the power of
// one less than that integer is a
// normalized HalfFloat

#define HALF_MIN_10_EXP	-4		// Minimum positive integer such
// that 10 raised to that power is
// a normalized HalfFloat

#define HALF_MAX_10_EXP	4		// Maximum positive integer such
// that 10 raised to that power is
// a normalized HalfFloat

namespace nc {
	namespace math {

		/*! \brief 16-bit floating point number

			Type HalfFloat can represent positive and negative numbers, whose
			magnitude is between roughly 6.1e-5 and 6.5e+4, with a relative
			error of 9.8e-4; numbers smaller than 6.1e-5 can be represented
			with an absolute error of 6.0e-8.  All integers from -2048 to
			+2048 can be represented exactly.

			Type HalfFloat behaves (almost) like the built-in C++ floating point
			types.  In arithmetic expressions, HalfFloat, float and double can be
			mixed freely.  Here are a few examples:

				HalfFloat a (3.5);
				float b (a + sqrt (a));
				a += b;
				b += a;
				b = a + 7;

			Conversions from HalfFloat to float are lossless; all HalfFloat numbers
			are exactly representable as floats.

			Conversions from float to HalfFloat may not preserve the float's
			value exactly.  If a float is not representable as a HalfFloat, the
			float value is rounded to the nearest representable HalfFloat.  If
			a float value is exactly in the middle between the two closest
			representable HalfFloat values, then the float value is rounded to
			the HalfFloat with the greater magnitude.

			Overflows during float-to-HalfFloat conversions cause arithmetic
			exceptions.  An overflow occurs when the float value to be
			converted is too large to be represented as a HalfFloat, or if the
			float value is an infinity or a NAN.

			The implementation of type HalfFloat makes the following assumptions
			about the implementation of the built-in C++ types:

				float is an IEEE 754 single-precision number
				sizeof (float) == 4
				sizeof (unsigned int) == sizeof (float)
				alignof (unsigned int) == alignof (float)
				sizeof (unsigned short) == 2 */


		class HalfFloat	{
		public:

			//! Construct uninitialized number
			inline HalfFloat();

			//! Construct from 32-bit floating point number
			/*! \param val                                         Value */
			inline HalfFloat( float val );

			inline HalfFloat( const HalfFloat& rhs )
			{
				val_ = rhs.val_;
			}

			//! Convert to 32-bit floating point number
			inline operator	float () const;

			inline HalfFloat	operator - () const;
/*
			inline float	operator + ( HalfFloat h ) const;
			inline float	operator - ( HalfFloat h ) const;
			inline float	operator * ( HalfFloat h ) const;
			inline float	operator / ( HalfFloat h ) const;
*/
			inline HalfFloat&	operator = ( HalfFloat h );
			inline HalfFloat&	operator += ( HalfFloat h );
			inline HalfFloat&	operator -= ( HalfFloat h );
			inline HalfFloat&	operator *= ( HalfFloat h );
			inline HalfFloat&	operator /= ( HalfFloat h );
			/*
			bool operator == ( HalfFloat h ) const
			{
				return nc::isEqual( toFloat(), h.toFloat() );
			}

			bool operator != ( HalfFloat h ) const
			{
				return !( *this == h );
			}
			*/

			//---------------------------------------------------------
			// Round to n-bit precision (n should be between 0 and 10).
			// After rounding, the significand's 10-n least significant
			// bits will be zero.
			//---------------------------------------------------------
			inline HalfFloat	round( unsigned int n ) const;

			//--------------------------------------------------------------------
			// Classification:
			//
			//	h.isFinite()		returns true if h is a normalized number,
			//				a denormalized number or zero
			//
			//	h.isNormalized()	returns true if h is a normalized number
			//
			//	h.isDenormalized()	returns true if h is a denormalized number
			//
			//	h.isZero()		returns true if h is zero
			//
			//	h.isNan()		returns true if h is a NAN
			//
			//	h.isInfinity()		returns true if h is a positive
			//				or a negative infinity
			//
			//	h.isNegative()		returns true if the sign bit of h
			//				is set (negative)
			//--------------------------------------------------------------------
			inline bool isFinite() const;
			inline bool isNormalized() const;
			inline bool isDenormalized() const;
			inline bool isZero() const;
			inline bool isNaN() const;
			inline bool isInfinity() const;
			inline bool isNegative() const;

			//--------------------------------------------
			// Special values
			//
			//	posInf()	returns +infinity
			//
			//	negInf()	returns +infinity
			//
			//	qNan()		returns a NAN with the bit
			//			pattern 0111111111111111
			//
			//	sNan()		returns a NAN with the bit
			//			pattern 0111110111111111
			//--------------------------------------------
			static inline HalfFloat posInf();
			static inline HalfFloat negInf();
			static inline HalfFloat qNaN();
			static inline HalfFloat sNaN();

			//--------------------------------------
			// Access to the internal representation
			//--------------------------------------
			inline uint16_t bits() const;
			inline void setBits( uint16_t bits );

			float                                                  toFloat() const;

		protected:

			static short                                           convert( int i );
			static float                                           overflow ();

			unsigned short                                         val_;
		};


		class HalfFloatConst : public HalfFloat{
		public:
			HalfFloatConst( nc::uint16_t val )
			{
				val_ = val;
			}
		};

		const HalfFloatConst HalfFloatPInf(0x7c00);
		const HalfFloatConst HalfFloatNInf(0xfc00);
		const HalfFloatConst HalfFloatQNaN(0x7fff);
		const HalfFloatConst HalfFloatSNaN(0x7dff);
		const HalfFloatConst HalfFloatIND(0xfe00);
		
		inline HalfFloat::HalfFloat()
		{
		}


		inline HalfFloat::HalfFloat( float f )
		{
			union _uif
			{
				unsigned int i;
				float f;
			};

			static const unsigned short _eLut[1 << 9] =
			{
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,  1024,  2048,  3072,  4096,  5120,  6144,  7168, 
				8192,  9216, 10240, 11264, 12288, 13312, 14336, 15360, 
				16384, 17408, 18432, 19456, 20480, 21504, 22528, 23552, 
				24576, 25600, 26624, 27648, 28672, 29696,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0, 33792, 34816, 35840, 36864, 37888, 38912, 39936, 
				40960, 41984, 43008, 44032, 45056, 46080, 47104, 48128, 
				49152, 50176, 51200, 52224, 53248, 54272, 55296, 56320, 
				57344, 58368, 59392, 60416, 61440, 62464,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
				0,     0,     0,     0,     0,     0,     0,     0, 
			};

			if (f == 0)
			//if( nc::isZero( f ) )
			{
				//
				// Common special case - zero.
				// For speed, we don't preserve the zero's sign.
				//
				val_ = 0;
			}
			else
			{
				//
				// We extract the combined sign and exponent, e, from our
				// floating-point number, f.  Then we convert e to the sign
				// and exponent of the HalfFloat number via a table lookup.
				//
				// For the most common case, where a normalized HalfFloat is produced,
				// the table lookup returns a non-zero value; in this case, all
				// we have to do, is round f's significand to 10 bits and combine
				// the result with e.
				//
				// For all other cases (overflow, zeroes, denormalized numbers
				// resulting from underflow, infinities and NANs), the table
				// lookup returns zero, and we call a longer, non-inline function
				// to do the float-to-HalfFloat conversion.
				//
				_uif x;

				x.f = f;

				register int e = (x.i >> 23) & 0x000001ff;

				e = _eLut[e];

				if (e)
				{
					//
					// Simple case - round the significand and
					// combine it with the sign and exponent.
					//
					val_ = e + (((x.i & 0x007fffff) + 0x00001000) >> 13);
				}
				else
				{
					//
					// Difficult case - call a function.
					//
					val_ = convert (x.i);
				}
			}
		}


		HalfFloat::operator float () const
		{
			return toFloat();
		}


		HalfFloat HalfFloat::round( unsigned int n ) const
		{
			//
			// Parameter check.
			//
			if( n >= 10 )
				return *this;

			//
			// Disassemble h into the sign, s,
			// and the combined exponent and significand, e.
			//
			unsigned short s = val_ & 0x8000;
			unsigned short e = val_ & 0x7fff;

			//
			// Round the exponent and significand to the nearest value
			// where ones occur only in the (10-n) most significant bits.
			// Note that the exponent adjusts automatically if rounding
			// up causes the significand to overflow.
			//
			e >>= 9 - n;
			e  += e & 1;
			e <<= 9 - n;

			//
			// Check for exponent overflow.
			//
			if (e >= 0x7c00)
			{
				//
				// Overflow occurred -- truncate instead of rounding.
				//
				e = val_;
				e >>= 10 - n;
				e <<= 10 - n;
			}

			//
			// Put the original sign bit back.
			//
			HalfFloat h;
			h.val_ = s | e;

			return h;
		}


		HalfFloat HalfFloat::operator - () const
		{
			HalfFloat h;
			h.val_ = val_ ^ 0x8000;
			return h;
		}


		HalfFloat& HalfFloat::operator = ( HalfFloat h )
		{
			val_ = h.val_;
			return *this;
		}
		/*
		float HalfFloat::operator + ( HalfFloat h ) const
		{
			return toFloat() + h.toFloat();
		}

		float HalfFloat::operator - ( HalfFloat h ) const
		{
			return toFloat() - h.toFloat();
		}

		float HalfFloat::operator * ( HalfFloat h ) const
		{
			return toFloat() * h.toFloat();
		}

		float HalfFloat::operator / ( HalfFloat h ) const
		{
			return toFloat() / h.toFloat();
		}
		*/



		HalfFloat& HalfFloat::operator += ( HalfFloat h )
		{
			*this = *this + h;
			return *this;
		}


		HalfFloat& HalfFloat::operator -= ( HalfFloat h )
		{
			*this = *this - h;
			return *this;
		}

		HalfFloat& HalfFloat::operator *= ( HalfFloat h )
		{
			*this = *this * h;
			return *this;
		}
		
		HalfFloat& HalfFloat::operator /= ( HalfFloat h )
		{
			*this = *this / h;
			return *this;
		}
		

		bool HalfFloat::isFinite() const
		{
			unsigned short e = (val_ >> 10) & 0x001f;
			return e < 31;
		}


		bool HalfFloat::isNormalized() const
		{
			unsigned short e = (val_ >> 10) & 0x001f;
			return e > 0 && e < 31;
		}


		bool HalfFloat::isDenormalized() const
		{
			unsigned short e = (val_ >> 10) & 0x001f;
			unsigned short m =  val_ & 0x3ff;
			return e == 0 && m != 0;
		}


		bool HalfFloat::isZero() const
		{
			return (val_ & 0x7fff) == 0;
		}


		bool HalfFloat::isNaN() const
		{
			unsigned short e = (val_ >> 10) & 0x001f;
			unsigned short m =  val_ & 0x3ff;
			return e == 31 && m != 0;
		}


		bool HalfFloat::isInfinity() const
		{
			unsigned short e = (val_ >> 10) & 0x001f;
			unsigned short m =  val_ & 0x3ff;
			return e == 31 && m == 0;
		}


		bool HalfFloat::isNegative() const
		{
			return (val_ & 0x8000) != 0;
		}


		HalfFloat HalfFloat::posInf()
		{
			HalfFloat h;
			h.val_ = 0x7c00;
			return h;
		}


		HalfFloat HalfFloat::negInf ()
		{
			HalfFloat h;
			h.val_ = 0xfc00;
			return h;
		}
		
		HalfFloat HalfFloat::qNaN()
		{
			HalfFloat h;
			h.val_ = 0x7fff;
			return h;
		}


		HalfFloat HalfFloat::sNaN()
		{
			HalfFloat h;
			h.val_ = 0x7dff;
			return h;
		}


		uint16_t HalfFloat::bits() const
		{
			return val_;
		}


		void HalfFloat::setBits( unsigned short bits )
		{
			val_ = bits;
		}


		typedef HalfFloat float16_t;

	}
}

#endif // __NC_HALFFLOAT_h__
