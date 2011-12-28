
#include "Random.h"

namespace nc{

	/*
	Xor-Shift bitパラメータ

	128 ビットXorshiftのパラメータ(11,8,19)
	論文中の他の組合せ(5,14,1),(15,4,21),(23,24,3),(5,12,29)
	他の組合せ(1,3,12),(1,3,15),(2,1,21),(2,21,6),(2,25,2)
	(3,2,21),(4,1,5),(6,5,17),(6,11,21),(6,11,25),(7,10,7),
	(7,11,19),(7,11,20),(7,12,11),(8,11,14),(9,11,6),(9,13,17)
	(9,24,1),(10,5,8),(10,11,12),(10,11,23),(11,5,12),(11,5,24)
	(11,5,26),(11,10,21),(11,16,1),(13,3,25),(14,3,23)
	(14,13,19)),(17,7,21),(18,13,19),(19,1,2),(20,5,17)
	(21,2,23),(21,9,4),(21,16,11),(22,3,12),(23,3,6),(25,3,10)
	(27,5,31),(27,19,5),(29,3,30)
	*/
	namespace {
		enum {
			XOR_SHIFT_A = 11,
			XOR_SHIFT_B = 8,
			XOR_SHIFT_C = 19
		};
	}

	Random::Random()
	{
		xor_seed[0] = 1812433254UL;
		xor_seed[1] = 3713160357UL;
		xor_seed[2] = 3109174145UL;
		xor_seed[3] = 64984499UL;
	}

	void Random::setSeed( uint32_t seed )
	{
		for(int i = 1; i <= 4; i++ )
		{
			xor_seed[ i - 1 ] = seed = 1812433253L * ( seed ^ ( seed >> 30 ) ) + i;
		}
	}

	nc::uint32_t Random::rand()
	{
		/*uint32_t t = xor_seed[0] ^ ( xor_seed[0] << XOR_SHIFT_A );

		xor_seed[0] = xor_seed[1];
		xor_seed[1] = xor_seed[2];
		xor_seed[2] = xor_seed[3];

		return xor_seed[3] ^= ( xor_seed[3] >> XOR_SHIFT_C ) ^ t ^ ( t >> XOR_SHIFT_B );
		*/

		//faster then above source. 
		//I dont know why.
		uint32_t t = xor_seed[0];
		uint32_t r = xor_seed[0];

		t <<= XOR_SHIFT_A;
		t ^= r;
		r = t;
		r >>= XOR_SHIFT_B;
		t ^= r;
		r = xor_seed[1];
		xor_seed[0] = r;
		r = xor_seed[2];
		xor_seed[1] = r;
		r = xor_seed[3];
		xor_seed[2] = r;
		t ^= r;
		r >>= XOR_SHIFT_C;
		r ^= t;
		xor_seed[3] = r;

		return r;
	}

	uint32_t Random::rand( uint32_t max )
	{
		return rand() % max;
	}

	uint32_t Random::rand( uint32_t min, uint32_t max )
	{
		NC_ASSERT( min <= max );
		return min + rand( max - min );
	}

	nc::uint32_t Random::randBack()
	{
		uint32_t u = xor_seed[3];
		uint32_t r = xor_seed[2];
		uint32_t t = xor_seed[3];

		xor_seed[3]=r;
		t ^= r;
		r >>= XOR_SHIFT_C;
		t ^= r;
		r = xor_seed[1];
		xor_seed[2] = r;
		r = xor_seed[0];
		xor_seed[1] = r;

		//inverse t^=t>>A 
		{
			uint32_t s = (( 1UL << XOR_SHIFT_B ) - 1) << (32 - XOR_SHIFT_B);
			uint32_t r;
			do{
				s >>= XOR_SHIFT_B;
				r = t;
				r >>= XOR_SHIFT_B;
				r &= s;
				t ^= r;
			}while(( s & 1UL ) == 0 );
		}

		//inverse t^=t<<A 
		{
			uint32_t s = ( 1UL << XOR_SHIFT_A ) - 1;
			uint32_t r;

			do{
				s <<= XOR_SHIFT_A;
				r = t;
				r <<= XOR_SHIFT_A;
				r &= s;
				t ^= r;
			}while(( s & 1UL << 31 ) == 0);
		}

		xor_seed[0]=t;
		return u;
	}

	nc::uint32_t Random::randBack( uint32_t max )
	{
		return randBack() % max;
	}

	nc::uint32_t Random::randBack( uint32_t min, uint32_t max )
	{
		NC_ASSERT( min <= max );
		return min + randBack( max - min );
	}

	real_t Random::frand()
	{
		return frand( 1.0f );
	}

	real_t Random::frand( real_t max )
	{
		return (rand() / real_t(RANDOM_MAX)) * max;
	}

	real_t Random::frand( real_t min, real_t max )
	{
		NC_ASSERT( min <= max );
		return min + frand( max - min );
	}

	real_t Random::frandBack()
	{
		return frandBack( 1.0f );
	}

	real_t Random::frandBack( real_t max )
	{
		return (randBack() / (real_t)RANDOM_MAX) * max;
	}

	real_t Random::frandBack( real_t min, real_t max )
	{
		NC_ASSERT( min <= max );
		return min + frandBack( max - min );
	}

}
