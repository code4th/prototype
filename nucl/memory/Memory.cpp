
#include "Memory.h"

#include "nucl/thread/Thread.h"
#include "nucl/external/tlsf-1.9/tlsf.h"

namespace nc
{

	namespace memory
	{
#if 0
		namespace
		{
			static tlsf_pool			g_tlsfPool = 0;
			static nc::thread::Mutex	g_globalHeapMutex;
		}



		void initializeMemoryAllocator( void* memResource, size_t memSize )
		{
			g_tlsfPool = tlsf_create( memResource, memSize );
		}

		void cleanupMemoryAllocator()
		{

		}


		void allocSystemHeap()
		{
			//tlsf_pool	
		}

		void allocSystemDebugHeap()
		{
			//tlsf_pool	
		}

		void allocDebugHeap()
		{

		}

		void allocThreadHeap( size_t memSize )
		{

		}

		vpod allocGlobalHeap( size_t memSize )
		{

		}
#endif 

#if 1
		namespace 
		{
			static tlsf_pool			g_tlsfPool = 0;
			static tlsf_pool			g_tlsfPool_tls[8] = { 0 };
			static nc::thread::Mutex	g_commonHeapMtex;

			NC_THREAD_LOCAL tlsf_pool	tls_tlsfPool = 0;
		}

		bool isAttachedMemoryPool()
		{
			return g_tlsfPool != 0;
		}

		bool isAttachedTlsMemoryPool()
		{
			return g_tlsfPool_tls[0] != 0;
		}


		void attachMemoryForCommonHeap( void* memPtr, size_t size )
		{
			g_tlsfPool = tlsf_create( memPtr, size );
		}

		void dettachMemortPool()
		{
			nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
			tlsf_destroy( g_tlsfPool );
			g_tlsfPool = 0;
		}

		void attachTlsMemoryPool(void *memPtr, size_t size, size_t maxThread)
		{
			g_tlsfPool_tls[0] = tlsf_create( memPtr, size );
		}

		void dettachTlsMemortPool()
		{
			nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
			tlsf_destroy( g_tlsfPool_tls[0] );
			g_tlsfPool_tls[0] = 0;
		}


		void attachMemoryForThreadHeap(void *memPtr, size_t size, uint32_t threadID )
		{

		}


#endif 

		void dumpAllocatedMemory()
		{
			nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
			if( g_tlsfPool == 0 ) return;
			tlsf_walk_heap( g_tlsfPool, 0, 0 );
		}

		bool validateHeap()
		{
			nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );

			if( g_tlsfPool == 0 ) return true;
			return tlsf_check_heap( g_tlsfPool ) ? false : true;
		}

		namespace system_heap
		{
			void* malloc( size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				return std::malloc( size );
			}

			void* memalign( size_t align, size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				//return ::memalign( align, size );
				return NULL;
			}

			void* realloc( void* ptr, size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				return std::realloc( ptr, size );
			}

			void  free( void* ptr )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				std::free( ptr );
			}

			size_t getUsingBytes()
			{
				return 0;//tlsf_get_used_bytes( g_tlsfPool );
			}

			size_t getFreeBytes()
			{
				return 0;//tlsf_get_free_bytes( g_tlsfPool );
			}

			size_t getTotalBytes()
			{
				return 0;//tlsf_get_total_bytes( g_tlsfPool );
			}

		}

		/*
		namespace common_heap
		{
			//that's SIMPLE lock!! instead SLOW!!
			//somebody help me.

			void* malloc( size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				return tlsf_malloc( g_tlsfPool, size );
			}

			void* memalign( size_t align, size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				return tlsf_memalign( g_tlsfPool, align, size );
			}

			void* realloc( void* ptr, size_t size )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				return tlsf_realloc( g_tlsfPool, ptr, size );
			}

			void  free( void* ptr )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( g_commonHeapMtex );
				tlsf_free( g_tlsfPool, ptr );
			}

			size_t getUsingBytes()
			{
				return tlsf_get_used_bytes( g_tlsfPool );
			}

			size_t getFreeBytes()
			{
				return tlsf_get_free_bytes( g_tlsfPool );
			}

			size_t getTotalBytes()
			{
				return tlsf_get_total_bytes( g_tlsfPool );
			}
		}
		*/

		//TLSî≈
		namespace thread_heap
		{
			//TLSóÃàÊÇèâä˙âª
			//each thread have to call this function before tls::malloc.
			void poolConstruct();

			//TLSóÃàÊÇäJï˙
			//each thread have to call this function before thread destruction.
			void poolDestruct();

			void* malloc( size_t size )
			{
				return tlsf_malloc( g_tlsfPool, size );
			}

			void* memalign( size_t align, size_t size )
			{
				return tlsf_memalign( g_tlsfPool, align, size );
			}

			void* realloc( void* ptr, size_t size )
			{
				return tlsf_realloc( g_tlsfPool, ptr, size );
			}

			void  free( void* ptr )
			{
				tlsf_free( g_tlsfPool, ptr );
			}

			size_t getUsingBytes()
			{
				return tlsf_get_used_bytes( g_tlsfPool );
			}

			size_t getFreeBytes()
			{
				return tlsf_get_free_bytes( g_tlsfPool );
			}

			size_t getTotalBytes()
			{
				return tlsf_get_total_bytes( g_tlsfPool );
			}
		}
		
	}  // namespace memory
	
}  // namespace nc

