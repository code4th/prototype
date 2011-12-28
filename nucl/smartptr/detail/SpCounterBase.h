#if !defined(NC_SP_COUNTER_BASE_H)
#define NC_SP_COUNTER_BASE_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"
#include <typeinfo>
#include "nucl/CheckedDelete.h"

#include "nucl/thread/Interlocked.h"

namespace nc
{
	namespace detail
	{

		//No thread Safe..

		class SpCounterBase
		{
		private:

			SpCounterBase( SpCounterBase const & );
			SpCounterBase & operator= ( SpCounterBase const & );

			int32_t useCount_;		// #shared
			int32_t weakCount_;	   // #weak + (#shared != 0)

		public:

			SpCounterBase(): useCount_( 1 ), weakCount_( 1 )
			{
			}

			virtual ~SpCounterBase() // nothrow
			{
			}

			// dispose() is called when useCount_ drops to zero, to release
			// the resources managed by *this.
			virtual void dispose() = 0; // nothrow

			// destroy() is called when weakCount_ drops to zero.
			virtual void destroy() // nothrow
			{
				delete this;
			}

			virtual void* getDeleter( std::type_info const & ) = 0;

			void addRefCopy()
			{
				nc::interlocked::increment( (volatile int32_t*)&useCount_ );
				//++useCount_;
			}

			bool addRefLock() // true on success
			{
				while(1)
				{
					int32_t tmp = static_cast< int32_t const volatile& >( useCount_ );
					if( tmp == 0 ) return false;

					if( interlocked::compareAndExchange( (volatile int32_t*)&useCount_, tmp, tmp + 1 ) == tmp ) return true;
				}


				//if( useCount_ == 0 ) return false;
				//++useCount_;
				//return true;
			}

			void release() // nothrow
			{
				interlocked::decrement( (volatile int32_t*)&useCount_ );
				if( useCount_ == 0 )
				//if( --useCount_ == 0 )
				{
					dispose();
					weakRelease();
				}
			}

			void weakAddRef() // nothrow
			{
				nc::interlocked::increment( (volatile int32_t*)&weakCount_ );
				//++weakCount_;
			}

			void weakRelease() // nothrow
			{
				interlocked::decrement( (volatile int32_t*)&weakCount_  );
				if( weakCount_ == 0 )
				//if( --weakCount_ == 0 )
				{
					destroy();
				}
			}

			int32_t useCount() const // nothrow
			{
				return static_cast< const volatile int32_t & >( useCount_ );
			}

		};




		template<class T>
		class SpCounterImpl_P: public SpCounterBase
		{
		private:

			T * ptr_;

			SpCounterImpl_P( SpCounterImpl_P const & );
			SpCounterImpl_P & operator= ( SpCounterImpl_P const & );

			typedef SpCounterImpl_P< T > This_t;

		public:

			explicit SpCounterImpl_P( T * ptr ): ptr_( ptr )
			{
			}

			virtual void dispose() // nothrow
			{
				nc::checkedDelete( ptr_ );
			}

			virtual void * getDeleter( std::type_info const & )
			{
				return 0;
			}

			#if defined(NC_Sp_USE_STD_ALLOCATOR)

			void * operator new( std::size_t )
			{
				return std::allocator< This_t >().allocate( 1, static_cast< This_t* >(0) );
			}

			void operator delete( void * ptr )
			{
				std::allocator< This_t >().deallocate( static_cast< This_t* >( ptr ), 1 );
			}

			#endif

		};


		template<class T, class DeleterT >
		class SpCounterImpl_PD: public SpCounterBase
		{
		private:

			T			ptr_;		// copy constructor must not throw
			DeleterT	deleter_;	// copy constructor must not throw

			SpCounterImpl_PD( SpCounterImpl_PD const & );
			SpCounterImpl_PD & operator= ( SpCounterImpl_PD const & );

			typedef SpCounterImpl_PD< T, DeleterT > This_t;

		public:

			// pre: d(p) must not throw

			SpCounterImpl_PD( T ptr, DeleterT d ): ptr_(ptr), deleter_( d )
			{
			}

			virtual void dispose() // nothrow
			{
				deleter_( ptr_ );
			}

			virtual void * getDeleter( std::type_info const & ti )
			{
				return ti == typeid(DeleterT) ? &reinterpret_cast<char&>( deleter_ ): 0;
			}

			#if defined(NC_Sp_USE_STD_ALLOCATOR)

			void * operator new( std::size_t )
			{
				return std::allocator<This_t>().allocate( 1, static_cast<This_t *>(0) );
			}

			void operator delete( void * ptr )
			{
				std::allocator<This_t>().deallocate( static_cast<This_t *>(ptr), 1 );
			}

			#endif

		};

		template<class T, class DeleterT, class AllocatorT>
		class SpCounterImpl_PDA: public SpCounterBase
		{
		private:

			//The AllocatorT is the same as std::allocator

			T			ptr_;		// copy constructor must not throw
			DeleterT	deleter_;	// copy constructor must not throw
			AllocatorT	allocator_;	// copy constructor must not throw 

			SpCounterImpl_PDA( SpCounterImpl_PDA const & );
			SpCounterImpl_PDA & operator= ( SpCounterImpl_PDA const & );

			typedef SpCounterImpl_PDA< T, DeleterT, AllocatorT > This_t;

		public:

			// pre: d( p ) must not throw

			SpCounterImpl_PDA( T ptr, DeleterT deleter, AllocatorT allocator ): ptr_( p ), deleter_( deleter ), allocator_( allocator )
			{
			}

			virtual void dispose() // nothrow
			{
				deleter_( ptr_ );
			}

			virtual void destroy() // nothrow
			{
				typedef typename AllocatorT::template rebind< This_t >::other Allocator2_t;

				Allocator2_t a2( allocator_ );

				this->~This_t();
				a2.deallocate( this, 1 );
			}

			virtual void * getDeleter( std::type_info const & ti )
			{
				return ti == typeid( DeleterT ) ? &reinterpret_cast<char&>( deleter_ ): 0;
			}
		};


	}
}

#endif