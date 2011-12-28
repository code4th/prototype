#ifndef Mutex_h__
#define Mutex_h__

#include "nucl/Assert.h"
#include "nucl/Time.h"

namespace nc{

	namespace generic{

		namespace thread{
			
			class Mutex{

			protected:
				Mutex( const Mutex& );
				Mutex& operator=( const Mutex& );

			public:
				Mutex();
				~Mutex();

				/**
				 * MutexをLock出来るかどうかを調べる
				 *
				 * @retval	Lock出来る == true
				 */
				bool try_lock();

				/**
				 * MutexをLockする（再帰可能）
				 *
				 * 再帰Lock可能だが、Lockと同じだけのUnLockをする必要がある。
				 *
				 * @retval	void
				 */
				void lock();

				/**
				 * MutexをUnlockする
				 *
				 * 再帰Lockされていた場合、同じ回数だけUnlockする必要がある。
				 *
				 * @retval	void
				 */
				void unlock();

			};

			class TimedMutex : public Mutex{

				TimedMutex( const TimedMutex& );
				TimedMutex& operator=( const TimedMutex& );

			public:
				TimedMutex();
				~TimedMutex();

				/**
				 * 制限時間付きのLock（相対時間）
				 *
				 * Lock出来なかった場合、最大引数で指定した時間だけ待つ。
				 * もし時間内にLock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
				 *
				 * @retval	Lock出来なかった == false
				 */
				bool timedLock( nc::time::Clock const & relative_time );

				/**
				 * 制限時間付きのLock（絶対時間）
				 *
				 * Lock出来なかった場合、最大引数で指定した時間になるまで待機
				 * もし時間内にLock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
				 *
				 * @retval	Lock出来なかった == false
				 */
				bool timedLock( nc::time::POSIXClock const & abs_time);
			};


			/**
			 * Read-Write Lock
			 *
			 * 読み取り手と書き込み手で、各々ReadLock/WriteLockを行う。
			 * WriteLockされている場合、readLock/writeLockは待たされます。
			 * ReadLockされている場合、readLockは待たされませんが、writeLockは待たされます。
			 */
			class ReadWriteLock{

			protected:

				ReadWriteLock( const ReadWriteLock& );
				ReadWriteLock& operator=( const ReadWriteLock& );

			public:
				ReadWriteLock();

				~ReadWriteLock();

				/**
				 * Read Lockします。
				 *
				 * writeLockされていた場合、writeUnlockするまで待たされます。
				 *
				 * @retval	void
				 */
				void readLock();

				/**
				 * Read-Lockを解除します。
				 *
				 * @retval	void
				 */
				void readUnlock();

				/**
				 * Write-Lockします。
				 *
				 * readLockされている場合、readUnlockされるまで待ちます。
				 * writeLockされている場合、writeUnlockされるまで待ちます。
				 *
				 * @retval	void
				 */
				void writeLock();

				/**
				 * Write-Lockを解除します。
				 *
				 * @retval	void
				 */
				void writeUnlock();

				/**
				 * Write-Lock出来るかどうかを調べる
				 *
				 * @retval	Lock出来る == true
				 */
				bool writeTryLock();

			};

			

			class TimedReadWriteLock : public ReadWriteLock{

				TimedReadWriteLock( const TimedReadWriteLock& );
				TimedReadWriteLock& operator=( const TimedReadWriteLock& );

			public:

				/**
				 * 制限時間付きのRead-Lock（相対時間）
				 *
				 * Read-Lock出来なかった場合、最大引数で指定した時間だけ待つ。
				 * もし時間内にRead-Lock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
				 *
				 * @retval	Lock出来なかった == false
				 * @sa		ReadWriteLock::readLock
				 */
				bool timedReadLock( nc::time::Clock const & relative_time );

				/**
				 * 制限時間付きのRead-Lock（絶対時間）
				 *
				 * Read-Lock出来なかった場合、最大引数で指定した時間になるまで待機
				 * もし時間内にLock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
				 *
				 * @retval	Lock出来なかった == false
				 * @sa		ReadWriteLock::readLock
				 */
				bool timedReadlock( nc::time::POSIXClock const & abs_time);

				/**
				 * 制限時間付きのWrite-Lock（相対時間）
				 *
				 * Write-Lock出来なかった場合、最大引数で指定した時間だけ待つ。
				 * もし時間内にWrite-Lock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
				 *
				 * @retval	Lock出来なかった == false
				 * @sa		ReadWriteLock::writeLock
				 */
				bool timedWriteLock( nc::time::Clock const & relative_time );

