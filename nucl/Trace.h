
#ifndef __NC_TRACE_h__
#define __NC_TRACE_h__


#ifdef NC_PLATFORM_USE_INTRINSIC_TRACE

	//各プラットフォーム毎の実装
	#include "PLT_Trace.h"

#else

	#include <stdio.h>
	#include "CurrentFunction.h"

	#define NC_PRINTF(...)		nc::printf( __VA_ARGS__ )	

	//#define NC_TRACE(...)		
	#define NC_TRACE(...)		nc::trace( __VA_ARGS__ )

	#define NC_TRACE_BEGIN		do{ NC_PRINTF( "[TR]," ); 
	#define NC_TRACE_IN(...)	NC_PRINTF( __VA_ARGS__ );
	#define NC_TRACE_END		NC_PRINTF( "," __FILE__ "(%d)\n", __LINE__ ); }while(false);

namespace nc{
	void trace( const char* str, ... );
	int printf( const char* str, ... );
}//namespace nc

#endif

#endif // __NC_TRACE_h__