

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
				 * �������ԕt����Lock�i���Ύ��ԁj
				 *
				 * Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����Ԃ����҂B
				 * �������ԓ���Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::Clock const & relative_time �҂��ԁi���Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
				 */
				bool timed_lock( nc::time::Clock const & relative_time );

				/**
				 * �������ԕt����Lock�i��Ύ��ԁj
				 *
				 * Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����ԂɂȂ�܂őҋ@
				 * �������ԓ���Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::POSIXClock const & abs_time �҂��ԁi��Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
				 */
				bool timed_lock( nc::time::POSIXClock const & abs_time);
			};

			
			/**
			 * Read-Write Lock
			 *
			 * �ǂݎ���Ə������ݎ�ŁA�e�XReadLock/WriteLock���s���B
			 * WriteLock����Ă���ꍇ�AreadLock/writeLock�͑҂�����܂��B
			 * ReadLock����Ă���ꍇ�AreadLock�͑҂�����܂��񂪁AwriteLock�͑҂�����܂��B
			 */
			class ReadWriteLock{

			protected:

				ReadWriteLock( const ReadWriteLock& );
				ReadWriteLock& operator=( const ReadWriteLock& );

			public:
				ReadWriteLock();

				~ReadWriteLock();

				/**
				 * Read Lock���܂��B
				 *
				 * writeLock����Ă����ꍇ�AwriteUnlock����܂ő҂�����܂��B
				 *
				 * @retval	void
				 */
				void readLock();

				/**
				 * Read-Lock���������܂��B
				 *
				 * @retval	void
				 */
				void readUnlock();

				/**
				 * Write-Lock���܂��B
				 *
				 * readLock����Ă���ꍇ�AreadUnlock�����܂ő҂��܂��B
				 * writeLock����Ă���ꍇ�AwriteUnlock�����܂ő҂��܂��B
				 *
				 * @retval	void
				 */
				void writeLock();

				/**
				 * Write-Lock���������܂��B
				 *
				 * @retval	void
				 */
				void writeUnlock();

				/**
				 * Write-Lock�o���邩�ǂ����𒲂ׂ�
				 *
				 * @retval	Lock�o���� == true
				 */
				bool writeTryLock();

			};

			

			class TimedReadWriteLock : public ReadWriteLock{

				TimedReadWriteLock( const TimedReadWriteLock& );
				TimedReadWriteLock& operator=( const TimedReadWriteLock& );

			public:

				/**
				 * �������ԕt����Read-Lock�i���Ύ��ԁj
				 *
				 * Read-Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����Ԃ����҂B
				 * �������ԓ���Read-Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::Clock const & relative_time �҂��ԁi���Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
				 * @sa		ReadWriteLock::readLock
				 */
				bool timedReadLock( nc::time::Clock const & relative_time );

				/**
				 * �������ԕt����Read-Lock�i��Ύ��ԁj
				 *
				 * Read-Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����ԂɂȂ�܂őҋ@
				 * �������ԓ���Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::POSIXClock const & abs_time �҂��ԁi��Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
				 * @sa		ReadWriteLock::readLock
				 */
				bool timedReadlock( nc::time::POSIXClock const & abs_time);

				/**
				 * �������ԕt����Write-Lock�i���Ύ��ԁj
				 *
				 * Write-Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����Ԃ����҂B
				 * �������ԓ���Write-Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::Clock const & relative_time �҂��ԁi���Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
				 * @sa		ReadWriteLock::writeLock
				 */
				bool timedWriteLock( nc::time::Clock const & relative_time );

				/**
				 * �������ԕt����Write-Lock�i��Ύ��ԁj
				 *
				 * Write-Lock�o���Ȃ������ꍇ�A�ő�����Ŏw�肵�����ԂɂȂ�܂őҋ@
				 * �������ԓ���Write-Lock�o���Ȃ������ꍇ��false��Ԃ��B
				 *
				 * @param	nc::time::POSIXClock const & abs_time �҂��ԁi��Ύ��ԁj
				 *
				 * @retval	Lock�o���Ȃ����� == false
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

