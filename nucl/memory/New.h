/*!
	�W����new��u�������镨�ł��B
	nc::malloc���g�p���Ă���̂ŁAnew�O�Ƀ������A���P�[�^�̏����������Ă��������B

	�O���[�o����new�͓���Ȋ֐��錾�ɂȂ�܂��̂�
	���̎����Ƃ͎኱�قȂ�܂��B

*/

#ifndef __NC_NEW_h__
#define __NC_NEW_h__

#include "nucl/Types.h"

namespace std{
	struct nothrow_t{};
	extern const nothrow_t nothrow;
}

//std::bad_alloc�𓊂��܂���B
//���s���͏��0��Ԃ��܂��B

void* operator new(size_t size);
void operator delete(void* ptr);

void* operator new(size_t size, const std::nothrow_t&);
void operator delete(void* pv, const std::nothrow_t&);

void* operator new(size_t size, void* pv);
void operator delete(void* pv, void*);

void* operator new[](size_t iSize);
void operator delete[](void* pv);

void* operator new[](size_t size, const std::nothrow_t&);
void operator delete[](void* pv, const std::nothrow_t&);

#endif // __NC_NEW_h__

