#if !defined(NC_ENABLE_SHARED_FROM_THIS_H)
#define NC_ENABLE_SHARED_FROM_THIS_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "nucl/NuclConfig.h"
#include "WeakPtr.h"
#include "SharedPtr.h"

namespace nc
{

	namespace detail
	{

		template<class T> class EnableSharedFromThis
		{
		protected:

			EnableSharedFromThis()
			{
			}

			EnableSharedFromThis(EnableSharedFromThis const &)
			{
			}

			EnableSharedFromThis & operator=(EnableSharedFromThis const &)
			{
				return *this;
			}

			~EnableSharedFromThis()
			{
			}

		public:

			SharedPtr<T> sharedFromThis()
			{
				SharedPtr<T> p( weakThis_ );
				NC_ASSERT( p.get() == this );
				return p;
			}

			SharedPtr<T const> sharedFromThis() const
			{
				SharedPtr<T const> p( weakThis_ );
				NC_ASSERT( p.get() == this );
				return p;
			}

		public: // actually private, but avoids compiler template friendship issues

			// Note: invoked automatically by SharedPtr; do not call
			template<class X, class Y> void _internalAcceptOwner( SharedPtr<X> const * ppx, Y * py ) const
			{
				if( weakThis_.expired() )
				{
					weakThis_ = SharedPtr<T>( *ppx, py );
				}
			}

		private:

			mutable WeakPtr<T> weakThis_;
		};
	}

}

#endif