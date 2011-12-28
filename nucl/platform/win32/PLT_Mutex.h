

#ifndef __NC_PLT_MUTEX_h__
#define __NC_PLT_MUTEX_h__

#include "PlatformInclude.h"
#include "nucl/Time.h"

namespace nc{

	namespace platform{

		namespace thread{

			class Mutex{

				CRITICAL_SECTION mutexObj_;

				Mutex( const Mutex& );
				Mutex& operator=( const Mutex& );

			public:
				Mutex();
				~Mutex();

				bool try_lock();
				void lock();
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
				bool timed_lock( nc::time::Clock const & relative_time );

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
				bool timed_lock( nc::time::POSIXClock const & abs_time);
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
	}//namespace platorm

	//namespace setup
	namespace thread{

		using nc::platform::thread::Mutex;
		using nc::platform::thread::TimedMutex;
		using nc::platform::thread::ReadWriteLock;
		using nc::platform::thread::TimedReadWriteLock;

	}//namespace thread

}//namespace nc

#endif // __NC_PLT_MUTEX_h__

