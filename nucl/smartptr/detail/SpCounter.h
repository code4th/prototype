#if !defined(NC_SP_COUNTER_H)
#define NC_SP_COUNTER_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"
#include "SpCounterBase.h"
#include "nucl/ThrowException.h"

#include <memory>
#include <functional>

namespace nc
{
	namespace detail
	{

		struct sp_nothrow_tag {};

		class WeakCount;

		class SharedCount
		{
		private:

			SpCounterBase * counter_;

			friend class WeakCount;

		public:

			SharedCount()
			: counter_(0) // nothrow
			{
			}

			template< class T >
			explicit SharedCount( T * ptr )
			: counter_( 0 )
			{
				#if !defined( NC_NO_EXCEPTIONS )

				try
				{
					counter_ = new SpCounterImpl_P< T >( ptr );
				}
				catch(...)
				{
					nc::checkedDelete( ptr );
					throw;
				}

				#else

				counter_ = new SpCounterImpl_P< T >( ptr );

				if( counter_ == 0 )
				{
					nc::checkedDelete( ptr );
					nc::throwException( std::bad_alloc() );
				}

				#endif

			}

			template< class T, class DeleterT >
			SharedCount( T ptr, DeleterT deleter )
			: counter_(0)
			{
				#if !defined( NC_NO_EXCEPTIONS )

				try
				{
					counter_ = new SpCounterImpl_PD< T, DeleterT >( ptr, deleter );
				}
				catch(...)
				{
					deleter( ptr ); // delete ptr
					throw;
				}

				#else

				counter_ = new SpCounterImpl_PD< T, DeleterT >(ptr, deleter);

				if(counter_ == 0)
				{
					deleter(ptr); // delete ptr
					nc::throwException(std::bad_alloc());
				}

				#endif

			}

			template<class T, class DeleterT, class AllocatorT>
			SharedCount( T ptr, DeleterT deleter, AllocatorT allocator )
			: counter_( 0 )
			{
				typedef SpCounterImpl_PDA<T, DeleterT, AllocatorT> Impl_t;
				typedef typename AllocatorT::template rebind< Impl_t >::other Allocator2_t;

				Allocator2_t allocator2( allocator );

				#if !defined( NC_NO_EXCEPTIONS )

				try
				{
					counter_ = allocator2.allocate( 1, static_cast< Impl_t* >( 0 ) );
					new( static_cast< void* >( counter_ ) ) Impl_t( ptr, deleter, allocator );
				}
				catch(...)
				{
					deleter( ptr );

					if( counter_ != 0 )
					{
						allocator2.deallocate( static_cast< Impl_t* >( counter_ ), 1 );
					}

					throw;
				}

				#else

				counter_ = allocator2.allocate( 1, static_cast< Impl_t* >( 0 ) );

				if( counter_ != 0 )
				{
					new( static_cast< void* >( counter_ ) ) Impl_t( ptr, deleter, allocator );
				}
				else
				{
					deleter( ptr );
					nc::throwException( std::bad_alloc() );
				}

				#endif

			}

			template<class T>
			explicit SharedCount( std::auto_ptr<T> & rhs )
			: counter_( new SpCounterImpl_P< T > ( rhs.get() ) )
			{
				#if defined(NC_NO_EXCEPTIONS)
				if( counter_ == 0 )
				{
					nc::throwException(std::bad_alloc());
				}
				#endif
				rhs.release();
			}

			~SharedCount() // nothrow
			{
				if( counter_ != 0 ) counter_->release();
			}

			SharedCount(SharedCount const & rhs)
			: counter_(rhs.counter_) // nothrow
			{
				if( counter_ != 0 ) counter_->addRefCopy();
			}

			#if defined( NC_HAS_RVALUE_REFS )
			SharedCount(SharedCount && rhs)
			: counter_(rhs.counter_) // nothrow
			{
				rhs.counter_ = 0;
			}
			#endif

			explicit SharedCount(WeakCount const & rhs); // throws bad_weak_ptr when r.use_count() == 0
			SharedCount( WeakCount const & rhs, sp_nothrow_tag ); // constructs an empty *this when r.use_count() == 0

