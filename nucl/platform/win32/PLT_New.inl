

#include "PLT_EX_StackTrace.h"

#include "nucl/Types.h"
#include "nucl/Trace.h"

static nc::uint32_t newCount = 0;

void* operator new(size_t size)
{
	void* addr = nc::memory::default_memory_allocator::malloc( size );

//	NC_TRACE( "New[%d], size=%d, addr=0x%08x", newCount++, size, addr );

//	StackTrace stt;
//	stt.Show();

	return addr;
}

void operator delete(void* ptr)
{
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

	void* addr = nc::memory::default_memory_allocator::malloc(iSize);

//	NC_TRACE( "New[%d], size=%d, addr=0x%08x", newCount++, iSize, addr );

//	StackTrace stt;
//	stt.Show();

	return addr;
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
