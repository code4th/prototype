

#ifndef __NC_ASSERT_h__
#define __NC_ASSERT_h__

#include "Trace.h"

#define NC_HALT()				if(1){ *((int*)0x00000000) = 0xFFFFFFFF; }

#define NC_ASSERT( exp )		if( !(exp) ){ NC_TRACE( "ASSERTION_FAILED, exp=" #exp ); NC_HALT(); }


#endif


