#ifndef __NC_SINGLETON_h__
#define __NC_SINGLETON_h__

#include "Assert.h"
#include "nucl/thread/Mutex.h"

namespace nc{
	
	//�ʏ�̃V���O���g���ƈႢ�A�����I�ɃC���X�^���X������������K�v������܂��B
	template< typename T >
	class Singleton{

		static nc::thread::Mutex mtex_;
		static T* instance_;

	public:

		//�C���X�^���X�쐬
		static void instantiate()
		{
			if( instance_ == 0 )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );
				if( instance_ == 0 )
				{
					//nc::memory::common_heap::malloc(
					instance_ = new T;
				}
			}
		}

		//�C���X�^���X�j��
		static void destroy()
		{
			if( instance_ )
			{
				nc::thread::ScopedLock< nc::thread::Mutex > lock( mtex_ );
				if( instance_ )
				{
					delete instance_;
					instance_ = 0;
				}
			}
		}

		static T& instance()
		{
			return *instance_;
		}
		
	};

	template< typename T >
	T* Singleton< T >::instance_ = 0;

	template< typename T >
	nc::thread::Mutex Singleton< T >::mtex_;

}//namespace nc

#endif // __NC_SINGLETON_h__