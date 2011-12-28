#ifndef __NC_STRING_h__
#define __NC_STRING_h__

#include <string>
#include "nucl/memory/MemoryAllocator.h"

namespace nc{
	
	//nucl用の文字列型
	//使用前にメモリアロケータを初期化しないといけない。

	//各種メモリポリシー用の文字列
	typedef std::basic_string<char, std::char_traits<char>, nc::memory::allocator<char, nc::memory::allocator_thread_policy::CommonHeapPolicy > >		CommonHeapString_t;
	typedef std::basic_string<char, std::char_traits<char>, nc::memory::allocator<char, nc::memory::allocator_thread_policy::ThreadHeapPolicy > >	ThreadLocalHeapString_t;
		
	//略記法
	typedef CommonHeapString_t		CString_t;
	typedef ThreadLocalHeapString_t	TlString_t;

	//std::stringの載せ替え
	typedef CString_t	string;

//	typedef std::string CString_t;
//	typedef std::string string;

}//namespace nc


#endif // __NC_STRING_h__