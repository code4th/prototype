#ifndef __NC_MEMORY_h__
#define __NC_MEMORY_h__

#include <limits>
#include "nucl/Types.h"
#include "nucl/thread/Mutex.h"

/*
 
Memory Allocator



[MEMO]
singleThreadとか言うくくりはやめよう。
CommonHeap, ThreadHeapと言う名称にしよう
TLSは、TLSでそういう機能があるしね

*/


namespace nc{

	namespace memory{
		
		/* usage
			memory_init()
			{
				attachMemoryPool( ptr, 512*1024*1024 );
				attachTlsMemoryPool( ptr2, 128*1024*1024, 8 );	
			}
		*/

		//nucl全体で使用するメモリを登録
		//not thread safe
		void attachMemoryForCommonHeap( void* memPtr, size_t size );

		void dettachMemortPool();

		//nuclのTLSで使用するメモリを登録
		//各TLSにメモリが等しく分配される。
		//not thread safe
		void attachTlsMemoryPool(void *memPtr, size_t size, size_t maxThread);

		//ThreadHeap用のメモリを登録する
		//threadIDが未指定の場合は、カレントスレッド用とする。
		//既に登録されている場合は、元のメモリは登録解除されます。
		void attachMemoryForThreadHeap(void *memPtr, size_t size, uint32_t threadID = 0 );



		void dettachTlsMemortPool();

		//現在確保中のメモリをダンプします。
		void dumpAllocatedMemory();

		//Heapの状態が正しいか検証します。
		//falseを帰す場合はHeapが壊れている可能性があります。
		bool validateHeap();


		//AllocateCountCallback用の関数タイプ
		typedef void (*AllocCountCallback_t)( uint32_t count, void* memPtr, size_t size );

		//引数で指定したAllocateCountのAllocが呼び出される際に、コールバックを呼び出す。
		void setAllocCountCallback( uint32_t count, AllocCountCallback_t callback );


		namespace system_heap
		{
			void* malloc( size_t size );
			void* memalign( size_t align, size_t size );
			void* realloc( void* ptr, size_t size );
			void  free( void* ptr );

			size_t getUsingBytes();
			size_t getFreeBytes();
			size_t getTotalBytes();
		}

		//全スレッド共通のヒープからメモリをアロケートする
		/*
		namespace common_heap
		{
			void* malloc( size_t size );
			void* memalign( size_t align, size_t size );
			void* realloc( void* ptr, size_t size );
			void  free( void* ptr );

			size_t getUsingBytes();
			size_t getFreeBytes();
			size_t getTotalBytes();
		}
		*/

		namespace common_heap = system_heap;
		
		//TLS版
		namespace thread_heap
		{
			//TLS領域を初期化
			//each thread have to call this function before tls::malloc.
			void poolConstruct();

			//TLS領域を開放
			//each thread have to call this function before thread destruction.
			void poolDestruct();

			void* malloc( size_t size );
			void* memalign( size_t align, size_t size );
			void* realloc( void* ptr, size_t size );
			void  free( void* ptr );	

			size_t getUsingBytes();
			size_t getFreeBytes();
			size_t getTotalBytes();
		}

		//add alias "tls" as thread_heap
		namespace tls = thread_heap;

		//system default recommend storage
		namespace default_memory_allocator = common_heap;


	}//namespace memory

}//namespace nc

#endif // __NC_MEMORY_h__