#ifndef __NC_MEMORY_h__
#define __NC_MEMORY_h__

#include <limits>
#include "nucl/Types.h"
#include "nucl/thread/Mutex.h"

/*
 
Memory Allocator



[MEMO]
singleThread�Ƃ�����������͂�߂悤�B
CommonHeap, ThreadHeap�ƌ������̂ɂ��悤
TLS�́ATLS�ł��������@�\�����邵��

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

		//nucl�S�̂Ŏg�p���郁������o�^
		//not thread safe
		void attachMemoryForCommonHeap( void* memPtr, size_t size );

		void dettachMemortPool();

		//nucl��TLS�Ŏg�p���郁������o�^
		//�eTLS�Ƀ����������������z�����B
		//not thread safe
		void attachTlsMemoryPool(void *memPtr, size_t size, size_t maxThread);

		//ThreadHeap�p�̃�������o�^����
		//threadID�����w��̏ꍇ�́A�J�����g�X���b�h�p�Ƃ���B
		//���ɓo�^����Ă���ꍇ�́A���̃������͓o�^��������܂��B
		void attachMemoryForThreadHeap(void *memPtr, size_t size, uint32_t threadID = 0 );



		void dettachTlsMemortPool();

		//���݊m�ے��̃��������_���v���܂��B
		void dumpAllocatedMemory();

		//Heap�̏�Ԃ������������؂��܂��B
		//false���A���ꍇ��Heap�����Ă���\��������܂��B
		bool validateHeap();


		//AllocateCountCallback�p�̊֐��^�C�v
		typedef void (*AllocCountCallback_t)( uint32_t count, void* memPtr, size_t size );

		//�����Ŏw�肵��AllocateCount��Alloc���Ăяo�����ۂɁA�R�[���o�b�N���Ăяo���B
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

		//�S�X���b�h���ʂ̃q�[�v���烁�������A���P�[�g����
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
		
		//TLS��
		namespace thread_heap
		{
			//TLS�̈��������
			//each thread have to call this function before tls::malloc.
			void poolConstruct();

			//TLS�̈���J��
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