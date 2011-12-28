

#include "Trace.h"
#include "nucl/memory/MemoryAllocator.h"
#include "nucl/thread/Thread.h"

#include <memory>
#include <stdio.h>

#define PRINTF_BUFF_SIZE	(sizeof(char) * 256)

namespace nc{

	void trace( const char* str, ... )
	{
		static NC_THREAD_LOCAL nc::uint32_t trCount = 0;

		//debug 出力がちゃんと表示されるようにするためのミューテックス
		static nc::thread::Mutex	s_traceMtex;
		nc::thread::ScopedLock< nc::thread::Mutex > lock( s_traceMtex );

		va_list arglist;

		va_start(arglist, str);

		printf( "[TR],%x,%d,", nc::thread::getCurrentThreadID(), trCount++ );
		vprintf( str, arglist );
		//printf( "," NC_CURRENT_FUNCTION "(%d)\n", __LINE__ );
		printf( "\n" );
	}

	int printf( const char* str, ... )
	{
		// maybe slow.. but its ok for me now.
		//std::auto_ptr< char > buff( nc::memory::default_memory_allocator::malloc( PRINTF_BUFF_SIZE ) );

		//debug 出力がちゃんと表示されるようにするためのミューテックス
		static nc::thread::Mutex	s_printfMtex;
		nc::thread::ScopedLock< nc::thread::Mutex > lock( s_printfMtex );
			
		va_list arglist;

		va_start(arglist, str);

		//flockfile(stdout);

		vprintf( str, arglist );

		//funlockfile(stdout);

		fflush( stdout );

		return 0;
	}


} //namespace nc