				/**
				 * 制限時間付きのWrite-Lock（絶対時間）
				 *
				 * Write-Lock出来なかった場合、最大引数で指定した時間になるまで待機
				 * もし時間内にWrite-Lock出来なかった場合はfalseを返す。
				 *
				 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
				 *
				 * @retval	Lock出来なかった == false
				 * @sa		ReadWriteLock::writeLock
				 */
				bool timedWritelock( nc::time::POSIXClock const & abs_time);

			};

		}// namespace thread

	}//namespace generic

}//namespace nc

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MUTEX

#include "PLT_Mutex.h"

#else

#include "Assert.h"
#include "pthread.h"

#define NC_PTHREAD_HAS_TIMEDLOCK

namespace nc{
	
	namespace platform{

		namespace detail{

			namespace thread{
	
				class Mutex{

				protected:
					pthread_mutex_t m;

					Mutex( const Mutex& );
					Mutex& operator=( const Mutex& );

				public:
					Mutex();
					~Mutex();

					/**
					 * MutexをLock出来るかどうかを調べる
					 *
					 * @retval	Lock出来る == true
					 */
					bool try_lock();

					/**
					 * MutexをLockする（再帰可能）
					 *
					 * 再帰Lock可能だが、Lockと同じだけのUnLockをする必要がある。
					 *
					 * @retval	void
					 */
					void lock();

					/**
					 * MutexをUnlockする
					 *
					 * 再帰Lockされていた場合、同じ回数だけUnlockする必要がある。
					 *
					 * @retval	void
					 */
					void unlock();

				};

				class TimedMutex : public Mutex{

					TimedMutex( const TimedMutex& );
					TimedMutex& operator=( const TimedMutex& );

					pthread_mutex_t m;

				#ifndef NC_PTHREAD_HAS_TIMEDLOCK
					pthread_cond_t cond;
					bool is_locked;
				#endif

				public:
					TimedMutex();
					~TimedMutex();

				#ifndef NC_PTHREAD_HAS_TIMEDLOCK


					/**
					 * MutexをLock出来るかどうかを調べる
					 *
					 * @retval	Lock出来る == true
					 */
					bool try_lock();

					/**
					 * MutexをLockする（再帰可能）
					 *
					 * 再帰Lock可能だが、Lockと同じだけのUnLockをする必要がある。
					 *
					 * @retval	void
					 */
					void lock();

					/**
					 * MutexをUnlockする
					 *
					 * 再帰Lockされていた場合、同じ回数だけUnlockする必要がある。
					 *
					 * @retval	void
					 */
					void unlock();

				#endif

					/**
					 * 制限時間付きのLock（相対時間）
					 *
					 * Lock出来なかった場合、最大引数で指定した時間だけ待つ。
					 * もし時間内にLock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
					 *
					 * @retval	Lock出来なかった == false
					 */
					bool timedLock( nc::time::Clock const & relative_time );

					/**
					 * 制限時間付きのLock（絶対時間）
					 *
					 * Lock出来なかった場合、最大引数で指定した時間になるまで待機
					 * もし時間内にLock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
					 *
					 * @retval	Lock出来なかった == false
					 */
					bool timedLock( nc::time::POSIXClock const & abs_time);
				};


				/**
				 * Read-Write Lock
				 *
				 * 読み取り手と書き込み手で、各々ReadLock/WriteLockを行う。
				 * WriteLockされている場合、readLock/writeLockは待たされます。
				 * ReadLockされている場合、readLockは待たされませんが、writeLockは待たされます。
				 */
				class ReadWriteLock{

				protected:

					pthread_rwlock_t rwlock_;

					ReadWriteLock( const ReadWriteLock& );
					ReadWriteLock& operator=( const ReadWriteLock& );

				public:
					ReadWriteLock();

					~ReadWriteLock();

					/**
					 * Read Lockします。
					 *
					 * writeLockされていた場合、writeUnlockするまで待たされます。
					 *
					 * @retval	void
					 */
					void readLock();


					/**
					 * Read-Lockを解除します。
					 *
					 * @retval	void
					 */
					void readUnlock();

					/**
					 * Write-Lockします。
					 *
					 * readLockされている場合、readUnlockされるまで待ちます。
					 * writeLockされている場合、writeUnlockされるまで待ちます。
					 *
					 * @retval	void
					 */
					void writeLock();

					/**
					 * Write-Lockを解除します。
					 *
					 * @retval	void
					 */
					void writeUnlock();

					/**
					 * Write-Lock出来るかどうかを調べる
					 *
					 * @retval	Lock出来る == true
					 */
					bool writeTryLock();

				};

				class TimedReadWriteLock : public ReadWriteLock{