			SharedCount & operator= ( SharedCount const & rhs ) // nothrow
			{
				SpCounterBase * tmp = rhs.counter_;

				if( tmp != counter_ )
				{
					if( tmp != 0 ) tmp->addRefCopy();
					if( counter_ != 0 ) counter_->release();
					counter_ = tmp;
				}

				return *this;
			}

			void swap(SharedCount & rhs ) // nothrow
			{
				SpCounterBase * tmp = rhs.counter_;
				rhs.counter_ = counter_;
				counter_ = tmp;
			}

			int32_t useCount() const // nothrow
			{
				return counter_ != 0 ? counter_->useCount() : 0;
			}

			bool unique() const // nothrow
			{
				return useCount() == 1;
			}

			bool empty() const // nothrow
			{
				return counter_ == 0;
			}

			friend inline bool operator== ( SharedCount const & lhs, SharedCount const & rhs )
			{
				return lhs.counter_ == rhs.counter_;
			}

			friend inline bool operator< ( SharedCount const & lhs, SharedCount const & rhs )
			{
				return std::less< SpCounterBase* >()( lhs.counter_, rhs.counter_ );
			}

			void * getDeleter( std::type_info const & ti ) const
			{
				return counter_? counter_->getDeleter( ti ): 0;
			}
		};


		class WeakCount
		{
		private:

			SpCounterBase * counter_;

			friend class SharedCount;

		public:

			WeakCount()
			: counter_(0) // nothrow
			{
			}

			WeakCount(SharedCount const & rhs)
			: counter_(rhs.counter_) // nothrow
			{
				if(counter_ != 0) counter_->weakAddRef();
			}

			WeakCount(WeakCount const & rhs)
			: counter_(rhs.counter_) // nothrow
			{
				if(counter_ != 0) counter_->weakAddRef();
			}

			#if defined( NC_HAS_RVALUE_REFS )
			WeakCount(WeakCount && rhs)
			: counter_(rhs.counter_) // nothrow
			{
				rhs.counter_ = 0;
			}
			#endif

			~WeakCount() // nothrow
			{
				if(counter_ != 0) counter_->weakRelease();
			}

			WeakCount & operator= (SharedCount const & rhs) // nothrow
			{
				SpCounterBase * tmp = rhs.counter_;

				if( tmp != counter_ )
				{
					if(tmp != 0) tmp->weakAddRef();
					if(counter_ != 0) counter_->weakRelease();
					counter_ = tmp;
				}

				return *this;
			}

			WeakCount & operator= (WeakCount const & rhs) // nothrow
			{
				SpCounterBase * tmp = rhs.counter_;

				if( tmp != counter_ )
				{
					if(tmp != 0) tmp->weakAddRef();
					if(counter_ != 0) counter_->weakRelease();
					counter_ = tmp;
				}

				return *this;
			}

			void swap(WeakCount & rhs) // nothrow
			{
				SpCounterBase * tmp = rhs.counter_;
				rhs.counter_ = counter_;
				counter_ = tmp;
			}

			int32_t useCount() const // nothrow
			{
				return counter_ != 0? counter_->useCount(): 0;
			}

			bool empty() const // nothrow
			{
				return counter_ == 0;
			}

			friend inline bool operator==(WeakCount const & lhs, WeakCount const & rhs)
			{
				return lhs.counter_ == rhs.counter_;
			}

			friend inline bool operator<(WeakCount const & lhs, WeakCount const & rhs)
			{
				return std::less< SpCounterBase* >()(lhs.counter_, rhs.counter_);
			}
		};

		inline SharedCount::SharedCount( WeakCount const & rhs )
		: counter_( rhs.counter_ )
		{
			if( counter_ == 0 || !counter_->addRefLock() )
			{
				nc::throwException( nc::bad_weak_ptr() );
			}
		}

		inline SharedCount::SharedCount( WeakCount const & rhs, sp_nothrow_tag )
		: counter_( rhs.counter_ )
		{
			if( counter_ != 0 && !counter_->addRefLock() )
			{
				counter_ = 0;
			}
		}

	}
}

#endif