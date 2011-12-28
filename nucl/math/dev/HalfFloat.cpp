
//#include <neopch.h>

#include "HalfFloat.h"

namespace nc {
	namespace math {

		float HalfFloat::toFloat() const
		{
			union _uif
			{
				unsigned int i;
				float f;
			};

			unsigned short y = val_;
			_uif fl;
			int s = (y >> 15) & 0x00000001;
			int e = (y >> 10) & 0x0000001f;
			int m =  y        & 0x000003ff;

			if (e == 0)
			{
				if (m == 0)
				{
					//
					// Plus or minus zero
					//
					fl.i = s << 31;
					return fl.f;
				}
				else
				{
					//
					// Denormalized number -- renormalize it
					//
					while (!(m & 0x00000400))
					{
						m <<= 1;
						e -=  1;
					}

					e += 1;
					m &= ~0x00000400;
				}
			}
			else if (e == 31)
			{
				if (m == 0)
				{
					//
					// Positive or negative infinity
					//
					fl.i = (s << 31) | 0x7f800000;
					return fl.f;
				}
				else
				{
					//
					// Nan -- preserve sign and significand bits
					//
					fl.i = (s << 31) | 0x7f800000 | (m << 13);
					return fl.f;
				}
			}

			//
			// Normalized number
			//
			e = e + (127 - 15);
			m = m << 13;

			//
			// Assemble s, e and m.
			//
			fl.i = (s << 31) | (e << 23) | m;
			return fl.f;
		}


		//-----------------------------------------------
		// Overflow handler for float-to-HalfFloat conversion;
		// generates a hardware floating-point overflow,
		// which may be trapped by the operating system.
		//-----------------------------------------------
		float HalfFloat::overflow()
		{
			volatile float f = 1e10;

			for (int i = 0; i < 10; i++)	
				f *= f;				// this will overflow before
			// the forloop terminates
			return f;
		}


		//-----------------------------------------------------
		// Float-to-HalfFloat conversion -- general case, including
		// zeroes, denormalized numbers and exponent overflows.
		//-----------------------------------------------------

		short HalfFloat::convert( int i )
		{
			//
			// Our floating point number, f, is represented by the bit
			// pattern in integer i.  Disassemble that bit pattern into
			// the sign, s, the exponent, e, and the significand, m.
			// Shift s into the position where it will go in in the
			// resulting HalfFloat number.
			// Adjust e, accounting for the different exponent bias
			// of float and HalfFloat (127 versus 15).
			//
			register int s =  (i >> 16) & 0x00008000;
			register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
			register int m =   i        & 0x007fffff;

			//
			// Now reassemble s, e and m into a HalfFloat:
			//

			if (e <= 0)
			{
				if (e < -10)
				{
					//
					// E is less than -10.  The absolute value of f is
					// less than HALF_MIN (f may be a small normalized
					// float, a denormalized float or a zero).
					//
					// We convert f to a HalfFloat zero.
					//

					return 0;
				}

				//
				// E is between -10 and 0.  F is a normalized float,
				// whose magnitude is less than HALF_NRM_MIN.
				//
				// We convert f to a denormalized HalfFloat.
				// 

				m = (m | 0x00800000) >> (1 - e);

				//
				// Round to nearest, round "0.5" up.
				//
				// Rounding may cause the significand to overflow and make
				// our number normalized.  Because of the way a HalfFloat's bits
				// are laid out, we don't have to treat this case separately;
				// the code below will handle it correctly.
				// 

				if (m &  0x00001000)
					m += 0x00002000;

				//
				// Assemble the HalfFloat from s, e (zero) and m.
				//

				return s | (m >> 13);
			}
			else if (e == 0xff - (127 - 15))
			{
				if (m == 0)
				{
					//
					// F is an infinity; convert f to a HalfFloat
					// infinity with the same sign as f.
					//

					return s | 0x7c00;
				}
				else
				{
					//
					// F is a NAN; we produce a HalfFloat NAN that preserves
					// the sign bit and the 10 leftmost bits of the
					// significand of f, with one exception: If the 10
					// leftmost bits are all zero, the NAN would turn 
					// into an infinity, so we have to set at least one
					// bit in the significand.
					//

					m >>= 13;
					return s | 0x7c00 | m | (m == 0);
				}
			}
			else
			{
				//
				// E is greater than zero.  F is a normalized float.
				// We try to convert f to a normalized HalfFloat.
				//

				//
				// Round to nearest, round "0.5" up
				//

				if (m &  0x00001000)
				{
					m += 0x00002000;

					if (m & 0x00800000)
					{
						m =  0;		// overflow in significand,
						e += 1;		// adjust exponent
					}
				}

				//
				// Handle exponent overflow
				//

				if (e > 30)
				{
					overflow ();	// Cause a hardware floating point overflow;
					return s | 0x7c00;	// if this returns, the HalfFloat becomes an
				}   			// infinity with the same sign as f.

				//
				// Assemble the HalfFloat from s, e and m.
				//

				return s | (e << 10) | (m >> 13);
			}
		}

	}
}