					TimedReadWriteLock( const TimedReadWriteLock& );
					TimedReadWriteLock& operator=( const TimedReadWriteLock& );

				public:

					/**
					 * 制限時間付きのRead-Lock（相対時間）
					 *
					 * Read-Lock出来なかった場合、最大引数で指定した時間だけ待つ。
					 * もし時間内にRead-Lock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
					 *
					 * @retval	Lock出来なかった == false
					 * @sa		ReadWriteLock::readLock
					 */
					bool timedReadLock( nc::time::Clock const & relative_time );

					/**
					 * 制限時間付きのRead-Lock（絶対時間）
					 *
					 * Read-Lock出来なかった場合、最大引数で指定した時間になるまで待機
					 * もし時間内にLock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
					 *
					 * @retval	Lock出来なかった == false
					 * @sa		ReadWriteLock::readLock
					 */
					bool timedReadlock( nc::time::POSIXClock const & abs_time);

					/**
					 * 制限時間付きのWrite-Lock（相対時間）
					 *
					 * Write-Lock出来なかった場合、最大引数で指定した時間だけ待つ。
					 * もし時間内にWrite-Lock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::Clock const & relative_time 待つ時間（相対時間）
					 *
					 * @retval	Lock出来なかった == false
					 * @sa		ReadWriteLock::writeLock
					 */
					bool timedWriteLock( nc::time::Clock const & relative_time );

					/**
					 * 制限時間付きのWrite-Lock（絶対時間）
					 *
					 * Write-Lock出来なかった場合、最大引数で指定した時間になるまで待機
					 * もし時間内にWrite-Lock出来なかった場合はfalseを返す。
					 *
					 * @param	nc::time::POSIXClock const & abs_time 待つ時間（絶対時間）
					 *
					 * @retval	Lock出来なかった == false
					 * @sa		ReadWriteLock::writeLock
					 */
					bool timedWritelock( nc::time::POSIXClock const & abs_time);
				};
			

				//--------------------inline impliment--------------------------
			
				inline Mutex::Mutex()
				{
					m = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
					//int const res = pthread_mutex_init(&m,NULL);
					//NC_ASSERT( !res );
				}

				inline Mutex::~Mutex()
				{
					pthread_mutex_destroy(&m);
				}

				inline bool Mutex::try_lock()
				{
					int const res=pthread_mutex_trylock(&m);
					NC_ASSERT( res && (res!=EBUSY) );

					//resh
					return !res;
				}

				inline void Mutex::lock()
				{
					int const res=pthread_mutex_lock(&m);
					NC_ASSERT( !res );
				}

				inline void Mutex::unlock()
				{
					pthread_mutex_unlock(&m);
				}

				//-----------------------

				inline timespec get_timespec( nc::time::POSIXClock clk )
				{
					timespec t;

					t.tv_sec  = (long)clk.sec();
					t.tv_nsec = (long)( (clk - nc::time::Sec( clk.sec() )).nsec() );
					return t;
				}

				inline TimedMutex::TimedMutex()
				{
#ifndef NC_PTHREAD_HAS_TIMEDLOCK
					int const res2=pthread_cond_init(&cond,NULL);
					NC_ASSERT( !res2 );
					is_locked=false;
#endif
				}

				inline TimedMutex::~TimedMutex()
				{
#ifndef NC_PTHREAD_HAS_TIMEDLOCK
					pthread_cond_destroy(&cond);
#endif
				}

				inline bool TimedMutex::timedLock( nc::time::Clock const & relative_time )
				{
					return timedLock( static_cast< nc::time::POSIXClock >( nc::time::getNowPOSIXClock() + relative_time ) );
				}

#ifndef NC_PTHREAD_HAS_TIMEDLOCK

				inline void TimedMutex::lock()
				{
					nc::thread::ScopedLock< nc::thread::Mutex > const local_lock(&m);
					while(is_locked)
					{
						pthread_cond_wait(&cond,&m);
					}
					is_locked=true;
				}

				inline void TimedMutex::unlock()
				{
					nc::thread::ScopedLock< nc::thread::Mutex >  const local_lock(&m);
					is_locked=false;
					pthread_cond_signal(&cond);
				}

				inline bool TimedMutex::try_lock()
				{
					nc::thread::ScopedLock< nc::thread::Mutex >  const local_lock(&m);
					if(is_locked)
					{
						return false;
					}
					is_locked=true;
					return true;
				}

