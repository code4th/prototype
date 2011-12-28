

/*
	デカイメモリーリソースを登録して頂く
	そのリソースをtlsfで管理する

	スレッドモデルには
	・グローバルメモリモデル
	・threadメモリモデル
	を用意する

	グローバルメモリモデルでは、全thread共通のメモリプールを使用する。
	threadメモリモデルでは、thread毎にメモリプールを分けて使用する

	SingleThreadModel
	ThreadShareModel
	ThreadLocalStorageModel ThreadLocalStorage TLS

*/
#ifndef MemoryAllocator_h__
#define MemoryAllocator_h__

#include <limits>
#include "nucl/Types.h"
#include "nucl/thread/Mutex.h"
#include "nucl/memory/Memory.h"

namespace nc
{
	namespace memory
	{

		namespace allocator_thread_policy
		{

			//ロックあり、スレッドセーフ
			class CommonHeapPolicy{
			public:
				static void* malloc( size_t size )
				{
					return common_heap::malloc( size );
				}

				static void* memalign( size_t align, size_t size )
				{
					return common_heap::memalign( align, size );
				}

				static void* realloc( void* ptr, size_t size )
				{
					return common_heap::realloc( ptr, size );
				}

				static void  free( void* ptr )
				{
					return common_heap::free( ptr );
				}
			};

			//ロックなし、スレッドセーフ
			//他スレッドは確保する事が出来ない。
			//確保したメモリへのアクセスがスレッドセーフなわけではないので注意
			class ThreadHeapPolicy{
			public:
				static void* malloc( size_t size )
				{
					return thread_heap::malloc( size );
				}

				static void* memalign( size_t align, size_t size )
				{
					return thread_heap::memalign( align, size );
				}

				static void* realloc( void* ptr, size_t size )
				{
					return thread_heap::realloc( ptr, size );
				}

				static void  free( void* ptr )
				{
					return thread_heap::free( ptr );
				}
			};
		}


		//void* malloc
		// std::allocatorの代替class
		// デフォルトでは、共有メモリからメモリをアロケートします。
		template <class T, class AllocatorThreadPolicyT = allocator_thread_policy::CommonHeapPolicy >
		class allocator{
		public:

			// 型定義
			// std::allocatorとの互換用にこのままにしておいてください
			typedef size_t		size_type;
			typedef ptrdiff_t	difference_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef T			value_type;

			// アロケータをU型にバインドする
			// std::allocatorとの互換用にこのままにしておいてください
			template <class U>
			struct rebind{
				typedef allocator<U, AllocatorThreadPolicyT> other;
			};

			allocator() {}

			allocator( const allocator& ) {}

			template <class U>
			allocator( const allocator<U, AllocatorThreadPolicyT>& ) {}

			~allocator(){}

			// メモリを割り当てる
			pointer allocate( size_type num, typename allocator<void, AllocatorThreadPolicyT >::const_pointer hint = 0 )
			{
				return (pointer)AllocatorThreadPolicyT::malloc( sizeof(value_type) * num );
			}

			// メモリを解放する
			void deallocate(pointer p, size_type num )
			{
				AllocatorThreadPolicyT::free( (void*)p );
			}


			// 割当て済みの領域を初期化する
			void construct(pointer p, const_reference value)
			{
				//placement new
				//外部からのオーバーロードは無いものとする
				new( (void*)p ) value_type(value);
			}

			// 初期化済みの領域を削除する
			void destroy(pointer p)
			{
				p->~T();
			}

			// アドレスを返す
			pointer address(reference value) const
			{
				return &value;
			}

			const_pointer address(const_reference value) const
			{
				return &value;
			}

			// 割当てることができる最大の要素数を返す
			size_type max_size() const
			{
#undef max
				return std::numeric_limits<size_t>::max() / sizeof(value_type);
			}
		};

		template< class AllocatorThreadPolicyT >
		class allocator< void, AllocatorThreadPolicyT >{
		public:
			typedef void* pointer;
			typedef const void* const_pointer;
			typedef void value_type;
			template < class U > struct rebind{
				typedef allocator< U, AllocatorThreadPolicyT > other;
			};
		};
		
		typedef allocator< allocator_thread_policy::CommonHeapPolicy > CommonHeapAllocator_t;
		typedef allocator< allocator_thread_policy::ThreadHeapPolicy > ThreadHeapAllocator_t;
		
	}//namespace memory

}//namespace nc

#endif // MemoryAllocator_h__

