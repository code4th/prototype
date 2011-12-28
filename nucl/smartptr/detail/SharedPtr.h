#if !defined(NC_SHARED_PTR_H)
#define NC_SHARED_PTR_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"
#include "nucl/Assert.h"

#include "SpCounter.h"
#include "SpConvertible.h"


namespace nc
{
	template<class T> class SharedPtr;
	template<class T> class WeakPtr;
	template<class T> class EnableSharedFromThis;
	template<class T> class EnableSharedFromThis2;

	namespace detail
	{

		struct static_cast_tag {};
		struct const_cast_tag {};
		struct dynamic_cast_tag {};
		struct polymorphic_cast_tag {};

		template<class T>
		struct shared_ptr_traits
		{
			typedef T & Reference_t;
		};

		template<>
		struct shared_ptr_traits<void>
		{
			typedef void Reference_t;
		};

		template<>
		struct shared_ptr_traits<void const>
		{
			typedef void Reference_t;
		};

		template<>
		struct shared_ptr_traits<void volatile>
		{
			typedef void Reference_t;
		};

		template<>
		struct shared_ptr_traits<void const volatile>
		{
			typedef void Reference_t;
		};

		// EnableSharedFromThis support

		template< class X, class Y, class T >
		inline void spEnableSharedFromThis( nc::SharedPtr<X> const * ppx, Y const * py, nc::EnableSharedFromThis< T > const * pe )
		{
			if( pe != 0 )
			{
				pe->_internalAcceptOwner( ppx, const_cast< Y* >( py ) );
			}
		}

		template< class X, class Y, class T >
		inline void spEnableSharedFromThis( nc::SharedPtr<X> * ppx, Y const * py, nc::EnableSharedFromThis2< T > const * pe )
		{
			if( pe != 0 )
			{
				pe->_internalAcceptOwner( ppx, const_cast< Y* >( py ) );
			}
		}

		inline void spEnableSharedFromThis( ... )
		{
		}


		// rvalue auto_ptr support based on a technique by Dave Abrahams

		template< class T, class R >
		struct SpEnableIfAutoPtr
		{
		};

		template< class T, class R >
		struct SpEnableIfAutoPtr< std::auto_ptr< T >, R >
		{
			typedef R type;
		}; 



	} // namespace detail

	// for SharedPtr with Array
	// ex)
	//	nc::SharedPtr< int > sp( new int[ 10 ], nc::ArrayDeleter< int >() )
	//
	template< typename T >
	class ArrayDeleter{
	public:
		void operator () (T* d) const
		{
			delete [] d;
		}
	};


	//
	//  SharedPtr
	//
	//  An enhanced relative of scoped_ptr with Reference_t counted copy semantics.
	//  The object pointed to is deleted when the last SharedPtr pointing to it
	//  is destroyed or reset.
	//

	template<class T>
	class SharedPtr
	{
	private:

		// Borland 5.5.1 specific workaround
		typedef SharedPtr<T> This_t;

	public:

		typedef T Element_t;
		typedef T Value_t;
		typedef T * Pointer_t;
		typedef typename nc::detail::shared_ptr_traits<T>::Reference_t Reference_t;

		SharedPtr()
		: ptr_(0)
		, counter_() // never throws in 1.30+
		{
		}

		template<class Y>
		explicit SharedPtr( Y * p )
		: ptr_( p )
		, counter_( p ) // Y must be complete
		{
			nc::detail::spEnableSharedFromThis( this, p, p );
		}

		//
		// Requirements: DeleterT's copy constructor must not throw
		//
		// SharedPtr will release p by calling d(p)
		//

		template<class Y, class DeleterT>
		SharedPtr(Y * p, DeleterT d)
		: ptr_(p)
		, counter_(p, d)
		{
			nc::detail::spEnableSharedFromThis( this, p, p );
		}

		// As above, but with allocator. AllocatorT's copy constructor shall not throw.

		template<class Y, class DeleterT, class AllocatorT>
		SharedPtr( Y * p, DeleterT d, AllocatorT a )
		: ptr_( p )
		, counter_( p, d, a )
		{
			nc::detail::spEnableSharedFromThis( this, p, p );
		}

		//  generated copy constructor, destructor are fine

		template<class Y>
		explicit SharedPtr(WeakPtr<Y> const & rhs)
		: counter_(rhs.counter_) // may throw
		{
			// it is now safe to copy rhs.ptr_, as counter_(rhs.counter_) did not throw
			ptr_ = rhs.ptr_;
		}