				inline bool TimedMutex::timedLock( nc::time::POSIXClock const & abs_time)
				{
					struct timespec const timeout = get_timespec(abs_time);
					nc::thread::ScopedLock< nc::thread::Mutex > const local_lock(&m);
					while(is_locked)
					{
						int const cond_res=pthread_cond_timedwait(&cond,&m,&timeout);
						if(cond_res==ETIMEDOUT)
						{
							return false;
						}
						NC_ASSERT(!cond_res);
					}
					is_locked=true;
					return true;
				}

#else

				inline bool TimedMutex::timedLock( nc::time::POSIXClock const & abs_time)
				{
					struct timespec const timeout = get_timespec(abs_time);
					int const res=pthread_mutex_timedlock(&m,&timeout);
					NC_ASSERT(!res || res==ETIMEDOUT);
					return !res;
				}

#endif


				//-----------------------

				inline ReadWriteLock::ReadWriteLock()
					: rwlock_( PTHREAD_RWLOCK_INITIALIZER )
				{
					//pthread_rwlock_init( &rwlock_, NULL );
				}

				inline ReadWriteLock::~ReadWriteLock()
				{
					pthread_rwlock_destroy( &rwlock_ );
				}

				inline void ReadWriteLock::readLock()
				{
					int ret = pthread_rwlock_rdlock( &rwlock_ );
				}

				inline void ReadWriteLock::readUnlock()
				{
					int ret = pthread_rwlock_unlock( &rwlock_ );
				}

				inline void ReadWriteLock::writeLock()
				{
					int ret = pthread_rwlock_wrlock( &rwlock_ );
				}

				inline void ReadWriteLock::writeUnlock()
				{
					int ret = pthread_rwlock_unlock( &rwlock_ );
				}

				//-----------------------


				inline bool TimedReadWriteLock::timedReadLock( nc::time::Clock const & relative_time )
				{
					return timedReadlock( static_cast< nc::time::POSIXClock >( nc::time::getNowPOSIXClock() + relative_time ) );
				}

				inline bool TimedReadWriteLock::timedReadlock( nc::time::POSIXClock const & abs_time )
				{
					struct timespec const timeout = get_timespec(abs_time);
					int const res=pthread_rwlock_timedrdlock(&rwlock_,&timeout);
					NC_ASSERT(!res || res==ETIMEDOUT);
					return !res;
				}

				inline bool TimedReadWriteLock::timedWriteLock( nc::time::Clock const & relative_time )
				{
					return timedWriteLock( static_cast< nc::time::POSIXClock >( nc::time::getNowPOSIXClock() + relative_time ) );
				}

				inline bool TimedReadWriteLock::timedWritelock( nc::time::POSIXClock const & abs_time )
				{
					struct timespec const timeout = get_timespec(abs_time);
					int const res=pthread_rwlock_timedwrlock(&rwlock_,&timeout);
					NC_ASSERT(!res || res==ETIMEDOUT);
					return !res;
				}



			}// namespace thread

		}//namespace detail

	}//namespace platform


	//namespace setup
	namespace thread{

		using nc::platform::detail::thread::Mutex;
		using nc::platform::detail::thread::TimedMutex;
		using nc::platform::detail::thread::ReadWriteLock;
		using nc::platform::detail::thread::TimedReadWriteLock;

	}//namespace thread

}//namespace nc

#endif
		

namespace nc
{
	namespace thread
	{

		class UniqueLock{

		};

		template< typename MutexT >
		class ScopedLock{
			MutexT& mtex_;
			bool locked_;
		
		public:
			ScopedLock( MutexT& mtex )
				: mtex_( mtex )
			{
				mtex.lock();
				locked_ = true;
			}

			~ScopedLock()
			{
				if( locked_ )
				{
					unlock();
				}
			}

			void unlock()
			{
				mtex_.unlock();
				locked_ = false;
			}
		};

		template< typename RWLockT >
		class ScopedReadLock{
			RWLockT & rwLock_;
			bool locked_;

		public:
			ScopedReadLock( RWLockT& rwLock )
				: rwLock_( rwLock )
			{
				rwLock_.readLock();
				locked_ = true;
			}

			~ScopedReadLock()
			{
				if( locked_ )
				{
					unlock();
				}
			}

			void unlock()
			{
				rwLock_.readUnlock();
				locked_ = false;
			}
		};

		template< typename RWLockT >
		class ScopedWriteLock{
			RWLockT & rwLock_;
			bool locked_;

		public:
			ScopedWriteLock( RWLockT& rwLock )
				: rwLock_( rwLock )
			{
				rwLock_.writeLock();
				locked_ = true;
			}

			~ScopedWriteLock()
			{
				if( locked_ )
				{
					unlock();
				}
			}

			void unlock()
			{
				rwLock_.writeUnlock();
				locked_ = false;
			}
		};

	}
}

#endif // Mutex_h__
