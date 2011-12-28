
#include "Mutex.h"


//#include <boost/thread/mutex.hpp>

#ifndef NC_PLATFORM_USE_INTRINSIC_MUTEX

namespace nc
{
	namespace platform
	{
		namespace detail
		{

			namespace thread
			{





			}//namespace thread
		}//namespace detail
	}//namespace platform
}//namespace nc

#endif //#ifndef NC_PLATFORM_USE_INTRINSIC_MUTEX

		/*
		class barrier
		{
		public:
			barrier(unsigned int count)
				: m_threshold(count), m_count(count), m_generation(0)
			{
				if (count == 0)
					throw std::invalid_argument("count cannot be zero.");
			}

			bool wait()
			{
				boost::mutex::scoped_lock lock(m_mutex);
				unsigned int gen = m_generation;

				if (--m_count == 0)
				{
					m_generation++;
					m_count = m_threshold;
					m_cond.notify_all();
					return true;
				}

				while (gen == m_generation)
					m_cond.wait(lock);
				return false;
			}

		private:
			mutex m_mutex;
			condition_variable m_cond;
			unsigned int m_threshold;
			unsigned int m_count;
			unsigned int m_generation;
		};
		*/


		//class Barrior{
		//	u32	flags_;
		//	u8	num_;
		//public:
		//	Barrior()
		//		: flags_(0)
		//		, num_(0)
		//	{

		//	}

		//	void join()
		//	{

		//	}
		//};