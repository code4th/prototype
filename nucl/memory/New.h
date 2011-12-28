/*!
	標準のnewを置き換える物です。
	nc::mallocを使用しているので、new前にメモリアロケータの初期化をしてください。

	グローバルなnewは特殊な関数宣言になりますので
	他の実装とは若干異なります。

*/

#ifndef __NC_NEW_h__
#define __NC_NEW_h__

#include "nucl/Types.h"

namespace std{
	struct nothrow_t{};
	extern const nothrow_t nothrow;
}

//std::bad_allocを投げません。
//失敗時は常に0を返します。

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

