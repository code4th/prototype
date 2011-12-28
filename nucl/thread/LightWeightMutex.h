#ifndef __NC_LIGHTWEIGHTMUTEX_h__
#define __NC_LIGHTWEIGHTMUTEX_h__

#include "Assert.h"
#include <pthread.h>

namespace nc
{
	namespace generic{

		namespace thread{

			class lightweight_mutex
			{
			private:

				lightweight_mutex(lightweight_mutex const &);
				lightweight_mutex & operator=(lightweight_mutex const &);

			public:

				lightweight_mutex()
				{}

				~lightweight_mutex()
				{}

				class scoped_lock;
				friend class scoped_lock;

				class scoped_lock
				{
				private:

					pthread_mutex_t & m_;

					scoped_lock(scoped_lock const &);
					scoped_lock & operator=(scoped_lock const &);

				public:

					scoped_lock(lightweight_mutex & m): m_(m.m_)
					{
						int res = pthread_mutex_lock( &m_ );
						NC_ASSERT( res == 0 );
					}

					~scoped_lock()
					{
						int res = pthread_mutex_unlock( &m_ );
						NC_ASSERT( res == 0 );
					}
				};
			};



			namespace detail{

				class lightweight_mutex
				{
				private:

					pthread_mutex_t m_;

					lightweight_mutex(lightweight_mutex const &);
					lightweight_mutex & operator=(lightweight_mutex const &);

				public:

					lightweight_mutex()
					{
						int res = pthread_mutex_init( &m_, 0 );
						NC_ASSERT( !res );
					}

					~lightweight_mutex()
					{
						int res = pthread_mutex_destroy( &m_ );
						NC_ASSERT( !res );
					}

					class scoped_lock;
					friend class scoped_lock;

					class scoped_lock
					{
					private:

						pthread_mutex_t & m_;

						scoped_lock(scoped_lock const &);
						scoped_lock & operator=(scoped_lock const &);

					public:

						scoped_lock(lightweight_mutex & m): m_(m.m_)
						{
							int res = pthread_mutex_lock( &m_ );
							NC_ASSERT( !res );
						}

						~scoped_lock()
						{
							int res = pthread_mutex_unlock( &m_ );
							NC_ASSERT( !res );
						}
					};
				};

			} // namespace detail

		}// namespace thread

	}

} // namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MUTEX

#include "PLT_LightWeightMutex.h"

#else

namespace nc{

	//namespace setup
	namespace thread{

		using nc::generic::thread

	}//namespace thread

}//namespace nc

#endif


#endif // __NC_LIGHTWEIGHTMUTEX_h__