		template<class Y>
		SharedPtr( WeakPtr<Y> const & rhs, nc::detail::sp_nothrow_tag )
		: ptr_( 0 )
		, counter_( rhs.counter_, nc::detail::sp_nothrow_tag() ) // never throws
		{
			if( !counter_.empty() )
			{
				ptr_ = rhs.ptr_;
			}
		}

		template<class Y>
		SharedPtr( SharedPtr<Y> const & rhs, typename nc::detail::SpEnableIfConvertible<Y,T>::type = nc::detail::SpEmpty() )
		: ptr_( rhs.ptr_ ), counter_( rhs.counter_ ) // never throws
		{
		}

		// aliasing
		template< class Y >
		SharedPtr( SharedPtr<Y> const & rhs, T * p )
		: ptr_( p )
		, counter_( rhs.counter_ ) // never throws
		{
		}

		template<class Y>
		SharedPtr(SharedPtr<Y> const & rhs, nc::detail::static_cast_tag)
		: ptr_(static_cast<Element_t *>(rhs.ptr_))
		, counter_(rhs.counter_)
		{
		}

		template<class Y>
		SharedPtr(SharedPtr<Y> const & rhs, nc::detail::const_cast_tag)
		: ptr_(const_cast<Element_t *>(rhs.ptr_))
		, counter_(rhs.counter_)
		{
		}

		template<class Y>
		SharedPtr(SharedPtr<Y> const & rhs, nc::detail::dynamic_cast_tag)
		: ptr_(dynamic_cast<Element_t *>(rhs.ptr_))
		, counter_(rhs.counter_)
		{
			if(ptr_ == 0) // need to allocate new counter -- the cast failed
			{
				counter_ = nc::detail::SharedCount();
			}
		}

		template<class Y>
		SharedPtr(SharedPtr<Y> const & rhs, nc::detail::polymorphic_cast_tag)
		: ptr_(dynamic_cast<Element_t *>(rhs.ptr_))
		, counter_(rhs.counter_)
		{
			if(ptr_ == 0)
			{
				nc::throw_exception(std::bad_cast());
			}
		}

		template<class Y>
		explicit SharedPtr(std::auto_ptr<Y> & rhs)
		: ptr_(rhs.get())
		, counter_()
		{
			Y * tmp = rhs.get();
			counter_ = nc::detail::SharedCount(rhs);
			nc::detail::spEnableSharedFromThis( this, tmp, tmp );
		}

		template<class Ap>
		explicit SharedPtr( Ap rhs, typename nc::detail::SpEnableIfAutoPtr<Ap, int>::type = 0 )
		: ptr_( rhs.get() )
		, counter_()
		{
			typename Ap::Element_t * tmp = rhs.get();
			counter_ = nc::detail::SharedCount( rhs );
			nc::detail::spEnableSharedFromThis( this, tmp, tmp );
		}


		// Move support
		#if defined( NC_HAS_RVALUE_REFS )

		SharedPtr( SharedPtr && rhs )
		: ptr_( rhs.ptr_ )
		, counter_() // never throws
		{
			counter_.swap( rhs.counter_ );
			rhs.ptr_ = 0;
		}

		template<class Y>
		SharedPtr( SharedPtr<Y> && rhs, typename nc::detail::SpEnableIfConvertible<Y,T>::type = nc::detail::SpEmpty() )
		: ptr_( rhs.ptr_ )
		, counter_() // never throws
		{
			counter_.swap( rhs.counter_ );
			rhs.ptr_ = 0;
		}

		#endif

		// assignment

		SharedPtr & operator=( SharedPtr const & rhs ) // never throws
		{
			This_t(rhs).swap(*this);
			return *this;
		}

		template<class Y>
		SharedPtr & operator=(SharedPtr<Y> const & rhs) // never throws
		{
			This_t(rhs).swap(*this);
			return *this;
		}


		template<class Y>
		SharedPtr & operator=( std::auto_ptr<Y> & rhs )
		{
			This_t(rhs).swap(*this);
			return *this;
		}

		template<class Ap>
		typename nc::detail::SpEnableIfAutoPtr< Ap, SharedPtr & >::type operator=( Ap rhs )
		{
			This_t( rhs ).swap( *this );
			return *this;
		}


		#if defined( NC_HAS_RVALUE_REFS )

		SharedPtr & operator=( SharedPtr && rhs ) // never throws
		{
			This_t( static_cast< SharedPtr && >( rhs ) ).swap( *this );
			return *this;
		}

		template<class Y>
		SharedPtr & operator=( SharedPtr<Y> && rhs ) // never throws
		{
			This_t( static_cast< SharedPtr<Y> && >( rhs ) ).swap( *this );
			return *this;
		}

