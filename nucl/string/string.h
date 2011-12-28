#ifndef __NC_STRING_h__
#define __NC_STRING_h__

#include <string>
#include "nucl/memory/MemoryAllocator.h"

namespace nc{
	
	//nucl�p�̕�����^
	//�g�p�O�Ƀ������A���P�[�^�����������Ȃ��Ƃ����Ȃ��B

	//�e�탁�����|���V�[�p�̕�����
	typedef std::basic_string<char, std::char_traits<char>, nc::memory::allocator<char, nc::memory::allocator_thread_policy::CommonHeapPolicy > >		CommonHeapString_t;
	typedef std::basic_string<char, std::char_traits<char>, nc::memory::allocator<char, nc::memory::allocator_thread_policy::ThreadHeapPolicy > >	ThreadLocalHeapString_t;
		
	//���L�@
	typedef CommonHeapString_t		CString_t;
	typedef ThreadLocalHeapString_t	TlString_t;

	//std::string�̍ڂ��ւ�
	typedef CString_t	string;

//	typedef std::string CString_t;
//	typedef std::string string;

}//namespace nc


#endif // __NC_STRING_h__