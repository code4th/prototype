
#include "PLT_Mutex.h"

nc::platform::thread::Mutex::Mutex()
{
	InitializeCriticalSection( &mutexObj_ );
}

nc::platform::thread::Mutex::~Mutex()
{
	DeleteCriticalSection( &mutexObj_ );
}

bool nc::platform::thread::Mutex::try_lock()
{
	return TryEnterCriticalSection( &mutexObj_ ) ? true : false;
}

void nc::platform::thread::Mutex::lock()
{
	EnterCriticalSection( &mutexObj_ );
}

void nc::platform::thread::Mutex::unlock()
{
	LeaveCriticalSection( &mutexObj_ );
}


