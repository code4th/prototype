

/*
	�f�J�C�������[���\�[�X��o�^���Ē���
	���̃��\�[�X��tlsf�ŊǗ�����

	�X���b�h���f���ɂ�
	�E�O���[�o�����������f��
	�Ethread���������f��
	��p�ӂ���

	�O���[�o�����������f���ł́A�Sthread���ʂ̃������v�[�����g�p����B
	thread���������f���ł́Athread���Ƀ������v�[���𕪂��Ďg�p����

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

			//���b�N����A�X���b�h�Z�[�t
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

			//���b�N�Ȃ��A�X���b�h�Z�[�t
			//���X���b�h�͊m�ۂ��鎖���o���Ȃ��B
			//�m�ۂ����������ւ̃A�N�Z�X���X���b�h�Z�[�t�Ȃ킯�ł͂Ȃ��̂Œ���
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
		// std::allocator�̑��class
		// �f�t�H���g�ł́A���L���������烁�������A���P�[�g���܂��B
		template <class T, class AllocatorThreadPolicyT = allocator_thread_policy::CommonHeapPolicy >
		class allocator{
		public:

			// �^��`
			// std::allocator�Ƃ̌݊��p�ɂ��̂܂܂ɂ��Ă����Ă�������
			typedef size_t		size_type;
			typedef ptrdiff_t	difference_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef T			value_type;

			// �A���P�[�^��U�^�Ƀo�C���h����
			// std::allocator�Ƃ̌݊��p�ɂ��̂܂܂ɂ��Ă����Ă�������
			template <class U>
			struct rebind{
				typedef allocator<U, AllocatorThreadPolicyT> other;
			};

			allocator() {}

			allocator( const allocator& ) {}

			template <class U>
			allocator( const allocator<U, AllocatorThreadPolicyT>& ) {}

			~allocator(){}

			// �����������蓖�Ă�
			pointer allocate( size_type num, typename allocator<void, AllocatorThreadPolicyT >::const_pointer hint = 0 )
			{
				return (pointer)AllocatorThreadPolicyT::malloc( sizeof(value_type) * num );
			}

			// ���������������
			void deallocate(pointer p, size_type num )
			{
				AllocatorThreadPolicyT::free( (void*)p );
			}


			// �����čς݂̗̈������������
			void construct(pointer p, const_reference value)
			{
				//placement new
				//�O������̃I�[�o�[���[�h�͖������̂Ƃ���
				new( (void*)p ) value_type(value);
			}

			// �������ς݂̗̈���폜����
			void destroy(pointer p)
			{
				p->~T();
			}

			// �A�h���X��Ԃ�
			pointer address(reference value) const
			{
				return &value;
			}

			const_pointer address(const_reference value) const
			{
				return &value;
			}

			// �����Ă邱�Ƃ��ł���ő�̗v�f����Ԃ�
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

