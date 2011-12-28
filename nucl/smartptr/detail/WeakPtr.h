#if !defined(NC_WEAK_PTR_H)
#define NC_WEAK_PTR_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"
#include "SharedPtr.h"

namespace nc
{

	template<class T> class WeakPtr
	{
	private:

		// Borland 5.5.1 specific workarounds
		typedef WeakPtr<T> this_type;

	public:

		typedef T Element_t;

		WeakPtr(): ptr_(0), counter_() // never throws in 1.30+
		{
		}

	//  generated copy constructor, assignment, destructor are fine


	//
	//  The "obvious" converting constructor implementation:
	//
	//  template<class Y>
	//  WeakPtr(WeakPtr<Y> const & rhs): ptr_(rhs.ptr_), counter_(rhs.counter_) // never throws
	//  {
	//  }
	//
	//  has a serious problem.
	//
	//  rhs.ptr_ may already have been invalidated. The ptr_(rhs.ptr_)
	//  conversion may require access to *rhs.ptr_ (virtual inheritance).
	//
	//  It is not possible to avoid spurious access violations since
	//  in multithreaded programs rhs.ptr_ may be invalidated at any point.
	//

		template<class Y>
		WeakPtr( WeakPtr<Y> const & rhs, typename nc::detail::SpEnableIfConvertible<Y,T>::type = nc::detail::SpEmpty() )
		: ptr_(rhs.lock().get()), counter_(rhs.counter_) // never throws
		{
		}

		#if defined( NC_HAS_RVALUE_REFS )

		template<class Y>
		WeakPtr( WeakPtr<Y> && rhs, typename nc::detail::SpEnableIfConvertible<Y,T>::type = nc::detail::SpEmpty() )
		: ptr_( rhs.lock().get() ), counter_( static_cast< nc::detail::WeakCount && >( rhs.counter_ ) ) // never throws
		{
			rhs.ptr_ = 0;
		}

		// for better efficiency in the T == Y case
		WeakPtr( WeakPtr && rhs ): ptr_( rhs.ptr_ ), counter_( static_cast< nc::detail::WeakCount && >( rhs.counter_ ) ) // never throws
		{
			rhs.ptr_ = 0;
		}

		// for better efficiency in the T == Y case
		WeakPtr & operator=( WeakPtr && rhs ) // never throws
		{
			this_type( static_cast< WeakPtr && >( rhs ) ).swap( *this );
			return *this;
		}
	
		#endif

		template<class Y>
		WeakPtr( SharedPtr<Y> const & rhs, typename nc::detail::SpEnableIfConvertible<Y,T>::type = nc::detail::SpEmpty() )
		: ptr_( rhs.ptr_ ), counter_( rhs.counter_ ) // never throws
		{
		}

		template<class Y>
		WeakPtr & operator=(WeakPtr<Y> const & rhs) // never throws
		{
			ptr_ = rhs.lock().get();
			counter_ = rhs.counter_;
			return *this;
		}

		#if defined( NC_HAS_RVALUE_REFS )

		template<class Y>
		WeakPtr & operator=( WeakPtr<Y> && rhs )
		{
			this_type( static_cast< WeakPtr<Y> && >( rhs ) ).swap( *this );
			return *this;
		}

		#endif

		template<class Y>
		WeakPtr & operator=(SharedPtr<Y> const & rhs) // never throws
		{
			ptr_ = rhs.ptr_;
			counter_ = rhs.counter_;
			return *this;
		}

		SharedPtr<T> lock() const // never throws
		{
			return SharedPtr<Element_t>( *this, nc::detail::sp_nothrow_tag() );
		}

		int32_t useCount() const // never throws
		{
			return counter_.useCount();
		}

		bool expired() const // never throws
		{
			return counter_.useCount() == 0;
		}

		bool _empty() const // extension, not in std::WeakPtr
		{
			return counter_.empty();
		}

		void reset() // never throws in 1.30+
		{
			this_type().swap(*this);
		}

		void swap(this_type & other) // never throws
		{
			std::swap(ptr_, other.ptr_);
			counter_.swap(other.counter_);
		}

		void _internalAssign(T * px2, nc::detail::SharedCount const & pn2)
		{
			ptr_ = px2;
			counter_ = pn2;
		}

		template<class Y> bool _internalLess(WeakPtr<Y> const & rhs) const
		{
			return counter_ < rhs.counter_;
		}

	// Tasteless as this may seem, making all members public allows member templates
	// to work in the absence of member template friends. (Matthew Langston)

	private:

		template<class Y> friend class WeakPtr;
		template<class Y> friend class SharedPtr;

		T *						ptr_;                       // contained pointer
		nc::detail::WeakCount	counter_; // reference counter

	};  // WeakPtr

	template<class T, class U> inline bool operator<(WeakPtr<T> const & a, WeakPtr<U> const & b)
	{
		return a._internalLess(b);
	}

	template<class T> void swap(WeakPtr<T> & a, WeakPtr<T> & b)
	{
		a.swap(b);
	}

}

#endif