		#endif



		void reset() // never throws in 1.30+
		{
			This_t().swap(*this);
		}

		template<class Y>
		void reset(Y * p) // Y must be complete
		{
			NC_ASSERT(p == 0 || p != ptr_); // catch self-reset errors
			This_t(p).swap(*this);
		}

		template<class Y, class DeleterT>
		void reset( Y * p, DeleterT d )
		{
			This_t( p, d ).swap( *this );
		}

		template<class Y, class DeleterT, class AllocatorT>
		void reset( Y * p, DeleterT d, AllocatorT a )
		{
			This_t( p, d, a ).swap( *this );
		}

		template<class Y> void reset( SharedPtr<Y> const & rhs, T * p )
		{
			This_t( rhs, p ).swap( *this );
		}

		Reference_t operator* () const // never throws
		{
			NC_ASSERT(ptr_ != 0);
			return *ptr_;
		}

		T * operator-> () const // never throws
		{
			NC_ASSERT(ptr_ != 0);
			return ptr_;
		}

		T * get() const // never throws
		{
			return ptr_;
		}


		typedef T * This_t::*unspecified_bool_type;

		operator unspecified_bool_type() const // never throws
		{
			return ptr_ == 0? 0: &This_t::ptr_;
		}

		// operator! is redundant, but some compilers need it
		bool operator! () const // never throws
		{
			return ptr_ == 0;
		}


		bool unique() const // never throws
		{
			return counter_.unique();
		}

		int32_t useCount() const // never throws
		{
			return counter_.useCount();
		}

		void swap(SharedPtr<T> & other) // never throws
		{
			std::swap(ptr_, other.ptr_);
			counter_.swap(other.counter_);
		}

		template<class Y> bool _internalLess(SharedPtr<Y> const & rhs) const
		{
			return counter_ < rhs.counter_;
		}

		void * _internal_get_deleter( std::type_info const & ti ) const
		{
			return counter_.getDeleter( ti );
		}

		bool _internal_equiv( SharedPtr const & rhs ) const
		{
			return ptr_ == rhs.ptr_ && counter_ == rhs.counter_;
		}

	private:

		template<class Y> friend class SharedPtr;
		template<class Y> friend class WeakPtr;


		T *							ptr_;	// contained Pointer_t
		nc::detail::SharedCount		counter_;	// Reference_t counter

	};  // SharedPtr

	template<class T, class U> inline bool operator==(SharedPtr<T> const & a, SharedPtr<U> const & b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const & a, SharedPtr<U> const & b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(SharedPtr<T> const & a, SharedPtr<U> const & b)
	{
		return a._internalLess(b);
	}

	template<class T> inline void swap(SharedPtr<T> & a, SharedPtr<T> & b)
	{
		a.swap(b);
	}

	template<class T, class U> SharedPtr<T> static_pointer_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::static_cast_tag());
	}

	template<class T, class U> SharedPtr<T> const_pointer_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::const_cast_tag());
	}

	template<class T, class U> SharedPtr<T> dynamic_pointer_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::dynamic_cast_tag());
	}

	// shared_*_cast names are deprecated. Use *_pointer_cast instead.

	template<class T, class U> SharedPtr<T> shared_static_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::static_cast_tag());
	}

	template<class T, class U> SharedPtr<T> shared_dynamic_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::dynamic_cast_tag());
	}

	template<class T, class U> SharedPtr<T> shared_polymorphic_cast(SharedPtr<U> const & rhs)
	{
		return SharedPtr<T>(rhs, nc::detail::polymorphic_cast_tag());
	}

	template<class T, class U> SharedPtr<T> shared_polymorphic_downcast(SharedPtr<U> const & rhs)
	{
		NC_ASSERT(dynamic_cast<T *>(rhs.get()) == rhs.get());
		return shared_static_cast<T>(rhs);
	}

	// get_pointer() enables nc::mem_fn to recognize SharedPtr

	template<class T> inline T * get_pointer(SharedPtr<T> const & p)
	{
		return p.get();
	}

	// operator<<

	template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, SharedPtr<Y> const & p)
	{
		os << p.get();
		return os;
	}

	// getDeleter

	template<class DeleterT, class T> DeleterT * getDeleter(SharedPtr<T> const & p)
	{
		return static_cast<DeleterT *>(p._internal_get_deleter(NC_SP_TYPEID(DeleterT)));
	}

	// atomic access
	// –¢ŽÀ‘•


}//namespace nc




#endif
