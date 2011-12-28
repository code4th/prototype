
#include "New.h"
#include "MemoryAllocator.h"

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_NEW

#include "PLT_New.inl"

#else
/*
void* operator new(size_t size) {
	return nc::memory::default_memory_allocator::malloc( size );
}

void operator delete(void* ptr) {
	nc::memory::default_memory_allocator::free(ptr);
}

void* operator new(size_t size, const std::nothrow_t&)
{
	return operator new( size );
}

void operator delete(void* pv, const std::nothrow_t&)
{
	operator delete( pv );
}

void* operator new(size_t size, void* pv) {
	return pv;
}
void operator delete(void* pv, void*) {
}

void* operator new[](size_t iSize) {
	return nc::memory::default_memory_allocator::malloc(iSize);
}
void operator delete[](void* pv) {
	nc::memory::default_memory_allocator::free(pv);
}

void* operator new[](size_t size, const std::nothrow_t&)
{
	return operator new[]( size );
}

void operator delete[](void* pv, const std::nothrow_t&)
{
	operator delete[]( pv );
}
*/
#endif //NC_PLATFORM_USE_INTRINSIC_NEW

