#ifndef __NC_HASH_h__
#define __NC_HASH_h__

// hash
namespace nc {
	
	template< typename T, typename StepT = T >
	struct Hash{
		size_t operator()( const T* val )
		{
			size_t c, result = 2166136261U;  // FNV1 hash. Perhaps the best string hash.
			while((c = (StepT)*val++) != 0)  // Using '!=' disables compiler warnings.
				result = (result * 16777619) ^ c;
			return (size_t)result;	
		}
	};


	// Hash< char >


}//namespace nc

#endif // __NC_HASH_h__